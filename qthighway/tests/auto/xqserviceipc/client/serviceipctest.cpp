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

#include <QtTest/QtTest>
#include <xqserviceipc.h>
#include "commondefs.h"

using namespace QtService;
// CONSTANTS
const int WAIT_TIME = 50000;     // 5 seconds, must be a multiple of WAIT_INTERVAL
const int WAIT_INTERVAL = 100;  // 100 ms

class QIPCTestThread: public QThread
{
private:
    QSemaphore* iSem;
    QString iName;
    int iCount;
    ServiceFwIPC* iIPC;
public:
    QIPCTestThread( QSemaphore* aSem, QString aName )
        {
        iSem = aSem;
        iName = aName;
        }
    ~QIPCTestThread()
        {
        }

protected:
    void run()
        {
        TServiceIPCBackends backend;
#ifdef __SYMBIAN32__
        backend = ESymbianServer;
#else 
        backend = ELocalSocket;
#endif //__SYMBIAN32__
            
        iIPC = new ServiceFwIPC( backend, NULL );
        iIPC->connect( TEST_SERVER_NAME );
        
        for( int i=0; i<100; ++i )
            {
            // Send 100 echo messages and verify the return
            QString echo="echoechoecho_";
            echo+=iName;
            bool sent = iIPC->sendSync( REQUEST_1, echo.toAscii() );
            if( sent == false )
                {
                qDebug() << "Send error~!";
                }

            // Wait for a reply
            iIPC->waitForRead();
            QString rtn = QString(iIPC->readAll());
            qDebug() << "ECHO Returned: " << rtn;
            if( rtn != echo )
                {
                QFAIL("Thread verification error");
                }
            // sleep a little and send again
            }
        delete iIPC;
        
        iSem->release(1);
        }
};

class QIPClientTest : public QObject
{
Q_OBJECT
    // Member data
    private:
        ServiceFwIPC* iIPC;
        bool connected;
        bool iWaitForDownloadComplete;
        int iTestLength;

    private slots:
        // Automatically run at the start of entire test set
        void initTestCase()
            {
            iIPC = NULL;
            connected = false;
            }   

        // Automatically run at the start of each test case
        //void init();

        // Automatically run at the end of each test case
        //void cleanup();

        // Automatically run at the end of entire test set
        //void cleanupTestCase();

        // ===============================================================
        // Test construction of the IPC client                    
        // ===============================================================
        void testConstruct() 
            {
            TServiceIPCBackends backend;
#ifdef __SYMBIAN32__
            backend = ESymbianServer;
#else 
            backend = ELocalSocket;
#endif //__SYMBIAN32__
            iIPC = new ServiceFwIPC( backend, this);

            if( iIPC == NULL )
                {
                QFAIL("Unable to create IPC object");
                }
            }

        // ===============================================================
        // Test connection to the client              
        // ===============================================================
        void testStartServerAndConnect()
            {
            connected = iIPC->connect( TEST_SERVER_NAME );
            if( !connected )
                {
                // Start the server and sleep 100ms to let it start
                iIPC->startServer( TEST_SERVER_NAME,TEST_SERVER_EXE );
                QTest::qSleep(100);
                connected = iIPC->connect( TEST_SERVER_NAME );

                }
            if( connected == false )
                {
                QFAIL("Connection failure, cannot continue");
                }
            }

        // ===============================================================
        // Test simple IPC       
        // ===============================================================
        void testSimpleIPC()
            {
            if( connected )
                {
                // Send a simple "ECHO" message
                qDebug() << "Test Case 1: ECHO client";
                QString echo="echoechoecho";
                bool sent = iIPC->sendSync( REQUEST_1, echo.toAscii() );
                if( sent == false )
                    {
                    QFAIL("ECHO send failure");
                    }

                // Wait for a reply
                iIPC->waitForRead();
                QString rtn = QString(iIPC->readAll());
                qDebug() << "ECHO Returned: " << rtn;
                if( rtn != echo )
                    {
                    QFAIL("ECHO receive failure");
                    }
                }
            else 
                {
                QFAIL("Cannot run!");
                }
            }

        // ===============================================================
        // Test simple IPC, big buffer of messages 1k characters
        // ===============================================================
        void testSimpleIPC2()
            {
            if( connected )
                {
                // Send a large 1k message
                qDebug() << "Test Case 2: Sending a big message";
                QString bigString="This is going to be a very long string!!"; // 40 characters
                QByteArray ary;
                for( int i=0; i< 25; ++i )
                    {
                    ary.append( bigString.toAscii() );
                    }
                bool sent = iIPC->sendSync( REQUEST_2, ary );
                if( sent == false )
                    {
                    QFAIL("Send large string failure");
                    }

                // Wait for a reply, the reply should be the length of data received
                iIPC->waitForRead();
                QString rtn = QString(iIPC->readAll());
                qDebug() << "ECHO Returned: " << rtn;
                if( rtn != "1000" )
                    {
                    QFAIL("Big String receive failure (1k)");
                    }
                }
            else 
                {
                QFAIL("Cannot run!");
                }
            }

        // ===============================================================
        // Test simple IPC, big buffer of messages 2k characters
        // ===============================================================
        void testSimpleIPC3()
            {
            if( connected )
                {
                // Send a large 1k message
                qDebug() << "Test Case 3: Sending a very big message";
                QString bigString="This is going to be a very long string!!"; // 40 characters
                QByteArray ary;
                for( int i=0; i< 50; ++i )
                    {
                    ary.append( bigString.toAscii() );
                    }
                bool sent = iIPC->sendSync( REQUEST_2, ary );
                if( sent == false )
                    {
                    QFAIL("Send large string failure");
                    }

                // Wait for a reply, the reply should be the length of data received
                iIPC->waitForRead();
                QString rtn = QString(iIPC->readAll());
                qDebug() << "ECHO Returned: " << rtn;
                if( rtn != "2000" )
                    {
                    QFAIL("Big String receive failure (2k)");
                    }
                }
            else 
                {
                QFAIL("Cannot run!");
                }
            }

        // ===============================================================
        // Test simple IPC, big buffer of messages 4k characters
        // ===============================================================
        void testSimpleIPC4()
            {
            if( connected )
                {
                // Send a large 1k message
                qDebug() << "Test Case 4: Sending a very big message";
                QString bigString="This is going to be a very long string!!"; // 40 characters
                QByteArray ary;
                for( int i=0; i< 100; ++i )
                    {
                    ary.append( bigString.toAscii() );
                    }
                bool sent = iIPC->sendSync( REQUEST_2, ary );
                if( sent == false )
                    {
                    QFAIL("Send large string failure");
                    }

                // Wait for a reply, the reply should be the length of data received
                iIPC->waitForRead();
                QString rtn = QString(iIPC->readAll());
                qDebug() << "ECHO Returned: " << rtn;
                if( rtn != "4000" )
                    {
                    QFAIL("Big String receive failure (4k)");
                    }
                }
            else 
                {
                QFAIL("Cannot run!");
                }
            }
        
        // ===============================================================
        // Test simple IPC, big buffer of messages 8k characters
        // ===============================================================
        void testSimpleIPC5()
            {
            if( connected )
                {
                // Send a large 1k message
                qDebug() << "Test Case 5: Sending a very big message";
                QString bigString="This is going to be a very long string!!"; // 40 characters
                QByteArray ary;
                for( int i=0; i< 200; ++i )
                    {
                    ary.append( bigString.toAscii() );
                    }
                bool sent = iIPC->sendSync( REQUEST_2, ary );
                if( sent == false )
                    {
                    QFAIL("Send large string failure");
                    }

                // Wait for a reply, the reply should be the length of data received
                iIPC->waitForRead();
                QString rtn = QString(iIPC->readAll());
                qDebug() << "ECHO Returned: " << rtn;
                if( rtn != "8000" )
                    {
                    QFAIL("Big String receive failure (8k)");
                    }
                }
            else 
                {
                QFAIL("Cannot run!");
                }
            }

        // ===============================================================
        // Test simple IPC, ECHO service with server returning data async
        // ===============================================================
        void testSimpleIPC6()
            {
            // Test ECHO service with async IPC on server side, callback later
            if( connected )
                {
                // Send a simple "ECHO" message
                qDebug() << "Test Case 6: ECHO client async server";
                QString echo="echoechoecho";
                bool sent = iIPC->sendSync( REQUEST_3, echo.toAscii() );
                if( sent == false )
                    {
                    QFAIL("ECHO send failure");
                    }

                // Wait for a reply
                iIPC->waitForRead();
                QString rtn = QString(iIPC->readAll());
                qDebug() << "ECHO Returned: " << rtn;
                if( rtn != echo )
                    {
                    QFAIL("ECHO receive failure");
                    }
                }
            else 
                {
                QFAIL("Cannot run!");
                }
            }

        // ===============================================================
        // Test simple IPC, big buffer of messages 1k/2k/4k/8k characters returned async
        // ===============================================================
        void testSimpleIPC7()
            {
            if( connected )
                {
                // Send a large 1k message
                qDebug() << "Test Case 7: Sending a big message";
                QString bigString="This is going to be a very long string!!"; // 40 characters
                QByteArray ary;
                for( int i=0; i< 25; ++i )
                    {
                    ary.append( bigString.toAscii() );
                    }
                bool sent = iIPC->sendSync( REQUEST_4, ary );
                if( sent == false )
                    {
                    QFAIL("Send large string failure");
                    }

                // Wait for a reply, the reply should be the length of data received
                iIPC->waitForRead();
                QString rtn = QString(iIPC->readAll());
                qDebug() << "ECHO Returned: " << rtn;
                if( rtn != "1000" )
                    {
                    QFAIL("Big String receive failure (1k)");
                    }

                // Reset for 2000 character case
                ary.clear();
                for( int i=0; i< 50; ++i )
                    {
                    ary.append( bigString.toAscii() );
                    }
                sent = iIPC->sendSync( REQUEST_4, ary );
                if( sent == false )
                    {
                    QFAIL("Send large string failure");
                    }

                // Wait for a reply, the reply should be the length of data received
                iIPC->waitForRead();
                rtn = QString(iIPC->readAll());
                qDebug() << "ECHO Returned: " << rtn;
                if( rtn != "2000" )
                    {
                    QFAIL("Big String receive failure (2k)");
                    }

                // Reset for 4000 character case
                ary.clear();
                for( int i=0; i< 100; ++i )
                    {
                    ary.append( bigString.toAscii() );
                    }
                sent = iIPC->sendSync( REQUEST_4, ary );
                if( sent == false )
                    {
                    QFAIL("Send large string failure");
                    }

                // Wait for a reply, the reply should be the length of data received
                iIPC->waitForRead();
                rtn = QString(iIPC->readAll());
                qDebug() << "ECHO Returned: " << rtn;
                if( rtn != "4000" )
                    {
                    QFAIL("Big String receive failure (4k)");
                    }

                // Reset for 4000 character case
                ary.clear();
                for( int i=0; i< 200; ++i )
                    {
                    ary.append( bigString.toAscii() );
                    }
                sent = iIPC->sendSync( REQUEST_4, ary );
                if( sent == false )
                    {
                    QFAIL("Send large string failure");
                    }

                // Wait for a reply, the reply should be the length of data received
                iIPC->waitForRead();
                rtn = QString(iIPC->readAll());
                qDebug() << "ECHO Returned: " << rtn;
                if( rtn != "8000" )
                    {
                    QFAIL("Big String receive failure (8k)");
                    }
                }
            else 
                {
                QFAIL("Cannot run!");
                }
            }

        // ===============================================================
        // Test receiving big buffers on client side sync
        // ===============================================================
        void testSimpleIPC8()
            {
             if( connected )
                {
                // Send a large 1k message
                qDebug() << "Test Case 8: Receiving a very big buffer";
                
                QByteArray ary;
                ary.setNum(1000);
                bool sent = iIPC->sendSync( REQUEST_5, ary );
                if( sent == false )
                    {
                    QFAIL("Sending request failed");
                    }

                // Wait for a reply, the reply should be the length of data received
                iIPC->waitForRead();
                QString rtn = QString(iIPC->readAll());
                if( rtn.length() != 1000 )
                    {
                    QFAIL("Receiving a very long string failed");
                    }
                else
                    {
                    qDebug() << "Test Case 8: Receiving a very big buffer 1000 ok";
                    }
                // Ask for a 2k return buffer
                //
                ary.clear();
                ary.setNum(2000);
                sent = iIPC->sendSync( REQUEST_5, ary );
                if( sent == false )
                    {
                    QFAIL("Sending request failed");
                    }
               
                // Wait for a reply, the reply should be the length of data received
                iIPC->waitForRead();
                rtn = QString(iIPC->readAll());
                if( rtn.length() != 2000 )
                    {
                    QFAIL("Receiving a very long string failed");
                    }
                else
                    {
                    qDebug() << "Test Case 8: Receiving a very big buffer 2000 ok";
                    }
                // Ask for a 2k return buffer
                //
                ary.clear();
                ary.setNum(4000);
                sent = iIPC->sendSync( REQUEST_5, ary );
                if( sent == false )
                    {
                    QFAIL("Sending request failed");
                    }

                // Wait for a reply, the reply should be the length of data received
                iIPC->waitForRead();
                rtn = QString(iIPC->readAll());
                if( rtn.length() != 4000 )
                    {
                    QFAIL("Receiving a very long string failed");
                    }
                 else
                    {
                    qDebug() << "Test Case 8: Receiving a very big buffer 4000 ok";
                    }

                ary.clear();
                ary.setNum(8000);
                sent = iIPC->sendSync( REQUEST_5, ary );
                if( sent == false )
                    {
                    QFAIL("Sending request failed");
                    }

                // Wait for a reply, the reply should be the length of data received
                iIPC->waitForRead();
                rtn = QString(iIPC->readAll());
                if( rtn.length() != 8000 )
                    {
                    QFAIL("Receiving a very long string failed");
                    }
                 else
                    {
                    qDebug() << "Test Case 8: Receiving a very big buffer 8000 ok";
                    }
                }
            else 
                {
                QFAIL("Cannot run!");
                }
            }

        // ===============================================================
        // Test receiving big buffers on client side async
        // ===============================================================
        void testSimpleIPC9()
            {
            if( connected )
                {
                doReceiveAsyncBigBuffer( 1000 );
#ifndef __SYMBIAN32__                
                doReceiveAsyncBigBuffer( 2000 );
                doReceiveAsyncBigBuffer( 4000 );
                doReceiveAsyncBigBuffer( 8000 );
#endif // __SYMBIAN32__               
                }
            else
                {
                QFAIL("Cannot run!");
                }
            }
        
        // ===============================================================
        // Test disconnect and destruction of the IPC client                    
        // ===============================================================
        void testSimpleIPC10()
            {
            // Test multi-threaded client read/write
            QSemaphore sem(0);
            
            QIPCTestThread* thread1 = new QIPCTestThread(&sem,"thread1");
            QIPCTestThread* thread2 = new QIPCTestThread(&sem,"thread2");
            thread1->start();
            thread2->start();
            // Wait until both threads die
            sem.acquire(2);
            delete thread1;
            delete thread2;
            }

        // ===============================================================
        // Test disconnect and destruction of the IPC client                    
        // ===============================================================
        void testDisconnect()
            {
            iIPC->sendSync( KILL_SERVER, "" );
            iIPC->waitForRead();
            iIPC->disconnect();
            QTest::qWait(2500);
            delete iIPC;
            iIPC = NULL;
            }

// Utility functions
private: 
        void WaitForTestComplete()
            {
            int num = WAIT_TIME / WAIT_INTERVAL;
            num*=3;
            iWaitForDownloadComplete = true;
            while ( num-- > 0 && iWaitForDownloadComplete )
                {
                    QTest::qWait(WAIT_INTERVAL);
                }
            if( iWaitForDownloadComplete == true )
                {
                qDebug() << "Timed out";
                }
            QVERIFY2( iWaitForDownloadComplete == false, "Test case timed out" );
            iWaitForDownloadComplete = false;
            }

        void doReceiveAsyncBigBuffer( int aSize )
            {
            qDebug() << "Test Case 9: Receiving a very big buffer of size: " << aSize;\
            iTestLength = aSize;
            QByteArray ary;
            ary.setNum(aSize);
            iIPC->sendAsync( REQUEST_5, ary );
            // Wait for signal
            QObject::connect(iIPC, SIGNAL( readyRead() ), 
                             this, SLOT( handleReadyRead() ) );
            WaitForTestComplete();
            // Cleanup signals
            QObject::disconnect(iIPC, SIGNAL( readyRead() ), 
                                this, SLOT( handleReadyRead() ) );
            }

public slots:
        void handleReadyRead()
            {
            QString rtn = iIPC->readAll();
            if( rtn.length() != iTestLength )
                {
                QFAIL("Receiving a very long string failed");
                }
            else
                {
                qDebug() << "Test Case 9: Receiving a very big buffer " << iTestLength << " ok";
                }
            iWaitForDownloadComplete = false;
            }
};

QTEST_MAIN(QIPClientTest);
#include "serviceipctest.moc"
