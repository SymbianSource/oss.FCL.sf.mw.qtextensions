/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/

#include <QtTest/QtTest>
#include <QEventLoop>
#include <QDebug>
#include <QtCore>
#include "mytestwindowgroup.h"
#include <W32STD.H>
#include <xqkeycapture.h>

#include <QFile>

class TestXQKeyCapture : public QObject
{
    Q_OBJECT

public:
    TestXQKeyCapture(): QObject(), keyCapture(0) {};


private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    
    void testCreateAndDestroy();

    void testCaptureKey_data();
    void testCaptureKey();

    void testCaptureKeyList_data();
    void testCaptureKeyList();

	void testCaptureKey_S60_data();
	void testCaptureKey_S60();

    void testCaptureKeyList_S60_data();
	void testCaptureKeyList_S60();

    void testCaptureKeyUpAndDowns_data();
    void testCaptureKeyUpAndDowns();

    void testCaptureKeyUpAndDownsList_data();
    void testCaptureKeyUpAndDownsList();

    void testCaptureKeyUpAndDowns_S60_data();
    void testCaptureKeyUpAndDowns_S60();

    void testCaptureKeyUpAndDownsList_S60_data();
    void testCaptureKeyUpAndDownsList_S60();

    void testCaptureLongKey_data();
    void testCaptureLongKey();

    void testCaptureLongKeyList_data();
    void testCaptureLongKeyList();

    void testCaptureLongKey_S60_data();
    void testCaptureLongKey_S60();

    void testCaptureLongKeyList_S60_data();
    void testCaptureLongKeyList_S60();

    void testCancelCaptureKey_data();
    void testCancelCaptureKey();

    void testCancelCaptureKeyList_data();
    void testCancelCaptureKeyList();

    void testCancelCaptureKey_S60_data();
    void testCancelCaptureKey_S60();

    void testCancelCaptureKeyList_S60_data();
    void testCancelCaptureKeyList_S60();

    void testCancelCaptureKeyUpAndDowns_data();
    void testCancelCaptureKeyUpAndDowns();

    void testCancelCaptureKeyUpAndDownsList_data();
    void testCancelCaptureKeyUpAndDownsList();

    void testCancelCaptureKeyUpAndDowns_S60_data();
    void testCancelCaptureKeyUpAndDowns_S60();

    void testCancelCaptureKeyUpAndDownsList_S60_data();
    void testCancelCaptureKeyUpAndDownsList_S60();

    void testCancelCaptureLongKey_data();
    void testCancelCaptureLongKey();

    void testCancelCaptureLongKeyList_data();
    void testCancelCaptureLongKeyList();

    void testCancelCaptureLongKey_S60_data();
    void testCancelCaptureLongKey_S60();

    void testCancelCaptureLongKeyList_S60_data();
    void testCancelCaptureLongKeyList_S60();

    void testErrorString();
    void testErrorId();
	
	void testKeyMapperFile();

private:
	QString clearString(const QString& line);
	QString clearString(const QString& line, const QString& prefix, const QString& comment);
    
private:
    XQKeyCapture* keyCapture;

    Qt::Key aKey;
    Qt::KeyboardModifiers aModifiersMap;
    Qt::KeyboardModifiers aModifier;
    
    int numOfArgs;
    WindowGroupActionType actionType;
    QList<unsigned int> results;
    QList<unsigned int> additionalResults;
    
    QList<long int> cancelResults;
    QList<long int> cancelAdditionalResults;
    
    long int additionalRequestHandle;
    
    bool willBeAdditionalRequest;
    bool additionalResult;
    bool ignoreWindowGroupAction;
    
    
public slots:
    void windowGroupAction( WindowGroupActionType, QList<unsigned int> );
    void windowGroupActionCancel( WindowGroupActionType, QList<long int> );
    
};

// ======== MEMBER FUNCTIONS ========
void TestXQKeyCapture::initTestCase()
{
    bool ret = connect( MyTestWindowGroup::Instance(), SIGNAL( windowGroupAction( WindowGroupActionType, QList<unsigned int> )),
            this, SLOT( windowGroupAction( WindowGroupActionType, QList<unsigned int> ) ) );
    QVERIFY( ret );
    ret = connect( MyTestWindowGroup::Instance(), SIGNAL( windowGroupActionCancel( WindowGroupActionType, QList<long int> )),
            this, SLOT( windowGroupActionCancel( WindowGroupActionType, QList<long int> ) ) );
    QVERIFY( ret );
    
}

void TestXQKeyCapture::cleanupTestCase()
{
    //delete MyTestWindowGroup::Instance();
}

void TestXQKeyCapture::init()
{
    keyCapture = new XQKeyCapture();
}

void TestXQKeyCapture::cleanup()
{
    delete keyCapture;
}

void TestXQKeyCapture::testCreateAndDestroy()
{
    //empty, all done in init and cleanup
}

////////////////////////////////////////////////////////////////
// CAPTURE
//Capture Key
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCaptureKey_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");

    QTest::addColumn<unsigned int>("symbianKey");
    QTest::addColumn<unsigned int>("symbianMask");
    QTest::addColumn<unsigned int>("symbianModifier");
        
    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<unsigned int>("additionalSymbianKey");
    
    QTest::newRow("esc_key") << static_cast<unsigned int> ( Qt::Key_Escape ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier ) 
                            << static_cast<unsigned int> ( EKeyEscape )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<long int> ( 12 )
                            << false
                            << static_cast<unsigned int> ( 0 );

    QTest::newRow("esc_key_not_supported") << static_cast<unsigned int> ( Qt::Key_Escape ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier ) 
                            << static_cast<unsigned int> ( EKeyEscape )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<long int> ( KErrNotSupported )
                            << false
                            << static_cast<unsigned int> ( 0 );
    
    QTest::newRow("esc_key_modifiers") << static_cast<unsigned int> ( Qt::Key_Escape ) 
                            << static_cast<unsigned int> ( Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt:: KeypadModifier )
                            << static_cast<unsigned int> ( Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt:: KeypadModifier ) 
                            << static_cast<unsigned int> ( EKeyEscape )
                            << static_cast<unsigned int> ( EModifierShift | EModifierCtrl | EModifierAlt | EModifierKeypad )
                            << static_cast<unsigned int> ( EModifierShift | EModifierCtrl | EModifierAlt | EModifierKeypad )
                            << static_cast<long int> ( 13 )
                            << false
                            << static_cast<unsigned int> ( 0 );
    
    QTest::newRow("shift_key") << static_cast<unsigned int> ( Qt::Key_Shift ) 
                            << static_cast<unsigned int> ( Qt::ShiftModifier )
                            << static_cast<unsigned int> ( Qt::ShiftModifier ) 
                            << static_cast<unsigned int> ( EKeyLeftShift )
                            << static_cast<unsigned int> ( EModifierShift )
                            << static_cast<unsigned int> ( EModifierShift )
                            << static_cast<long int> ( 12 )
                            << true
                            << static_cast<unsigned int> ( EKeyRightShift );

    QTest::newRow("control_key") << static_cast<unsigned int> ( Qt::Key_Control ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier ) 
                            << static_cast<unsigned int> ( EKeyLeftCtrl )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<long int> ( 12 )
                            << true
                            << static_cast<unsigned int> ( EKeyRightCtrl );
                            
    QTest::newRow("meta_key") << static_cast<unsigned int> ( Qt::Key_Super_R ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier ) 
                            << static_cast<unsigned int> ( EKeyRightFunc )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<long int> ( 12 )
                            << true
                            << static_cast<unsigned int> ( EKeyRightFunc );


}

void TestXQKeyCapture::testCaptureKey()
{
    numOfArgs = 3;
    actionType = WGATCaptureKey;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianMask << symbianModifier;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);

    keyCapture->captureKey( static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );

//    keyCapture->captureKey( QList<Qt::Key>() << static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
}

void TestXQKeyCapture::testCaptureKeyList_data() 
{
    testCaptureKey_data();
}

void TestXQKeyCapture::testCaptureKeyList()
{
    numOfArgs = 3;
    actionType = WGATCaptureKey;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianMask << symbianModifier;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);

    keyCapture->captureKey( QList<Qt::Key>() << static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
}

////////////////////////////////////////////////////////////////
//Capture Key Up And Downs
////////////////////////////////////////////////////////////////

void TestXQKeyCapture::testCaptureKey_S60_data()
{
    QTest::addColumn<unsigned int>("s60Key");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");

    QTest::addColumn<unsigned int>("symbianKey");
    QTest::addColumn<unsigned int>("symbianMask");
    QTest::addColumn<unsigned int>("symbianModifier");
        
    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<unsigned int>("additionalSymbianKey");
    
    QTest::newRow("esc_key") << static_cast<unsigned int> ( EKeyEscape ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier ) 
                            << static_cast<unsigned int> ( EKeyEscape )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<long int> ( 12 )
                            << false
                            << static_cast<unsigned int> ( 0 );

    QTest::newRow("esc_key_not_supported") << static_cast<unsigned int> ( EKeyEscape ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier ) 
                            << static_cast<unsigned int> ( EKeyEscape )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<long int> ( KErrNotSupported )
                            << false
                            << static_cast<unsigned int> ( 0 );
    
    QTest::newRow("esc_key_modifiers") << static_cast<unsigned int> ( EKeyEscape ) 
                            << static_cast<unsigned int> ( Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt:: KeypadModifier )
                            << static_cast<unsigned int> ( Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt:: KeypadModifier ) 
                            << static_cast<unsigned int> ( EKeyEscape )
                            << static_cast<unsigned int> ( EModifierShift | EModifierCtrl | EModifierAlt | EModifierKeypad )
                            << static_cast<unsigned int> ( EModifierShift | EModifierCtrl | EModifierAlt | EModifierKeypad )
                            << static_cast<long int> ( 13 )
                            << false
                            << static_cast<unsigned int> ( 0 );
    
    QTest::newRow("shift_key") << static_cast<unsigned int> ( EKeyLeftShift ) 
                            << static_cast<unsigned int> ( Qt::ShiftModifier )
                            << static_cast<unsigned int> ( Qt::ShiftModifier ) 
                            << static_cast<unsigned int> ( EKeyLeftShift )
                            << static_cast<unsigned int> ( EModifierShift )
                            << static_cast<unsigned int> ( EModifierShift )
                            << static_cast<long int> ( 12 )
                            << true
                            << static_cast<unsigned int> ( EKeyRightShift );

    QTest::newRow("control_key") << static_cast<unsigned int> ( EKeyRightCtrl ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier ) 
                            << static_cast<unsigned int> ( EKeyRightCtrl )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<long int> ( 12 )
                            << true
                            << static_cast<unsigned int> ( EKeyRightFunc );
                            
    QTest::newRow("meta_key") << static_cast<unsigned int> ( EKeyLeftCtrl ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier ) 
                            << static_cast<unsigned int> ( EKeyLeftCtrl )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<long int> ( 12 )
                            << true
                            << static_cast<unsigned int> ( EKeyLeftFunc );


}

void TestXQKeyCapture::testCaptureKey_S60()
{
    numOfArgs = 3;
    actionType = WGATCaptureKey;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, s60Key);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianMask << symbianModifier;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    keyCapture->captureKey( static_cast<TUint> (s60Key), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
}

void TestXQKeyCapture::testCaptureKeyList_S60_data()
{
    testCaptureKey_S60_data();
}

void TestXQKeyCapture::testCaptureKeyList_S60()
{
    numOfArgs = 3;
    actionType = WGATCaptureKey;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, s60Key);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianMask << symbianModifier;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    keyCapture->captureKey(QList<TUint>() << static_cast<TUint> (s60Key), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
}

////////////////////////////////////////////////////////////////
//Capture Key Up And Downs
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCaptureKeyUpAndDowns_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");

    QTest::addColumn<unsigned int>("symbianKey");
    QTest::addColumn<unsigned int>("symbianMask");
    QTest::addColumn<unsigned int>("symbianModifier");
        
    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<unsigned int>("additionalSymbianKey");
    
    QTest::newRow("esc_key") << static_cast<unsigned int> ( Qt::Key_Escape ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier ) 
                            << static_cast<unsigned int> ( EStdKeyEscape )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<long int> ( 12 )
                            << false
                            << static_cast<unsigned int> ( 0 );
}

void TestXQKeyCapture::testCaptureKeyUpAndDowns()
{
    numOfArgs = 3;
    actionType = WGATCaptureKeyUpAndDowns;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianMask << symbianModifier;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    keyCapture->captureKeyUpAndDowns( static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
}

void TestXQKeyCapture::testCaptureKeyUpAndDownsList_data()
{
    testCaptureKeyUpAndDowns_data();
}

void TestXQKeyCapture::testCaptureKeyUpAndDownsList()
{
    numOfArgs = 3;
    actionType = WGATCaptureKeyUpAndDowns;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianMask << symbianModifier;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    keyCapture->captureKeyUpAndDowns( QList<Qt::Key>() << static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
}

////////////////////////////////////////////////////////////////
//Capture Key Up And Downs
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCaptureKeyUpAndDowns_S60_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");

    QTest::addColumn<unsigned int>("symbianKey");
    QTest::addColumn<unsigned int>("symbianMask");
    QTest::addColumn<unsigned int>("symbianModifier");
        
    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<unsigned int>("additionalSymbianKey");
    
    QTest::newRow("esc_key") << static_cast<unsigned int> ( EStdKeyEscape ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier ) 
                            << static_cast<unsigned int> ( EStdKeyEscape )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<long int> ( 12 )
                            << false
                            << static_cast<unsigned int> ( 0 );
}

void TestXQKeyCapture::testCaptureKeyUpAndDowns_S60()
{
    numOfArgs = 3;
    actionType = WGATCaptureKeyUpAndDowns;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianMask << symbianModifier;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    keyCapture->captureKeyUpAndDowns( static_cast<TUint> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
}

void TestXQKeyCapture::testCaptureKeyUpAndDownsList_S60_data()
{
    testCaptureKeyUpAndDowns_S60_data();
}

void TestXQKeyCapture::testCaptureKeyUpAndDownsList_S60()
{
    numOfArgs = 3;
    actionType = WGATCaptureKeyUpAndDowns;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianMask << symbianModifier;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    keyCapture->captureKeyUpAndDowns( QList<TUint>() << static_cast<TUint> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
}

////////////////////////////////////////////////////////////////
//Capture Long Key
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCaptureLongKey_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");
    QTest::addColumn<int>("longFlags");
    
    QTest::addColumn<unsigned int>("symbianKey");
    QTest::addColumn<unsigned int>("symbianMask");
    QTest::addColumn<unsigned int>("symbianModifier");
    QTest::addColumn<int>("symbianPriority");
    QTest::addColumn<int>("symbianLongFlags");
        
    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<unsigned int>("additionalSymbianKey");
    
    
    QTest::newRow("esc_key") << static_cast<unsigned int> ( Qt::Key_Escape ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier ) 
                            << static_cast<int> ( XQKeyCapture::LongNormal ) 
                            << static_cast<unsigned int> ( EKeyEscape )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<int> ( 0 ) //priority
                            << static_cast<int> ( XQKeyCapture::LongNormal ) 
                            << static_cast<long int> ( 12 )
                            << false
                            << static_cast<unsigned int> ( 0 );

}

void TestXQKeyCapture::testCaptureLongKey()
{
    numOfArgs = 6;
    actionType = WGATCaptureLongKey;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);
    QFETCH(int, longFlags);
    
    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);
    QFETCH(int, symbianPriority);
    QFETCH(int, symbianLongFlags);
    
    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianKey << symbianMask << symbianModifier << symbianPriority << symbianLongFlags;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier << symbianPriority << symbianLongFlags;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    keyCapture->captureLongKey( static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ), static_cast<XQKeyCapture::LongFlags> (longFlags) );
}

void TestXQKeyCapture::testCaptureLongKeyList_data()
{
    testCaptureLongKey_data();
}

void TestXQKeyCapture::testCaptureLongKeyList()
{
    numOfArgs = 6;
    actionType = WGATCaptureLongKey;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);
    QFETCH(int, longFlags);
    
    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);
    QFETCH(int, symbianPriority);
    QFETCH(int, symbianLongFlags);
    
    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianKey << symbianMask << symbianModifier << symbianPriority << symbianLongFlags;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier << symbianPriority << symbianLongFlags;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    keyCapture->captureLongKey( QList<Qt::Key>() << static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ), static_cast<XQKeyCapture::LongFlags> (longFlags) );
}

////////////////////////////////////////////////////////////////
//Capture Long Key
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCaptureLongKey_S60_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");
    QTest::addColumn<int>("longFlags");
    
    QTest::addColumn<unsigned int>("symbianKey");
    QTest::addColumn<unsigned int>("symbianMask");
    QTest::addColumn<unsigned int>("symbianModifier");
    QTest::addColumn<int>("symbianPriority");
    QTest::addColumn<int>("symbianLongFlags");
        
    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<unsigned int>("additionalSymbianKey");
    
    
    QTest::newRow("esc_key") << static_cast<unsigned int> ( EKeyEscape ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier ) 
                            << static_cast<int> ( XQKeyCapture::LongNormal ) 
                            << static_cast<unsigned int> ( EKeyEscape )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<int> ( 0 ) //priority
                            << static_cast<int> ( XQKeyCapture::LongNormal ) 
                            << static_cast<long int> ( 12 )
                            << false
                            << static_cast<unsigned int> ( 0 );

}

void TestXQKeyCapture::testCaptureLongKey_S60()
{
    numOfArgs = 6;
    actionType = WGATCaptureLongKey;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);
    QFETCH(int, longFlags);
    
    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);
    QFETCH(int, symbianPriority);
    QFETCH(int, symbianLongFlags);
    
    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianKey << symbianMask << symbianModifier << symbianPriority << symbianLongFlags;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier << symbianPriority << symbianLongFlags;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    keyCapture->captureLongKey( static_cast<TUint> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ), static_cast<XQKeyCapture::LongFlags> (longFlags) );
}

void TestXQKeyCapture::testCaptureLongKeyList_S60_data()
{
    testCaptureLongKey_S60_data();
}

void TestXQKeyCapture::testCaptureLongKeyList_S60()
{
    numOfArgs = 6;
    actionType = WGATCaptureLongKey;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);
    QFETCH(int, longFlags);
    
    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);
    QFETCH(int, symbianPriority);
    QFETCH(int, symbianLongFlags);
    
    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianKey << symbianMask << symbianModifier << symbianPriority << symbianLongFlags;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier << symbianPriority << symbianLongFlags;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    keyCapture->captureLongKey( QList<TUint>() << static_cast<TUint> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ), static_cast<XQKeyCapture::LongFlags> (longFlags) );
}

////////////////////////////////////////////////////////////////
// CANCEL
//Cancel Capture Key
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCancelCaptureKey_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");

    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<long int>("additionalReqNum");
    
    QTest::newRow("esc_key") << static_cast<unsigned int> ( Qt::Key_Escape ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier ) 
                            << static_cast<long int> ( 12 )
                            << false
                            << static_cast<long int> ( 0 );

    QTest::newRow("esc_key_not_supported") << static_cast<unsigned int> ( Qt::Key_Escape ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier ) 
                            << static_cast<long int> ( KErrNotSupported )
                            << false
                            << static_cast<long int> ( 0 );
    
    QTest::newRow("esc_key_modifiers") << static_cast<unsigned int> ( Qt::Key_Escape ) 
                            << static_cast<unsigned int> ( Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt:: KeypadModifier )
                            << static_cast<unsigned int> ( Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt:: KeypadModifier ) 
                            << static_cast<long int> ( 13 )
                            << false
                            << static_cast<long int> ( 0 );
    
    QTest::newRow("shift_key") << static_cast<unsigned int> ( Qt::Key_Shift ) 
                            << static_cast<unsigned int> ( Qt::ShiftModifier )
                            << static_cast<unsigned int> ( Qt::ControlModifier ) 
                            << static_cast<long int> ( 15 )
                            << true
                            << static_cast<long int> ( 16 );

    QTest::newRow("control_key") << static_cast<unsigned int> ( Qt::Key_Control ) 
                            << static_cast<unsigned int> ( Qt::ControlModifier )
                            << static_cast<unsigned int> ( Qt::ShiftModifier ) 
                            << static_cast<long int> ( 17 )
                            << true
                            << static_cast<long int> ( 18 );
                            
    QTest::newRow("meta_key") << static_cast<unsigned int> ( Qt::Key_Meta ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier ) 
                            << static_cast<long int> ( 19 )
                            << true
                            << static_cast<long int> ( 20 );


}

void TestXQKeyCapture::testCancelCaptureKey()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureKey;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureKey( static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureKey( static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
}

void TestXQKeyCapture::testCancelCaptureKeyList_data()
{
    testCancelCaptureKey_data();
}

void TestXQKeyCapture::testCancelCaptureKeyList()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureKey;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureKey( static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureKey( QList<Qt::Key>() << static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
}

////////////////////////////////////////////////////////////////
// CANCEL
//Cancel Capture Key
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCancelCaptureKey_S60_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");

    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<long int>("additionalReqNum");
    
    QTest::newRow("esc_key") << static_cast<unsigned int> ( EKeyEscape ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier ) 
                            << static_cast<long int> ( 12 )
                            << false
                            << static_cast<long int> ( 0 );

    QTest::newRow("esc_key_not_supported") << static_cast<unsigned int> ( EKeyEscape ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier ) 
                            << static_cast<long int> ( KErrNotSupported )
                            << false
                            << static_cast<long int> ( 0 );
    
    QTest::newRow("esc_key_modifiers") << static_cast<unsigned int> ( EKeyEscape ) 
                            << static_cast<unsigned int> ( Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt:: KeypadModifier )
                            << static_cast<unsigned int> ( Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt:: KeypadModifier ) 
                            << static_cast<long int> ( 13 )
                            << false
                            << static_cast<long int> ( 0 );
    
    QTest::newRow("shift_key") << static_cast<unsigned int> ( EKeyLeftShift ) 
                            << static_cast<unsigned int> ( Qt::ShiftModifier )
                            << static_cast<unsigned int> ( Qt::ControlModifier ) 
                            << static_cast<long int> ( 15 )
                            << true
                            << static_cast<long int> ( 16 );

    QTest::newRow("control_key") << static_cast<unsigned int> ( EKeyLeftCtrl ) 
                            << static_cast<unsigned int> ( Qt::ControlModifier )
                            << static_cast<unsigned int> ( Qt::ShiftModifier ) 
                            << static_cast<long int> ( 17 )
                            << true
                            << static_cast<long int> ( 18 );
                            
    QTest::newRow("meta_key") << static_cast<unsigned int> ( EKeyRightCtrl ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier ) 
                            << static_cast<long int> ( 19 )
                            << true
                            << static_cast<long int> ( 20 );


}

void TestXQKeyCapture::testCancelCaptureKey_S60()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureKey;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureKey( static_cast<TUint> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureKey( static_cast<TUint> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
}

void TestXQKeyCapture::testCancelCaptureKeyList_S60_data()
{
    testCancelCaptureKey_S60_data();
}

void TestXQKeyCapture::testCancelCaptureKeyList_S60()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureKey;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureKey( static_cast<TUint> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureKey( QList<TUint>() << static_cast<TUint> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
}

////////////////////////////////////////////////////////////////
//Cancel Capture Key Up And Downs
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCancelCaptureKeyUpAndDowns_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");

    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<long int>("additionalReqNum");
    
    QTest::newRow("esc_key") << static_cast<unsigned int> ( Qt::Key_Escape ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier ) 
                            << static_cast<long int> ( 34 )
                            << false
                            << static_cast<long int> ( 35 );

}

void TestXQKeyCapture::testCancelCaptureKeyUpAndDowns()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureKeyUpAndDowns;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureKeyUpAndDowns( static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureKeyUpAndDowns( static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
}

void TestXQKeyCapture::testCancelCaptureKeyUpAndDownsList_data()
{
    testCancelCaptureKeyUpAndDowns_data();
}

void TestXQKeyCapture::testCancelCaptureKeyUpAndDownsList()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureKeyUpAndDowns;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureKeyUpAndDowns( static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureKeyUpAndDowns(QList<Qt::Key>() << static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
}

////////////////////////////////////////////////////////////////
//Cancel Capture Key Up And Downs
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCancelCaptureKeyUpAndDowns_S60_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");

    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<long int>("additionalReqNum");
    
    QTest::newRow("esc_key") << static_cast<unsigned int> ( EKeyEscape ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier ) 
                            << static_cast<long int> ( 34 )
                            << false
                            << static_cast<long int> ( 35 );

}

void TestXQKeyCapture::testCancelCaptureKeyUpAndDowns_S60()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureKeyUpAndDowns;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureKeyUpAndDowns( static_cast<TUint> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureKeyUpAndDowns( static_cast<TUint> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
}

void TestXQKeyCapture::testCancelCaptureKeyUpAndDownsList_S60_data()
{
    testCancelCaptureKeyUpAndDowns_S60_data();
}

void TestXQKeyCapture::testCancelCaptureKeyUpAndDownsList_S60()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureKeyUpAndDowns;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureKeyUpAndDowns( static_cast<TUint> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureKeyUpAndDowns(QList<TUint>() << static_cast<TUint> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ) );
}

////////////////////////////////////////////////////////////////
//Cancel Capture Long Key
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCancelCaptureLongKey_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");
    QTest::addColumn<int>("longFlags");
    
    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<long int>("additionalReqNum");
    
    QTest::newRow("esc_key") << static_cast<unsigned int> ( Qt::Key_Escape ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<int> ( XQKeyCapture::LongNormal ) 
                            << static_cast<long int> ( 22 )
                            << false
                            << static_cast<long int> ( 23 );





}

void TestXQKeyCapture::testCancelCaptureLongKey()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureLongKey;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);
    QFETCH(int, longFlags);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureLongKey( static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ), static_cast<XQKeyCapture::LongFlags> (longFlags) );
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureLongKey( static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ), static_cast<XQKeyCapture::LongFlags> (longFlags) );
}

void TestXQKeyCapture::testCancelCaptureLongKeyList_data()
{
    testCancelCaptureLongKey_data();
}

void TestXQKeyCapture::testCancelCaptureLongKeyList()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureLongKey;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);
    QFETCH(int, longFlags);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureLongKey( static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ), static_cast<XQKeyCapture::LongFlags> (longFlags) );
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureLongKey( QList<Qt::Key>() << static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ), static_cast<XQKeyCapture::LongFlags> (longFlags) );
}

////////////////////////////////////////////////////////////////
//Cancel Capture Long Key
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCancelCaptureLongKey_S60_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");
    QTest::addColumn<int>("longFlags");
    
    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<long int>("additionalReqNum");
    
    QTest::newRow("esc_key") << static_cast<unsigned int> ( EKeyEscape ) 
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<unsigned int> ( Qt::NoModifier )
                            << static_cast<int> ( XQKeyCapture::LongNormal ) 
                            << static_cast<long int> ( 22 )
                            << false
                            << static_cast<long int> ( 23 );





}

void TestXQKeyCapture::testCancelCaptureLongKey_S60()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureLongKey;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);
    QFETCH(int, longFlags);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureLongKey( static_cast<TUint> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ), static_cast<XQKeyCapture::LongFlags> (longFlags) );
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureLongKey( static_cast<TUint> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ), static_cast<XQKeyCapture::LongFlags> (longFlags) );
}

void TestXQKeyCapture::testCancelCaptureLongKeyList_S60_data()
{
    testCancelCaptureLongKey_S60_data();
}

void TestXQKeyCapture::testCancelCaptureLongKeyList_S60()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureLongKey;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);
    QFETCH(int, longFlags);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureLongKey( static_cast<TUint> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ), static_cast<XQKeyCapture::LongFlags> (longFlags) );
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureLongKey(QList<TUint>() << static_cast<TUint> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ), static_cast<XQKeyCapture::LongFlags> (longFlags) );
}



////////////////////////////////////////////////////////////////
// ERRORS
//errorString
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testErrorString()
{
    keyCapture->errorString();
}

////////////////////////////////////////////////////////////////
// ERRORS
//errorId
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testErrorId()
{
    keyCapture->errorId();
}

QString TestXQKeyCapture::clearString(const QString& line) {
    QString s(line);
    s.replace(" ", "");
    s.replace("\t", "");
    return s.trimmed();
}

QString TestXQKeyCapture::clearString(const QString& line, const QString& prefix, const QString& comment) {
    QString s(line);
    s.replace(prefix, comment);
    s.replace(" ", "");
    s.replace("\t", "");
    return s.trimmed();
}

////////////////////////////////////////////////////////////////
// TEST KEY MAPPER FILE
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testKeyMapperFile()
{
    // test only for emulator build
    #ifdef __WINSCW__   
    
    QString firstline("static const KeyMapping keyMapping[] = {");
    QString lastline("};");
    QString comment("//");
    
    QStringList qt;
    QStringList kc;

    QFile qtFile("c:\\qkeymapper_s60.cpp");
    QVERIFY2(qtFile.open(QIODevice::ReadOnly | QIODevice::Text), "Failed to open: qtFile");
    
    QFile kcFile("c:\\keymapper.cpp");
    QVERIFY2(kcFile.open(QIODevice::ReadOnly | QIODevice::Text), "Failed to open: kcFile");
    
    QTextStream inQtFile(&qtFile);
    bool test(false);
    while (!inQtFile.atEnd()) {
        QString line = inQtFile.readLine();
        // trim everything that is on right side of comment and add to list if needed
        if (test) qt.append(clearString(line.split(comment).at(0)));
        if (line.contains(firstline)) {
            test = true;
        }
        if (line.contains(lastline)) {
            test = false;
        }
    }
    test = false;
    QTextStream inKcFile(&kcFile);
    while (!inKcFile.atEnd()) {
        QString line = inKcFile.readLine();
        // trim everything that is on right side of comment and add to list if needed
        if (test) kc.append(clearString(line.split(comment).at(0)));
        if (line.contains(firstline)) {
            test = true;
        }
        if (line.contains(lastline)) {
            test = false;
        }
    }
    
    QVERIFY2(qt.count() == kc.count(), "Amount of lines inside key definition is different");
    
    for(int i = 0; i < kc.size(); i++) {
        QString keys = kc.at(i);
        QVERIFY2(qt.contains(keys), "qtFile does not contain key(s) from capture keys");
    }
    
    for(int i = 0; i < qt.size(); i++) {
        QString keys = qt.at(i);
        QVERIFY2(kc.contains(keys), "kcFile does not conatin qt keys");
    }    
#else
    // Skip test on hw
    QSKIP( "This test is valid only on emulator", SkipSingle);
#endif // __WINSCW__
}


////////////////////////////////////////////////////////////////
// REQUEST SLOT
//windowGroupAction
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::windowGroupAction( WindowGroupActionType wgat, QList<unsigned int> paramList )
{
    if( !ignoreWindowGroupAction )
    {
        QVERIFY( wgat == actionType );
        QVERIFY( paramList.count() == numOfArgs );
        
        for( int i = 0; i < numOfArgs; i++)
            {
            if( additionalResult )
                QVERIFY( paramList[i] == additionalResults[i] );
            else 
                QVERIFY( paramList[i] == results[i] );
            }
    }
    if( willBeAdditionalRequest ){
        additionalResult = true;
        MyTestWindowGroup::Instance()->setRequestNumber(additionalRequestHandle);
        }
}

////////////////////////////////////////////////////////////////
//windowGroupActionCancel
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::windowGroupActionCancel( WindowGroupActionType wgat, QList<long int> paramList )
{
    QVERIFY( wgat == actionType );
    QVERIFY( paramList.count() == numOfArgs );
    
    for( int i = 0; i < numOfArgs; i++)
        {
        if( additionalResult )
            QVERIFY( paramList[i] == cancelAdditionalResults[i] );
        else 
            QVERIFY( paramList[i] == cancelResults[i] );
        }

    if( willBeAdditionalRequest ){
        additionalResult = true;
        }
}

////////////////////////////////////////////////////////////////
// MAIN
//main
////////////////////////////////////////////////////////////////

#ifdef _XQKEYCAPTURE_UNITTEST_LOG_TO_C_
    main (int argc, char* argv[]) 
    {
        QApplication app(argc, argv);
        TestXQKeyCapture tc;
        int c = 3;
        char* v[] = {argv[0], "-o", "c:/test.txt"};
        return QTest::qExec(&tc, c, v);
    }
#else
    QTEST_MAIN(TestXQKeyCapture)
#endif

#include "test_xqkeycapture.moc"
