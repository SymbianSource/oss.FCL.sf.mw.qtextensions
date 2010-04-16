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

#include <QDebug>
#include <xqservicerequest.h>
#include <QTimer>
#include <QPixmap>
#include "testservice.h"
#include "mytimer.h"

TestService::TestService(QObject* parent ) 
 : XQServiceProvider("com.nokia.services.testservice.TestService", parent)
{
    publishAll();
    timer1 = new QTimer(this);
    timer2 = new QTimer(this);
    mRequestIndex1 = -1;
    mRequestIndex2 = -1;
    timerKey = 0;    
}

TestService::~TestService()
{
    delete timer1;
    delete timer2;
}

int TestService::syncNoParams()
{
    qDebug() << "[QTH] [TestService] syncNoParams";

    for( int i = 0; i<20000; i++ )
        {
        int ii = i;
        }
    
    qDebug() << "[QTH] [TestService] syncNoParams end";
    return 1;
}

int TestService::syncWithParams(QString param1, int param2)
{
    qDebug() << "[QTH] [TestService] syncWithParam ";
    qDebug() << "[QTH] [TestService] param1: " << param1;
    qDebug() << "[QTH] [TestService] param2: " << param2;
    
    for( int i = 0; i<20000; i++ )
        {
        int ii = i;
        }
    
    qDebug() << "[QTH] [TestService] syncWithParam end";
    return 1;
}

int TestService::asyncNoParams()
{
    qDebug() << "[QTH] [TestService] asyncNoParams START";
    QString returnString("my_request");

    mRequestIndex1 = setCurrentRequestAsync();
    
    bool res = connect(timer1, SIGNAL(timeout()), this, SLOT(DoneAsyncNoParams()));
    qDebug() << "[QTH] [TestService] \t connect timer1->timeout() to this->DoneAsyncNoParams() = " << res ;
    timer1->start(10000);
    qDebug() << "[QTH] [TestService] asyncNoParams END";
    return 1;
}

void TestService::DoneAsyncNoParams()
    {
    qDebug() << "[QTH] [TestService] DoneasyncNoParams START";
    
    if (mRequestIndex1 == -1) {
        qDebug() << "[QTH] [TestService] \t mRequestIndex1 == -1 !!!!";
        return;
    }
    
    qDebug() << "[QTH] [TestService] \t disconnect";
    disconnect(timer1, SIGNAL(timeout()), this, SLOT(DoneAsyncNoParams()));
    qDebug() << "[QTH] [TestService] \t completeRequest(1, QVariant(1)) start";
    completeRequest(mRequestIndex1,  QVariant(1));
    qDebug() << "[QTH] [TestService] \t completeRequest(" << mRequestIndex1 << ", QVariant(1)) end";
    qDebug() << "[QTH] [TestService] DoneasyncNoParams END";
    }


int TestService::asyncWithParams(QString param1, int param2)
{
    timerKey++;
    qDebug() << "[QTH] [TestService] asyncWithParams START";
    qDebug() << "[QTH] [TestService] \t param1: " << param1;
    qDebug() << "[QTH] [TestService] \t param2: " << param2;

    qDebug() << "[QTH] [TestService] \t setCurrentRequestAsync";
    mRequestIndex2 = setCurrentRequestAsync();
    qDebug() << "[QTH] [TestService] \t currentRequestIndex = " << mRequestIndex2;
    

    MyTimer* mt = new MyTimer();

    mt->setTimerKey(timerKey);
    mt->setRequestId(mRequestIndex2);
    qDebug() << "[QTH] [TestService] \t insert a new timer having key = " << timerKey;
    timers.insert(timerKey, mt);
    qDebug() << "[QTH] [TestService] \t size of timers map = " << timers.size();
    
    qDebug() << "[QTH] [TestService] \t connect(timer2, SIGNAL(timeout()), this, SLOT(DoneAsyncWithParams()))";
    bool res = connect(mt, SIGNAL(timeout(int)), this, SLOT(DoneAsyncWithParams(int)));
    qDebug() << "[QTH] [TestService] \t connect result = " << res;
    
    mt->start(10000);
    
    QString returnString;
    returnString.append(param1);
    returnString.append(" ");
    returnString.append(param2);
    qDebug() << "[QTH] [TestService] \t returnString = " << returnString;
    qDebug() << "[QTH] [TestService] asyncWithParams END";
    return 1;
}


void TestService::DoneAsyncWithParams(int timerId)
{
    qDebug() << "[QTH] [TestService] DoneAsyncWithParams START";
    qDebug() << "[QTH] [TestService] \t timerId = " << timerId;
    
    if (!timers.contains(timerId)) {
        qDebug() << "[QTH] [TestService] \t timers map does not contain timer having key = " << timerId << " !!!! ";
        qDebug() << "[QTH] [TestService] \t return !!!";
        qDebug() << "[QTH] [TestService] DoneAsyncWithParams END (1)";
        return;
    } else {
        MyTimer* mt = timers[timerId];
        disconnect(mt, SIGNAL(timeout(int)), this, SLOT(DoneAsyncWithParams(int)));
        int requestId = mt->getRequestId();
        qDebug() << "[QTH] [TestService] \t requestId = " << requestId;
        timers.remove(timerId);
        delete mt;
        mt = NULL;
        
        if (requestId == -1) {
            qDebug() << "[QTH] [TestService] \t requestId == -1 !!!!";
            return;
        }        
        qDebug() << "[QTH] [TestService] \t completeRequest -> requestId = " << requestId << ", retValue = QVariant(\"helloworld!\")";
        completeRequest(requestId,  QVariant("helloworld!"));        
        qDebug() << "[QTH] [TestService] DoneAsyncWithParams END (2)";
    }
}

// not used
void TestService::longRunningRequest()
{
    qDebug() << "[QTH] [TestService] longRunningRequest START";
    qDebug() << "[QTH] [TestService] \t QTimer::singleShot(30000, this, SLOT(completeLongRunningRequest()))";
    QTimer::singleShot(30000, this, SLOT(completeLongRunningRequest()));
    qDebug() << "[QTH] [TestService] longRunningRequest END";
}

void TestService::completeLongRunningRequest()
{
    qDebug() << "[QTH] [TestService] completeLongRunningRequest END";
    completeRequest(mRequestIndex3 = setCurrentRequestAsync(), QVariant(true));
    qDebug() << "[QTH] [TestService] completeLongRunningRequest END";
}
