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
#include <QtCore/qmetaobject.h>

#include "xqservicerequest.h"
#include "xqserviceprovider.h"

class testServiceProvider;

class ut_XQServiceProvider : public QObject
{
    Q_OBJECT
public:
    ut_XQServiceProvider() {}
    ~ut_XQServiceProvider() {}

private slots:
    void initTestCase();

    void send_data();
    void send();
    
public slots:

signals:

private:
    testServiceProvider* service;
};

class testServiceProvider : public XQServiceProvider
{
    Q_OBJECT
public:
    testServiceProvider( QObject *parent = 0 );
    ~testServiceProvider();
    
public slots:
    QString test1(const QString& par1);
    QString test2(const QString& par1, int par2);
    QString test3(const QString& par1, int par2, const QByteArray& par3);
    
public:
    void reset();

    QString mMessage;
    QString mPar1;
    int mPar2;
    QByteArray mPar3;
    QString mRetValue;
};

testServiceProvider::testServiceProvider(QObject *parent)
: XQServiceProvider(QLatin1String("com.nokia.services.TestInterface"),parent)
{
publishAll();
}

testServiceProvider::~testServiceProvider()
{
}

void testServiceProvider::reset()
{
mMessage.clear();
mPar1.clear();
mPar2=0;
mPar3.clear();
mRetValue.clear();
}

QString testServiceProvider::test1(const QString& par1)
{
mMessage=QString("test1(QString)");
mRetValue = par1+"-retTest1";
mPar1 = par1;
return mRetValue;
}

QString testServiceProvider::test2(const QString& par1, int par2)
{
mMessage=QString("test2(QString,int)");
mRetValue = par1+"-retTest2";
mPar1 = par1;
mPar2 = par2;
return mRetValue;
}

QString testServiceProvider::test3(const QString& par1, int par2, const QByteArray& par3)
{
mMessage=QString("test3(QString,int,QByteArray)");
mRetValue = par1+"-retTest3";
mPar1 = par1;
mPar2 = par2;
mPar3 = par3;
return mRetValue;
}

void ut_XQServiceProvider::initTestCase()
{
    service = new testServiceProvider(this); 
}

void ut_XQServiceProvider::send_data()
{
    QTest::addColumn<QString>("par1");
    QTest::addColumn<int>("par2");
    QTest::addColumn<QByteArray>("par3");

    QTest::newRow("simple") 
        << QString("PARAM1")
        << 1000
        << QByteArray(32, 'A');

    QTest::newRow("longdata-1024")
        << QString("PARAM1")+QString(1024, QChar('X'))
        << 1000
        << QByteArray(1024, 'A');

    QTest::newRow("longdata-2048")
        << QString("PARAM1")+QString(2048, QChar('X'))
        << 1000
        << QByteArray(2048, 'A');

    QTest::newRow("longdata-4096")
        << QString("PARAM1")+QString(4096, QChar('X'))
        << 1000
        << QByteArray(4096, 'A');
    
    QTest::newRow("longdata-64K")
        << QString("PARAM1")+QString(65535, QChar('X'))
        << 1000
        << QByteArray(65535, 'A');
}

void ut_XQServiceProvider::send()
{
    QFETCH(QString, par1);
    QFETCH(int, par2);
    QFETCH(QByteArray, par3);
    
    QVariant retValue;
    bool res;
    
    QString msg1=QString("test1(QString)");
    XQServiceRequest snd1(QLatin1String("com.nokia.services.TestInterface"),msg1);
    snd1 << par1;
    res = snd1.send(retValue);        
    // Check that the correct values were delivered.
    QCOMPARE(res, true);        
    QCOMPARE(service->mPar1, par1);        
    QCOMPARE(service->mMessage, msg1);        
    QCOMPARE(service->mRetValue, retValue.toString());        


    QString msg2=QString("test2(QString,int)");
    XQServiceRequest snd2(QLatin1String("com.nokia.services.TestInterface"),msg2);
    snd2 << par1;
    snd2 << par2;
    res = snd2.send(retValue);        
    // Check that the correct values were delivered.
    QCOMPARE(res, true);        
    QCOMPARE(service->mPar1, par1);        
    QCOMPARE(service->mPar2, par2);
    QCOMPARE(service->mMessage, msg2);        
    QCOMPARE(service->mRetValue, retValue.toString());        

    QString msg3=QString("test3(QString,int,QByteArray)");
    XQServiceRequest snd3(QLatin1String("com.nokia.services.TestInterface"),msg3);
    snd3 << par1;
    snd3 << par2;
    snd3 << par3;
    res = snd3.send(retValue);        
    // Check that the correct values were delivered.
    QCOMPARE(res, true);        
    QCOMPARE(service->mPar1, par1);        
    QCOMPARE(service->mPar2, par2);
    QCOMPARE(service->mPar3, par3);
    QCOMPARE(service->mMessage, msg3);        
    QCOMPARE(service->mRetValue, retValue.toString());        
}

QTEST_MAIN(ut_XQServiceProvider)

#include "ut_xqserviceprovider.moc"
