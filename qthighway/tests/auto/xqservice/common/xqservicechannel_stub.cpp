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

/* THIS IS A XQServiceChannel STUB used only for UNIT TEST */

#include <QThreadStorage>

#include "xqservicechannel.h"
#include "xqservicethreaddata.h"
#include "xqsharablefile.h"

static QThreadStorage<XQServiceChannelPrivate *> xqchannel;

XQServiceChannel::XQServiceChannel(const QString& channel, bool isServer, QObject *parent)
    : QObject(parent)
{
    d = new XQServiceChannelPrivate(this, channel, isServer);
    xqchannel.setLocalData(d);
}


XQServiceChannel::~XQServiceChannel()
{
}

bool XQServiceChannel::connectChannel()
{
    return true;
}

QString XQServiceChannel::channel() const
{
    return d->channel;
}

QVariant XQServiceChannel::receive(const QString& msg, const QByteArray &data, const XQSharableFile &sf)
{
    emit received(msg, data,sf);
    return QVariant();
}

void XQServiceChannel::commandReceive(const XQServiceCommand cmd)
{
    emit commandReceived(cmd);
}

bool XQServiceChannel::send(const QString& channel, const QString& msg,
                       const QByteArray &data, QVariant &retValue, bool sync, XQServiceRequestCompletedAsync* rc)
{
    return send(channel,msg,data,retValue,sync,rc,NULL);
}

bool XQServiceChannel::send(const QString& channel, const QString& msg,
                            const QByteArray &data, QVariant &retValue,
                            bool sync, XQServiceRequestCompletedAsync* rc,
                            const void* userData)
{
    XQServiceChannelPrivate* channelPrivate = xqchannel.localData();
    XQSharableFile dummy;
    retValue = channelPrivate->object->receive(msg,data,dummy);
    return true;   
}

bool XQServiceChannel::cancelPendingSend(const QString& channel)
{
	return true ;
}

QVariant XQServiceChannel::sendLocally(const QString& ch, const QString& msg,
                                const QByteArray &data, const XQSharableFile &sf)
{
	/*
    // filter out internal events
    if (ch.isEmpty())
        return;

    // feed local clients with received data
    XQServiceThreadData *td = XQService::serviceThreadData();
    QList<XQServiceChannelPrivatePointer> clients = td->clientMap[ch];
    for (int i = 0; i < clients.size(); ++i) {
    XQServiceChannelPrivate *channel = clients.at(i).data();
    if (channel->object)
        channel->object->receive(msg, data);
    }
    */
    return QVariant();
}

int XQServiceChannel::latestError()
    {
    return -4998;
    }

