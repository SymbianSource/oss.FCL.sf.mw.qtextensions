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
* Description:  Class implement extended wrapper for ECom framework
*
*/

#include "xqpluginloader.h"
#include "xqpluginloaderprivate.h"

#include <qfileinfo.h>
#include "qdebug.h"
// -----------------------------------------------------------------------------
// XQPluginLoader()
// -----------------------------------------------------------------------------
//
XQPluginLoader::XQPluginLoader(QObject* parent) 
: 
QObject(parent),
d(0),
did_load( false )
{
}

// -----------------------------------------------------------------------------
// XQPluginLoader(int, QObject*)
// -----------------------------------------------------------------------------
//
XQPluginLoader::XQPluginLoader(int requestedUid, QObject* parent) 
: 
QObject(parent),
d(XQPluginLoaderPrivate::findOrCreate(requestedUid)), 
did_load( false )
{
}

// -----------------------------------------------------------------------------
// ~XQPluginLoader2()
// -----------------------------------------------------------------------------
//
XQPluginLoader::~XQPluginLoader()
{
    if(d) {
        d->release();
    }
}

// -----------------------------------------------------------------------------
// listImplementations(const QString &interfaceName, QList<XQPluginInfo> &impls)
// -----------------------------------------------------------------------------
//
bool XQPluginLoader::listImplementations(
        const QString &interfaceName, 
        QList< XQPluginInfo > &impls)
{
    RImplInfoPtrArray infoArr;
    TRAPD( errCode, XQPluginLoaderPrivate::listImplementationsL( infoArr, interfaceName, impls ) );
    infoArr.ResetAndDestroy();
    return ( KErrNone == errCode );
}

// -----------------------------------------------------------------------------
// uid()
// -----------------------------------------------------------------------------
//
int XQPluginLoader::uid()const
{
    return ( d ? d->uid : KNullUid.iUid );
}

// -----------------------------------------------------------------------------
// instance()
// -----------------------------------------------------------------------------
//
QObject* XQPluginLoader::instance()
{
    if (!load())
        return 0;
#ifndef Q_OS_SYMBIAN
    if (d->instance)
        return d->instance();
	else
		return 0;
#else
    return d->instance();
#endif
    
}

// -----------------------------------------------------------------------------
// isLoaded()
// -----------------------------------------------------------------------------
//
bool XQPluginLoader::isLoaded() const
{
    return d && d->pHnd
#ifndef Q_OS_SYMBIAN
        && d->instance;
#else
        ;
#endif    
}

// -----------------------------------------------------------------------------
// load()
// -----------------------------------------------------------------------------
//
bool XQPluginLoader::load()
{
    if (!d)
        return false;
    if (did_load)
        return d->pHnd;
    did_load = true;
    return d->loadPlugin();
}

// -----------------------------------------------------------------------------
// unload()
// -----------------------------------------------------------------------------
//
bool XQPluginLoader::unload()
{
    if (did_load) {
        did_load = false;
        return d->unload();
    }
    if (d)  // Ouch
        d->errorString = tr("The plugin was not loaded.");
    return false;
}

// -----------------------------------------------------------------------------
// setUid ( int )
// -----------------------------------------------------------------------------
//
void XQPluginLoader::setUid ( int uid )
{
    if (d) {
        d->release();
        d = 0;
        did_load = false;
    }
    d = XQPluginLoaderPrivate::findOrCreate( uid );
}

// -----------------------------------------------------------------------------
// errorString ()
// -----------------------------------------------------------------------------
//
QString XQPluginLoader::errorString () const
{
    return (!d || d->errorString.isEmpty()) ? tr("Unknown error") : d->errorString;
}
