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
        : mSynchronous(true), mParent(parent),mServiceManager(NULL)
    {    
        XQSERVICE_DEBUG_PRINT("XQServiceRequest_Private::XQServiceRequest_Private(1)");
        
    };

    XQServiceRequest_Private(const QString& fullServiceName, 
                             const QString& message, 
                             const bool &synchronous, 
                             XQServiceRequest* parent)
        : mService(fullServiceName), mMessage(message), mSynchronous(synchronous), mParent(parent),mServiceManager(NULL)
    {    
        XQSERVICE_DEBUG_PRINT("XQServiceRequest_Private::XQServiceRequest_Private(2)");
    };

    XQServiceRequest_Private(const XQAiwInterfaceDescriptor &descriptor, 
                             const QString& message, 
                             const bool &synchronous, 
                            XQServiceRequest* parent)
            : mMessage(message), mSynchronous(synchronous), mParent(parent),mServiceManager(NULL)
    {
        XQSERVICE_DEBUG_PRINT("XQServiceRequest_Private::XQServiceRequest_Private(3)");
        
        // Construct service name understood by the QtHighway FW
        // (The descriptor has been created from the  XML)
        mService = descriptor.serviceName() + "." + descriptor.interfaceName();
        XQSERVICE_DEBUG_PRINT("XQServiceRequest_Private(3)::service=%s", qPrintable(mService));

        // Remember the descriptor
        mRequestUtil.mDescriptor = descriptor; 
    };
    
    ~XQServiceRequest_Private();
    // from XQServiceRequestCompletedAsync
    void requestCompletedAsync(const QVariant &retValue);
    void requestErrorAsync(int err);
    
    QList<QVariant> mArguments;
    QString mService;
    QString mMessage;
    bool mSynchronous;
    XQServiceRequest* mParent;
    XQRequestUtil mRequestUtil;
    XQServiceManager* mServiceManager;
    QString mUniqueChannelName;
    
};

XQServiceRequest_Private::~XQServiceRequest_Private()
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest_Private::~XQServiceRequest_Private");
    delete mServiceManager;
};

void XQServiceRequest_Private::requestCompletedAsync(const QVariant &retValue)
{    
    XQSERVICE_DEBUG_PRINT("XQServiceRequest_Private::requestCompletedAsync");
    emit mParent->requestCompleted(retValue);
}

void XQServiceRequest_Private::requestErrorAsync(int err)
{    
    XQSERVICE_DEBUG_PRINT("XQServiceRequest_Private::requestErrorAsync");
    emit mParent->requestError(err);
}

/*!
  Construct a null service request.
  setService() and setMessage() must be called before send(), but the
  service may be written prior to the calls.
 */
XQServiceRequest::XQServiceRequest()
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::XQServiceRequest(1)");
    mData = new XQServiceRequest_Private(this);
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
    mData = new XQServiceRequest_Private(service,message,synchronous,this);
}

/*!
  Copy constructor. Any data previously written to the \a orig
  service will be in the copy.
*/
XQServiceRequest::XQServiceRequest(const XQServiceRequest& orig)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::XQServiceRequest(3)");
    mData = new XQServiceRequest_Private(orig.mData->mService,orig.mData->mMessage,orig.mData->mSynchronous,this);
    mData->mArguments = orig.mData->mArguments;
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
    mData = new XQServiceRequest_Private(descriptor,message,synchronous,this);
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

    mData->mService = orig.mData->mService;
    mData->mMessage = orig.mData->mMessage;
    mData->mArguments = orig.mData->mArguments;
    mData->mRequestUtil = orig.mData->mRequestUtil;

    return *this;
}

/*!
  Destructs the service request. Unlike QtopiaIpcEnvelope, the
  request is not automatically sent.
*/
XQServiceRequest::~XQServiceRequest()
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::~XQServiceRequest %s", qPrintable(mData->mUniqueChannelName));
    XQServiceAdaptor::cancelPendingSend(mData->mUniqueChannelName);
    delete mData;
}

/*!
  Returns true if either the service() or message() is not set.

  \sa service(), message()
 */
bool XQServiceRequest::isNull() const
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::isNull");
    bool ret = mData->mService.isEmpty() || mData->mService.isNull() || mData->mMessage.isNull();
	XQSERVICE_DEBUG_PRINT("mData->mService.isEmpty() = %d", mData->mService.isEmpty());
	XQSERVICE_DEBUG_PRINT("mData->mService.isNull() = %d", mData->mService.isNull());
	XQSERVICE_DEBUG_PRINT("mData->mMessage.isNull() = %d", mData->mMessage.isNull());
	XQSERVICE_DEBUG_PRINT("return %d", ret);
	return ret;
}

bool XQServiceRequest::isSynchronous() const
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::isSynchronous");
    return mData->mSynchronous;
}

void XQServiceRequest::setSynchronous(const bool& synchronous)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::setSynchronous");
    mData->mSynchronous = synchronous;
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
    if (isNull())
    {
        XQSERVICE_DEBUG_PRINT("XQServiceRequest::send error: null request");
        XQService::serviceThreadData()->setLatestError(XQService::EArgumentError);
        return false;
    }
    
    // Handle sharable file argument(s), if any
    if (!handleSharableFileArgs())
    {
        XQSERVICE_DEBUG_PRINT("XQServiceRequest::send error:invalid sharable file");
        XQService::serviceThreadData()->setLatestError(XQService::EArgumentError);
        return false;
    }
    
    mData->mRequestUtil.setSynchronous(mData->mSynchronous); // Ensure option is set !
    mData->mRequestUtil.mOperation = mData->mMessage;  // Save the operation name for startup
    
    // !!!
    // Add the info as extra argument to the request
    // This shall be removed by the server
    // !!!
    addArg(qVariantFromValue(mData->mRequestUtil.mInfo));
       
    // Pass always the util instance onwards as user data.
    // It can be utilized by the XQServiceManager::startServer
    // e.g. to optimize startup of a service server

    // Create unique channel name to separate multiple client requests to same channel name.
    quint32 handle = (unsigned int)mData;
    mData->mUniqueChannelName = QString("%1:").arg(handle) + mData->mService;
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::send(2):uniqueChannel=%s", qPrintable(mData->mUniqueChannelName));
    return XQServiceAdaptor::send(mData->mUniqueChannelName,  message(), mData->mArguments, retData, mData->mSynchronous,mData,
                                 (const void *)&mData->mRequestUtil);
}
/*!
  Sets the \a service to which the request will be sent.

  \sa service()
 */
void XQServiceRequest::setService(const QString& fullServiceName)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::setService");
    XQSERVICE_DEBUG_PRINT("service: %s", qPrintable(fullServiceName));
    mData->mService = fullServiceName;
    mData->mArguments.clear();
    mData->mRequestUtil.mDescriptor = XQAiwInterfaceDescriptor(); // Invalid descriptor
}

/*!
  \fn QString XQServiceRequest::service() const

  Returns the service to which this request will be sent.

  \sa setService()
*/
QString XQServiceRequest::service() const
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::service");
    XQSERVICE_DEBUG_PRINT("service: %s", qPrintable(mData->mService));
    return mData->mService;
}

/*!
    Sets the \a message to be sent to the service.

    \sa message()
*/
void XQServiceRequest::setMessage(const QString& message)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::setMessage");
    XQSERVICE_DEBUG_PRINT("message: %s", qPrintable(message));
    mData->mMessage = message;
    mData->mArguments.clear();
}

QString XQServiceRequest::message() const
{ 
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::message");
    XQSERVICE_DEBUG_PRINT("message: %s", qPrintable(mData->mMessage));
    return mData->mMessage;
}

const QList<QVariant> &XQServiceRequest::arguments() const 
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::arguments");
    return mData->mArguments; 
}

void XQServiceRequest::setArguments(const QList<QVariant> &arguments)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::setArguments");
    mData->mArguments = arguments;
}

int XQServiceRequest::latestError()
    {
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::latestError");
    return XQServiceAdaptor::latestError();
    }

void XQServiceRequest::setInfo(const XQRequestInfo &info)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::setInfo");
    mData->mRequestUtil.mInfo = info;
}

XQRequestInfo XQServiceRequest::info() const
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::info");
    return mData->mRequestUtil.mInfo;
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
    mData->mArguments.append(v);
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
    mData->mRequestUtil.mSharableFileArgs.clear();
    
    for(int i=0; i < mData->mArguments.size(); i++)
    {
        if (QString(mData->mArguments[i].typeName()) == QString("XQSharableFile"))
        {
            XQSERVICE_DEBUG_PRINT("XQServiceRequest::sharable file detected");
            // Pick up the sharable file(s) to utility so that no need to scan any more later
            XQSharableFile file = mData->mArguments[i].value<XQSharableFile>();
            if (!file.isValid())
            {
                // No point to pass invalid file handle onwards
                XQSERVICE_DEBUG_PRINT("\t Invalid sharable file");
                ret = false;
                break;
            }
            if (mData->mRequestUtil.mSharableFileArgs.count() > 0)
            {
                XQSERVICE_DEBUG_PRINT("\t Too many sharable files");
                ret = false;
                break;
            }
            mData->mRequestUtil.mSharableFileArgs.append(file);
            XQSERVICE_DEBUG_PRINT("XQServiceRequest::sharable file added");
        }
    }

    if (!ret)
    {
        mData->mRequestUtil.mSharableFileArgs.clear();
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
    stream << action.mData->mArguments;

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
    stream >> action.mData->mArguments;
}

/*!
    \internal
    \fn void XQServiceRequest::serialize(Stream &stream) const
*/
template <typename Stream> void XQServiceRequest::serialize(Stream &stream) const
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::serialize");
    stream << mData->mArguments;
    stream << mData->mService;
    stream << mData->mMessage;
    stream << mData->mSynchronous;
}

/*!
    \internal
    \fn void XQServiceRequest::deserialize(Stream &stream)
*/
template <typename Stream> void XQServiceRequest::deserialize(Stream &stream)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::deserialize");
    stream >> mData->mArguments;
    stream >> mData->mService;
    stream >> mData->mMessage;
    stream >> mData->mSynchronous;
}

Q_IMPLEMENT_USER_METATYPE(XQServiceRequest)
