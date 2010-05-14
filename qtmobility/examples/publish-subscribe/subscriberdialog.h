/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef SUBSCRIBERDIALOG_H
#define SUBSCRIBERDIALOG_H

#include <qmobilityglobal.h>
#include <QDialog>

QTM_BEGIN_NAMESPACE
class QValueSpaceSubscriber;
QTM_END_NAMESPACE

QTM_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui {
    class SubscriberDialog;
}

class QTableWidget;
class QListWidget;
QT_END_NAMESPACE

class SubscriberDialog : public QDialog
{
    Q_OBJECT

public:
    SubscriberDialog(QWidget *parent = 0);
    ~SubscriberDialog();

#ifdef QTM_EXAMPLES_SMALL_SCREEN
signals:
    void switchRequested();
#endif

protected:
    void changeEvent(QEvent *e);

//! [0]
private slots:
    void changeSubscriberPath();
    void subscriberChanged();
//! [0]

private:
    Ui::SubscriberDialog *ui;
    QValueSpaceSubscriber *subscriber;

    QTableWidget *tableWidget;
    QListWidget *listWidget;
};

#endif // SUBSCRIBERDIALOG_H