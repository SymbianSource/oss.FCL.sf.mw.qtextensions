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

//
// TODO: this file is only tempral while we wait for Qt patch approval and include
// 
 
#include "keymapper.h"
#include <e32keys.h>

QKeyMapper::QKeyMapper()
{
    fillKeyMap();
}

QKeyMapper::~QKeyMapper()
{
}

void QKeyMapper::fillKeyMap()
{
    using namespace Qt;
    keyMapping.append(KeyMapping(EKeyBackspace, EStdKeyBackspace, Key_Backspace));
    keyMapping.append(KeyMapping(EKeyTab, EStdKeyTab, Key_Tab));
    keyMapping.append(KeyMapping(EKeyEnter, EStdKeyEnter, Key_Enter));
    keyMapping.append(KeyMapping(EKeyEscape, EStdKeyEscape, Key_Escape));
    keyMapping.append(KeyMapping(EKeySpace, EStdKeySpace, Key_Space));
    keyMapping.append(KeyMapping(EKeyDelete, EStdKeyDelete, Key_Delete));
    keyMapping.append(KeyMapping(EKeyPrintScreen, EStdKeyPrintScreen, Key_SysReq));
    keyMapping.append(KeyMapping(EKeyPause, EStdKeyPause, Key_Pause));
    keyMapping.append(KeyMapping(EKeyHome, EStdKeyHome, Key_Home));
    keyMapping.append(KeyMapping(EKeyEnd, EStdKeyEnd, Key_End));
    keyMapping.append(KeyMapping(EKeyPageUp, EStdKeyPageUp, Key_PageUp));
    keyMapping.append(KeyMapping(EKeyPageDown, EStdKeyPageDown, Key_PageDown));
    keyMapping.append(KeyMapping(EKeyInsert, EStdKeyInsert, Key_Insert));
    keyMapping.append(KeyMapping(EKeyLeftArrow, EStdKeyLeftArrow, Key_Left));
    keyMapping.append(KeyMapping(EKeyRightArrow, EStdKeyRightArrow, Key_Right));
    keyMapping.append(KeyMapping(EKeyUpArrow, EStdKeyUpArrow, Key_Up));
    keyMapping.append(KeyMapping(EKeyDownArrow, EStdKeyDownArrow, Key_Down));
    keyMapping.append(KeyMapping(EKeyLeftShift, EStdKeyLeftShift, Key_Shift));
    keyMapping.append(KeyMapping(EKeyRightShift, EStdKeyRightShift, Key_Shift));
    keyMapping.append(KeyMapping(EKeyLeftAlt, EStdKeyLeftAlt, Key_Alt));
    keyMapping.append(KeyMapping(EKeyRightAlt, EStdKeyRightAlt, Key_AltGr));
    keyMapping.append(KeyMapping(EKeyLeftCtrl, EStdKeyLeftCtrl, Key_Control));
    keyMapping.append(KeyMapping(EKeyRightCtrl, EStdKeyRightCtrl, Key_Control));
    keyMapping.append(KeyMapping(EKeyLeftFunc, EStdKeyLeftFunc, Key_Super_L));
    keyMapping.append(KeyMapping(EKeyRightFunc, EStdKeyRightFunc, Key_Super_R));
    keyMapping.append(KeyMapping(EKeyCapsLock, EStdKeyCapsLock, Key_CapsLock));
    keyMapping.append(KeyMapping(EKeyNumLock, EStdKeyNumLock, Key_NumLock));
    keyMapping.append(KeyMapping(EKeyScrollLock, EStdKeyScrollLock, Key_ScrollLock));
    keyMapping.append(KeyMapping(EKeyF1, EStdKeyF1, Key_F1));
    keyMapping.append(KeyMapping(EKeyF2, EStdKeyF2, Key_F2));
    keyMapping.append(KeyMapping(EKeyF3, EStdKeyF3, Key_F3));
    keyMapping.append(KeyMapping(EKeyF4, EStdKeyF4, Key_F4));
    keyMapping.append(KeyMapping(EKeyF5, EStdKeyF5, Key_F5));
    keyMapping.append(KeyMapping(EKeyF6, EStdKeyF6, Key_F6));
    keyMapping.append(KeyMapping(EKeyF7, EStdKeyF7, Key_F7));
    keyMapping.append(KeyMapping(EKeyF8, EStdKeyF8, Key_F8));
    keyMapping.append(KeyMapping(EKeyF9, EStdKeyF9, Key_F9));
    keyMapping.append(KeyMapping(EKeyF10, EStdKeyF10, Key_F10));
    keyMapping.append(KeyMapping(EKeyF11, EStdKeyF11, Key_F11));
    keyMapping.append(KeyMapping(EKeyF12, EStdKeyF12, Key_F12));
    keyMapping.append(KeyMapping(EKeyF13, EStdKeyF13, Key_F13));
    keyMapping.append(KeyMapping(EKeyF14, EStdKeyF14, Key_F14));
    keyMapping.append(KeyMapping(EKeyF15, EStdKeyF15, Key_F15));
    keyMapping.append(KeyMapping(EKeyF16, EStdKeyF16, Key_F16));
    keyMapping.append(KeyMapping(EKeyF17, EStdKeyF17, Key_F17));
    keyMapping.append(KeyMapping(EKeyF18, EStdKeyF18, Key_F18));
    keyMapping.append(KeyMapping(EKeyF19, EStdKeyF19, Key_F19));
    keyMapping.append(KeyMapping(EKeyF20, EStdKeyF20, Key_F20));
    keyMapping.append(KeyMapping(EKeyF21, EStdKeyF21, Key_F21));
    keyMapping.append(KeyMapping(EKeyF22, EStdKeyF22, Key_F22));
    keyMapping.append(KeyMapping(EKeyF23, EStdKeyF23, Key_F23));
    keyMapping.append(KeyMapping(EKeyF24, EStdKeyF24, Key_F24));
    keyMapping.append(KeyMapping(EKeyOff, EStdKeyOff, Key_PowerOff));
//    keyMapping.append(KeyMapping(EKeyMenu, EStdKeyMenu, Key_Menu)); // Menu is EKeyApplication0
    keyMapping.append(KeyMapping(EKeyHelp, EStdKeyHelp, Key_Help));
    keyMapping.append(KeyMapping(EKeyDial, EStdKeyDial, Key_Call));
    keyMapping.append(KeyMapping(EKeyIncVolume, EStdKeyIncVolume, Key_VolumeUp));
    keyMapping.append(KeyMapping(EKeyDecVolume, EStdKeyDecVolume, Key_VolumeDown));
    keyMapping.append(KeyMapping(EKeyDevice0, EStdKeyDevice0, Key_Context1)); // Found by manual testing.
    keyMapping.append(KeyMapping(EKeyDevice1, EStdKeyDevice1, Key_Context2)); // Found by manual testing.
    keyMapping.append(KeyMapping(EKeyDevice3, EStdKeyDevice3, Key_Select));
//    keyMapping.append(KeyMapping(EKeyDevice7, EStdKeyDevice7, Key_Camera));  //not supported by qt yet
    keyMapping.append(KeyMapping(EKeyApplication0, EStdKeyApplication0, Key_Menu)); // Found by manual testing.
    keyMapping.append(KeyMapping(EKeyApplication1, EStdKeyApplication1, Key_Launch1)); // Found by manual testing.
    keyMapping.append(KeyMapping(EKeyApplication2, EStdKeyApplication2, Key_MediaPlay)); // Found by manual testing.
    keyMapping.append(KeyMapping(EKeyApplication3, EStdKeyApplication3, Key_MediaStop)); // Found by manual testing.
    keyMapping.append(KeyMapping(EKeyApplication4, EStdKeyApplication4, Key_MediaNext)); // Found by manual testing.
    keyMapping.append(KeyMapping(EKeyApplication5, EStdKeyApplication5, Key_MediaPrevious)); // Found by manual testing.
    keyMapping.append(KeyMapping(EKeyApplication6, EStdKeyApplication6, Key_Launch6));
    keyMapping.append(KeyMapping(EKeyApplication7, EStdKeyApplication7, Key_Launch7));
    keyMapping.append(KeyMapping(EKeyApplication8, EStdKeyApplication8, Key_Launch8));
    keyMapping.append(KeyMapping(EKeyApplication9, EStdKeyApplication9, Key_Launch9));
    keyMapping.append(KeyMapping(EKeyApplicationA, EStdKeyApplicationA, Key_LaunchA));
    keyMapping.append(KeyMapping(EKeyApplicationB, EStdKeyApplicationB, Key_LaunchB));
    keyMapping.append(KeyMapping(EKeyApplicationC, EStdKeyApplicationC, Key_LaunchC));
    keyMapping.append(KeyMapping(EKeyApplicationD, EStdKeyApplicationD, Key_LaunchD));
    keyMapping.append(KeyMapping(EKeyApplicationE, EStdKeyApplicationE, Key_LaunchE));
    keyMapping.append(KeyMapping(EKeyApplicationF, EStdKeyApplicationF, Key_LaunchF));
//    keyMapping.append(KeyMapping(EKeyApplication19, EStdKeyApplication19, Key_CameraFocus));  //not supported by qt yet
    keyMapping.append(KeyMapping(EKeyYes, EStdKeyYes, Key_Yes));
    keyMapping.append(KeyMapping(EKeyNo, EStdKeyNo, Key_No));    	
}

int QKeyMapper::mapS60KeyToQt(TUint s60key)
{
	int res = Qt::Key_unknown;
	for ( int i = 0, size = keyMapping.count(); i<size; i++){
		if ( keyMapping[i].s60KeyCode == s60key){
			res = keyMapping[i].qtKey;
			break;
		}
	}
	return res;
}

int QKeyMapper::mapS60ScanCodesToQt(TUint s60scanCode)
{
	int res = Qt::Key_unknown;
	for ( int i = 0, size = keyMapping.count(); i<size; i++){
		if ( keyMapping[i].s60ScanCode == s60scanCode){
			res = keyMapping[i].qtKey;
			break;
		}
	}
	return res;
}

int QKeyMapper::mapQtToS60Key(int qtKey)
{
    int res = KErrUnknown;
	for ( int i = 0, size = keyMapping.count(); i<size; i++){
		if ( keyMapping[i].qtKey == qtKey){
			res = keyMapping[i].s60KeyCode;
			break;
		}
	}
	return res;
}

int QKeyMapper::mapQtToS60ScanCodes(int qtKey)
{
	int res = KErrUnknown;
	for ( int i = 0, size = keyMapping.count(); i<size; i++){
		if ( keyMapping[i].qtKey == qtKey){
			res = keyMapping[i].s60ScanCode;
			break;
		}
	}
	return res;
}
