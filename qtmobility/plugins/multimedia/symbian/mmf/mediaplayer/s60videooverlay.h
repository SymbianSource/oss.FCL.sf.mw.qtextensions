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

#ifndef S60VIDEOOVERLAY_H
#define S60VIDEOOVERLAY_H

#include <QtCore/qobject.h>
#include <qvideowindowcontrol.h>

QT_USE_NAMESPACE

class QAbstractVideoSurface;
class S60VideoSurface;

class S60VideoOverlay : public QVideoWindowControl
{
    Q_OBJECT

public:
    S60VideoOverlay(QObject *parent = 0);
    ~S60VideoOverlay();

    WId winId() const;
    void setWinId(WId id);

    QRect displayRect() const;
    void setDisplayRect(const QRect &rect);

    bool isFullScreen() const;
    void setFullScreen(bool fullScreen);

    QSize nativeSize() const;

    Qt::AspectRatioMode aspectRatioMode() const;
    void setAspectRatioMode(Qt::AspectRatioMode mode);

    QSize customAspectRatio() const;
    void setCustomAspectRatio(const QSize &customRatio);

    void repaint();

    int brightness() const;
    void setBrightness(int brightness);

    int contrast() const;
    void setContrast(int contrast);

    int hue() const;
    void setHue(int hue);

    int saturation() const;
    void setSaturation(int saturation);

    QAbstractVideoSurface *surface() const;

private slots:
    void surfaceFormatChanged();

private:
    void setScaledDisplayRect();

    S60VideoSurface *m_surface;
    Qt::AspectRatioMode m_aspectRatioMode;
    QRect m_displayRect;
    QSize m_aspectRatio;
    bool m_fullScreen;
};

#endif // S60VIDEOOVERLAY_H
