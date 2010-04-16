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
#include <QTimer>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>
#include <QString>
#include <QCheckBox>

#include <QListView>

#include "testapp2.h"

TestApp2::TestApp2(QWidget *parent, Qt::WFlags f) :
    QWidget(parent, f)
    {
    QPalette p = qApp->palette();
    QColor color(80, 20, 20);
    QColor bg(256, 20, 20);
    p.setColor(QPalette::Highlight, color.lighter(200));
    p.setColor(QPalette::Text, Qt::white);
    p.setColor(QPalette::Base, bg);
    p.setColor(QPalette::WindowText, Qt::white);
    p.setColor(QPalette::Window, bg);
    p.setColor(QPalette::ButtonText, Qt::white);
    p.setColor(QPalette::Button, color.lighter(150));
    p.setColor(QPalette::Link, QColor(240, 40, 40));

    qApp->setPalette(p);

    QPushButton *quitButton = new QPushButton(tr("quit"));
    connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

    QPushButton *sendButton = new QPushButton(tr("asynsend 10 times"));

    connect(sendButton, SIGNAL(clicked()), this, SLOT(asyncsend()));

    label = new QLabel("Test App2");

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
    }

void TestApp2::requestCompleted(const QVariant& data)
    {
    i++;
    qDebug() << "[QTH] [TestApp2] result: " << data.toString() << "Times" << i;

    if (i <= 10)
        {
        qDebug() << "[QTH] [TestApp2] trigger again()";
        QString string;
        string.append("Test app2 -->");
        string.append(48 + i);
        string.append("times");
        label->setText(string);
        asyncsend();
        }
    else
        {
        label->setText("Test app2 DONE!");
        /*if(request)
         {
         delete request;
         request=NULL;
         }*/
        qDebug() << "[QTH] [TestApp2] DONE 10-time sending !!!!!!!!!!!";
        }
    }

void TestApp2::asyncsend()
    {
    qDebug() << "[QTH] [TestApp2] asyncsend";

    QString service(
            "com.nokia.services.testservice.TestService");
    QString method("asyncNoParams()");

    if (!request)
        {
        request = new XQServiceRequest(service, method, false);
        connect(request, SIGNAL(requestCompleted(QVariant)), this,
                SLOT(requestCompleted(QVariant)));
        }

    bool res = request->send();
    }

int main(int argc, char* argv[])
    {
    qInstallMsgHandler(XQSERVICEMESSAGEHANDLER);
    qDebug() << "[QTH] [TestApp2] App2 started";
    QApplication a(argc, argv);

    TestApp2* tester = new TestApp2();

    int rv = a.exec();
    qDebug() << "[QTH] [TestApp2] App2 exits";
    delete tester;
    return rv;
    }

