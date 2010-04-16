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

#include "xqkeycapture.h"

#include "keycapture_p.h"
#include <qnamespace.h>

XqKeyCapture::XqKeyCapture() :
    d(new KeyCapturePrivate())
{

}

XqKeyCapture::~XqKeyCapture()
{
    delete d;
}

bool XqKeyCapture::captureKey(Qt::Key aKey, Qt::KeyboardModifiers aModifiersMap,
	Qt::KeyboardModifiers aModifier)
{
    return d->captureKey(aKey, aModifiersMap, aModifier);
}

bool XqKeyCapture::captureLongKey(Qt::Key aKey, Qt::KeyboardModifiers aModifiersMap,
	Qt::KeyboardModifiers aModifier, XqKeyCapture::LongFlags aLongType)
{
    return d->captureLongKey(aKey, aModifiersMap, aModifier, aLongType);
}

bool XqKeyCapture::captureKeyUpAndDowns(Qt::Key aKey, Qt::KeyboardModifiers aModifiersMap,
	Qt::KeyboardModifiers aModifier)
{
    return d->captureKeyUpAndDowns(aKey, aModifiersMap, aModifier);
}

bool XqKeyCapture::cancelCaptureKey(Qt::Key aKey, Qt::KeyboardModifiers aModifiersMap,
	Qt::KeyboardModifiers aModifier)
{
    return d->cancelCaptureKey(aKey, aModifiersMap, aModifier);
}

bool XqKeyCapture::cancelCaptureLongKey(Qt::Key aKey, Qt::KeyboardModifiers aModifiersMap,
	Qt::KeyboardModifiers aModifier, XqKeyCapture::LongFlags aLongType)
{
    return d->cancelCaptureLongKey(aKey, aModifiersMap, aModifier, aLongType);
}

bool XqKeyCapture::cancelCaptureKeyUpAndDowns(Qt::Key aKey, Qt::KeyboardModifiers aModifiersMap,
	Qt::KeyboardModifiers aModifier)
{
    return d->cancelCaptureKeyUpAndDowns(aKey, aModifiersMap, aModifier);
}

QString XqKeyCapture::errorString() const
{
    return d->errorString();
}

int XqKeyCapture::errorId() const
{
    return d->errorId();
}
