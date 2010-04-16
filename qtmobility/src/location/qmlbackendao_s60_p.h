/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef QMLBACKENDAO_H
#define QMLBACKENDAO_H

#include <e32base.h>    // For CActive, link against: euser.lib
#include <lbs.h>
#include <lbscommon.h>
#include <lbssatellite.h>
#include "qgeopositioninfosource.h"
//#include "qgeopositioninfosources60.h"

QTM_BEGIN_NAMESPACE

class CQGeoPositionInfoSourceS60 ;
class CQGeoSatelliteInfoSourceS60;

// Types of request
enum RequestType {
    RegularUpdate = 1,  // Regular updates
    OnceUpdate,     // request update
    DeviceStatus       // device updates
};

//
class CQMLBackendAO : public CActive
{
public:
    // Cancel and destroy
    ~CQMLBackendAO();

    // Two-phased constructor.
    static CQMLBackendAO* NewL(QObject *aRequester,
                               RequestType  aRequestType, TPositionModuleId  aModId = TUid::Uid(0)
                              );

    // Two-phased constructor.
    static CQMLBackendAO* NewLC(QObject *aRequester,
                                RequestType  aRequestType, TPositionModuleId  aModID);

    // checks any pending request in activeobject
    bool isRequestPending();

    // Async call to get notifications about device status.
    void notifyDeviceStatus(TPositionModuleStatusEventBase &aStatusEvent) ;

    // requesting for position update once
    void requestUpdate(int aTimeout);

    // cancels an outstanding update request
    void cancelUpdate();

    // Sets an interval in millisecs for regular notifications
    int setUpdateInterval(int aMilliSec);

    void startUpdates();



private:
    // C++ constructor
    CQMLBackendAO();

    // Second-phase constructor
    TInt ConstructL(QObject *aRequester, RequestType  aRequestType,
                    TPositionModuleId  aModId);

    // Device Notifications are handled
    void handleDeviceNotification(int aError);

    // regular position notifications are handled
    void handlePosUpdateNotification(int aError);

    void CancelAll();

    bool initializePosInfo();

private:
    // From CActive
    // Handle completion
    void RunL();

    // How to cancel me
    void DoCancel();

    // Override to handle leaves from RunL(). Default implementation causes
    // the active scheduler to panic.
    int RunError(int aError);

private:

    // Request is a device or a regular
    CQGeoPositionInfoSourceS60 *mRequester;

    //Request is a device for Satellite update only
    CQGeoSatelliteInfoSourceS60 *mRequesterSatellite;

    // Request type once, regular, device
    RequestType  mRequestType;

    // Positioning Information
    RPositioner mPositioner;

    HPositionGenericInfo *mPosInfo;

    TPositionSatelliteInfo mPosSatInfo;
};

QTM_END_NAMESPACE

#endif // QMLBACKENDAO_H
