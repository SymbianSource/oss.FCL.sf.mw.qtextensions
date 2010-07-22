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

#ifndef KEYCAPTUREPRIVATE_STUB_H
#define KEYCAPTUREPRIVATE_STUB_H

#include <qnamespace.h>
#include <QString>

class KeyCapturePrivate {

public:
    KeyCapturePrivate();
    ~KeyCapturePrivate();

    bool initRemote(XQKeyCapture::CapturingFlags, XQKeyCapture::HandlingEvents);
        
    bool closeRemote(XQKeyCapture::CapturingFlags, XQKeyCapture::HandlingEvents);
    
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
    
    friend class XQKeyCapture;
};

#endif /* KEYCAPTUREPRIVATE_STUB_H */
