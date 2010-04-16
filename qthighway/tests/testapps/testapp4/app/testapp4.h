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

#ifndef T_TEST2_H
#define T_TEST2_H

#include <QObject>
#include <QVariant>
#include <QWidget>
#include <QLabel>
#include <xqservicerequest.h>
#include "testdll.h"

class TestApp4Widget : public QWidget
{

Q_OBJECT

public:
   
    TestApp4Widget(QWidget *parent = 0, Qt::WFlags f = 0);

private:
    int i;
    XQServiceRequest* request;
    QLabel *label;
	TestDll* td;
};

#endif // T_TEST2_H
