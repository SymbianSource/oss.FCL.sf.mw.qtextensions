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

#include <QObject>
#include <qglobal.h>

#include "xqservicelog.h"
#include "xqaiwservicedriver.h"
#include "xqaiwfiledriver.h"
#include "xqaiwuridriver.h"
#include "xqaiwrequest.h"

/*!
    \class XQAiwRequest
    \inpublicgroup QtBaseModule

    \ingroup ipc
    \brief Encapsulates the core functionality of the interworking requests
    
    The XQAiwRequest class encapsulates the core functionality of the interworking requests and hides the implementation details. 
    This object is created by the XQApplicationManager::create factory method.
    
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
    
    See the "examples/appmgrclient" included in the QtHighway release for usage examples.
    
    <b>Example usage:</b> \n
    The usage pattern for all the XQAiwRequest variants implemented as service providers , interface, QUrl, QFile, is similar both embedded
    and non-embedded usage.
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

            // Connect result handling signal
            connect(request, SIGNAL(requestOk(const QVariant&)), this, SLOT(handleOk(const QVariant&)));
            // Connect error handling signal or apply lastError function instead.
            connect(request, SIGNAL(requestError(int,const QString&)), this, SLOT(handleError(int,const QString&)));

            // Set function parameters
           QList<QVariant> args;
           args << parameter1;
           args << parameter2;
           request->setArguments(args);

           // In this example, request embedded launch (window groups chained)
           request->setEmbedded(true);

           // Send the request
           bool res = request.send();
           if  (!res) 
           {
               // Request failed. 
              return false;
           }
         
           // If making multiple requests to same service, you can save the request as member variable
           // In this example all done.
           delete request;
           return true;
        }

        void Client::handleOk(const QVariant& result)
        {
           // Handle result here or just save them.
           // Result could be a service specific error code also.
           // 
        }

        void Client::handleError(int errorCode, const QString& errorMessage)
        {
           // Handle error
        }
    \endcode
    
    \sa XQApplicationManager
*/

/*!
    Constructs interworking request to service application by the given interface \a descriptor
    which points to the dedicated implementation. The service application is not started during
    creation of the request.
    \param descriptor Points to the dedicated service implementation. Obtained via the XQApplicationManager::list function.
    \param operation Service function to be called, equals \a message parameter in XQServiceRequest.
    \param embedded True if window groups should be chained, false otherwise
    \return Constructed interworking request to service application object.
*/
XQAiwRequest::XQAiwRequest(const XQAiwInterfaceDescriptor& descriptor, const QString &operation, bool embedded)
    : QObject(),
      currentRequest(NULL),
      errorMsg(),
      errorCode(0),
      completeSignalConnected(false),
      errorSignalConnected(false)
{

    XQSERVICE_DEBUG_PRINT("XQAiwRequest::XQAiwRequest: %s %s,%d,%x",
                          qPrintable(descriptor.interfaceName()),
                          qPrintable(operation),
                          embedded,
                          descriptor.property(XQAiwInterfaceDescriptor::ImplementationId).toInt());
    
    // Initialize service request
    // The XQServiceRequest should actually accept service descriptor as input....
    currentRequest = new XQAiwServiceDriver(descriptor, operation);
    if (currentRequest)
    {
        currentRequest->setEmbedded(embedded);
    }
}

/*!
    Constructs interworking request to service application by the given uri and the interface \a descriptor
    which points to the dedicated implementation. The service application is not started during
    creation of the request.
    \param uri Uri for the given interworking request to service application.
    \param descriptor Points to the dedicated service implementation. Obtained via the XQApplicationManager::list function.
    \param operation Service function to be called, equals \a message parameter in XQServiceRequest.
    \return Constructed interworking request to service application object.
*/
XQAiwRequest::XQAiwRequest(
    const QUrl &uri, const XQAiwInterfaceDescriptor& descriptor, const QString &operation)
    : QObject(),
      currentRequest(NULL),
      errorMsg(),
      errorCode(0),
      completeSignalConnected(false),
      errorSignalConnected(false)
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::XQAiwRequest (uri): %s %s,%x",
                          qPrintable(descriptor.interfaceName()),
                          qPrintable(operation),
                          descriptor.property(XQAiwInterfaceDescriptor::ImplementationId).toInt());

    if (XQAiwUriDriver::hasCustomHandler(uri))
    {
        // Custom handling goes via URI driver
        currentRequest = new XQAiwUriDriver(uri, descriptor, operation);
    }
    else
    {
        // Otherwise, apply service based URI access
        currentRequest = new XQAiwServiceDriver(descriptor, operation);
    }
    
}


/*!
    Constructs interworking request to service application by the file and the interface \a descriptor
    which points to the dedicated implementation. The service application is not started during
    creation of the request.
    \param file File for the given interworking request to service application.
    \param descriptor Points to the dedicated service implementation. Obtained via the XQApplicationManager::list function.
    \param operation Service function to be called, equals \a message parameter in XQServiceRequest.
    \return Constructed interworking request to service application object.
*/
XQAiwRequest::XQAiwRequest(
    const QFile &file, const XQAiwInterfaceDescriptor& descriptor, const QString &operation)
    : QObject(),
     currentRequest(NULL),
      errorMsg(),
      errorCode(0),
      completeSignalConnected(false),
      errorSignalConnected(false)
{

   XQSERVICE_DEBUG_PRINT("XQAiwRequest::XQAiwRequest (file): %s %x",
                          qPrintable(file.fileName()),
                          descriptor.property(XQAiwInterfaceDescriptor::ImplementationId).toInt());

    // Initialize file service request
   if (!descriptor.interfaceName().isEmpty())
   {
       // Apply normal service request
       XQSERVICE_DEBUG_PRINT("Apply service driver");
       currentRequest = new XQAiwServiceDriver(descriptor, operation);
   }
   else 
   {
       // The is no service provider for the file.
       // So as backup plan, apply file driver to handle non-service file launches
       XQSERVICE_DEBUG_PRINT("Apply file driver");
       currentRequest = new XQAiwFileDriver(file, descriptor, operation);
   }
       
}

/*!
    Constructs interworking request to service application by the sharable file and the interface \a descriptor
    which points to the dedicated implementation. The service application is not started during
    creation of the request.
    \param file Sharable file for the given interworking request to service application.
    \param descriptor Points to the dedicated service implementation. Obtained via the XQApplicationManager::list function.
    \param operation Service function to be called, equals \a message parameter in XQServiceRequest.
    \return Constructed interworking request to service application object.
*/
XQAiwRequest::XQAiwRequest(
     const XQSharableFile &file, const XQAiwInterfaceDescriptor& descriptor, const QString &operation)
    : QObject(),
      currentRequest(NULL),
      errorMsg(),
      errorCode(0),
      completeSignalConnected(false),
      errorSignalConnected(false)
{

    XQSERVICE_DEBUG_PRINT("XQAiwRequest::XQAiwRequest (file handle): %x",
                          descriptor.property(XQAiwInterfaceDescriptor::ImplementationId).toInt());

    // Initialize file service request
    if (!descriptor.interfaceName().isEmpty())
    {
       // Apply normal service request
        XQSERVICE_DEBUG_PRINT("Apply service driver");
        currentRequest = new XQAiwServiceDriver(descriptor, operation);
    }
    else 
    {
       // The is no service provider for the file.
       // So as backup plan, apply file driver to handle non-service file launches
        XQSERVICE_DEBUG_PRINT("Apply file driver");
        currentRequest = new XQAiwFileDriver(file, descriptor, operation);
    }

}


XQAiwRequest::~XQAiwRequest()
{
    XQSERVICE_DEBUG_PRINT("~XQAiwRequest::XQAiwRequest");

    // Disconnect signals
    if (completeSignalConnected)
    {
        disconnect(currentRequest, SIGNAL(requestOk(const QVariant&)), this, SLOT(handleAsyncResponse(const QVariant&)));
    }
    if (errorSignalConnected)
    {
        disconnect(currentRequest, SIGNAL(requestError(int,const QString&)), this, SLOT(handleAsyncError(int)));
    }
    
    delete currentRequest; // Destructor cancels the async request
    
    for (int i=0; i<actionList.size(); i++)
    {
        delete actionList[i];
    }
}

/*!
    Create a QAction related to request from the registration data. Caller can
    add the action to wanted UI widget. When the action  is triggered the XQAiwRequest
    emits triggered() signal for caller.
    The XQAiwRequest owns the action (caller shall not delete the action object).
    \return QAction object, if there was action attached to request. Otherwise 0.
*/
QAction *XQAiwRequest::createAction()
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::createAction");

    QAction * action = currentRequest->createAction();
    if (action)
    {
        if (!connect(action, SIGNAL(triggered(bool)), this, SLOT(sendFromAction(bool)))) {
            XQSERVICE_CRITICAL_PRINT("Failed to connect QAction triggered signal to XQAiwRequest.");
        }
        actionList.append(action);
        return action; 
    }
    
    return NULL;
}

/*!
    Set arguments for the request. This shall be called before sending
    add the action to wanted UI widget. For the attached action, the
    triggered() signal emitted by the request is the last chance to
    add aguments.
    \param arguments List of arguments that will be transferred to service provider function
                     to be called
*/
void XQAiwRequest::setArguments(const QList<QVariant> &arguments)
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::setArguments");
    currentRequest->setArguments(arguments);
}


/*!
    Returns the last error code occured.
    IPC errors:
        - ENoError          = 0
        - EConnectionError  = -5000,  (Server might be busy)
        - EConnectionClosed = -4999,  
        - EServerNotFound   = -4998,
        - EIPCError         = -4997,
        - EUnknownError     = -4996,
        - ERequestPending   = -4995,  (already pending request exists)
        - EMessageNotFound  = -4994,
        - EArgumentError    = -4993
    \return Error code as integer value.
    \sa xqserviceglobal.h for error codes

*/
int XQAiwRequest::lastError() const
{
    int err = currentRequest->lastError();
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::lastError %d", err);
    return err;
}

/*!
    Returns the last error as text for debugging purposes.
    The content and details of the text may vary over API
    development time evolution.
    \return Error code as QString value.
*/
const QString& XQAiwRequest::lastErrorMessage() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::lastErrorMessage");
    return currentRequest->lastErrorMessage();
}


/*!
    Returns the implementation descriptor of a service attached to request.
    Caller can check meta-data information of the request.
    \return Implementation descriptor attached to the request.
*/
const XQAiwInterfaceDescriptor &XQAiwRequest::descriptor() const 
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::descriptor");
    return currentRequest->descriptor();
}

/*!
    Starts the service application if necessary and sends request on-ward.
    The results are delivered via requestOk() and requestError() signals.
    If the request is synchronous, the client application is blocked until
    service provider completes the request.
    \return True on success, false otherwise
*/
bool XQAiwRequest::send()
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::send");

    // do request
    if (sendExecute())
    {
        if (currentRequest->isSynchronous())
        {
            XQSERVICE_DEBUG_PRINT("XQAiwRequest::emit requestOk");
            emit requestOk(result);
            result.clear();
        }
        return true;
    } else
    {
        XQSERVICE_DEBUG_PRINT("XQAiwRequest::emit requestError");
        emit requestError(lastError(), lastErrorMessage());
        return false;
    }
}


/*!
    Convinience method for sending a synchronous request on-ward.
    The returnValue delivered via the output parameter.
    \return True on success, false otherwise
*/
bool XQAiwRequest::send(QVariant &returnValue)
{

    XQSERVICE_DEBUG_PRINT("XQAiwRequest::send(retValue)");

    // do request
    if (sendExecute())
    {
        if (currentRequest->isSynchronous())
        {
            XQSERVICE_DEBUG_PRINT("XQAiwRequest::set retValue ");
            // Do not emit requestOk as return value delivered directly
            returnValue = result; // Copy return value
            result.clear();
        }
        return true;
    } else
    {
        XQSERVICE_DEBUG_PRINT("XQAiwRequest::emit requestError");
        emit requestError(lastError(), lastErrorMessage());
        return false;
    }
   
}

/*!
    Request service application to be launched in embedded mode.
    \param embedded If set to true, service application will be launched
                    in embedded mode
*/
void XQAiwRequest::setEmbedded(bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::setEmbedded=%d",embedded);
    currentRequest->setEmbedded(embedded);
}

/*!
    Get the value of embedded option of the request.
    \return True if request is set to launch service application in embedded
            mode, false otherwise
*/
bool XQAiwRequest::isEmbedded() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::isEmbedded");
    return currentRequest->isEmbedded();
}

/*!
    Sets service operation. The XQApplicationManager::create() functions for
    files and URLs set the default operation, but it can be overriden using
    this function.
    \param operation Operation to be set to the request.
*/
void XQAiwRequest::setOperation(const QString &operation)
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::setOperation");
    currentRequest->setOperation(operation);
}

/*!
    Returns operation attached to the request.
    \return Operation attached to the request
*/
const QString &XQAiwRequest::operation() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::operation");
    return currentRequest->operation();
}

/*!
    Sets request as synchronous or asynchronous, based on the \a synchronous value.
    \param synchronous If set to true, request will be synchronous.
                       If set to false, request will be asynchronous
*/
void XQAiwRequest::setSynchronous(bool synchronous)
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::setSynchronous=%d", synchronous);
    currentRequest->setSynchronous(synchronous);
}

/*!
    Returns the value of the synchronous option.
    \return True if request is synchronous, false otherwise
*/
bool XQAiwRequest::isSynchronous() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::isSynchronous");
    return currentRequest->isSynchronous();
}

/*!
    Requests service application to be launched to background initially,
    or if already running, to go to background.
    \param background If set to true, service application will be launched
                      to background
*/
void XQAiwRequest::setBackground(bool background )
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::setbackground=%d", background);
    currentRequest->setBackground(background);
}

/*!
    Returns the value of the background option.
    \return True if request is set to launch service
                 application to background
*/
bool XQAiwRequest::isBackground() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::isBackground");
    return currentRequest->isBackground();
}

/*!
    Used to set additional UI behavior type options to the request.
    Embedded and background options are handled by their own functions.
    This function should not be used to implement additional data
    parameters for operations!
    \param info UI bahavior type option to be set to the request.
*/
void XQAiwRequest::setInfo(const XQRequestInfo &info)
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::setInfo");
    return currentRequest->setInfo(info);
}

/*!
    Returns additional options attached to the request.
    \return Additional options attached to the request.
*/
XQRequestInfo XQAiwRequest::info() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::info");
    return currentRequest->info();
}



const QVariant &XQAiwRequest::results() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::results");
    return result;
}


void XQAiwRequest::sendFromAction(bool checked)
{
    Q_UNUSED(checked);
    
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::sendFromAction");
    
    emit triggered(); // Last chance to setup request parameters

    // do request
    if (sendExecute())
    {
        if (isSynchronous())
        {
            emit requestOk(result);
            result.clear();
        }
    } else
    {
        emit requestError(lastError(), lastErrorMessage());
    }
}

bool XQAiwRequest::sendExecute()
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::sendExecute>>>");

    QStringList list;
    bool res = true;
    if (!isSynchronous() && !completeSignalConnected)
    {
        // Set async request signals once
        XQSERVICE_DEBUG_PRINT("request::async send");
        connect(currentRequest, SIGNAL(requestOk(const QVariant&)), this, SLOT(handleAsyncResponse(const QVariant&)));
        completeSignalConnected = true;
    }
    if (!errorSignalConnected)
    {
        // Connect always error signal  once
        connect(currentRequest, SIGNAL(requestError(int,const QString&)), this, SLOT(handleAsyncError(int)));
        errorSignalConnected = true;
    }
    
    XQSERVICE_DEBUG_PRINT("request::send>>>");
    res = currentRequest->send(result);  // Result is valid for sync request only    
    XQSERVICE_DEBUG_PRINT("request::send: %d<<<", res);
    
    errorCode = currentRequest->lastError(); // ask always
    if (errorCode || !res)
    {
        res = false;
    }
    
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::sendExecute: %d<<<", res);
    
    return res;

}



void XQAiwRequest::handleAsyncResponse(const QVariant& value)
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::handleAsyncResponse");
    emit requestOk(value);
}

void XQAiwRequest::handleAsyncError(int err)
{
   XQSERVICE_DEBUG_PRINT("XQAiwRequest::handleAsyncError");
   errorCode = err;
   emit requestError(lastError(), lastErrorMessage());
}
