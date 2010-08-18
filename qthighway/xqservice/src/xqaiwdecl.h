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


//
// Common Application Interworking declarations
// ---------------------------------------------
//

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
 File sharing interface and related operations
*/
#define XQI_FILE_SHARE QLatin1String("com.nokia.symbian.IFileShare")
#define XQOP_FILE_SHARE QLatin1String("send(QVariant)")

/*!
 The scheme of the activity URI
*/
#define XQURI_SCHEME_ACTIVITY QLatin1String("appto")
/*!
 Activity key name in query part of the activity URI
*/
#define XQURI_KEY_ACTIVITY_NAME QLatin1String("activityname")

/*!
 Miscellaneous needed URI schemes
*/
#define XQURI_SCHEME_HTTP QLatin1String("http")
#define XQURI_SCHEME_HTTPS QLatin1String("https")
#define XQURI_SCHEME_MAILTO QLatin1String("mailto")
#define XQURI_SCHEME_FILE QLatin1String("file")


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


//
// Media related Application Interworking declarations
// ---------------------------------------------------
//

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
namespace XQService
{
    enum VideoBrowseSortKey
    {
        SortUndefined = 0,
        SortDateTime  = 1,
        SortTitle     = 2,
        SortSize      = 3
    };
}
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


//
// Logs related Application Interworking declarations
// ---------------------------------------------------
//

/*!
  Logs view interface and related operations constants
  Operation: XQOP_LOGS_SHOW   (show(QVariantMap))
    parameters: 
      XQLOGS_VIEW_INDEX: enum LogsViewIndex: 0 - all recent, 1 - received, 2 - called, 3 - missed
      XQLOGS_SHOW_DIALPAD: bool
      XQLOGS_DIALPAD_TEXT: QString
*/
namespace XQService
{  
    enum LogsViewIndex
    {
        LogsViewAll = 0,
        LogsViewReceived,
        LogsViewCalled,
        LogsViewMissed
    };
}
#define XQI_LOGS_VIEW QLatin1String("com.nokia.symbian.ILogsView") 
#define XQOP_LOGS_SHOW QLatin1String("show(QVariantMap)")
#define XQLOGS_VIEW_INDEX QLatin1String("ViewIndex")
#define XQLOGS_SHOW_DIALPAD QLatin1String("ShowDialpad")
#define XQLOGS_DIALPAD_TEXT QLatin1String("DialpadText")

//
// Contacts related Application Interworking declarations
// ------------------------------------------------------
//

/*!
  Contact editing interface.
 */
#define XQI_CONTACTS_EDIT QLatin1String("com.nokia.symbian.IContactsEdit")

/*!
 Launches editor for creating a new contact with a given detail.
    Parameter1: Indicates what type of contact field is supplied.
        One of the following values:
          QContactPhoneNumber::DefinitionName
          QContactEmailAddress::DefinitionName
          QContactOnlineAccount::DefinitionName
     (you need to include qcontactdetails.h)
    Parameter2: The actual detail value, for eg. "123456" 
 \return One of these constants defined in cntservicescontact.h:
         KCntServicesReturnValueContactSaved
         KCntServicesReturnValueContactDeleted
         KCntServicesReturnValueContactNotModified
 */
#define XQOP_CONTACTS_EDIT_CREATE_NEW QLatin1String("editCreateNew(QString,QString)")

/*!
 Launches editor for creating a new contact with a given detail.
 Becomes available in wk30.
    Parameter1: Indicates what type of contact field is supplied.
        One of the following values:
          QContactPhoneNumber::DefinitionName
          QContactEmailAddress::DefinitionName
          QContactOnlineAccount::DefinitionName
      (you need to include qcontactdetails.h)
    Parameter2: The actual detail value, for eg. "123456"
    Parameter3: sub type, for eg. QContactPhoneNumber::SubTypeMobile
 \return One of these constants defined in cntservicescontact.h:
         KCntServicesReturnValueContactSaved
         KCntServicesReturnValueContactDeleted
         KCntServicesReturnValueContactNotModified
 */
#define XQOP_CONTACTS_EDIT_CREATE_NEW_WITH_SUBTYPE QLatin1String("editCreateNew(QString,QString,QString)")

/*!
 Launches editor for creating a new contact based on vCard  indicated in arg.
 The user is able to edit and save the contact. 
 Parameter1: vCard file name including the full path.
 \return One of these constants defined in cntservicescontact.h:
         KCntServicesReturnValueContactSaved
         KCntServicesReturnValueContactDeleted
         KCntServicesReturnValueContactNotModified
 */
#define XQOP_CONTACTS_EDIT_CREATE_NEW_VCARD QLatin1String("editCreateNewFromVCard(QString)")

/*!
 Launch editor for existing contact that is specified with contact ID. 
 Only the default backend storage is supported.
Parameter1: Contact ID.
 \return One of these constants defined in cntservicescontact.h:
         KCntServicesReturnValueContactSaved
         KCntServicesReturnValueContactDeleted
         KCntServicesReturnValueContactNotModified
 */
#define XQOP_CONTACTS_EDIT_EXISTING QLatin1String("editExisting(int)")

/*!
 Launch editor for updating an existing contact with a given detail.
 A contact fetch view is opened first, where the user can choose which contact
 to update. 
    Parameter1: Indicates what type of contact field is supplied.
        One of the following values:
          QContactPhoneNumber::DefinitionName
          QContactEmailAddress::DefinitionName
          QContactOnlineAccount::DefinitionName
      (you need to include qcontactdetails.h)
    Parameter2: The actual detail value, for eg. "123456"
 \return One of these constants defined in cntservicescontact.h:
         KCntServicesReturnValueContactSaved
         KCntServicesReturnValueContactDeleted
         KCntServicesReturnValueContactNotModified
 */
#define XQOP_CONTACTS_EDIT_UPDATE_EXISTING QLatin1String("editUpdateExisting(QString,QString)")


/*!
 Launch editor for updating an existing contact with a given detail.
 Becomes available in wk30.
 A contact fetch view is opened first, where the user can choose which contact
 to update. 
    Parameter1: Indicates what type of contact field is supplied.
        One of the following values:
          QContactPhoneNumber::DefinitionName
          QContactEmailAddress::DefinitionName
          QContactOnlineAccount::DefinitionName
      (you need to include qcontactdetails.h)
    Parameter2: The actual detail value, for eg. "123456"
 \return One of these constants defined in cntservicescontact.h:
         KCntServicesReturnValueContactSaved
         KCntServicesReturnValueContactDeleted
         KCntServicesReturnValueContactNotModified
 */
#define XQOP_CONTACTS_EDIT_UPDATE_EXISTING_WITH_SUBTYPE QLatin1String("editUpdateExisting(QString,QString,QString)")

/*!
 Contacts fetching interface.
 */
#define XQI_CONTACTS_FETCH QLatin1String("com.nokia.symbian.IContactsFetch")

/*!
 Fetch multiple contacts
 cntservicescontact.h has some constant definitions and a class for
 transforming the result.
 Parameter1: Title string.
 Parameter2: Action type string, from cntservicescontact.h 
 \return An object of type CntServicesContactList containing the contact ID.
 */
#define XQOP_CONTACTS_FETCH_MULTIPLE QLatin1String("multiFetch(QString,QString)")

/*!
 Fetch multiple contacts
 cntservicescontact.h has some constant definitions and a class for
 transforming the result.
 Parameter1: Title string.
 Parameter2: Action type string, from cntservicescontact.h 
 \return An object of type CntServicesContactList containing the contact ID.
 */
#define XQOP_CONTACTS_FETCH_SINGLE QLatin1String("singleFetch(QString,QString)")

/*!
 Contacts viewing interface.
 */
#define XQI_CONTACTS_VIEW QLatin1String("com.nokia.symbian.IContactsView")

/*!
 Opens contact card.
 Parameter1: Contact ID.
 */
#define XQOP_CONTACTS_VIEW_CONTACT_CARD QLatin1String("openContactCard(int)")

/*!
 Open contact card for in-memory contact which is not saved yet.
 In the UI there is an option to save the contact either by updating an
 existing contact or by creating a new one. 

    Parameter1: Indicates what type of contact field is supplied.
        One of the following values:
          QContactPhoneNumber::DefinitionName
          QContactEmailAddress::DefinitionName
          QContactOnlineAccount::DefinitionName
      (you need to include qcontactdetails.h)
    Parameter2: The actual detail value, for eg. "123456"
 */
#define XQOP_CONTACTS_VIEW_TEMP_CONTACT_CARD_WITH_DETAIL QLatin1String("openTemporaryContactCard(QString,QString)")

/*!
 Open the group members view.
 On the flip side of the group members view, there is the "group action" view.
 Parameter: The group ID. (native ID)
            Only groups in the main Qt Mobility backend are supported.
*/
#define XQOP_CONTACTS_VIEW_GROUP QLatin1String("openGroup(int)")

//
// Messaging related Application Interworking declarations
// ------------------------------------------------------
//
#define XQI_MESSAGE_SEND QLatin1String("com.nokia.symbian.IMessageSend")


/*!
 Incomplete list of interfaces.
 To be completed with operation signatures and related declarations.
*/
#define XQI_FILE_FETCH QLatin1String("com.nokia.symbian.IFileFetch")



#endif
