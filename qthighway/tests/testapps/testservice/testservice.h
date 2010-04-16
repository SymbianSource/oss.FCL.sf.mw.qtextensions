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

#ifndef ACTIVITYSERVERSERVICE_H
#define ACTIVITYSERVERSERVICE_H

#include <QObject>
#include <QVariant>
#include <QWidget>
#include <QLabel>
#include <xqserviceprovider.h>
#include "mytimer.h"

class TestService : public XQServiceProvider
{
    Q_OBJECT

public:
    TestService(QObject *parent = 0 );
    ~TestService();

    void longRunningRequest();

public slots:

    // request handling methods
    int syncNoParams();
    int syncWithParams(QString param1, int param2);
    int asyncNoParams();
    int asyncWithParams(QString param1, int param2);    

private slots:

    void completeLongRunningRequest();  
    void DoneAsyncNoParams();
    void DoneAsyncWithParams(int timerId);
    
private:
    QTimer* timer1;
    QTimer* timer2;
    QHash<int, MyTimer*> timers;

    int mRequestIndex1;
	int mRequestIndex2;
	int mRequestIndex3;
	int timerKey;
};

#endif // ACTIVITYSERVERSERVICE_H
