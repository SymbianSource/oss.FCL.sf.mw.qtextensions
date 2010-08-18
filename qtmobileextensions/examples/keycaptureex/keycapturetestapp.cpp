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

#include <QDebug>
#include <QApplication>
#include <QKeyEvent>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QImageReader>
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>
#include <QString>
#include <QCheckBox>
#include <QPlainTextEdit>
#include <QString>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QEvent>


#include <QFont>

#include "keycapturetestapp.h"
#include "txlogger.h"

#include "mapping.h"
#include "capturerequest.h"

KeyCaptureTestApp::KeyCaptureTestApp( QWidget *parent) : QMainWindow(parent)
{   
    TX_ENTRY
    
    qApp->installEventFilter(this);
    QCoreApplication::instance()->installEventFilter(this);
    
    
	setWindowTitle(tr("KeyCaptureTestApp"));
    
    mKeyCapture = new XQKeyCapture();
    
    mKeysMap.insert("Up", Qt::Key_Up);
    mKeysMap.insert("Down", Qt::Key_Down);
    mKeysMap.insert("Menu", Qt::Key_Menu);
    
    
    mKeysMenu = new QMenu(this);
    foreach (QString value, mKeysMap.keys())
        mKeysMenu->addAction(value)->setData( QVariant(value) );
    
    mLongFlagsMap.insert("LongNormal", XQKeyCapture::LongNormal);
    mLongFlagsMap.insert("LongRepeatEvents", XQKeyCapture::LongRepeatEvents);
    mLongFlagsMap.insert("LongShortEventImmediately", XQKeyCapture::LongShortEventImmediately);
    mLongFlagsMap.insert("LongWaitNotApplicable", XQKeyCapture::LongWaitNotApplicable);
    mLongFlagsMap.insert("LongWaitShort", XQKeyCapture::LongWaitShort);
    
    mLongFlagsMenu = new QMenu(this);
    foreach (QString value, mLongFlagsMap.keys())
        mLongFlagsMenu->addAction(value)->setData( QVariant(value) );
    
	QMenu *captureMenu = menuBar()->addMenu(QString("Capture"));
    connect(captureMenu, SIGNAL(triggered(QAction*)), this, SLOT(triggered(QAction*)));
	captureMenu->addAction(QString("Key"))->setData( QVariant(1) );
	captureMenu->addAction(QString("Long Key"))->setData( QVariant(2) );
	captureMenu->addAction(QString("Up and Down Key"))->setData( QVariant(3) );

	QMenu *cancelCaptureMenu = menuBar()->addMenu(QString("Cancel Capture"));
    connect(cancelCaptureMenu, SIGNAL(triggered(QAction*)), this, SLOT(triggered(QAction*)));
	cancelCaptureMenu->addAction(QString("Cancel Key"))->setData( QVariant(4) );
	cancelCaptureMenu->addAction(QString("Cancel Long Key"))->setData( QVariant(5) );
	cancelCaptureMenu->addAction(QString("Cancel Up and Down Key"))->setData( QVariant(6) );

    QMenu *remoteMenu = menuBar()->addMenu(QString("Remote"));

    // *** remcon ***
    
    remoteAllOn = remoteMenu->addAction(QString("Turn on all"));
    remoteAllOff = remoteMenu->addAction(QString("Turn off all"));
    
    toggleRemoteBasic = remoteMenu->addAction(QString("Basic Remote"));
    toggleRemoteBasic->setCheckable(true);
    
    toggleRemoteCallHandlingEx = remoteMenu->addAction(QString("Call Handl. Ex Remote"));
    toggleRemoteCallHandlingEx->setCheckable(true);
    
    toggleRemoteExtEvents = remoteMenu->addAction(QString("Extended Remote Events"));
    toggleRemoteExtEvents->setCheckable(true);

    connect(toggleRemoteBasic, SIGNAL(toggled(bool)), this, SLOT(enableRemBasic(bool)));
    connect(toggleRemoteCallHandlingEx, SIGNAL(toggled(bool)), this, SLOT(enableRemCallHandlingEx(bool)));
    connect(toggleRemoteExtEvents, SIGNAL(toggled(bool)), this, SLOT(enableRemoteExtEvents(bool)));

    connect(remoteAllOn, SIGNAL(triggered(bool)), this, SLOT(remoteAll(bool)));
    connect(remoteAllOff, SIGNAL(triggered(bool)), this, SLOT(remoteNone(bool)));
    
    // *** utilities ***

    connect(menuBar()->addAction(QString("Clear Log")), SIGNAL(triggered()), this, SLOT(cleanLog()));
    connect(menuBar()->addAction(QString("Exit")), SIGNAL(triggered()), qApp, SLOT(quit()));

    QWidget *window = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;

	mTextArea = new QPlainTextEdit("");
	mTextArea->setTextInteractionFlags(Qt::NoTextInteraction);
	QFont font = QFont(mTextArea->font());
	font.setPixelSize(10);
	mTextArea->setFont(font);
	
	layout->addWidget(mTextArea);
	
    window->setLayout(layout);
    setCentralWidget(window);
    window->show();;

    mappingPtr = new Mapping();
    
    TX_EXIT
}
	
KeyCaptureTestApp::~KeyCaptureTestApp()
{
    delete mappingPtr;
}

void KeyCaptureTestApp::triggered(QAction* aAction)
{
    CaptureRequest request(mappingPtr);
    if (!request.setType(aAction)){
        return;
    }
    if (!request.setKey(mKeysMenu->exec(), &mKeysMap)){
        return;
    }
//TODO: add modifiersMap selection menu;
//TODO: add modifier selection menu;
    if ( request.mRequestType == CaptureRequest::RequestTypeLongKey || request.mRequestType == CaptureRequest::RequestTypeCancelLongKey){
        if (!request.setLongFlags(mLongFlagsMenu->exec(), &mLongFlagsMap)){
            return;
        }
    }
    procesAction(request);
}

void KeyCaptureTestApp::cleanLog()
{
    mTextArea->setPlainText("");
}

void KeyCaptureTestApp::procesAction(CaptureRequest request)
{
    TX_ENTRY
    switch (request.mRequestType) {
        case CaptureRequest::RequestTypeKey :
            mKeyCapture->captureKey(request.mKey, request.mModifiersMap, request.mModifier );
            addTextLine(QString("%1:%2\n").arg(request.toString()).arg(mKeyCapture->errorString()));
            break;
        case CaptureRequest::RequestTypeLongKey :
            mKeyCapture->captureLongKey(request.mKey, request.mModifiersMap, request.mModifier, request.mLongFlags);
            addTextLine(QString("%1:%2\n").arg(request.toString()).arg(mKeyCapture->errorString()));
            break;
        case CaptureRequest::RequestTypeKeyUpAndDowns :
            mKeyCapture->captureKeyUpAndDowns(request.mKey, request.mModifiersMap, request.mModifier );
            addTextLine(QString("%1:%2\n").arg(request.toString()).arg(mKeyCapture->errorString()));
            break;
        case CaptureRequest::RequestTypeCancelKey :
            mKeyCapture->cancelCaptureKey(request.mKey, request.mModifiersMap, request.mModifier );
            addTextLine(QString("%1:%2\n").arg(request.toString()).arg(mKeyCapture->errorString()));
            break;
        case CaptureRequest::RequestTypeCancelLongKey :
            mKeyCapture->cancelCaptureLongKey(request.mKey, request.mModifiersMap, request.mModifier, request.mLongFlags);
            addTextLine(QString("%1:%2\n").arg(request.toString()).arg(mKeyCapture->errorString()));
            break;
        case CaptureRequest::RequestTypeCancelKeyUpAndDowns :
            mKeyCapture->cancelCaptureKeyUpAndDowns(request.mKey, request.mModifiersMap, request.mModifier );
            addTextLine(QString("%1:%2\n").arg(request.toString()).arg(mKeyCapture->errorString()));
            break;
        default:
            break;
    }

    TX_EXIT    
}

void KeyCaptureTestApp::addTextLine(QString aText)
{
    TX_LOG_ARGS( QString("aText=%1").arg(aText));
	if ( !aText.endsWith("\n"))
		aText = aText + "\n";
	QString msg = mTextArea->toPlainText();
	msg = aText + msg;
	mTextArea->setPlainText(msg);
}

bool KeyCaptureTestApp::event(QEvent *ev)
{
    processEvent(ev);
    return QMainWindow::event(ev);
}

bool KeyCaptureTestApp::eventFilter(QObject *o, QEvent *ev)
{
    processEvent(ev);
    return QMainWindow::eventFilter(o, ev);
}

void KeyCaptureTestApp::processEvent(QEvent *ev)
{
    if (ev){
        if (ev->type() == QEvent::KeyPress){
           QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev);
           QString keyName = mappingPtr->name(static_cast<Qt::Key>(keyEvent->key())); 
           
           addTextLine(QString("KeyPress:%1\n").arg(keyName));
        } else if (ev->type() == QEvent::KeyRelease){
           QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev);
           QString keyName = mappingPtr->name(static_cast<Qt::Key>(keyEvent->key()));
           
           addTextLine(QString("KeyRelease:%1\n").arg(keyName));
        } else if (ev->type() == XQKeyCapture::remoteEventType_KeyPress()){
           QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev);
                      
           QString keyName = mappingPtr->name(static_cast<Qt::Key>(keyEvent->key()));
                      
           addTextLine(QString("KeyPress:%1\n").arg(keyName));
           addTextLine(QString("Native virtual key:%1\n").arg((int)keyEvent->nativeVirtualKey()));
        } else if (ev->type() == XQKeyCapture::remoteEventType_KeyRelease()){
           QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev);
                      
           QString keyName = mappingPtr->name(static_cast<Qt::Key>(keyEvent->key()));
                      
           addTextLine(QString("KeyRelease:%1\n").arg(keyName));
           addTextLine(QString("Native virtual key:%1\n").arg((int)keyEvent->nativeVirtualKey()));
        }
    }
}

void KeyCaptureTestApp::enableRemBasic(bool enable)
{
    if (enable) {
        addTextLine("Remote Basic enabled");
        QFlags<XQKeyCapture::CapturingFlag> flags = XQKeyCapture::CaptureNone;
        if (toggleRemoteExtEvents->isChecked())
            flags = XQKeyCapture::CaptureEnableRemoteExtEvents;
        if (toggleRemoteCallHandlingEx->isChecked())
            flags |= XQKeyCapture::CaptureCallHandlingExt;
        mKeyCapture->captureRemoteKeys(flags |= XQKeyCapture::CaptureBasic);
    } else {
        addTextLine("Remote Basic disabled");
        mKeyCapture->cancelCaptureRemoteKeys(XQKeyCapture::CaptureBasic);
    }
}

void KeyCaptureTestApp::enableRemCallHandlingEx(bool enable)
{
    if (enable) {
        addTextLine("Remote Call Handling Ext. enabled");
        QFlags<XQKeyCapture::CapturingFlag> flags = XQKeyCapture::CaptureNone;
        if (toggleRemoteExtEvents->isChecked())
            flags = XQKeyCapture::CaptureEnableRemoteExtEvents;
        if (toggleRemoteBasic->isChecked())
            flags |= XQKeyCapture::CaptureBasic;
        mKeyCapture->captureRemoteKeys(flags | XQKeyCapture::CaptureCallHandlingExt);
    } else {
        addTextLine("Remote Call Handling Ext. disabled");
        mKeyCapture->cancelCaptureRemoteKeys(XQKeyCapture::CaptureCallHandlingExt);
    }
}

void KeyCaptureTestApp::enableRemoteExtEvents(bool enable)
{
    if (enable) {
        addTextLine("Remote Events Ext. enabled");
        QFlags<XQKeyCapture::CapturingFlag> flags = XQKeyCapture::CaptureNone;
        if (toggleRemoteCallHandlingEx->isChecked())
            flags = XQKeyCapture::CaptureCallHandlingExt;
        if (toggleRemoteBasic->isChecked())
            flags |= XQKeyCapture::CaptureBasic;
        mKeyCapture->captureRemoteKeys(flags | XQKeyCapture::CaptureEnableRemoteExtEvents);
    } else {
        addTextLine("Remote Events Ext. disabled");
        mKeyCapture->cancelCaptureRemoteKeys(XQKeyCapture::CaptureEnableRemoteExtEvents);
    }
}

void KeyCaptureTestApp::remoteAll(bool enable)
{
    Q_UNUSED(enable);
    toggleRemoteBasic->setChecked(true);
    toggleRemoteCallHandlingEx->setChecked(true);
    toggleRemoteExtEvents->setChecked(true);
    addTextLine("Remote: enable all");
    mKeyCapture->captureRemoteKeys(XQKeyCapture::CaptureCallHandlingExt |  XQKeyCapture::CaptureBasic | 
            XQKeyCapture::CaptureEnableRemoteExtEvents);
}

void KeyCaptureTestApp::remoteNone(bool enable)
{
    Q_UNUSED(enable);
    toggleRemoteBasic->setChecked(false);
    toggleRemoteCallHandlingEx->setChecked(false);
    toggleRemoteExtEvents->setChecked(false);
    addTextLine("Remote: disable all");
    mKeyCapture->cancelCaptureRemoteKeys(XQKeyCapture::CaptureCallHandlingExt | XQKeyCapture::CaptureBasic | 
            XQKeyCapture::CaptureEnableRemoteExtEvents);
}
