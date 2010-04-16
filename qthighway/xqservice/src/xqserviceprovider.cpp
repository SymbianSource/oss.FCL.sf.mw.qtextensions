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

#include <xqserviceprovider.h>
#include <qmetaobject.h>
#include <QByteArray>

#include <xqserviceadaptor.h>
//#include <xqserviceservice.h>
#include <xqserviceutil.h>

/*!
    \class XQServiceProvider
    \inpublicgroup QtBaseModule

    \brief The XQServiceProvider class provides an interface to messages on a XQService service
    which simplifies remote slot invocations

    Service messages consist of a service name, a message name, and a list of parameter values.
    Qt extension extension dispatches service messages to the applications associated with the service
    name, on the application's \c{QPE/Application/appname} channel, where
    \c{appname} is the application's name.

    The use of XQServiceProvider will be demonstrated using the \c{Time}
    service.  This has a single message called \c{editTime()} which asks
    the service to pop up a dialog allowing the user to edit the current time.

    \code
    class TimeService : public XQServiceProvider
    {
        Q_OBJECT
    public:
        TimeService( QObject *parent = 0 );

    public slots:
        void editTime(QTime time);
    };

    TimeService::TimeService( QObject *parent )
        : XQServiceProvider( "Time", parent )
    {
        publishAll();
    }
    \endcode

    The call to publishAll() causes all public slots within \c{TimeService}
    to be automatically registered as Service messages.  This can be
    useful if the service has many message types.

    The client can send a request to the service using QtopiaServiceRequest:

    \code
    XQServiceRequest req( "Time", "editTime()" );
    req << QTime::currentTime();
    req.send();
    \endcode

*/

class ServiceAdaptorProxy : public XQServiceAdaptor
{
    Q_OBJECT

public:
    ServiceAdaptorProxy(const QString &channel, QObject *parent=0);
    virtual ~ServiceAdaptorProxy() ;
    
    QString memberToMessage( const QByteArray& member );
};

ServiceAdaptorProxy::ServiceAdaptorProxy(const QString &channel, QObject *parent) :
        XQServiceAdaptor(channel, parent) 
{
    XQSERVICE_DEBUG_PRINT("ServiceAdaptorProxy::ServiceAdaptorProxy");
    XQSERVICE_DEBUG_PRINT("channel: %s", qPrintable(channel));
}

ServiceAdaptorProxy::~ServiceAdaptorProxy()
{
    XQSERVICE_DEBUG_PRINT("ServiceAdaptorProxy::~ServiceAdaptorProxy");
}

QString ServiceAdaptorProxy::memberToMessage( const QByteArray& member )
{
    XQSERVICE_DEBUG_PRINT("ServiceAdaptorProxy::memberToMessage");
    XQSERVICE_DEBUG_PRINT("member: %s", member.constData());
// TO BE CHECKED
//    return m_channel + "::" + XQServiceAdaptor::memberToMessage( member );
    return XQServiceAdaptor::memberToMessage( member );
}

class XQServiceProvider_Private
{
public:
    XQServiceProvider_Private(const QString &service);

    ~XQServiceProvider_Private();
    
    XQServiceAdaptor *m_adaptor;

    QString m_service;
    bool m_publishAllCalled;
    QObject* plugin;
};

XQServiceProvider_Private::XQServiceProvider_Private(const QString &service) :
        m_adaptor(NULL),
        m_service(service),
        m_publishAllCalled(false),
		plugin(NULL)
{
    XQSERVICE_DEBUG_PRINT("XQServiceProvider_Private::XQServiceProvider_Private");
    XQSERVICE_DEBUG_PRINT("service: %s", qPrintable(service));
    m_adaptor = new ServiceAdaptorProxy(service);
}

XQServiceProvider_Private::~XQServiceProvider_Private()
{
    XQSERVICE_DEBUG_PRINT("XQServiceProvider_Private::~XQServiceProvider_Private");
    delete m_adaptor;
}


/*!
    Construct a remote service object for \a service and attach it to \a parent.
*/
XQServiceProvider::XQServiceProvider( const QString& service, QObject *parent )
    : QObject( parent )
{
    XQSERVICE_DEBUG_PRINT("XQServiceProvider::XQServiceProvider");
    XQSERVICE_DEBUG_PRINT("service: %s", qPrintable(service));
    m_data = new XQServiceProvider_Private(service);
    connect(m_data->m_adaptor, SIGNAL(returnValueDelivered()), this, SIGNAL(returnValueDelivered())); 
    connect(m_data->m_adaptor, SIGNAL(clientDisconnected()), this, SIGNAL(clientDisconnected())); 
}

/*!
    Destroy this service handling object.
*/
XQServiceProvider::~XQServiceProvider()
{
    XQSERVICE_DEBUG_PRINT("XQServiceProvider::~XQServiceProvider");
    if (m_data)
        delete m_data;
}


void XQServiceProvider::SetPlugin(QObject* impl_plugin)
    {
    m_data->plugin=impl_plugin;
    }


/*!
    Publishes all slots on this object within subclasses of XQServiceProvider.
    This is typically called from a subclass constructor.
*/
void XQServiceProvider::publishAll()
{
    XQSERVICE_DEBUG_PRINT("XQServiceProvider::publishAll");
	if (!m_data->plugin) {
	    m_data->m_adaptor->publishAll(this,XQServiceProvider::staticMetaObject.methodCount(),XQServiceAdaptor::Slots);
	}
    else {
        m_data->m_adaptor->publishAll(m_data->plugin, 0, XQServiceAdaptor::Slots);
    } 
}

/*!
*   Sets current request to asynchronous mode so that provider can complete the
*   request later via the completeRequest() call. 
*   \return Request ID which shall be used in the completeRequest() call.
*/
int XQServiceProvider::setCurrentRequestAsync()
{
    XQSERVICE_DEBUG_PRINT("XQServiceProvider::setCurrentRequestAsync");
    return m_data->m_adaptor->setCurrentRequestAsync();
}

/*!
*   Completes the asynchronous request with the given value
*   \param index Request ID got from the setCurrentRequestAsync call.
*   \param retValue The value
*   \return true on success, false if index points to non-existing request.
*/
bool XQServiceProvider::completeRequest(int index, const QVariant& retValue)
{
    XQSERVICE_DEBUG_PRINT("XQServiceProvider::completeRequest");
    XQSERVICE_DEBUG_PRINT("index: %d, retValue: %s", index, qPrintable(retValue.toString()));
    return m_data->m_adaptor->completeRequest(index, retValue);
}

/*!
*   Return additional request information attached to  request
*   \return Request info
*/
XQRequestInfo XQServiceProvider::requestInfo() const
{
    return m_data->m_adaptor->requestInfo();
}

#include "xqserviceprovider.moc"
