/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef CNTSIMCONTACTSAVEREQUEST_H_
#define CNTSIMCONTACTSAVEREQUEST_H_

#include "cntabstractsimrequest.h"

#ifdef SYMBIANSIM_BACKEND_USE_ETEL_TESTSERVER
#include <mpbutil_etel_test_server.h>
#else
#include <mpbutil.h>
#endif
#include <qcontactmanager.h>

QTM_BEGIN_NAMESPACE
class QContactSaveRequest;
QTM_END_NAMESPACE
class CntSymbianSimEngine;

QTM_USE_NAMESPACE

class CntSimContactSaveRequest : public CntAbstractSimRequest
{
Q_OBJECT
public:
    CntSimContactSaveRequest(CntSymbianSimEngine *engine, QContactSaveRequest *req);
    virtual ~CntSimContactSaveRequest();
    bool start();
    bool cancel();
    
public Q_SLOTS:
    void writeComplete(QContact contact, QContactManager::Error error);
    void writeNext();    

private:
    QContactSaveRequest *m_req;
    QList<QContact> m_contacts;
    int m_index;
    QMap<int, QContactManager::Error> m_errorMap;
};

#endif // CNTSIMCONTACTSAVEREQUEST_H_
