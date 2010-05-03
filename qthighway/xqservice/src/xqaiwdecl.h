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

/*!
 Image fetching interface and related operations
*/
#define XQI_IMAGE_FETCH QLatin1String("com.nokia.symbian.IImageFetch")
#define XQOP_IMAGE_FETCH QLatin1String("fetch(void)")

/*!
 Music fetching interface and related operations
*/
#define XQI_MUSIC_FETCH QLatin1String("com.nokia.symbian.IMusicFetch")
#define XQOP_MUSIC_FETCH QLatin1String("fetch(void)")

/*!
 Video fetching interface and related operations
*/
#define XQI_VIDEO_FETCH QLatin1String("com.nokia.symbian.IVideoFetch")
#define XQOP_VIDEO_FETCH QLatin1String("fetch(void)")

/*!
 Camera capture interface and related operations
 Operation: capture(int mode, const QVariantMap &parameters)
  mode: image = 0, video = 1
  parameters: 
   CameraIndex: int:  primary = 0, secondary = 1
   Quality: int: 0 = default, 1 = lowest, 2 = highest
   AllowModeSwitch: bool
   AllowCamera_switch: bool
   allow_quality_change: bool
*/
#define XQI_CAMERA_CAPTURE QLatin1String("com.nokia.symbian.ICameraCapture")
#define XQOP_CAMERA_CAPTURE QLatin1String("capture(int, const QVariantMap &)")
#define XQCAMERA_INDEX QLatin1String("CameraIndex")
#define XQCAMERA_QUALITY QLatin1String("Quality")
#define XQCAMERA_MODE_SWITCH QLatin1String("AllowModeSwitch")
#define XQCAMERA_INDEX_SWITCH QLatin1String("AllowCameraSwitch")
#define XQCAMERA_QUALITY_CHANGE QLatin1String("AllowQualityChange")

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

/*!
 Service XML custom property, which contains the name of text key that
 contains the service status
 \see XQApplicationMgr::isEnabled()
*/
#define XQCUSTOM_PROP_AIW_SERVICE_STATUS QLatin1String("aiw_status")

#endif
