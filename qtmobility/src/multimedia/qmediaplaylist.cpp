/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qmediaplaylist.h"
#include "qmediaplaylist_p.h"
#include "qmediaplaylistprovider.h"
#include "qlocalmediaplaylistprovider.h"
#include "qmediaplaylistioplugin.h"
#include "qmediaservice.h"
#include "qmediaplaylistcontrol.h"
#include "qmediaplayercontrol.h"

#include <QtCore/qlist.h>
#include <QtCore/qfile.h>
#include <QtCore/qurl.h>
#include <QtCore/qcoreevent.h>
#include <QtCore/qcoreapplication.h>

#include "qmediapluginloader_p.h"

QTM_BEGIN_NAMESPACE

Q_GLOBAL_STATIC_WITH_ARGS(QMediaPluginLoader, playlistIOLoader,
        (QMediaPlaylistIOInterface_iid, QLatin1String("/playlistformats"), Qt::CaseInsensitive))


/*!
    \class QMediaPlaylist
    \ingroup multimedia

    \preliminary
    \brief The QMediaPlaylist class provides a list of media content to play.

    QMediaPlaylist is intended to be used with other media objects,
    like QMediaPlayer or QMediaImageViewer.
    QMediaPlaylist allows to access the service intrinsic playlist functionality
    if available, otherwise it provides the the local memory playlist implementation.

\code
    player = new QMediaPlayer;

    playlist = new QMediaPlaylist;
    playlist->setMediaObject(player);
    playlist->append(QUrl("http://example.com/movie1.mp4"));
    playlist->append(QUrl("http://example.com/movie2.mp4"));
    playlist->append(QUrl("http://example.com/movie3.mp4"));

    playlist->setCurrentIndex(1);

    player->play();
\endcode

    Depending on playlist source implementation,
    most of playlist modifcation operations can be asynchronous.

    \sa QMediaContent
*/


/*!
    \enum QMediaPlaylist::PlaybackMode

    The QMediaPlaylist::PlaybackMode describes the order items in playlist are played.

    \value CurrentItemOnce    The current item is played only once.

    \value CurrentItemInLoop  The current item is played in the loop.

    \value Linear             Playback starts from the first to the last items and stops.
                              next item is a null item when the last one is currently playing.

    \value Loop               Playback continues from the first item after the last one finished playing.

    \value Random             Play items in random order.
*/



/*!
  Create a new playlist object for with the given \a parent.  
*/

QMediaPlaylist::QMediaPlaylist(QObject *parent)
    : QObject(parent)
    , d_ptr(new QMediaPlaylistPrivate)
{
    Q_D(QMediaPlaylist);

    d->q_ptr = this;
    d->localPlaylistControl = new QLocalMediaPlaylistControl(this);

    setMediaObject(0);
}

/*!
  Destroys the playlist.
  */

QMediaPlaylist::~QMediaPlaylist()
{
    Q_D(QMediaPlaylist);

    if (d->mediaObject)
        d->mediaObject->unbind(this);

    delete d_ptr;
}

QMediaObject *QMediaPlaylist::mediaObject() const
{
    return d_func()->mediaObject;
}

/*!
  If \a mediaObject is null or doesn't have an intrinsic playlist,
  internal local memory playlist source will be created.
*/
void QMediaPlaylist::setMediaObject(QMediaObject *mediaObject)
{
    Q_D(QMediaPlaylist);

    if (mediaObject && mediaObject == d->mediaObject)
        return;

    QMediaService *service = mediaObject
            ? mediaObject->service() : 0;

    QMediaPlaylistControl *newControl = 0;

    if (service)
        newControl = qobject_cast<QMediaPlaylistControl*>(service->control(QMediaPlaylistControl_iid));

    if (!newControl)
        newControl = d->localPlaylistControl;

    if (d->control != newControl) {
        int oldSize = 0;
        if (d->control) {
            QMediaPlaylistProvider *playlist = d->control->playlistProvider();
            oldSize = playlist->mediaCount();
            disconnect(playlist, SIGNAL(loadFailed(QMediaPlaylist::Error,QString)),
                    this, SLOT(_q_loadFailed(QMediaPlaylist::Error,QString)));

            disconnect(playlist, SIGNAL(mediaChanged(int,int)), this, SIGNAL(mediaChanged(int,int)));
            disconnect(playlist, SIGNAL(mediaAboutToBeInserted(int,int)), this, SIGNAL(mediaAboutToBeInserted(int,int)));
            disconnect(playlist, SIGNAL(mediaInserted(int,int)), this, SIGNAL(mediaInserted(int,int)));
            disconnect(playlist, SIGNAL(mediaAboutToBeRemoved(int,int)), this, SIGNAL(mediaAboutToBeRemoved(int,int)));
            disconnect(playlist, SIGNAL(mediaRemoved(int,int)), this, SIGNAL(mediaRemoved(int,int)));

            disconnect(playlist, SIGNAL(loaded()), this, SIGNAL(loaded()));

            disconnect(d->control, SIGNAL(playbackModeChanged(QMediaPlaylist::PlaybackMode)),
                    this, SIGNAL(playbackModeChanged(QMediaPlaylist::PlaybackMode)));
            disconnect(d->control, SIGNAL(currentIndexChanged(int)),
                    this, SIGNAL(currentIndexChanged(int)));
            disconnect(d->control, SIGNAL(currentMediaChanged(QMediaContent)),
                    this, SIGNAL(currentMediaChanged(QMediaContent)));
        }

        d->control = newControl;
        QMediaPlaylistProvider *playlist = d->control->playlistProvider();
        connect(playlist, SIGNAL(loadFailed(QMediaPlaylist::Error,QString)),
                this, SLOT(_q_loadFailed(QMediaPlaylist::Error,QString)));

        connect(playlist, SIGNAL(mediaChanged(int,int)), this, SIGNAL(mediaChanged(int,int)));
        connect(playlist, SIGNAL(mediaAboutToBeInserted(int,int)), this, SIGNAL(mediaAboutToBeInserted(int,int)));
        connect(playlist, SIGNAL(mediaInserted(int,int)), this, SIGNAL(mediaInserted(int,int)));
        connect(playlist, SIGNAL(mediaAboutToBeRemoved(int,int)), this, SIGNAL(mediaAboutToBeRemoved(int,int)));
        connect(playlist, SIGNAL(mediaRemoved(int,int)), this, SIGNAL(mediaRemoved(int,int)));

        connect(playlist, SIGNAL(loaded()), this, SIGNAL(loaded()));

        connect(d->control, SIGNAL(playbackModeChanged(QMediaPlaylist::PlaybackMode)),
                this, SIGNAL(playbackModeChanged(QMediaPlaylist::PlaybackMode)));
        connect(d->control, SIGNAL(currentIndexChanged(int)),
                this, SIGNAL(currentIndexChanged(int)));
        connect(d->control, SIGNAL(currentMediaChanged(QMediaContent)),
                this, SIGNAL(currentMediaChanged(QMediaContent)));

        if (oldSize)
            emit mediaRemoved(0, oldSize-1);

        if (playlist->mediaCount()) {
            emit mediaAboutToBeInserted(0,playlist->mediaCount()-1);
            emit mediaInserted(0,playlist->mediaCount()-1);
        }
    }

    if (d->mediaObject)
        d->mediaObject->unbind(this);

    d->mediaObject = mediaObject;
    if (d->mediaObject)
        d->mediaObject->bind(this);
}

/*!
  \property QMediaPlaylist::playbackMode

  This property defines the order, items in playlist are played.

  \sa QMediaPlaylist::PlaybackMode
*/

QMediaPlaylist::PlaybackMode QMediaPlaylist::playbackMode() const
{
    return d_func()->control->playbackMode();
}

void QMediaPlaylist::setPlaybackMode(QMediaPlaylist::PlaybackMode mode)
{
    Q_D(QMediaPlaylist);
    d->control->setPlaybackMode(mode);
}

/*!
  Returns position of the current media source in the playlist.
*/
int QMediaPlaylist::currentIndex() const
{
    return d_func()->control->currentIndex();
}

/*!
  Returns the current media content.
*/

QMediaContent QMediaPlaylist::currentMedia() const
{
    return d_func()->playlist()->media(currentIndex());
}

/*!
  Returns the index of item, which were current after calling next()
  \a steps times.

  Returned value depends on the size of playlist, current position
  and playback mode.

  \sa QMediaPlaylist::playbackMode
*/
int QMediaPlaylist::nextIndex(int steps) const
{
    return d_func()->control->nextIndex(steps);
}

/*!
  Returns the index of item, which were current after calling previous()
  \a steps times.

  \sa QMediaPlaylist::playbackMode
*/

int QMediaPlaylist::previousIndex(int steps) const
{
    return d_func()->control->previousIndex(steps);
}


/*!
  Returns the number of items in the playlist.

  \sa isEmpty()
  */
int QMediaPlaylist::mediaCount() const
{
    return d_func()->playlist()->mediaCount();
}

/*!
  Returns true if the playlist contains no items; otherwise returns false.
  \sa size()
  */
bool QMediaPlaylist::isEmpty() const
{
    return mediaCount() == 0;
}

/*!
  Returns true if the playlist can be modified; otherwise returns false.
  \sa size()
  */
bool QMediaPlaylist::isReadOnly() const
{
    return d_func()->playlist()->isReadOnly();
}

/*!
  Returns the media content at \a index in the playlist.
*/

QMediaContent QMediaPlaylist::media(int index) const
{
    return d_func()->playlist()->media(index);
}

/*!
  Append the media \a content to the playlist.

  Returns true if the operation is successfull, other wise return false.
  */
bool QMediaPlaylist::addMedia(const QMediaContent &content)
{
    return d_func()->control->playlistProvider()->addMedia(content);
}

/*!
  Append multiple media content \a items to the playlist.

  Returns true if the operation is successfull, other wise return false.
  */
bool QMediaPlaylist::addMedia(const QList<QMediaContent> &items)
{
    return d_func()->control->playlistProvider()->addMedia(items);
}

/*!
  Insert the media \a content to the playlist at position \a pos.

  Returns true if the operation is successful, otherwise false.
*/

bool QMediaPlaylist::insertMedia(int pos, const QMediaContent &content)
{
    return d_func()->playlist()->insertMedia(pos, content);
}

/*!
  Insert multiple media content \a items to the playlist at position \a pos.

  Returns true if the operation is successful, otherwise false.
*/

bool QMediaPlaylist::insertMedia(int pos, const QList<QMediaContent> &items)
{
    return d_func()->playlist()->insertMedia(pos, items);
}

/*!
  Remove the item from the playlist at position \a pos.

  Returns true if the operation is successfull, other wise return false.
  */
bool QMediaPlaylist::removeMedia(int pos)
{
    Q_D(QMediaPlaylist);
    return d->playlist()->removeMedia(pos);
}

/*!
  Remove the items from the playlist from position \a start to \a end inclusive.

  Returns true if the operation is successfull, other wise return false.
  */
bool QMediaPlaylist::removeMedia(int start, int end)
{
    Q_D(QMediaPlaylist);
    return d->playlist()->removeMedia(start, end);
}

/*!
  Remove all the items from the playlist.

  Returns true if the operation is successfull, other wise return false.
  */
bool QMediaPlaylist::clear()
{
    Q_D(QMediaPlaylist);
    return d->playlist()->clear();
}

bool QMediaPlaylistPrivate::readItems(QMediaPlaylistReader *reader)
{
    while (!reader->atEnd())
        playlist()->addMedia(reader->readItem());

    return true;
}

bool QMediaPlaylistPrivate::writeItems(QMediaPlaylistWriter *writer)
{
    for (int i=0; i<playlist()->mediaCount(); i++) {
        if (!writer->writeItem(playlist()->media(i)))
            return false;
    }
    writer->close();
    return true;
}

/*!
  Load playlist from \a location. If \a format is specified, it is used,
  otherwise format is guessed from location name and data.

  New items are appended to playlist.

  QMediaPlaylist::loaded() signal is emited if playlist was loaded succesfully,
  otherwise the playlist emits loadFailed().
*/
void QMediaPlaylist::load(const QUrl &location, const char *format)
{
    Q_D(QMediaPlaylist);

    d->error = NoError;
    d->errorString.clear();

    if (d->playlist()->load(location,format))
        return;

    if (isReadOnly()) {
        d->error = AccessDeniedError;
        d->errorString = tr("Could not add items to read only playlist.");
        emit loadFailed();
        return;
    }

    foreach (QString const& key, playlistIOLoader()->keys()) {
        QMediaPlaylistIOInterface* plugin = qobject_cast<QMediaPlaylistIOInterface*>(playlistIOLoader()->instance(key));
        if (plugin && plugin->canRead(location,format)) {
            QMediaPlaylistReader *reader = plugin->createReader(location,QByteArray(format));
            if (reader && d->readItems(reader)) {
                delete reader;
                emit loaded();
                return;
            }
            delete reader;
        }
    }

    d->error = FormatNotSupportedError;
    d->errorString = tr("Playlist format is not supported");
    emit loadFailed();

    return;
}

/*!
  Load playlist from QIODevice \a device. If \a format is specified, it is used,
  otherwise format is guessed from device data.

  New items are appended to playlist.

  QMediaPlaylist::loaded() signal is emited if playlist was loaded succesfully,
  otherwise the playlist emits loadFailed().
*/
void QMediaPlaylist::load(QIODevice * device, const char *format)
{
    Q_D(QMediaPlaylist);

    d->error = NoError;
    d->errorString.clear();

    if (d->playlist()->load(device,format))
        return;

    if (isReadOnly()) {
        d->error = AccessDeniedError;
        d->errorString = tr("Could not add items to read only playlist.");
        emit loadFailed();
        return;
    }

    foreach (QString const& key, playlistIOLoader()->keys()) {
        QMediaPlaylistIOInterface* plugin = qobject_cast<QMediaPlaylistIOInterface*>(playlistIOLoader()->instance(key));
        if (plugin && plugin->canRead(device,format)) {
            QMediaPlaylistReader *reader = plugin->createReader(device,QByteArray(format));
            if (reader && d->readItems(reader)) {
                delete reader;
                emit loaded();
                return;
            }
            delete reader;
        }
    }

    d->error = FormatNotSupportedError;
    d->errorString = tr("Playlist format is not supported");
    emit loadFailed();

    return;
}

/*!
  Save playlist to \a location. If \a format is specified, it is used,
  otherwise format is guessed from location name.

  Returns true if playlist was saved succesfully, otherwise returns false.
  */
bool QMediaPlaylist::save(const QUrl &location, const char *format)
{
    Q_D(QMediaPlaylist);

    d->error = NoError;
    d->errorString.clear();

    if (d->playlist()->save(location,format))
        return true;

    QFile file(location.toLocalFile());

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        d->error = AccessDeniedError;
        d->errorString = tr("The file could not be accessed.");
        return false;
    }

    return save(&file, format);
}

/*!
  Save playlist to QIODevice \a device using format \a format.

  Returns true if playlist was saved succesfully, otherwise returns false.
*/
bool QMediaPlaylist::save(QIODevice * device, const char *format)
{
    Q_D(QMediaPlaylist);

    d->error = NoError;
    d->errorString.clear();

    if (d->playlist()->save(device,format))
        return true;

    foreach (QString const& key, playlistIOLoader()->keys()) {
        QMediaPlaylistIOInterface* plugin = qobject_cast<QMediaPlaylistIOInterface*>(playlistIOLoader()->instance(key));
        if (plugin && plugin->canWrite(device,format)) {
            QMediaPlaylistWriter *writer = plugin->createWriter(device,QByteArray(format));
            if (writer && d->writeItems(writer)) {
                delete writer;
                return true;
            }
            delete writer;
        }
    }

    d->error = FormatNotSupportedError;
    d->errorString = tr("Playlist format is not supported.");

    return false;
}

/*!
    Returns the last error condition.
*/
QMediaPlaylist::Error QMediaPlaylist::error() const
{
    return d_func()->error;
}

/*!
    Returns the string describing the last error condition.
*/
QString QMediaPlaylist::errorString() const
{
    return d_func()->errorString;
}

/*!
  Shuffle items in the playlist.
*/
void QMediaPlaylist::shuffle()
{
    d_func()->playlist()->shuffle();
}


/*!
    Advance to the next media content in playlist.
*/
void QMediaPlaylist::next()
{
    d_func()->control->next();
}

/*!
    Return to the previous media content in playlist.
*/
void QMediaPlaylist::previous()
{
    d_func()->control->previous();
}

/*!
    Activate media content from playlist at position \a playlistPosition.
*/

void QMediaPlaylist::setCurrentIndex(int playlistPosition)
{
    d_func()->control->setCurrentIndex(playlistPosition);
}

/*!
    \fn void QMediaPlaylist::mediaInserted(int start, int end)

    This signal is emitted after media has been inserted into the playlist.
    The new items are those between \a start and \a end inclusive.
 */

/*!
    \fn void QMediaPlaylist::mediaRemoved(int start, int end)

    This signal is emitted after media has been removed from the playlist.
    The removed items are those between \a start and \a end inclusive.
 */

/*!
    \fn void QMediaPlaylist::mediaChanged(int start, int end)

    This signal is emitted after media has been changed in the playlist
    between \a start and \a end positions inclusive.
 */

/*!
    \fn void QMediaPlaylist::currentIndexChanged(int position)

    Signal emitted when playlist position changed to \a position.
*/

/*!
    \fn void QMediaPlaylist::playbackModeChanged(QMediaPlaylist::PlaybackMode mode)

    Signal emitted when playback mode changed to \a mode.
*/

/*!
    \fn void QMediaPlaylist::mediaAboutToBeInserted(int start, int end)

    Signal emitted when item to be inserted at \a start and ending at \a end.
*/

/*!
    \fn void QMediaPlaylist::mediaAboutToBeRemoved(int start, int end)

    Signal emitted when item to de deleted ar \a start and ending at \a end.
*/

/*!
    \fn void QMediaPlaylist::currentMediaChanged(const QMediaContent &content)

    Signal emitted when current media changes to \a content.
*/

/*!
    \property QMediaPlaylist::currentIndex
    \brief Current position.
*/

/*!
    \property QMediaPlaylist::currentMedia
    \brief Current media content.
*/

/*!
    \fn QMediaPlaylist::loaded()

    Signal emitted when playlist finished loading.
*/

/*!
    \fn QMediaPlaylist::loadFailed()

    Signal emitted if failed to load playlist.
*/

/*!
    \enum QMediaPlaylist::Error

    This enum describes the QMediaPlaylist error codes.

    \value NoError                 No errors.
    \value FormatError             Format error.
    \value FormatNotSupportedError Format not supported.
    \value NetworkError            Network error.
    \value AccessDeniedError       Access denied error.
*/

#include "moc_qmediaplaylist.cpp"
#include "moc_qmediaplaylist_p.cpp"
QTM_END_NAMESPACE

