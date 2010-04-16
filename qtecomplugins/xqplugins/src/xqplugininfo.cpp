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
* Description:  Class implement plugin description
*
*/

#include "xqplugininfo.h"
#include "xqplugininfoprivate.h"

XQPluginInfo::XQPluginInfo()
:
d_ptr(new XQPluginInfoPrivate())
{
}

XQPluginInfo::XQPluginInfo(const XQPluginInfo &pluginInfo)
:
d_ptr(new XQPluginInfoPrivate(*pluginInfo.d_ptr))
{
}

XQPluginInfo::XQPluginInfo(quint32 id, 
                           quint32 version, 
                           const QString &dllName, 
                           const QString &opaqueData)
:
d_ptr(new XQPluginInfoPrivate(id, version, dllName, opaqueData))
{
}

XQPluginInfo &XQPluginInfo::operator=(const XQPluginInfo &other)
{
    if (this != &other) {
        if (d_ptr != 0){
            delete d_ptr;
        }
        d_ptr = new XQPluginInfoPrivate(*other.d_ptr);
    }
    return *this;
}

XQPluginInfo::~XQPluginInfo()
{
    if (d_ptr != 0){
        delete d_ptr;
    }
}

quint32 XQPluginInfo::uid() const
{
    return d_ptr->uid();
}

quint32 XQPluginInfo::version() const
{
    return d_ptr->version();
}

const QString& XQPluginInfo::dllName() const
{
    return d_ptr->dllName();
}

const QString& XQPluginInfo::opaqueData() const
{
    return d_ptr->opaqueData();
}
