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

#ifndef XQPLUGINLOADER_H
#define XQPLUGINLOADER_H

//Incudes
#include <QtGlobal>
#include <QObject>
#include <QList>
#include <QString>
//Forward class declarations
class XQPluginInfo;
class XQPluginLoaderPrivate;

#ifdef BUILD_XQPLUGINS_DLL
#define DLL_EXPORT Q_DECL_EXPORT
#else
#define DLL_EXPORT Q_DECL_IMPORT
#endif


//Class declaration
class DLL_EXPORT XQPluginLoader : public QObject
{
public:
    /**
     * Constructor
     * @param parent - address of class instance parent 
     */
    XQPluginLoader(QObject* parent = 0);
    
    /**
     * Constructor
     * @param uid - UID of plugin that should be loaded
     * @param parent - address of class instance parent 
     */
    XQPluginLoader(int uid, QObject* parent = 0);
    
    /**
     * Destructor
     */
    virtual ~XQPluginLoader();
    
    /**
     * List available plugins which implement requested interface. 
     * Function resolve plugins using interface name
     * @param interfaceName - requested interface name
     * @param impls - destination list where resolved plugins info will be stored
     * @return true on success, false on any error
     */
    static bool listImplementations(const QString &interfaceName, 
                             QList<XQPluginInfo > &impls);
    
    
    /**
     * Function return UID of requested plugin
     */
    int uid()const;
    
    
    /**
     * Function return pointer to plugin root-component instance
     * @return instance address on success, 0 otherwise  
     */
    QObject* instance();
    
    /**
     * Function return information if plugin have been loaded
     * @return true if plugin have been loaded, false otherwise 
     */
    bool isLoaded() const;
    
    /**
     * Function load requested plugin
     * @return true on success, false otherwise
     */
    bool load();
    
    
    /**
     * Function unload plugin
     * @return true on success, false otherwise 
     */
    bool unload();
    
    /**
     * 
     */
    void setUid ( int uid );
    
    /**
     * 
     */
    QString errorString () const;
    
private:
    /**
     * Private extended plugin loader implementation.
     * Own
     */
    XQPluginLoaderPrivate* d;
    
    /**
     * Flag that inform about loading state
     */
    bool did_load;
    
    Q_DISABLE_COPY(XQPluginLoader)
};

#endif
