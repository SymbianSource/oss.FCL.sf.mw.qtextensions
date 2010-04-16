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

#include "xqservicerequest.h"
#include "xqservicethreaddata.h"

#include "xqservicechannel.h"
#include "xqserviceadaptor.h"
#include "xqrequestutil.h"
#include <QStringList>
#include <xqservicemanager.h>

class XQServiceRequest_Private : public XQServiceRequestCompletedAsync
{
public:
    XQServiceRequest_Private(XQServiceRequest* parent)
        : m_Synchronous(true), m_Parent(parent),serviceManager(NULL)
    {    
        XQSERVICE_DEBUG_PRINT("XQServiceRequest_Private::XQServiceRequest_Private(1)");
        
    };

    XQServiceRequest_Private(const QString& service, 
                             const QString& message, 
                             const bool &synchronous, 
                             XQServiceRequest* parent)
        : m_Service(service), m_Message(message), m_Synchronous(synchronous), m_Parent(parent),serviceManager(NULL)
    {    
        XQSERVICE_DEBUG_PRINT("XQServiceRequest_Private::XQServiceRequest_Private(2)");
    };

    XQServiceRequest_Private(const XQAiwInterfaceDescriptor &descriptor, 
                             const QString& message, 
                             const bool &synchronous, 
                            XQServiceRequest* parent)
            : m_Message(message), m_Synchronous(synchronous), m_Parent(parent),serviceManager(NULL)
    {
        XQSERVICE_DEBUG_PRINT("XQServiceRequest_Private::XQServiceRequest_Private(3)");
        // Construct service name understood by the QtHighway FW
        // (The descriptor has been created from the  XML)
        m_Service = descriptor.serviceName() + "." + descriptor.interfaceName();
        XQSERVICE_DEBUG_PRINT("XQServiceRequest_Private(3)::service=%s", qPrintable(m_Service));

        // Remember the descriptor
        m_RequestUtil.mDescriptor = descriptor; 
    };
    
    ~XQServiceRequest_Private();
    // from XQServiceRequestCompletedAsync
    void requestCompletedAsync(const QVariant &retValue);
    void requestErrorAsync(int err);
    
    QList<QVariant> m_arguments;
    QString m_Service;
    QString m_Message;
    bool m_Synchronous;
    XQServiceRequest* m_Parent;
    XQRequestUtil m_RequestUtil;
    XQServiceManager* serviceManager;

    
};

XQServiceRequest_Private::~XQServiceRequest_Private()
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest_Private::~XQServiceRequest_Private");
    delete serviceManager;
};

void XQServiceRequest_Private::requestCompletedAsync(const QVariant &retValue)
{    
    XQSERVICE_DEBUG_PRINT("XQServiceRequest_Private::requestCompletedAsync");
    emit m_Parent->requestCompleted(retValue);
}

void XQServiceRequest_Private::requestErrorAsync(int err)
{    
    XQSERVICE_DEBUG_PRINT("XQServiceRequest_Private::requestErrorAsync");
    emit m_Parent->requestError(err);
}

/*!
  Construct a null service request.
  setService() and setMessage() must be called before send(), but the
  service may be written prior to the calls.
 */
XQServiceRequest::XQServiceRequest()
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::XQServiceRequest(1)");
    m_data = new XQServiceRequest_Private(this);
}

/*!
  Construct a service request that will send \a message to
  a \a service when send() is called. The service may be written
  prior to the calls.
*/
XQServiceRequest::XQServiceRequest(const QString& service, const QString& message, const bool &synchronous)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::XQServiceRequest(2)");
    XQSERVICE_DEBUG_PRINT("service: %s, message: %s, synchronous: %d", qPrintable(service), qPrintable(message), synchronous);
    m_data = new XQServiceRequest_Private(service,message,synchronous,this);
}

/*!
  Copy constructor. Any data previously written to the \a orig
  service will be in the copy.
*/
XQServiceRequest::XQServiceRequest(const XQServiceRequest& orig)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::XQServiceRequest(3)");
    m_data = new XQServiceRequest_Private(orig.m_data->m_Service,orig.m_data->m_Message,orig.m_data->m_Synchronous,this);
    m_data->m_arguments = orig.m_data->m_arguments;
}

/*!
  Construct a service request by service descriptor which contains exact details of the service and interface.
  a \a service when send() is called. The service may be written
  prior to the calls.
*/
XQServiceRequest::XQServiceRequest(const XQAiwInterfaceDescriptor &descriptor, const QString& message, const bool &synchronous)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::XQServiceRequest(2)");
    XQSERVICE_DEBUG_PRINT("service: %s, interface %s, message: %s, synchronous: %d",
                          qPrintable(descriptor.serviceName()), qPrintable(descriptor.interfaceName()),
                          qPrintable(message), synchronous);
    m_data = new XQServiceRequest_Private(descriptor,message,synchronous,this);
}

/*!
  Assignment operator.
  Any data previously written to the \a orig
  service will be in the copy.
*/
XQServiceRequest& XQServiceRequest::operator=(const XQServiceRequest& orig)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::operator=");
    if( &orig == this )
        return *this;

    m_data->m_Service = orig.m_data->m_Service;
    m_data->m_Message = orig.m_data->m_Message;
    m_data->m_arguments = orig.m_data->m_arguments;
    m_data->m_RequestUtil = orig.m_data->m_RequestUtil;

    return *this;
}

/*!
  Destructs the service request. Unlike QtopiaIpcEnvelope, the
  request is not automatically sent.
*/
XQServiceRequest::~XQServiceRequest()
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::~XQServiceRequest=");
    XQServiceAdaptor::cancelPendingSend(m_data->m_Service);
    delete m_data;
}

/*!
  Returns true if either the service() or message() is not set.

  \sa service(), message()
 */
bool XQServiceRequest::isNull() const
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::isNull");
    bool ret = m_data->m_Service.isEmpty() || m_data->m_Service.isNull() || m_data->m_Message.isNull();
	XQSERVICE_DEBUG_PRINT("m_data->m_Service.isEmpty() = %d", m_data->m_Service.isEmpty());
	XQSERVICE_DEBUG_PRINT("m_data->m_Service.isNull() = %d", m_data->m_Service.isNull());
	XQSERVICE_DEBUG_PRINT("m_data->m_Message.isNull() = %d", m_data->m_Message.isNull());
	XQSERVICE_DEBUG_PRINT("return %d", ret);
	return ret;
}

bool XQServiceRequest::isSynchronous() const
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::isSynchronous");
    return m_data->m_Synchronous;
}

void XQServiceRequest::setSynchronous(const bool& synchronous)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::setSynchronous");
    m_data->m_Synchronous = synchronous;
}
/*!
  \fn QString XQServiceRequest::send()
  Sends the request. Returns false if there was no application that could
  service the request.
*/
bool XQServiceRequest::send()
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::send(1)");
    QVariant retValue;
    return send(retValue);
}

/*!
  \fn QString XQServiceRequest::send()
  Sends the request. Returns false if there was no application that could
  service the request.
*/
bool XQServiceRequest::send(QVariant& retData)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::send(2)");
    if (isNull()) {
        XQSERVICE_DEBUG_PRINT("Request is null");
        return false;
    }
    
    if (m_data->m_Service.isNull())
        return false;

    // Handle sharable file argument(s), if any
    if (!handleSharableFileArgs())
    {
        XQService::serviceThreadData()->setLatestError(XQService::EArgumentError);
        return false;
    }
    
    m_data->m_RequestUtil.setSynchronous(m_data->m_Synchronous); // Ensure option is set !
    
    // !!!
    // Add the info as extra argument to the request
    // This shall be removed by the server
    // !!!
    addArg(qVariantFromValue(m_data->m_RequestUtil.mInfo));

    XQSERVICE_DEBUG_PRINT("XQServiceRequest::send(2):mDescriptor");
        
    // Pass always the util instance onwards as user data.
    // It can be utilized by the XQServiceManager::startServer
    // e.g. to optimize startup of a service server
    return XQServiceAdaptor::send(m_data->m_Service,  message(), m_data->m_arguments, retData, m_data->m_Synchronous,m_data,
                                 (const void *)&m_data->m_RequestUtil);
}
/*!
  Sets the \a service to which the request will be sent.

  \sa service()
 */
void XQServiceRequest::setService(const QString& service)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::setService");
    XQSERVICE_DEBUG_PRINT("service: %s", qPrintable(service));
    m_data->m_Service = service;
    m_data->m_arguments.clear();
}

/*!
  \fn QString XQServiceRequest::service() const

  Returns the service to which this request will be sent.

  \sa setService()
*/
QString XQServiceRequest::service() const
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::service");
    XQSERVICE_DEBUG_PRINT("service: %s", qPrintable(m_data->m_Service));
    return m_data->m_Service;
}

/*!
    Sets the \a message to be sent to the service.

    \sa message()
*/
void XQServiceRequest::setMessage(const QString& message)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::setMessage");
    XQSERVICE_DEBUG_PRINT("message: %s", qPrintable(message));
    m_data->m_Message = message;
    m_data->m_arguments.clear();
}

QString XQServiceRequest::message() const
{ 
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::message");
    XQSERVICE_DEBUG_PRINT("message: %s", qPrintable(m_data->m_Message));
    return m_data->m_Message;
}

const QList<QVariant> &XQServiceRequest::arguments() const 
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::arguments");
    return m_data->m_arguments; 
}

void XQServiceRequest::setArguments(const QList<QVariant> &arguments)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::setArguments");
    m_data->m_arguments = arguments;
}

int XQServiceRequest::latestError()
    {
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::latestError");
    return XQServiceAdaptor::latestError();
    }

void XQServiceRequest::setInfo(const XQRequestInfo &info)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::setInfo");
    m_data->m_RequestUtil.mInfo = info;
}

XQRequestInfo XQServiceRequest::info() const
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::info");
    return m_data->m_RequestUtil.mInfo;
}
    
/*!
  \fn QString XQServiceRequest::message() const

  Returns the message of the request.

  \sa setMessage()
*/

/*!
    \fn const QList<QVariant> &XQServiceRequest::arguments() const

    Returns the complete list of arguments for this service request.
*/

/*!
    \fn void XQServiceRequest::setArguments(const QList<QVariant> &arguments)

    Sets the complete list of \a arguments for this service request.
*/

/*!
    \fn XQServiceRequest &XQServiceRequest::operator<< (const T &var)

    Adds \a var to the list of arguments for this service request.
*/

/*!
    \fn XQServiceRequest &XQServiceRequest::operator<< (const char *var)

    Adds \a var to the list of arguments for this service request.
*/

/*!
    \fn void XQServiceRequest::addArg(const QVariant& var)

    Adds the variant \a var to the list of arguments, so that the variant's
    value is serialized in send() rather than the variant itself.
*/

/*!
    \internal
*/
void XQServiceRequest::addArg(const QVariant& v)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::addArg %s,%d", v.typeName());
    XQSERVICE_DEBUG_PRINT("v: %s", qPrintable(v.toString()));
    m_data->m_arguments.append(v);
}


/*!
    \fn void XQServiceRequest::handleSharableFileArg()

    Picks the XQSharableFile argument, if any, into the request util
    By this way scan parameter list only once.
*/

/*!
    \internal
*/
bool XQServiceRequest::handleSharableFileArgs()
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::handleSharableFile");

    bool ret = true;
    m_data->m_RequestUtil.mSharableFileArgs.clear();
    
    for(int i=0; i < m_data->m_arguments.size(); i++)
    {
        if (QString(m_data->m_arguments[i].typeName()) == QString("XQSharableFile"))
        {
            XQSERVICE_DEBUG_PRINT("XQServiceRequest::sharable file detected");
            // Pick up the sharable file(s) to utility so that no need to scan any more later
            XQSharableFile file = m_data->m_arguments[i].value<XQSharableFile>();
            if (!file.isValid())
            {
                // No point to pass invalid file handle onwards
                XQSERVICE_DEBUG_PRINT("\t Invalid sharable file");
                ret = false;
                break;
            }
            if (m_data->m_RequestUtil.mSharableFileArgs.count() > 0)
            {
                XQSERVICE_DEBUG_PRINT("\t Too many sharable files");
                ret = false;
                break;
            }
            m_data->m_RequestUtil.mSharableFileArgs.append(file);
            XQSERVICE_DEBUG_PRINT("XQServiceRequest::sharable file added");
        }
    }

    if (!ret)
    {
        m_data->m_RequestUtil.mSharableFileArgs.clear();
    }

    return ret;
}


/*!
    \internal
*/
QByteArray XQServiceRequest::serializeArguments(const XQServiceRequest &action)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::serializeArguments");
    QByteArray ret;
    QBuffer *buffer = new QBuffer(&ret);
    buffer->open(QIODevice::WriteOnly);
    QDataStream stream(buffer);
    stream << action.m_data->m_arguments;

    delete buffer;
    return ret;
}
/*!
    \internal
*/
void XQServiceRequest::deserializeArguments(XQServiceRequest &action,
        const QByteArray &data)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::deserializeArguments");
    QDataStream stream(data);
    stream >> action.m_data->m_arguments;
}

/*!
    \internal
    \fn void XQServiceRequest::serialize(Stream &stream) const
*/
template <typename Stream> void XQServiceRequest::serialize(Stream &stream) const
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::serialize");
    stream << m_data->m_arguments;
    stream << m_data->m_Service;
    stream << m_data->m_Message;
    stream << m_data->m_Synchronous;
}

/*!
    \internal
    \fn void XQServiceRequest::deserialize(Stream &stream)
*/
template <typename Stream> void XQServiceRequest::deserialize(Stream &stream)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::deserialize");
    stream >> m_data->m_arguments;
    stream >> m_data->m_Service;
    stream >> m_data->m_Message;
    stream >> m_data->m_Synchronous;
}

Q_IMPLEMENT_USER_METATYPE(XQServiceRequest)
