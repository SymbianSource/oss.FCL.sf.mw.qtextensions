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

#include "xqservicelog.h"
#include "xqappmgr.h"
#include <xqaiwrequest.h>
#include <xqaiwinterfacedescriptor.h>
#include <QList>
#include "xqappmgr_p.h"

XQApplicationManager::XQApplicationManager()
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::XQApplicationManager");
    d = new XQApplicationManagerPrivate();
    
}
XQApplicationManager::~XQApplicationManager()
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::~XQApplicationManager");
    delete d;
};

/*!
    Create AIW request for interface and operation name.
    The first found implementation is applied. If you need to activate specific implementation
    you shall first list() implementations and use the overloaded create() with XQAiwInterfaceDescriptor
    to select the correct implementation.
    
    \param interface Interface name as mentioned in the service registry file
    \param operation The function signature to be called via the interface.
                     Can be set later via XQAiwRequest::setOperation.
    \param embedded True if embedded call, false otherwise
                     Can be set later via XQAiwRequest::setEmbedded.
    \return The application interworking request instance, NULL if no service is available
    \see list()
    \see create( const QString &service, const QString &interface, const QString &operation, bool embedded)
    \see create( const XQAiwInterfaceDescriptor &implementation, const QString &operation, bool embedded);
    
    Example usage:
    \code
    // componentglobal.h
    const QString photosApplication = "Photos";
    const QString imageInterface = "com.nokia.symbian.IImageFetch";
    const QString selectOperation = "select(QString&,bool)";

    #include <xqappmgr.h>;
    #include "componentglobal.h";
    
    // XQApplicationManager mAppMgr;  // manager as class member
    
    XQAiwRequest * req = this->mAppMgr.create(imageInterface, selectOperation);
    if (req)
    {
        // There was service available
        QList<QVariant> args;
        args << folder;
        args << previewOn;
        req->setArguments(args);
        
        connect(req, SIGNAL(requestOk(const QVariant&)), this, SLOT(handleOk(const QVariant&)));
        connect(req, SIGNAL(requestError(int,const QString&)), this, SLOT(handleError(int,const QString&)));

        req->send();
    }
    \endcode
*/
XQAiwRequest* XQApplicationManager::create(
    const QString &interface, const QString &operation, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::create(interface)");
    return d->create(interface, operation, embedded);
}


/*!
    Create AIW request for interface implementation descriptor and operation name.
    The descriptor is got from the list() call.
    As combination [service,interface,version] shall be unique,
    the descriptor points to one implementation and thus selects correct
    implementation.
    
    \param implementation Valid interface descriptor obtained by the "list" call.
    \param operation The function signature to be called via the interface.
                     Can be set later via XQAiwRequest::setOperation.
    \param embedded True if embedded call, false otherwise
                     Can be set later via XQAiwRequest::setEmbedded.
    \return The application interworking request instance.
    \see list()
    \see create( const QString &interface, const QString &operation, bool embedded)
    \see create( const QString &service, const QString &interface, const QString &operation, bool embedded)

    Example usage:
    \code
    XQApplicationManager appMgr;  // Prefer one instance only 
    QList<XQAiwInterfaceDescriptor> list = appMgr.list(imageInterface, "");
    //
    // Populate a user interface widget and select proper implementation via descriptor
    // ... 
    XQAiwRequest * req = appMgr.create(selectedDescriptor, selectOperation);
    //
    // ... the rest as usual...
    //
    \endcode
*/
XQAiwRequest* XQApplicationManager::create(
    const XQAiwInterfaceDescriptor &implementation, const QString &operation, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::create (impl.)");
    return d->create(implementation, operation, embedded);
}


/*!
    Create AIW request for interface and service name.
    As combination [service,interface,version] shall be unique,
    the descriptor points to one implementation and thus selects correct
    implementation.
    
    \param service Service name as mentioned in the service registry file
    \param interface Interface name as mentioned in the service registry file
    \param operation The function signature to be called via the interface.
                     Can be set later via XQAiwRequest::setOperation.
    \param embedded True if embedded call, false otherwise
                     Can be set later via XQAiwRequest::setEmbedded.
    \return The application interworking request instance
    \see XQApplicationManager::create( const QString &interface, const QString &operation, bool embedded)
    \see create( const XQAiwInterfaceDescriptor &implementation, const QString &operation, bool embedded);

    Example usage:
    \code
    // componentglobal.h
    QString photosApplication = "Photos";
    QString imageInterface = "com.nokia.symbian.IImageFetch";
    QString selectOperation = "select(QString&,bool)";

    #include <xqappmgr.h>;
    #include "componentglobal.h";

    // XQApplicationManager mAppMgr;  // manager as class member

    XQAiwRequest * req = this->mAppMgr.create(photosApplication, imageInterface, selectOperation);
    if (req)
    {
        ...
    }
    \endcode
*/
XQAiwRequest* XQApplicationManager::create(
    const QString &service, const QString &interface, const QString &operation, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::create(service+interface)");
    return d->create(service, interface, operation, embedded);
}

/*!
    List implementation descriptors by interface name.
    \param interface Interface name as mentioned in the service registry file
    \param operation The operation signature to be called.  Reserved for future use.
    \param embedded True if embedded call, false otherwise
    \return The list of implementation descriptors.
    \see list

    Example usage:
    \code
    \code
    // Use some variant of "list"
    QString imageInterface = "com.nokia.symbian.IImageFetch";
    QList<XQAiwInterfaceDescriptor> list = this->mAppmgr.list(imageInterface, "");
    //
    // Populate a user interface widget and select proper implementation via descriptor
    // ... 
    XQAiwRequest * req = this->mAppMgr.create(selectedDescriptor, selectOperation);
    //
    // ... the rest as usual...
    //
    \endcode
*/
QList<XQAiwInterfaceDescriptor> XQApplicationManager::list(const QString &interface, const QString &operation)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::list");
    return d->list(interface, operation);
}

/*!
    List implementation(s) descriptors by service and interface name.
*/
QList<XQAiwInterfaceDescriptor> XQApplicationManager::list(
    const QString &service, const QString &interface, const QString &operation)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::list");
    return d->list(service, interface, operation);
}

/*!
    Create AIW request for uri.
    The interface name applied implicitly is declared by the constant XQI_URI_VIEW
    unless there is custom handler attached to URI scheme, the 
    A service declares support for scheme(s) (CSV list) by adding the custom property key
    (see the constant XQCUSTOM_PROP_SCHEMES value) to the service XML.
    By default, the operation name declared by constant XQOP_URI_VIEW is used.
    Certain URI schemes, like "application" or "http"  have custom handling and thus they override the service handling.
     - "appto"  URIs is handled by corresponding activity framework
     - "http:" and "https: URIs are handled by QDesktopServices::openUrl()
     - "file": URIs is handled via the XQI_FILE_VIEW interface.
       Example, QFile file("C:\\data\\Others\\test.txt");
         
    \return The application interworking request instance
    \see XQAiwInterfaceDescriptor for constants values
*/
XQAiwRequest* XQApplicationManager::create( const QUrl &uri, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::create(url)");
    return d->create(uri, NULL, embedded);
}

XQAiwRequest* XQApplicationManager::create(
    const QUrl &uri, const XQAiwInterfaceDescriptor& implementation, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::create(uri+impl)");
    return d->create(uri, &implementation, embedded);
}

/*!
    Create AIW request for the given file and the MIME type attached to the file.
    The interface name applied implicitly is declared by the constant XQI_FILE_VIEW
    By default, the operation name declared by constant XQOP_FILE_VIEW is used.
*/
XQAiwRequest* XQApplicationManager::create( const QFile &file, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::create(file)");
    return d->create(file, NULL, embedded);
}

XQAiwRequest* XQApplicationManager::create(
    const QFile &file, const XQAiwInterfaceDescriptor& implementation, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::create(file+impl)");
    return d->create(file, &implementation, embedded);
}


/*!
    List implementations that support handling the URI scheme of the given uri
    The interface name applied implicitly is declared by the constant XQI_URI_VIEW
*/

QList<XQAiwInterfaceDescriptor> XQApplicationManager::list(const QUrl &uri)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::list(uri)");
    return d->list(uri);
}

/*!
    List implementations that support handling the MIME type of of the given file
    The interface name applied implicitly is declared by the constant XQI_FILE_VIEW
*/
QList<XQAiwInterfaceDescriptor> XQApplicationManager::list(const QFile &file)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::list(file)");
    return d->list(file);
}

/*!
    List implementations that support handling the MIME type of of the given sharable file
    The interface name applied implicitly is declared by the constant XQI_FILE_VIEW
*/
QList<XQAiwInterfaceDescriptor> XQApplicationManager::list(const XQSharableFile &file)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::list(XQSharableFile)");
    return d->list(file);
}

/*!
    Create AIW request for the given file and the MIME type attached to the sharable file
    The interface name applied implicitly is declared by the constant XQI_FILE_VIEW
    By default, the operation name declared by constant XQOP_FILE_VIEW_SHARABLE is used.
*/
XQAiwRequest* XQApplicationManager::create(const XQSharableFile &file, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::create(XQSharableFile)");
    return d->create(file, NULL, embedded);
}

XQAiwRequest* XQApplicationManager::create(
    const XQSharableFile &file, const XQAiwInterfaceDescriptor& implementation, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::create(XQSharableFile+impl)");
    return d->create(file, &implementation, embedded);
}

int XQApplicationManager::lastError() const
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::lastError");
    return d->lastError();
}


bool XQApplicationManager::isRunning(const XQAiwInterfaceDescriptor& implementation) const
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::isRunning");
    return d->isRunning(implementation);
}


bool XQApplicationManager::getDrmAttributes(const QFile &file, const QList<int> &attributeNames, QVariantList &attributeValues)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::drmAttributes (file)");
    return d->getDrmAttributes(file, attributeNames, attributeValues);
}

bool XQApplicationManager::getDrmAttributes(const XQSharableFile &file, const QList<int> &attributeNames, QVariantList &attributeValues)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::drmAttributes (XQSharableFile)");
    return d->getDrmAttributes(file, attributeNames, attributeValues);
}

XQApplicationManager::ServiceStatus XQApplicationManager::status(const XQAiwInterfaceDescriptor& implementation)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::status");
    return (ServiceStatus)d->status(implementation);
}


