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

#ifndef PHONESERVICE_H
#define PHONESERVICE_H

#include <QWidget>
#include <QModelIndex>
#include <QMap>
#include <QStringList>
#include <xqserviceprovider.h>
#include <xqsharablefile.h>

class QLineEdit;
class QPushButton;
class DialerService;
class UriService;
class QLabel;
class FileService;
class XQSharableFile;

class ServiceApp : public QWidget
{
    Q_OBJECT
public:
    ServiceApp( QWidget *parent = 0, Qt::WFlags f = 0 );
    ~ServiceApp();

    void setLabelNumber(QString label,QString number);

public slots:    
    void endCall();
    
public slots:
    void quit();
    void answer();
    void handleAnswerDelivered();
    
private:
    QLabel *mLabel;
    QLineEdit *mNumber;
    //QPushButton *mEndCallButton;
    DialerService* mService;
    UriService* mUriService;
    FileService* mFileService;
};

class DialerService : public XQServiceProvider
{
    Q_OBJECT
public:
    DialerService( ServiceApp *parent = 0 );
    ~DialerService();
    
    void complete(QString number);
    bool asyncAnswer() {return mAsyncAnswer;}
public slots:
    int dial(const QString& number, bool asyncAnswer);

private slots:
   void handleClientDisconnect();

private:
    ServiceApp* mServiceApp;
    QString mNumber;
    bool mAsyncAnswer;
    int mAsyncReqId;
};

class UriService : public XQServiceProvider
{
    Q_OBJECT
    public:
        UriService( ServiceApp *parent = 0 );
        ~UriService();
        bool asyncAnswer() {return mAsyncAnswer;}
        void complete(bool ok);
        
    public slots:
        bool view(const QString& uri);
        bool view(const QString& uri, bool returnValue);
        
    private slots:
        void handleClientDisconnect();

    private:
        ServiceApp* mServiceApp;
        bool mAsyncAnswer;
        int mAsyncReqId;
        bool mRetValue;
};

class FileService : public XQServiceProvider
{
    Q_OBJECT
    public:
        FileService( ServiceApp *parent = 0 );
        ~FileService();
        bool asyncAnswer() {return mAsyncAnswer;}
        void complete(bool ok);

    public slots:
        bool view(QString file);
        bool view(XQSharableFile file);
        
    private slots:
        void handleClientDisconnect();

    private:
        ServiceApp* mServiceApp;
        bool mAsyncAnswer;
        int mAsyncReqId;
        bool mRetValue;
};


#endif
