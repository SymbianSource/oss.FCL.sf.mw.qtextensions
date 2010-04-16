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
#include "ut_xqservicechannel_threadstorage.h"

#include "xqservicechannel.h"

XQServiceIpcClient::XQServiceIpcClient(
    const QString& ipcConName, bool isServer, bool isSync, XQServiceRequestCompletedAsync* rc,
    const void *userData)
    : QObject()
{
}

/*
void XQServiceIpcClient::init()
{
}
*/

XQServiceIpcClient::~XQServiceIpcClient()
{
}

bool XQServiceIpcClient::listen()
{
    return !testStorage()->failConnect;
}

bool XQServiceIpcClient::connectToServer()
{
    return !testStorage()->failConnect;
}

bool XQServiceIpcClient::handleRequest( ServiceIPCRequest */*aRequest*/ )
{
    return true;
}

void XQServiceIpcClient::handleCancelRequest( ServiceIPCRequest* /*aRequest*/ )
{
}

void XQServiceIpcClient::handleDeleteRequest( ServiceIPCRequest* /*aRequest*/ )
{
}

bool XQServiceIpcClient::send
    (const QString& ch, const QString& msg, const QByteArray& data, QByteArray& retData, int cmd)
{
    if (!testStorage()->failSend) {
        XQServiceChannel::sendLocally(ch, msg, data);
    }

    return !testStorage()->failSend;
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

void XQServiceIpcClient::readyRead()
{
}

void XQServiceIpcClient::readDone()
{
}

