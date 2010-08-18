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

// ======== LOCAL FUNCTIONS ========
static bool isKeySet(const QString &key)
{
    XQSERVICE_DEBUG_PRINT("XQServiceUtil isKeySet %s", qPrintable(key));
    QStringList args = QCoreApplication::arguments();
    foreach (QString arg, args)
    {
        XQSERVICE_DEBUG_PRINT("arg: %s", qPrintable(arg));
        if (!arg.compare(key,Qt::CaseInsensitive))
        {
            XQSERVICE_DEBUG_PRINT("Key set");
            return true;
        }
    }
    XQSERVICE_DEBUG_PRINT("Key not set");
    return false;
    
}

static bool isKeySet(int argc, char **argv, const QString &key)
{
    XQSERVICE_DEBUG_PRINT("XQServiceUtil argv isKeySet %s", qPrintable(key));
    for (int i=0; i < argc; i++)
    {
        QString arg(argv[i]);
        XQSERVICE_DEBUG_PRINT("arg: %s", qPrintable(arg));
        if (!arg.compare(key,Qt::CaseInsensitive)) {
            XQSERVICE_DEBUG_PRINT("Key set");
            return true;
        }
    }
    XQSERVICE_DEBUG_PRINT("Key not set");
    return false;

}



static QString keyValue(const QString &key)
{
    XQSERVICE_DEBUG_PRINT("XQServiceUtil keyValue %s", qPrintable(key));
    QString ret;
    QStringList args = QCoreApplication::arguments();
    foreach (QString arg, args) {
        if (arg.contains(key,Qt::CaseInsensitive)) {
            QStringList l= arg.split("=");
            ret = l.value(1);
        }
    }
    XQSERVICE_DEBUG_PRINT("key value=%s", qPrintable(ret));
    return ret;
}


static QString keyValue(int argc, char **argv, const QString &key)
{
    XQSERVICE_DEBUG_PRINT("XQServiceUtil argv keyValue %s", qPrintable(key));
    QString ret;
    for (int i=0; i < argc; i++)
    {
        QString arg(argv[i]);
        XQSERVICE_DEBUG_PRINT("arg: %s", qPrintable(arg));
        if (arg.contains(key,Qt::CaseInsensitive)) {
            QStringList l= arg.split("=");
            ret = l.value(1);
        }
    }
    XQSERVICE_DEBUG_PRINT("key value=%s", qPrintable(ret));
    return ret;
}


// ======== MEMBER FUNCTIONS ========
void XQServiceUtil::toBackground( bool value )
{
    XQSERVICE_DEBUG_PRINT("XQServiceUtil::toBackground");
    XQSERVICE_DEBUG_PRINT("value: %d", value);
    RWsSession ws;
    int sid = RProcess().SecureId().iId;  // Assumes UID3 == SID !!!
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
    return isKeySet(QString::fromLatin1(XQServiceUtils::StartupArgEmbedded));
}

bool XQServiceUtil::isService()
{
    XQSERVICE_DEBUG_PRINT("XQServiceUtil::isService");
    return isKeySet(QString::fromLatin1(XQServiceUtils::StartupArgService));
}

QString XQServiceUtil::interfaceName()
{
    XQSERVICE_DEBUG_PRINT("XQServiceUtil::interfaceName");
    return keyValue(QString::fromLatin1(XQServiceUtils::StartupArgInterfaceName));
}

QString XQServiceUtil::operationName()
{
    XQSERVICE_DEBUG_PRINT("XQServiceUtil::operationName");
    return keyValue(QString::fromLatin1(XQServiceUtils::StartupArgOperationName));
}

QString XQServiceUtil::serviceName()
{
    XQSERVICE_DEBUG_PRINT("XQServiceUtil::serviceName");
    return keyValue(QString::fromLatin1(XQServiceUtils::StartupArgServiceName));
}

bool XQServiceUtil::isEmbedded(int argc, char **argv)
{
    XQSERVICE_DEBUG_PRINT("XQServiceUtil::isEmbedded argv");
    return isKeySet(argc,argv,QString::fromLatin1(XQServiceUtils::StartupArgEmbedded));
}


bool XQServiceUtil::isService(int argc, char **argv)
{
    XQSERVICE_DEBUG_PRINT("XQServiceUtil::isService argv");
    return isKeySet(argc,argv,QString::fromLatin1(XQServiceUtils::StartupArgService));
}

QString XQServiceUtil::interfaceName(int argc, char **argv)
{
    XQSERVICE_DEBUG_PRINT("XQServiceUtil::interfaceName");
    return keyValue(argc,argv,QString::fromLatin1(XQServiceUtils::StartupArgInterfaceName));
}


QString XQServiceUtil::operationName(int argc, char **argv)
{
    XQSERVICE_DEBUG_PRINT("XQServiceUtil::operationName argv");
    return keyValue(argc,argv,QString::fromLatin1(XQServiceUtils::StartupArgOperationName));
}

QString XQServiceUtil::serviceName(int argc, char **argv)
{
    XQSERVICE_DEBUG_PRINT("XQServiceUtil::serviceName argv");
    return keyValue(argc,argv,QString::fromLatin1(XQServiceUtils::StartupArgServiceName));
}

