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

/*
* Module Test cases for Service Framework IPC 
*/

#include <QtCore>
#include <QCoreApplication>
#include <xqserviceipcobserver.h>
#include <xqserviceipcrequest.h>
#include <xqserviceipcserver.h>
#include "commondefs.h"

#ifdef __SYMBIAN32__
#include <e32base.h>
#endif // __SYMBIAN32__

using namespace QtService;

// CONSTANTS
class QIPCServerTest : public QObject, public MServiceIPCObserver
{
    Q_OBJECT
    public:
        QIPCServerTest::QIPCServerTest() 
            {
            TServiceIPCBackends backend;
#ifdef __SYMBIAN32__
            backend = ESymbianServer;
#else 
            backend = ELocalSocket;
#endif //__SYMBIAN32__
            iServer = new ServiceFwIPCServer( backend, this, this );
            iServer->listen( TEST_SERVER_NAME );
            }

        QIPCServerTest::~QIPCServerTest()
            {
            iServer->disconnect();
            }

    private:
        // ===============================================================
        // From MServiceIPCObserver                  
        // ===============================================================
        bool handleRequest( ServiceIPCRequest* aRequest )
            {
            if( aRequest->getOperation() == REQUEST_1 )
                {
                // Simply echo the data back to the client
                aRequest->write( aRequest->getData() );
                aRequest->completeRequest();
                }
            else if( aRequest->getOperation() == REQUEST_2 )
                {
                int length = aRequest->getData().length();
                QString reply;
                reply.setNum( length );
                aRequest->write( reply.toAscii() );
                aRequest->completeRequest();
                }
            else if( aRequest->getOperation() == REQUEST_3 ||
                     aRequest->getOperation() == REQUEST_4 )
                {
                // Callback in 100ms
                QTimer::singleShot( 100, this, SLOT( doAsyncCallback() ) );
                iAsyncRequest = aRequest;
                }
            else if( aRequest->getOperation() == REQUEST_5 )
                {
                int length = aRequest->getData().toLong();
                
                QByteArray data;
                for( int i=0; i<length; ++i )
                    {
                    data.append( "a" );
                    }
                aRequest->write( data );
                aRequest->completeRequest();
                }
            else if( aRequest->getOperation() == KILL_SERVER )
                {
                aRequest->write( "bye" );
                aRequest->completeRequest();
                QCoreApplication::quit();
                }
            return true;
            }

        // ===============================================================
        // From MServiceIPCObserver         
        // ===============================================================
        void handleCancelRequest( ServiceIPCRequest* /*aRequest*/ )
            {
            }
        
        void handleDeleteRequest( ServiceIPCRequest* /*aRequest*/ )
        {
        }
    
    public slots:

        // ===============================================================
        // From MServiceIPCObserver         
        // ===============================================================
        void doAsyncCallback()
            {
            if( iAsyncRequest ) 
                {
                if( iAsyncRequest->getOperation() == REQUEST_3 )
                    {
                    // Simply echo the data back to the client
                    iAsyncRequest->write( iAsyncRequest->getData() );
                    iAsyncRequest->completeRequest();
                    iAsyncRequest = NULL;
                    }
                else if ( iAsyncRequest->getOperation() == REQUEST_4 )
                    {
                    int length = iAsyncRequest->getData().length();
                    QString reply;
                    reply.setNum( length );
                    iAsyncRequest->write( reply.toAscii() );
                    iAsyncRequest->completeRequest();
                    iAsyncRequest = NULL;
                    }
                }
            }
    private:
        ServiceFwIPCServer* iServer;
        ServiceIPCRequest*  iAsyncRequest;  // not owned
};

#ifdef __SYMBIAN32__
LOCAL_C void StartServerL()
    {
    // Some test code 
    User::LeaveIfError(User::RenameThread(_L("testipcserver")));
    CActiveScheduler* scheduler = new(ELeave)CActiveScheduler;
    CleanupStack::PushL(scheduler);    
    CActiveScheduler::Install(scheduler);
    QIPCServerTest* server = new QIPCServerTest();
    RProcess::Rendezvous(KErrNone);
    
    RProcess proc;
    TInt err = proc.Create(_L("serviceipctest.exe"),_L(""));
    proc.Resume();
    CActiveScheduler::Start();
    CActiveScheduler::Install(NULL);
    CleanupStack::PopAndDestroy(scheduler);
    }
#endif // __SYMBIAN32__

int main(int argc, char *argv[])
{
    // Simple test application
#if 1 
    QCoreApplication a(argc, argv);
    QIPCServerTest* server = new QIPCServerTest();
        
    int rtn = a.exec();
    delete server;
#else // __SYMBIAN32__
    // Symbian currently does not support >1 QT processes on windows, so we run a traditional server
    CTrapCleanup* cleanup=CTrapCleanup::New();
    TInt rtn(KErrNoMemory);
    if (cleanup)
        {
        TRAP(rtn,StartServerL());
        }
        
    delete cleanup;
#endif // __SYMBIAN32__

    return rtn;
}

#include "serviceipcservertest.moc"
