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

#include <QtTest>
#include <QtCore/qmetaobject.h>

#include "xqserviceadaptor.h"
#include "xqservicechannel.h"
#include "qslotinvoker.h"
#include "qsignalintercepter.h"

class ut_XQServiceAdaptor : public QObject
{
    Q_OBJECT
public:
    ut_XQServiceAdaptor() {}
    ~ut_XQServiceAdaptor() {}

private slots:
    void initTestCase();

    void create();

    void messageToSignal_data();
    void messageToSignal();

    void signalToMessage_data();
    void signalToMessage();

public slots:
    void signalSeen();
    void receive(const QString& msg, const QByteArray& data);

signals:
    void received();
    void sig_message1();
    void sig_message2(QString);

private:
    QEventLoop *eventLoop;
    bool signalAlreadySeen;
    QString lastMsg;
    QByteArray lastData;
    int count;

    void expectSignal(QObject *object, const char *signal);
    void disconnectSignal(QObject *object, const char *signal);
    bool waitForSignal(int timeout = 1000);
    static QByteArray argsToData(const QString& msg, const QList<QVariant>& args);
};

void ut_XQServiceAdaptor::initTestCase()
{
    eventLoop = 0;
    signalAlreadySeen = false;
    count = 0;
}

void ut_XQServiceAdaptor::create()
{
    // Create an adaptor and check that the channel name is returned correctly.
    XQServiceAdaptor *adaptor = new XQServiceAdaptor(QLatin1String("com.nokia.test.Receiver"));
    QCOMPARE(adaptor->channel(), QLatin1String("com.nokia.test.Receiver"));

    // Hook a message to a local signal, and then test that it is delivered.
    // This tests that the adaptor is basically sane.
    XQServiceAdaptor::connect(adaptor, MESSAGE(foobar()), this, SIGNAL(received()));
    expectSignal(this, SIGNAL(received()));
    QByteArray data;
    QVariant retValue;
    XQServiceChannel::send(QLatin1String("com.nokia.test.Receiver"), QLatin1String("foobar()"), data,retValue,false); //async
    QVERIFY(waitForSignal());
    disconnectSignal(this, SIGNAL(received()));

    // Clean up.
    delete adaptor;
}

void ut_XQServiceAdaptor::messageToSignal_data()
{
    QTest::addColumn<QString>("chan");
    QTest::addColumn<QString>("msg");
    QTest::addColumn< QList<QVariant> >("params");

    QTest::newRow("simple")
        << "com.nokia.test.Receiver" << "message1()"
        << QList<QVariant>();

    QTest::newRow("singlearg")
        << "com.nokia.test.Receiver" << "message2(QString)"
        << (QList<QVariant>() << QString::fromLatin1("foo"));
}

// Test the conversion of XQService messages into signals.
void ut_XQServiceAdaptor::messageToSignal()
{
    QFETCH(QString, chan);
    QFETCH(QString, msg);
    QFETCH(QList<QVariant>, params);

    // Construct the equivalent of MESSAGE(msg).
    QByteArray messageName(1, QMESSAGE_CODE + '0');
    messageName += msg.toLatin1();

    // Construct the equivalent of SIGNAL("sig_" msg).
    QByteArray signalName(1, QSIGNAL_CODE + '0');
    signalName += "sig_";
    signalName += msg.toLatin1();

    // Construct the adaptor and hook up the message.
    XQServiceAdaptor *adaptor = new XQServiceAdaptor(chan);
    XQServiceAdaptor::connect(adaptor, messageName, this, signalName);

    // Send a raw XQService message and wait for the signal.
    expectSignal(this, signalName.constData());
    QVariant retValue;
    XQServiceChannel::send(chan, msg, argsToData(msg, params),retValue,false); //async
    QVERIFY(waitForSignal());
    disconnectSignal(this, signalName.constData());

    // Clean up.
    delete adaptor;
}

void ut_XQServiceAdaptor::signalToMessage_data()
{
    messageToSignal_data();
}

// Test the conversion of signals into XQService messages.
void ut_XQServiceAdaptor::signalToMessage()
{
    QFETCH(QString, chan);
    QFETCH(QString, msg);
    QFETCH(QList<QVariant>, params);

    // Construct the equivalent of MESSAGE(msg).
    QByteArray messageName(1, QMESSAGE_CODE + '0');
    messageName += msg.toLatin1();

    // Construct the equivalent of SIGNAL("sig_" msg).
    QByteArray signalName(1, QSIGNAL_CODE + '0');
    signalName += "sig_";
    signalName += msg.toLatin1();

    // Construct the adaptor and hook up the message.
    XQServiceAdaptor *adaptor = new XQServiceAdaptor(chan);
    XQServiceAdaptor::connect(this, signalName, adaptor, messageName);

    // Consruct a XQService channel object to receive the message.
    XQServiceChannel *channel = new XQServiceChannel(chan,true);
    QVERIFY(channel->connectChannel());
    
    connect(channel, SIGNAL(received(QString,QByteArray)),
            this, SLOT(receive(QString,QByteArray)));

    // Emit the signal using QSlotInvoker and wait for the message.
    count = 0;
    QSlotInvoker *invoker = new QSlotInvoker(this, signalName.constData());
    expectSignal(this, SIGNAL(received()));
    invoker->invoke(params);
    QVERIFY(waitForSignal());
    QCOMPARE(count, 1);
    QCOMPARE(lastMsg, msg);
    QCOMPARE(lastData, argsToData(msg, params));
    disconnectSignal(this, SIGNAL(received()));

    // Clean up.
    delete invoker;

    delete adaptor;
    delete channel;
}

void ut_XQServiceAdaptor::expectSignal(QObject *object, const char *signal)
{
    signalAlreadySeen = false;
    connect(object, signal, this, SLOT(signalSeen()));
}

void ut_XQServiceAdaptor::disconnectSignal(QObject *object, const char *signal)
{
    disconnect(object, signal, this, SLOT(signalSeen()));
}

bool ut_XQServiceAdaptor::waitForSignal(int timeout)
{
    if (signalAlreadySeen)
        return true;
    QEventLoop loop;
    eventLoop = &loop;
    QTimer::singleShot(timeout, eventLoop, SLOT(quit()));
    loop.exec();
    eventLoop = 0;
    return signalAlreadySeen;
}

void ut_XQServiceAdaptor::signalSeen()
{
    signalAlreadySeen = true;
    if (eventLoop)
        eventLoop->quit();
}

void ut_XQServiceAdaptor::receive(const QString& msg, const QByteArray& data)
{
    lastMsg = msg;
    lastData = data;
    ++count;
    emit received();
}

class ExtendedVariant : public QVariant
{
public:
    ExtendedVariant() : QVariant() {}
    explicit ExtendedVariant(const QVariant& value)
        : QVariant(value) {}

    void load(QDataStream& stream, int typeOrMetaType)
    {
        clear();
        create(typeOrMetaType, 0);
        d.is_null = false;
        QMetaType::load(stream, d.type, const_cast<void *>(constData()));
    }

    void save(QDataStream& stream) const
    {
        QMetaType::save(stream, d.type, constData());
    }
};

QByteArray ut_XQServiceAdaptor::argsToData(const QString& msg, const QList<QVariant>& args)
{
    QByteArray array;
    {
        QDataStream stream
            (&array, QIODevice::WriteOnly | QIODevice::Append);
        QList<QVariant>::ConstIterator iter;
        if (!msg.contains(QLatin1String("QVariant"))) {
            for (iter = args.begin(); iter != args.end(); ++iter) {
                ExtendedVariant copy(*iter);
                copy.save(stream);
            }
        } else {
            QByteArray name = msg.toLatin1();
            name = QMetaObject::normalizedSignature(name.constData());
            int numParams = 0;
            int *params = QSignalIntercepter::connectionTypes
                    (name, numParams);
            int index = 0;
            for (iter = args.begin(); iter != args.end(); ++iter, ++index) {
                if (index < numParams &&
                     params[index] == QSignalIntercepter::QVariantId) {
                    // We need to handle QVariant specially because we actually
                    // need the type header in this case.
                    stream << *iter;
                } else {
                    ExtendedVariant copy(*iter);
                    copy.save(stream);
                }
            }
            if (params)
                qFree(params);
        }
        // Stream is flushed and closed at this point.
    }
    return array;
}

QTEST_MAIN(ut_XQServiceAdaptor)

#include "ut_xqserviceadaptor.moc"
