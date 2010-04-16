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
* Description: Common application interworking declarations 
*              All declations shall have XQ prefix
*
*/

#ifndef XQAIWDECL_H
#define XQAIWDECL_H

#include <QLatin1String>

//
// All declations shall have XQ prefix
//


// Public service related constants

/*!
 File viewing interface and related operations
*/
#define XQI_FILE_VIEW QLatin1String("com.nokia.symbian.IFileView")
#define XQOP_FILE_VIEW QLatin1String("view(QString)")
#define XQOP_FILE_VIEW_SHARABLE QLatin1String("view(XQSharableFile)")

/*!
 URI viewing interface and related operations
*/
#define XQI_URI_VIEW QLatin1String("com.nokia.symbian.IUriView")
#define XQOP_URI_VIEW QLatin1String("view(QString)")


// Public URI related constants
/*!
 The scheme of the activity URI
*/
#define XQURI_SCHEME_ACTIVITY QLatin1String("appto")
#define XQURI_SCHEME_HTTP QLatin1String("http")
#define XQURI_SCHEME_HTTPS QLatin1String("https")
#define XQURI_SCHEME_MAILTO QLatin1String("mailto")
#define XQURI_SCHEME_FILE QLatin1String("file")

/*!
 Activity key name in query part of the activity URI
*/
#define XQURI_KEY_ACTIVITY_NAME QLatin1String("activityname")


// Common Application Interworking related constants

/*!
 Service XML custom property, which contains the name of text resource file that
 contains the CUSTOM_PROP_AIW_TEXT.
 \see XQAiwRequest::createAction()
*/
#define XQCUSTOM_PROP_AIW_TEXT_FILE QLatin1String("aiw_action_text_file")

/*!
 Service XML custom property, that contains the Text Id within the
 CUSTOM_PROP_AIW_TEXT_FILE.  The localized text is attached to
 to QAction returned by the XQAiwRequest::createAction()
*/
#define XQCUSTOM_PROP_AIW_TEXT QLatin1String("aiw_action_text")

/*!
 Service XML custom property, that contains the name of the icon to be attached
 to QAction returned by the XQAiwRequest::createAction()
*/
#define XQCUSTOM_PROP_AIW_ICON QLatin1String("aiw_action_icon")

/*!
 Service XML custom property, that contains schemes the XQI_URI_VIEW can handle
*/
#define XQCUSTOM_PROP_SCHEMES QLatin1String("schemes")

#endif
