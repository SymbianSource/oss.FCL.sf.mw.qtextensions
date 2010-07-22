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
#define XQOP_IMAGE_FETCH QLatin1String("fetch()")

/*!
 Music fetching interface and related operations
*/
#define XQI_MUSIC_FETCH QLatin1String("com.nokia.symbian.IMusicFetch")
#define XQOP_MUSIC_FETCH QLatin1String("fetch()")

/*!
 Tones fetching interface and related operations
*/
#define XQI_TONE_FETCH QLatin1String("com.nokia.symbian.IToneFetch")
#define XQOP_TONE_FETCH QLatin1String("fetch()")

/*!
 Video fetching interface and related operations
*/
#define XQI_VIDEO_FETCH QLatin1String("com.nokia.symbian.IVideoFetch")
#define XQOP_VIDEO_FETCH QLatin1String("fetch()")

/*!
 Video collection browser interface and related operations
*/
#define XQI_VIDEO_BROWSE QLatin1String("com.nokia.symbian.IVideoBrowse")
#define XQOP_VIDEO_BROWSE QLatin1String("browseVideos(int, int)")

/*!
 Video player interface and related operations
*/
#define XQI_VIDEO_PLAY QLatin1String("com.nokia.symbian.IVideoView")
#define XQOP_VIDEO_PLAY QLatin1String("playMedia(QString)")
#define XQOP_VIDEO_PLAY_PD QLatin1String("playMedia(QString, int)")
#define XQOP_VIDEO_CLOSE QLatin1String("closePlayer()")

/*!
 Camera capture interface and related operations
 Operation: capture(int mode, QVariantMap parameters)
  mode: image = 0, video = 1
  parameters: 
   CameraIndex: int:  primary = 0, secondary = 1
   Quality: int: 0 = default, 1 = lowest, 2 = highest
   AllowModeSwitch: bool
   AllowCamera_switch: bool
   allow_quality_change: bool
*/
#define XQI_CAMERA_CAPTURE QLatin1String("com.nokia.symbian.ICameraCapture")
#define XQOP_CAMERA_CAPTURE QLatin1String("capture(int,QVariantMap)")
#define XQCAMERA_INDEX QLatin1String("CameraIndex")
#define XQCAMERA_QUALITY QLatin1String("Quality")
#define XQCAMERA_MODE_SWITCH QLatin1String("AllowModeSwitch")
#define XQCAMERA_INDEX_SWITCH QLatin1String("AllowCameraSwitch")
#define XQCAMERA_QUALITY_CHANGE QLatin1String("AllowQualityChange")

/*!
 Incomplete list of interfaces.
 To be completed with operation signatures and related declarations.
*/
#define XQI_LOG_VIEW QLatin1String("com.nokia.symbian.ILogsView")
#define XQI_CONTACT_VIEW QLatin1String("com.nokia.symbian.IContactView")
#define XQI_CONTACT_FETCH QLatin1String("com.nokia.symbian.IContactFetch")
#define XQI_FILE_FETCH QLatin1String("com.nokia.symbian.IFileFetch")
#define XQI_FILE_SHARE QLatin1String("com.nokia.symbian.IFileShare")

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

/*!
 A key name for XQRequestInfo object to pass a window title
 to be shown in service application instead of the default title.
 Service application is responsible to handle the key value.
 \see XQApplicationMgr::XQRequestInfo::setInfo()
*/
#define XQINFO_KEY_WINDOW_TITLE QLatin1String("WindowTitle")


#endif
