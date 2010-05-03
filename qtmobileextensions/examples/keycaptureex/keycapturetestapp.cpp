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

#include "keycapturetestapp.h"
#include "txlogger.h"


KeyCaptureTestApp::KeyCaptureTestApp( QWidget *parent) : QMainWindow(parent)
{
    TX_ENTRY
    
//    qApp->installEventFilter(this);
//    QCoreApplication::instance()->installEventFilter(this);
    
    
	setWindowTitle(tr("KeyCaptureTestApp"));
    
    mKeyCapture = new XqKeyCapture();
    
    mKeysMap.insert("Up_Qt",    Qt::Key_Up);
    mKeysMap.insert("Down_Qt",  Qt::Key_Down);
    mKeysMap.insert("Menu_Qt",  Qt::Key_Launch0);
    
    mKeysMap.insert("Up_S60",   EKeyUpArrow);
    mKeysMap.insert("Down_S60", EKeyDownArrow);
    mKeysMap.insert("Menu_S60", EKeyApplication1);
    
    
    mKeysMenu = new QMenu(this);
    foreach (QString value, mKeysMap.keys())
        mKeysMenu->addAction(value)->setData( QVariant(value) );
    
    mLongFlagsMap.insert("LongNormal", XqKeyCapture::LongNormal);
    mLongFlagsMap.insert("LongRepeatEvents", XqKeyCapture::LongRepeatEvents);
    mLongFlagsMap.insert("LongShortEventImmediately", XqKeyCapture::LongShortEventImmediately);
    mLongFlagsMap.insert("LongWaitNotApplicable", XqKeyCapture::LongWaitNotApplicable);
    mLongFlagsMap.insert("LongWaitShort", XqKeyCapture::LongWaitShort);
    
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

    connect(menuBar()->addAction(QString("Clear Log")), SIGNAL(triggered()), this, SLOT(cleanLog()));
    connect(menuBar()->addAction(QString("Exit")), SIGNAL(triggered()), qApp, SLOT(quit()));


    QWidget *window = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;

	mTextArea = new QPlainTextEdit("Log area\n");
	layout->addWidget(mTextArea);
	
    window->setLayout(layout);
    setCentralWidget(window);
    window->show();;

    
    TX_EXIT
}
	
KeyCaptureTestApp::~KeyCaptureTestApp()
{
	
}

void KeyCaptureTestApp::triggered(QAction* aAction)
{
    CaptureRequest request;
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
    switch (request.mRequestType){
        case CaptureRequest::RequestTypeKey :
            
            if (request.isQtKey) 
                mKeyCapture->captureKey((Qt::Key) request.mKey, request.mModifiersMap, request.mModifier );
            else
                mKeyCapture->captureKey((TUint) request.mKey, request.mModifiersMap, request.mModifier );
            
            addTextLine(QString("%1:%2\n").arg(request.toString()).arg(mKeyCapture->errorString()));
            break;
        case CaptureRequest::RequestTypeLongKey :
            if (request.isQtKey)
                mKeyCapture->captureLongKey((Qt::Key) request.mKey, request.mModifiersMap, request.mModifier, request.mLongFlags);
            else
                mKeyCapture->captureLongKey((TUint) request.mKey, request.mModifiersMap, request.mModifier, request.mLongFlags);
            
            addTextLine(QString("%1:%2\n").arg(request.toString()).arg(mKeyCapture->errorString()));
            break;
        case CaptureRequest::RequestTypeKeyUpAndDowns :
            if (request.isQtKey)
                mKeyCapture->captureKeyUpAndDowns((Qt::Key) request.mKey, request.mModifiersMap, request.mModifier );
            else
                mKeyCapture->captureKeyUpAndDowns((TUint) request.mKey, request.mModifiersMap, request.mModifier );

            addTextLine(QString("%1:%2\n").arg(request.toString()).arg(mKeyCapture->errorString()));
            break;
        case CaptureRequest::RequestTypeCancelKey :
            if (request.isQtKey)
                mKeyCapture->cancelCaptureKey((Qt::Key) request.mKey, request.mModifiersMap, request.mModifier );
            else
                mKeyCapture->cancelCaptureKey((TUint) request.mKey, request.mModifiersMap, request.mModifier );
                
            addTextLine(QString("%1:%2\n").arg(request.toString()).arg(mKeyCapture->errorString()));
            break;
        case CaptureRequest::RequestTypeCancelLongKey :
            if (request.isQtKey)
                mKeyCapture->cancelCaptureLongKey((Qt::Key) request.mKey, request.mModifiersMap, request.mModifier, request.mLongFlags);
            else
                mKeyCapture->cancelCaptureLongKey((TUint) request.mKey, request.mModifiersMap, request.mModifier, request.mLongFlags);
                
            addTextLine(QString("%1:%2\n").arg(request.toString()).arg(mKeyCapture->errorString()));
            break;
        case CaptureRequest::RequestTypeCancelKeyUpAndDowns :
            if (request.isQtKey)
                mKeyCapture->cancelCaptureKeyUpAndDowns((Qt::Key) request.mKey, request.mModifiersMap, request.mModifier );
            else
                mKeyCapture->cancelCaptureKeyUpAndDowns((TUint) request.mKey, request.mModifiersMap, request.mModifier );
            
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
           addTextLine(QString("KeyPress:%1\n").arg(keyEvent->key(), 0, 16) );
       }else if (ev->type() == QEvent::KeyRelease){
           QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev);
           addTextLine(QString("KeyRelease:%1\n").arg(keyEvent->key(), 0, 16) );
       }
    }
}

CaptureRequest::CaptureRequest()
{
    mRequestType = RequestTypeUndefined;
    mKey = Qt::Key_unknown;    
    mModifiersMap = Qt::NoModifier;
    mModifier = Qt::NoModifier;
    mLongFlags = XqKeyCapture::LongNormal; 
}

CaptureRequest::~CaptureRequest()
{
    
}

QString CaptureRequest::toString()
{
    QString res = "Request ";
    switch (mRequestType){
        case CaptureRequest::RequestTypeKey :
            res +="Key(";
            break;
        case CaptureRequest::RequestTypeLongKey :
            res +="LongKey(";            
            break;
        case CaptureRequest::RequestTypeKeyUpAndDowns :
            res +="UpAndDowns(";  
            break;
        case CaptureRequest::RequestTypeCancelKey :
            res +="CancelKey(";
            break;
        case CaptureRequest::RequestTypeCancelLongKey :
            res +="CancelLongKey(";
            break;
        case CaptureRequest::RequestTypeCancelKeyUpAndDowns :
            res +="CancelUpAndDowns(";
            break;
        default:
            res +="Unknown";
            break;
    }
    if ( mRequestType == RequestTypeUndefined)
        return res;
    res +=QString("%1").arg(mKey, 0, 16);
    //TODO::put to res mModifiersMap    
    //TODO::put to res mModifier
    if ( mRequestType == RequestTypeLongKey || mRequestType == RequestTypeCancelLongKey)
        res +=QString(",%1)").arg(mLongFlags, 0, 16);
    else
        res +=")";
    
    return res;
}

bool CaptureRequest::setType(QAction* action)
{
    if (!action)
        return false;
    bool ok;
    int act = action->data().toInt(&ok);
    if (ok){
        switch (act){
            case 1 : mRequestType = RequestTypeKey; break;
            case 2 : mRequestType = RequestTypeLongKey; break;
            case 3 : mRequestType = RequestTypeKeyUpAndDowns; break;
            case 4 : mRequestType = RequestTypeCancelKey; break;
            case 5 : mRequestType = RequestTypeCancelLongKey; break;
            case 6 : mRequestType = RequestTypeCancelKeyUpAndDowns; break;
        }
        return mRequestType!=RequestTypeUndefined;
    }else{
        return false;
    }
}

//bool CaptureRequest::setKey(QAction* action, QMap<QString, Qt::Key> *map)
bool CaptureRequest::setKey(QAction* action, QMap<QString, long> *map)
{
    if (!action || !map || map->count()==0)
        return false;
    QString key = action->data().toString();
    if ( !key.isNull() && map->contains(key)){
        if (key.contains("_Qt")) {
            isQtKey = true;
        } else {
            isQtKey = false;
        }
        mKey = map->value(key);
        return true;
    }
    return false;
}

bool CaptureRequest::setLongFlags(QAction* action, QMap<QString, XqKeyCapture::LongFlags> *map)
{
    if (!action || !map || map->count()==0)
        return false;
    QString flag = action->data().toString();
    if ( !flag.isNull() && map->contains(flag)){
        mLongFlags = map->value(flag);
        return true;
    }
    return false;
}
