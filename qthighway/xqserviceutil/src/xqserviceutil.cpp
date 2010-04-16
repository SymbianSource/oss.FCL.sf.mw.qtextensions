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
#include "xqrequestutil.h"
#include "xqserviceutil.h"

#include <QCoreApplication>
#include <QStringList>

#include <apgcli.h>
#include <apacmdln.h>
#include <apgtask.h>
#include <coemain.h>
#include <e32std.h>
#include <w32std.h> // RWsSession
#include <apacmdln.h>
#include <eikenv.h>
/*
void XQServiceUtil::hideFromFsw( bool hide )
{
    RAknUiServer uiServer;
    int sid = RProcess().SecureId().iId;
    uiServer.Connect();
    uiServer.HideApplicationFromFsw( hide, sid );
    uiServer.Close();
}
*/

void XQServiceUtil::toBackground( bool value )
{
    XQSERVICE_DEBUG_PRINT("XQServiceUtil::toBackground");
    XQSERVICE_DEBUG_PRINT("value: %d", value);
    RWsSession ws;
    int sid = RProcess().SecureId().iId;
    XQSERVICE_DEBUG_PRINT("sid: %d", sid);
    if (ws.Connect() == KErrNone) {
        XQSERVICE_DEBUG_PRINT("Connected to window server");
        TApaTaskList tasklist(ws);
        TApaTask task = tasklist.FindApp(TUid::Uid(sid));
        XQSERVICE_DEBUG_PRINT("task.Exists(): %x", task.Exists());
        if (task.Exists()) {
            if (value) {
                task.SendToBackground();
            }
            else {
                task.BringToForeground();
            }
        }

        ws.Close();
    }
}

bool XQServiceUtil::isEmbedded()
{
    XQSERVICE_DEBUG_PRINT("XQServiceUtil::isEmbedded");
    QStringList args = QCoreApplication::arguments();
    foreach (QString arg, args) {
        XQSERVICE_DEBUG_PRINT("arg: %s", qPrintable(arg));
        if (!arg.compare(QString::fromLatin1(XQServiceUtils::StartupArgEmbedded),Qt::CaseInsensitive)) {
            XQSERVICE_DEBUG_PRINT("Is embedded");
            return true;
        }
    }
    XQSERVICE_DEBUG_PRINT("Not embedded");
    return false;
}

bool XQServiceUtil::isService()
{
    XQSERVICE_DEBUG_PRINT("XQServiceUtil::isService");
    QStringList args = QCoreApplication::arguments();
    foreach (QString arg, args) {
        XQSERVICE_DEBUG_PRINT("arg: %s", qPrintable(arg));
        if (!arg.compare(QString::fromLatin1(XQServiceUtils::StartupArgService),Qt::CaseInsensitive)) {
            XQSERVICE_DEBUG_PRINT("Is service");
            return true;
        }
    }
    XQSERVICE_DEBUG_PRINT("Not service");
    return false;
}

