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

/*!
 Constructor.
*/
XqKeyCapture::XqKeyCapture() :
    d(new KeyCapturePrivate())
{

}

/*!
 Destructor.
*/
XqKeyCapture::~XqKeyCapture()
{
    delete d;
}

/*!
 Selects a given key for capturing key pressing. Requires a Qt key code.
 \param aKey A Qt key.
 \param aModifiersMap
 \param aModifier 
 \retval Returns true if aKey was succesfully added to the capturing system, otherwise returns false.
 */
bool XqKeyCapture::captureKey(Qt::Key aKey,
    Qt::KeyboardModifiers aModifiersMap, Qt::KeyboardModifiers aModifier)
{
    return d->captureKey(aKey, aModifiersMap, aModifier);
}

/*!
 Selects a given key for capturing key pressing. Requires a S60 key code (TKeyCode).
 \param aKey A S60 key code (TKeyCode).
 \param aModifiersMap
 \param aModifier 
 \retval Returns true if aKey was succesfully added to the capturing system, otherwise returns false.
 */
bool XqKeyCapture::captureKey(TUint aKey,
    Qt::KeyboardModifiers aModifiersMap, Qt::KeyboardModifiers aModifier)
{
    return d->captureKey(aKey, aModifiersMap, aModifier);
}

/*!
 Selects a given key for capturing long pressing. Requires a Qt key code.
 \param aKey A Qt key.
 \param aModifiersMap
 \param aModifier 
 \retval Returns true if aKey was succesfully added to the capturing system, otherwise returns false.
 */
bool XqKeyCapture::captureLongKey(Qt::Key aKey,
    Qt::KeyboardModifiers aModifiersMap, Qt::KeyboardModifiers aModifier,
    XqKeyCapture::LongFlags aLongType)
{
    return d->captureLongKey(aKey, aModifiersMap, aModifier, aLongType);
}

/*!
 Selects a given key for capturing long pressing. Requires a S60 key code (TKeyCode).
 \param aKey A S60 key code (TKeyCode).
 \param aModifiersMap
 \param aModifier 
 \retval Returns true if aKey was succesfully added to the capturing system, otherwise returns false.
 */
bool XqKeyCapture::captureLongKey(TUint aKey,
    Qt::KeyboardModifiers aModifiersMap, Qt::KeyboardModifiers aModifier,
    XqKeyCapture::LongFlags aLongType)
{
    return d->captureLongKey(aKey, aModifiersMap, aModifier, aLongType);
}

/*!
 Selects a given key for capturing pressing up and down. Requires a Qt key code.
 \param aKey A Qt key.
 \param aModifiersMap
 \param aModifier 
 \retval Returns true if aKey was succesfully added to the capturing system, otherwise returns false.
 */
bool XqKeyCapture::captureKeyUpAndDowns(Qt::Key aKey,
    Qt::KeyboardModifiers aModifiersMap, Qt::KeyboardModifiers aModifier)
{
    return d->captureKeyUpAndDowns(aKey, aModifiersMap, aModifier);
}

/*!
 Selects a given key for capturing pressing up and down. Requires a S60 key scan code (TStdScanCode).
 \param aKey A S60 key scan code (TStdScanCode).
 \param aModifiersMap
 \param aModifier 
 \retval Returns true if aKey was succesfully added to the capturing system, otherwise returns false.
 */
bool XqKeyCapture::captureKeyUpAndDowns(TUint aKey,
    Qt::KeyboardModifiers aModifiersMap, Qt::KeyboardModifiers aModifier)
{
    return d->captureKeyUpAndDowns(aKey, aModifiersMap, aModifier);
}

/*!
 Deselects a given key from key capturing. Requires a Qt key code.
 \param aKey A Qt key.
 \param aModifiersMap
 \param aModifier 
 \retval Returns true if aKey was succesfully removed from the capturing system, otherwise returns false.
 */
bool XqKeyCapture::cancelCaptureKey(Qt::Key aKey,
    Qt::KeyboardModifiers aModifiersMap, Qt::KeyboardModifiers aModifier)
{
    return d->cancelCaptureKey(aKey, aModifiersMap, aModifier);
}

/*!
 Deselects a given key from key capturing. Requires a S60 key code (TKeyCode).
 \param aKey A S60 key code (TKeyCode).
 \param aModifiersMap
 \param aModifier 
 \retval Returns true if aKey was succesfully removed from the capturing system, otherwise returns false.
 */
bool XqKeyCapture::cancelCaptureKey(TUint aKey,
    Qt::KeyboardModifiers aModifiersMap, Qt::KeyboardModifiers aModifier)
{
    return d->cancelCaptureKey(aKey, aModifiersMap, aModifier);
}

/*!
 Deselects a given key from capturing long pressing. Requires a Qt key code.
 \param aKey A Qt key.
 \param aModifiersMap
 \param aModifier 
 \retval Returns true if aKey was succesfully removed from the capturing system, otherwise returns false.
 */
bool XqKeyCapture::cancelCaptureLongKey(Qt::Key aKey,
    Qt::KeyboardModifiers aModifiersMap, Qt::KeyboardModifiers aModifier,
    XqKeyCapture::LongFlags aLongType)
{
    return d->cancelCaptureLongKey(aKey, aModifiersMap, aModifier,
        aLongType);
}

/*!
 Deselects a given key from capturing long pressing. Requires a S60 key code (TKeyCode).
 \param aKey A S60 key code (TKeyCode).
 \param aModifiersMap
 \param aModifier 
 \retval Returns true if aKey was succesfully removed from the capturing system, otherwise returns false.
 */
bool XqKeyCapture::cancelCaptureLongKey(TUint aKey,
    Qt::KeyboardModifiers aModifiersMap, Qt::KeyboardModifiers aModifier,
    XqKeyCapture::LongFlags aLongType)
{
    return d->cancelCaptureLongKey(aKey, aModifiersMap, aModifier,
        aLongType);
}

/*!
 Deselects a given key from capturing pressing up and down. Requires a Qt key code.
 \param aKey A Qt key.
 \param aModifiersMap
 \param aModifier 
 \retval Returns true if aKey was succesfully removed from the capturing system, otherwise returns false.
 */
bool XqKeyCapture::cancelCaptureKeyUpAndDowns(Qt::Key aKey,
    Qt::KeyboardModifiers aModifiersMap, Qt::KeyboardModifiers aModifier)
{
    return d->cancelCaptureKeyUpAndDowns(aKey, aModifiersMap, aModifier);
}

/*!
 Deselects a given key from capturing pressing up and down. Requires a S60 key scan code (TStdScanCode).
 \param aKey A S60 key scan code (TStdScanCode).
 \param aModifiersMap
 \param aModifier 
 \retval Returns true if aKey was succesfully removed from the capturing system, otherwise returns false.
 */
bool XqKeyCapture::cancelCaptureKeyUpAndDowns(TUint aKey,
    Qt::KeyboardModifiers aModifiersMap, Qt::KeyboardModifiers aModifier)
{
    return d->cancelCaptureKeyUpAndDowns(aKey, aModifiersMap, aModifier);
}

/*!
 Returns latest error string.
 \retval Latest error string.
 */
QString XqKeyCapture::errorString() const
{
    return d->errorString();
}

/*!
 Returns latest error id.
 \retval Latest error id.
 */
int XqKeyCapture::errorId() const
{
    return d->errorId();
}
