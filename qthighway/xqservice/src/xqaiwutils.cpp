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

#include <e32std.h>
#include <apgcli.h>
#include <apacmdln.h> 
#include <apparc.h>
#include <apmstd.h>
#include <w32std.h>
#include <apgtask.h>

#include "xqaiwdecl.h"
#include "xqservicelog.h"
#include <xqserviceglobal.h>  // Error codes
#include <xqserviceipcconst.h>
#include "xqaiwutils.h"


class XQAiwUtilsPrivate : public QObject
{
    public:

        XQAiwUtilsPrivate();
        virtual ~XQAiwUtilsPrivate();
        
        void launchApplicationL(int applicationId, const QString &cmdArguments);
        int findApplicationFromApa(const QFile &file, int &applicationId);
        int findApplicationFromApa(const XQSharableFile &file, int &applicationId);
        bool applicationExists(int applicationId);
        int toIntFromHex(const QString &str, bool *ok);
        
    public:
        RApaLsSession apaSession;
    
};


XQAiwUtils::XQAiwUtils()
   : d(NULL)
{
    XQSERVICE_DEBUG_PRINT("XQAiwUtils::XQAiwUtils");
    d = new XQAiwUtilsPrivate();
}

XQAiwUtils::~XQAiwUtils()
{
    XQSERVICE_DEBUG_PRINT("XQAiwUtils::~XQAiwUtils");
    delete d;
};

int XQAiwUtils::launchApplication(int applicationId, const QString &cmdArguments)
{
    TInt error = KErrNone;
    TRAP(error, d->launchApplicationL(applicationId, cmdArguments));
    return mapError(error);
}

int XQAiwUtils::mapError(int symbianError)
{
    XQSERVICE_DEBUG_PRINT("XQAiwUtils::doMapErrors");
    XQSERVICE_DEBUG_PRINT("error: %d", symbianError);
    int error(XQService::ENoError);
    switch (symbianError)
    {
        case KErrNone:
        {
            error = XQService::ENoError;
            break;
        }
        
        case KErrPermissionDenied:
        case KErrServerTerminated:
        {
            error = XQService::EConnectionClosed;
            break;
        }
        case KErrServerBusy:
        {
            error = XQService::EConnectionError;
            break;
        }
        case KErrArgument:
        {
            error = XQService::EArgumentError;
            break;
        }
        case KErrNoMemory:
        {
            error = XQService::EIPCError;
            break;
        }
        case KErrNotFound:
        {
            error = XQService::EServerNotFound;
            break;
        }
        
        default:
        {
            error = XQService::EUnknownError;
            break;
        }
    }
    XQSERVICE_DEBUG_PRINT("error: %d", error);
    return error;
    
}

int XQAiwUtils::findApplication(const QFile &file, int &applicationId)
{
    XQSERVICE_DEBUG_PRINT("XQAiwUtils::findApplication %s", qPrintable(file.fileName()));
    TInt error = KErrNone;
    int appId = 0;
    error = d->findApplicationFromApa(file, appId);
    if (!error)
    {
        applicationId = appId;
    }
    return error;

}

int XQAiwUtils::findApplication(const XQSharableFile &file, int &applicationId)
{
    XQSERVICE_DEBUG_PRINT("XQAiwUtils::findApplication (handle)");
    TInt error = KErrNone;
    int appId = 0;
    error = d->findApplicationFromApa(file, appId);
    if (!error)
    {
        applicationId = appId;
    }
    return error;

}


int XQAiwUtils::findApplication(const QUrl &uri, int &applicationId)
{
    XQSERVICE_DEBUG_PRINT("XQAiwUtils::findapplication %s", qPrintable(uri.toString()));
    int appId = 0;
    bool idOk = false;
    if (uri.scheme() == XQURI_SCHEME_ACTIVITY)  // application://uid3
    {
        QString uid = uri.authority(); 
        XQSERVICE_DEBUG_PRINT("findApplication::authority=%s", qPrintable(uid));
        appId = d->toIntFromHex(uid, &idOk);
        XQSERVICE_DEBUG_PRINT("XQAiwUriDriver::appid=%x,%d", appId, idOk);

        if (idOk)
        {
            idOk = d->applicationExists(appId);
        }
    }
    else if (uri.scheme() == XQURI_SCHEME_FILE)  // file://
    {
        TInt err = d->findApplicationFromApa(uri.toLocalFile(), appId);
        idOk = (err == KErrNone);
    }

    if (idOk)
    {
        applicationId = appId;
        return mapError(KErrNone);
    }

    return mapError(KErrNotFound);

}

// Create space separated command line args
QString XQAiwUtils::createCmdlineArgs(const QList<QVariant> &args)
{
    XQSERVICE_DEBUG_PRINT("XQAiwUtils::createCmdlineArgs");
    
    QString argsStr = "";
    for ( int i = 0; i < args.size(); ++i )
    {
        QVariant v = args.at(i);
        QString s = v.toString();
        if (!s.isEmpty())
        {
            argsStr += (i==0 ? "" : " ");
            argsStr += s;
        }
    }

    return argsStr;
    
}


// Error error message for R&D purposes
QString XQAiwUtils::createErrorMessage(int errorCode, const QString context, const QString detail)
{
    QString txt;
    switch (errorCode)
    {
        case XQService::ENoError:
                txt =  "ENoError";
        break;

        case XQService::EConnectionError:
                txt ="EConnectionError";
        break;

        case XQService::EConnectionClosed:
                txt = "EConnectionClosed";
        break;

        case XQService::EServerNotFound:
                txt = "EServerNotFound";
        break;

        case XQService::EIPCError:
                txt = "EIPCError";
        break;

        case XQService::EUnknownError:
                txt = "EUnknownError";
        break;

        case XQService::ERequestPending:
                txt = "ERequestPending";
        break;

        case XQService::EMessageNotFound:
                txt = "EMessageNotFound";
        break;

        case XQService::EArgumentError:
                txt = "EArgumentError";
        break;

        default:
            txt = QString("AIW error: %1").arg(errorCode);
            break;

    }

    QString ret = "AIW error: ";
    ret += txt;
    ret += " (";
    ret += context;
    ret += ",";
    ret += detail;
    ret += ")";

    return ret;
}


void XQAiwUtilsPrivate::launchApplicationL(int applicationId, const QString &cmdArguments)
{
    XQSERVICE_DEBUG_PRINT("XQAiwUtils::launchApplication");
    XQSERVICE_DEBUG_PRINT("applicationId=%x, cmdArguments %s", applicationId, qPrintable(cmdArguments));

    TPtrC cmdArgs( reinterpret_cast<const TUint16*>(cmdArguments.utf16()) );
    TUid uid;
    uid.iUid = applicationId;

    RWsSession wsSession;
    User::LeaveIfError(wsSession.Connect());
    CleanupClosePushL(wsSession);

    TApaTaskList taskList( wsSession );
    TApaTask task = taskList.FindApp( uid );

    if ( task.Exists() )
    {
        // Switching
        XQSERVICE_DEBUG_PRINT("XQAiwUtils::launchApplication: switch to existing");
        // TODO: How to pass new aguments to  running process ? Use SendMessage ?
        task.BringToForeground();
        CleanupStack::PopAndDestroy();  // wsSession
    }
    else
    {
        // Start application
        TApaAppInfo aInfo;
        User::LeaveIfError( apaSession.GetAppInfo( aInfo, uid ) );
        CApaCommandLine* cmdLine = CApaCommandLine::NewLC();
        cmdLine->SetExecutableNameL( aInfo.iFullName );
        RProcess newApp;
        User::LeaveIfError(newApp.Create(aInfo.iFullName, cmdArgs));
        cmdLine->SetProcessEnvironmentL(newApp);
        newApp.Resume();
        newApp.Close(); // Close the handle (not the app)
        CleanupStack::PopAndDestroy(2);  // cmdLine, wsSession
    }

    XQSERVICE_DEBUG_PRINT("application started");

}

// ----- XQAiwUtilsPrivate --- 

XQAiwUtilsPrivate::XQAiwUtilsPrivate()
{
    apaSession.Connect();
}

XQAiwUtilsPrivate::~XQAiwUtilsPrivate()
{
    apaSession.Close();
}


int XQAiwUtilsPrivate::findApplicationFromApa(const QFile &file, int &applicationId)
{
    QString fileName = file.fileName();
    XQSERVICE_DEBUG_PRINT("XQAiwUtilsPrivate::::findApplicationFromApa %s", qPrintable(fileName));
    
    TPtrC name( reinterpret_cast<const TUint16*>(fileName.utf16()) );

    // Get the UID and MIME type for the given file name.
    TUid uid;
    uid.iUid=0;
    TDataType dataType;
    TInt err = apaSession.AppForDocument(name, uid, dataType);
    XQSERVICE_DEBUG_PRINT("\tFind status %d,%x", err, uid.iUid);
    if (err || uid.iUid == 0)
    {
        XQSERVICE_DEBUG_PRINT("\tHandler not found");
        return KErrNotFound;
    }

    applicationId = uid.iUid;  // return value

    XQSERVICE_DEBUG_PRINT("\tapplicationId=%x", applicationId);
    return KErrNone;
    
}

int XQAiwUtilsPrivate::findApplicationFromApa(const XQSharableFile &file, int &applicationId)
{
    XQSERVICE_DEBUG_PRINT("XQAiwUtilsPrivate::findApplicationFromApa (handle)");
    RFile fileHandle;
    if (!file.getHandle(fileHandle))
    {
        XQSERVICE_DEBUG_PRINT("\tInvalid handle");
        return KErrArgument;
    }

    // Get the UID and MIME type for the given file name.
    TUid uid;
    uid.iUid=0;
    TDataType dataType;
    TInt err = apaSession.AppForDocument(fileHandle, uid, dataType);
    XQSERVICE_DEBUG_PRINT("\tFind status %d,%x", err, uid.iUid);
    if (err || uid.iUid == 0)
    {
        XQSERVICE_DEBUG_PRINT("\tHandler not found");
        return KErrNotFound;
    }

    applicationId = uid.iUid;  // return value

    XQSERVICE_DEBUG_PRINT("\tapplicationId=%x", applicationId);
    return KErrNone;

}


bool XQAiwUtilsPrivate::applicationExists(int applicationId)
{
    TUid uid;
    uid.iUid = applicationId;

    TApaAppInfo aInfo;
    return apaSession.GetAppInfo( aInfo, uid ) == KErrNone;
    
}

//
// For some reason QString::toInt(0,16) does not work...
// Implement own converter
//
int XQAiwUtilsPrivate::toIntFromHex(const QString &str, bool *ok)
{
    int result=0;
    int power = 0;
    int base=16;
    QString s = str.toUpper();
    for (int i=s.length()-1; i >= 0; i--)
    {
        int val = (int)s[i].toLatin1();
        int num;
        if ((val >= (int)'A') && (val <= (int)'F'))
            num = 10 + (val - (int)'A');
        else if ((val >= (int)'0') && (val <= (int)'9'))
            num = val - (int)'0';
        else
        {
            *ok = false;
            return 0;
        }
        
        int multiplier = 1;
        for (int j=0; j < power; j++) 
            multiplier *= base; // Calculate power
        
        result += multiplier*num;
        power++;
    }

    *ok = true;

    return result;
}

