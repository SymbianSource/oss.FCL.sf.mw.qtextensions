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

#include "xqcentralrepositoryutils_p.h"
#include "xqsettingsmanager.h"
#include "xqsettingsmanager_p.h"
#include "ccentralrepositoryhandler.h"
#include "xqcentralrepositorysearchcriteria.h"

XQCentralRepositoryUtilsPrivate::XQCentralRepositoryUtilsPrivate(XQSettingsManager& settingsManager)
    : m_settingsManagerPrivate(*settingsManager.d)
{
}

XQCentralRepositoryUtilsPrivate::~XQCentralRepositoryUtilsPrivate()
{
}

#ifdef XQSETTINGSMANAGER_NO_CENREPKEY_CREATION_DELETION
bool XQCentralRepositoryUtilsPrivate::createKey(const XQCentralRepositorySettingsKey& /*key*/, const QVariant& /*value*/)
{
    m_settingsManagerPrivate.iError = KErrNotSupported;
    return false;
}
#else    
bool XQCentralRepositoryUtilsPrivate::createKey(const XQCentralRepositorySettingsKey& key, const QVariant& value)
{
    CCentralRepositoryHandler* handler = m_settingsManagerPrivate.cenRepHandlerInstance(key.uid());
    
    if (handler)
    {
        switch (value.type())
        {
            case QVariant::Int:
            {
                m_settingsManagerPrivate.iError = handler->createKey(key.key(), value.toInt());
                break;
            }
            case QVariant::Double:
            {
                m_settingsManagerPrivate.iError = handler->createKey(key.key(), value.toDouble());
                break;
            }
            case QVariant::String:
            {
                m_settingsManagerPrivate.iError = handler->createKey(key.key(), TPtrC16(reinterpret_cast<const TUint16*>(value.toString().utf16())));
                break;
            }
            case QVariant::ByteArray:
            {
                m_settingsManagerPrivate.iError = handler->createKey(key.key(), TPtrC8((TUint8*)(value.toByteArray().constData())));
                break;
            }
            default:
            {
                m_settingsManagerPrivate.iError = KErrArgument;
                break;
            }
        }
    }
    else
    {
        m_settingsManagerPrivate.iError = XQSettingsManager::UnknownError;
    }
    return m_settingsManagerPrivate.iError == KErrNone;
}
#endif

#ifdef XQSETTINGSMANAGER_NO_CENREPKEY_CREATION_DELETION
bool XQCentralRepositoryUtilsPrivate::deleteKey(const XQCentralRepositorySettingsKey& /*key*/)
{
    m_settingsManagerPrivate.iError = KErrNotSupported;
    return false;
}
#else
bool XQCentralRepositoryUtilsPrivate::deleteKey(const XQCentralRepositorySettingsKey& key)
{
    CCentralRepositoryHandler* handler = m_settingsManagerPrivate.cenRepHandlerInstance(key.uid());
    
    if (handler)
    {
        m_settingsManagerPrivate.iError = handler->deleteKey(key.key());
    }
    else
    {
        m_settingsManagerPrivate.iError = XQSettingsManager::UnknownError;
    }
    return m_settingsManagerPrivate.iError == KErrNone;
}
#endif

bool XQCentralRepositoryUtilsPrivate::resetKey(const XQCentralRepositorySettingsKey& key)
{
    CCentralRepositoryHandler* handler = m_settingsManagerPrivate.cenRepHandlerInstance(key.uid());
    
    if (handler)
    {
        m_settingsManagerPrivate.iError = handler->resetKey(key.key());
    }
    else
    {
        m_settingsManagerPrivate.iError = XQSettingsManager::UnknownError;
    }
    return m_settingsManagerPrivate.iError == KErrNone;
}

bool XQCentralRepositoryUtilsPrivate::resetRepository(long int repositoryUid)
{
    CCentralRepositoryHandler* handler = m_settingsManagerPrivate.cenRepHandlerInstance(repositoryUid);
    
    if (handler)
    {
        m_settingsManagerPrivate.iError = handler->resetRepository();
    }
    else
    {
        m_settingsManagerPrivate.iError = XQSettingsManager::UnknownError;
    }
    return m_settingsManagerPrivate.iError == KErrNone;
}

QList<XQCentralRepositorySettingsKey> XQCentralRepositoryUtilsPrivate::findKeys(const XQCentralRepositorySearchCriteria& criteria)
{
    long int repositoryUid = criteria.repositoryUid();
    CCentralRepositoryHandler* handler = m_settingsManagerPrivate.cenRepHandlerInstance(repositoryUid);
    if (!handler)
    {
        m_settingsManagerPrivate.iError = XQSettingsManager::UnknownError;
        return QList<XQCentralRepositorySettingsKey>();
    }

    QList<XQCentralRepositorySettingsKey> keys;

    TRAP(m_settingsManagerPrivate.iError,
        RArray<TUint32> foundKeys;
        CleanupClosePushL(foundKeys);
        if (criteria.value().isNull())
        {
            User::LeaveIfError(handler->findKeyL(criteria.partialKey(), criteria.bitMask(),
                foundKeys));
        }
        else
        {
            switch (criteria.value().type())
            {
                case QVariant::Int:
                {
                    User::LeaveIfError(handler->findKeyL(criteria.partialKey(), criteria.bitMask(),
                        criteria.value().toInt(), criteria.negativeCriteria(), foundKeys));
                    break;
                }
                case QVariant::Double:
                {
                    User::LeaveIfError(handler->findKeyL(criteria.partialKey(), criteria.bitMask(),
                        criteria.value().toDouble(), criteria.negativeCriteria(), foundKeys));
                    break;
                }
                case QVariant::String:
                {
                    TPtrC16 stringValue(reinterpret_cast<const TUint16*>(criteria.value().toString().utf16()));
                    User::LeaveIfError(handler->findKeyL(criteria.partialKey(), criteria.bitMask(),
                        stringValue, criteria.negativeCriteria(), foundKeys));
                    break;
                }
                case QVariant::ByteArray:
                {
                    TPtrC8 byteArrayValue((TUint8*)(criteria.value().toByteArray().constData()));
                    User::LeaveIfError(handler->findKeyL(criteria.partialKey(), criteria.bitMask(),
                        byteArrayValue, criteria.negativeCriteria(), foundKeys));
                    break;
                }
                default:
                {
                    User::Leave(KErrArgument);
                    break;
                }
            }
        }
        for (int i = 0; i < foundKeys.Count(); ++i)
        {
            XQCentralRepositorySettingsKey key(repositoryUid, foundKeys[i]);
            keys.append(key);
        }

        CleanupStack::PopAndDestroy(&foundKeys);
    )
    return keys;
}

bool XQCentralRepositoryUtilsPrivate::startTransaction(long int repositoryUid, XQCentralRepositoryUtils::TransactionMode transactionMode)
{
    CCentralRepositoryHandler* handler = m_settingsManagerPrivate.cenRepHandlerInstance(repositoryUid);
    
    if (handler)
    {
        CRepository::TTransactionMode mode = CRepository::EConcurrentReadWriteTransaction;
        switch (transactionMode)
        {
            case XQCentralRepositoryUtils::ConcurrentReadWriteTransaction:
            {
                mode = CRepository::EConcurrentReadWriteTransaction;
                break;
            }
            case XQCentralRepositoryUtils::ReadTransaction:
            {
                mode = CRepository::EReadTransaction;
                break;
            }
            case XQCentralRepositoryUtils::ReadWriteTransaction:
            {
                mode = CRepository::EReadWriteTransaction;
                break;
            }
            default:
            {
                m_settingsManagerPrivate.iError = KErrArgument;
                return false;
            }
        }
        m_settingsManagerPrivate.iError = handler->startTransaction(mode);
    }
    else
    {
        m_settingsManagerPrivate.iError = XQSettingsManager::UnknownError;
    }
    return m_settingsManagerPrivate.iError == KErrNone;
}

bool XQCentralRepositoryUtilsPrivate::commitTransaction(long int repositoryUid)
{
    CCentralRepositoryHandler* handler = m_settingsManagerPrivate.cenRepHandlerInstance(repositoryUid);
    
    if (handler)
    {
        m_settingsManagerPrivate.iError = handler->commitTransaction();
    }
    else
    {
        m_settingsManagerPrivate.iError = XQSettingsManager::UnknownError;
    }
    return m_settingsManagerPrivate.iError == KErrNone;
}

void XQCentralRepositoryUtilsPrivate::cancelTransaction(long int repositoryUid)
{
    CCentralRepositoryHandler* handler = m_settingsManagerPrivate.cenRepHandlerInstance(repositoryUid);
    if (!handler)
    {
        m_settingsManagerPrivate.iError = XQSettingsManager::UnknownError;
        return;
    }
    m_settingsManagerPrivate.iError = XQSettingsManager::NoError;
    handler->cancelTransaction();
}

void XQCentralRepositoryUtilsPrivate::failTransaction(long int repositoryUid)
{
    CCentralRepositoryHandler* handler = m_settingsManagerPrivate.cenRepHandlerInstance(repositoryUid);
    if (!handler)
    {
        m_settingsManagerPrivate.iError = XQSettingsManager::UnknownError;
        return;
    }
    m_settingsManagerPrivate.iError = XQSettingsManager::NoError;
    handler->failTransaction();
}

XQCentralRepositoryUtils::TransactionState XQCentralRepositoryUtilsPrivate::transactionState(long int repositoryUid) const
{
    CCentralRepositoryHandler* handler = m_settingsManagerPrivate.cenRepHandlerInstance(repositoryUid);
    if (!handler)
    {
        m_settingsManagerPrivate.iError = XQSettingsManager::UnknownError;
        return XQCentralRepositoryUtils::UnknownTransaction;
    }

    m_settingsManagerPrivate.iError = XQSettingsManager::NoError;
    switch (handler->transactionState())
    {
        case 0: return XQCentralRepositoryUtils::NotInTransaction;
        case 2: return XQCentralRepositoryUtils::ActiveTransaction;
        case 6: return XQCentralRepositoryUtils::FailedTransaction;
        default:
        {
            break;
        }
    }
    return XQCentralRepositoryUtils::UnknownTransaction;
}
