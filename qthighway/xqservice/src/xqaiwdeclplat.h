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
* Description: Platform wide application interworking declarations
*              All declations shall have XQ prefix
*
*/

#ifndef XQAIWDECLPLAT_H
#define XQAIWDECLPLAT_H

#include <QLatin1String>

//
// All declations shall have XQ prefix
//

/*!
 FM Radio controlling interface and related operations
 Operation: command( int commandId )
  commandId: defined in radioserviceconst.h
  
*/
#define XQI_RADIO_CONTROL QLatin1String("com.nokia.symbian.IRadioControl")
#define XQOP_RADIO_CONTROL QLatin1String("command(int)")

/*!
 FM Radio monitoring interface and related operations
*/
#define XQI_RADIO_MONITOR QLatin1String("com.nokia.symbian.IRadioMonitor")
#define XQOP_RADIO_MONITOR QLatin1String("requestNotifications(void)")

// Platform service related constants

#endif
