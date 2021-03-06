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

#include "xqsysinfo_p.h"
#include "xqsysinfo.h"
#include "cdeviceinfo.h"
#include <e32const.h> 
#include <sysutil.h>
#include <etel3rdparty.h> 
#include <f32file.h>
#include <featdiscovery.h> 

XQSysInfoPrivate::XQSysInfoPrivate(XQSysInfo *sysInfo) : q(sysInfo)
{
    TRAP(iError, iDeviceInfo = CDeviceInfo::NewL();)
}

XQSysInfoPrivate::~XQSysInfoPrivate()
{
    delete iDeviceInfo;
}

XQSysInfo::Language XQSysInfoPrivate::currentLanguage() const
{
    return static_cast<XQSysInfo::Language>(User::Language());
}

QString XQSysInfoPrivate::imei() const
{
    TBuf<CTelephony::KPhoneSerialNumberSize> imei(iDeviceInfo->imei());
    return QString::fromUtf16(imei.Ptr(), imei.Length());
}

QString XQSysInfoPrivate::model() const
{
    TBuf<CTelephony::KPhoneModelIdSize> model(iDeviceInfo->model());
    return QString::fromUtf16(model.Ptr(), model.Length());
}

QString XQSysInfoPrivate::manufacturer() const
{
    TBuf<CTelephony::KPhoneModelIdSize> manufacturer(
            iDeviceInfo->manufacturer());
    return QString::fromUtf16(manufacturer.Ptr(), manufacturer.Length()); 
}

QString XQSysInfoPrivate::imsi() const
{   
    TBuf<CTelephony::KIMSISize> imsi(iDeviceInfo->imsi());
    return QString::fromUtf16(imsi.Ptr(), imsi.Length()); 
}

QString XQSysInfoPrivate::softwareVersion() const
{
    QString version;
    TBuf<KSysUtilVersionTextLength> versionBuf;
    if (SysUtil::GetSWVersion(versionBuf) == KErrNone) {
        version = QString::fromUtf16(versionBuf.Ptr(), versionBuf.Length());
    }
    return version;
}

uint XQSysInfoPrivate::batteryLevel() const
{
    return iDeviceInfo->batteryLevel();
}

int XQSysInfoPrivate::signalStrength() const
{
    return iDeviceInfo->signalStrength();
}

int XQSysInfoPrivate::memory() const
{
    TMemoryInfoV1Buf info;
    UserHal::MemoryInfo(info);
    return info().iFreeRamInBytes;    
}

QString XQSysInfoPrivate::browserVersion() const
{
    return QString();
}

qlonglong XQSysInfoPrivate::diskSpace(XQSysInfo::Drive drive)
{
    RFs fsSession;
    TInt64 driveFreeSize = 0;
    TRAP(iError,
        CleanupClosePushL(fsSession);
        User::LeaveIfError(fsSession.Connect()); 
        int driveIndex = static_cast<TDriveNumber>(drive);
        
        if (fsSession.IsValidDrive(driveIndex)) {
            TDriveInfo driveInfo;
            User::LeaveIfError(fsSession.Drive(driveInfo, driveIndex));
            TVolumeInfo volumeInfo;
            User::LeaveIfError(fsSession.Volume(volumeInfo, driveIndex));
            driveFreeSize = volumeInfo.iFree;
        }
        CleanupStack::PopAndDestroy(&fsSession);
    )
    return driveFreeSize;
}

bool XQSysInfoPrivate::isDiskSpaceCritical(XQSysInfo::Drive drive) const
{
    bool isCritical = false;

    TRAP(iError,
        int driveIndex = static_cast<TDriveNumber>(drive);
        
        RFs fsSession;
        CleanupClosePushL(fsSession);
        if (fsSession.IsValidDrive(driveIndex)) {
            User::LeaveIfError(fsSession.Connect());
        
            if (SysUtil::DiskSpaceBelowCriticalLevelL(
                    &fsSession, 0, driveIndex)) {
                isCritical = true;
            } else {
                isCritical = false;
            }
        } else {
            User::Leave(KErrNotFound);
        }
        CleanupStack::PopAndDestroy(&fsSession);
    )
    return isCritical;
}

bool XQSysInfoPrivate::isSupported(int featureId)
{
    bool isFeatureSupported = false;
    TRAPD(iError, isFeatureSupported = CFeatureDiscovery::IsFeatureSupportedL(featureId);) //insp: major, error checking is not reliable
    return isFeatureSupported;
}

void XQSysInfoPrivate::SignalStatusL(TInt32 aStrength, TInt8 /*aBars*/)
{
    emit q->networkSignalChanged(aStrength);
}

void XQSysInfoPrivate::BatteryLevelL(TUint aChargeLevel, CTelephony::TBatteryStatus /*aBatteryStatus*/)
{
    emit q->batteryLevelChanged(aChargeLevel);
}

bool XQSysInfoPrivate::isNetwork() const
{
    return (signalStrength() != 0);
}

XQSysInfo::Error XQSysInfoPrivate::error() const
{
    switch (iError) {
    case KErrNone:
        return XQSysInfo::NoError;
    case KErrNoMemory:
        return XQSysInfo::OutOfMemoryError;
    case KErrNotFound:
        return XQSysInfo::DriveNotFoundError;
    default:
        return XQSysInfo::UnknownError;
    }   
}
