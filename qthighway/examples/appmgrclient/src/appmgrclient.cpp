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
#include <XQSharableFile.h>

#include <xqaiwdeclplat.h>

#include "appmgrservices.h"
#include "appmgrclient.h"


AppMgrClient::AppMgrClient(QWidget *parent, Qt::WFlags f)
    : QWidget(parent, f),
      req1(0),
      req2(0),
      req3(0),
      req4(0),
      req5(0),
      req6(0),
      req7(0),
      req8(0),
      req9(0),
      actionButton(0),
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

    QPushButton *testButton1 = 0;
    QPushButton *anyTestButton = 0;
    
    QPushButton *quitButton = new QPushButton(tr("quit"));
    connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

    testButton1 = new QPushButton("Tests");
    mMenu = new QMenu(this);
    testButton1->setMenu(mMenu);
    anyTestButton = new QPushButton(tr("Any test"));
    connect(anyTestButton, SIGNAL(clicked()), this, SLOT(anyTest()));

    QAction *test1 = new QAction("Interface", this);
    connect(test1, SIGNAL(triggered()), this, SLOT(test1()));
    QAction *test2 = new QAction("Descriptor", this);
    connect(test2, SIGNAL(triggered()), this, SLOT(test2()));
    QAction *test3 = new QAction("Errors", this);
    connect(test3, SIGNAL(triggered()), this, SLOT(test3()));
    QAction *test4 = new QAction("QAction", this);
    connect(test4, SIGNAL(triggered()), this, SLOT(test4()));
    QAction *test5 = new QAction("appto:", this);
    connect(test5, SIGNAL(triggered()), this, SLOT(test5()));
    QAction *test6 = new QAction("testto:", this);
    connect(test6, SIGNAL(triggered()), this, SLOT(test6()));
    QAction *test7 = new QAction("MIME", this);
    connect(test7, SIGNAL(triggered()), this, SLOT(test7()));
    QAction *test8 = new QAction("URI", this);
    connect(test8, SIGNAL(triggered()), this, SLOT(test8()));
    QAction *test9 = new QAction("XQSharableFile", this);
    connect(test9, SIGNAL(triggered()), this, SLOT(test9()));

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

    testButton1->setMenu(mMenu);
    
    mCheckEmbedded = new QCheckBox("Embedded");
    mSynchronous = new QCheckBox("Synchronous");
    mBackground = new QCheckBox("Background");
    mCheckDeleteRequest = new QCheckBox("Delete request");
    mGenericSend = new QCheckBox("Use generic send()");
    mCheckEmbedded->setCheckState(Qt::Checked);
    mSynchronous->setCheckState(Qt::Checked);
    mGenericSend->setCheckState(Qt::Checked);  // Apply one send() for both embedded/non-embedded
    
    mReqArg = new QLineEdit("0");
    
    mTextRetValue = new QLineEdit("no ret value set");

    QLabel *label = new QLabel("APPMGR CLIENT TEST");

    vl = new QVBoxLayout;
    vl->setMargin(0);
    vl->setSpacing(0);

    vl->addWidget(label);
    vl->addWidget(mCheckEmbedded);
    vl->addWidget(mSynchronous);
    vl->addWidget(mBackground);
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
    delete mMenu;
    
}

void AppMgrClient::test(XQAiwRequest **req, const QString& interface, const QString& operation, bool embedded)
{
    qDebug() << "AppMgrClient::test START";    

    bool embed = (mCheckEmbedded->checkState() == Qt::Checked);
    bool sync = (mSynchronous->checkState() == Qt::Checked);
    bool background = (mBackground->checkState() == Qt::Checked);
    
    qDebug() << "AppMgrClient:test: embed=" << embed << ",sync=" << sync << "background=" << background;
    
    if (!*req)
    {
        *req = appmgr.create(interface, operation);
        connectSignals(*req);        
    }
    // Test embedded funcions
    (*req)->setEmbedded(embed);
    (*req)->setSynchronous(sync);
    (*req)->setBackground(background);
    
    qDebug("AppMgrClient::isEmbbedded %d", (*req)->isEmbedded());
    
    test(req, mReqArg->text());

    qDebug() << "AppMgrClient::test END";    
    
}


void AppMgrClient::test(XQAiwRequest **req, XQAiwInterfaceDescriptor &impl, const QString& operation, bool embedded)
{
    qDebug() << "AppMgrClient::test START";    

    if (!*req)
    {
        *req = appmgr.create(impl, operation);
        connectSignals(*req);        
    }
    // Test embedded funcions
    qDebug("AppMgrClient::isEmbbedded %d", (*req)->isEmbedded());

    test(req, mReqArg->text());

    qDebug() << "AppMgrClient::test END";    

}



void AppMgrClient::test(XQAiwRequest **req, const QString &arg)
{
    qDebug() << "AppMgrClient::testreq START";    

    if (!req || !*req)
    {
        qDebug() << "AIW-ERROR:AppMgrClient::NULL request";
        return;
    }

    
    bool embed = (mCheckEmbedded->checkState() == Qt::Checked);
    bool sync = (mSynchronous->checkState() == Qt::Checked);
    bool background = (mBackground->checkState() == Qt::Checked);
    
    // Set arguments for request
    QList<QVariant> args;
    args << arg;
    args << QVariant(!sync);
    (*req)->setArguments(args);

    bool genericSend = (mGenericSend->checkState() == Qt::Checked);
    (*req)->setEmbedded(embed);
    (*req)->setSynchronous(sync);
    (*req)->setBackground(background);
    
    // Make the request
    if (genericSend || !sync)
    {
        if (!(*req)->send())
        {
            qDebug() << "AIW-ERROR: AppMgrClient:test: Send failed" << (*req)->lastError();;
        }
    }
    else if (!genericSend && sync)
    {
        QVariant retValue;
        if (!(*req)->send(retValue))
        {
            qDebug() << "AIW-ERROR: AppMgrClient:test: Send(retValue) failed" << (*req)->lastError();;
        }
        else
        {
            if (retValue.canConvert<QString>())
            {
                qDebug("AppMgrClient::retValue=%s,%s",
                       retValue.typeName(),
                       qPrintable(retValue.value<QString>()));
                mTextRetValue->setText(retValue.value<QString>());
            }
            else
            {
                qDebug("AppMgrClient:retValue=%s",
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
        qDebug() << "AIW-NOTE: Request deleted";
        delete *req;
        *req = 0;
    }

    qDebug() << "AppMgrClient::test END";

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
        qDebug() << "Creating file failed " << QString(dir + "/" + fileName);
        return;
    }
    QTextStream out(&file);
    out << "Text in file" << "\n";
    file.close();
    
}


void AppMgrClient::test1()
{
    qDebug() << "AppMgrClient::test1 START";
    test(&req1, IDIAL, OPERATION1);
    qDebug() << "AppMgrClient::test1 END";

    /*
    mReqArg->setText("77777"); 
    qDebug() << "AppMgrClient::test1 second call";
    test(&req1, IDIAL, OPERATION1);
    */
}


void AppMgrClient::test2()
{
    qDebug() << "AppMgrClient::test2 START";
    
    QList<XQAiwInterfaceDescriptor> list=appmgr.list(IDIAL, "");
    qDebug() << "AppMgrClient::Found implementations: " << list.count();
    int i=0;
    foreach (XQAiwInterfaceDescriptor d, list)
    {
        qDebug("AppMgrClient::Service[%d]=%s",i,qPrintable(d.serviceName()));
        qDebug("AppMgrClient::Interface[%d]=%s",i,qPrintable(d.interfaceName()));
        qDebug("AppMgrClient::Implementation Id[%d]=%x",i,d.property(XQAiwInterfaceDescriptor::ImplementationId).toInt());
        i++;
    }
    
    if (!list.isEmpty())
    {
        // Use the first found
        qDebug() << "AppMgrClient::Using implementation nbr: " << mImplIndex;
        test(&req2,list[mImplIndex], OPERATION1);
    }

    qDebug() << "AppMgrClient::test2 END";
    
}

void AppMgrClient::test3()
{
    qDebug() << "AppMgrClient::test3 START";
    
    test(&req3,IDIAL,ERR_OPERATION1);
    test(&req3,ERR_IDIAL,ERR_OPERATION1);
    test(&req3,ERR_IDIAL,ERR_OPERATION1);
    
    qDebug() << "AppMgrClient::test3 END";
    
}

void AppMgrClient::test4()
{
    qDebug() << "AppMgrClient::test4 START";

    bool embed = (mCheckEmbedded->checkState() == Qt::Checked);
    bool sync = (mSynchronous->checkState() == Qt::Checked);

    if (req4)
    {
        delete req4;  // Previous
        req4 = 0;
    }

    // Only hbserviceprovider support localization
    req4 = appmgr.create(QLatin1String("com.nokia.services.hbserviceprovider"), IDIAL, OPERATION1);
    if (!req4)
    {
        qDebug() << "AIW-ERROR:AppMgrClient::NULL request";
        return;
    }

    connectSignals(req4); 
    req4->setEmbedded(embed);
    req4->setSynchronous(embed);

    if (actionButton)
    {
        // Previous
        vl->removeWidget(actionButton);
        delete actionButton;
        actionButton = 0;
        update();
    }
    
    QAction *action = req4->createAction();  // Also connects the triggered event to req !!!!
    qDebug() << "AppMgrClient::action:" << action->isEnabled();
    
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
        qDebug() << "AppMgrClient::test4 No action available";

    }

    qDebug() << "AppMgrClient::test4 END";
    
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
    qDebug() << "AppMgrClient::test5 START";

    // E0022E73 == ServiceApp
    QUrl uri(XQURI_SCHEME_ACTIVITY + "://E0022E73?" + XQURI_KEY_ACTIVITY_NAME + "=emailView&view=myview"); 
    qDebug() << "AppMgrClient::Uri=" << uri.toString();
    qDebug() << "AppMgrClient::isValid=" << uri.isValid();
    qDebug() << "AppMgrClient::Uri authority=" << uri.authority();
    QString old=mReqArg->text();
    if (!req5)
    {
        req5 = appmgr.create(uri);
        connectSignals(req5); 
    }
    
    mReqArg->setText(uri.encodedQuery()); 
    test(&req5, mReqArg->text());
    mReqArg->setText(old);
    
    qDebug() << "AppMgrClient::test5 END";
    
}

void AppMgrClient::test6()
{
    qDebug() << "AppMgrClient::test6 START";

    QUrl uri("testto://authority?param1=value1&param1=value2"); 
    qDebug() << "AppMgrClient::Uri=" << uri.toString();
    qDebug() << "AppMgrClient::isValid=" << uri.isValid();
    qDebug() << "AppMgrClient::Uri authority=" << uri.authority();

    QList<XQAiwInterfaceDescriptor> uriHandlers = appmgr.list(uri);
    // Note : Only services supporting custom property are returned
    foreach (XQAiwInterfaceDescriptor d, uriHandlers)
    {
        qDebug() << "AppMgrClient::Service=" << d.serviceName();
        qDebug() << "AppMgrClient::Interface=" << d.interfaceName();
        qDebug("AppMgrClient::Implementation Id=%x",d.property(XQAiwInterfaceDescriptor::ImplementationId).toInt());
    }

    if (!req6)
    {
        req6 = appmgr.create(uri);
        connectSignals(req6); 
    }

    test(&req6, uri.toString());

    qDebug() << "AppMgrClient::test6 END";
    
}

void AppMgrClient::test7()
{
    qDebug() << "AppMgrClient::test7 START";
    
    // Should launch viewer for text/plain MimeTestApp.
    // Create test file
    createTestFile("C:/data/Others", "test.txt");
    
    QFile file("C:/data/Others/test.txt");
    qDebug() << "AppMgrClient::File=" << file.fileName();
    qDebug() << "AppMgrClient::exists=" << file.exists();

    QList<XQAiwInterfaceDescriptor> fileHandlers = appmgr.list(file);
    foreach (XQAiwInterfaceDescriptor d, fileHandlers)
    {
        qDebug() << "AppMgrClient::Service=" << d.serviceName();
        qDebug() << "AppMgrClient::Interface=" << d.interfaceName();
        qDebug() << "AppMgrClient::Implementation Id=" << d.property(XQAiwInterfaceDescriptor::ImplementationId).toInt();
    }
    
    if (!req7)
    {
        req7 = appmgr.create(file);
        connectSignals(req7); 
    }
    test(&req7, file.fileName());

    qDebug() << "AppMgrClient::test7 END";
    
}


void AppMgrClient::test8()
{
    qDebug() << "AppMgrClient::test8 START";

    // E0022E73 == ServiceApp
    QUrl uri("http://www.nokia.com"); 
    qDebug() << "AppMgrClient::Uri=" << uri.toString();
    qDebug() << "AppMgrClient::isValid=" << uri.isValid();
    qDebug() << "AppMgrClient::Uri authority=" << uri.authority();

    if (!req8)
    {
        req8 = appmgr.create(uri);
        connectSignals(req8); 
    }
    test(&req8, uri.toString());

    qDebug() << "AppMgrClient::test8 END";
    
}

void AppMgrClient::test9()
{

    qDebug() << "AppMgrClient::test9 START";    

    bool embed = (mCheckEmbedded->checkState() == Qt::Checked);
    bool sync = (mSynchronous->checkState() == Qt::Checked);
    bool deleteRequest = (mCheckDeleteRequest->checkState() == Qt::Checked);


    // Access data-caged file 
    XQSharableFile sf;
    createTestFile("c:/private/e0022e74", "test.txt");
    if (!sf.open("c:\\private\\e0022e74\\test.txt"))
    {
        qDebug() << "AppMgrClient:file open failed";
        return;
    }

    // Just test listing by sharable file
    QList<XQAiwInterfaceDescriptor> fileHandlers = appmgr.list(sf);
    if (fileHandlers.count() > 0)
    {
        XQAiwInterfaceDescriptor d = fileHandlers.first();
        qDebug() << "AppMgrClient::File Service=" << d.serviceName();
        qDebug() << "AppMgrClient::File Interface=" << d.interfaceName();
        qDebug() << "AppMgrClient::Handler Implementation Id=" << d.property(XQAiwInterfaceDescriptor::ImplementationId).toInt();
        if (!req9)
        {
            // Create by descriptor
            req9 = appmgr.create(sf, d);
        }
        if (!req9)
        {
            sf.close();
            qDebug() << "AppMgrClient:anyTest: ERROR IN XQAppMgr API";
            return ;
        }
    }
    else
    {
        sf.close();
        qDebug() << "AppMgrClient:anyTest: NO HANDLER FOUND";
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
        qDebug() << "AIW-ERROR:XQService::EMessageNotFound";
        deleteRequest = true;
    }
    // Remember to close the file !!!
    sf.close();

    if (deleteRequest)
    {
        delete req9;
        req9 = 0;
    }

    qDebug() << "AppMgrClient::test9 END";
    
}




#define TXT_ID QLatin1String("txt_aiw_action_text")

void AppMgrClient::anyTest()
{


    qDebug() << "AppMgrClient:name" << qApp->applicationName();
    qDebug() << "AppMgrClient:dirpath" << qApp->applicationDirPath();
    qDebug() << "AppMgrClient:filename" << qApp->applicationFilePath();
    qDebug() << "AppMgrClient:pid" << qApp->applicationPid();
    
    QFileInfo appinfo (qApp->applicationFilePath());
    qDebug() << "AppMgrClient:appinfo.applicationFilePath" << qApp->applicationFilePath();
    qDebug() << "AppMgrClient:appinfo.absolutePath" << appinfo.absolutePath();
    qDebug() << "AppMgrClient:appinfo.baseName" << appinfo.baseName();
    
    QString lang = QLocale::system().name();
    qDebug() << "AppMgrClient::anyTest:" << lang;

    // QString textFile = "z:/resource/qt/translations/hbserviceprovider";
    QString textFile = "hbserviceprovider";
    QFileInfo info(textFile);
    qDebug() << "AppMgrClient::base" << info.baseName();
    qDebug() << "AppMgrClient::path" << info.filePath();
    if (info.baseName() == info.filePath())
    {
        textFile = qApp->applicationFilePath().left(2) + "/resource/qt/translations/" + textFile;
        qDebug() << "AppMgrClient::path added" << textFile;
    }
    
    textFile += "_"; 
    textFile += lang;
    qDebug() << "AppMgrClient::anyTest:" << textFile;

    QTranslator translator;
    bool res = translator.load(textFile);
    qDebug() << "AppMgrClient::anyTest:" << res;
    if (res)
    {
        qApp->installTranslator(&translator);
    }
    
    QString textId = TXT_ID;
    QByteArray ba = textId.toLatin1();
    const char *textPtr = ba.data();
    
    QString text = qtTrId(textPtr);  // translate
    qDebug() << "Translated text:" << text;

    qApp->removeTranslator(&translator);

}




// Aiw request responses
void AppMgrClient::handleOk(const QVariant& result)
{
    XQAiwRequest *r = (XQAiwRequest *)sender();
    int impl=-1;
    impl = (r->descriptor().property(XQAiwInterfaceDescriptor::ImplementationId)).toInt();
    
    if (result.canConvert<QString>())
    {
        qDebug("AppMgrClient::%x:handleOk result=%s,%s",
               impl,
               result.typeName(),
               qPrintable(result.value<QString>()));
        mTextRetValue->setText(result.value<QString>());
    }
    else
    {
        qDebug("AppMgrClient::%x:handleOk result=%s",
               impl,
               result.typeName());
        mTextRetValue->setText("Not displayable");
    }
}
  
void AppMgrClient::handleError(int errorCode, const QString& errorMessage)
{
    XQAiwRequest *r = (XQAiwRequest *)sender();
    int impl=-1;
    impl = (r->descriptor().property(XQAiwInterfaceDescriptor::ImplementationId)).toInt();

    qDebug("AppMgrClient::%x:handleError code=%d, errorMessage:%s",
           impl, errorCode, qPrintable(errorMessage));
    
    mTextRetValue->setText(errorMessage);
}


