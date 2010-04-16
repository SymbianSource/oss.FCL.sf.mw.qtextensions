/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, version 2.1 of the License.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, 
* see "http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html/".
*
* Description:
*
*/

#ifndef XQAPPLICATIONMANAGER_H
#define XQAPPLICATIONMANAGER_H

#include<QObject>
#include<QString>
#include<QList>
#include<QUrl>
#include<QFile>

#include <xqaiwglobal.h>
#include <xqaiwinterfacedescriptor.h>
#include <xqaiwrequest.h>
#include <xqsharablefile.h>

class XQApplicationManagerPrivate;

class XQAIW_EXPORT XQApplicationManager : public QObject
{
    Q_OBJECT

public:
            
    XQApplicationManager();
    virtual ~XQApplicationManager();

public:
            
    XQAiwRequest* create( const QString& interface, const QString& operation, bool embedded = true);

    XQAiwRequest* create( const XQAiwInterfaceDescriptor& implementation, const QString& operation, bool embedded = true);

    XQAiwRequest* create( const QString& service, const QString& interface, const QString& operation, bool embedded = true);

    QList<XQAiwInterfaceDescriptor> list(const QString& interface, const QString& operation);
    QList<XQAiwInterfaceDescriptor> list(const QString& service, const QString& interface, const QString& operation);

    XQAiwRequest* create( const QUrl &url, bool embedded = true);
    XQAiwRequest* create( const QUrl &url, const XQAiwInterfaceDescriptor& implementation, bool embedded = true);
    XQAiwRequest* create( const QFile &file, bool embedded = true);
    XQAiwRequest* create( const QFile &file, const XQAiwInterfaceDescriptor& implementation, bool embedded = true);
    
    QList<XQAiwInterfaceDescriptor> list(const QUrl& uri);
    QList<XQAiwInterfaceDescriptor> list(const QFile& file);
    
    QList<XQAiwInterfaceDescriptor> list(const XQSharableFile& file);
    XQAiwRequest* create( const XQSharableFile &file, bool embedded = true);
    XQAiwRequest* create( const XQSharableFile &file, const XQAiwInterfaceDescriptor& implementation, bool embedded = true);

    int lastError() const;

private:
    // Disable copy contructor
    Q_DISABLE_COPY(XQApplicationManager)
    XQApplicationManagerPrivate *d;
};

#endif
