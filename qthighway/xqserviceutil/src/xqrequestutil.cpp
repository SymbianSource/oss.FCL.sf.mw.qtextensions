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

#include "xqservicelog.h"
#include "xqserviceerrdefs.h"

#include "xqrequestutil.h"
   
XQRequestUtil::XQRequestUtil()
{
    XQSERVICE_DEBUG_PRINT("XQRequestUtil::XQRequestUtil");
}

XQRequestUtil::~XQRequestUtil()
{
    XQSERVICE_DEBUG_PRINT("XQRequestUtil::~XQRequestUtil");
}

void XQRequestUtil::setSynchronous(bool on)
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::setSynchronous %d", on);
    mInfo.setInfo(XQServiceUtils::OptSynchronous, on);
}


const XQSharableFile *XQRequestUtil::getSharableFile(int index) const
{
    if (index >= 0 && index < mSharableFileArgs.size())
    {
        return &mSharableFileArgs.at(index);
    }

    return 0;
}

void XQRequestUtil::setSecurityInfo(const quint32 sid, const quint32 vid, const quint32 caps )
{
    XQSERVICE_DEBUG_PRINT("XQRequestUtil::setSecurityInfo");

    mInfo.setInfo(XQServiceUtils::InfoSID, sid);
    mInfo.setInfo(XQServiceUtils::InfoVID, vid);
    mInfo.setInfo(XQServiceUtils::InfoCap, caps);

}


int XQRequestUtil::mapError(int error)
{
    XQSERVICE_DEBUG_PRINT("XQRequestUtil::mapError");
    XQSERVICE_DEBUG_PRINT("\terror=%d", error);
    
    int mappedError(XQService::ENoError);

    switch (error)
    {
        case KErrNone:
        {
            mappedError = XQService::ENoError;
            break;
        }
        
        case KErrPermissionDenied:
        case KErrServerTerminated:
        {
            mappedError = XQService::EConnectionClosed;
            break;
        }
        case KErrServerBusy:
        {
            mappedError = XQService::EConnectionError;
            break;
        }
        case KErrArgument:
        {
            mappedError = XQService::EArgumentError;
            break;
        }

        case KErrNoMemory:
        {
            mappedError = XQService::EIPCError;
            break;
        }
        case KErrNotFound:
        {
            mappedError = XQService::EServerNotFound;
            break;
        }
        

        default:
        {
            mappedError = error;  // Leave it as is
            break;
        }
    }


    XQSERVICE_DEBUG_PRINT("\tmapper error=%d", mappedError);
    return mappedError;

}
