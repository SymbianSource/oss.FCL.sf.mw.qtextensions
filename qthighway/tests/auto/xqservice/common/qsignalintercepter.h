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

#ifndef QSIGNALINTERCEPTER_H
#define QSIGNALINTERCEPTER_H

#include <qobject.h>
#include <qvariant.h>
#include <qlist.h>

class QSignalIntercepterPrivate;

class QSignalIntercepter : public QObject
{
    // Note: Do not put Q_OBJECT here.
    friend class QSlotInvoker;
    friend class QCopProxy;
public:
    QSignalIntercepter( QObject *sender, const QByteArray& signal,
                        QObject *parent=0 );
    ~QSignalIntercepter();

    QObject *sender() const;
    QByteArray signal() const;

    bool isValid() const;

    static const int QVariantId = -243;

    static int *connectionTypes( const QByteArray& member, int& nargs );

protected:
    int qt_metacall( QMetaObject::Call c, int id, void **a );
    virtual void activated( const QList<QVariant>& args ) = 0;

private:
    QSignalIntercepterPrivate *d;

    static int typeFromName( const QByteArray& name );
};

#endif
