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

#ifndef XQKEYCAPTURE_H
#define XQKEYCAPTURE_H

#include <qnamespace.h>
#include <QString>
#include <QObject>

#include <w32std.h>

#ifdef XQKEYCAPTURE_LIBRARY
#define XQKEYCAPTURE_EXPORT Q_DECL_EXPORT
#else
#define XQKEYCAPTURE_EXPORT Q_DECL_IMPORT
#endif

class KeyCapturePrivate;

class XQKEYCAPTURE_EXPORT XqKeyCapture {

public:
    enum LongFlags {
    LongShortEventImmediately = ELongCaptureShortEventImmediately,
    LongRepeatEvents = ELongCaptureRepeatEvents,
    LongNormal = ELongCaptureNormal,
    LongWaitShort = ELongCaptureWaitShort,
    LongWaitNotApplicable = 0
    };

public:
    XqKeyCapture();

    ~XqKeyCapture();

    bool captureKey(Qt::Key aKey, Qt::KeyboardModifiers aModifiersMap = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier);
    
    bool captureLongKey(Qt::Key aKey, Qt::KeyboardModifiers aModifiersMap = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier, XqKeyCapture::LongFlags aLongType = XqKeyCapture::LongNormal);
    
    bool captureKeyUpAndDowns(Qt::Key aKey, Qt::KeyboardModifiers aModifiersMap = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier);
    

    bool cancelCaptureKey(Qt::Key aKey, Qt::KeyboardModifiers aModifiersMap = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier);
    
    bool cancelCaptureLongKey(Qt::Key aKey, Qt::KeyboardModifiers aModifiersMap = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier, XqKeyCapture::LongFlags aLongType = XqKeyCapture::LongNormal);
    
    bool cancelCaptureKeyUpAndDowns(Qt::Key aKey, Qt::KeyboardModifiers aModifiersMap =
        Qt::NoModifier, Qt::KeyboardModifiers aModifier = Qt::NoModifier);

    QString errorString() const;

    int errorId() const;

private:
    KeyCapturePrivate* d;

};

#endif /* XQKEYCAPTURE_H */
