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

#ifndef QMEDIASERVICEPROVIDERPLUGIN_H
#define QMEDIASERVICEPROVIDERPLUGIN_H

#include <QtCore/qstringlist.h>
#include <QtCore/qplugin.h>
#include <QtCore/qfactoryinterface.h>
#include <qmobilityglobal.h>
#include <qmediaserviceprovider.h>

#ifdef Q_MOC_RUN
# pragma Q_MOC_EXPAND_MACROS
#endif

QTM_BEGIN_NAMESPACE

class QMediaService;

struct Q_MEDIA_EXPORT QMediaServiceProviderFactoryInterface : public QFactoryInterface
{
    virtual QStringList keys() const = 0;
    virtual QMediaService* create(QString const& key) = 0;
    virtual void release(QMediaService *service) = 0;
};

QTM_END_NAMESPACE

#define QMediaServiceProviderFactoryInterface_iid \
    "com.nokia.Qt.QMediaServiceProviderFactoryInterface/1.0"
QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(QtMobility::QMediaServiceProviderFactoryInterface, QMediaServiceProviderFactoryInterface_iid)
QT_END_NAMESPACE

QTM_BEGIN_NAMESPACE

struct Q_MEDIA_EXPORT QMediaServiceSupportedFormatsInterface
{
    virtual ~QMediaServiceSupportedFormatsInterface() {}
    virtual QtMedia::SupportEstimate hasSupport(const QString &mimeType, const QStringList& codecs) const = 0;
    virtual QStringList supportedMimeTypes() const = 0;
};

QTM_END_NAMESPACE

#define QMediaServiceSupportedFormatsInterface_iid \
    "com.nokia.Qt.QMediaServiceSupportedFormatsInterface/1.0"
QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(QtMobility::QMediaServiceSupportedFormatsInterface, QMediaServiceSupportedFormatsInterface_iid)
QT_END_NAMESPACE

QTM_BEGIN_NAMESPACE

struct Q_MEDIA_EXPORT QMediaServiceSupportedDevicesInterface
{
    virtual ~QMediaServiceSupportedDevicesInterface() {}
    virtual QList<QByteArray> devices(const QByteArray &service) const = 0;
    virtual QString deviceDescription(const QByteArray &service, const QByteArray &device) = 0;
};

QTM_END_NAMESPACE

#define QMediaServiceSupportedDevicesInterface_iid \
    "com.nokia.Qt.QMediaServiceSupportedDevicesInterface/1.0"
QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(QtMobility::QMediaServiceSupportedDevicesInterface, QMediaServiceSupportedDevicesInterface_iid)
QT_END_NAMESPACE

QTM_BEGIN_NAMESPACE

struct Q_MEDIA_EXPORT QMediaServiceFeaturesInterface
{
    virtual ~QMediaServiceFeaturesInterface() {}
    virtual QMediaServiceProviderHint::Features supportedFeatures(const QByteArray &service) const = 0;
};

QTM_END_NAMESPACE

//MOC doesn't recognire paramitized macros
#define QMediaServiceFeaturesInterface_iid \
    "com.nokia.Qt.QMediaServiceFeaturesInterface/1.0"
QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(QtMobility::QMediaServiceFeaturesInterface, QMediaServiceFeaturesInterface_iid)
QT_END_NAMESPACE

QTM_BEGIN_NAMESPACE

class Q_MEDIA_EXPORT QMediaServiceProviderPlugin : public QObject, public QMediaServiceProviderFactoryInterface
{
    Q_OBJECT
    Q_INTERFACES(QtMobility::QMediaServiceProviderFactoryInterface:QFactoryInterface)

public:
    virtual QStringList keys() const = 0;
    virtual QMediaService* create(const QString& key) = 0;
    virtual void release(QMediaService *service) = 0;
};

QTM_END_NAMESPACE


#endif  // QMEDIASERVICEPROVIDERPLUGIN_H
