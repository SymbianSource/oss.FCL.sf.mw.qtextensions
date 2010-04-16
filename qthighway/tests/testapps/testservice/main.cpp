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

#include <QApplication>
#include "testservice.h"
#include <xqservicelog.h>


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

#include "main.h"

TestServiceView::TestServiceView( QWidget *parent, Qt::WFlags f ) : QWidget( parent,f )
{
    qDebug() << "[QTH] [TestService] TestServiceView::TestServiceView START";
    QPalette p = qApp->palette();
    //p.setColor(QPalette::Highlight, color.lighter(200));
    //p.setColor(QPalette::Text, Qt::white);
    //p.setColor(QPalette::Base, bg);
    //p.setColor(QPalette::WindowText, Qt::red);
    //p.setColor(QPalette::Window, Qt::yellow);
    //p.setColor(QPalette::ButtonText, Qt::blue);
    //p.setColor(QPalette::Button, Qt::green);
    //p.setColor(QPalette::Link, QColor(240,40,40));

    qApp->setPalette(p);

    QPushButton *quitButton = new QPushButton(tr("quit"));
    connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

    label = new QLabel("TEST SERVICE");

    QVBoxLayout *vl = new QVBoxLayout;
    vl->setMargin(0);
    vl->setSpacing(0);

    vl->addWidget(label);
    //vl->addWidget(sendButton);
    vl->addWidget(quitButton);

    setLayout(vl);
    //showMaximized();
    showFullScreen();
    service = new TestService();
    qDebug() << "[QTH] [TestService] TestServiceView::TestServiceView END";
}

TestServiceView::~TestServiceView()
{
    delete service;
}

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    qInstallMsgHandler(XQSERVICEMESSAGEHANDLER); 
    TestServiceView view;
    return a.exec();
}
