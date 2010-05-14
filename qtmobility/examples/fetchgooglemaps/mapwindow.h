/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <qmobilityglobal.h>
#include <QMainWindow>

class QWebView;

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

QTM_BEGIN_NAMESPACE
class QGeoPositionInfo;
class QGeoPositionInfoSource;
#ifndef Q_WS_MAEMO_5
class QNetworkSession;
#endif
QTM_END_NAMESPACE

QTM_USE_NAMESPACE

class MapWindow : public QMainWindow
{
    Q_OBJECT
public:
    MapWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MapWindow();
    void start();

private slots:
    void delayedInit();
    void waitForFix();
    void positionUpdated(const QGeoPositionInfo &info);
    void loadStarted();
    void loadFinished(bool ok);

private:
    QWebView *webView;
    QLabel *posLabel;
    QLabel *headingAndSpeedLabel;
    QLabel *dateTimeLabel;
    bool loading;
#ifndef Q_WS_MAEMO_5
    QTM_PREPEND_NAMESPACE(QNetworkSession) *session;
#endif
    bool usingLogFile;
    QGeoPositionInfoSource *location;
    bool waitingForFix;
};

#endif