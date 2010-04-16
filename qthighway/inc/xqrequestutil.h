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

#ifndef XQREQUESTUTIL_H
#define XQREQUESTUTIL_H

#include <QList>
#include <QVariant>

#include <xqserviceglobal.h>
#include <xqaiwinterfacedescriptor.h>
#include "xqrequestinfo.h"
#include "xqsharablefile.h"


//
// XQRequestUtil is internal helper class for QtHighway request handling
//
// Option key names for XQRequestInfo
namespace XQServiceUtils
{
    static const char * OptEmbedded= "XQEmb";    // Option Embedded / Non-Embedded call
    static const char * OptBackground= "XQBg";   // Option Service to Background / Foreground
    static const char * OptSynchronous= "XQSync"; // Option Syncronous / Asynchronous call
    static const char * InfoSID= "XQSid"; // Client secure ID
    static const char * InfoVID= "XQVid"; // Client vendor ID
    static const char * InfoCap= "XQCap"; //Client cap
    
    // Startup arguments
    static const char * StartupArgEmbedded = "embedded=yes";
    static const char * StartupArgService = "service=yes";
}


class XQSERVICEUTIL_EXPORT XQRequestUtil
{
    public:
        XQRequestUtil() ;
        virtual ~XQRequestUtil() ;

    // Helper to set the option OptSynchronous 
    void setSynchronous(bool on);
    void setSecurityInfo(const quint32 sid, const quint32 vid, const quint32 caps );
    const XQSharableFile *getSharableFile(int index) const;
    static int mapError(int error);
        
    public:
        XQAiwInterfaceDescriptor mDescriptor;
        XQRequestInfo mInfo;
        QList<XQSharableFile> mSharableFileArgs;   // Use list even though one file possible to transfer
};

#endif
