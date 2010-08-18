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

#ifndef MYKEY_H_
#define MYKEY_H_

#include <QPushButton>
#include <QPlainTextEdit>
#include <QEvent>
#include <QKeyEvent>
#include <QMap>

class MyKeyButton : public QPushButton 
{
    Q_OBJECT
    
public:
    MyKeyButton(QPlainTextEdit *logger, QWidget *parent = 0)
    :
    QPushButton(QString("dummy"), parent)
    {
        justLogger = logger;
        keyLabels.insert(Qt::Key_VolumeUp, "Volume Up");        
        keyLabels.insert(Qt::Key_VolumeDown, "Volume Down");        
    }
    
    ~MyKeyButton() 
    {
    }
    
    bool event(QEvent *event)
    {
        if (justLogger) {
            if (event->type() != QEvent::KeyPress) {
//                QString keyName = mapNaturalName((dynamic_cast<QKeyEvent*>event)->key());
//                justLogger->appendPlainText(QString("P> %1").arg(keyName));
                justLogger->appendPlainText(QString("P> %1").arg(static_cast<QKeyEvent*>(event)->key()));
            }
            if (event->type() != QEvent::KeyRelease) {
//                QString keyName = mapNaturalName((dynamic_cast<QKeyEvent*>event)->key());
//                justLogger->appendPlainText(QString("r> %1").arg(keyName));
                justLogger->appendPlainText(QString("R> %1").arg(static_cast<QKeyEvent*>(event)->key()));
            }
        }
        return QPushButton::event(event);
    }
    
private:
    
    QString mapNaturalName(Qt::Key key) {
        QString name = keyLabels[key];
        if (name.isEmpty()) {
            name = QString("0x%1").arg(static_cast<int>(key), 0, 16);
        }
        return name;
    }
    
private:
    // not own
    QPlainTextEdit *justLogger;
    QMap<Qt::Key, QString> keyLabels;
};


#endif /* MYKEY_H_ */
