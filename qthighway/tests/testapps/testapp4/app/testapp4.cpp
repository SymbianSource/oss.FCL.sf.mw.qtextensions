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

#include "testapp4.h"
#include "testdll.h"


TestApp4Widget::TestApp4Widget( QWidget *parent, Qt::WFlags f ) : QWidget( parent,f )
    {
	td = new TestDll();
    qDebug() << "[QTH] [TestApp4] TestApp4Widget: START";
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
    connect(sendButton, SIGNAL(clicked()), td, SLOT(asyncsend()));

    label = new QLabel("Test App4");

    QVBoxLayout *vl = new QVBoxLayout;
    vl->setMargin(0);
    vl->setSpacing(0);

    vl->addWidget(label);
    vl->addWidget(sendButton);
    vl->addWidget(quitButton);

    setLayout(vl);

    showFullScreen();
    
    i = 0;
    request = NULL;
    td->asyncsend();
    qDebug() << "[QTH] [TestApp4] TestApp4Widget: END";
    }

int main(int argc, char* argv[])
   {
    qInstallMsgHandler(XQSERVICEMESSAGEHANDLER);
    qDebug() << "[QTH] [TestApp4] main START";
    QApplication a(argc, argv);
	TestDll* td = new TestDll(&a);
	td->asyncsend();
	
	//TestApp4Widget* t4 = new TestApp4Widget();
	
    int rv = a.exec();    
    //delete td;
    qDebug() << "[QTH] [TestApp4] exits";
    qDebug() << "[QTH] [TestApp4] main END";
	return rv;
    }
    
