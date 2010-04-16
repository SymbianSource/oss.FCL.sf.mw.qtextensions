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

/* THIS IS A XQServiceIpcClient STUB used only for UNIT TEST */

#include "xqserviceipcclient.h"
#include "xqservicechannel.h"
#include "xqservicethreaddata.h"


struct XQServicePacketHeader
{
    int totalLength;
    int command;
    int chLength;
    int msgLength;
    int dataLength;
};

XQServiceIpcClient::XQServiceIpcClient(const QString& ipcConName, bool isServer, bool isSync,
                                       XQServiceRequestCompletedAsync* rc,
                                       const void *userData)
    : QObject(), serviceIpc(NULL), serviceIpcServer(NULL), callBackRequestComplete(rc),
      mUserData(userData)
{
    mIpcConName = ipcConName;
    server = isServer;
    synchronous = isSync ;
    currentRequest = NULL ;
}

XQServiceIpcClient::~XQServiceIpcClient()
{
disconnected();
}

bool XQServiceIpcClient::listen()
{
    return true;
}

bool XQServiceIpcClient::connectToServer()
{
    return true;
}

bool XQServiceIpcClient::handleRequest( ServiceIPCRequest *aRequest )
{
    bool result(true);
    int index = setCurrentRequest(aRequest);
        
    QVariant ret;

    if (!requestAsync) {
        completeRequest(index,ret);
    }
    
    return result;
}

/*!
 * From MServiceIPCObserver
 * \see MServiceIPCObserver::handleCancelRequest( ServiceIPCRequest *aRequest )
 */
void XQServiceIpcClient::handleCancelRequest( ServiceIPCRequest* /*aRequest*/ )
{
//TODO:
}

/*!
 * From MServiceIPCObserver
 * \see MServiceIPCObserver::handleCancelRequest( ServiceIPCRequest *aRequest )
 */
void XQServiceIpcClient::handleDeleteRequest( ServiceIPCRequest* /*aRequest*/ )
{
//TODO:
}

bool XQServiceIpcClient::sendChannelCommand(int cmd, const QString& ch)
{    
    return true;
}

bool XQServiceIpcClient::send
    (const QString& ch, const QString& msg, const QByteArray& data, QByteArray &retData, int cmd)
{
    return true;
}

bool XQServiceIpcClient::cancelPendingSend(const QString& channel)
{
    return true;
}

void XQServiceIpcClient::disconnected()
{
}

void XQServiceIpcClient::clientError(int error)
{
}

int XQServiceIpcClient::setCurrentRequest(ServiceIPCRequest* request)
{
    if (currentRequest && request)
        return 0;
    currentRequest = request;
    requestAsync = false;
    //TODO:
    return 1;
}

int XQServiceIpcClient::setCurrentRequestAsync()
{
    requestAsync = true;
    //TODO:
    return 1;
}

bool XQServiceIpcClient::completeRequest(int index, const QVariant& retValue)
{
    //TODO:
    Q_UNUSED(index);
    if (!currentRequest)
        return false;
    
    bool ret = true;
    setCurrentRequest(NULL) ;
    return ret;
}


//
// This function need to be called during a slot call before returning from
// the slot.
// The lastId might change after returning from the slot call as
// other possible requests may arrive
//
XQRequestInfo XQServiceIpcClient::requestInfo() const
{
    // Dummy
    return XQRequestInfo();
}

//
// This internal function need to be called before a slot call to set the request info
// The provider can then call requestInfo() to get the data.
// The lastId might change after returning from the slot call as
// other possible requests may arrive
//
bool XQServiceIpcClient::setRequestInfo(XQRequestInfo &info)
{
    // Dummy
    return true;
}



void XQServiceIpcClient::readyRead()
{
}

void XQServiceIpcClient::readDone()
{
}

