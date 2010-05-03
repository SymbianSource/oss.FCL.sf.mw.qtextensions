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

class TestXqKeyCapture : public QObject
{
    Q_OBJECT

public:
    TestXqKeyCapture(): QObject(), keyCapture(0) {};


private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    
    void testCreateAndDestroy();

    void testCaptureKey_data();
    void testCaptureKey();
	
	void testCaptureKey_S60_data();
	void testCaptureKey_S60();

    void testCaptureKeyUpAndDowns_data();
    void testCaptureKeyUpAndDowns();
	
    void testCaptureKeyUpAndDowns_S60_data();
    void testCaptureKeyUpAndDowns_S60();
    
    void testCaptureLongKey_data();
    void testCaptureLongKey();
	
    void testCaptureLongKey_S60_data();
    void testCaptureLongKey_S60();
    
    void testCancelCaptureKey_data();
    void testCancelCaptureKey();
	
    void testCancelCaptureKey_S60_data();
    void testCancelCaptureKey_S60();

    void testCancelCaptureKeyUpAndDowns_data();
    void testCancelCaptureKeyUpAndDowns();
	
    void testCancelCaptureKeyUpAndDowns_S60_data();
    void testCancelCaptureKeyUpAndDowns_S60();
    
    void testCancelCaptureLongKey_data();
    void testCancelCaptureLongKey();
	
    void testCancelCaptureLongKey_S60_data();
    void testCancelCaptureLongKey_S60();

    void testErrorString();
    void testErrorId();
	
	void testKeyMapperFile();

private:
	QString clearString(const QString& line, const QString& prefix, const QString& comment);
    
private:
    XqKeyCapture* keyCapture;

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
void TestXqKeyCapture::initTestCase()
{
    bool ret = connect( MyTestWindowGroup::Instance(), SIGNAL( windowGroupAction( WindowGroupActionType, QList<unsigned int> )),
            this, SLOT( windowGroupAction( WindowGroupActionType, QList<unsigned int> ) ) );
    QVERIFY( ret );
    ret = connect( MyTestWindowGroup::Instance(), SIGNAL( windowGroupActionCancel( WindowGroupActionType, QList<long int> )),
            this, SLOT( windowGroupActionCancel( WindowGroupActionType, QList<long int> ) ) );
    QVERIFY( ret );
    
}

void TestXqKeyCapture::cleanupTestCase()
{
    //delete MyTestWindowGroup::Instance();
}

void TestXqKeyCapture::init()
{
    keyCapture = new XqKeyCapture();
}

void TestXqKeyCapture::cleanup()
{
    delete keyCapture;
}

void TestXqKeyCapture::testCreateAndDestroy()
{
    //empty, all done in init and cleanup
}

////////////////////////////////////////////////////////////////
// CAPTURE
//Capture Key
////////////////////////////////////////////////////////////////
void TestXqKeyCapture::testCaptureKey_data()
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

void TestXqKeyCapture::testCaptureKey()
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
}

////////////////////////////////////////////////////////////////
//Capture Key Up And Downs
////////////////////////////////////////////////////////////////
void TestXqKeyCapture::testCaptureKeyUpAndDowns_data()
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

void TestXqKeyCapture::testCaptureKeyUpAndDowns()
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

////////////////////////////////////////////////////////////////
//Capture Key Up And Downs
////////////////////////////////////////////////////////////////
void TestXqKeyCapture::testCaptureKeyUpAndDowns_S60_data()
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

void TestXqKeyCapture::testCaptureKeyUpAndDowns_S60()
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

////////////////////////////////////////////////////////////////
//Capture Long Key
////////////////////////////////////////////////////////////////
void TestXqKeyCapture::testCaptureLongKey_data()
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
                            << static_cast<int> ( XqKeyCapture::LongNormal ) 
                            << static_cast<unsigned int> ( EKeyEscape )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<int> ( 0 ) //priority
                            << static_cast<int> ( XqKeyCapture::LongNormal ) 
                            << static_cast<long int> ( 12 )
                            << false
                            << static_cast<unsigned int> ( 0 );

}

void TestXqKeyCapture::testCaptureLongKey()
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
    keyCapture->captureLongKey( static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ), static_cast<XqKeyCapture::LongFlags> (longFlags) );
}

////////////////////////////////////////////////////////////////
//Capture Long Key
////////////////////////////////////////////////////////////////
void TestXqKeyCapture::testCaptureLongKey_S60_data()
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
                            << static_cast<int> ( XqKeyCapture::LongNormal ) 
                            << static_cast<unsigned int> ( EKeyEscape )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<unsigned int> ( 0 )
                            << static_cast<int> ( 0 ) //priority
                            << static_cast<int> ( XqKeyCapture::LongNormal ) 
                            << static_cast<long int> ( 12 )
                            << false
                            << static_cast<unsigned int> ( 0 );

}

void TestXqKeyCapture::testCaptureLongKey_S60()
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
    keyCapture->captureLongKey( static_cast<TUint> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ), static_cast<XqKeyCapture::LongFlags> (longFlags) );
}

////////////////////////////////////////////////////////////////
// CANCEL
//Cancel Capture Key
////////////////////////////////////////////////////////////////
void TestXqKeyCapture::testCancelCaptureKey_data()
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

void TestXqKeyCapture::testCancelCaptureKey()
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

////////////////////////////////////////////////////////////////
// CANCEL
//Cancel Capture Key
////////////////////////////////////////////////////////////////
void TestXqKeyCapture::testCancelCaptureKey_S60_data()
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

void TestXqKeyCapture::testCancelCaptureKey_S60()
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

////////////////////////////////////////////////////////////////
//Cancel Capture Key Up And Downs
////////////////////////////////////////////////////////////////
void TestXqKeyCapture::testCancelCaptureKeyUpAndDowns_data()
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

void TestXqKeyCapture::testCancelCaptureKeyUpAndDowns()
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

////////////////////////////////////////////////////////////////
//Cancel Capture Key Up And Downs
////////////////////////////////////////////////////////////////
void TestXqKeyCapture::testCancelCaptureKeyUpAndDowns_S60_data()
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

void TestXqKeyCapture::testCancelCaptureKeyUpAndDowns_S60()
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

////////////////////////////////////////////////////////////////
//Cancel Capture Long Key
////////////////////////////////////////////////////////////////
void TestXqKeyCapture::testCancelCaptureLongKey_data()
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
                            << static_cast<int> ( XqKeyCapture::LongNormal ) 
                            << static_cast<long int> ( 22 )
                            << false
                            << static_cast<long int> ( 23 );





}

void TestXqKeyCapture::testCancelCaptureLongKey()
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
    keyCapture->captureLongKey( static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ), static_cast<XqKeyCapture::LongFlags> (longFlags) );
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureLongKey( static_cast<Qt::Key> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ), static_cast<XqKeyCapture::LongFlags> (longFlags) );
}

////////////////////////////////////////////////////////////////
//Cancel Capture Long Key
////////////////////////////////////////////////////////////////
void TestXqKeyCapture::testCancelCaptureLongKey_S60_data()
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
                            << static_cast<int> ( XqKeyCapture::LongNormal ) 
                            << static_cast<long int> ( 22 )
                            << false
                            << static_cast<long int> ( 23 );





}

void TestXqKeyCapture::testCancelCaptureLongKey_S60()
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
    keyCapture->captureLongKey( static_cast<TUint> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ), static_cast<XqKeyCapture::LongFlags> (longFlags) );
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureLongKey( static_cast<TUint> (qtKey), Qt::KeyboardModifier( qtMask ), Qt::KeyboardModifier( qtModifier ), static_cast<XqKeyCapture::LongFlags> (longFlags) );
}

void TestXqKeyCapture::testCaptureKey_S60_data()
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

void TestXqKeyCapture::testCaptureKey_S60()
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

////////////////////////////////////////////////////////////////
// ERRORS
//errorString
////////////////////////////////////////////////////////////////
void TestXqKeyCapture::testErrorString()
{
    keyCapture->errorString();
}

////////////////////////////////////////////////////////////////
// ERRORS
//errorId
////////////////////////////////////////////////////////////////
void TestXqKeyCapture::testErrorId()
{
    keyCapture->errorId();
}

QString TestXqKeyCapture::clearString(const QString& line, const QString& prefix, const QString& comment) {
    QString s(line);
    s.replace(prefix, comment);
    s.replace(" ", "");
    s.replace("\t", "");
    return s.trimmed();
}

////////////////////////////////////////////////////////////////
// TEST KEY MAPPER FILE
////////////////////////////////////////////////////////////////
void TestXqKeyCapture::testKeyMapperFile()
{
    QString prefix("    keyMapping.append(KeyMapping(");
    QString comment("//");
    
    QStringList qt;
    QStringList kc;

    QFile qtFile("c:\qkeymapper_s60.cpp");
    QVERIFY(qtFile.open(QIODevice::ReadOnly | QIODevice::Text));
    
    QFile kcFile("c:\keymapper.cpp");
    QVERIFY(kcFile.open(QIODevice::ReadOnly | QIODevice::Text));
    
    QTextStream inQtFile(&qtFile);
    while (!inQtFile.atEnd()) {
        QString line = inQtFile.readLine();
        if (line.contains(prefix) && !line.contains(comment)) {
            qt.append(clearString(line, prefix, comment));
        }
    }

    QTextStream inKcFile(&kcFile);
    while (!inKcFile.atEnd()) {
        QString line = inKcFile.readLine();
        if (line.contains(prefix) && !line.contains(comment)) {
            kc.append(clearString(line, prefix, comment));
        }
    }
    
    QVERIFY(qt.size() == kc.size());
    
    for(int i = 0; i < kc.size(); i++) {
        QString keys = kc.at(i);
        QVERIFY(qt.contains(keys));
    }
    
    for(int i = 0; i < qt.size(); i++) {
        QString keys = qt.at(i);
        QVERIFY(kc.contains(keys));
    }    
}


////////////////////////////////////////////////////////////////
// REQUEST SLOT
//windowGroupAction
////////////////////////////////////////////////////////////////
void TestXqKeyCapture::windowGroupAction( WindowGroupActionType wgat, QList<unsigned int> paramList )
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
void TestXqKeyCapture::windowGroupActionCancel( WindowGroupActionType wgat, QList<long int> paramList )
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
        TestXqKeyCapture tc;
        int c = 3;
        char* v[] = {argv[0], "-o", "c:/test.txt"};
        return QTest::qExec(&tc, c, v);
    }
#else
    QTEST_MAIN(TestXqKeyCapture)
#endif

#include "test_xqkeycapture.moc"
