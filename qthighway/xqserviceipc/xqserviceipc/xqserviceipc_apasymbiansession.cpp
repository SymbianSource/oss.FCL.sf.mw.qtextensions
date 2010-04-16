/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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

#include "xqserviceipc_apasymbiansession.h"

#include "xqserviceipcconst.h"
#include <QString>

#include <xqservicemanager.h>

#include <QString>

namespace QtService
{
// ============================== MEMBER FUNCTIONS ============================

/*!
    \class RApaIPCSession

    Symbian class encapsulating RMessage2 interface
*/

/*!
    Send a message
    @param aFunction function code
    @return message completion code
*/
TInt RApaIPCSession::SendReceiveL(TInt aFunction) const
{
    XQSERVICE_DEBUG_PRINT("RApaIPCSession::SendReceiveL(1)");
    return User::LeaveIfError(RSessionBase::SendReceive(aFunction));
}

/*!
    Send a message
    @param aFunction function code
    @param aArgs parameter to server
    @return message completion code
*/
TInt RApaIPCSession::SendReceiveL(TInt aFunction, const TIpcArgs& aArgs) const
{
    XQSERVICE_DEBUG_PRINT("RApaIPCSession::SendReceiveL(2)");
    return User::LeaveIfError(RSessionBase::SendReceive(aFunction,aArgs));
}

/*!
    Send message asynchronously
    @param aFunction function code
    @param aStatus the request status object used to contain the 
           completion status of the request
*/
EXPORT_C void RApaIPCSession::SendReceive(TInt aFunction,
                                          TRequestStatus& aStatus) const
{
    XQSERVICE_DEBUG_PRINT("RApaIPCSession::SendReceive(1)");
    RSessionBase::SendReceive(aFunction,aStatus);
    XQSERVICE_DEBUG_PRINT("RApaIPCSession::SendReceive(1) done");
}

/*!
    Send message asynchronously
    @param aFunction function code
    @param aArgs parameter to server
    @param aStatus the request status object used to contain the 
           completion status of the request
*/
void RApaIPCSession::SendReceive(TInt aFunction,
                                       const TIpcArgs& aArgs,
                                       TRequestStatus& aStatus) const
{
    XQSERVICE_DEBUG_PRINT("RApaIPCSession::SendReceive(2)");
    RSessionBase::SendReceive(aFunction,aArgs,aStatus);
    XQSERVICE_DEBUG_PRINT("RApaIPCSession::SendReceive(2) done");
}

/*!
    Connect to server
    @param aServer server name
    @param aVersion version of the server
    @return KErrNone success, otherwise system error code
*/
TInt RApaIPCSession::Connect(const TDesC& aServer, 
                                 const TVersion& aVersion)
{
    XQSERVICE_DEBUG_PRINT("RApaIPCSession::Connect");
    iVersion = aVersion;
    return CreateSession(aServer, aVersion, 10);
}

/*!
    Get version info
    @return version info
*/
TVersion RApaIPCSession::Version() const
{
    XQSERVICE_DEBUG_PRINT("RApaIPCSession::Version");
    return iVersion;
}


TUid RApaIPCSession::ServiceUid() const
{
    XQSERVICE_DEBUG_PRINT("RApaIPCSession::ServiceUid");
    return iUid;
}

void RApaIPCSession::setServiceUid(TInt uid)
{
    XQSERVICE_DEBUG_PRINT("RApaIPCSession::setServiceUid %x", uid);
    iUid.iUid = uid;
}

}

// END OF FILE
