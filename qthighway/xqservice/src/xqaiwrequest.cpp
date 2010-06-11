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
       // This means descriptor is incompete and created for carrying implementation information only.
       // See XQApplicationManagerPrivate::getAppDescriptor or XQApplicationManagerPrivate::listMimeHandlers
       XQSERVICE_DEBUG_PRINT("Apply file driver");
       currentRequest = new XQAiwFileDriver(file, descriptor, operation);
   }
       
}


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

    // No other way to pass sharable file as via service request

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
    emits "triggered" signal for caller.
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
    "triggered" signal emitted by the request is the last chance to
    add aguments.
*/
void XQAiwRequest::setArguments(const QList<QVariant> &arguments)
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::setArguments");
    currentRequest->setArguments(arguments);
}


/*!
    Returns the last error code occured.
    IPC errors:
        EConnectionError  = -5000,  (Server might be busy)
        EConnectionClosed = -4999,  
        EServerNotFound   = -4998,
        EIPCError         = -4997,
        EUnknownError     = -4996,
        ERequestPending   = -4995   (already pending request exists)
        EM   = -4995   (already pending request exists)

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
*/
const QString& XQAiwRequest::lastErrorMessage() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::lastErrorMessage");
    return currentRequest->lastErrorMessage();
}


/*!
    Returns the implementation descriptor of a service attached to request.
    Caller can check meta-data information of the request.
*/
const XQAiwInterfaceDescriptor &XQAiwRequest::descriptor() const 
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::descriptor");
    return currentRequest->descriptor();
}

/*!
    Send request on-ward.
    The results are delivered via "requestOk" and "requestError" signals.
    \return true on success, false otherwise
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
    \return true on success, false otherwise
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


void XQAiwRequest::setEmbedded(bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::setEmbedded=%d",embedded);
    currentRequest->setEmbedded(embedded);
}
bool XQAiwRequest::isEmbedded() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::isEmbedded");
    return currentRequest->isEmbedded();
}

void XQAiwRequest::setOperation(const QString &operation)
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::setOperation");
    currentRequest->setOperation(operation);
}

const QString &XQAiwRequest::operation() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::operation");
    return currentRequest->operation();
}


void XQAiwRequest::setSynchronous(bool synchronous)
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::setSynchronous=%d", synchronous);
    currentRequest->setSynchronous(synchronous);
}
bool XQAiwRequest::isSynchronous() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::isSynchronous");
    return currentRequest->isSynchronous();
}

void XQAiwRequest::setBackground(bool background )
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::setbackground=%d", background);
    currentRequest->setBackground(background);
}

bool XQAiwRequest::isBackground() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::isBackground");
    return currentRequest->isBackground();
}

void XQAiwRequest::setInfo(const XQRequestInfo &info)
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::setInfo");
    return currentRequest->setInfo(info);
}

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
