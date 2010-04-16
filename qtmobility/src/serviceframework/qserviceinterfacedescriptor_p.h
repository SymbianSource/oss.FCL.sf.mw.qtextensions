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

#ifndef QSERVICEINTERFACEDESCRIPTOR_P_H
#define QSERVICEINTERFACEDESCRIPTOR_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qserviceinterfacedescriptor.h"

#include <QString>
#include <QHash>

QTM_BEGIN_NAMESPACE

class QServiceInterfaceDescriptorPrivate
{
public:
    QServiceInterfaceDescriptorPrivate()
    {
        major = -1;
        minor = -1;
        scope = QService::UserScope;
    }

    bool operator==(const QServiceInterfaceDescriptorPrivate& other) const
    {
        if (major == other.major 
                && minor == other.minor
                && interfaceName == other.interfaceName
                && serviceName == other.serviceName
                && attributes == other.attributes 
                && customAttributes == other.customAttributes
                && scope == other.scope)
            return true;
        return false;
    }

    QServiceInterfaceDescriptorPrivate& operator=(const QServiceInterfaceDescriptorPrivate& other)
    {
        serviceName = other.serviceName;
        interfaceName = other.interfaceName;
        minor = other.minor;
        major = other.major;
        attributes = other.attributes;
        customAttributes = other.customAttributes;
        scope = other.scope;

        return *this;
    }

    static QServiceInterfaceDescriptorPrivate *getPrivate(QServiceInterfaceDescriptor *descriptor)
    {
        return descriptor->d;
    }

    static const QServiceInterfaceDescriptorPrivate *getPrivate(const QServiceInterfaceDescriptor *descriptor)
    {
        return descriptor->d;
    }

    static void setPrivate(QServiceInterfaceDescriptor *descriptor, QServiceInterfaceDescriptorPrivate *p)
    {
        descriptor->d = p;
    }

    QString serviceName;
    QString interfaceName;
    QHash<QServiceInterfaceDescriptor::Attribute, QVariant> attributes;
    QHash<QString, QString> customAttributes;
    int major;
    int minor;
    QService::Scope scope;
};
QTM_END_NAMESPACE

#endif //QSERVICEINTERFACEDESCRIPTOR_P_H
