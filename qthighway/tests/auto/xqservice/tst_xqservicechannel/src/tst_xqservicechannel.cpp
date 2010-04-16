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

#include <QtTest/QtTest>
#include <QtCore/qthread.h>
#include <QtCore/qcoreevent.h>
#include "xqservicechannel.h"
#include "xqsharablefile.h"

const QString ConstReturnValue("ReturnValue=OK");

class TestChannel: public XQServiceChannel 
{
    Q_OBJECT

public:
    TestChannel(const QString& ch, QObject *parent = 0) 
        : XQServiceChannel(ch,true,parent),count(0)
        {
        }
    
    ~TestChannel() 
        {
        }
    
    QVariant receive(const QString& msg, const QByteArray &data);
    
signals:
    void received();
      
public:
    QString lastMsg;
    QByteArray lastData;
    int count;
};

QVariant TestChannel::receive(const QString& msg, const QByteArray &data)
{
    lastMsg = msg;
    lastData = data;
    ++count;
    return QVariant(ConstReturnValue);
}

class ServerThread : public QThread
{
    Q_OBJECT

public:
    ServerThread(const QString ch,QObject *parent = 0) : QThread(parent), tstChannel(NULL), channel(ch)
    {
    }
    
    ~ServerThread() 
    {
    }

public slots:

signals:
    void serverThreadStarted();
      
protected:
    void run();

public:
    QString channel;
    TestChannel* tstChannel;
};

void ServerThread::run()
{
    tstChannel = new TestChannel(channel);
    bool ret = tstChannel->connectChannel();
    emit serverThreadStarted();
    exec();
    delete tstChannel;
}

class tst_XQServiceChannel : public QObject, public XQServiceRequestCompletedAsync
{
    Q_OBJECT
public:
    tst_XQServiceChannel() {}
    ~tst_XQServiceChannel() {}

private slots:
    void initTestCase();
    void init();

    void send_data();
    void send();

    void sendLocally_data();
    void sendLocally();

    void cleanup();
    void cleanupTestCase();
    
public slots:
    void signalSeen();

public:
    void requestCompletedAsync(const QVariant &retValue);
    void requestErrorAsync(int err);
    
signals:
    void received();
    void sendLocally(const QString& ch, const QString& msg,
                            const QByteArray &data);    
protected:

private:
    ServerThread* startThreadChannel(const QString &ch);
    void stopThreadChannel(ServerThread* st);

    ServerThread *serverThread;

    QEventLoop *eventLoop;
    bool signalAlreadySeen;
    TestChannel* tstChannelLocally;
    
    void expectSignal(QObject *object, const char *signal);
    bool waitForSignal(int timeout = 1000);
    QVariant lastRetValue ;
};

void tst_XQServiceChannel::initTestCase()
{
    serverThread = startThreadChannel(QLatin1String("TestChannel")) ;
    tstChannelLocally = new TestChannel(QLatin1String("TestChannelLocally"));
    QVERIFY(tstChannelLocally->connectChannel());
    connect(tstChannelLocally, SIGNAL(received()),
            this, SIGNAL(received()));
}

void tst_XQServiceChannel::cleanupTestCase()
{
   stopThreadChannel(serverThread) ;
}

void tst_XQServiceChannel::init()
{
}

void tst_XQServiceChannel::cleanup()
{
}

void tst_XQServiceChannel::send_data()
{
    QString currentChannel;

    if (qstrcmp(QTest::currentTestFunction(), "sendLocally") == 0){
        currentChannel = "TestChannelLocally";
    }
    else {
        currentChannel = "TestChannel";
    }

    QTest::addColumn<QString>("chan");
    QTest::addColumn<QString>("msg");
    QTest::addColumn<QByteArray>("data");

    QTest::newRow("simple") 
        << currentChannel 
        << "message()" 
        << QByteArray();

    QTest::newRow("withargs")
        << currentChannel 
        << "message(QString,int,QByteArray)"
        << QByteArray(32, 'A');

    QTest::newRow("longmessage")
        << currentChannel
        << (QLatin1String("message(QString,int,QByteArray)") + QString(1024, QChar('X')))
        << QByteArray(32, 'A');

    QTest::newRow("longdata-1024")
        << currentChannel 
        << "message(QString,int,QByteArray)"
        << QByteArray(1024, 'A');

    QTest::newRow("longdata-2048")
        << currentChannel 
        << "message(QString,int,QByteArray)"
        << QByteArray(2048, 'A');

    QTest::newRow("longdata-4096")
        << currentChannel 
        << "message(QString,int,QByteArray)"
        << QByteArray(4096, 'A');
    
    QTest::newRow("longdata-64K")
        << currentChannel 
        << "message(QString,int,QByteArray)"
        << QByteArray(65535, 'A');
}

void tst_XQServiceChannel::send()
{
    QFETCH(QString, chan);
    QFETCH(QString, msg);
    QFETCH(QByteArray, data);

    serverThread->tstChannel->count = 0;

    // We expect to see the received() signal if the message is delivered.
    expectSignal(this, SIGNAL(received()));

    QVariant ret; // not used for asyn
    
    // Send the message.
    QVERIFY(XQServiceChannel::send(chan, msg, data, ret, false,this));

    // Wait until the message is delivered or a timeout occurs.
    QVERIFY(waitForSignal());

    // Check that the correct values were delivered.
    QCOMPARE(lastRetValue.toString(), ConstReturnValue);
    QCOMPARE(serverThread->tstChannel->lastMsg, msg);
    QCOMPARE(serverThread->tstChannel->lastData, data);
    QCOMPARE(serverThread->tstChannel->count, 1);
}

void tst_XQServiceChannel::sendLocally_data()
{
    send_data();
}

void tst_XQServiceChannel::sendLocally()
{
    QFETCH(QString, chan);
    QFETCH(QString, msg);
    QFETCH(QByteArray, data);
    
    tstChannelLocally->count = 0;
    
    // Send the message.
    XQSharableFile dummy;
    QVariant ret = XQServiceChannel::sendLocally(chan, msg, data, dummy);
    
    // Check that the correct values were delivered.
    QCOMPARE(ret.toString(), ConstReturnValue);
    QCOMPARE(tstChannelLocally->lastMsg, msg);
    QCOMPARE(tstChannelLocally->lastData, data);
    QCOMPARE(tstChannelLocally->count, 1);

}

void tst_XQServiceChannel::expectSignal(QObject *object, const char *signal)
{
    signalAlreadySeen = false;
    connect(object, signal, this, SLOT(signalSeen()));
}

bool tst_XQServiceChannel::waitForSignal(int timeout)
{
    if (signalAlreadySeen)
        return true;
    QEventLoop loop;
    eventLoop = &loop;
    QTimer::singleShot(timeout, eventLoop, SLOT(quit()));
    loop.exec();
    eventLoop = 0;
    return signalAlreadySeen;
}

void tst_XQServiceChannel::signalSeen()
{
    signalAlreadySeen = true;
    if (eventLoop)
        eventLoop->quit();
}

ServerThread* tst_XQServiceChannel::startThreadChannel(const QString &ch)
{
    ServerThread* st = new ServerThread(ch, this);
    expectSignal(st, SIGNAL(serverThreadStarted()));
    st->start();

    QTest::qVerify(waitForSignal(), "startThreadChannel", "", __FILE__, __LINE__);
    
    disconnect(st, SIGNAL(serverThreadStarted()), this, SLOT(signalSeen()));
    connect(st->tstChannel, SIGNAL(received()),
            this, SIGNAL(received()));

    eventLoop = 0;
    signalAlreadySeen = false;
    return st;
}

void tst_XQServiceChannel::stopThreadChannel(ServerThread* st)
{
    st->quit();
    st->wait();
    delete st;
}

void tst_XQServiceChannel::requestCompletedAsync(const QVariant &retValue)
{
    lastRetValue = retValue;
    emit received();
}

void tst_XQServiceChannel::requestErrorAsync(int err)
{
    //nothing to emit for error here
}

QTEST_MAIN(tst_XQServiceChannel)

#include "tst_xqservicechannel.moc"
