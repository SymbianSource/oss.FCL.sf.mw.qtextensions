/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef QAUDIOSYSTEMPLUGIN_H
#define QAUDIOSYSTEMPLUGIN_H

#include <QtCore/qstring.h>
#include <QtCore/qplugin.h>
#include <QtCore/qfactoryinterface.h>

#include <qmobilityglobal.h>
#include <qtmedianamespace.h>

#include "qaudioformat.h"
#include "qaudiodeviceinfo.h"
#include "qaudiosystem.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

struct Q_MULTIMEDIA_EXPORT QAudioSystemFactoryInterface : public QFactoryInterface
{
    virtual QList<QByteArray> availableDevices(QAudio::Mode) const = 0;
    virtual QAbstractAudioInput* createInput(const QByteArray& device) = 0;
    virtual QAbstractAudioOutput* createOutput(const QByteArray& device) = 0;
    virtual QAbstractAudioDeviceInfo* createDeviceInfo(const QByteArray& device, QAudio::Mode mode) = 0;
};

#define QAudioSystemFactoryInterface_iid \
    "com.nokia.qt.QAudioSystemFactoryInterface"
Q_DECLARE_INTERFACE(QAudioSystemFactoryInterface, QAudioSystemFactoryInterface_iid)

class Q_MULTIMEDIA_EXPORT QAudioSystemPlugin : public QObject, public QAudioSystemFactoryInterface
{
    Q_OBJECT
    Q_INTERFACES(QAudioSystemFactoryInterface:QFactoryInterface)

public:
    QAudioSystemPlugin(QObject *parent = 0);
    ~QAudioSystemPlugin();

    virtual QStringList keys() const = 0;
    virtual QList<QByteArray> availableDevices(QAudio::Mode) const = 0;
    virtual QAbstractAudioInput* createInput(const QByteArray& device) = 0;
    virtual QAbstractAudioOutput* createOutput(const QByteArray& device) = 0;
    virtual QAbstractAudioDeviceInfo* createDeviceInfo(const QByteArray& device, QAudio::Mode mode) = 0;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QAUDIOSYSTEMPLUGIN_H
