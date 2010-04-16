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

#ifndef QT7PLAYERSERVICE_H
#define QT7PLAYERSERVICE_H

#include <QtCore/qobject.h>
#include <qmediaservice.h>


QTM_BEGIN_NAMESPACE
class QMediaMetaData;
class QMediaPlayerControl;
class QMediaPlaylist;
class QMediaPlaylistNavigator;
class QT7PlayerControl;
class QT7PlayerMetaDataControl;
class QT7VideoOutputControl;
class QT7VideoWindowControl;
class QT7VideoWidgetControl;
class QT7VideoRendererControl;
class QT7VideoOutput;
class QT7PlayerSession;

class QT7PlayerService : public QMediaService
{
Q_OBJECT
public:
    QT7PlayerService(QObject *parent = 0);
    ~QT7PlayerService();

    QMediaControl *control(const char *name) const;

private slots:
    void updateVideoOutput();

private:
    QT7PlayerSession *m_session;
    QT7PlayerControl *m_control;    
    QT7VideoOutputControl *m_videoOutputControl;
    QT7VideoWindowControl *m_videoWidnowControl;
    QT7VideoWidgetControl *m_videoWidgetControl;
    QT7VideoRendererControl *m_videoRendererControl;
    QT7PlayerMetaDataControl *m_playerMetaDataControl;
};

QTM_END_NAMESPACE

#endif
