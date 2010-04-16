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

#ifndef QSLOTINVOKER_H
#define QSLOTINVOKER_H

#include <qobject.h>
#include <qvariant.h>
#include <qlist.h>

class QSlotInvokerPrivate;

class QSlotInvoker : public QObject
{
    Q_OBJECT
public:
    QSlotInvoker( QObject *receiver, const QByteArray& member,
                  QObject *parent=0 );
    ~QSlotInvoker();

    bool memberPresent() const;
    bool canInvoke( int numArgs ) const;
    QObject *receiver() const;
    QByteArray member() const;
    int *parameterTypes() const;
    int parameterTypesCount() const;

public slots:
    QVariant invoke( const QList<QVariant>& args );

private slots:
    void receiverDestroyed();

private:
    QSlotInvokerPrivate *d;
};

#endif
