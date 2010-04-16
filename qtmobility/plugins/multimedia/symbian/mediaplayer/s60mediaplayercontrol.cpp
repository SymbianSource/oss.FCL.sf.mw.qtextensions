
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

#include "s60mediaplayercontrol.h"
#include "s60mediaplayersession.h"

#include <QtCore/qdir.h>
#include <QtCore/qurl.h>
#include <QtCore/qdebug.h>

S60MediaPlayerControl::S60MediaPlayerControl(MS60MediaPlayerResolver& mediaPlayerResolver, QObject *parent)
    : QMediaPlayerControl(parent),
      m_mediaPlayerResolver(mediaPlayerResolver),
      m_session(NULL),
      m_stream(NULL)
{
}

S60MediaPlayerControl::~S60MediaPlayerControl()
{
}

qint64 S60MediaPlayerControl::position() const
{
    if (m_session)
        return m_session->position();
    return 0;
}

qint64 S60MediaPlayerControl::duration() const
{
    if (m_session)
        return m_session->duration();
    return -1;
}

QMediaPlayer::State S60MediaPlayerControl::state() const
{
    if (m_session)
        return m_session->state();
    return QMediaPlayer::StoppedState;
}

QMediaPlayer::MediaStatus S60MediaPlayerControl::mediaStatus() const
{   
    if (m_session)
        return m_session->mediaStatus();
    return QMediaPlayer::NoMedia;
}

int S60MediaPlayerControl::bufferStatus() const
{
    return -1;
}

int S60MediaPlayerControl::volume() const
{
    if (m_session)
        return m_session->volume();
    return m_mediaSettings.volume();
}

bool S60MediaPlayerControl::isMuted() const
{
   if (m_session)
       return  m_session->isMuted();
   return m_mediaSettings.isMuted();
}

bool S60MediaPlayerControl::isSeekable() const
{
    if (m_session)
       return  m_session->isSeekable();
    return false;
}

QMediaTimeRange S60MediaPlayerControl::availablePlaybackRanges() const
{
    QMediaTimeRange ranges;

    if(m_session && m_session->isSeekable())
        ranges.addInterval(0, m_session->duration());

    return ranges;
}

qreal S60MediaPlayerControl::playbackRate() const
{
    //None of symbian players supports this.
    return m_mediaSettings.playbackRate();
}

void S60MediaPlayerControl::setPlaybackRate(qreal rate)
{
    //None of symbian players supports this.
    m_mediaSettings.setPlaybackRate(rate);
    emit playbackRateChanged(playbackRate());
    
}

void S60MediaPlayerControl::setPosition(qint64 pos)
{
    if (m_session)
        m_session->setPosition(pos);
}

void S60MediaPlayerControl::play()
{
    if (m_session)
        m_session->play();
}

void S60MediaPlayerControl::pause()
{
    if (m_session)
        m_session->pause();
}

void S60MediaPlayerControl::stop()
{
    if (m_session)
        m_session->stop();
}

void S60MediaPlayerControl::setVolume(int volume)
{
    int boundVolume = qBound(0, volume, 100);
    if (boundVolume == m_mediaSettings.volume())
        return;
    
    m_mediaSettings.setVolume(boundVolume);
    if (m_session)
        m_session->setVolume(boundVolume);

    emit volumeChanged(boundVolume);
}

void S60MediaPlayerControl::setMuted(bool muted)
{
    if (m_mediaSettings.isMuted() == muted)
        return;
    
    m_mediaSettings.setMuted(muted);
    if (m_session)
        m_session->setMuted(muted);
    
    emit mutedChanged(muted);
}

QMediaContent S60MediaPlayerControl::media() const
{
    return m_currentResource;
}

const QIODevice *S60MediaPlayerControl::mediaStream() const
{
    return m_stream;
}

void S60MediaPlayerControl::setMedia(const QMediaContent &source, QIODevice *stream)
{
    Q_UNUSED(stream)
    // we don't want to set & load media again when it is already loaded    
    if (m_session && m_currentResource == source && m_session->state() == QMediaPlayer::LoadedMedia)
        return;

    if (source.isNull())
        return;
    
    // store to variable as session is created based on the content type.
    m_currentResource = source;
    if (m_session)
        m_session->stop();
    
    S60MediaPlayerSession *newSession = m_mediaPlayerResolver.PlayerSession(); 

    m_session = newSession;
    
    if (m_session) {
        QUrl url = source.canonicalUrl();
        
        if (m_session->mediaFileLocal())
            m_session->load(url);	
        else
            m_session->loadUrl(url);
            
        emit mediaChanged(m_currentResource);
    } else {
        emit error(QMediaPlayer::FormatError, QString("Symbian: -5"));
        emit mediaStatusChanged(QMediaPlayer::InvalidMedia);
    }
}

void S60MediaPlayerControl::setVideoOutput(QObject *output)
{
    if (!m_session)
        m_session = m_mediaPlayerResolver.VideoPlayerSession();
    m_session->setVideoRenderer(output);
}

bool S60MediaPlayerControl::isAudioAvailable() const
{
    if (m_session)
        return m_session->isAudioAvailable(); 
    return false;
}

bool S60MediaPlayerControl::isVideoAvailable() const
{
    if (m_session)
        return m_session->isVideoAvailable();
    return false;
}

const S60MediaSettings& S60MediaPlayerControl::mediaControlSettings() const
{
    return m_mediaSettings;
}
