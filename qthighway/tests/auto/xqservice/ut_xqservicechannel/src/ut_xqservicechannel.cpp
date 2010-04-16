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

#include <QtTest>
#include <QThread>

#include "xqservicechannel.h"

#include "ut_xqservicechannel_threadstorage.h"


static QThreadStorage<XQServiceChannelStorage *> testServiceStorage;

XQServiceChannelStorage* testStorage()
{
    if (!testServiceStorage.hasLocalData()) {
        testServiceStorage.setLocalData(new XQServiceChannelStorage());
    }
    
    return testServiceStorage.localData() ;
}

class testChannel  : public XQServiceChannel
{
    Q_OBJECT
public:
    explicit testChannel(const QString& channel, bool isServer, QObject *parent=0);
    virtual ~testChannel();

public slots:
    QVariant receive(const QString& msg, const QByteArray &data);
    
signals:
    void received();

public:
    
public:
    void clear();
    
    QString lastMsg;
    QByteArray lastData;
    int count;

};

testChannel::testChannel(const QString& channel, bool isServer, QObject *parent)
: XQServiceChannel(channel,isServer,parent)
{
}

testChannel::~testChannel()
{
}

QVariant testChannel::receive(const QString& msg, const QByteArray &data)
{
lastMsg = msg;
lastData = data;
count++;
return QVariant();
}

void testChannel::clear()
{
lastMsg.clear();
lastData.clear();
count=0;
}

class ut_XQServiceChannel : public QObject
{
    Q_OBJECT
    
public:
    ut_XQServiceChannel() {}
    ~ut_XQServiceChannel() {}

private slots:
    void initTestCase();
    void init();

    void connectFail();
    
    void sendFail();

    void returnValue();

    void testSendLocally();

    void cleanup();
    void cleanupTestCase();
    
public slots:
    
signals:

public:

private:
};

void ut_XQServiceChannel::initTestCase()
{
}

void ut_XQServiceChannel::cleanupTestCase()
{
}

void ut_XQServiceChannel::init()
{
    testStorage()->resetAll();
}

void ut_XQServiceChannel::cleanup()
{
}

void ut_XQServiceChannel::connectFail()
{
    XQServiceChannel* tstChannel;
    
    QString channel("com.nokia.test.Receiver");
    //fail in connect
    testStorage()->failConnect = true;
    
    tstChannel = new XQServiceChannel(channel,true );
    QCOMPARE(channel,tstChannel->channel());
    QVERIFY(!tstChannel->connectChannel());
    delete tstChannel; 

    //OK
    testStorage()->failConnect = false;
    tstChannel = new XQServiceChannel(channel,true );
    QCOMPARE(channel,tstChannel->channel());
    QVERIFY(tstChannel->connectChannel());
    delete tstChannel; 
}

void ut_XQServiceChannel::sendFail()
{
    QByteArray data;
    QVariant retData;
    QString channel("com.nokia.test.Receiver");
    
    testStorage()->failConnect = true;
    //fail in connection
    QVERIFY(!XQServiceChannel::send(channel,QString("test(QString)"), data, retData,false)); //sync
    
    testStorage()->failConnect = false;
    testStorage()->failSend = true;
    //fail in send
    QVERIFY(!XQServiceChannel::send(channel,QString("test(QString)"), data, retData,false)); //sync

    testStorage()->failSend = false;
    //send ok
    QVERIFY(XQServiceChannel::send(channel,QString("test(QString)"), data, retData,false)); //sync
}

void ut_XQServiceChannel::returnValue()
{
    QString channel("com.nokia.test.Receiver");
//TODO: test return value
    testChannel* tstChannel = new testChannel(channel,true );
    QCOMPARE(channel,tstChannel->channel());
    QVERIFY(tstChannel->connectChannel());

    delete tstChannel; 
}

void ut_XQServiceChannel::testSendLocally()
{
    QString channel("com.nokia.test.Receiver");
    
    testChannel* tstChannel1 = new testChannel(channel+"_1",true );
    QCOMPARE(channel+"_1",tstChannel1->channel());
    QVERIFY(tstChannel1->connectChannel());

    testChannel* tstChannel2 = new testChannel(channel+"_2",true );
    QCOMPARE(channel+"_2",tstChannel2->channel());
    QVERIFY(tstChannel2->connectChannel());

    testChannel* tstChannel3 = new testChannel(channel+"_3",true );
    QCOMPARE(channel+"_3",tstChannel3->channel());
    QVERIFY(tstChannel3->connectChannel());

    QByteArray data(200,'A');
    QVariant retData;
    QString msg("test(QString)");
    tstChannel1->clear();
    tstChannel2->clear();
    tstChannel3->clear();
    QVERIFY(XQServiceChannel::send(channel+"_1",msg, data, retData,false)); //sync
    QCOMPARE(tstChannel1->count,1);
    QCOMPARE(tstChannel1->lastMsg,msg);
    QCOMPARE(tstChannel1->lastData,data);
    QCOMPARE(tstChannel2->count,0);
    QCOMPARE(tstChannel3->count,0);
    tstChannel1->clear();

    QVERIFY(XQServiceChannel::send(channel+"_2",msg, data, retData,false)); //sync
    QCOMPARE(tstChannel2->count,1);
    QCOMPARE(tstChannel2->lastMsg,msg);
    QCOMPARE(tstChannel2->lastData,data);
    QCOMPARE(tstChannel1->count,0);
    QCOMPARE(tstChannel3->count,0);
    tstChannel2->clear();

    QVERIFY(XQServiceChannel::send(channel+"_3",msg, data, retData,false)); //sync
    QCOMPARE(tstChannel3->count,1);
    QCOMPARE(tstChannel3->lastMsg,msg);
    QCOMPARE(tstChannel3->lastData,data);
    QCOMPARE(tstChannel1->count,0);
    QCOMPARE(tstChannel2->count,0);
    tstChannel3->clear();

    delete tstChannel1; 
    delete tstChannel2; 
    delete tstChannel3; 
}

QTEST_MAIN(ut_XQServiceChannel)

#include "ut_xqservicechannel.moc"
