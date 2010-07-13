/*
#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, version 2.1 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not,
# see "http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html/".
#
# Description:
#
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

#include "testapp.h"

TestApp1::TestApp1( QWidget *parent, Qt::WFlags f ) : QWidget( parent,f )
    {
    qDebug() << "[QTH] [TestApp1] TestApp1: START";
    QPalette p = qApp->palette();
    QColor color(80,20,20);
    QColor bg(256,20,20);
    p.setColor(QPalette::Highlight, color.lighter(200));
    p.setColor(QPalette::Text, Qt::white);
    p.setColor(QPalette::Base, bg);
    p.setColor(QPalette::WindowText, Qt::white);
    p.setColor(QPalette::Window, bg);
    p.setColor(QPalette::ButtonText, Qt::white);
    p.setColor(QPalette::Button, color.lighter(150));
    p.setColor(QPalette::Link, QColor(240,40,40));

    qApp->setPalette(p);

    QPushButton *quitButton = new QPushButton(tr("quit"));
    connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

    QPushButton *sendButton = new QPushButton(tr("asynsend 10 times"));
    connect(sendButton, SIGNAL(clicked()), this, SLOT(asyncsend()));

    label = new QLabel("Test App1");

    QVBoxLayout *vl = new QVBoxLayout;
    vl->setMargin(0);
    vl->setSpacing(0);

    vl->addWidget(label);
    vl->addWidget(sendButton);
    vl->addWidget(quitButton);

    setLayout(vl);
    //showMaximized();
    showFullScreen();
    
    i = 0;
    request = NULL;
    asyncsend();
    qDebug() << "[QTH] [TestApp1] TestApp1: END";
    }

void TestApp1::requestCompletedWithError(int err)
    {
    qDebug() << "[QTH] [TestApp1] requestCompletedWithError START ";
    qDebug() << "[QTH] [TestApp1] error = " << err << " !!!";
    qDebug() << "[QTH] [TestApp1] requestCompletedWithError END ";
    }

void TestApp1::requestCompleted(const QVariant& data)
{
    i++;
    qDebug() << "[QTH] [TestApp1] requestCompleted START ";
	qDebug() << "[QTH] [TestApp1] \t data: " << data.toString() << ", i: " << i;
    
    if ( i <= 20 )
        {
        qDebug() << "[QTH] [TestApp1] \t i <= 20 trigger again()";
        QString string;
        string.append("Test app1 -->");
        string.append(48+i);
        string.append("times");
        label->setText(string);
        asyncsend();
        }
    else
        {
		/*qDebug() << "[QTH] [TestApp1] \t i > 20, setText()";
        label->setText("Test app1 Done!");
        if(request)
            {
			qDebug() << "[QTH] [TestApp1] \t \t request != null, delete request";
            delete request;
            request=NULL;
            }*/
		qDebug() << "[QTH] [TestApp1] \t request sent 20 times!!!";
        }

	qDebug() << "[QTH] [TestApp1] requestCompleted STOP";
}



void TestApp1::asyncsend()
{
	qDebug() << "[QTH] [TestApp1] asyncsend: START";

	QString testString("Test Application 1");
	int testInt(112233);

	QString service("com.nokia.services.testservice.TestService");
	QString method("asyncWithParams(QString,int)");

	if (!request)
		{
		qDebug() << "[QTH] [TestApp1] \t creating request";
		request = new XQServiceRequest(service, method, false);
		qDebug() << "[QTH] [TestApp1] \t connecting signal request->requestCompleted to slot this.requestCompleted";
		bool res = connect(request, SIGNAL(requestCompleted(QVariant)), this, SLOT(requestCompleted(QVariant)) );
		qDebug() << "[QTH] [TestApp1] \t connect = " << res;
		qDebug() << "[QTH] [TestApp1] \t connecting signal request->requestError to slot this.requestCompletedWithError";
		res = connect(request, SIGNAL(requestError(int)), this, SLOT(requestCompletedWithError(int)) );
		qDebug() << "[QTH] [TestApp1] \t connect = " << res;
		*request << testString;
		*request << testInt;
		qDebug() << "[QTH] [TestApp1] \t request: " << testString << ", " << testInt;
		}

	qDebug() << "[QTH] [TestApp1] \t before request->send()";
	bool res = request->send();
	qDebug() << "[QTH] [TestApp1] \t after request->send()";
	qDebug() << "[QTH] [TestApp1] asyncsend: STOP";
}

int main(int argc, char* argv[])
   {
    qInstallMsgHandler(XQSERVICEMESSAGEHANDLER);
    qDebug() << "[QTH] [TestApp1] main START";
    QApplication a(argc, argv);

    TestApp1* tester = new TestApp1();
    
    int rv = a.exec();    
    qDebug() << "[QTH] [TestApp1] App1 exits";
    delete tester;
    qDebug() << "[QTH] [TestApp1] main END";
	return rv;    
    }
    