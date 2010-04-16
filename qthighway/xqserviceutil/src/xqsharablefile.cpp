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
#include "xqSharableFile.h"
#include <QVariant>


XQSharableFile::XQSharableFile() :
   mHandle(0)
{
    XQSERVICE_DEBUG_PRINT("XQSharableFile::XQSharableFile");
}

XQSharableFile::XQSharableFile(RFile &file)
{
    XQSERVICE_DEBUG_PRINT("XQSharableFile::XQSharableFile(RFile)");
    setHandle(file);
}

XQSharableFile::~XQSharableFile()
{
    XQSERVICE_DEBUG_PRINT("XQSharableFile::~XQSharableFile");
    // One need to call close() explicitelly when all done
}


bool XQSharableFile::setHandle(RFile &file)
{
    XQSERVICE_DEBUG_PRINT("XQSharableFile::setNativeHandle");
    
    TFileName name;
    TInt err = file.FullName(name);
    if (err != KErrNone)
    {
        XQSERVICE_DEBUG_PRINT("XQSharableFile::fullName err%d", err);
        mHandle = 0;
        return false;
    }
        
    mFileName = QString::fromUtf16(name.Ptr(), name.Length());

    mHandle = *((qlonglong *)&file);
    QString s = QString("File handle is %1").arg(mHandle);
    XQSERVICE_DEBUG_PRINT("%s", qPrintable(s));

    return true;
}

bool XQSharableFile::getHandle(RFile &handle) const
{
    XQSERVICE_DEBUG_PRINT("XQSharableFile::nativeHandle");
    if (mHandle == 0)
        return false;
    
    handle = *((RFile *)&mHandle);
    return true;
}


QString XQSharableFile::fileName() const
{
    XQSERVICE_DEBUG_PRINT("XQSharableFile::fileName");
    return mFileName;
}

bool XQSharableFile::isValid() const
{
    XQSERVICE_DEBUG_PRINT("XQSharableFile::isValid=%d", mHandle != 0);
    return mHandle != 0;
}

bool XQSharableFile::open(const QString &fileName)
{
    close();   // Close possibly existing old one
        
    TInt err = mSharableFS.Connect();
    if (err != KErrNone)
    {
        return false;
    }
    
    mSharableFS.ShareProtected();
    TPtrC name( reinterpret_cast<const TUint16*>(fileName.utf16()));
    RFile f;
    err = f.Open(mSharableFS, name, EFileShareReadersOnly);
    if (err != KErrNone)
    {
        // Error in opening, close the created session
        mSharableFS.Close();
        return false;
    }

    this->setHandle(f);
    return true;
}

void XQSharableFile::close()
{
    XQSERVICE_DEBUG_PRINT("XQSharableFile::close");
    RFile f;
    if (getHandle(f))
    {
        f.Close();
    }

    if (mSharableFS.Handle() != NULL)
    {
        mSharableFS.Close();
    }
    
}


template <typename Stream> void XQSharableFile::serialize(Stream &s) const
{
    XQSERVICE_DEBUG_PRINT("XQSharableFile::serialize");
    s << mFileName;
    s << mHandle;
    QString str = QString("\tFile handle is %1").arg(mHandle);
    XQSERVICE_DEBUG_PRINT("%s", qPrintable(str));
}

template <typename Stream> void XQSharableFile::deserialize(Stream &s)
{
    XQSERVICE_DEBUG_PRINT("XQSharableFile::deserialize");
    
    s >> mFileName;
    s >> mHandle;
    
    QString str = QString("\tFile handle is %1").arg(mHandle);
    XQSERVICE_DEBUG_PRINT("%s", qPrintable(str));
    
}

bool XQSharableFile::operator==( const XQSharableFile &other )
{
    return (mFileName == other.mFileName) && (mHandle == other.mHandle);
}


Q_IMPLEMENT_USER_METATYPE(XQSharableFile)
