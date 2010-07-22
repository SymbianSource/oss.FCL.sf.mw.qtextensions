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

#ifndef KEYCAPTUREPRIVATE_S60_H
#define KEYCAPTUREPRIVATE_S60_H

#include <qnamespace.h>
#include <QString>
#include <QList>
#include "capturerequest_s60.h"

class RWindowGroup;
class RWsSession;
class TargetWrapper;

class QKeyMapperPrivate;
#ifdef _XQKEYCAPTURE_UNITTEST_
class MyTestWindowGroup;
#endif

class KeyCapturePrivate
    {

public:
    KeyCapturePrivate();
    ~KeyCapturePrivate();

    bool initRemote(XQKeyCapture::CapturingFlags flags);
    
    bool closeRemote(XQKeyCapture::CapturingFlags flags);

    bool captureKey(Qt::Key aKey, Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier);
    
    bool captureKey(TUint aKey, Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier);
    
    bool captureLongKey(Qt::Key aKey, Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier, XQKeyCapture::LongFlags aLongType);
    
    bool captureLongKey(TUint aKey, Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier, XQKeyCapture::LongFlags aLongType);
    
    bool captureKeyUpAndDowns(Qt::Key aKey,
        Qt::KeyboardModifiers aModifiersMask, Qt::KeyboardModifiers aModifier);

    bool captureKeyUpAndDowns(TUint aKey,
        Qt::KeyboardModifiers aModifiersMask, Qt::KeyboardModifiers aModifier);

    bool cancelCaptureKey(Qt::Key aKey, Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier);
    
    bool cancelCaptureKey(TUint aKey, Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier);
    
    bool cancelCaptureLongKey(Qt::Key aKey,
        Qt::KeyboardModifiers aModifiersMask, Qt::KeyboardModifiers aModifier,
        XQKeyCapture::LongFlags aLongType);
    
    bool cancelCaptureLongKey(TUint aKey,
        Qt::KeyboardModifiers aModifiersMask, Qt::KeyboardModifiers aModifier,
        XQKeyCapture::LongFlags aLongType);
    
    bool cancelCaptureKeyUpAndDowns(Qt::Key aKey,
        Qt::KeyboardModifiers aModifiersMask, Qt::KeyboardModifiers aModifier);

    bool cancelCaptureKeyUpAndDowns(TUint aKey,
        Qt::KeyboardModifiers aModifiersMask, Qt::KeyboardModifiers aModifier);

    QString errorString() const;

    int errorId() const;

private:
    static int mRemoteEventType_KeyPress;
    static int mRemoteEventType_KeyRelease;
    
private:

    bool resetRemote(XQKeyCapture::CapturingFlags flags);
    
    bool doCapture(TUint aKey, Qt::KeyboardModifiers aModifiersMask,
            Qt::KeyboardModifiers aModifier,
            CaptureRequest::CaptureRequestType type,
            XQKeyCapture::LongFlags aLongType =
                    XQKeyCapture::LongWaitNotApplicable);

    bool doCancelCapture(TUint aKey, Qt::KeyboardModifiers aModifiersMask,
            Qt::KeyboardModifiers aModifier,
            CaptureRequest::CaptureRequestType type,
            XQKeyCapture::LongFlags aLongType =
                    XQKeyCapture::LongWaitNotApplicable);

    void regenerateError();

private:
    int mLastError;
    QString mLastErrorString;
#ifndef _XQKEYCAPTURE_UNITTEST_
    RWindowGroup* mWindowGroup; //not owned
#else
    MyTestWindowGroup* mWindowGroup;
#endif
    QList<CaptureRequest*> *mRequestsList;
    QKeyMapperPrivate* mMapper;
    
    TargetWrapper *tgWrapper;
    
    friend class XQKeyCapture;
    };

#endif /* KEYCAPTUREPRIVATE_S60_H */
