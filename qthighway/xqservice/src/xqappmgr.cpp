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

/*!
    \class XQApplicationManager
    \inpublicgroup QtBaseModule

    \ingroup ipc
    \brief Factory class to list interface descriptors and create interworking objects (XQAiwRequest)
    
    XQApplicationManager lists interface descriptors by interface and /or service name. It is also used to
    create interworking objects (XQAiwRequest).
    
    This class is a part of API to be used by the applications instead of using XQServiceRequest directly.
    
    The Application Manager API offers centric place for applications UIs to handle application to application interworking use cases, like:
    - Synchronous out-of-process service call from client to service provider, where service provider needs to complete the request before
      control comes back to requesting client.
    - Asynchronous out-of-process service call from client to service provider, where Service provider completes the request whenever suitable.
      The control returns back requesting as soon the service provider has received the asynchronous call (can be applied to notifications as well).
    - Embedded out-of-process service call. In this case window groups are chained and "Back" returns to client window.
    - Any named Qt type in the Qt meta-object system can be used as a service call parameter or return value. Also own, custom meta-types are supported.
    - Launched service provider application (.exe) if not already running when client makes service call to it.
    - List and discover services dynamically.
    - Apply UI related options upon service launch, like "launch as embedded", "launch to foreground" and "launch to backround".
    - Opening files to be viewed by a file viewing interface.
    - Opening URI to be viewed by a URI viewing interface. Includes also launching activity URIs (appto) as fire-and-forget manner.
    - Miscellanous AIW support, like get service stasus or get DRM attributes.
    
    <b>Example usage:</b> \n
    
    The usage pattern for all the XQApplicationManager variants implemented as service providers , interface, QUrl, QFile, is similar both embedded
    and non-embedded usage:
    \code
        // Recommended way is to add XQApplicationManager as member variable to class
        // Later on when caching of services
        // You can use the class also as local variable.
        class Client
        {

        public:
             // Service access
            bool accessService(void);

        private slots:
                void handleOk(const QVariant &result);
                void handleError(int errorCode, const QString& errorMessage);
        private:
              XQApplicationManager mAiwMgr;
        };


        //  In client.cpp
        bool Client::accessService(void)
        {
            QString parameter1("+3581234567890");
            int parameter2 = 3;

            bool embedded=true;  // or false

            XQAiwRequest *request;
            // Create request by interface name, the very first service implementation
            // applied.
            request = mAiwMgr.create("Interface", "functionName2(QString, int)", embedded);

            // If dedicated service is wanted, apply this 
            // request = mAiwMgr.create("Service", "Interface", 
            //                          "functionName2(QString, int)", embedded);

            if (request == NULL)
            {
                // Service not found 
                return false;
            }
            // ... Perform further processing
        }
    \endcode
    
    Access service by descriptor:
    \code
        QList<XQAiwInterfaceDescriptor> implementations = appmgr.list("Interface");

       // Display service in UI and make selection possible.
        foreach (XQAiwInterfaceDescriptor d, implementations)
        {
            qDebug() << "Service=" << d.serviceName();
            qDebug() << "Interface=" << d.interfaceName();
            qDebug("Implementation Id=%x",d.property(XQAiwInterfaceDescriptor::ImplementationId).toInt());
        }


        // Select correct implementation
        XQAiwInterfaceDescriptor selectedImpl = doSelectService();

        // The difference to the previous example is is how request is created
        // via application mgr.

        // ...See previous example
        request = mAiwMgr.create(selectedImpl, embedded);
        // ....See previous example
    \endcode
    
    The XQApplicationManager supports opening activity (see Terminology) URIs (appto scheme) as fire-and-forget mannner:
    \code
        QUrl url("appto://10207C62?activityname=MusicMainView"); 

        // The difference to the previous example is is how request is created
        // via application mgr.
        request = mAiwMgr.create(url);
        if (request == NULL)
        {
            // No handlers for the URI
            return;
        }

        // Set function parameters
        QList<QVariant> args;
        args << uri.toSring();
        request->setArguments(args);

        // Send the request
        bool res = request.send();
        if  (!res) 
        {
            // Request failed. 
            int error = request->lastError();
            // Handle error
        }
     
       // All done.
       delete request;
    \endcode
    
    \sa XQAiwRequest
*/

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
    \param embedded True if embedded (window groups chained) call, false otherwise.
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
    
    \param implementation Valid interface descriptor obtained by the list() call.
    \param operation The function signature to be called via the interface.
                     Can be set later via XQAiwRequest::setOperation.
                     Apply the xqaiwdecl.h file for common constants.
    \param embedded True if embedded call, false otherwise.
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
    \param embedded True if embedded (window groups chained) call, false otherwise.
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
    List available implementations for the given \a service and \a interface names from the service registry.
    The \a operation is reserved for future use.
    \param service Service name as mentioned in the service registry file
    \param interface Interface name as mentioned in the service registry file
    \param operation The operation signature to be called.  Reserved for future use.
    \return List of found interface descriptors that matched to both the \a service and \a interface names, otherwise empty list.
    \sa list(const QString &interface, const QString &operation)
*/
QList<XQAiwInterfaceDescriptor> XQApplicationManager::list(
    const QString &service, const QString &interface, const QString &operation)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::list");
    return d->list(service, interface, operation);
}

/*!
    Creates AIW request to view the  given URI (having a attached scheme).
    The interface name applied implicitly is the XQI_URI_VIEW (from xqaiwdecl.h),
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
    Creates AIW request to view the given URI by service implementation.
    The interface name applied implicitly is the XQI_URI_VIEW (from xqaiwdecl.h),
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

/*!
    Same as basic create(const QFile &file, bool embedded), but applies the interface descriptor to select the dedicated implementation.
    \param file The file to be viewed
    \param implementation Valid interface descriptor obtained by the list(const QFile &file) call.
    \param embedded True if embedded (window groups chained) call, false otherwise
    \return The application interworking request instance, or NULL if no viewer found.
    \sa xqaiwdecl.h for constants values
*/
XQAiwRequest* XQApplicationManager::create(
    const QFile &file, const XQAiwInterfaceDescriptor& implementation, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::create(file+impl)");
    return d->create(file, &implementation, embedded);
}


/*!
    List implementations that support handling the URI scheme of the given uri.
    The interface name applied implicitly is declared by the constant XQI_URI_VIEW.
    \param uri The URI scheme that should be matched to the interface
    \return List of found interface descriptors that matched to the URI scheme, otherwise empty list.
    \sa list(const QString &interface, const QString &operation)
*/

QList<XQAiwInterfaceDescriptor> XQApplicationManager::list(const QUrl &uri)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::list(uri)");
    return d->list(uri);
}

/*!
    List implementations that support handling the MIME type of the given file.
    The interface name applied implicitly is declared by the constant XQI_FILE_VIEW.
    \param file File which MIME type should be supported by the interface.
    \return List of found interface descriptors for applications that can handle the file, otherwise empty list.
    \sa list(const QString &interface, const QString &operation)
*/
QList<XQAiwInterfaceDescriptor> XQApplicationManager::list(const QFile &file)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::list(file)");
    return d->list(file);
}

/*!
    List implementations that support handling the MIME type of of the given sharable file.
    The interface name applied implicitly is declared by the constant XQI_FILE_VIEW.
    \param file Sharable file which MIME type should be supported by the interface.
    \return List of found interface descriptors for applications that can handle the file, otherwise empty list.
    \sa list(const QString &interface, const QString &operation)
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
    \param file The sharable file to be viewed
    \param embedded True if embedded (window groups chained) call, false otherwise
    \return The application interworking request instance, or NULL if no viewer found.
*/
XQAiwRequest* XQApplicationManager::create(const XQSharableFile &file, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::create(XQSharableFile)");
    return d->create(file, NULL, embedded);
}

/*!
    Same as basic create(const XQSharableFile &file, bool embedded), but applies the interface descriptor to select the dedicated implementation.
    \param file The sharable file to be viewed
    \param implementation Valid interface descriptor obtained by the list(const XQSharableFile &file) call.
    \param embedded True if embedded (window groups chained) call, false otherwise
    \return The application interworking request instance, or NULL if no viewer found.
    \sa xqaiwdecl.h for constants values
*/
XQAiwRequest* XQApplicationManager::create(
    const XQSharableFile &file, const XQAiwInterfaceDescriptor& implementation, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::create(XQSharableFile+impl)");
    return d->create(file, &implementation, embedded);
}

/*!
    Returns error code of the last performed operation.
    \return Error code of the last operation, 0 if no error occured.
*/
int XQApplicationManager::lastError() const
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::lastError");
    return d->lastError();
}

/*!
    Tests whether given implementation is running. That is, the service provider has published
    the full service name attached to the given interface descriptor.
    \param implementation Interface that is tested for being run.
    \return True if the implementation is running, false otherwise.
*/
bool XQApplicationManager::isRunning(const XQAiwInterfaceDescriptor& implementation) const
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::isRunning");
    return d->isRunning(implementation);
}

/*!
    Gets the values of the DRM related \a attributeNames, like "IsProtected",
    "IsForwardable", "MimeType" for a given \a file.
    \param file File for which DRM attributes are retrieved
    \param attributeNames List of attributes that should be retrieved (check #DrmAttribute)
    \param attributeValues On success fills this list whith values, where each value is QVariant of the integer or string type.
                           If attribute value does not exists or other error occurs when reading the attribute, the invalid QVariant
                           value is returned.
    \return True on success, upon error returns false (e.g file does not exists or other general error).
*/
bool XQApplicationManager::getDrmAttributes(const QFile &file, const QList<int> &attributeNames, QVariantList &attributeValues)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::drmAttributes (file)");
    return d->getDrmAttributes(file, attributeNames, attributeValues);
}

/*!
    Gets the values of the DRM related \a attributeNames, like "IsProtected",
    "IsForwardable", "MimeType" for a given sharable file.
    \param file Sharable file for which DRM attributes are retrieved
    \param attributeNames List of attributes that should be retrieved (check #DrmAttribute)
    \param attributeValues On success fills this list whith values, where each value is QVariant of the integer or string type.
                           If attribute value does not exists or other error occurs when reading the attribute, the invalid QVariant
                           value is returned.
    \return True on success, upon error returns false (e.g file does not exists or other general error).
*/
bool XQApplicationManager::getDrmAttributes(const XQSharableFile &file, const QList<int> &attributeNames, QVariantList &attributeValues)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::drmAttributes (XQSharableFile)");
    return d->getDrmAttributes(file, attributeNames, attributeValues);
}

/*!
    Checks the status of the given service interface.
    \param implementation Interface which status is being checked.
    \return Status of the service.
*/
XQApplicationManager::ServiceStatus XQApplicationManager::status(const XQAiwInterfaceDescriptor& implementation)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::status");
    return (ServiceStatus)d->status(implementation);
}


