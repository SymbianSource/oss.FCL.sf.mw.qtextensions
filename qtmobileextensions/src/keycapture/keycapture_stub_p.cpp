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

#include "keycapture_stub_p.h"

KeyCapturePrivate::KeyCapturePrivate()
{

}

KeyCapturePrivate::~KeyCapturePrivate()
{
}

void KeyCapturePrivate::captureKey(Qt::Key aKey, Qt::KeyboardModifier aModifiersMask,
	Qt::KeyboardModifier aModifier)
{
    Q_UNUSED(aKey);
    Q_UNUSED(aModifiersMask);
    Q_UNUSED(aModifier);
}

void KeyCapturePrivate::captureLongKey(Qt::Key aKey, Qt::KeyboardModifier aModifiersMask,
	Qt::KeyboardModifier aModifier, XqKeyCapture::LongFlags aLongType)
{
    Q_UNUSED(aKey);
    Q_UNUSED(aModifiersMask);
    Q_UNUSED(aModifier);
}

void KeyCapturePrivate::captureKeyUpAndDowns(Qt::Key aKey, Qt::KeyboardModifier aModifiersMask,
	Qt::KeyboardModifier aModifier)
{
    Q_UNUSED(aKey);
    Q_UNUSED(aModifiersMask);
    Q_UNUSED(aModifier);
}

void KeyCapturePrivate::cancelCaptureKey(Qt::Key aKey, Qt::KeyboardModifier aModifiersMask,
	Qt::KeyboardModifier aModifier)
{
    Q_UNUSED(aKey);
    Q_UNUSED(aModifiersMask);
    Q_UNUSED(aModifier);
}

void KeyCapturePrivate::cancelCaptureLongKey(Qt::Key aKey, Qt::KeyboardModifier aModifiersMask,
	Qt::KeyboardModifier aModifier, XqKeyCapture::LongFlags aLongType)
{
    Q_UNUSED(aKey);
    Q_UNUSED(aModifiersMask);
    Q_UNUSED(aModifier);
}

void KeyCapturePrivate::cancelCaptureKeyUpAndDowns(Qt::Key aKey,
	Qt::KeyboardModifier aModifiersMask, Qt::KeyboardModifier aModifier)
{
    Q_UNUSED(aKey);
    Q_UNUSED(aModifiersMask);
    Q_UNUSED(aModifier);
}

QString KeyCapturePrivate::errorString() const
{
    return QString();
}

int KeyCapturePrivate::errorId() const
{
    return 0;
}