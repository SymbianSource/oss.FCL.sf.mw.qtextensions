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

#include <xqservicerequest.h>
#include <xqservicemanager.h>
#include <QList>
#include "xqservicelog.h"
#include "xqaiwutils.h"
#include "xqaiwuridriver.h"
#include "xqaiwdecl.h"
#include "xqappmgr_p.h"

XQApplicationManagerPrivate::XQApplicationManagerPrivate():
   serviceMgr(0),
   aiwUtilities(0)
   
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate");
}

XQApplicationManagerPrivate::~XQApplicationManagerPrivate()
{
    XQSERVICE_DEBUG_PRINT("~XQApplicationManagerPrivate");
    delete serviceMgr;
    delete aiwUtilities;
}

XQAiwRequest* XQApplicationManagerPrivate::create( const QString &interface, const QString &operation, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::create(interface)");
    return create("", interface, operation, embedded);
}

XQAiwRequest* XQApplicationManagerPrivate::create(
    const QString &service, const QString &interface, const QString &operation, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::create(service+interface)");

    if (!serviceMgr)
        serviceMgr = new XQServiceManager();
    if (!serviceMgr)
    {
        XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: Can not create service manager");
        return 0;
    }

    QList<XQAiwInterfaceDescriptor> impls;
    if (service.isEmpty())
        impls = serviceMgr->findInterfaces(interface);
    else
        impls = serviceMgr->findInterfaces(service, interface);

    // Pick up the first implementation
    if (impls.count())
    {
        return new XQAiwRequest(impls[0], operation, embedded);
    }
    return 0;
}


XQAiwRequest* XQApplicationManagerPrivate::create(
    const XQAiwInterfaceDescriptor &implementation, const QString &operation, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::create (impl)");
    return new XQAiwRequest(implementation, operation, embedded);
}

XQAiwRequest* XQApplicationManagerPrivate::create(
    const QUrl &uri, const XQAiwInterfaceDescriptor *implementation, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::create (uri)");
    XQSERVICE_DEBUG_PRINT("uri=%s", qPrintable(uri.toString()));
    
    if (!uri.isValid())
    {
        XQSERVICE_CRITICAL_PRINT("Invalid URI %s", qPrintable(uri.toString()));
        return 0;
    }

    if (hasCustomHandler(uri))
    {
        XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::custom handler applied");
        XQAiwInterfaceDescriptor nullDescr;
        return new XQAiwRequest(uri, nullDescr, XQOP_URI_VIEW);
    }
    
    const XQAiwInterfaceDescriptor *impl = implementation;
    QList<XQAiwInterfaceDescriptor> impls;
    if (impl == 0) 
    {
        // Implementation not given, find first one        
        impls = list(uri);
        if (impls.count())
        {
            impl = &impls[0];
            XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::create (impl) use first)");
        }
    }

    XQAiwRequest *req = 0;
    if (impl != 0)
    {
        // Create service request for viewing the URI
        XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::create (impl) creating request)");
        req = new XQAiwRequest(uri, *impl, XQOP_URI_VIEW);
        if (req)
        {
            req->setEmbedded(embedded);
        }
    }

    return req;

    
}

XQAiwRequest* XQApplicationManagerPrivate::create(
     const QFile &file, const XQAiwInterfaceDescriptor *implementation, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::create(file)");
    XQSERVICE_DEBUG_PRINT("file=%s", qPrintable(file.fileName()));

    const XQAiwInterfaceDescriptor *impl = implementation;
    QList<XQAiwInterfaceDescriptor> impls;
    if (impl == 0) 
    {
        // Implementation not given, find services capable of handling the "file"
        impls = list(file);
        if (impls.count())
        {
            impl = &impls[0];
        }
    }

    XQAiwRequest * req = 0;
    if (impl != 0)
    {
        QVariant v = impl->property(XQAiwInterfaceDescriptor::ImplementationId);
        req = new XQAiwRequest(file, *impl, XQOP_FILE_VIEW);
        if (req)
        {
            req->setEmbedded(embedded);
            XQSERVICE_DEBUG_PRINT("File handled by %x", v.toInt());
        }
    }
    
    return req;
}

XQAiwRequest* XQApplicationManagerPrivate::create(
    const XQSharableFile &file, const XQAiwInterfaceDescriptor *implementation, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::create(XQSharableFile)");
    if (!file.isValid())
    {
        XQSERVICE_DEBUG_PRINT("\tInvalid XQSharableFile)");
        return 0;
    }
    
    const XQAiwInterfaceDescriptor *impl = implementation;
    QList<XQAiwInterfaceDescriptor> impls;
    if (impl == 0) 
    {
        // Implementation not given, find services capable of handling the "file"
        impls = list(file);
        if (impls.count())
        {
            impl = &impls[0];
        }
    }

    XQAiwRequest * req = 0;
    if (impl != 0)
    {
        QVariant v = impl->property(XQAiwInterfaceDescriptor::ImplementationId);
        req = new XQAiwRequest(file, *impl, XQOP_FILE_VIEW_SHARABLE);
        if (req)
        {
            req->setEmbedded(embedded);
            XQSERVICE_DEBUG_PRINT("File handled by %x", v.toInt());
        }
    }

    return req;
}



QList<XQAiwInterfaceDescriptor> XQApplicationManagerPrivate::list(
    const QString &interface, const QString& operation)
{

    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::list(interface)");
    Q_UNUSED(operation);

    if (!serviceMgr)
        serviceMgr = new XQServiceManager();
    
    QList<XQAiwInterfaceDescriptor> result;    
    if (!serviceMgr)
    {
        XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: Can not create service manager");
        return result;
    }
    
    return serviceMgr->findInterfaces(interface);
}

QList<XQAiwInterfaceDescriptor> XQApplicationManagerPrivate::list(
    const QString &service, const QString& interface, const QString& operation)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::list (service+interface)");
    Q_UNUSED(operation);

    if (!serviceMgr)
        serviceMgr = new XQServiceManager();
    
    QList<XQAiwInterfaceDescriptor> result;    
    if (!serviceMgr)
    {
        XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: Can not create service manager");
        return result;
    }

    
    return serviceMgr->findInterfaces(service, interface);
}

QList<XQAiwInterfaceDescriptor> XQApplicationManagerPrivate::list(
    const QUrl &uri)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::list (uri) %s", qPrintable(uri.toString()));

    QList<XQAiwInterfaceDescriptor> result;
    if (!uri.isValid())
    {
        XQSERVICE_CRITICAL_PRINT("Invalid URI %s", qPrintable(uri.toString()));
        return result; // Empty
    }

    QString scheme = uri.scheme();
    
    if (scheme.compare(XQURI_SCHEME_FILE,Qt::CaseInsensitive) == 0)
    {
        QFile file (uri.toLocalFile());
        return list(file);  // Apply file based listing
    }
    
    //  Find all services implementing URI interface and support URI scheme
    QList<XQAiwInterfaceDescriptor> uriHandlers;
    uriHandlers = list(XQI_URI_VIEW, "");

    // Pick up services supporting the required scheme
    foreach (XQAiwInterfaceDescriptor uh, uriHandlers)
    {
        //  Find services that support the scheme
        if (uh.customProperty(XQCUSTOM_PROP_SCHEMES).contains(scheme,Qt::CaseInsensitive))  // Allow multiple schemes in same string
        {
            XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: Service %s can handle scheme %s",
                                  qPrintable(uh.serviceName()), qPrintable(scheme))
            result.append(uh);
        }
    }

    return result;
}

//
// List services capable of handling the file by MIME type
//
QList<XQAiwInterfaceDescriptor> XQApplicationManagerPrivate::list(const QFile &file)
{

    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::list (file) %s", qPrintable(file.fileName()));

    // List first MIME handlers for file
    QList<XQAiwInterfaceDescriptor> mimeHandlers;
    mimeHandlers = listMimeHandlers(file);

    // Then list those file services that can support the MIME type
    return listFileHandlers(mimeHandlers);
}

//
// List services capable of handling the sharable file by MIME type
//
QList<XQAiwInterfaceDescriptor> XQApplicationManagerPrivate::list(const XQSharableFile &file)
{

    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::list (handle)");

    // List first MIME handlers for the sharable file
    QList<XQAiwInterfaceDescriptor> mimeHandlers;
    if (!file.isValid())
    {
        return mimeHandlers; // Empty set
    }
    mimeHandlers = listMimeHandlers(file);

    // Then list those file services that can support the MIME type
    return listFileHandlers(mimeHandlers);
}


int XQApplicationManagerPrivate::lastError() const
{
    int err=0;
    if (serviceMgr)
        err = serviceMgr->latestError();
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: lastError=%d", err);
    return err;
}


// ---- PRIVATE FUNCTIONS ----

bool XQApplicationManagerPrivate::hasCustomHandler(const QUrl &uri) const
{
    return XQAiwUriDriver::hasCustomHandler(uri);
}

//
// Get file handlers
// TODO: Currently only one, default handler is returned !!!
//       Possibly later all handlers need to be returned.
//
QList<XQAiwInterfaceDescriptor> XQApplicationManagerPrivate::listMimeHandlers(const QFile &file)
{

    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::listMimeHandlers %s", qPrintable(file.fileName()));
    QList<XQAiwInterfaceDescriptor> mimeHandlers;

    if (aiwUtilities == 0)
        aiwUtilities = new XQAiwUtils();
    if (aiwUtilities == 0)
    {
        XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: Can not create aiw utils");
        return mimeHandlers;
    }

    XQAiwInterfaceDescriptor descriptor;

    // Find default application that can handle MIME type (only one at the moment)
    int applicationId = 0;
    if (aiwUtilities->findApplication(file, applicationId) != XQService::ENoError)
    {
        return mimeHandlers;  // Empty set
    }

    // Set incomplete descriptor
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: Found file handler %x", applicationId);
    descriptor.setProperty(XQAiwInterfaceDescriptor::ImplementationId, QVariant(applicationId));
    
    mimeHandlers.append(descriptor);

    return mimeHandlers;
}

//
// Get handlers for sharable file
// TODO: Currently only one, default handler is returned !!!
//       Possibly later all handlers need to be returned.
//
QList<XQAiwInterfaceDescriptor> XQApplicationManagerPrivate::listMimeHandlers(const XQSharableFile &file)
{

    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::listMimeHandlers (handle)");
    QList<XQAiwInterfaceDescriptor> mimeHandlers;

    if (aiwUtilities == 0)
        aiwUtilities = new XQAiwUtils();
    if (aiwUtilities == 0)
    {
        XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: Can not create aiw utils");
        return mimeHandlers;
    }

    XQAiwInterfaceDescriptor descriptor;

    // Find default application that can handle MIME type (only one at the moment)
    int applicationId = 0;
    if (aiwUtilities->findApplication(file, applicationId) != XQService::ENoError)
    {
        return mimeHandlers;  // Empty set
    }

    // Set incomplete descriptor
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: Found file handler %x", applicationId);
    descriptor.setProperty(XQAiwInterfaceDescriptor::ImplementationId, QVariant(applicationId));

    mimeHandlers.append(descriptor);

    return mimeHandlers;
}


// To be called only for URI having scheme XQURI_SCHEME_ACTIVITY
// Check that application exists and fill in only the implementationId
bool XQApplicationManagerPrivate::getAppDescriptor(const QUrl &uri,  XQAiwInterfaceDescriptor *descriptor)
{

    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::getAppDescriptor %s)", qPrintable(uri.toString()));

    if (aiwUtilities == 0)
        aiwUtilities = new XQAiwUtils();
    if (aiwUtilities == 0)
    {
        XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: Can not create aiw utils");
        return false;
    }

    int applicationId = 0;
    if (aiwUtilities->findApplication(uri, applicationId) != XQService::ENoError)
    {
        return false;  // No application registered for file
    }

    // Create incomplete descriptor
    descriptor->setProperty(XQAiwInterfaceDescriptor::ImplementationId, QVariant(applicationId));
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: Found uri handler %x", applicationId);

    return  true; 
}


//
// Get file handlers that can support the give MIME type
//
QList<XQAiwInterfaceDescriptor> XQApplicationManagerPrivate::listFileHandlers(
    const QList<XQAiwInterfaceDescriptor> &mimeHandlers)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::listFileHandlers");

    //  Find all services implementing URI interface, support "file" scheme and can handle the file
    //  by MIME type
    QList<XQAiwInterfaceDescriptor> fileServices;
    fileServices = list(XQI_FILE_VIEW, "");
    if (fileServices.count() == 0)
    {
        return fileServices; // Empty set
    }

    QList<XQAiwInterfaceDescriptor> result;
    foreach (XQAiwInterfaceDescriptor fs, fileServices)
    {
        //  Find services that support the file scheme
        XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: Service %s can handle file", qPrintable(fs.serviceName()));
        int serviceAppId = fs.property(XQAiwInterfaceDescriptor::ImplementationId).toInt();
        foreach (XQAiwInterfaceDescriptor mh, mimeHandlers)
        {
            int mimeAppId = mh.property(XQAiwInterfaceDescriptor::ImplementationId).toInt();
            if (mimeAppId == serviceAppId)
            {
                // Return only those services that support file scheme and claim to support also MIME type
                XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: Service %s (UID=%x) can handle file and MIME type",
                                      qPrintable(fs.serviceName()), serviceAppId);
                result.append(fs);
            }
        }
    }

    return result;
}
