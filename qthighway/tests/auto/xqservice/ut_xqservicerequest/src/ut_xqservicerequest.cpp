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
#include <qmetaobject.h>
#include <QVariant>

#include "xqservicerequest.h"
#include "xqserviceprovider.h"

class testServiceProvider;

class ut_XQServiceRequest : public QObject
{
    Q_OBJECT
public:
    ut_XQServiceRequest() {}
    ~ut_XQServiceRequest() {}

private slots:
    void initTestCase();

    void testClassIstantiation();
    void testSerializeArguments();
    
    void testParam1();
    void testParam2();
    void testParam3();
    void testParam4();
    void testParam5();

    void testRetParamBool();
    void testRetParamInt();
    void testRetParamUInt();
    void testRetParamQString();
    void testRetParamQStringList();
    void testRetParamQByteArray();
    void testRetParamQDate();
    void testRetParamQTime();
    void testRetParamQDateTime();
    void testRetParamQUrl();
    void testRetParamQSize();
    void testRetParamQSizeF();
    void testRetParamQPoint();
    void testRetParamQPointF();
    void testRetParamQRect();
    void testRetParamQRectF();
    void testRetParamTestContact();
    void testLatestError();
    
public slots:

signals:

private:
    testServiceProvider* provider;
};

class TestContact
{
    friend bool operator==( const TestContact &c1, const TestContact &c2 );
public:
    
    TestContact();
    ~TestContact();
    void setSurname(const QString&);
    void setFirstname(const QString&);
    void setNumber(const QString&);
    void setDate(const QDate&);
    void setUid(const QUuid&);

    QString surname();
    QString firstname();
    QString number();
    QDate date();
    QUuid uid();

    template <typename Stream> void serialize(Stream &stream) const;
    template <typename Stream> void deserialize(Stream &stream);
   
private:
    QString mSurname;
    QString mFirstname;
    QString mNumber;
    QDate   mBorn;
    QUuid    mUid;
};

inline bool operator== ( const TestContact & c1, const TestContact & c2 )
{
    return (c1.mSurname == c2.mSurname) && 
           (c1.mFirstname == c2.mFirstname) && 
           (c1.mNumber == c2.mNumber) && 
           (c1.mBorn == c2.mBorn) && 
           (c1.mUid == c2.mUid);
}



Q_DECLARE_USER_METATYPE(TestContact)

#define METHOD_TEST1 "test1(bool,int,uint,QString,QStringList,QVariant)"
#define METHOD_TEST2 "test2(QByteArray,QDate,QTime,QDateTime,QUrl)"
#define METHOD_TEST3 "test3(QSize,QSizeF,QPoint,QPointF,QRect,QRectF)"
#define METHOD_TEST4 "test4(TestContact)"
#define METHOD_TEST5 "test5(QString,QString,QString,QString)"
#define SERVICE_TEST "com.nokia.services.TestInterface"

class testServiceProvider : public XQServiceProvider
{
    Q_OBJECT
public:
    testServiceProvider( QObject *parent = 0 );
    ~testServiceProvider();
    
public slots:
    QString test1(bool parBool,
                  int parInt, 
                  uint parUint, 
                  const QString& parQstring, 
                  const QStringList& parQStringList,
                  const QVariant& parVariant);

    QString test2(const QByteArray& parQByteArray,
                  const QDate& parQdate,
                  const QTime& parQTime, 
                  const QDateTime& parQDateTime, 
                  const QUrl& parQUrl);

    QString test3(const QSize& parQSize,
                  const QSizeF& parQSizeF, 
                  const QPoint& parQPoint,
                  const QPointF& parQPointF,
                  const QRect& parQRect, 
                  const QRectF& parQRectF);

    QString test4(const TestContact& parTestContact) ;

    QString test5(
            const QString& par1,
            const QString& par2,
            const QString& par3,
            const QString& par4);

    bool retParamBool();
    int retParamInt();
    uint retParamUInt();
    QString retParamQString(int size);
    QStringList retParamQStringList();
    QByteArray retParamQByteArray(int size);
    QDate retParamQDate();
    QTime retParamQTime();
    QDateTime retParamQDateTime();    
    QUrl retParamQUrl();
    QSize retParamQSize();
    QSizeF retParamQSizeF();
    QPoint retParamQPoint();
    QPointF retParamQPointF();
    QRect retParamQRect();
    QRectF retParamQRectF();

    TestContact retParamTestContact();

public:
    void reset();

    QString mMessage;
    QString mRetValue;

    bool mParBool;
    int mParInt;
    uint mParUint;
    QString mParQString;
    QStringList mParQStringList;
    QVariant mParVariant;

    QString mParQString1;
    QString mParQString2;
    QString mParQString3;
    QString mParQString4;
    
    QByteArray mParQByteArray;
    QDate mParQDate;
    QTime mParQTime; 
    QDateTime mParQDateTime; 
    QUrl mParQUrl;

    QSize mParQSize;
    QSizeF mParQSizeF;
    QPoint mParQPoint;
    QPointF mParQPointF;
    QRect mParQRect;
    QRectF mParQRectF; 
    
    TestContact mParTestContact;
    
};

testServiceProvider::testServiceProvider(QObject *parent)
: XQServiceProvider(QLatin1String(SERVICE_TEST),parent)
{
publishAll();
}

testServiceProvider::~testServiceProvider()
{
}

void testServiceProvider::reset()
{
    mMessage.clear();
    
    mParBool=false;
    mParInt=0;
    mParUint=0;
    mParQString.clear();
    mParQStringList.clear();
    mParVariant.clear();
    
    mRetValue.clear();

    mParQByteArray.clear();
    mParQDate.setDate(0,0,0);
    
}

QString testServiceProvider::test1(
        bool parBool,
        int parInt, 
        uint parUint, 
        const QString& parQstring, 
        const QStringList& parQStringList, 
        const QVariant& parVariant)
{
    mMessage=QString(METHOD_TEST1);
    mRetValue = "retTest1";
    mParBool=parBool;
    mParInt=parInt;
    mParUint=parUint;
    mParQString=parQstring;
    mParQStringList=parQStringList;
    mParVariant=parVariant;
    return mRetValue;
}

QString testServiceProvider::test2(
        const QByteArray& parQByteArray,
        const QDate& parQdate,
        const QTime& parQTime, 
        const QDateTime& parQDateTime, 
        const QUrl& parQUrl)
{
    mMessage=QString(METHOD_TEST2);
    mRetValue = "retTest2";
    mParQByteArray=parQByteArray;
    mParQDate=parQdate;
    mParQTime=parQTime; 
    mParQDateTime=parQDateTime; 
    mParQUrl=parQUrl;
    return mRetValue;
}

QString testServiceProvider::test3(
        const QSize& parQSize,
        const QSizeF& parQSizeF, 
        const QPoint& parQPoint,
        const QPointF& parQPointF,
        const QRect& parQRect, 
        const QRectF& parQRectF)
{
    mMessage=QString(METHOD_TEST3);
    mRetValue = "retTest3";
    mParQSize=parQSize;
    mParQSizeF=parQSizeF;
    mParQPoint=parQPoint;
    mParQPointF=parQPointF;
    mParQRect=parQRect;
    mParQRectF=parQRectF; 
    return mRetValue;
}

QString testServiceProvider::test4(const TestContact& parTestContact)
{
    mMessage=QString(METHOD_TEST4);
    mRetValue = "retTest4";
    mParTestContact=parTestContact;
    return mRetValue;
}

QString testServiceProvider::test5(
        const QString& par1,
        const QString& par2,
        const QString& par3,
        const QString& par4)
{
    mMessage=QString(METHOD_TEST5);
    mRetValue = "retTest5";
    mParQString1=par1;
    mParQString2=par2;
    mParQString3=par3;
    mParQString4=par4;
    return mRetValue;
}

bool testServiceProvider::retParamBool()
{
mParBool = true;
return mParBool;
}

int testServiceProvider::retParamInt()
{
mParInt = -98765;
return mParInt;
}

uint testServiceProvider::retParamUInt()
{
mParUint = 98765;
return mParUint;
}

QString testServiceProvider::retParamQString(int size)
{
mParQString.clear();
mParQString = QString(size,'A');
return mParQString;
}

QStringList testServiceProvider::retParamQStringList()
{
mParQStringList.clear();
for (int i = 0 ; i < 10 ; i++)
    mParQStringList << "string "+QString::number(i); 
return mParQStringList;
}

QByteArray testServiceProvider::retParamQByteArray(int size)
{
mParQByteArray=QByteArray(size,'A');
return mParQByteArray;
}

QDate testServiceProvider::retParamQDate()
{
mParQDate = QDate(2009,12,11);
return mParQDate;
}

QTime testServiceProvider::retParamQTime()
{
mParQTime = QTime(10,9,8);
return mParQTime;
}

QDateTime testServiceProvider::retParamQDateTime()
{
mParQDateTime = QDateTime(mParQDate,mParQTime);
return mParQDateTime;
}

QUrl testServiceProvider::retParamQUrl()
{
mParQUrl = QUrl("http://www.nokia.com");
return mParQUrl;
}

QSize testServiceProvider::retParamQSize()
{
mParQSize = QSize(123,321);
return mParQSize;
}

QSizeF testServiceProvider::retParamQSizeF()
{
mParQSizeF = QSizeF(1234567890.54321,987654321.12345);
return mParQSizeF;
}

QPoint testServiceProvider::retParamQPoint()
{
mParQPoint = QPoint(1234,4321);
return mParQPoint;
}

QPointF testServiceProvider::retParamQPointF()
{
mParQPointF = QPointF(1234567890.54321,987654321.12345);
return mParQPointF;
}

QRect testServiceProvider::retParamQRect()
{
mParQRect = QRect(mParQPoint,mParQSize); 
return mParQRect;
}

QRectF testServiceProvider::retParamQRectF()
{
mParQRectF = QRectF(mParQPointF,mParQSizeF); 
return mParQRectF;
}

TestContact testServiceProvider::retParamTestContact()
{
mParTestContact.setSurname("piro-RetParam");
mParTestContact.setFirstname("stephen-RetParam");
mParTestContact.setNumber(QString("+35850987654321"));
mParTestContact.setDate(QDate(1963,12,17));
mParTestContact.setUid(QUuid::createUuid());
return mParTestContact;
}

void ut_XQServiceRequest::initTestCase()
{
    provider = new testServiceProvider(this); 
}

void ut_XQServiceRequest::testClassIstantiation()
{
    QString service("com.nokia.service.test");
    QString msg("test(QString)");

    {
        XQServiceRequest snd;
        QVERIFY(snd.isNull());
        snd.setService(service);
        snd.setMessage(msg);        
        QVERIFY(!snd.isNull());
        QCOMPARE(snd.service(),service);
        QCOMPARE(snd.message(),msg);
    }

    {
        XQServiceRequest snd(service,msg);
        QVERIFY(!snd.isNull());
        QCOMPARE(snd.service(),service);
        QCOMPARE(snd.message(),msg);
        
        XQServiceRequest snd1(snd);
        QVERIFY(!snd1.isNull());
        QCOMPARE(snd1.service(),service);
        QCOMPARE(snd1.message(),msg);
        
        QCOMPARE(snd,snd1);
    }
}

void ut_XQServiceRequest::testSerializeArguments()
{
    XQServiceRequest snd(QLatin1String(SERVICE_TEST),"test(QString,QDate,QTime)");
    
    snd << QString("test")
        << QDate(2009,1,1)
        << QTime(8,1,2);
    
    QByteArray ret = XQServiceRequest::serializeArguments(snd);

    XQServiceRequest snd1(QLatin1String(SERVICE_TEST),"test(QString,QDate,QTime)");
    XQServiceRequest::deserializeArguments(snd1, ret);


    QCOMPARE(snd,snd1);
}

void ut_XQServiceRequest::testParam1()
{
    QString service(SERVICE_TEST);
    QVariant ret;
    bool par1 = true;
    int par2 = -12345;
    uint par3 = 54321;
    QString par4("Test String");
    QStringList par5;
    par5 << "string 1" << "string 2" << "string 3" ; 
    QVariant par6 = qVariantFromValue(QDate(1963,12,17));

    QString msg(METHOD_TEST1);
    XQServiceRequest snd(service,msg);
    
    snd << par1 << par2 << par3 << par4 << par5 << par6;
    bool res = snd.send(ret);        
    // Check that the correct values were delivered.
    QCOMPARE(res,true);        
    QCOMPARE(provider->mParBool,par1);        
    QCOMPARE(provider->mParInt, par2);
    QCOMPARE(provider->mParUint, par3);
    QCOMPARE(provider->mParQString, par4);
    QCOMPARE(provider->mParQStringList, par5);
    QCOMPARE(provider->mParVariant, par6);
    QCOMPARE(provider->mMessage, msg);        
    QCOMPARE(provider->mRetValue, ret.toString());
}

void ut_XQServiceRequest::testParam2()
{
    QString service(SERVICE_TEST);
    QVariant ret;
    QByteArray par1(1024,'A');
    QDate par2(2009,12,17);
    QTime par3(11,12,13); 
    QDateTime par4(par2,par3); 
    QUrl par5("http://www.nokia.com");

    QString msg(METHOD_TEST2);
    XQServiceRequest snd(service,msg);
    
    snd << par1 << par2 << par3 << par4 << par5;
    bool res = snd.send(ret);        
    // Check that the correct values were delivered.
    QCOMPARE(res,true);        
    QCOMPARE(provider->mParQByteArray,par1);        
    QCOMPARE(provider->mParQDate, par2);
    QCOMPARE(provider->mParQTime, par3);
    QCOMPARE(provider->mParQDateTime, par4);
    QCOMPARE(provider->mParQUrl, par5);
    QCOMPARE(provider->mMessage, msg);        
    QCOMPARE(provider->mRetValue, ret.toString());        
}

void ut_XQServiceRequest::testParam3()
{
    QString service(SERVICE_TEST);
    QVariant ret;
    QSize par1(123,321);
    QSizeF par2(1234567890.54321,987654321.12345);
    QPoint par3(1234,4321);
    QPointF par4(1234567890.54321,987654321.12345);
    QRect par5(par3,par1); 
    QRectF par6(par4,par2);

    QString msg(METHOD_TEST3);
    XQServiceRequest snd(service,msg);
    
    snd << par1 << par2 << par3 << par4 << par5 << par6;
    bool res = snd.send(ret);        
    // Check that the correct values were delivered.
    QCOMPARE(res,true);        
    QCOMPARE(provider->mParQSize,par1);        
    QCOMPARE(provider->mParQSizeF, par2);
    QCOMPARE(provider->mParQPoint, par3);
    QCOMPARE(provider->mParQPointF, par4);
    QCOMPARE(provider->mParQRect, par5);
    QCOMPARE(provider->mParQRectF, par6);
    QCOMPARE(provider->mMessage, msg);        
    QCOMPARE(provider->mRetValue, ret.toString());        
}

void ut_XQServiceRequest::testParam4()
{
    QString service(SERVICE_TEST);
    QVariant ret;
    TestContact par1;
    par1.setSurname("piro");
    par1.setFirstname("stephen");
    par1.setNumber(QString("+35850987654321"));
    par1.setDate(QDate(1963,12,17));
    par1.setUid(QUuid::createUuid());
    
    QString msg(METHOD_TEST4);
    XQServiceRequest snd(service,msg);
    
    snd << par1 ;
    bool res = snd.send(ret);        
    // Check that the correct values were delivered.
    QCOMPARE(res,true);        
    QCOMPARE(provider->mParTestContact,par1);        
    QCOMPARE(provider->mMessage, msg);        
    QCOMPARE(provider->mRetValue, ret.toString());        
}

void ut_XQServiceRequest::testParam5()
{
    QString service(SERVICE_TEST);
    QVariant ret;
    QString par1 = "parameter 1";
    QString par2 = "parameter 2";
    QString par3 = "parameter 3";
    QString par4 = "parameter 4";
    
    QString msg(METHOD_TEST5);
    XQServiceRequest snd(service,msg);
    
    snd << par1 << par2 << par3 << par4;
    bool res = snd.send(ret);        
    // Check that the correct values were delivered.
    QCOMPARE(res,true);        
    QCOMPARE(provider->mParQString1,par1);        
    QCOMPARE(provider->mParQString2,par2);        
    QCOMPARE(provider->mParQString3,par3);        
    QCOMPARE(provider->mParQString4,par4);        
    QCOMPARE(provider->mMessage, msg);        
    QCOMPARE(provider->mRetValue, ret.toString());        
}

void ut_XQServiceRequest::testRetParamBool()
{
    XQServiceRequest snd(SERVICE_TEST,"retParamBool()");
    bool ret;
    bool res = snd.send(ret);        
    QCOMPARE(res,true);        
    QCOMPARE(provider->mParBool, ret);        
}

void ut_XQServiceRequest::testRetParamInt()
{
    XQServiceRequest snd(SERVICE_TEST,"retParamInt()");
    int ret;
    bool res = snd.send(ret);        
    QCOMPARE(res,true);        
    QCOMPARE(provider->mParInt, ret);        
}

void ut_XQServiceRequest::testRetParamUInt()
{
    XQServiceRequest snd(SERVICE_TEST,"retParamUInt()");
    uint ret;
    bool res = snd.send(ret);        
    QCOMPARE(res,true);        
    QCOMPARE(provider->mParUint, ret);        
}

void ut_XQServiceRequest::testRetParamQString()
{
    XQServiceRequest snd(SERVICE_TEST,"retParamQString(int)");
    QList<int> testSizes;
    testSizes << 100 << 1024 << 2048 << 4096 ;
    foreach(int size,testSizes) {
        QString ret;
        snd.setMessage("retParamQString(int)");
        snd << size ;
        bool res = snd.send(ret);        
        QCOMPARE(res,true);        
        QCOMPARE(provider->mParQString, ret);
    }
}

void ut_XQServiceRequest::testRetParamQStringList()
{
    XQServiceRequest snd(SERVICE_TEST,"retParamQStringList()");
    QStringList ret;
    bool res = snd.send(ret);        
    QCOMPARE(res,true);        
    QCOMPARE(provider->mParQStringList, ret);        
}

void ut_XQServiceRequest::testRetParamQByteArray()
{
    XQServiceRequest snd(SERVICE_TEST,"retParamQByteArray(int)");
    QList<int> testSizes;
    testSizes << 100 << 1024 << 2048 << 4096 ;
    foreach(int size,testSizes) {
        QByteArray ret;
        snd.setMessage("retParamQByteArray(int)");
        snd << size ;
        bool res = snd.send(ret);        
        QCOMPARE(res,true);        
        QCOMPARE(provider->mParQByteArray, ret);        
    }

}

void ut_XQServiceRequest::testRetParamQDate()
{
    XQServiceRequest snd(SERVICE_TEST,"retParamQDate()");
    QDate ret;
    bool res = snd.send(ret);        
    QCOMPARE(res,true);        
    QCOMPARE(provider->mParQDate, ret);        
}

void ut_XQServiceRequest::testRetParamQTime()
{
    XQServiceRequest snd(SERVICE_TEST,"retParamQTime()");
    QTime ret;
    bool res = snd.send(ret);        
    QCOMPARE(res,true);        
    QCOMPARE(provider->mParQTime, ret);        
}

void ut_XQServiceRequest::testRetParamQDateTime()
{
    XQServiceRequest snd(SERVICE_TEST,"retParamQDateTime()");
    QDateTime ret;
    bool res = snd.send(ret);        
    QCOMPARE(res,true);        
    QCOMPARE(provider->mParQDateTime, ret);        
}

void ut_XQServiceRequest::testRetParamQUrl()
{
    XQServiceRequest snd(SERVICE_TEST,"retParamQUrl()");
    QUrl ret;
    bool res = snd.send(ret);        
    QCOMPARE(res,true);        
    QCOMPARE(provider->mParQUrl, ret);        
}

void ut_XQServiceRequest::testRetParamQSize()
{
    XQServiceRequest snd(SERVICE_TEST,"retParamQSize()");
    QSize ret;
    bool res = snd.send(ret);        
    QCOMPARE(res,true);        
    QCOMPARE(provider->mParQSize, ret);        
}

void ut_XQServiceRequest::testRetParamQSizeF()
{
    XQServiceRequest snd(SERVICE_TEST,"retParamQSizeF()");
    QSizeF ret;
    bool res = snd.send(ret);        
    QCOMPARE(res,true);        
    QCOMPARE(provider->mParQSizeF, ret);        
}

void ut_XQServiceRequest::testRetParamQPoint()
{
    XQServiceRequest snd(SERVICE_TEST,"retParamQPoint()");
    QPoint ret;
    bool res = snd.send(ret);        
    QCOMPARE(res,true);        
    QCOMPARE(provider->mParQPoint, ret);        
}

void ut_XQServiceRequest::testRetParamQPointF()
{
    XQServiceRequest snd(SERVICE_TEST,"retParamQPointF()");
    QPointF ret;
    bool res = snd.send(ret);        
    QCOMPARE(res,true);        
    QCOMPARE(provider->mParQPointF, ret);        
}

void ut_XQServiceRequest::testRetParamQRect()
{
    XQServiceRequest snd(SERVICE_TEST,"retParamQRect()");
    QRect ret;
    bool res = snd.send(ret);        
    QCOMPARE(res,true);        
    QCOMPARE(provider->mParQRect, ret);        
}

void ut_XQServiceRequest::testRetParamQRectF()
{
    XQServiceRequest snd(SERVICE_TEST,"retParamQRectF()");
    QRectF ret;
    bool res = snd.send(ret);        
    QCOMPARE(res,true);        
    QCOMPARE(provider->mParQRectF, ret);        
}

void ut_XQServiceRequest::testRetParamTestContact()
{
    XQServiceRequest snd(SERVICE_TEST,"retParamTestContact()");
    TestContact ret;
    bool res = snd.send(ret);        
    QCOMPARE(res,true);        
    QCOMPARE(provider->mParTestContact,ret);        
}

void ut_XQServiceRequest::testLatestError()
{
    QString service(SERVICE_TEST);
    QString msg(METHOD_TEST1);
    XQServiceRequest snd(service,msg);
    int error = snd.latestError();    
    QCOMPARE(error,-4998);    
}


TestContact::TestContact()
{
}

TestContact::~TestContact()
{
}

void TestContact::setSurname(const QString& surname)
{
mSurname = surname;
}

void TestContact::setFirstname(const QString& name)
{
mFirstname = name;
}

void TestContact::setNumber(const QString& number)
{
mNumber = number;
}

void TestContact::setDate(const QDate& date)
{
mBorn = date;
}

void TestContact::setUid(const QUuid& uid)
{
mUid = uid;
}


QString TestContact::surname()
{
return mSurname;
}

QString TestContact::firstname()
{
return mFirstname;
}

QString TestContact::number()
{
return mNumber;
}

QDate TestContact::date()
{
return mBorn;
}

QUuid TestContact::uid()
{
return mUid;
}

Q_IMPLEMENT_USER_METATYPE(TestContact)


template <typename Stream> void TestContact::serialize(Stream &s) const
{
    s << mSurname;
    s << mFirstname;
    s << mNumber;
    s << mBorn;
    s << mUid;
}

template <typename Stream> void TestContact::deserialize(Stream &s)
{
    s >> mSurname;
    s >> mFirstname;
    s >> mNumber;
    s >> mBorn;
    s >> mUid;
}
        
QTEST_MAIN(ut_XQServiceRequest)

#include "ut_xqservicerequest.moc"
