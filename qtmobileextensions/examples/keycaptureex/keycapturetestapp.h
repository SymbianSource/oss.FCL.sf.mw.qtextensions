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

#ifndef KeyCaptureTestApp_H
#define KEYCAPTUREMAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <xqkeycapture.h>

class QPlainTextEdit;
class QAction;
class QWidget;
class QMenu;
class XqKeyCapture;
class CaptureRequest;


class KeyCaptureTestApp : public QMainWindow
{
Q_OBJECT
public:
    KeyCaptureTestApp(QWidget *parent = 0);
	~KeyCaptureTestApp();
	void addTextLine(QString);
	
    bool event(QEvent *event);    
    bool eventFilter(QObject *, QEvent *);
public slots:
    void triggered(QAction* aAction);
	void cleanLog();
private:
    void procesAction(CaptureRequest request);
    void processEvent(QEvent *event);
private:	
    QPlainTextEdit *mTextArea;

    XqKeyCapture *mKeyCapture;
    
    QMap<QString, Qt::Key> mKeysMap;
    QMenu* mKeysMenu;
    
    QMap<QString, XqKeyCapture::LongFlags> mLongFlagsMap;
    QMenu* mLongFlagsMenu;
    
};


class CaptureRequest
{
public:
    CaptureRequest();
    ~CaptureRequest();
    
    enum RequestType {
        RequestTypeUndefined =-1,
        RequestTypeKey,
        RequestTypeLongKey,
        RequestTypeKeyUpAndDowns,
        RequestTypeCancelKey,
        RequestTypeCancelLongKey,
        RequestTypeCancelKeyUpAndDowns        
    };
    
    bool setType(QAction* action);
    bool setKey(QAction* action, QMap<QString, Qt::Key> *map);
    bool setLongFlags(QAction* action, QMap<QString, XqKeyCapture::LongFlags> *map);
    
    QString toString();
public:
    RequestType mRequestType;
    Qt::Key mKey;    
    Qt::KeyboardModifier mModifiersMap;
    Qt::KeyboardModifier mModifier;
    XqKeyCapture::LongFlags mLongFlags;
};
#endif // KEYCAPTUREMAINWINDOW_H
