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

#include <QtGlobal>
#include <QApplication>
#include <QKeyEvent>
#include <QLabel>
#include <QStackedWidget>
#include <QImageReader>
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QList>
#include <QLineEdit>
#include <QString>
#include <QCheckBox>
#include <QAction>
#include <QListView>
#include <QUrl>
#include <QFileInfo>
#include <math.h>
#include <QCompleter>
#include <xqserviceglobal.h>
#include <QDir>
#include <QTranslator>
#include <cntservicescontact.h>

#include <XQSharableFile.h>

// Include declarations to see if any compilation issues
#include <xqaiwdecl.h>
#include <xqaiwdeclplat.h>

#include "appmgrservices.h"
#include "appmgrclient.h"


AppMgrClient::AppMgrClient(QWidget *parent, Qt::WFlags f)
    : QWidget(parent, f),
      actionButton(0),
      req1(0),
      req2(0),
      req3(0),
      req4(0),
      req5(0),
      req6(0),
      req7(0),
      req8(0),
      req9(0),
      req10(0),
      mImplIndex(0)      
{
    /* Adjust the palette */
#if defined(Q_WS_S60)
    QPalette p = qApp->palette();
    QColor color(192,192,192);
    QColor bg(0,128,192);
    p.setColor(QPalette::Highlight, color.lighter(200));
    p.setColor(QPalette::Text, Qt::black);
    p.setColor(QPalette::Base, bg);
    p.setColor(QPalette::WindowText, Qt::black);
    p.setColor(QPalette::Window, bg);
    p.setColor(QPalette::ButtonText, Qt::black);
    p.setColor(QPalette::Button, color.lighter(150));
    p.setColor(QPalette::Link, QColor(240,40,40));
    qApp->setPalette(p);
#endif

    QPushButton *quitButton = new QPushButton(tr("quit"));
    connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

    
    QPushButton *testButton1 = 0;
    QPushButton *anyTestButton = 0;

    testButton1 = new QPushButton("Tests");
    mMenu = new QMenu(this);
    testButton1->setMenu(mMenu);
    anyTestButton = new QPushButton(tr("Any test"));
    connect(anyTestButton, SIGNAL(clicked()), this, SLOT(anyTest()));

    QAction *test1 = new QAction("1:Interface", this);
    connect(test1, SIGNAL(triggered()), this, SLOT(test1()));
    QAction *test2 = new QAction("2:Descriptor", this);
    connect(test2, SIGNAL(triggered()), this, SLOT(test2()));
    QAction *test3 = new QAction("3:Errors", this);
    connect(test3, SIGNAL(triggered()), this, SLOT(test3()));
    QAction *test4 = new QAction("4:QAction", this);
    connect(test4, SIGNAL(triggered()), this, SLOT(test4()));
    QAction *test5 = new QAction("5:appto:", this);
    connect(test5, SIGNAL(triggered()), this, SLOT(test5()));
    QAction *test6 = new QAction("6:testto:", this);
    connect(test6, SIGNAL(triggered()), this, SLOT(test6()));
    QAction *test7 = new QAction("7:MIME", this);
    connect(test7, SIGNAL(triggered()), this, SLOT(test7()));
    QAction *test8 = new QAction("8:URI", this);
    connect(test8, SIGNAL(triggered()), this, SLOT(test8()));
    QAction *test9 = new QAction("9:XQSharableFile", this);
    connect(test9, SIGNAL(triggered()), this, SLOT(test9()));
    QAction *test10 = new QAction("10:Select contact", this);
    connect(test10, SIGNAL(triggered()), this, SLOT(test10()));
    QAction *test11 = new QAction("11:getDrmAttr", this);
    connect(test11, SIGNAL(triggered()), this, SLOT(test11()));

    mMenu = new QMenu(this);
    mMenu->addAction(test1);
    mMenu->addAction(test2);
    mMenu->addAction(test3);
    mMenu->addAction(test4);
    mMenu->addAction(test5);
    mMenu->addAction(test6);
    mMenu->addAction(test7);
    mMenu->addAction(test8);
    mMenu->addAction(test9);
    mMenu->addAction(test10);
    mMenu->addAction(test11);

    testButton1->setMenu(mMenu);
    
    mCheckEmbedded = new QCheckBox("Embedded");
    mSynchronous = new QCheckBox("Synchronous");
    mBackground = new QCheckBox("Background");
    mForeground = new QCheckBox("Foreground");
    mCheckDeleteRequest = new QCheckBox("Delete request");
    mGenericSend = new QCheckBox("Use generic send()");
    mCheckEmbedded->setCheckState(Qt::Checked);
    mSynchronous->setCheckState(Qt::Checked);
    mGenericSend->setCheckState(Qt::Checked);  // Apply one send() for both embedded/non-embedded
    
    mReqArg = new QLineEdit("?");
    
    mTextRetValue = new QLineEdit("no ret value set");

    QFileInfo appinfo (qApp->applicationFilePath());
    mAppName = appinfo.baseName();

    QLabel *label = new QLabel(mAppName);

    vl = new QVBoxLayout;
    vl->setMargin(0);
    vl->setSpacing(0);

    vl->addWidget(label);
    vl->addWidget(mCheckEmbedded);
    vl->addWidget(mSynchronous);
    vl->addWidget(mBackground);
    vl->addWidget(mForeground);
    vl->addWidget(mCheckDeleteRequest);
    vl->addWidget(mGenericSend);
    vl->addWidget(mReqArg);
    vl->addWidget(mTextRetValue);
    vl->addWidget(new QLabel(" "));
    vl->addWidget(testButton1);
    vl->addWidget(anyTestButton);
    vl->addWidget(quitButton);

    setLayout(vl);

#if defined(Q_WS_X11) || defined(Q_WS_WIN)
    setFixedSize(QSize(360,640)); // nHD
#elif defined(Q_WS_S60)
    // showMaximized();
    showFullScreen();
#endif

}

AppMgrClient::~AppMgrClient()
{
    delete req1;
    delete req2;
    delete req3;
    delete req4;
    delete req5;
    delete req6;
    delete req7;
    delete req8;
    delete req9;
    delete req10;
    delete mMenu;
    
}

void AppMgrClient::test(XQAiwRequest **req, const QString& interface, const QString& operation)
{
    qDebug() << mAppName << " test START"; 

    bool embed = (mCheckEmbedded->checkState() == Qt::Checked);
    bool sync = (mSynchronous->checkState() == Qt::Checked);
    bool background = (mBackground->checkState() == Qt::Checked);
    
    qDebug() << mAppName << " test: embed=" << embed << ",sync=" << sync << "background=" << background;
    
    if (!*req)
    {
        *req = appmgr.create(interface, operation);
        if (!*req)
        {
            qDebug() << mAppName << " Last error=" << appmgr.lastError();
            return;
        }
        connectSignals(*req);        
    }
    // Test embedded funcions
    (*req)->setEmbedded(embed);
    (*req)->setSynchronous(sync);
    (*req)->setBackground(background);
    
    
    qDebug("%s::isEmbbedded %d", qPrintable(mAppName), (*req)->isEmbedded());
    
    test(req, mReqArg->text());

    qDebug() << mAppName << " test END";    
    
}


void AppMgrClient::test(XQAiwRequest **req, XQAiwInterfaceDescriptor &impl, const QString& operation)
{
    qDebug() << mAppName << " test START";    

    if (!*req)
    {
        *req = appmgr.create(impl, operation);
        connectSignals(*req);        
    }
    // Test embedded funcions
    qDebug("%s::isEmbbedded %d", qPrintable(mAppName),(*req)->isEmbedded());

    test(req, mReqArg->text());

    qDebug() << mAppName << " test END";    

}



void AppMgrClient::test(XQAiwRequest **req, const QString &arg)
{
    qDebug() << mAppName << " testreq START";    

    if (!req || !*req)
    {
        qDebug() <<  mAppName << " AIW-ERROR::NULL request";
        return;
    }

    
    bool embed = (mCheckEmbedded->checkState() == Qt::Checked);
    bool sync = (mSynchronous->checkState() == Qt::Checked);
    bool background = (mBackground->checkState() == Qt::Checked);
    bool foreground = (mForeground->checkState() == Qt::Checked);
    
    // Set arguments for request
    QList<QVariant> args;
    args << arg;
    if ((*req)->operation() == OPERATION1)
    {
        qDebug() << mAppName << " test: add bool arg" << !sync;
        args << QVariant(!sync);
    }
    (*req)->setArguments(args);

    bool genericSend = (mGenericSend->checkState() == Qt::Checked);
    (*req)->setEmbedded(embed);
    (*req)->setSynchronous(sync);
    (*req)->setBackground(background);
    
    // Apply additional options
    XQRequestInfo options;
    options.setForeground(foreground);
    (*req)->setInfo(options);
    
    // Make the request
    if (genericSend || !sync)
    {
        if (!(*req)->send())
        {
            qDebug() << mAppName << " AIW-ERROR:test: Send failed" << (*req)->lastError();;
        }
    }
    else if (!genericSend && sync)
    {
        QVariant retValue;
        if (!(*req)->send(retValue))
        {
            qDebug() << mAppName <<  " AIW-ERROR: test: Send(retValue) failed" << (*req)->lastError();;
        }
        else
        {
            if (retValue.canConvert<QString>())
            {
                qDebug("%s::retValue=%s,%s", qPrintable(mAppName),
                       retValue.typeName(),
                       qPrintable(retValue.value<QString>()));
                mTextRetValue->setText(retValue.value<QString>());
            }
            else
            {
                qDebug("%s:retValue=%s", qPrintable(mAppName),
                       retValue.typeName());
                mTextRetValue->setText("Not displayable");
            }

        }
    }

    // Delete request if wanted
    bool deleteRequest = (mCheckDeleteRequest->checkState() == Qt::Checked);
    
    if ((*req)->lastError() == XQService::EMessageNotFound)
    {
        // Slot was not found
        deleteRequest = true;
    }
        
    if (deleteRequest)
    {
        qDebug() << mAppName <<  " AIW-NOTE: Request deleted";
        delete *req;
        *req = 0;
    }

    qDebug() << mAppName << " test END";

    update();
    

}


void AppMgrClient::connectSignals(XQAiwRequest *req)
{
    if (req)
    {
            // Connect signals once
        connect(req, SIGNAL(requestOk(const QVariant&)), this, SLOT(handleOk(const QVariant&)));
        connect(req, SIGNAL(requestError(int,const QString&)), this, SLOT(handleError(int,const QString&)));
    }
}


void AppMgrClient::createTestFile(const QString &dir, const QString &fileName)
{

    QDir d(".");
    d.mkpath(dir);
    qDebug() << dir;

    QFile file(dir + "/" + fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << mAppName <<  " Creating file failed " << QString(dir + "/" + fileName);
        return;
    }
    QTextStream out(&file);
    out << "Text in file" << "\n";
    file.close();
    
}

bool AppMgrClient::testRunning(const QString & service, const QString & interface)
{

    QList<XQAiwInterfaceDescriptor> impls = appmgr.list(service, interface, QString(""));
    qDebug() << mAppName <<  " isRunning" << impls.count();
    if (impls.count() > 0)
    {
        bool b = appmgr.isRunning(impls[0]);
        qDebug() << mAppName <<  " isRunning=" << b;
        return b;
    }
    else
    {
        qDebug("%s isRunning: no service found (%s,%s)", qPrintable(mAppName),
               qPrintable(service),qPrintable(interface));
        return false;
    }
   
}


void AppMgrClient::test1()
{

    qDebug() << mAppName << " test1 START";
    test(&req1, IDIAL, OPERATION1);
    qDebug() << mAppName << " test1 END";

    qDebug("%s::isRunning=%d", qPrintable(mAppName), testRunning("com.nokia.services.serviceapp", IDIAL));
    
}


void AppMgrClient::test2()
{
    qDebug() << mAppName << " test2 START";
    
    QList<XQAiwInterfaceDescriptor> list=appmgr.list(IDIAL, "");
    qDebug() << mAppName << " list implementations: " << list.count();
    int i=0;
    Q_ASSERT(list.count() > 0);
    foreach (XQAiwInterfaceDescriptor d, list)
    {
        qDebug("%s::Service[%d]=%s",qPrintable(mAppName),i,qPrintable(d.serviceName()));
        qDebug("%s::Interface[%d]=%s",qPrintable(mAppName),i,qPrintable(d.interfaceName()));
        qDebug("%s::Implementation Id[%d]=%x",qPrintable(mAppName),i,d.property(XQAiwInterfaceDescriptor::ImplementationId).toInt());
        qDebug("%s::isRunning=%d", qPrintable(mAppName), testRunning(d.serviceName(), d.interfaceName()));
        qDebug("%s::status=%d", qPrintable(mAppName), appmgr.status(d));
        i++;
    }

    QList<XQAiwInterfaceDescriptor> list2=appmgr.list("com.nokia.services.serviceapp", IDIAL, "");
    qDebug() << mAppName << " list implementations 2: " << list2.count();
    i=0;
    Q_ASSERT(list2.count() > 0);
    foreach (XQAiwInterfaceDescriptor d, list2)
    {
        qDebug("%s::Service[%d]=%s",qPrintable(mAppName),i,qPrintable(d.serviceName()));
        qDebug("%s::Interface[%d]=%s",qPrintable(mAppName),i,qPrintable(d.interfaceName()));
        qDebug("%s::Implementation Id[%d]=%x",qPrintable(mAppName),i,d.property(XQAiwInterfaceDescriptor::ImplementationId).toInt());
        i++;
    }

    QList<XQAiwInterfaceDescriptor> list3=appmgr.list("serviceapp", IDIAL, "");
    qDebug() << mAppName << " New: list implementations: " << list3.count();
    i=0;
    Q_ASSERT(list3.count() > 0);
    foreach (XQAiwInterfaceDescriptor d, list3)
    {
        qDebug("%s::Service[%d]=%s",qPrintable(mAppName),i,qPrintable(d.serviceName()));
        qDebug("%s::Interface[%d]=%s",qPrintable(mAppName),i,qPrintable(d.interfaceName()));
        qDebug("%s::Implementation Id[%d]=%x",qPrintable(mAppName),i,d.property(XQAiwInterfaceDescriptor::ImplementationId).toInt());
        i++;
    }
    
    if (!list.isEmpty())
    {
        // Use the first found
        qDebug() << mAppName << " Using implementation nbr: " << mImplIndex;
        test(&req2,list[mImplIndex], OPERATION1);
    }

    qDebug() << mAppName << " test2 END";
    
}

void AppMgrClient::test3()
{
    qDebug() << mAppName << " test3 START";
    
    test(&req3,IDIAL,ERR_OPERATION1);
    // test(&req3,ERR_IDIAL,ERR_OPERATION1);
    // test(&req3,ERR_IDIAL,ERR_OPERATION1);
    
    qDebug() << mAppName << " test3 END";
    
}

void AppMgrClient::test4()
{
    qDebug() << mAppName << " test4 START";

    bool embed = (mCheckEmbedded->checkState() == Qt::Checked);
    bool sync = (mSynchronous->checkState() == Qt::Checked);
    bool foreground = (mForeground->checkState() == Qt::Checked);

    if (req4)
    {
        delete req4;  // Previous
        req4 = 0;
    }

    // Only hbserviceprovider support localization
    req4 = appmgr.create(QLatin1String("com.nokia.services.hbserviceprovider"), IDIAL, OPERATION1);
    if (!req4)
    {
        qDebug() << mAppName << " AIW-ERROR::NULL request";
        return;
    }

    connectSignals(req4);
    req4->setSynchronous(sync);

    // In this test case, apply "options" for other options
    XQRequestInfo options;
    options.setEmbedded(embed);
    options.setForeground(foreground);
    req4->setInfo(options);

    if (actionButton)
    {
        // Previous
        vl->removeWidget(actionButton);
        delete actionButton;
        actionButton = 0;
        update();
    }
    
    QAction *action = req4->createAction();  // Also connects the triggered event to req !!!!
    qDebug() << mAppName << " action:" << action->isEnabled();
    
    // Create UI
    if (action)
    {
        actionButton = new QPushButton(action->text());
        actionButton->addAction(action);
        vl->addWidget(actionButton);
        setLayout(vl);
        // For some reason triggered does not come from request
        // Workaround...
        connect(actionButton, SIGNAL(clicked()), action, SIGNAL(triggered()));
        connect(req4, SIGNAL(triggered()), this, SLOT(test4ActionTriggered()));
    }
    else
    {
        qDebug() << mAppName << " test4 No action available";

    }

    qDebug() << mAppName << " test4 END";
    
}

void AppMgrClient::test4ActionTriggered()
{
    XQAiwRequest *r = (XQAiwRequest *)sender();
    
    // Set arguments for request
    bool embed = (mCheckEmbedded->checkState() == Qt::Checked);
    QList<QVariant> args;
    args << QVariant(mReqArg->text());
    args << QVariant(!embed);
    r->setArguments(args);

}

void AppMgrClient::test5()
{
    qDebug() << mAppName << " test5 START";

    QString str("e0022e70");
    uint ui = str.toUInt();
    qDebug() << mAppName << " toUInt value=" << ui;
    bool b=false;
    ui=str.toUInt(&b,16);
    qDebug() << mAppName << " toUInt status=" << b <<  ",value=" << ui;
    
    
    QUrl uri(XQURI_SCHEME_ACTIVITY + "://E0022E70?" + XQURI_KEY_ACTIVITY_NAME + "=MainView&key1=data1&key2=data2"); 
    qDebug() << mAppName << " Uri=" << uri.toString();
    qDebug() << mAppName << " isValid=" << uri.isValid();
    qDebug() << mAppName << " Uri authority=" << uri.authority();
    QString old=mReqArg->text();
    if (!req5)
    {
        req5 = appmgr.create(uri);
        connectSignals(req5); 
    }
    
    mReqArg->setText(uri.encodedQuery()); 
    test(&req5, mReqArg->text());
    mReqArg->setText(old);
    
    qDebug() << mAppName << " test5 END";
    
}

void AppMgrClient::test6()
{
    qDebug() << mAppName << " test6 START";

    QUrl uri("test2to://authority?param1=value1&param1=value2"); 
    // QUrl uri(mReqArg->text()); 
    qDebug() << mAppName << " Uri=" << uri.toString();
    qDebug() << mAppName << " isValid=" << uri.isValid();
    qDebug() << mAppName << " Uri authority=" << uri.authority();
    
    if (!uri.isValid())
    {
        qDebug() << mAppName << " Invalid URI " << mReqArg->text();
        return;
    }

    QList<XQAiwInterfaceDescriptor> uriHandlers = appmgr.list(uri);
    // Note : Only services supporting custom property are returned
    foreach (XQAiwInterfaceDescriptor d, uriHandlers)
    {
        qDebug() << mAppName << " Service=" << d.serviceName();
        qDebug() << mAppName << " Interface=" << d.interfaceName();
        qDebug("%s::Implementation Id=%x",qPrintable(mAppName),d.property(XQAiwInterfaceDescriptor::ImplementationId).toInt());
        qDebug("%s::isRunning=%d", qPrintable(mAppName), testRunning(d.serviceName(), d.interfaceName()));
    }

    if (!req6)
    {
        req6 = appmgr.create(uri);
        connectSignals(req6);
    }

    test(&req6, uri.toString());

    qDebug() << mAppName << " test6 END";
    
}

void AppMgrClient::test7()
{
    qDebug() << mAppName << " test7 START";
    
    // Should launch viewer for text/plain MimeTestApp.
    // Create test file
    createTestFile("C:/data/Others", "test.txt");

    // Copy files from DrmTestFiles.zip into correct location
    QFile file("C:/data/Others/one.jpg");
    qDebug() << mAppName << " File=" << file.fileName();
    qDebug() << mAppName << " exists=" << file.exists();

    QList<XQAiwInterfaceDescriptor> fileHandlers = appmgr.list(file);
    foreach (XQAiwInterfaceDescriptor d, fileHandlers)
    {
        qDebug() << mAppName << " Service=" << d.serviceName();
        qDebug() << mAppName << " Interface=" << d.interfaceName();
        qDebug() << mAppName << " Implementation Id=" << d.property(XQAiwInterfaceDescriptor::ImplementationId).toInt();
    }
    
    if (!req7)
    {
        req7 = appmgr.create(file);
        connectSignals(req7); 
    }
    test(&req7, file.fileName());

    qDebug() << mAppName << " test7 END";
    
}


void AppMgrClient::test8()
{
    qDebug() << mAppName << " test8 START";

    // E0022E73 == ServiceApp
    QUrl uri("http://www.nokia.com");
    
    // Copy files from DrmTestFiles.zip into correct location
    // Uncomment for "file" scheme testing
    // QUrl uri("file:///C:/data/Others/one.jpg");
    
    qDebug() << mAppName << " Uri=" << uri.toString();
    qDebug() << mAppName << " isValid=" << uri.isValid();
    qDebug() << mAppName << " Uri authority=" << uri.authority();

    if (!req8)
    {
        req8 = appmgr.create(uri);
        connectSignals(req8); 
    }
    test(&req8, uri.toString());

    qDebug() << mAppName << " test8 END";
    
}

void AppMgrClient::test9()
{

    qDebug() << mAppName << " test9 START";    

    bool embed = (mCheckEmbedded->checkState() == Qt::Checked);
    bool sync = (mSynchronous->checkState() == Qt::Checked);
    bool deleteRequest = (mCheckDeleteRequest->checkState() == Qt::Checked);


    // Access data-caged file 
    XQSharableFile sf;
    QString fileDir = "c:/private/e0022e74";
    
    if (mAppName == "appmgrclient2")
    {
        fileDir = "c:/private/e0022e76";
    }

    createTestFile(fileDir, "test.txt");
    if (!sf.open(fileDir + "\\test.txt"))
    {
        qDebug() << mAppName << " file open failed " << (fileDir + "/test.txt");
        return;
    }

    // Just test listing by sharable file
    QList<XQAiwInterfaceDescriptor> fileHandlers = appmgr.list(sf);
    if (fileHandlers.count() > 0)
    {
        XQAiwInterfaceDescriptor d = fileHandlers.first();
        qDebug() << mAppName << " File Service=" << d.serviceName();
        qDebug() << mAppName << " File Interface=" << d.interfaceName();
        qDebug() << mAppName << " Handler Implementation Id=" << d.property(XQAiwInterfaceDescriptor::ImplementationId).toInt();
        if (!req9)
        {
            // Create by descriptor
            req9 = appmgr.create(sf, d);
        }
        if (!req9)
        {
            sf.close();
            qDebug() << mAppName <<  " anyTest: ERROR IN XQAppMgr API";
            return ;
        }
    }
    else
    {
        sf.close();
        qDebug() << mAppName <<  " anyTest: NO HANDLER FOUND";
    }

    connectSignals(req9);        
    req9->setEmbedded(embed);
    req9->setSynchronous(sync);
    
    QList<QVariant> args;
    args << qVariantFromValue(sf);
    req9->setArguments(args);
    
    req9->send();
    if (req9->lastError() == XQService::EMessageNotFound)
    {
        // Slot was not found
        qDebug() << mAppName <<  " AIW-ERROR:XQService::EMessageNotFound";
        deleteRequest = true;
    }
    // Remember to close the file !!!
    sf.close();

    if (deleteRequest)
    {
        delete req9;
        req9 = 0;
    }

    qDebug() << mAppName << " test9 END";
    
}

// Test 10
void AppMgrClient::test10()
{

    qDebug() << mAppName << " test10 START";

    bool embed = (mCheckEmbedded->checkState() == Qt::Checked);
    bool sync = (mSynchronous->checkState() == Qt::Checked);
    bool background = (mBackground->checkState() == Qt::Checked);

    qDebug() << mAppName <<  " test10: embed=" << embed << ",sync=" << sync << "background=" << background;

    if (!req10)
    {
        req10 = appmgr.create(QLatin1String("com.nokia.services.phonebookservices"), QLatin1String("Fetch"), QLatin1String(""));
        connect(req10, SIGNAL(requestOk(const QVariant&)), this, SLOT(showRecipients(const QVariant&)));
        connect(req10, SIGNAL(requestError(int,const QString&)), this, SLOT(handleError(int,const QString&)));
    }

    if (!req10)
    {
        qDebug() << mAppName <<  " AIW-ERROR: NULL request";
        return;
    }

    // Set request attributes
    req10->setOperation("fetch(QString,QString,QString)");
    req10->setEmbedded(embed);
    req10->setSynchronous(sync);
    req10->setBackground(background);

    // Set arguments
    QList<QVariant> args;
    args << "Contact"; 
    args << KCntActionAll;   
    args << KCntFilterDisplayAll;
    req10->setArguments(args);

    // Send the request
    req10->send();

    bool deleteRequest = (mCheckDeleteRequest->checkState() == Qt::Checked);
    if (deleteRequest)
    {
        delete req10;
        req10 = 0;
    }
    
    qDebug() << mAppName << " test10 END";


}


void AppMgrClient::test11()
{
    qDebug() << mAppName << " test11 START";

    // Copy files from DrmTestFiles.zip into correct location
    QList<QString> drmFiles;
    drmFiles.append("C:/data/Others/RoAcqoffer-111-aac-i15m.ort");
    drmFiles.append("C:/data/Others/SD_Celebration_SP.dcf");
    drmFiles.append("C:/data/Others/111-test1.odf");
    drmFiles.append("C:/data/Others/SD_jpg_sun.dcf");
    drmFiles.append("C:/data/Others/STC1_128_44_16_2_CBR.wma");
    drmFiles.append("C:/data/Others/test.txt");
    drmFiles.append("C:/data/Others/foo.txt");

    QList<int> attrNames;
    attrNames.append(XQApplicationManager::MimeType);
    attrNames.append(XQApplicationManager::IsProtected);
    attrNames.append(XQApplicationManager::IsForwardable);

    // Test with file names
    foreach (QString f, drmFiles)
    {
        QFile file(f);
        QVariantList attrValues;
        bool ok = appmgr.getDrmAttributes(file, attrNames, attrValues);
        qDebug() << mAppName << " getDrmAttributes for " << f << " status=" << ok;
        int i=0;
        foreach (QVariant v, attrValues)
        {
            qDebug() << mAppName << " Attribute " << attrNames[i] << "=" << qPrintable(v.toString());
            i++;
        }
    }

    // Test with file handles
    foreach (QString f, drmFiles)
    {
        XQSharableFile file;
        file.open(f);  // Create handle manually
        QVariantList attrValues;
        bool ok = appmgr.getDrmAttributes(file, attrNames, attrValues);
        qDebug() << mAppName << " getDrmAttributes for file " << file.fileName() << " handle status=" << ok;
        int i=0;
        foreach (QVariant v, attrValues)
        {
            qDebug() << mAppName << " Attribute " << attrNames[i] << "=" << qPrintable(v.toString());
            i++;
        }
        file.close();
    }
    
    qDebug() << mAppName << " test11 END";

}


void AppMgrClient::showRecipients(const QVariant &value)
{
    qDebug("%s::showRecipients::variant(%d,%s)", qPrintable(mAppName),value.type(), value.typeName());
    
    CntServicesContactList list;
    if(value.canConvert<CntServicesContactList>())
    {
        qDebug() << mAppName << " showRecipients: canConvert";
        list = qVariantValue<CntServicesContactList>(value);
    }
    else
    {
        qDebug() << mAppName << " showRecipients: canConvert NOK !!!";
        return;
    }    

    if (list.count() == 0)
    {
        qDebug() << mAppName << " showRecipients: Count==0";
    }
    else {
        for (int i = 0; i < list.count(); ++i)
        {
            qDebug() << mAppName << " showRecipients[" << i << "]=" << list[i].mDisplayName;
            qDebug() << mAppName << " showRecipients[" << i << "]=" << list[i].mPhoneNumber;
            qDebug() << mAppName << " showRecipients[" << i << "]=" << list[i].mEmailAddress;

        }
    }
}




#define TXT_ID QLatin1String("txt_aiw_action_text")

void AppMgrClient::anyTest()
{

    qDebug() << mAppName << " anyTest START";

    bool embed = (mCheckEmbedded->checkState() == Qt::Checked);
    bool sync = (mSynchronous->checkState() == Qt::Checked);
    bool background = (mBackground->checkState() == Qt::Checked);

    XQAiwRequest *req=0;
    req = appmgr.create(QLatin1String("com.nokia.services.serviceapp"), IDIAL, QLatin1String("testContactList(CntServicesContactList)"));

    if (!req)
    {
        qDebug() << mAppName << " AIW-ERROR NULL request";
        return;
    }

    // Comment next line  if using the operation signature given in the "create"
    req->setOperation(QLatin1String("testVariant(QVariant)"));
    
    connectSignals(req);        

    // Set request attributes
    req->setEmbedded(embed);
    req->setSynchronous(sync);
    req->setBackground(background);

    QList<QVariant> args;
    
    // Just construct dummies
    MetaDummy1 dummy1;
    MetaDummy2 dummy2;
    
    CntServicesContact cnt1;
    cnt1.mDisplayName = "Test1";
    cnt1.mPhoneNumber = "050-1111111";
    cnt1.mEmailAddress = "test1.test@nokia.com";
    
    CntServicesContact cnt2;
    cnt2.mDisplayName = "Test2";
    cnt2.mPhoneNumber = "050-2222222";
    cnt2.mEmailAddress = "test2.test@nokia.com";

    CntServicesContactList list;
    list.append(cnt1);
    list.append(cnt2);
    
    args.clear();
    args << qVariantFromValue(list);
    req->setArguments(args);

    req->send();
    
    bool deleteRequest = (mCheckDeleteRequest->checkState() == Qt::Checked);
    if (deleteRequest)
    {
        delete req;
        req = 0;
    }

    qDebug() << mAppName << " test END";

    //  ---- OLD TESTS ------
    
    /*
    qDebug() << mAppName << "  name" << qApp->applicationName();
    qDebug() << mAppName << " dirpath" << qApp->applicationDirPath();
    qDebug() << mAppName << " filename" << qApp->applicationFilePath();
    qDebug() << mAppName << " pid" << qApp->applicationPid();

    QFileInfo appinfo (qApp->applicationFilePath());
    qDebug() << mAppName << " appinfo.applicationFilePath" << qApp->applicationFilePath();
    qDebug() << mAppName << " appinfo.absolutePath" << appinfo.absolutePath();
    qDebug() << mAppName << " appinfo.baseName" << appinfo.baseName();

    QString lang = QLocale::system().name();
    qDebug() << mAppName << " anyTest:" << lang;

    // QString textFile = "z:/resource/qt/translations/hbserviceprovider";
    QString textFile = "hbserviceprovider";
    QFileInfo info(textFile);
    qDebug() << mAppName << " base" << info.baseName();
    qDebug() << mAppName << " path" << info.filePath();
    if (info.baseName() == info.filePath())
    {
        textFile = qApp->applicationFilePath().left(2) + "/resource/qt/translations/" + textFile;
        qDebug() << mAppName << " path added" << textFile;
    }

    textFile += "_"; 
    textFile += lang;
    qDebug() << mAppName << " anyTest:" << textFile;

    QTranslator translator;
    bool res = translator.load(textFile);
    qDebug() << mAppName << " anyTest:" << res;
    if (res)
    {
        qApp->installTranslator(&translator);
    }

    QString textId = TXT_ID;
    QByteArray ba = textId.toLatin1();
    const char *textPtr = ba.data();

    QString text = qtTrId(textPtr);  // translate
    qDebug() << mAppName << " translated text:" << text;

    qApp->removeTranslator(&translator);

    Q_ASSERT(0==1);
    */

}




// Aiw request responses
void AppMgrClient::handleOk(const QVariant& result)
{
    XQAiwRequest *r = static_cast<XQAiwRequest *>(sender());
    
    int impl=-1;
    impl = (r->descriptor().property(XQAiwInterfaceDescriptor::ImplementationId)).toInt();
    QString interface = r->descriptor().interfaceName();
    QString service = r->descriptor().serviceName();
    
    if (result.canConvert<CntServicesContactList>())
    {
        showRecipients(result);
    }
    else if (result.canConvert<QString>())
    {
        qDebug("%s::handleOk from [%s.%s,%x]=(%s,%s)",
               qPrintable(mAppName),
               qPrintable(service),
               qPrintable(interface),
               impl,
               result.typeName(),
               qPrintable(result.value<QString>()));
        mTextRetValue->setText(result.value<QString>());
    }

    else
    {
        qDebug("%s::handleOk from [%s.%s,%x]=(%s)",
               qPrintable(mAppName),
               qPrintable(service),
               qPrintable(interface),
               impl,
               result.typeName());
        mTextRetValue->setText("Not displayable");
    }
}
  
void AppMgrClient::handleError(int errorCode, const QString& errorMessage)
{
    XQAiwRequest *r = static_cast<XQAiwRequest *>(sender());
    QString interface = r->descriptor().interfaceName();
    QString service = r->descriptor().serviceName();
    
    int impl=-1;
    impl = (r->descriptor().property(XQAiwInterfaceDescriptor::ImplementationId)).toInt();

    qDebug("%s::handleError from [%s.%s,%d]=(%d,%s)",
           qPrintable(mAppName),
           qPrintable(service),
           qPrintable(interface),
           impl, errorCode, qPrintable(errorMessage));
    
    mTextRetValue->setText(errorMessage);
}



Q_IMPLEMENT_USER_METATYPE(MetaDummy1)
Q_IMPLEMENT_USER_METATYPE(MetaDummy2)
Q_IMPLEMENT_USER_METATYPE(CntServicesContact)
Q_IMPLEMENT_USER_METATYPE_NO_OPERATORS(CntServicesContactList)
