/****************************************************************************
**
** Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in Technology Preview License Agreement accompanying
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
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** If you have questions regarding the use of this file, please
** contact Nokia at http://qt.nokia.com/contact.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef XQSAIWINTERFACEDESCRIPTOR_P_H
#define XQSAIWINTERFACEDESCRIPTOR_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of the QLibrary class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "xqaiwinterfacedescriptor.h"

#include <QString>
#include <QHash>

QT_BEGIN_NAMESPACE

class XQAiwInterfaceDescriptorPrivate
{
public:
    XQAiwInterfaceDescriptorPrivate()
    {
        major = -1;
        minor = -1;
        systemScope = false;
    }

    bool operator==(const XQAiwInterfaceDescriptorPrivate& other) const
    {
        if (major == other.major 
                && minor == other.minor
                && interfaceName == other.interfaceName
                && serviceName == other.serviceName
                && properties == other.properties 
                && customProperties == other.customProperties
                && systemScope == other.systemScope)
            return true;
        return false;
    }

    XQAiwInterfaceDescriptorPrivate& operator=(const XQAiwInterfaceDescriptorPrivate& other)
    {
        serviceName = other.serviceName;
        interfaceName = other.interfaceName;
        minor = other.minor;
        major = other.major;
        properties = other.properties;
        customProperties = other.customProperties;
        systemScope = other.systemScope;

        return *this;
    }

    static XQAiwInterfaceDescriptorPrivate *getPrivate(XQAiwInterfaceDescriptor *descriptor)
    {
        return descriptor->d;
    }

    static const XQAiwInterfaceDescriptorPrivate *getPrivate(const XQAiwInterfaceDescriptor *descriptor)
    {
        return descriptor->d;
    }

    static void setPrivate(XQAiwInterfaceDescriptor *descriptor, XQAiwInterfaceDescriptorPrivate *p)
    {
        descriptor->d = p;
    }

    QString serviceName;
    QString interfaceName;
    QHash<XQAiwInterfaceDescriptor::PropertyKey, QVariant> properties;
    QHash<QString, QString> customProperties;
    int major;
    int minor;
    bool systemScope;
};
QT_END_NAMESPACE

#endif //XQSAIWINTERFACEDESCRIPTOR_P_H
