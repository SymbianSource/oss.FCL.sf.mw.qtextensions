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
#include <xqservicelog.h>
#include <QApplication>
#include <QKeyEvent>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QImageReader>
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>
#include <QString>
#include <QCheckBox>

#include <QListView>

#include "TestDll.h"

TestDll::TestDll(QObject *parent) : QObject(parent)
    {
	qDebug() << "[QTH] [TestDll] MyServiceTester: START";
	i = 0;
    request = NULL;
    //asyncsend();
    qDebug() << "[QTH] [TestDll] MyServiceTester: END";
    }


void TestDll::requestCompleted(const QVariant& data)
{
    i++;
    qDebug() << "[QTH] [TestDll] requestCompleted START ";
	qDebug() << "[QTH] [TestDll] \t data: " << data.toString() << ", i: " << i;
    
    if ( i <= 20 )
        {
        qDebug() << "[QTH] [TestDll] \t i <= 20 trigger again()";
        QString string;
        qDebug() << "Test app3 --> " << 48 << i << " times";
        asyncsend();
        }
    else
        {
		/*qDebug() << "[QTH] [MyServiceTester] \t i > 20, setText()";
        label->setText("Test app1 Done!");
        if(request)
            {
			qDebug() << "[QTH] [MyServiceTester] \t \t request != null, delete request";
            delete request;
            request=NULL;
            }*/
		qDebug() << "[QTH] [TestDll] \t request sent 20 times!!!";
        }

	qDebug() << "[QTH] [TestDll] requestCompleted STOP";
}



void TestDll::asyncsend()
{
	qDebug() << "[QTH] [TestDll] asyncsend: START";

	QString testString("Test Application 3");
	int testInt(112233);

	QString service("com.nokia.services.testservice.TestService");
	QString method("asyncWithParams(QString,int)");

	if (!request)
		{
		qDebug() << "[QTH] [TestDll] \t creating request";
		request = new XQServiceRequest(service, method, false);
		qDebug() << "[QTH] [TestDll] \t connectint signal request->requestCompleted to slot this.requestCompleted";
		bool res = connect(request, SIGNAL(requestCompleted(QVariant)), this, SLOT(requestCompleted(QVariant)) );
		qDebug() << "[QTH] [TestDll] \t connect = " << res;
		*request << testString;
		*request << testInt;
		qDebug() << "[QTH] [TestDll] \t request: " << testString << ", " << testInt;
		}

	qDebug() << "[QTH] [TestDll] \t before request->send()";
	bool res = request->send();
	qDebug() << "[QTH] [TestDll] \t after request->send()";
	qDebug() << "[QTH] [TestDll] asyncsend: STOP";
}
