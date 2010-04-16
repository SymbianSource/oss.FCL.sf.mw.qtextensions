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

#include "xqservicelog.h"

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
#include <QListView>
#include <QMessageBox>
#include <xqsharablefile.h>

#include "serviceapp.h"
#include <xqserviceutil.h>

ServiceApp::ServiceApp(QWidget *parent, Qt::WFlags f)
    : QWidget(parent, f),
      mService(NULL),
      mUriService(NULL),
      mFileService(NULL)
{
    XQSERVICE_DEBUG_PRINT("ServiceApp::ServiceApp");
    if (XQServiceUtil::isService())
    {
        mService = new DialerService(this);
        mUriService = new UriService(this);
        mFileService = new FileService(this);
    }
    /* Adjust the palette */
#if defined(Q_WS_S60)
    QPalette p = qApp->palette();
    QColor color(192,192,192);
    QColor bg(201,250,250);
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

    QPushButton *quitButton = new QPushButton(tr("Quit"));
    QPushButton *answerButton = new QPushButton(tr("Async Answer"));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(quit()));
    connect(answerButton, SIGNAL(clicked()), this, SLOT(answer()));

    /*
    mEndCallButton = new QPushButton(tr("End Call"));
    mEndCallButton->setEnabled(false);
    connect(mEndCallButton, SIGNAL(clicked()), this, SLOT(endCall()));
    */
    bool isService = XQServiceUtil::isService();
    
    QString t = "SERVICEAPP:\n";
    t = t + (isService ?  "    LAUNCHED AS SERVICE\n" : "    LAUNCHED NORMALLY\n");
    t = t + (XQServiceUtil::isEmbedded() ? "    EMBEDDED\n" : "    NOT EMBEDDED\n");
    
    QStringList args = QApplication::arguments();
    foreach (QString arg, args)
    {
        t += "cmdline arg=" + arg + "\n";
    }
    
    QLabel *title = new QLabel(t);

    mLabel = new QLabel("");
    mNumber = new QLineEdit("");
    
    QVBoxLayout *vl = new QVBoxLayout;
    vl->setMargin(0);
    vl->setSpacing(0);

    vl->addWidget(answerButton);
    vl->addWidget(quitButton);
    vl->addWidget(title);
    vl->addWidget(mLabel);
    vl->addWidget(mNumber);

    setLayout(vl);
   
#if defined(Q_WS_X11) || defined(Q_WS_WIN)
    setFixedSize(QSize(360,640)); // nHD
#elif defined(Q_WS_S60)
    // showMaximized();
    showFullScreen();
#endif
//    new DialerService(this);
}


ServiceApp::~ServiceApp()
{
    XQSERVICE_DEBUG_PRINT("ServiceApp::~ServiceApp");
    delete mService;
    delete mUriService;
    delete mFileService;
}

void ServiceApp::quit()
{
    XQSERVICE_DEBUG_PRINT("ServiceApp::quit");
    qApp->quit();
}

void ServiceApp::answer()
{
    XQSERVICE_DEBUG_PRINT("ServiceApp::answer");
    if (mService && mService->asyncAnswer())
    {
        // connect(mService, SIGNAL(returnValueDelivered()), this, SLOT(handleAnswerDelivered()));
        mService->complete(mNumber->text());
    }
    if (mUriService && mUriService->asyncAnswer())
    {
        connect(mUriService, SIGNAL(returnValueDelivered()), this, SLOT(handleAnswerDelivered()));
        mUriService->complete(true);
    }
    if (mFileService && mFileService->asyncAnswer())
    {
        connect(mUriService, SIGNAL(returnValueDelivered()), this, SLOT(handleAnswerDelivered()));
        mUriService->complete(true);
    }
}

void ServiceApp::handleAnswerDelivered()
{
    XQSERVICE_DEBUG_PRINT("ServiceApp::handleAnswerDelivered");
    quit();
    
}


void ServiceApp::endCall()
{
    XQSERVICE_DEBUG_PRINT("ServiceApp::endCall");
    //QVBoxLayout *vl = qobject_cast<QVBoxLayout *>(layout()) ;
    //vl->removeWidget(mEndCallButton);

    //XQServiceUtil::toBackground(true);
}

void ServiceApp::setLabelNumber(QString label,QString number)
{
    XQSERVICE_DEBUG_PRINT("ServiceApp::setLabelNumber");
    mLabel->setText(label);
    mNumber->setText(number);
}

// ----------DialerService---------------

DialerService::DialerService(ServiceApp* parent)
: XQServiceProvider(QLatin1String("com.nokia.services.serviceapp.Dialer"),parent),
    mServiceApp(parent),
    mAsyncReqId(-1),
    mAsyncAnswer(false)
{
    XQSERVICE_DEBUG_PRINT("DialerService::DialerService");
    publishAll();
    connect(this, SIGNAL(returnValueDelivered()), parent, SLOT(handleAnswerDelivered()));
}

DialerService::~DialerService()
{
    XQSERVICE_DEBUG_PRINT("DialerService::~DialerService");
}

void DialerService::complete(QString number)
{
    if (mAsyncReqId == -1)
        return;
    XQSERVICE_DEBUG_PRINT("DialerService::complete");
    completeRequest(mAsyncReqId,number.toInt());
}

int DialerService::dial(const QString& number, bool asyncAnswer)
{
    XQSERVICE_DEBUG_PRINT("DialerService::dial: %s,%d", qPrintable(number), asyncAnswer);
    XQRequestInfo info = requestInfo();
    
    XQSERVICE_DEBUG_PRINT("\tRequest info: sid=%X,vid=%X", info.clientSecureId(), info.clientVendorId());
    QSet<int> caps = info.clientCapabilities();
    QSetIterator<int> i(caps);
    while (i.hasNext())
        qDebug() << "Has capability " << i.next();    
    XQSERVICE_DEBUG_PRINT("\tRequest info: embed=%d,sync=%d", info.isEmbedded(), info.isSynchronous());
    
    QString label = "Dialer::dial:\n";
    label += QString("number=%1\n").arg(number);
    label += QString("asyncAnswer=%1\n").arg(asyncAnswer);
    
    mAsyncAnswer = asyncAnswer; 
    mNumber = number ;
    mServiceApp->setLabelNumber(label, number);
    int ret = 0;
    if (mAsyncAnswer)
    {
        mAsyncReqId = setCurrentRequestAsync();
        connect(this, SIGNAL(clientDisconnected()), this, SLOT(handleClientDisconnect()));
    }
    else
    {
        ret = number.toInt();
    }
    return ret;
}

void DialerService::handleClientDisconnect()
{
    XQSERVICE_DEBUG_PRINT("DialerService::handleClientDisconnect");
    // Just quit service application if client ends
    mAsyncAnswer = false;
    mServiceApp->quit();
}


// ----------UriService---------------

UriService::UriService(ServiceApp* parent)
: XQServiceProvider(QLatin1String("com.nokia.services.serviceapp.com.nokia.symbian.IUriView"),parent),
    mServiceApp(parent),
    mAsyncReqId(-1),
    mAsyncAnswer(false)
    
{
    XQSERVICE_DEBUG_PRINT("UriService::UriService");
    publishAll();
}

UriService::~UriService()
{
    XQSERVICE_DEBUG_PRINT("UriService::~UriService");
}

void UriService::complete(bool ok)
{
    if (mAsyncReqId == -1)
        return;
    XQSERVICE_DEBUG_PRINT("UriService::complete");
    completeRequest(mAsyncReqId, QVariant(mRetValue));
}

bool UriService::view(const QString& uri)
{
    XQSERVICE_DEBUG_PRINT("UriService::view(1)");
    return view(uri, true);
}

bool UriService::view(const QString& uri, bool retValue)
{
    XQSERVICE_DEBUG_PRINT("UriService::view(2)");
    QString label = "IUri::view\n:";
    label += QString ("Uri=%1\n").arg(uri);
    label += QString ("retValue=%1\n").arg(retValue);
    QString param = QString ("retValue=%1\n").arg(retValue);

    mAsyncAnswer = !XQServiceUtil::isEmbedded();;
    mRetValue = retValue;
    mServiceApp->setLabelNumber(label,param);
    if (mAsyncAnswer)
    {
        mAsyncReqId = setCurrentRequestAsync();
        connect(this, SIGNAL(clientDisconnected()), this, SLOT(handleClientDisconnect()));
    }
    
    return retValue;
}

void UriService::handleClientDisconnect()
{
    XQSERVICE_DEBUG_PRINT("UriService::handleClientDisconnect");

    // Just quit application
    mAsyncAnswer = false;
    mServiceApp->quit();
}


// ----------FileService---------------

FileService::FileService(ServiceApp* parent)
: XQServiceProvider(QLatin1String("com.nokia.services.serviceapp.com.nokia.symbian.IFileView"),parent),
    mServiceApp(parent),
    mAsyncReqId(-1),
    mAsyncAnswer(false)

{
    XQSERVICE_DEBUG_PRINT("FileService::FileService");
    publishAll();
}

FileService::~FileService()
{
    XQSERVICE_DEBUG_PRINT("FileService::~FileService");
}

void FileService::complete(bool ok)
{
    if (mAsyncReqId == -1)
        return;
    XQSERVICE_DEBUG_PRINT("FileService::complete");
    completeRequest(mAsyncReqId, QVariant(ok));
}

bool FileService::view(QString file)
{
    XQSERVICE_DEBUG_PRINT("FileService::view(QString)");
    QString label = "IFile::view\n:";
    QString param = QString ("File=%1\n").arg(file);

    mAsyncAnswer = !XQServiceUtil::isEmbedded();;
    mServiceApp->setLabelNumber(label,param);
    if (mAsyncAnswer)
    {
        mAsyncReqId = setCurrentRequestAsync();
        connect(this, SIGNAL(clientDisconnected()), this, SLOT(handleClientDisconnect()));
    }
    
    return true;
}


bool FileService::view(XQSharableFile sf)
{
    XQSERVICE_DEBUG_PRINT("FileService::view(XQSharebleFile)");
    QString label = "IFile::view\n:";
    QString param = QString ("File=%1\n").arg(sf.fileName());

    RFile file;
    bool ok = sf.getHandle( file );
    if (ok)
    {
        HBufC8* data = HBufC8::NewL(100);
        TPtr8 ptr = data->Des();
        TInt err  = file.Read( ptr );
        QString text = QString::fromUtf8((const char *)(data->Ptr()), data->Length());
        XQSERVICE_DEBUG_PRINT("FileService::file read,%d,%s", err, qPrintable(text));
        sf.close();
        delete data;
    }

    mAsyncAnswer = !XQServiceUtil::isEmbedded();;
    mServiceApp->setLabelNumber(label,param);
    if (mAsyncAnswer)
    {
        mAsyncReqId = setCurrentRequestAsync();
        connect(this, SIGNAL(clientDisconnected()), this, SLOT(handleClientDisconnect()));
    }
    return true;
}


void FileService::handleClientDisconnect()
{
    XQSERVICE_DEBUG_PRINT("FileService::handleClientDisconnect");

    // Just quit application
    mAsyncAnswer = false;
    mServiceApp->quit();
}

