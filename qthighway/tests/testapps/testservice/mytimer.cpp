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
#include "MyTimer.h"

MyTimer::MyTimer()
    {
    qDebug() << "[QTH] [MyTimer] MyTimer() START";
    timer = new QTimer();
    bool res = connect(timer, SIGNAL(timeout()), this, SLOT(emitMyTimeout()));
    qDebug() << "[QTH] [MyTimer] \t connect = " << res; 
    qDebug() << "[QTH] [MyTimer] MyTimer() END";
    }

void MyTimer::emitMyTimeout()
    {
    qDebug() << "[QTH] [MyTimer] emitMyTimeout, timerKey = " << timerKey;
    emit timeout(timerKey);
    }

void MyTimer::start(int msec)
    {
    qDebug() << "[QTH] [MyTimer] start(" << msec << ")";
    timer->start(msec);
    }

void MyTimer::setTimerKey(int ti)
    {
    timerKey = ti;
    }

void MyTimer::setRequestId(int ri)
    {
    requestId = ri;
    }

int MyTimer::getRequestId()
    {
    return requestId;
    }

MyTimer::~MyTimer()
    {
    qDebug() << "[QTH] [MyTimer] ~MyTimer() START";
    delete timer;
    qDebug() << "[QTH] [MyTimer] ~MyTimer() END";
    }
