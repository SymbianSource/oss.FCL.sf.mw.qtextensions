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
#include "xqrequestinfo.h"
#include "xqrequestutil.h"

XQRequestInfo::XQRequestInfo()
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::XQRequestInfo");
}

XQRequestInfo::~XQRequestInfo()
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::~XQRequestInfo");
}

void XQRequestInfo::setEmbedded(bool on)
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::setEmbedded %d", on);
    setInfo(XQServiceUtils::OptEmbedded, on);
}

bool XQRequestInfo::isEmbedded() const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::isEmbedded");
    return info(XQServiceUtils::OptEmbedded).toBool();
}

void XQRequestInfo::setBackground(bool on)
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::setBackground %d", on);
    setInfo(XQServiceUtils::OptBackground, on);
}

bool XQRequestInfo::isBackground() const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::isBackground");
    return info(XQServiceUtils::OptBackground).toBool();
}

void XQRequestInfo::setForeground(bool on)
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::setForeground %d", on);
    setInfo(XQServiceUtils::OptForeground, on);
}

bool XQRequestInfo::isForeground() const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::isForeground");
    return info(XQServiceUtils::OptForeground).toBool();
}


bool XQRequestInfo::isSynchronous() const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::isSynchronous");
    return info(XQServiceUtils::OptSynchronous).toBool();
}


quint32 XQRequestInfo::clientSecureId() const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::clientSecureId");
    bool b=false;
    quint32 id = info(XQServiceUtils::InfoSID).toUInt(&b);
    XQSERVICE_DEBUG_PRINT("\tSID status=%d", b);
    return id;
}

quint32 XQRequestInfo::clientVendorId() const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::clientVendorId");
    bool b=false;
    quint32 id = info(XQServiceUtils::InfoVID).toUInt(&b);
    XQSERVICE_DEBUG_PRINT("\tVID status=%d", b);
    return id;
}

QSet<int> XQRequestInfo::clientCapabilities() const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::clientCapabilities");
    bool b=false;
    quint32 caps = info(XQServiceUtils::InfoCap).toUInt(&b);
    XQSERVICE_DEBUG_PRINT("\tCaps status", b);
    QSet<int> ret;
    if (!b)
        return ret;  // Empty set
    
    for(int i = 0; i < 32; i++)
    {
        quint32 ix = 0;
        ix |= 1 << i;
        if (caps & ix)
        {
            ret.insert(i);
        }
    }
    
    return ret;
}


void XQRequestInfo::setInfo(const QString &key, const QVariant &value)
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::setInfo %s:%s,%s",
        qPrintable(key), qPrintable(value.toString()), value.typeName());
    mInfo.insert(key, value);
}

QVariant XQRequestInfo::info(const QString &key) const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::info %s", qPrintable(key));
    QVariant v =  mInfo.value(key);
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::info type=%s,valid=%d,str=%s", v.typeName(), v.isValid(), qPrintable(v.toString()));
    return v;
}

QStringList XQRequestInfo::infoKeys() const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::infoKeys");
    return mInfo.keys();
}

int XQRequestInfo::id() const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::id");
    bool b=false;
    int id = info(XQServiceUtils::InfoId).toInt(&b);
    XQSERVICE_DEBUG_PRINT("\tId status=%d", b);
    if (!b)
    {
        return -1;
    }
    return id;
}

bool XQRequestInfo::isValid() const
{
    return !mInfo.isEmpty();
}

template <typename Stream> void XQRequestInfo::serialize(Stream &s) const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::serialize");
    s << mInfo;
}

template <typename Stream> void XQRequestInfo::deserialize(Stream &s)
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::de-serialize");
    s >> mInfo;
}

Q_IMPLEMENT_USER_METATYPE(XQRequestInfo)
