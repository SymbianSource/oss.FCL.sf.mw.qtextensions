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
* Description:  Symbian implementation for IPC server
*
*/

#include "xqservicelog.h"

#include "xqserviceipcserver_apasymbianserver.h"
#include "xqserviceipcserver_apasymbiansession.h"

namespace QtService
{
// Server Security Policy
const TUint KServerRangeCount = 2;
const TInt KServerRanges[KServerRangeCount] = { 
                0, //range is [0-1)
                2 //range is [2-KMaxTInt] 
                };
const TUint8 KSeverElementsIndex[KServerRangeCount] = { 
                0,
                CPolicyServer::ENotSupported };

const CPolicyServer::TPolicyElement KServerPolicyElements[] = { {
                _INIT_SECURITY_POLICY_C1(ECapabilityWriteDeviceData),
                CPolicyServer::EFailClient } };

const CPolicyServer::TPolicy KServerPolicy = {
                CPolicyServer::EAlwaysPass, //specifies all connect attempts should pass
                KServerRangeCount, KServerRanges, KSeverElementsIndex,
                KServerPolicyElements };

enum
{
    EServerPriority = CActive::EPriorityStandard
};

// ======== MEMBER FUNCTIONS ========

/*!
    \class CApaSymbianServer
    \brief Symbian client server implementation
*/

/*!
    Constructor.
*/
CApaSymbianServer::CApaSymbianServer() /*:
CPolicyServer( EServerPriority,  KServerPolicy)*/                                                 
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianServer::CApaSymbianServer");
    SetPriority(EServerPriority);
}

/*!
    2nd phased constructor
*/
void CApaSymbianServer::ConstructL()
{
}

/*!
    Two phased constructor
*/
CApaSymbianServer* CApaSymbianServer::NewL()
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianServer::NewL");
    CApaSymbianServer* self = new( ELeave ) CApaSymbianServer;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

/*!
    Destructor
*/
CApaSymbianServer::~CApaSymbianServer()
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianServer::~CApaSymbianServer");
}

/*!
    Start listening for new service requests.
    \param aServerName name of the server.
    \return true if successful.
*/
bool CApaSymbianServer::listen( const QString& aServerName )
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianServer::listen");
    XQSERVICE_DEBUG_PRINT("aServerName: %s", qPrintable(aServerName));
    bool listening(true);

    // Needs to be here becuase Q_Ptr isonly set after constructor of public
    iObserver = Observer();
    TPtrC serverName(reinterpret_cast<const TUint16*> (aServerName.utf16()));
    TInt err=0;
    TRAP( err, StartL(serverName) );
    XQSERVICE_DEBUG_PRINT("listen status=%d", err);
    if (err != KErrNone) {
        listening = false;
    }
    // Complete the server rendezvous that th client started
    XQSERVICE_DEBUG_PRINT("CApaSymbianServer::Rendezvouz");
    RProcess::Rendezvous(KErrNone);
    XQSERVICE_DEBUG_PRINT("CApaSymbianServer::Rendezvouz done");
    XQSERVICE_DEBUG_PRINT("listening: %d", listening);
    return listening;
}

/*!
    Shutdown the server and stop serving clients.
*/
void CApaSymbianServer::disconnect()
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianServer::disconnect");
    // Symbian Servers do not have disconnect, 
    // the process has to exit
}

/*!
    Create a new session, derived from CPolicyServer.
    \param aVersion Version of the server.
    \param aMessage Message object.
*/
CSession2* CApaSymbianServer::NewSessionL(const TVersion& aVersion,
                                          const RMessage2& aMessage) const
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianServer::NewSessionL");
    TVersion v(1, 0, 0);
    if (!User::QueryVersionSupported(v, aVersion)) {
        XQSERVICE_DEBUG_PRINT("Not supported version");
        User::Leave(KErrNotSupported);
    }

    // Create a new Symbian Session for the client
  return CApaServerSymbianSession::NewL(iObserver);
}

CPolicyServer::TCustomResult CApaSymbianServer::CreateServiceSecurityCheckL
                                    ( TUid /*aServiceType*/, 
                                      const RMessage2& /*aMsg*/, 
                                      TInt& /*aAction*/,
                                      TSecurityInfo& /*aMissing*/ )
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianServer::CreateServiceSecurityCheckL");
    return CPolicyServer::EPass;
}

}

// END OF FILE
