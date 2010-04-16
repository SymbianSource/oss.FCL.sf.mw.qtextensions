/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the $MODULE$ of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the either Technology Preview License Agreement or the
** Beta Release License Agreement.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include "symmake.h"
#include "initprojectdeploy_symbian.h"

#include <qstring.h>
#include <qhash.h>
#include <qstringlist.h>
#include <qdir.h>
#include <qdatetime.h>
#include <stdlib.h>
#include <qdebug.h>

#define RESOURCE_DIRECTORY_MMP "/resource/apps"
#define RESOURCE_DIRECTORY_RESOURCE "\\\\resource\\\\apps\\\\"
#define REGISTRATION_RESOURCE_DIRECTORY_HW "/private/10003a3f/import/apps"
#define PLUGIN_COMMON_DEF_FILE_FOR_MMP "./plugin_common.def"
#define PLUGIN_COMMON_DEF_FILE_ACTUAL "plugin_commonU.def"
#define BLD_INF_FILENAME_LEN (sizeof(BLD_INF_FILENAME) - 1)

#define BLD_INF_RULES_BASE "BLD_INF_RULES."
#define BLD_INF_TAG_PLATFORMS "prj_platforms"
#define BLD_INF_TAG_MMPFILES "prj_mmpfiles"
#define BLD_INF_TAG_TESTMMPFILES "prj_testmmpfiles"
#define BLD_INF_TAG_EXTENSIONS "prj_extensions"
#define RSS_RULES "RSS_RULES"
#define RSS_RULES_BASE "RSS_RULES."
#define RSS_TAG_NBROFICONS "number_of_icons"
#define RSS_TAG_ICONFILE "icon_file"

#define DUMP_VAR(v) \
{ \
    QString s(v); \
    QStringList list = project->values(s); \
    printf("----------------------------------\n", qPrintable(s)); \
    printf("Dumping %s (%d items) from %s, %d\n", \
            qPrintable(s), \
            list.count(), \
            __FILE__, \
            __LINE__); \
    foreach(QString l, list) \
        printf("\t%s\n", qPrintable(l)); \
}

QString SymbianMakefileGenerator::fixPathForMmp(const QString& origPath, const QDir& parentDir)
{
    static QString epocRootStr;
    if (epocRootStr.isEmpty()) {
        QFileInfo efi(epocRoot());
        epocRootStr = efi.canonicalFilePath();
        if (epocRootStr.isEmpty()) {
            fprintf(stderr, "Unable to resolve epocRoot '%s' to real dir on current drive, defaulting to '/' for mmp paths\n", qPrintable(epocRoot()));
            epocRootStr = "/";
        }
        if (!epocRootStr.endsWith("/"))
            epocRootStr += "/";

        epocRootStr += "epoc32/";
    }

    QString resultPath = origPath;

    // Make it relative, unless it starts with "%epocroot%/epoc32/"
    if (resultPath.startsWith(epocRootStr, Qt::CaseInsensitive)) {
        resultPath.replace(epocRootStr, "/epoc32/", Qt::CaseInsensitive);
    } else {
        resultPath = parentDir.relativeFilePath(resultPath);
    }
    resultPath = QDir::cleanPath(resultPath);

    if (resultPath.isEmpty())
        resultPath = ".";

    return resultPath;
}

QString SymbianMakefileGenerator::canonizePath(const QString& origPath)
{
    // Since current path gets appended almost always anyway, use it as default
    // for nonexisting paths.
    static QString defaultPath;
    if (defaultPath.isEmpty()) {
        QFileInfo fi(".");
        defaultPath = fi.canonicalFilePath();
    }

    // Prepend epocroot to any paths beginning with "/epoc32/"
    QString resultPath = QDir::fromNativeSeparators(origPath);
    QString tempPath(resultPath);
    bool isEpoc = false;
    if (resultPath.startsWith("/epoc32/", Qt::CaseInsensitive)) {
        isEpoc = true;
        resultPath = QDir::fromNativeSeparators(epocRoot()) + resultPath.mid(1);
    }

    QFileInfo fi(fileInfo(resultPath));
    if(fi.isDir()) {
        if (isEpoc)
            resultPath = fi.absoluteFilePath();//canonicalFilePath();
        else
            resultPath = fi.canonicalFilePath();            
    } else {
        if (isEpoc)
            resultPath = fi.absolutePath();//canonicalPath();
        else
            resultPath = fi.canonicalPath();
    }
    //some fix for the not existed EPOC32\include folder
    if (isEpoc) {
        int index = resultPath.lastIndexOf("/epoc32/"); 
        QString tmpRes = resultPath.mid(index);
        if (tmpRes != tempPath) {
            //we have the problems for not existed include directory
            //change the path
            resultPath.replace(tmpRes, tempPath);
            }
    }

    resultPath = QDir::cleanPath(resultPath);

    if (resultPath.isEmpty())
        resultPath = defaultPath;

    return resultPath;
}

SymbianMakefileGenerator::SymbianMakefileGenerator() : MakefileGenerator() { }
SymbianMakefileGenerator::~SymbianMakefileGenerator() { }

void SymbianMakefileGenerator::writeHeader(QTextStream &t) {
    t << "// ============================================================================" << endl;
    t << "// * Makefile for building: " << escapeFilePath(var("TARGET")) << endl;
    t << "// * Generated by qmake (" << qmake_version() << ") (Qt " << QT_VERSION_STR << ") on: ";
    t << QDateTime::currentDateTime().toString(Qt::ISODate) << endl;
    t << "// * This file is generated by qmake and should not be modified by the" << endl;
    t << "// * user." << endl;
    t << "// * Project:  " << fileFixify(project->projectFile()) << endl;
    t << "// * Template: " << var("TEMPLATE") << endl;
    //if(!project->isActiveConfig("build_pass"))
    //    t << "// = Command: " << build_args().replace("$(QMAKE)",
    //                  (project->isEmpty("QMAKE_QMAKE") ? QString("qmake") : var("QMAKE_QMAKE"))) << endl;
    t << "// ============================================================================" << endl;
    t << endl;

    // defining define for bld.inf

    QString shortProFilename = project->projectFile();
    shortProFilename.replace(0, shortProFilename.lastIndexOf("/") + 1, QString(""));
    shortProFilename.replace(QString(".pro"), QString(""));

    QString bldinfDefine = shortProFilename;
    bldinfDefine.append("_");
    bldinfDefine.append(generate_uid(project->projectFile()));

    bldinfDefine.prepend("BLD_INF_");
    removeSpecialCharacters(bldinfDefine);

    t << "#define " << bldinfDefine.toUpper() << endl << endl;
}

bool SymbianMakefileGenerator::writeMakefile(QTextStream &t) {
    writeHeader(t);

    QString numberOfIcons;
    QString iconFile;
    QStringList userRssRules;
    readRssRules(numberOfIcons, iconFile, userRssRules);

    // Get the application translations and convert to symbian OS lang code, i.e. decical number
    QStringList symbianLangCodes = symbianLangCodesFromTsFiles();

    // Generate pkg files if there are any actual files to deploy
    bool generatePkg = false;
    if (getTargetExtension() == "exe") {
        generatePkg = true;
    } else {
        foreach(QString item, project->values("DEPLOYMENT")) {
            if (!project->values(item + ".sources").isEmpty()) {
                generatePkg = true;
                break;
            }
        }
    }

    if (generatePkg) {
        QStringList platformList = project->values("SYMBIAN_PLATFORMS");
        foreach(QString platform, platformList) {
            if(platform.compare("WINSCW", Qt::CaseInsensitive)) {
                generatePkgFile(platform.toLower(), "udeb", iconFile);
                generatePkgFile(platform.toLower(), "urel", iconFile);
            }
        }
    }

    writeBldInfContent(t, generatePkg);

    // Generate empty wrapper makefile here, because wrapper makefile must exist before writeMkFile,
    // but all required data is not yet available.
    bool isPrimaryMakefile = true;
    QString wrapperFileName("Makefile");
    QString outputFileName = fileInfo(Option::output.fileName()).fileName();
    if (outputFileName != BLD_INF_FILENAME) {
        wrapperFileName.append(".").append((outputFileName.size() > BLD_INF_FILENAME_LEN && outputFileName.left(BLD_INF_FILENAME_LEN) == BLD_INF_FILENAME) ? outputFileName.mid(8) : outputFileName);
        isPrimaryMakefile = false;
    }

    QFile wrapperMakefile(wrapperFileName);
    if(wrapperMakefile.open(QIODevice::WriteOnly)) {
        generatedFiles << wrapperFileName;
    } else {
        fprintf(stderr, "Error: Could not open wrapper makefile '%s'\n", qPrintable(wrapperFileName));
        return false;
    }

    if (getTargetExtension() == "subdirs") {
            // If we have something to deploy, generate extension makefile for just that, since
            // normal extension makefile is not getting generated and we need emulator deployment to be done.
            if (generatePkg)
                writeMkFile(wrapperFileName, true);
            writeWrapperMakefile(wrapperMakefile, isPrimaryMakefile);
            return true;
        }

    writeMkFile(wrapperFileName, false);

    QString shortProFilename = project->projectFile();
    shortProFilename.replace(0, shortProFilename.lastIndexOf("/") + 1, QString(""));
    shortProFilename.replace(QString(".pro"), QString(""));

    QString mmpFilename = shortProFilename;
    mmpFilename.append("_");
    mmpFilename.append(uid3);
    mmpFilename.append(".mmp");
    writeMmpFile(mmpFilename, symbianLangCodes);

    if (getTargetExtension() == "exe") {
        if (!project->values("CONFIG").contains("no_icon", Qt::CaseInsensitive)) {
            QString appname = escapeFilePath(fileFixify(project->first("TARGET")));
            appname = removePathSeparators(appname);
            writeRegRssFile(appname, userRssRules);
            writeRssFile(appname, numberOfIcons, iconFile);
            writeLocFile(appname, symbianLangCodes);
        }
    }

    writeCustomDefFile();
    writeWrapperMakefile(wrapperMakefile, isPrimaryMakefile);

    return true;
}

bool SymbianMakefileGenerator::generatePkgFile(const QString &compiler, const QString &config, const QString &iconFile) {
    QString pkgFilename = QString("%1_%2_%3.%4")
                          .arg(fileInfo(project->projectFile()).completeBaseName())
                          .arg(compiler)
                          .arg(config)
                          .arg("pkg");
    QFile pkgFile(pkgFilename);
    if (!pkgFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    generatedFiles << pkgFile.fileName();

    //   header info
    QTextStream t(&pkgFile);
    t << QString("; %1 generated by qmake at %2").arg(pkgFilename).arg(QDateTime::currentDateTime().toString(Qt::ISODate))  << endl;
    t << "; This file is generated by qmake and should not be modified by the user" << endl;
    t << ";" << endl << endl;

    // language, (*** hardcoded to english atm)
    t << "; Language" << endl;
    t << "&EN" << endl << endl;

    // name of application, UID and version
    QString applicationName = project->first("TARGET");
    int last = applicationName.lastIndexOf(QLatin1Char('/'));
    applicationName = applicationName.mid( last == -1 ? 0 : last+1 );

    QString applicationVersion = project->first("VERSION").isEmpty() ? "1,0,0" : project->first("VERSION").replace('.', ',');

    t << "; SIS header: name, uid, version" << endl;
    t << QString("#{\"%1\"},(%2),%3").arg(applicationName).arg(uid3).arg(applicationVersion) << endl << endl;

    // vendor names (*** hardcoded for now)
    t << "; Localised Vendor name" << endl;
    t << "%{\"Nokia, Qt Software\"}" << endl << endl;
    t << "; Unique Vendor name" << endl;
    t << ":\"Nokia, Qt Software\"" << endl << endl;

    // Dependencies
    t << "; Dependencies" << endl;
    foreach(QString item, project->values("DEPLOYMENT")) {
        QStringList dependencies = project->values(item + ".depends");
        foreach(QString dependency, dependencies) {
            t << dependency << endl;
        }
    }
    t << endl;

    // install paths on the phone *** should be dynamic at some point
    QString installPathBin = "!:\\sys\\bin";
    QString installPathResource = "!:\\resource\\apps";
    QString installPathRegResource = "!:\\private\\10003a3f\\import\\apps";

    // find location of builds
    QString epocReleasePath = QString("%1epoc32/release/%2/%3")
                              .arg(epocRoot())
                              .arg(compiler)
                              .arg(config);


    if (getTargetExtension() == "exe") {
        // deploy .exe file
        t << "; Executable and default resource files" << endl;
        QString exeFile = applicationName + ".exe";
        t << QString("\"%1/%2\"    - \"%3\\%4\"")
             .arg(epocReleasePath)
             .arg(exeFile)
             .arg(installPathBin)
             .arg(exeFile) << endl;

        // deploy rsc & reg_rsc file
        if (!project->values("CONFIG").contains("no_icon", Qt::CaseInsensitive)) {
            t << QString("\"%1epoc32/data/z/resource/apps/%2\"    - \"%3\\%4\"")
                 .arg(epocRoot())
                 .arg(applicationName + ".rsc")
                 .arg(installPathResource)
                 .arg(applicationName + ".rsc") << endl;

            t << QString("\"%1epoc32/data/z/private/10003a3f/import/apps/%2\"    - \"%3\\%4\"")
                 .arg(epocRoot())
                 .arg(applicationName + "_reg.rsc")
                 .arg(installPathRegResource)
                 .arg(applicationName + "_reg.rsc") << endl;

            QString myIconFile = iconFile;
            myIconFile = myIconFile.replace("\\\\", "\\");

            if (!iconFile.isEmpty())  {
                t << QString("\"%1epoc32/data/z%2\"    - \"!:%3\"")
                         .arg(epocRoot())
                         .arg(QString(myIconFile).replace('\\','/'))
                         .arg(myIconFile) << endl << endl;
            }
        }
    }

    // deploy any additional DEPLOYMENT  files
    DeploymentList depList;
    QString remoteTestPath;
    remoteTestPath = QString("!:\\private\\%1").arg(privateDirUid);

    initProjectDeploySymbian( project, depList, remoteTestPath, true, compiler, config, generatedDirs, generatedFiles );
    if (depList.size())
        t << "; DEPLOYMENT" << endl;
    for (int i=0; i<depList.size(); ++i)  {
        t << QString("\"%1\"    - \"%2\"")
             .arg(QString(depList.at(i).from).replace('\\','/'))
             .arg(depList.at(i).to) << endl;
    }

    return true;
}

bool SymbianMakefileGenerator::writeCustomDefFile() {
    if(targetType.compare("plugin", Qt::CaseInsensitive) == 0 && !project->values("CONFIG").contains("stdbinary", Qt::CaseInsensitive)) {
        // Create custom def file for plugin
        QFile ft(QLatin1String(PLUGIN_COMMON_DEF_FILE_ACTUAL));

        if(ft.open(QIODevice::WriteOnly)) {
            generatedFiles << ft.fileName();
            QTextStream t(&ft);

            t << "; ==============================================================================" << endl;
            t << "; Generated by qmake (" << qmake_version() << ") (Qt " << QT_VERSION_STR << ") on: ";
            t << QDateTime::currentDateTime().toString(Qt::ISODate) << endl;
            t << "; This file is generated by qmake and should not be modified by the" << endl;
            t << "; user." << endl;
            t << ";  Name        : " PLUGIN_COMMON_DEF_FILE_ACTUAL << endl;
            t << ";  Part of     : " << project->values("TARGET").join(" ") << endl;
            t << ";  Description : Fixes common plugin symbols to known ordinals" << endl;
            t << ";  Version     : " << endl;
            t << ";" << endl;
            t << "; ==============================================================================" << "\n" << endl;

            t << endl;

            t << "EXPORTS" << endl;
            t << "\tqt_plugin_query_verification_data @ 1 NONAME" << endl;
            t << "\tqt_plugin_instance @ 2 NONAME" << endl;
            t << endl;
        } else {
            return false;
        }
    }

    return true;
}

void SymbianMakefileGenerator::init()
{
    MakefileGenerator::init();
/*
    DUMP_VAR("GENERATED_SOURCES");
*/
    // fixing again !!!
    if(0 != project->values("QMAKE_PLATFORM").size())
        platform = varGlue("QMAKE_PLATFORM", "", " ", "");

    if(0 == project->values("QMAKESPEC").size())
        project->values("QMAKESPEC").append(qgetenv("QMAKESPEC"));

    if(!isConfigSetToSymbian())
        project->values("QMAKE_LIBS") += escapeFilePaths(project->values("LIBS"));


    // bld.inf
    project->values("MAKEFILE") += BLD_INF_FILENAME;

    // .mmp
    initMmpVariables();

    // UID1
    uid1 = generateUID1();

    // check TARGET.UID2 and TARGET.UID3 presence
    if(0 != project->values("TARGET.UID3").size()) {
        uid3 = project->first("TARGET.UID3");
    } else {
        uid3 = generateUID3();
    }

    // some fix
    if((project->values("TEMPLATE")).contains("app"))
        targetType = "exe";
    else if((project->values("TEMPLATE")).contains("lib")) {
        // check CONFIG to see if we are to build staticlib or dll
        if(project->values("CONFIG").contains("staticlib") || project->values("CONFIG").contains("static"))
            targetType = "staticlib";
        else if (project->values("CONFIG").contains("plugin"))
            targetType = "plugin";
        else // for now it will be default
            targetType = "dll";
    }
    else // fix
        targetType = "subdirs";

    if(0 != project->values("TARGET.UID2").size()) {
        uid2 = project->first("TARGET.UID2");
    } else if (project->values("CONFIG").contains("stdbinary", Qt::CaseInsensitive)) {
        uid2 = "0x20004C45";
    } else {
        if(getTargetExtension() == "exe") {
            if(project->values("QT").contains("gui", Qt::CaseInsensitive)) {
                // exe and also gui
                uid2 = "0x100039CE";
            } else {
                // exe but not gui.. uid2 is ignored anyway
                // set it to 0
                uid2 = "0";
            }
        } else if(getTargetExtension() == "dll" || getTargetExtension() == "lib") {
            uid2 = "0x1000008d";
        }
    }

    uid2 = uid2.trimmed();
    uid3 = uid3.trimmed();

    // UID is valid as either hex or decimal, so just convert it to number and back to hex
    // to get proper string for private dir
    bool conversionOk = false;
    uint uidNum = uid3.toUInt(&conversionOk, 0);

    if (!conversionOk) {
        fprintf(stderr, "Error: Invalid UID \"%s\".", uid3.toUtf8().constData());
    } else {
        privateDirUid.setNum(uidNum, 16);
        while (privateDirUid.length() < 8)
            privateDirUid.insert(0,QLatin1Char('0'));
    }
}

QString SymbianMakefileGenerator::getTargetExtension() {
    QString ret;
    if(targetType.compare("exe", Qt::CaseInsensitive) == 0 || targetType.compare("app", Qt::CaseInsensitive) == 0) {
        ret.append("exe");
    } else if (targetType.compare("staticlib",Qt::CaseInsensitive) == 0) {
        ret.append("lib");
    } else if (targetType.compare("dll", Qt::CaseInsensitive) == 0 || targetType.compare("plugin", Qt::CaseInsensitive) == 0) {
        ret.append("dll");
    } else if (targetType.compare("subdirs", Qt::CaseInsensitive) == 0) {
        // just fix
        ret.append("subdirs");
    } else {
        // if nothing said then assume "exe"
        ret.append("exe");
    }

    return ret;
}

bool SymbianMakefileGenerator::isConfigSetToSymbian() {
    return project->values("CONFIG").contains("symbian", Qt::CaseInsensitive);
}

QString SymbianMakefileGenerator::generateUID1() {
    // just for now
    return QString("");
}

QString SymbianMakefileGenerator::generateUID2() {
    // standard stuff; picked form symbian
    // later meybe read from somewhere
    return QString("");
}

QString SymbianMakefileGenerator::generateUID3() {

    QString target = project->first("TARGET");
    QString currPath = qmake_getpwd();
    target.prepend("/").prepend(currPath);
    return generate_test_uid(target);

}

bool SymbianMakefileGenerator::initMmpVariables() {
    QStringList sysincspaths;
    QStringList srcincpaths;
    QStringList srcpaths;

    srcpaths << project->values("SOURCES") << project->values("GENERATED_SOURCES");
    srcpaths << project->values("UNUSED_SOURCES") << project->values("UI_SOURCES_DIR");
    srcpaths << project->values("UI_DIR");

    QDir current = QDir::current();
    QString canonizedCurrent = canonizePath(".");

    for(int j = 0; j < srcpaths.size(); ++j) {
        QFileInfo fi(fileInfo(srcpaths.at(j)));
        // Sometimes sources have other than *.c* files (e.g. *.moc); prune them.
        if (fi.suffix().startsWith("c")) {
            if(fi.filePath().length() > fi.fileName().length() ) {
                appendIfnotExist(srcincpaths, fi.path() );
                sources[canonizePath(fi.path())] += fi.fileName();
            } else {
                sources[canonizedCurrent] += fi.fileName();
                appendIfnotExist(srcincpaths, canonizedCurrent);
            }
        }
    }

    QStringList incpaths;
    incpaths << project->values("INCLUDEPATH");
    incpaths << QLibraryInfo::location(QLibraryInfo::HeadersPath);
    incpaths << project->values("HEADERS");
    incpaths << srcincpaths;
    incpaths << project->values("UI_HEADERS_DIR");
    incpaths << project->values("UI_DIR");

    QString epocPath("epoc32");
    for(int j = 0; j < incpaths.size(); ++j) {
        QString includepath = canonizePath(incpaths.at(j));
        appendIfnotExist(sysincspaths, includepath);
        if (!includepath.contains(epocPath)) // No temp dirs for epoc includes
            appendIfnotExist(sysincspaths, includepath + QString("/tmp"));
    }

    // remove duplicate include path entries
    // convert to native directory separators
    // to check if includepaths are same
    QStringList temporary;
    for(int i = 0; i < sysincspaths.size(); ++i) {
        QString origPath = sysincspaths.at(i);
        QFileInfo origPathInfo(fileInfo(origPath));
        bool bFound = false;

        for(int j = 0; j < temporary.size(); ++j) {
            QString tmpPath = temporary.at(j);
            QFileInfo tmpPathInfo(fileInfo(tmpPath));

            if(origPathInfo.absoluteFilePath() == tmpPathInfo.absoluteFilePath()) {
                bFound = true;
                if(!tmpPathInfo.isRelative() && origPathInfo.isRelative()) {
                    // we keep the relative notation
                    temporary.removeOne(tmpPath);
                    temporary << origPath;
                }
            }
        }

        if(!bFound)
            temporary << origPath;

    }

    sysincspaths.clear();
    sysincspaths << temporary;

    systeminclude.insert("SYSTEMINCLUDE", sysincspaths);

    return true;
}

bool SymbianMakefileGenerator::removeDuplicatedStrings(QStringList& stringList) {

    QStringList tmpStringList;

    for(int i = 0; i < stringList.size(); ++i) {
        QString string = stringList.at(i);
        if(tmpStringList.contains(string))
            continue;
        else
            tmpStringList.append(string);
    }

    stringList.clear();
    stringList = tmpStringList;
    return true;
}

bool SymbianMakefileGenerator::writeMmpFileHeader(QTextStream &t){
    t << "// ==============================================================================" << endl;
    t << "// Generated by qmake (" << qmake_version() << ") (Qt " << QT_VERSION_STR << ") on: ";
    t << QDateTime::currentDateTime().toString(Qt::ISODate) << endl;
    t << "// This file is generated by qmake and should not be modified by the" << endl;
    t << "// user." << endl;
    t << "//  Name        : " << escapeFilePath(fileFixify(project->projectFile().remove(project->projectFile().length()-4,4))) << ".mmp" << endl;
    t << "// ==============================================================================" << endl << endl;

    return true;
}

bool SymbianMakefileGenerator::writeMmpFile(QString &filename, QStringList &symbianLangCodes)
{
    QFile ft(filename);
    if(ft.open(QIODevice::WriteOnly)) {
        generatedFiles << ft.fileName();

        //printf("WRITING: %s \n", qPrintable(filename));

        QTextStream t(&ft);

        writeMmpFileHeader(t);

        writeMmpFileTargetPart(t);

        writeMmpFileResourcePart(t, symbianLangCodes);

        writeMmpFileMacrosPart(t);

        writeMmpFileIncludePart(t);

        QDir current = QDir::current();

        for(QMap<QString, QStringList>::iterator it = sources.begin(); it != sources.end(); ++it) {
            QStringList values = it.value();
            QString currentSourcePath = it.key();

            if (values.size())
                t << "SOURCEPATH \t" <<  fixPathForMmp(currentSourcePath, current) << endl;

            for (int i = 0; i < values.size(); ++i) {
                QString sourceFileName = values.at(i);
                t << "SOURCE\t\t" << sourceFileName << endl;
            }
            t << endl;
        }
        t << endl;

        writeMmpFileLibraryPart(t);

        writeMmpFileCapabilityPart(t);

        writeMmpFileCompilerOptionPart(t);

        writeMmpFileBinaryVersionPart(t);

        writeMmpFileRulesPart(t);
    } else {
        return false;
    }

    return true;
}

bool SymbianMakefileGenerator::writeMmpFileMacrosPart(QTextStream& t) {

    t << endl;

    if(isConfigSetToSymbian())
        return true;

    QStringList &defines = project->values("DEFINES");
    if (defines.size())
        t << "// Qt Macros" << endl;
    for(int i = 0; i < defines.size(); ++i) {
        QString def = defines.at(i);
        addMacro(t, def);
    }

    // These are required in order that all methods will be correctly exported e.g from qtestlib
    QStringList &exp_defines = project->values("PRL_EXPORT_DEFINES");
    if (exp_defines.size())
        t << endl << "// Qt Export Defines" << endl;
    for(int i = 0; i < exp_defines.size(); ++i) {
        QString def = exp_defines.at(i);
        addMacro(t, def);
    }

    t << endl;

    return true;
}

bool SymbianMakefileGenerator::addMacro(QTextStream& t, const QString& value) {
     t << "MACRO" << "\t\t" <<  value << endl;
     return true;
}


bool SymbianMakefileGenerator::writeMmpFileTargetPart(QTextStream& t) {
    QString targetTypeValue("");
    if(getTargetExtension() == "exe") {
        t << "TARGET" << "\t\t" << removePathSeparators(escapeFilePath(fileFixify(project->first("TARGET"))).append(".exe")) << "\n";
        if (project->values("CONFIG").contains("stdbinary", Qt::CaseInsensitive))
	    targetTypeValue = QString("STDEXE");
        else
	    targetTypeValue = QString("EXE");
    }  else if (getTargetExtension() == "dll"){
        t << "TARGET" << "\t\t" << removePathSeparators(escapeFilePath(fileFixify(project->first("TARGET"))).append(".dll")) << "\n";
        if (project->values("CONFIG").contains("stdbinary", Qt::CaseInsensitive))
	    targetTypeValue = QString("STDDLL");
        else
	    targetTypeValue = QString("DLL");
    }   else if (getTargetExtension() == "lib"){
        t << "TARGET" << "\t\t" << removePathSeparators(escapeFilePath(fileFixify(project->first("TARGET"))).append(".lib")) << "\n";
        if (project->values("CONFIG").contains("stdbinary", Qt::CaseInsensitive))
	    targetTypeValue = QString("STDLIB");
        else
	    targetTypeValue = QString("LIB");
    }   else {
        printf("unexpected target and targettype %s\n", getTargetExtension().toAscii().data());
    }
    if (0 != project->values("TARGETTYPE_OVERRIDE").size()) {
        targetTypeValue = project->first("TARGETTYPE_OVERRIDE");
    }

    t << "TARGETTYPE" << "\t\t" << targetTypeValue << endl;

    t << endl;

    t << "UID" << "\t\t" << uid2 << " " << uid3 << endl;

    if(0 != project->values("TARGET.SID").size()) {
        t << "SECUREID" << "\t\t" << project->values("TARGET.SID").join(" ") << endl;
    } else {
        if(0 == uid3.size())
            t << "SECUREID" << "\t\t" << "0" << endl;
        else
            t << "SECUREID" << "\t\t" << uid3 << endl;
    }

    // default value used from mkspecs..qconfig.h is 0
    if(0 != project->values("TARGET.VID").size()) {
        t << "VENDORID" << "\t\t" << project->values("TARGET.VID").join(" ") << endl;
    }

    t << endl;

    if(0 != project->first("TARGET.EPOCSTACKSIZE").size())
        t << "EPOCSTACKSIZE" << "\t\t" << project->first("TARGET.EPOCSTACKSIZE") << endl;
    if(0 != project->values("TARGET.EPOCHEAPSIZE").size())
        t << "EPOCHEAPSIZE" << "\t\t" << project->values("TARGET.EPOCHEAPSIZE").join(" ") << endl;
    if(0 != project->values("TARGET.EPOCALLOWDLLDATA").size())
        t << "EPOCALLOWDLLDATA" << endl;

    if(targetType.compare("plugin", Qt::CaseInsensitive) == 0 && !project->values("CONFIG").contains("stdbinary", Qt::CaseInsensitive)) {
        // use custom def file for Qt plugins
        t << "DEFFILE " PLUGIN_COMMON_DEF_FILE_FOR_MMP << endl;
    }

    t << endl;

    return true;
}


/*
    Application registration resource files
    should be installed to the

    \private\10003a3f\import\apps directory.
*/
bool SymbianMakefileGenerator::writeMmpFileResourcePart(QTextStream& t, QStringList &symbianLangCodes) {
    if((getTargetExtension() == "exe") &&
        !project->values("CONFIG").contains("no_icon", Qt::CaseInsensitive)) {
        QString target = escapeFilePath(fileFixify(project->first("TARGET")));
        target = removePathSeparators(target);

        QString locTarget = target;
        locTarget.append(".rss");

        t << "SOURCEPATH\t\t\t. " << endl;
        t << "LANG SC ";    // no endl
        foreach(QString lang, symbianLangCodes) {
            t << lang << " "; // no endl
        }
        t << endl;
        t << "START RESOURCE\t\t" << locTarget << endl;
        t << "HEADER" << endl;
        t << "TARGETPATH\t\t\t" RESOURCE_DIRECTORY_MMP<< endl;
        t << "END" << endl << endl;

        // now append extension
        QString regTarget = target;
        regTarget.append("_reg.rss");
        // must state SOURCEPATH for resources
        // relative placement (relative to dir where .mmp located)
        // absolute placement (!RELATIVE! to EPOCROOT dir)


        t << "SOURCEPATH\t\t\t. " << endl;
        t << "START RESOURCE\t\t" << regTarget << endl;
        if (isForSymbianSbsv2())
            t << "DEPENDS " << target << ".rsg" << endl;
        t << "TARGETPATH\t\t" REGISTRATION_RESOURCE_DIRECTORY_HW << endl;
        t << "END" << endl << endl;
     }
     return true;
}

bool SymbianMakefileGenerator::writeMmpFileSystemIncludePart(QTextStream& t) {

    QDir current = QDir::current();

    for(QMap<QString, QStringList>::iterator it = systeminclude.begin(); it != systeminclude.end(); ++it) {
        QStringList values = it.value();
        for (int i = 0; i < values.size(); ++i) {
            QString handledPath = values.at(i);
            t << "SYSTEMINCLUDE" << "\t\t" << fixPathForMmp(handledPath, current) << endl;
        }
    }

    t << endl;

    return true;
}

bool SymbianMakefileGenerator::writeMmpFileIncludePart(QTextStream& t) {

    writeMmpFileSystemIncludePart(t);

    return true;
}

bool SymbianMakefileGenerator::writeMmpFileLibraryPart(QTextStream& t) {
    QStringList &libs = project->values("LIBS");
    libs << project->values("QMAKE_LIBS");

    removeDuplicatedStrings(libs);

    for(int i = 0; i < libs.size(); ++i) {
        QString lib = libs.at(i);
        // The -L flag is uninteresting, since all symbian libraries exist in the
        // same directory.
        if(lib.startsWith("-l")) {
            lib.remove(0,2);
            QString mmpStatement;
            if (lib.endsWith(".dll")) {
                lib.chop(4);
                mmpStatement = "LIBRARY\t\t";
            } else if (lib.endsWith(".lib")) {
                lib.chop(4);
                mmpStatement = "STATICLIBRARY\t";
            } else {
                // Hacky way to find out what kind of library it is. Check the
                // ARMV5 build directory for library type. We default to shared
                // library, since that is probably more common.
                QString udebStaticLibLocation(epocRoot());
                QString urelStaticLibLocation(udebStaticLibLocation);
                udebStaticLibLocation += QString("epoc32/release/armv5/udeb/%1.lib").arg(lib);
                urelStaticLibLocation += QString("epoc32/release/armv5/urel/%1.lib").arg(lib);
                if (QFile::exists(udebStaticLibLocation) || QFile::exists(urelStaticLibLocation)) {
                    mmpStatement = "STATICLIBRARY\t";
                } else {
                    mmpStatement = "LIBRARY\t\t";
                }
            }
            t << mmpStatement <<  lib << ".lib" << endl;
        }
    }

    t << endl;

    return true;
}

bool SymbianMakefileGenerator::writeMmpFileCapabilityPart(QTextStream& t) {
    if(0 != project->first("TARGET.CAPABILITY").size()) {
        QStringList &capabilities = project->values("TARGET.CAPABILITY");
        t << "CAPABILITY" << "\t\t";

        for(int i = 0; i < capabilities.size(); ++i) {
            QString cap = capabilities.at(i);
            t << cap << " ";
        }
    }
    else {
        t << "CAPABILITY" << "\t\t" << "None";
    }
    t << endl << endl;

    return true;
}

bool SymbianMakefileGenerator::writeMmpFileCompilerOptionPart(QTextStream& t) {
    QString cw, armcc;

    if(0 != project->values("QMAKE_CXXFLAGS.CW").size()) {
        cw.append(project->values("QMAKE_CXXFLAGS.CW").join(" "));
        cw.append(" ");
    }

    if(0 != project->values("QMAKE_CXXFLAGS.ARMCC").size()) {
        armcc.append(project->values("QMAKE_CXXFLAGS.ARMCC").join(" "));
        armcc.append(" ");
    }

    if(0 != project->values("QMAKE_CFLAGS.CW").size()) {
        cw.append(project->values("QMAKE_CFLAGS.CW").join(" "));
        cw.append(" ");
    }

    if(0 != project->values("QMAKE_CFLAGS.ARMCC").size()) {
        armcc.append(project->values("QMAKE_CFLAGS.ARMCC").join(" "));
        armcc.append(" ");
    }

    if(0 != project->values("QMAKE_CXXFLAGS").size()) {
        cw.append(project->values("QMAKE_CXXFLAGS").join(" "));
        cw.append(" ");
        armcc.append(project->values("QMAKE_CXXFLAGS").join(" "));
        armcc.append(" ");
    }

    if(0 != project->values("QMAKE_CFLAGS").size()) {
        cw.append(project->values("QMAKE_CFLAGS").join(" "));
        cw.append(" ");
        armcc.append(project->values("QMAKE_CFLAGS").join(" "));
        armcc.append(" ");
    }

    if (!cw.isEmpty() && cw[cw.size()-1] == ' ')
        cw.chop(1);
    if (!armcc.isEmpty() && armcc[armcc.size()-1] == ' ')
        armcc.chop(1);

    if (!cw.isEmpty())
        t << "OPTION" << '\t' << " CW " << cw <<  endl;
    if (!armcc.isEmpty())
        t << "OPTION" << '\t' << " ARMCC "<< armcc <<  endl;
    // others to come

    t <<  endl;
    return true;
}

bool SymbianMakefileGenerator::writeMmpFileBinaryVersionPart(QTextStream& t) {
    QString applicationVersion = project->first("VERSION");
    QStringList verNumList = applicationVersion.split('.');
    uint major = 0;
    uint minor = 0;
    uint patch = 0;
    bool success = false;

    if (verNumList.size() > 0) {
        major = verNumList[0].toUInt(&success);
        if (success && verNumList.size() > 1) {
            minor = verNumList[1].toUInt(&success);
            if (success && verNumList.size() > 2) {
                patch = verNumList[2].toUInt(&success);
            }
        }
    }

    QString mmpVersion;
    if (success && major <= 0xFFFF && minor <= 0xFF && patch <= 0xFF) {
        // Symbian binary version only has major and minor components, so compress
        // Qt's minor and patch values into the minor component. Since Symbian's minor
        // component is a 16 bit value, only allow 8 bits for each to avoid overflow.
        mmpVersion.append(QString::number(major))
            .append('.')
            .append(QString::number((minor << 8) + patch));
    } else {
        if (!applicationVersion.isEmpty())
            fprintf(stderr, "Invalid VERSION string: %s\n", qPrintable(applicationVersion));
        mmpVersion = "10.0"; // Default binary version for symbian is 10.0
    }

    t << "VERSION " << mmpVersion  << endl;

    return true;
}

bool SymbianMakefileGenerator::writeMmpFileRulesPart(QTextStream& t) {
    foreach(QString item, project->values("MMP_RULES")) {
        t << endl;
        // If there is no stringlist defined for a rule, use rule name directly
        // This is convenience for defining single line mmp statements
        if (project->values(item).isEmpty()) {
            t << item << endl;
        } else {
            foreach(QString itemRow, project->values(item)) {
                t << itemRow << endl;
            }
        }
    }
    return true;
}

bool SymbianMakefileGenerator::writeBldInfContent(QTextStream &t, bool addDeploymentExtension) {
    // Read user defined bld inf rules
    QMap<QString, QStringList> userBldInfRules;
    for(QMap<QString, QStringList>::iterator it = project->variables().begin(); it != project->variables().end(); ++it) {
        if (it.key().startsWith(BLD_INF_RULES_BASE)) {
            QString newKey = it.key().mid(sizeof(BLD_INF_RULES_BASE)-1);
            if (newKey.isEmpty()) {
                fprintf(stderr, "Warning: Empty BLD_INF_RULES key encountered\n");
                continue;
            }
            QStringList newValues;
            QStringList values = it.value();
            foreach(QString item, values) {
                // If there is no stringlist defined for a rule, use rule name directly
                // This is convenience for defining single line statements
                if (project->values(item).isEmpty()) {
                    newValues << item;
                } else {
                    foreach(QString itemRow, project->values(item)) {
                        newValues << itemRow;
                    }
                }
            }
            userBldInfRules.insert(newKey, newValues);
        }
    }

    // Add includes of subdirs bld.inf files

    QString mmpfilename = escapeFilePath(fileFixify(project->projectFile()));
    mmpfilename = mmpfilename.replace(mmpfilename.lastIndexOf(".")+1, 3, "mmp");
    QString currentPath = qmake_getpwd();

    if(!currentPath.endsWith(QString("/")))
        currentPath.append("/");

    QStringList mmpProjects = project->values("MMPFILES_DIRECT_DEPENDS");
    QStringList shadowProjects = project->values("SHADOW_BLD_INFS");

    removeDuplicatedStrings(mmpProjects);
    removeDuplicatedStrings(shadowProjects);

    // go in reverse order ... as that is the way how I build the list
    QListIterator<QString> iT(mmpProjects);
    iT.toBack();
    while(iT.hasPrevious()) {
        QString fullMmpName = iT.previous();
        QString relativePath;
        QString bldinfFilename;

        QString fullProFilename = fullMmpName;
        fullProFilename.replace(QString(".mmp"), QString(".pro"));
        QString uid = generate_uid(fullProFilename);

        QString cleanMmpName = fullProFilename;
        cleanMmpName.replace(QString(".pro"), QString(""));
        cleanMmpName.replace(0, cleanMmpName.lastIndexOf("/") + 1, QString(""));

        if(shadowProjects.contains(BLD_INF_FILENAME "." + cleanMmpName)) { // shadow project
            QDir directory(currentPath);
            relativePath = directory.relativeFilePath(fullProFilename);
            bldinfFilename = BLD_INF_FILENAME "." + cleanMmpName;
            if(relativePath.contains("/")) {
                // shadow .pro not in same
                // directory as parent .pro
                if(relativePath.startsWith("..")) {
                    // shadow .pro out of parent .pro
                    relativePath.replace(relativePath.lastIndexOf("/"), relativePath.length(), QString(""));
                    bldinfFilename.prepend("/").prepend(relativePath);
                } else {
                    relativePath.replace(relativePath.lastIndexOf("/"), relativePath.length(), QString(""));
                    bldinfFilename.prepend("/").prepend(relativePath);
                }
            } else {
                // shadow .pro and parent .pro in same directory
                bldinfFilename.prepend("./");
            }
        } else { // regular project
            // calc relative path
            QDir directory(currentPath);
            relativePath = directory.relativeFilePath(fullProFilename);
            relativePath.replace(relativePath.lastIndexOf("/"), relativePath.length(), QString(""));
            bldinfFilename = relativePath.append("/").append(BLD_INF_FILENAME);
        }

        QString bldinfDefine = QString("BLD_INF_") + cleanMmpName + QString("_") + uid;
        bldinfDefine = bldinfDefine.toUpper();
        removeSpecialCharacters(bldinfDefine);

        t << "#ifndef " << bldinfDefine << endl;
        t << "\t#include \"" << QDir::toNativeSeparators(bldinfFilename) << "\"" << endl;
        t << "#endif // " << bldinfDefine << endl;
    }

    // Add supported project platforms

    t << endl << BLD_INF_TAG_PLATFORMS << endl << endl;
    if(0 != project->values("SYMBIAN_PLATFORMS").size())
        t << project->values("SYMBIAN_PLATFORMS").join(" ") << endl;

    QStringList userItems = userBldInfRules.value(BLD_INF_TAG_PLATFORMS);
    foreach(QString item, userItems)
        t << item << endl;
    userBldInfRules.remove(BLD_INF_TAG_PLATFORMS);
    t << endl;

    // Add project mmps and old style extension makefiles
    QString mmpTag;
    if (project->values("CONFIG").contains("symbian_test", Qt::CaseInsensitive))
        mmpTag = QLatin1String(BLD_INF_TAG_TESTMMPFILES);
    else
        mmpTag = QLatin1String(BLD_INF_TAG_MMPFILES);

    t << endl << mmpTag << endl << endl;

    writeBldInfMkFilePart(t, addDeploymentExtension);
    if (getTargetExtension() == "subdirs") {
        mmpProjects.removeOne(mmpfilename);
    }

    if(getTargetExtension() != "subdirs") {
        QString shortProFilename = project->projectFile();
        shortProFilename.replace(0, shortProFilename.lastIndexOf("/") + 1, QString(""));
        shortProFilename.replace(QString(".pro"), QString(""));

        QString mmpFilename = shortProFilename + QString("_") + uid3 + QString(".mmp");

        t << mmpFilename << endl;
    }

    userItems = userBldInfRules.value(mmpTag);
    foreach(QString item, userItems)
        t << item << endl;
    userBldInfRules.remove(mmpTag);

    t << endl << BLD_INF_TAG_EXTENSIONS << endl << endl;

    // Generate extension rules
    writeBldInfExtensionRulesPart(t);

    userItems = userBldInfRules.value(BLD_INF_TAG_EXTENSIONS);
    foreach(QString item, userItems)
        t << item << endl;
    userBldInfRules.remove(BLD_INF_TAG_EXTENSIONS);

    // Add rest of the user defined content

    for(QMap<QString, QStringList>::iterator it = userBldInfRules.begin(); it != userBldInfRules.end(); ++it) {
        t << endl << endl << it.key() << endl << endl;
        userItems = it.value();
        foreach(QString item, userItems)
            t << item << endl;
    }

    return true;
}

bool SymbianMakefileGenerator::writeRegRssFile(QString &appName, QStringList &userItems) {
    QString filename(appName);
    filename.append("_reg.rss");
    QFile ft(filename);
    if(ft.open(QIODevice::WriteOnly)) {
        generatedFiles << ft.fileName();
		QTextStream t(&ft);
        t << "// ============================================================================" << endl;
        t << "// * Generated by qmake (" << qmake_version() << ") (Qt " << QT_VERSION_STR << ") on: ";
        t << QDateTime::currentDateTime().toString(Qt::ISODate) << endl;
        t << "// * This file is generated by qmake and should not be modified by the" << endl;
        t << "// * user." << endl;
        t << "// ============================================================================" << endl;
        t << endl;
		t << "#include <" << appName << ".rsg>" << endl;
		t << "#include <appinfo.rh>" << endl;
        t << endl;
		//t << "#include <data_caging_paths.hrh>" << "\n" << endl;
		t << "UID2 " << "KUidAppRegistrationResourceFile" << endl;
		t << "UID3 " << uid3 << endl << endl;
		t << "RESOURCE APP_REGISTRATION_INFO" << endl;
		t << "\t{" << endl;
		t << "\tapp_file=\"" << appName << "\";" << endl;
		t << "\tlocalisable_resource_file=\"" RESOURCE_DIRECTORY_RESOURCE << appName << "\";" << endl;
		t << endl;

        foreach(QString item, userItems)
            t << "\t" << item << endl;
		t << "\t}" << endl;
    } else {
        return false;
    }
    return true;
}

bool SymbianMakefileGenerator::writeRssFile(QString &appName, QString &numberOfIcons, QString &iconFile) {
    QString filename(appName);
    filename.append(".rss");
    QFile ft(filename);
    if(ft.open(QIODevice::WriteOnly)) {
        generatedFiles << ft.fileName();
		QTextStream t(&ft);
        t << "// ============================================================================" << endl;
        t << "// * Generated by qmake (" << qmake_version() << ") (Qt " << QT_VERSION_STR << ") on: ";
        t << QDateTime::currentDateTime().toString(Qt::ISODate) << endl;
        t << "// * This file is generated by qmake and should not be modified by the" << endl;
        t << "// * user." << endl;
        t << "// ============================================================================" << endl;
        t << endl;
        t << "#include <appinfo.rh>" << endl;
        t << "#include \"" << appName << ".loc\"" << endl;
        t << endl;
        t << "RESOURCE LOCALISABLE_APP_INFO r_localisable_app_info" << endl;
		t << "\t{" << endl;
        t << "\tshort_caption = STRING_r_short_caption;" << endl;
        t << "\tcaption_and_icon =" << endl;
        t << "\tCAPTION_AND_ICON_INFO" << endl;
		t << "\t\t{" << endl;
        t << "\t\tcaption = STRING_r_caption;" << endl;

        if(numberOfIcons.isEmpty() || iconFile.isEmpty() ) {
            // There can be maximum one item in this tag, validated when parsed
            t << "\t\tnumber_of_icons = 0;" << endl;
            t << "\t\ticon_file = \"\";" << endl;
        }
        else {
            // There can be maximum one item in this tag, validated when parsed
            t << "\t\tnumber_of_icons = " << numberOfIcons << ";" << endl;
            t << "\t\ticon_file = \"" << iconFile << "\";" << endl;
        }
		t << "\t\t};" << endl;
		t << "\t}" << endl;
		t << endl;
	} else {
        return false;
    }
    return true;
}

bool SymbianMakefileGenerator::writeLocFile(QString &appName, QStringList &symbianLangCodes) {
    QString filename(appName);
    filename.append(".loc");
    QFile ft(filename);
    if(ft.open(QIODevice::WriteOnly)) {
        generatedFiles << ft.fileName();
		QTextStream t(&ft);
        t << "// ============================================================================" << endl;
        t << "// * Generated by qmake (" << qmake_version() << ") (Qt " << QT_VERSION_STR << ") on: ";
        t << QDateTime::currentDateTime().toString(Qt::ISODate) << endl;
        t << "// * This file is generated by qmake and should not be modified by the" << endl;
        t << "// * user." << endl;
        t << "// ============================================================================" << endl;
        t << endl;
        t << "#ifdef LANGUAGE_SC" << endl;
        //t << "#include \"" << appName << ".l01\"" << endl;
        t << "#define STRING_r_short_caption \"" << appName  << "\"" << endl;
        t << "#define STRING_r_caption \"" << appName  << "\"" << endl;
        foreach(QString lang, symbianLangCodes) {
            t << "#elif defined LANGUAGE_" << lang << endl;
            //t << "#include \"" << appName << ".l" << lang << "\"" << endl;
            t << "#define STRING_r_short_caption \"" << appName  << "\"" << endl;
            t << "#define STRING_r_caption \"" << appName  << "\"" << endl;
        }
        t << "#else" << endl;
        t << "#define STRING_r_short_caption \"" << appName  << "\"" << endl;
        t << "#define STRING_r_caption \"" << appName  << "\"" << endl;
        t << "#endif" << endl;
    } else {
        return false;
    }
    return true;
}

void SymbianMakefileGenerator::readRssRules(QString &numberOfIcons, QString &iconFile, QStringList &userRssRules) {
    for(QMap<QString, QStringList>::iterator it = project->variables().begin(); it != project->variables().end(); ++it) {
        if (it.key().startsWith(RSS_RULES_BASE)) {
            QString newKey = it.key().mid(sizeof(RSS_RULES_BASE)-1);
            if (newKey.isEmpty()) {
                fprintf(stderr, "Warning: Empty RSS_RULES_BASE key encountered\n");
                continue;
            }
            QStringList newValues;
            QStringList values = it.value();
            foreach(QString item, values) {
                // If there is no stringlist defined for a rule, use rule name directly
                // This is convenience for defining single line statements
                if (project->values(item).isEmpty()) {
                    newValues << item;
                } else {
                    foreach(QString itemRow, project->values(item)) {
                        newValues << itemRow;
                    }
                }
            }
            // Verify thet there is exactly one value in RSS_TAG_NBROFICONS
            if (newKey == RSS_TAG_NBROFICONS) {
                if (newValues.count() == 1) {
                    numberOfIcons = newValues[0];
                } else {
                    fprintf(stderr, "Warning: There must be exactly one value in '%s%s'\n",
                        RSS_RULES_BASE, RSS_TAG_NBROFICONS);
                    continue;
                }
            // Verify thet there is exactly one value in RSS_TAG_ICONFILE
            } else if (newKey == RSS_TAG_ICONFILE) {
                if (newValues.count() == 1) {
                    iconFile = newValues[0];
                } else {
                    fprintf(stderr, "Warning: There must be exactly one value in '%s%s'\n",
                        RSS_RULES_BASE, RSS_TAG_ICONFILE);
                    continue;
                }
            } else {
                fprintf(stderr, "Warning: Unsupported key:'%s%s'\n",
                    RSS_RULES_BASE, newKey.toLatin1().constData());
                continue;
            }
        }
    }

    foreach(QString item, project->values(RSS_RULES)) {
        // If there is no stringlist defined for a rule, use rule name directly
        // This is convenience for defining single line mmp statements
        if (project->values(item).isEmpty()) {
            userRssRules << item;
        } else {
            userRssRules << project->values(item);
        }
    }

    // Validate that either both RSS_TAG_NBROFICONS and RSS_TAG_ICONFILE keys exist
    // or neither of them exist
    if ( !((numberOfIcons.isEmpty() && iconFile.isEmpty()) ||
        (!numberOfIcons.isEmpty() && !iconFile.isEmpty())) ) {
        numberOfIcons.clear();
        iconFile.clear();
        fprintf(stderr, "Warning: Both or neither of '%s%s' and '%s%s' keys must exist.\n",
            RSS_RULES_BASE, RSS_TAG_NBROFICONS, RSS_RULES_BASE, RSS_TAG_ICONFILE );
    }

    // Validate that RSS_TAG_NBROFICONS contains only numbers
    if( !numberOfIcons.isEmpty() ) {
        bool ok;
        numberOfIcons = numberOfIcons.simplified();
        int tmp = numberOfIcons.toInt(&ok);
        if (!ok) {
            numberOfIcons.clear();
            iconFile.clear();
            fprintf(stderr, "Warning: '%s%s' must be integer in decimal format.\n",
                RSS_RULES_BASE, RSS_TAG_NBROFICONS );
        }
    }
}

QStringList SymbianMakefileGenerator::symbianLangCodesFromTsFiles() {
    QStringList tsfiles;
    QStringList symbianLangCodes;
    tsfiles << project->values("TRANSLATIONS");

    fillQt2S60LangMapTable();

    foreach(QString file, tsfiles) {
        int extIndex = file.lastIndexOf(".");
        int langIndex = file.lastIndexOf("_", (extIndex - file.length()));
        langIndex += 1;
        QString qtlang = file.mid(langIndex, extIndex - langIndex );
        QString s60lang = qt2S60LangMapTable.value(qtlang, QString("SC"));

        if( !symbianLangCodes.contains(s60lang) && s60lang != "SC" )
            symbianLangCodes += s60lang;
    }

    return symbianLangCodes;
}

void SymbianMakefileGenerator::fillQt2S60LangMapTable() {
    qt2S60LangMapTable.reserve(170); // 165 items at time of writing.
    qt2S60LangMapTable.insert("ab", "SC");            //Abkhazian                     //
    qt2S60LangMapTable.insert("om", "SC");            //Afan                          //
    qt2S60LangMapTable.insert("aa", "SC");            //Afar	                         //
    qt2S60LangMapTable.insert("af", "34");            //Afrikaans	                 //Afrikaans
    qt2S60LangMapTable.insert("sq", "35");            //Albanian                      //Albanian
    qt2S60LangMapTable.insert("am", "36");            //Amharic                       //Amharic
    qt2S60LangMapTable.insert("ar", "37");            //Arabic                        //Arabic
    qt2S60LangMapTable.insert("hy", "38");            //Armenian                      //Armenian
    qt2S60LangMapTable.insert("as", "SC");            //Assamese                      //
    qt2S60LangMapTable.insert("ay", "SC");            //Aymara                        //
    qt2S60LangMapTable.insert("az", "SC");            //Azerbaijani                   //
    qt2S60LangMapTable.insert("ba", "SC");            //Bashkir                       //
    qt2S60LangMapTable.insert("eu", "SC");            //Basque                        //
    qt2S60LangMapTable.insert("bn", "41");            //Bengali                       //Bengali
    qt2S60LangMapTable.insert("dz", "SC");            //Bhutani                       //
    qt2S60LangMapTable.insert("bh", "SC");            //Bihari                        //
    qt2S60LangMapTable.insert("bi", "SC");            //Bislama                       //
    qt2S60LangMapTable.insert("br", "SC");            //Breton                        //
    qt2S60LangMapTable.insert("bg", "42");            //Bulgarian                     //Bulgarian
    qt2S60LangMapTable.insert("my", "43");            //Burmese                       //Burmese
    qt2S60LangMapTable.insert("be", "40");            //Byelorussian                  //Belarussian
    qt2S60LangMapTable.insert("km", "SC");            //Cambodian                     //
    qt2S60LangMapTable.insert("ca", "44");            //Catalan                       //Catalan
    qt2S60LangMapTable.insert("zh", "SC");            //Chinese                       //
    qt2S60LangMapTable.insert("co", "SC");            //Corsican                      //
    qt2S60LangMapTable.insert("hr", "45");            //Croatian                      //Croatian
    qt2S60LangMapTable.insert("cs", "25");            //Czech                         //Czech
    qt2S60LangMapTable.insert("da", "07");            //Danish                        //Danish
    qt2S60LangMapTable.insert("nl", "18");            //Dutch                         //Dutch
    qt2S60LangMapTable.insert("en", "01");            //English                       //English(UK)
    qt2S60LangMapTable.insert("eo", "SC");            //Esperanto                     //
    qt2S60LangMapTable.insert("et", "49");            //Estonian                      //Estonian
    qt2S60LangMapTable.insert("fo", "SC");            //Faroese                       //
    qt2S60LangMapTable.insert("fj", "SC");            //Fiji                          //
    qt2S60LangMapTable.insert("fi", "09");            //Finnish                       //Finnish
    qt2S60LangMapTable.insert("fr", "02");            //French                        //French
    qt2S60LangMapTable.insert("fy", "SC");            //Frisian                       //
    qt2S60LangMapTable.insert("gd", "52");            //Gaelic                        //Gaelic
    qt2S60LangMapTable.insert("gl", "SC");            //Galician                      //
    qt2S60LangMapTable.insert("ka", "53");            //Georgian                      //Georgian
    qt2S60LangMapTable.insert("de", "03");            //German                        //German
    qt2S60LangMapTable.insert("el", "54");            //Greek                         //Greek
    qt2S60LangMapTable.insert("kl", "SC");            //Greenlandic                   //
    qt2S60LangMapTable.insert("gn", "SC");            //Guarani                       //
    qt2S60LangMapTable.insert("gu", "56");            //Gujarati                      //Gujarati
    qt2S60LangMapTable.insert("ha", "SC");            //Hausa                         //
    qt2S60LangMapTable.insert("he", "57");            //Hebrew                        //Hebrew
    qt2S60LangMapTable.insert("hi", "58");            //Hindi                         //Hindi
    qt2S60LangMapTable.insert("hu", "17");            //Hungarian                     //Hungarian
    qt2S60LangMapTable.insert("is", "15");            //Icelandic                     //Icelandic
    qt2S60LangMapTable.insert("id", "59");            //Indonesian                    //Indonesian
    qt2S60LangMapTable.insert("ia", "SC");            //Interlingua                   //
    qt2S60LangMapTable.insert("ie", "SC");            //Interlingue                   //
    qt2S60LangMapTable.insert("iu", "SC");            //Inuktitut                     //
    qt2S60LangMapTable.insert("ik", "SC");            //Inupiak                       //
    qt2S60LangMapTable.insert("ga", "60");            //Irish                         //Irish
    qt2S60LangMapTable.insert("it", "05");            //Italian                       //Italian
    qt2S60LangMapTable.insert("ja", "32");            //Japanese                      //Japanese
    qt2S60LangMapTable.insert("jv", "SC");            //Javanese                      //
    qt2S60LangMapTable.insert("kn", "62");            //Kannada                       //Kannada
    qt2S60LangMapTable.insert("ks", "SC");            //Kashmiri                      //
    qt2S60LangMapTable.insert("kk", "63");            //Kazakh                        //Kazakh
    qt2S60LangMapTable.insert("rw", "SC");            //Kinyarwanda                   //
    qt2S60LangMapTable.insert("ky", "SC");            //Kirghiz                       //
    qt2S60LangMapTable.insert("ko", "65");            //Korean                        //Korean
    qt2S60LangMapTable.insert("ku", "SC");            //Kurdish                       //
    qt2S60LangMapTable.insert("rn", "SC");            //Kurundi                       //
    qt2S60LangMapTable.insert("lo", "66");            //Laothian                      //Laothian
    qt2S60LangMapTable.insert("la", "SC");            //Latin                         //
    qt2S60LangMapTable.insert("lv", "67");            //Latvian                       //Latvian
    qt2S60LangMapTable.insert("ln", "SC");            //Lingala                       //
    qt2S60LangMapTable.insert("lt", "68");            //Lithuanian                    //Lithuanian
    qt2S60LangMapTable.insert("mk", "69");            //Macedonian                    //Macedonian
    qt2S60LangMapTable.insert("mg", "SC");            //Malagasy                      //
    qt2S60LangMapTable.insert("ms", "70");            //Malay                         //Malay
    qt2S60LangMapTable.insert("ml", "71");            //Malayalam                     //Malayalam
    qt2S60LangMapTable.insert("mt", "SC");            //Maltese                       //
    qt2S60LangMapTable.insert("mi", "SC");            //Maori                         //
    qt2S60LangMapTable.insert("mr", "72");            //Marathi                       //Marathi
    qt2S60LangMapTable.insert("mo", "73");            //Moldavian                     //Moldovian
    qt2S60LangMapTable.insert("mn", "74");            //Mongolian                     //Mongolian
    qt2S60LangMapTable.insert("na", "SC");            //Nauru                         //
    qt2S60LangMapTable.insert("ne", "SC");            //Nepali                        //
    qt2S60LangMapTable.insert("nb", "08");            //Norwegian                     //Norwegian
    qt2S60LangMapTable.insert("oc", "SC");            //Occitan                       //
    qt2S60LangMapTable.insert("or", "SC");            //Oriya                         //
    qt2S60LangMapTable.insert("ps", "SC");            //Pashto                        //
    qt2S60LangMapTable.insert("fa", "SC");            //Persian                       //
    qt2S60LangMapTable.insert("pl", "27");            //Polish                        //Polish
    qt2S60LangMapTable.insert("pt", "13");            //Portuguese                    //Portuguese
    qt2S60LangMapTable.insert("pa", "77");            //Punjabi                       //Punjabi
    qt2S60LangMapTable.insert("qu", "SC");            //Quechua                       //
    qt2S60LangMapTable.insert("rm", "SC");            //RhaetoRomance                 //
    qt2S60LangMapTable.insert("ro", "78");            //Romanian                      //Romanian
    qt2S60LangMapTable.insert("ru", "16");            //Russian                       //Russian
    qt2S60LangMapTable.insert("sm", "SC");            //Samoan                        //
    qt2S60LangMapTable.insert("sg", "SC");            //Sangho                        //
    qt2S60LangMapTable.insert("sa", "SC");            //Sanskrit                      //
    qt2S60LangMapTable.insert("sr", "79");            //Serbian                       //Serbian
    qt2S60LangMapTable.insert("sh", "SC");            //SerboCroatian                 //
    qt2S60LangMapTable.insert("st", "SC");            //Sesotho                       //
    qt2S60LangMapTable.insert("tn", "SC");            //Setswana                      //
    qt2S60LangMapTable.insert("sn", "SC");            //Shona                         //
    qt2S60LangMapTable.insert("sd", "SC");            //Sindhi                        //
    qt2S60LangMapTable.insert("si", "80");            //Singhalese                    //Sinhalese
    qt2S60LangMapTable.insert("ss", "SC");            //Siswati                       //
    qt2S60LangMapTable.insert("sk", "26");            //Slovak                        //Slovak
    qt2S60LangMapTable.insert("sl", "28");            //Slovenian                     //Slovenian
    qt2S60LangMapTable.insert("so", "81");            //Somali                        //Somali
    qt2S60LangMapTable.insert("es", "04");            //Spanish                       //Spanish
    qt2S60LangMapTable.insert("su", "SC");            //Sundanese                     //
    qt2S60LangMapTable.insert("sw", "84");            //Swahili                       //Swahili
    qt2S60LangMapTable.insert("sv", "06");            //Swedish                       //Swedish
    qt2S60LangMapTable.insert("tl", "39");            //Tagalog                       //Tagalog
    qt2S60LangMapTable.insert("tg", "SC");            //Tajik                         //
    qt2S60LangMapTable.insert("ta", "87");            //Tamil                         //Tamil
    qt2S60LangMapTable.insert("tt", "SC");            //Tatar                         //
    qt2S60LangMapTable.insert("te", "88");            //Telugu                        //Telugu
    qt2S60LangMapTable.insert("th", "33");            //Thai                          //Thai
    qt2S60LangMapTable.insert("bo", "89");            //Tibetan                       //Tibetan
    qt2S60LangMapTable.insert("ti", "90");            //Tigrinya                      //Tigrinya
    qt2S60LangMapTable.insert("to", "SC");            //Tonga                         //
    qt2S60LangMapTable.insert("ts", "SC");            //Tsonga                        //
    qt2S60LangMapTable.insert("tr", "14");            //Turkish                       //Turkish
    qt2S60LangMapTable.insert("tk", "92");            //Turkmen                       //Turkmen
    qt2S60LangMapTable.insert("tw", "SC");            //Twi                           //
    qt2S60LangMapTable.insert("ug", "SC");            //Uigur                         //
    qt2S60LangMapTable.insert("uk", "93");            //Ukrainian                     //Ukrainian
    qt2S60LangMapTable.insert("ur", "94");            //Urdu                          //Urdu
    qt2S60LangMapTable.insert("uz", "SC");            //Uzbek                         //
    qt2S60LangMapTable.insert("vi", "96");            //Vietnamese                    //Vietnamese
    qt2S60LangMapTable.insert("vo", "SC");            //Volapuk                       //
    qt2S60LangMapTable.insert("cy", "97");            //Welsh                         //Welsh
    qt2S60LangMapTable.insert("wo", "SC");            //Wolof                         //
    qt2S60LangMapTable.insert("xh", "SC");            //Xhosa                         //
    qt2S60LangMapTable.insert("yi", "SC");            //Yiddish                       //
    qt2S60LangMapTable.insert("yo", "SC");            //Yoruba                        //
    qt2S60LangMapTable.insert("za", "SC");            //Zhuang                        //
    qt2S60LangMapTable.insert("zu", "98");            //Zulu                          //Zulu
    qt2S60LangMapTable.insert("nn", "75");            //Nynorsk                       //NorwegianNynorsk
    qt2S60LangMapTable.insert("bs", "SC");            //Bosnian                       //
    qt2S60LangMapTable.insert("dv", "SC");            //Divehi                        //
    qt2S60LangMapTable.insert("gv", "SC");            //Manx                          //
    qt2S60LangMapTable.insert("kw", "SC");            //Cornish                       //
    qt2S60LangMapTable.insert("ak", "SC");            //Akan                          //
    qt2S60LangMapTable.insert("kok", "SC");           //Konkani                       //
    qt2S60LangMapTable.insert("gaa", "SC");           //Ga                            //
    qt2S60LangMapTable.insert("ig", "SC");            //Igbo                          //
    qt2S60LangMapTable.insert("kam", "SC");           //Kamba                         //
    qt2S60LangMapTable.insert("syr", "SC");           //Syriac                        //
    qt2S60LangMapTable.insert("byn", "SC");           //Blin                          //
    qt2S60LangMapTable.insert("gez", "SC");           //Geez                          //
    qt2S60LangMapTable.insert("kfo", "SC");           //Koro                          //
    qt2S60LangMapTable.insert("sid", "SC");           //Sidamo                        //
    qt2S60LangMapTable.insert("cch", "SC");           //Atsam                         //
    qt2S60LangMapTable.insert("tig", "SC");           //Tigre                         //
    qt2S60LangMapTable.insert("kaj", "SC");           //Jju                           //
    qt2S60LangMapTable.insert("fur", "SC");           //Friulian	                     //
    qt2S60LangMapTable.insert("ve", "SC");            //Venda	                     //
    qt2S60LangMapTable.insert("ee", "SC");            //Ewe	                         //
    qt2S60LangMapTable.insert("wa", "SC");            //Walamo	                     //
    qt2S60LangMapTable.insert("haw", "SC");           //Hawaiian	                     //
    qt2S60LangMapTable.insert("kcg", "SC");           //Tyap	                         //
    qt2S60LangMapTable.insert("ny", "SC");            //Chewa	                     //
}

void SymbianMakefileGenerator::appendIfnotExist(QStringList &list, QString value)
{
    if(!list.contains(value))
        list += value;
}

void SymbianMakefileGenerator::appendIfnotExist(QStringList &list, QStringList values)
{
    foreach(QString item, values)
        appendIfnotExist(list, item);
}

QString SymbianMakefileGenerator::removePathSeparators(QString &file)
{
    QString ret = file;
    while(ret.indexOf(QDir::separator()) > 0) {
        ret.remove(0, ret.indexOf(QDir::separator())+1);
    }

    return ret;
}


QString SymbianMakefileGenerator::removeTrailingPathSeparators(QString &file)
{
    QString ret = file;
    if(ret.endsWith(QDir::separator())) {
        ret.remove(ret.length()-1,1);
    }

    return ret;
}

bool SymbianMakefileGenerator::generateCleanCommands(QTextStream& t,
                                                     const QStringList& toClean,
                                                     const QString& cmd,
                                                     const QString& cmdOptions,
                                                     const QString& itemPrefix,
                                                     const QString& itemSuffix) {
    for (int i = 0; i < toClean.size(); ++i) {
        QString item = toClean.at(i);
        item.prepend(itemPrefix).append(itemSuffix);
#if defined(Q_OS_WIN)
        t << "\t-@ if EXIST \"" << QDir::toNativeSeparators(item) << "\" ";
        t << cmd << " " << cmdOptions << " \"" << QDir::toNativeSeparators(item) << "\"" << endl;
#else
        t << "\t-if test -f " << QDir::toNativeSeparators(item) << "; then ";
        t << cmd << " " << cmdOptions << " " << QDir::toNativeSeparators(item) << "; fi" << endl;
#endif
    }

    return true;
}

QString SymbianMakefileGenerator::getWithoutSpecialCharacters(QString& str) {
    QString tmp = str;

    tmp.replace(QString("/"), QString("_"));
    tmp.replace(QString("\\"), QString("_"));
    tmp.replace(QString("-"), QString("_"));
    tmp.replace(QString(":"), QString("_"));
    tmp.replace(QString("."), QString("_"));

    return tmp;
}

void SymbianMakefileGenerator::removeSpecialCharacters(QString& str) {
    str.replace(QString("/"), QString("_"));
    str.replace(QString("\\"), QString("_"));
    str.replace(QString("-"), QString("_"));
    str.replace(QString(":"), QString("_"));
    str.replace(QString("."), QString("_"));
    str.replace(QString(" "), QString("_"));
}
