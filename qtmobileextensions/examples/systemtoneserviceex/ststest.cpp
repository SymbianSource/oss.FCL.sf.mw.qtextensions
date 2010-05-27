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
#include <QApplication>
#include <QKeyEvent>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
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
#include <QMap>
#include <QAction>

#include "ststest.h"
#include <XQSystemToneService>

STSTest::STSTest( QWidget *parent, Qt::WFlags f ) 
:
QWidget(parent, f),
sts(new XQSystemToneService())
{
    callbackCleanTimer.setSingleShot(true);
    connect(&callbackCleanTimer, SIGNAL(timeout()), this, SLOT(cleanCallback()));
    
    QPushButton *quitButton = new QPushButton(tr("QUIT"));
    connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

    QPushButton *playToneButton = new QPushButton(tr("Play Tone"));
    connect(playToneButton, SIGNAL(clicked()), this, SLOT(playTone()));

    QPushButton *playAlarmButton = new QPushButton(tr("Play Alarm"));
    connect(playAlarmButton, SIGNAL(clicked()), this, SLOT(playAlarm()));

    QPushButton *stopAlarmButton = new QPushButton(tr("Stop Alarm"));
    connect(stopAlarmButton, SIGNAL(clicked()), this, SLOT(stopAlarm()));
    

    connect(sts, SIGNAL(toneStarted()), this, SLOT(startCallback()));
    connect(sts, SIGNAL(alarmStarted(unsigned int)), this, SLOT(startCallback(unsigned int)));
    connect(sts, SIGNAL(alarmFinished(unsigned int)), this, SLOT(stopCallback(unsigned int)));


    box = new QComboBox();
    spinBox = new QSpinBox();
    
    QVBoxLayout *vl = new QVBoxLayout;
    QGridLayout *gl = new QGridLayout();
    
    vl->setMargin(10);
    vl->setSpacing(0);

//    gl->setMargin(0);
//    gl->setSpacing(0);
//    gl->setDefaultPositioning();

    
    QList<QPair<int, QString> > tonesList;
    

    // alarms
    tonesList.append(QPair<int,QString>(XQSystemToneService::CalendarAlarmTone, "[A] Calendar Alarm"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::ClockAlarmTone, "[A] Clock Alarm"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::ToDoAlarmTone, "[A] ToDo Alarm"));

    tonesList.append(QPair<int,QString>(XQSystemToneService::IncomingCallTone, "[A] Incoming Call"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::IncomingCallLine2Tone, "[A] Incomming Call Line 2"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::IncomingDataCallTone, "[A] Incomming Data Call"));

    // tones
    tonesList.append(QPair<int,QString>(XQSystemToneService::BurstModeTone, "[T] Burst Mode"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::CaptureTone, "[T] Capture"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::CallRecordingTone, "[T] Call Recording"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::RecordingStartTone, "[T] Recording Start"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::SelfTimerTone, "[T] Self Timer"));

    tonesList.append(QPair<int,QString>(XQSystemToneService::ConfirmationBeepTone, "[T] Confirmation Beep"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::DefaultBeepTone, "[T] Default Beep"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::ErrorBeepTone, "[T] Error Beep"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::InformationBeepTone, "[T] Information Beep"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::WarningBeepTone, "[T] Warning Beep"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::IntegratedHandsFreeActivatedTone, "[T] Integrated Hands Free Act."));

    tonesList.append(QPair<int,QString>(XQSystemToneService::TouchScreenTone, "[T] Touch Screen"));

    tonesList.append(QPair<int,QString>(XQSystemToneService::ChatAlertTone, "[T] Chat Alert"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::EmailAlertTone, "[T] Email Alert"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::MmsAlertTone, "[T] MMS Alert"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::SmsAlertTone, "[T] SMS Alert"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::DeliveryReportTone, "[T] Delivery Report"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::MessageSendFailureTone, "[T] Message Send Failure"));

    tonesList.append(QPair<int,QString>(XQSystemToneService::BatteryLowTone, "[T] Battery Low"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::BatteryRechargedTone, "[T] Battery Recharge"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::PowerOnTone, "[T] Power On"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::PowerOffTone, "[T] Power Off"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::WakeUpTone, "[T] Wake Up"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::WrongChargerTone, "[T] Wrong Charger"));

    tonesList.append(QPair<int,QString>(XQSystemToneService::AutomaticRedialCompleteTone, "[T] Automatic Redial Compl."));

    tonesList.append(QPair<int,QString>(XQSystemToneService::VoiceStartTone, "[T] Voice Start"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::VoiceErrorTone, "[T] Voice Error"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::VoiceAbortTone, "[T] Voice Abort"));

    vl->addLayout(gl);

    QListIterator<QPair<int, QString> > iter(tonesList);

    gl->setDefaultPositioning(4, Qt::Horizontal);
    
    while (iter.hasNext()) {
        QPair<int, QString> item = iter.next();
        box->addItem(item.second, item.first);
    }

    vl->addStretch(4);
    vl->addWidget(box);

    vl->addStretch(1);
    vl->addWidget(playToneButton);
    vl->addStretch(1);
    vl->addWidget(playAlarmButton);
    vl->addStretch(1);
    vl->addWidget(currCtxLabel = new QLabel("Last context: ?"));
    vl->addStretch(2);
    vl->addWidget(stopAlarmButton);
    vl->addStretch(1);
    vl->addWidget(new QLabel("Context:"));
    vl->addWidget(spinBox);

    vl->addStretch(1);
    vl->addWidget(callbackLabel = new QLabel("SIGNALS: ---"));
    
    vl->addStretch(4);
    vl->addWidget(quitButton);

    setLayout(vl);
    //showMaximized();
    showFullScreen();
}

STSTest::~STSTest()
{
    delete sts;
}

void STSTest::playTone()
{
    int id = box->itemData(box->currentIndex(), Qt::UserRole).toInt();
    
    qDebug("Beep!!! (%d)", id);
    
    sts->playTone((XQSystemToneService::ToneType)id);
    
    currCtxLabel->setText(QString("Last context: -"));
}

void STSTest::playAlarm()
{
    int id = box->itemData(box->currentIndex(), Qt::UserRole).toInt();
    
    qDebug("Beep!!! (%d)", id);
    unsigned int ctx = 0;
    
    sts->playAlarm((XQSystemToneService::AlarmType)id, ctx);
    
    currCtxLabel->setText(QString("Last context: %1").arg(ctx));
}

void STSTest::stopAlarm()
{
    int ctx = spinBox->value();

    qDebug("Silence! (%d)", ctx);
    sts->stopAlarm(ctx);
}

void STSTest::startCallback()
{
    callbackLabel->setText(QString("SIGNALS: playing tone started..."));
    callbackCleanTimer.start(1000);
}

void STSTest::startCallback(unsigned int ctx)
{
    callbackLabel->setText(QString("SIGNALS: (%1) playing alarm started...").arg(ctx));
    callbackCleanTimer.start(1000);
}

void STSTest::stopCallback(unsigned int ctx)
{
    callbackLabel->setText(QString("SIGNALS: (%1) playing alarm finished...").arg(ctx));
    callbackCleanTimer.start(1000);
}

void STSTest::cleanCallback()
{
    callbackLabel->setText("SIGNALS: ---");
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    STSTest* tester = new STSTest();
    int rv = a.exec();    
    delete tester;
	return rv;    
}
    
