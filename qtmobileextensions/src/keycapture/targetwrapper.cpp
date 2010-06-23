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
basicApi(false),
callHandlingApi(false),
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

void TargetWrapper::setBasicApi(bool basic)
{
    basicApi = basic;
}

void TargetWrapper::setCallHandlingApi(bool callHandling)
{
    callHandlingApi = callHandling;
}


void TargetWrapper::init()
{
    try {
        delete selector;
    
        QT_TRAP_THROWING(selector = CRemConInterfaceSelector::NewL());
        
        if (basicApi) {
            QT_TRAP_THROWING(target = CRemConCoreApiTarget::NewL(*selector, *this));
            QT_TRAP_THROWING(handler = ResponseHandler::NewL(*target));
        }
         
        if (callHandlingApi) {
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
            sendPressKey(key, Qt::NoModifier);
            break;
        case ERemConCoreApiButtonRelease:
            TX_LOG_ARGS("ERemConCoreApiButtonRelease");
            sendReleaseKey(key, Qt::NoModifier);
            break;
        case ERemConCoreApiButtonClick:
            TX_LOG_ARGS("ERemConCoreApiButtonClick");
            sendPressKey(key, Qt::NoModifier);
            sendReleaseKey(key, Qt::NoModifier);
            break;
        default:
            break;
    }
    
    
    handler->CompleteAnyKey(aOperationId);

}

void TargetWrapper::AnswerCall()
{
    sendPressKey(Qt::Key_Call, Qt::NoModifier);
    sendReleaseKey(Qt::Key_Call, Qt::NoModifier);
    handlerEx->CompleteAnyKey(0);
}

void TargetWrapper::EndCall()
{
    sendPressKey(Qt::Key_Hangup, Qt::NoModifier);
    sendReleaseKey(Qt::Key_Hangup, Qt::NoModifier);
    handlerEx->CompleteAnyKey(0);
}

void TargetWrapper::AnswerEndCall()
{
    sendPressKey(Qt::Key_Hangup, Qt::NoModifier);  //TODO: Qt::Key_ToggleCallHangup
    sendReleaseKey(Qt::Key_Hangup, Qt::NoModifier); 
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

void TargetWrapper::sendPressKey(Qt::Key key, Qt::KeyboardModifiers modFlags)
{
    QWidget *widget = getTargetWidget();
    if (widget) {
        QKeyEvent event(QKeyEvent::KeyPress, key, modFlags, 0, false, 1);
        QApplication::sendEvent(widget, &event);
    }
    qDebug("sending key event!");
}

void TargetWrapper::sendReleaseKey(Qt::Key key, Qt::KeyboardModifiers modFlags)
{
    QWidget *widget = getTargetWidget();
    if (widget) {
        QKeyEvent event(QKeyEvent::KeyRelease, key, modFlags, 0, false, 1);
        QApplication::sendEvent(widget, &event);
    }
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
