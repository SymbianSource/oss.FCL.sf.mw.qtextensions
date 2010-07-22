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
    Creates AIW request by interface and operation name.
    The first found service implementation is returned. If you need to activate specific implementation
    you shall first list() implementations and use the overloaded create() with XQAiwInterfaceDescriptor
    to select the correct implementation. 
    
    \param interface Interface name as mentioned in the service registry file.
                     Apply the xqaiwdecl.h file for common constants.
    \param operation The function signature to be called via the interface.
                     Can be set later via XQAiwRequest::setOperation.
                     Apply the xqaiwdecl.h file for common constants.
    \param embedded True if embedded (window groups chained) call, false otherwise
                    Can be set later via XQAiwRequest::setEmbedded.
    \return The application interworking request instance, NULL if no service is available
    \sa list(const QString &interface, const QString &operation)
    \sa create( const XQAiwInterfaceDescriptor &implementation, const QString &operation, bool embedded);
    \sa xqaiwdecl.h for constants values
    
    Example usage:
    \code
    #include <xqaiwdecl.h>
    #include <xqapplicationmanager.h>
    
    // XQApplicationManager mAppMgr;  // Set manager as class member
    
    XQAiwRequest * req = this->mAppMgr.create(XQI_IMAGE_FETCH, XQOP_IMAGE_FETCH, false);
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
    Creates AIW request by interface implementation descriptor and operation name.
    The descriptor is got from the list() call.
    As combination [service,interface,version] shall be unique,
    the descriptor points to one implementation and thus selects correct
    implementation.
    
    \param implementation Valid interface descriptor obtained by the "list" call.
    \param operation The function signature to be called via the interface.
                     Can be set later via XQAiwRequest::setOperation.
                     Apply the xqaiwdecl.h file for common constants.
    \param embedded True if embedded call, false otherwise
                     Can be set later via XQAiwRequest::setEmbedded.
    \return The application interworking request instance, NULL if no service is available
    \sa list()
    \sa create( const QString &interface, const QString &operation, bool embedded)
    \sa create( const QString &service, const QString &interface, const QString &operation, bool embedded)
    \sa xqaiwdecl.h for constants values

    Example usage:
    \code
    #include <xqaiwdecl.h>
    #include <xqapplicationmanager.h>
    
    XQApplicationManager appMgr;  // Prefer one instance only 
    QList<XQAiwInterfaceDescriptor> list = appMgr.list(XQI_CAMERA_CAPTURE, "");
    //
    // Populate a user interface widget and select proper implementation via descriptor
    // ... 
    XQAiwRequest * req = appMgr.create(selectedDescriptor, XQOP_CAMERA_CAPTURE);
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
    Creates AIW request by service and interface name.  You should not normally use this
    overload as typically service request are done by the interface name only.
    Internally this applies list() operation and applies the first found service
    implementation.
    
    \param service Service name as mentioned in the service registry file
    \param interface Interface name as mentioned in the service registry file
    \param operation The function signature to be called via the interface.
                     Can be set later via XQAiwRequest::setOperation.
    \param embedded True if embedded (window groups chained) call, false otherwise
                     Can be set later via XQAiwRequest::setEmbedded.
    \return The application interworking request instance, NULL if no service is available
    \sa XQApplicationManager::create( const QString &interface, const QString &operation, bool embedded)
    \sa create( const XQAiwInterfaceDescriptor &implementation, const QString &operation, bool embedded);
    \sa xqaiwdecl.h for constants values

    Example usage:
    \code
    #include <xqaiwdecl.h>
    #include <xqapplicationmanager.h>

    #include <xqappmgr.h>;
    #include "componentglobal.h";

    // XQApplicationManager mAppMgr;  // manager as class member

    // Use embedded call.
    XQAiwRequest * req = this->mAppMgr.create(QLatin1String("photos"), XQI_IMAGE_FETCH, XQOP_IMAGE_FETCH, true);
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
    \return The list of implementation descriptors, or empty list if no implementations found.
    \sa xqaiwdecl.h for constants values

    Example usage:
    \code
    #include <xqaiwdecl.h>
    #include <xqapplicationmanager.h>
    QList<XQAiwInterfaceDescriptor> list = this->mAppmgr.list(XQI_IMAGE_FETCH, "");
    //
    // Populate a user interface widget and select proper implementation via descriptor
    // ... 
    XQAiwRequest * req = this->mAppMgr.create(selectedDescriptor, XQOP_IMAGE_FETCH);
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
    \sa list(const QString &interface, const QString &operation)
*/
QList<XQAiwInterfaceDescriptor> XQApplicationManager::list(
    const QString &service, const QString &interface, const QString &operation)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::list");
    return d->list(service, interface, operation);
}

/*!
    Creates AIW request to view the  given URI (having a attached scheme)
    The interface name applied implicitly isthe XQI_URI_VIEW (from xqaiwdecl.h),
    unless there is custom handling attached to URI scheme.
    The first found service implementation is applied.
    A service declares support for scheme(s) (CSV list) by adding the custom property key
    (see the constant XQCUSTOM_PROP_SCHEMES value) to the service XML.
    By default, the operation name declared by constant XQOP_URI_VIEW is used.
    Custom handling for certainsoverride the default service handling:
     - "http:" and "https: schemes are handled by QDesktopServices::openUrl()
     - "appto"  URIs is handled by corresponding Activity Manager Framework
     - "file": Local file scheme is handled via the XQI_FILE_VIEW interface
               (the same as applie to e.g. create(QFile))
         
    \param uri The URI to be viewed
    \param embedded True if embedded (window groups chained) call, false otherwise
    \return The application interworking request instance, or NULL if no URI viewer found.
    \sa xqaiwdecl.h for constants values
*/
XQAiwRequest* XQApplicationManager::create( const QUrl &uri, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::create(url)");
    return d->create(uri, NULL, embedded);
}

/*!
    Creates AIW request to view the given URI by service implementation
    The interface name applied implicitly is XQI_URI_VIEW (from xqaiwdecl.h),
    unless there is custom handling attached to URI scheme.
    A service declares support for scheme(s) (CSV list) by adding the custom property key
    (see the constant XQCUSTOM_PROP_SCHEMES value) to the service XML.
    Custom handling for certainsoverride the default service handling:
     - "http:" and "https: schemes are handled by QDesktopServices::openUrl()
     - "appto"  URIs is handled by corresponding Activity Manager Framework
     - "file": Local file scheme is handled via the XQI_FILE_VIEW interface
               (the same as applie to e.g. create(QFile))

    \param uri The URI to be viewed
    \param embedded True if embedded (window groups chained) call, false otherwise
    \param implementation Valid interface descriptor obtained by the "list(QUrl)" call.
    \return The application interworking request instance, or NULL if no URI viewer found.
    \sa xqaiwdecl.h for constants values
*/
XQAiwRequest* XQApplicationManager::create(
    const QUrl &uri, const XQAiwInterfaceDescriptor& implementation, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::create(uri+impl)");
    return d->create(uri, &implementation, embedded);
}

/*!
    Create AIW requests to view the given file and having the MIME type attached.
    The interface name applied implicitly is declared by the constant XQI_FILE_VIEW
    The first found service implementation is applied.
    \param file The file to be viewed
    \param embedded True if embedded (window groups chained) call, false otherwise
    \return The application interworking request instance, or NULL if no viewer found.
    \sa xqaiwdecl.h for constants values
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


