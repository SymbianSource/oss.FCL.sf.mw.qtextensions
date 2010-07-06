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

#include "targetwrapper.h"

#include <QMenuBar>
#include <QApplication>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QTimer>

#include <QDebug>
#include <QtGlobal>

#include "responsehandler.h"
#include "responsehandlerex.h"

#include "txlogger.h"

TargetWrapper::TargetWrapper()
:
selector(0),
target(0),
targetEx(0),
handler(0),
handlerEx(0)
{
    initMapping();
}

TargetWrapper::~TargetWrapper()
{
    delete selector;
}

void TargetWrapper::init(XQKeyCapture::CapturingFlags flags)
{
    captureFlags = flags;
    try {
        delete selector;
    
        QT_TRAP_THROWING(selector = CRemConInterfaceSelector::NewL());
        
        if (captureFlags & XQKeyCapture::CaptureBasic) {
            QT_TRAP_THROWING(target = CRemConCoreApiTarget::NewL(*selector, *this));
            QT_TRAP_THROWING(handler = ResponseHandler::NewL(*target));
        }
         
        if (captureFlags & XQKeyCapture::CaptureCallHandlingExt) {
            QT_TRAP_THROWING(targetEx = CRemConCallHandlingTarget::NewL(*selector, *this));
            QT_TRAP_THROWING(handlerEx = ResponseHandlerEx::NewL(*targetEx));
        }
        
        QT_TRAP_THROWING(selector->OpenTargetL());
    
    } catch (const std::exception &e) {
        delete selector;
        selector = 0;
        target = 0;
        targetEx = 0;
        handler = 0;
        handlerEx = 0;
        qDebug() << "TargetWrapper::init - exception: " << e.what();
        throw;
    }    
}

Qt::Key TargetWrapper::mapKey(TRemConCoreApiOperationId aOperationId)
{
    Qt::Key key = keyMapping.value(aOperationId);
    
    if (key != Qt::Key())
        return key;
    else {
        return Qt::Key_unknown;
    }
    
}

void TargetWrapper::MrccatoCommand(TRemConCoreApiOperationId aOperationId, TRemConCoreApiButtonAction aButtonAct)
{
    Qt::Key key = mapKey(aOperationId); 

    switch (aButtonAct) {
        case ERemConCoreApiButtonPress:
            TX_LOG_ARGS("ERemConCoreApiButtonPress");
            sendKey(QEvent::KeyPress, key, Qt::NoModifier, aOperationId);
            break;
        case ERemConCoreApiButtonRelease:
            TX_LOG_ARGS("ERemConCoreApiButtonRelease");
            sendKey(QEvent::KeyRelease, key, Qt::NoModifier, aOperationId);
            break;
        case ERemConCoreApiButtonClick:
            TX_LOG_ARGS("ERemConCoreApiButtonClick");
            sendKey(QEvent::KeyPress, key, Qt::NoModifier, aOperationId);
            sendKey(QEvent::KeyRelease, key, Qt::NoModifier, aOperationId);
            break;
        default:
            return;
    }

    handler->CompleteAnyKey(aOperationId);
}

void TargetWrapper::AnswerCall()
{
    sendKey(QEvent::KeyPress, Qt::Key_Call, Qt::NoModifier);
    sendKey(QEvent::KeyRelease, Qt::Key_Call, Qt::NoModifier);
    handlerEx->CompleteAnyKey(0);
}

void TargetWrapper::EndCall()
{
    sendKey(QEvent::KeyPress, Qt::Key_Hangup, Qt::NoModifier);
    sendKey(QEvent::KeyRelease, Qt::Key_Hangup, Qt::NoModifier);
    handlerEx->CompleteAnyKey(0);
}

void TargetWrapper::AnswerEndCall()
{
    sendKey(QEvent::KeyPress, Qt::Key_Hangup, Qt::NoModifier);  //TODO: Qt::Key_ToggleCallHangup
    sendKey(QEvent::KeyRelease, Qt::Key_Hangup, Qt::NoModifier); 
    handlerEx->CompleteAnyKey(0);
}

void TargetWrapper::VoiceDial( const TBool aActivate )
{
    Q_UNUSED(aActivate)
}

void TargetWrapper::LastNumberRedial()
{
}

void TargetWrapper::DialCall( const TDesC8& aTelNumber )
{
    Q_UNUSED(aTelNumber)
}

void TargetWrapper::MultipartyCalling( const TDesC8& aData )
{
    Q_UNUSED(aData)
}

void TargetWrapper::GenerateDTMF( const TChar aChar )
{
    Q_UNUSED(aChar)
}

void TargetWrapper::SpeedDial( const TInt aIndex )    
{
    Q_UNUSED(aIndex)
}

void TargetWrapper::sendKey(QEvent::Type eventType, Qt::Key key, Qt::KeyboardModifiers modFlags, 
                    TRemConCoreApiOperationId aOperationId)
{
    QWidget *widget = getTargetWidget();
    if (widget) {
        QKeyEvent *event = NULL;
        if (captureFlags & XQKeyCapture::CaptureEnableRemoteExtEvents){
            if (eventType == QEvent::KeyPress){
                event = QKeyEvent::createExtendedKeyEvent(XQKeyCapture::remoteEventType_KeyPress(), 
                        key, modFlags, 0, aOperationId, 0);
            } else if (eventType == QEvent::KeyRelease){
                event = QKeyEvent::createExtendedKeyEvent(XQKeyCapture::remoteEventType_KeyRelease(), 
                        key, modFlags, 0, aOperationId, 0);
            }
        } else {
            event = new QKeyEvent(eventType, key, modFlags);
        }
        
        if (event){
            QApplication::sendEvent(widget, event);
            qDebug("sending key event!");
            delete event;
        }
    }
}

QWidget *TargetWrapper::getTargetWidget()
{
    QWidget *widget;
    widget = QWidget::keyboardGrabber();
    
    if (!widget) {
        widget = QApplication::focusWidget();
    }
    
    if (!widget) {
        if (QApplication::activePopupWidget()) {
            widget = QApplication::activePopupWidget()->focusWidget();
            if (!widget) {
                widget = QApplication::activePopupWidget();
            }
        }
    }

    return widget;
}



void TargetWrapper::initMapping()
{
    keyMapping.insert(ERemConCoreApiSelect, Qt::Key_Select);
    keyMapping.insert(ERemConCoreApiUp, Qt::Key_Up);
    keyMapping.insert(ERemConCoreApiDown, Qt::Key_Down);
    keyMapping.insert(ERemConCoreApiLeft, Qt::Key_Left); // Qt::Key_Direction_L
    keyMapping.insert(ERemConCoreApiRight, Qt::Key_Right); // Qt::Key_Direction_R
    keyMapping.insert(ERemConCoreApiRightUp, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiRightDown, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiLeftUp, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiLeftDown, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiRootMenu, Qt::Key_TopMenu); // Qt::Key_Menu
    keyMapping.insert(ERemConCoreApiSetupMenu, Qt::Key_unknown); 
    keyMapping.insert(ERemConCoreApiContentsMenu, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiFavoriteMenu, Qt::Key_Favorites); 
    keyMapping.insert(ERemConCoreApiExit, Qt::Key_unknown); // Qt::Key_Escape, Qt::Key_Cancel, Qt::Key_No
    keyMapping.insert(ERemConCoreApi0, Qt::Key_0);
    keyMapping.insert(ERemConCoreApi1, Qt::Key_1);
    keyMapping.insert(ERemConCoreApi2, Qt::Key_2);
    keyMapping.insert(ERemConCoreApi3, Qt::Key_3);
    keyMapping.insert(ERemConCoreApi4, Qt::Key_4);
    keyMapping.insert(ERemConCoreApi5, Qt::Key_5);
    keyMapping.insert(ERemConCoreApi6, Qt::Key_6);
    keyMapping.insert(ERemConCoreApi7, Qt::Key_7);
    keyMapping.insert(ERemConCoreApi8, Qt::Key_8);
    keyMapping.insert(ERemConCoreApi9, Qt::Key_9);
    keyMapping.insert(ERemConCoreApiDot, Qt::Key_Period);
    keyMapping.insert(ERemConCoreApiEnter, Qt::Key_Enter);
    keyMapping.insert(ERemConCoreApiClear, Qt::Key_Clear);
    keyMapping.insert(ERemConCoreApiChannelUp, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiChannelDown, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiPreviousChannel, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiSoundSelect, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiInputSelect, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiDisplayInformation, Qt::Key_unknown);   // Qt::Key_Time ???
    keyMapping.insert(ERemConCoreApiHelp, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiPageUp, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiPageDown, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiPower, Qt::Key_unknown); // Qt::Key_PowerOff, Qt::Key_WakeUp, Qt::Key_PowerDown, Qt::Key_Suspend
    keyMapping.insert(ERemConCoreApiVolumeUp, Qt::Key_VolumeUp);
    keyMapping.insert(ERemConCoreApiVolumeDown, Qt::Key_VolumeDown);
    keyMapping.insert(ERemConCoreApiMute, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiPlay, Qt::Key_MediaPlay);
    keyMapping.insert(ERemConCoreApiStop, Qt::Key_MediaStop);
    keyMapping.insert(ERemConCoreApiPause, Qt::Key_unknown); // NEW: Qt::Key_MediaPause
    keyMapping.insert(ERemConCoreApiRecord, Qt::Key_MediaRecord);
    keyMapping.insert(ERemConCoreApiRewind, Qt::Key_AudioRewind);
    keyMapping.insert(ERemConCoreApiFastForward, Qt::Key_AudioForward);
    keyMapping.insert(ERemConCoreApiEject, Qt::Key_Eject);
    keyMapping.insert(ERemConCoreApiForward, Qt::Key_MediaNext);
    keyMapping.insert(ERemConCoreApiBackward, Qt::Key_MediaPrevious);
    keyMapping.insert(ERemConCoreApiAngle, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiSubpicture, Qt::Key_unknown);  // Qt::Key_SplitScreen ???
    keyMapping.insert(ERemConCoreApiPausePlayFunction, Qt::Key_MediaPlay); // NEW: Media_PausePlay
    keyMapping.insert(ERemConCoreApiRestoreVolumeFunction, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiTuneFunction, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiSelectDiskFunction, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiSelectAvInputFunction, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiSelectAudioInputFunction, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiF1, Qt::Key_F1);
    keyMapping.insert(ERemConCoreApiF2, Qt::Key_F2);
    keyMapping.insert(ERemConCoreApiF3, Qt::Key_F3);
    keyMapping.insert(ERemConCoreApiF4, Qt::Key_F4);
    keyMapping.insert(ERemConCoreApiF5, Qt::Key_F5);
    keyMapping.insert(ENop, Qt::Key_unknown);
}
