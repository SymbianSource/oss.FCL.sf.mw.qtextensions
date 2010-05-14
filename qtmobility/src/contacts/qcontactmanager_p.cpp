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

#include <src/global/qbuildcfg.h>
#include "qcontactmanager.h"
#include "qcontactmanager_p.h"
#include "qcontactmanagerengine.h"
#include "qcontactmanagerenginefactory.h"

#include "qcontact_p.h"

#include <QSharedData>
#include <QtPlugin>
#include <QPluginLoader>

#include <QDebug>
#include <QDir>
#include <QFile>

#include <QApplication>

#if defined(Q_OS_SYMBIAN)
# include <f32file.h>
#endif

#include "qcontactmemorybackend_p.h"
#include "qcontactinvalidbackend_p.h"

QTM_BEGIN_NAMESPACE

/* Shared QContactManager stuff here, default engine stuff below */
QHash<QString, QContactManagerEngineFactory*> QContactManagerData::m_engines;

bool QContactManagerData::m_discovered;
bool QContactManagerData::m_discoveredStatic;
QStringList QContactManagerData::m_pluginPaths;

static void qContactsCleanEngines()
{
    QContactManagerData::m_discovered = false;
    QList<QContactManagerEngineFactory*> factories = QContactManagerData::m_engines.values();
    for (int i=0; i < factories.count(); i++) {
        delete factories.at(i);
    }
    QContactManagerData::m_engines.clear();
}


static int parameterValue(const QMap<QString, QString>& parameters, const char* key, int defaultValue)
{
    if (parameters.contains(QString::fromAscii(key))) {
        bool ok;
        int version = parameters.value(QString::fromAscii(key)).toInt(&ok);
        
        if (ok)
            return version;
    }
    return defaultValue;
}

void QContactManagerData::createEngine(const QString& managerName, const QMap<QString, QString>& parameters)
{
    m_engine = 0;

    QString builtManagerName = managerName.isEmpty() ? QContactManager::availableManagers().value(0) : managerName;
    if (builtManagerName == QLatin1String("memory")) {
        m_engine = QContactMemoryEngine::createMemoryEngine(parameters);
    } else {
        int implementationVersion = parameterValue(parameters, QTCONTACTS_IMPLEMENTATION_VERSION_NAME, -1);

        bool found = false;
        bool loadedDynamic = false;

        /* First check static factories */
        loadStaticFactories();

        /* See if we got a fast hit */
        QList<QContactManagerEngineFactory*> factories = m_engines.values(builtManagerName);
        m_error = QContactManager::NoError;

        while(!found) {
            foreach (QContactManagerEngineFactory* f, factories) {
                QList<int> versions = f->supportedImplementationVersions();
                if (implementationVersion == -1 ||//no given implementation version required
                        versions.isEmpty() || //the manager engine factory does not report any version
                        versions.contains(implementationVersion)) {
                    m_engine = f->engine(parameters, &m_error);
                    found = true;
                    break;
                }
            }
            
            // Break if found or if this is the second time through
            if (loadedDynamic || found)
                break;

            // otherwise load dynamic factories and reloop
            loadFactories();
            factories = m_engines.values(builtManagerName);
            loadedDynamic = true;
        }

        // XXX remove this
        // the engine factory could lie to us, so check the real implementation version
        if (m_engine && (implementationVersion != -1 && m_engine->managerVersion() != implementationVersion)) {
            m_error = QContactManager::VersionMismatchError;
            m_engine = 0;
        }

        if (!m_engine) {
            if (m_error == QContactManager::NoError)
                m_error = QContactManager::DoesNotExistError;
            m_engine = new QContactInvalidEngine();
        }
    }
}


void QContactManagerData::loadStaticFactories()
{
    if (!m_discoveredStatic) {
#if !defined QT_NO_DEBUG
        const bool showDebug = qgetenv("QT_DEBUG_PLUGINS").toInt() > 0;
#endif

        m_discoveredStatic = true;

        /* Clean stuff up at the end */
        qAddPostRoutine(qContactsCleanEngines);

        /* Loop over all the static plugins */
        QObjectList staticPlugins = QPluginLoader::staticInstances();
        for (int i=0; i < staticPlugins.count(); i++ ){
            QContactManagerEngineFactory *f = qobject_cast<QContactManagerEngineFactory*>(staticPlugins.at(i));
            if (f) {
                QString name = f->managerName();
#if !defined QT_NO_DEBUG
                if (showDebug)
                    qDebug() << "Static: found an engine plugin" << f << "with name" << name;
#endif
                if (name != QLatin1String("memory") && name != QLatin1String("invalid") && !name.isEmpty()) {
                    // we also need to ensure that we haven't already loaded this factory.
                    if (m_engines.keys().contains(name)) {
                        qWarning() << "Static contacts plugin" << name << "has the same name as a currently loaded plugin; ignored";
                    } else {
                        m_engines.insertMulti(name, f);
                    }
                } else {
                    qWarning() << "Static contacts plugin with reserved name" << name << "ignored";
                }
            }
        }
    }
}

class DirChecker
{
public:
    DirChecker();
    ~DirChecker();
    bool checkDir(const QDir& dir);

private:
#if defined(Q_OS_SYMBIAN)
    RFs rfs;
#endif
};

#if defined(Q_OS_SYMBIAN)
DirChecker::DirChecker()
{
    qt_symbian_throwIfError(rfs.Connect());
}

bool DirChecker::checkDir(const QDir& dir)
{
    bool pathFound = false;
    // In Symbian, going cdUp() in a c:/private/<uid3>/ will result in *platsec* error at fileserver (requires AllFiles capability)
    // Also, trying to cd() to a nonexistent directory causes *platsec* error. This does not cause functional harm, but should
    // nevertheless be changed to use native Symbian methods to avoid unnecessary platsec warnings (as per qpluginloader.cpp).
    // Use native Symbian code to check for directory existence, because checking
    // for files from under non-existent protected dir like E:/private/<uid> using
    // QDir::exists causes platform security violations on most apps.
    QString nativePath = QDir::toNativeSeparators(dir.absolutePath());
    TPtrC ptr = TPtrC16(static_cast<const TUint16*>(nativePath.utf16()), nativePath.length());
    TUint attributes;
    TInt err = rfs.Att(ptr, attributes);
    if (err == KErrNone) {
        // yes, the directory exists.
        pathFound = true;
    }
    return pathFound;
}

DirChecker::~DirChecker()
{
    rfs.Close();
}
#else
DirChecker::DirChecker()
{
}

DirChecker::~DirChecker()
{
}

bool DirChecker::checkDir(const QDir &dir)
{
    return dir.exists();
}
#endif

/* Plugin loader */
void QContactManagerData::loadFactories()
{
#if !defined QT_NO_DEBUG
    const bool showDebug = qgetenv("QT_DEBUG_PLUGINS").toInt() > 0;
#endif

    // Always do this..
    loadStaticFactories();

    if (!m_discovered || QApplication::libraryPaths() != m_pluginPaths) {
        m_discovered = true;
        m_pluginPaths = QApplication::libraryPaths();

        /* Discover a bunch o plugins */
        QStringList plugins;

        QStringList paths;
        QSet<QString> processed;

        paths << QApplication::applicationDirPath() << QApplication::libraryPaths();
        QString val = qt_mobility_configure_prefix_path_str;
        if(val.length() > 0){
            paths << val;
        }
#if !defined QT_NO_DEBUG
        if (showDebug)
            qDebug() << "Plugin paths:" << paths;
#endif

        DirChecker dirChecker;

        /* Enumerate our plugin paths */
        for (int i=0; i < paths.count(); i++) {
            if (processed.contains(paths.at(i)))
                continue;
            processed.insert(paths.at(i));
            QDir pluginsDir(paths.at(i));
            if (!dirChecker.checkDir(pluginsDir))
                continue;

#if defined(Q_OS_WIN)
            if (pluginsDir.dirName().toLower() == QLatin1String("debug") || pluginsDir.dirName().toLower() == QLatin1String("release"))
                pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
            if (pluginsDir.dirName() == QLatin1String("MacOS")) {
                pluginsDir.cdUp();
                pluginsDir.cdUp();
                pluginsDir.cdUp();
            }
#endif

            QString subdir(QLatin1String("plugins/contacts"));
            if (pluginsDir.path().endsWith(QLatin1String("/plugins"))
                || pluginsDir.path().endsWith(QLatin1String("/plugins/")))
                subdir = QLatin1String("contacts");

            if (dirChecker.checkDir(QDir(pluginsDir.path() + QLatin1Char('/') + subdir))) {
                pluginsDir.cd(subdir);
                const QStringList& files = pluginsDir.entryList(QDir::Files);
#if !defined QT_NO_DEBUG
                if (showDebug)
                    qDebug() << "Looking for contacts plugins in" << pluginsDir.path() << files;
#endif
                for (int j=0; j < files.count(); j++) {
                    plugins <<  pluginsDir.absoluteFilePath(files.at(j));
                }
            }
        }

        /* Now discover the dynamic plugins */
        for (int i=0; i < plugins.count(); i++) {
            QPluginLoader qpl(plugins.at(i));
            QContactManagerEngineFactory *f = qobject_cast<QContactManagerEngineFactory*>(qpl.instance());
            if (f) {
                QString name = f->managerName();
#if !defined QT_NO_DEBUG
                if (showDebug)
                    qDebug() << "Dynamic: found a contact engine plugin" << f << "with name" << name;
#endif
                if (name != QLatin1String("memory") && name != QLatin1String("invalid") && !name.isEmpty()) {
                    // we also need to ensure that we haven't already loaded this factory.
                    if (m_engines.keys().contains(name)) {
                        qWarning() << "Contacts plugin" << plugins.at(i) << "has the same name as currently loaded plugin" << name << "; ignored";
                    } else {
                        m_engines.insertMulti(name, f);
                    }
                } else {
                    qWarning() << "Contacts plugin" << plugins.at(i) << "with reserved name" << name << "ignored";
                }
            }

            /* Debugging */
#if !defined QT_NO_DEBUG
            if (showDebug && !f) {
                qDebug() << "Unknown plugin:" << qpl.errorString();
                if (qpl.instance()) {
                    qDebug() << "[qobject:" << qpl.instance() << "]";
                }
            }
#endif
        }
        
        QStringList engineNames;
        foreach (QContactManagerEngineFactory* f, m_engines.values()) {
            QStringList versions;
            foreach (int v, f->supportedImplementationVersions()) {
                versions << QString::fromAscii("%1").arg(v);
            }
            engineNames << QString::fromAscii("%1[%2]").arg(f->managerName()).arg(versions.join(QString::fromAscii(",")));
        }
#if !defined QT_NO_DEBUG
        if (showDebug) {
            qDebug() << "Found engines:" << engineNames;
        }
#endif
    }
}

// trampoline for private classes
QContactManagerEngine* QContactManagerData::engine(const QContactManager* manager)
{
    if (manager)
        return manager->d->m_engine;
    return 0;
}

QTM_END_NAMESPACE

