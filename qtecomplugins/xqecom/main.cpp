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

#include <QString>
#include <QStringList>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QDateTime>
#include <QFileInfo>

#define IMPLEMENTATION_UID_LIT "KQtEcomPluginImplementationUid"

void runRSSFileGenerator(QStringList params)
{
    QString appName = params[1];
	QString uid3 = params[2];
	QString interfacename = params[3];
	QString configuration = params[4];
	QString configurationFile = params[5];
	
	QString rssFilename = appName;
    rssFilename.append(QString::fromLatin1(".rss"));

    QFile ft(rssFilename);
	if(!ft.exists()){
		if(ft.open(QIODevice::WriteOnly)) {
			QTextStream t(&ft);

			t << "// ==============================================================================" << endl;
	        t << "// Generated by xqecom on " << QDateTime::currentDateTime().toString() << endl;
	        t << "// This file is generated by xqecom and should not be modified by the user." << endl;
			t << "//  Name        : " << rssFilename << endl;
			t << "//  Part of     : " << endl;
			t << "//  Description : Fixes common plugin symbols to known ordinals" << endl;
			t << "//  Version     : " << endl;
			t << "//" << endl;
			t << "// ==============================================================================" << "\n" << endl;
			t << endl;

	        if (interfacename.isEmpty()) {
	            interfacename = QString(appName).append(QString::fromLatin1(".dll"));
	        }
			
	        QTextStream configStream;
	        QFile cfgFile;
			
			if (!configurationFile.isEmpty()) {
				cfgFile.setFileName(configurationFile);
				if (cfgFile.open(QIODevice::ReadOnly)) {
					configStream.setDevice( &cfgFile );
				} else {
					printf("Not generated: ");
					printf((QFileInfo(ft).absoluteFilePath()).toLocal8Bit().data() );
					printf(" - Cannot open file: %s\n", configurationFile.toLocal8Bit().data());						
					ft.remove();
					return;
				}
	        } else {
				configStream.setString( &configuration, QIODevice::ReadOnly );
	        }

			
	        t << "#include <registryinfov2.rh>" << endl << endl;
	        t << "#include <xqtecom.hrh>" << endl << endl;
	        t << "#include <ecomstub_" << uid3 << ".hrh>" << endl << endl;
	        t << "RESOURCE REGISTRY_INFO theInfo" << endl << "{" << endl;
	        t << "resource_format_version = RESOURCE_FORMAT_VERSION_2;" << endl;
	        t << "dll_uid = " << uid3 << ";" << endl;
	        t << "interfaces =" << endl << "\t{" << endl << "\tINTERFACE_INFO" << endl << "\t\t{" << endl;
	        t << "\t\tinterface_uid = KQtEcomPluginInterfaceUID;" << endl;
	        t << "\t\timplementations =" << endl << "\t\t\t{" << endl;
	        t << "\t\t\tIMPLEMENTATION_INFO" << endl << "\t\t\t\t{" << endl;
	        t << "\t\t\t\timplementation_uid = " << IMPLEMENTATION_UID_LIT << ";" << endl;
	        t << "\t\t\t\tversion_no = 1;" << endl;
	        t << "\t\t\t\tdisplay_name = \"" << appName+QString(QString::fromLatin1(".dll")) << "\";" << endl;
	        t << "\t\t\t\t// SERVICE.INTERFACE_NAME" << endl;
	        t << "\t\t\t\tdefault_data = \"" << interfacename << "\";" << endl;
	        t << "\t\t\t\t// SERVICE.CONFIGURATION" << endl;
	        t << "\t\t\t\topaque_data = \""; 
			while( !configStream.atEnd() ) {
				configStream.skipWhiteSpace();
				t << configStream.readLine( 255 ).replace(QString::fromLatin1("\""), QString::fromLatin1("\\\""));
			};
			t << "\";" << endl;
	        t << "\t\t\t\t}" << endl << "\t\t\t};" << endl << "\t\t}" << endl << "\t};" << endl << "}" << endl;
			printf("Generated: ");
			printf((QFileInfo(ft).absoluteFilePath()).toLocal8Bit().data() );
			printf("\n");
	    }else{
			printf("Not generated: ");
			printf((QFileInfo(ft).absoluteFilePath()).toLocal8Bit().data() );
			printf(" - Error %d\n",ft.error());
		}
	}else{
		printf("Not generated: ");
		printf((QFileInfo(ft).absoluteFilePath()).toLocal8Bit().data() );
		printf(" - File exists\n");					
	}
}

void runXQPkgGenerator(QStringList params)
{
    QString appName = params[1];
    QString uid3=params[2];
	
    QString outputFileName=appName+QString(QString::fromLatin1(".pkg"));

    QFile ft(outputFileName);
	if(!ft.exists()){
	    if(ft.open(QIODevice::WriteOnly)) {
	        QTextStream t(&ft);
	        t << "// ============================================================================" << endl;
	        t << "// Generated by xqecom on " << QDateTime::currentDateTime().toString() << endl;
	        t << "// This file is generated by xqecom and should not be modified by the user." << endl;
	        t << "// ============================================================================" << endl;
	        t << endl;
	        t << "; Language" << endl;
	        t << "&EN" << endl;
	        t << endl;
	        t << "; SIS header: name, uid, version" << endl;
	        t << "#{"<< appName <<"},(" << uid3 << "),1,0,0" << endl;
	        t << endl;
	        t << "; Localised Vendor name" << endl;
	        t << "%{\"Nokia, Qt Software\"}" << endl;
	        t << endl;
	        t << "; Unique Vendor name" << endl;
	        t << ":\"Nokia, Qt Software\"" << endl;
	        t << endl;
	        t << "; Dependencies" << endl;
	        t << "[0x101F7961],0,0,0,{\"S60ProductID\"}" << endl;
	        t << "[0x102032BE],0,0,0,{\"S60ProductID\"}" << endl;
	        t << "[0x102752AE],0,0,0,{\"S60ProductID\"}" << endl;
	        t << "[0x1028315F],0,0,0,{\"S60ProductID\"}" << endl;
	        t << "(0x2001E61C), 4, 5, 0, {\"QtLibs pre-release\"}" << endl;
	        t << endl;
	        t << ";files" << endl;
	        t << "\"\\epoc32\\release\\armv5\\urel\\"<< appName <<".dll\"    - \"!:\\sys\\bin\\"<< appName <<".dll\"" << endl;
	        t << "\"\\epoc32\\data\\Z\\resource\\plugins\\"<< appName <<".rsc\" - \"!:\\resource\\plugins\\"<< appName <<".rsc\"" << endl;
			printf("Generated: ");
			printf((QFileInfo(ft).absoluteFilePath()).toLocal8Bit().data() );
			printf("\n");
	    }else{
			printf("Not generated: ");
			printf((QFileInfo(ft).absoluteFilePath()).toLocal8Bit().data() );
			printf(" - Error %d\n",ft.error());
		}
	}else{
		printf("Not generated: ");
		printf((QFileInfo(ft).absoluteFilePath()).toLocal8Bit().data() );
		printf(" - File exists\n");					
	}
}

void runXQIbyGenerator(QStringList params)
{
    QString appName = params[1];
	
    QString outputFileName=appName+QString(QString::fromLatin1(".iby"));
	QString headerGuard = appName+QString(QString::fromLatin1("_IBY"));
	headerGuard=headerGuard.toUpper();

    QFile ft(outputFileName);
	if(!ft.exists()){
	    if(ft.open(QIODevice::WriteOnly)) {
	        QTextStream t(&ft);
	        t << "// ============================================================================" << endl;
	        t << "// Generated by xqecom on " << QDateTime::currentDateTime().toString() << endl;
	        t << "// This file is generated by xqecom and should not be modified by the user." << endl;
	        t << "// ============================================================================" << endl;
	        t << endl;
	        t << "#ifndef " << headerGuard << endl;
	        t << "#define " << headerGuard << endl;
	        t << endl;
	        t << "#include <bldvariant.hrh>" << endl;
	        t << endl;
			t << "ECOM_PLUGIN( " << appName << ".dll, " << appName << ".rsc )" << endl;
	        t << endl;
	        t << "#endif //" << headerGuard << endl;
			printf("Generated: ");
			printf((QFileInfo(ft).absoluteFilePath()).toLocal8Bit().data() );
			printf("\n");
	    }else{
			printf("Not generated: ");
			printf((QFileInfo(ft).absoluteFilePath()).toLocal8Bit().data() );
			printf(" - Error %d\n",ft.error());
		}
	}else{
		printf("Not generated: ");
		printf((QFileInfo(ft).absoluteFilePath()).toLocal8Bit().data() );
		printf(" - File exists\n");					
	}
}

void runXQStubGenerator(QStringList params)
{
    QString appName = params[1];
    QString uid3 = params[2];	
	
	QString outputHeaderFileName=QString::fromLatin1("ecomstub_") + uid3 + QString::fromLatin1(".hrh");
	QString outputSourceFileName=QString::fromLatin1("ecomstub_") + uid3 + QString::fromLatin1(".cpp");
	QString headerGuard = QString::fromLatin1("ECOMSTUB_%1_HRH").arg(uid3);

    QFile fth(outputHeaderFileName);
	if(!fth.exists()){
	    if(fth.open(QIODevice::WriteOnly)) {
	        QTextStream t(&fth);
	        t << "// ============================================================================" << endl;
	        t << "// Generated by xqecom on " << QDateTime::currentDateTime().toString() << endl;
	        t << "// This file is generated by xqecom and should not be modified by the user." << endl;
	        t << "// ============================================================================" << endl;
	        t << endl;
			t << "#ifndef " << headerGuard << endl;
			t << "#define " << headerGuard << endl;
			
			t << "#define " << IMPLEMENTATION_UID_LIT << " " << uid3 << endl;
			t << "#endif //" << headerGuard << endl;
			printf("Generated: ");
			printf((QFileInfo(fth).absoluteFilePath()).toLocal8Bit().data() );
			printf("\n");
	    }else{
			printf("Not generated: ");
			printf((QFileInfo(fth).absoluteFilePath()).toLocal8Bit().data() );
			printf(" - Error %d\n",fth.error());
		}
	}else{
		printf("Not generated: ");
		printf((QFileInfo(fth).absoluteFilePath()).toLocal8Bit().data() );
		printf(" - File exists\n");					
	}
	
    QFile ft(outputSourceFileName);
	if(!ft.exists()){
	    if(ft.open(QIODevice::WriteOnly)) {
	        QTextStream t(&ft);
	        t << "// ============================================================================" << endl;
	        t << "// Generated by xqecom on " << QDateTime::currentDateTime().toString() << endl;
	        t << "// This file is generated by xqecom and should not be modified by the user." << endl;
	        t << "// ============================================================================" << endl;
	        t << endl;
	        
			t << "#include <xqplugin.h>" << endl;
	        t << "#include <" << outputHeaderFileName << ">" << endl;
	        t << "#include <ecom/implementationproxy.h>" << endl;
	        t << "XQ_PLUGIN_ECOM_HEADER(" << appName << ")" << endl;
	        //t << "const TImplementationProxy implementationTable[] = \n\t{\n\tIMPLEMENTATION_PROXY_ENTRY( " << IMPLEMENTATION_UID_LIT << ", C" 
			
			t << "const TImplementationProxy implementationTable[] = \n\t{\n\tIMPLEMENTATION_PROXY_ENTRY(" << uid3 << ", C" 
			  << appName
	          <<"Factory::NewL)\n\t};" << endl << endl;
			  
	        t << "EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)"
	          << endl <<"\t{\n\taTableCount = sizeof( implementationTable ) / sizeof( TImplementationProxy );"
	          << endl <<"\treturn implementationTable;"
	          << endl <<"\t}" << endl;
			printf("Generated: ");
			printf((QFileInfo(ft).absoluteFilePath()).toLocal8Bit().data() );
			printf("\n");
	    }else{
			printf("Not generated: ");
			printf((QFileInfo(ft).absoluteFilePath()).toLocal8Bit().data() );
			printf(" - Error %d\n",ft.error());
		}
	}else{
		printf("Not generated: ");
		printf((QFileInfo(ft).absoluteFilePath()).toLocal8Bit().data() );
		printf(" - File exists\n");					
	}
}

int main(int argc, char *argv[])
{
    QStringList params;
    for (int i=0 ; i < argc ; i++) {
        params << QString::fromLatin1(argv[i]);
//		printf(argv[i]);
//		printf("\n");
//		fprintf(stderr, "%s\n",argv[i]);
   }
	if (params.count() < 6) {
        fprintf(stderr, "xqecom error: invalid parameters(%d)! ",params.count());
        fprintf(stderr, "Usage: xqecom <app name> <uid3> <interface name> <configuration> <configuration-filename>\n");
        return 1;
    }
	
	if ( !params[2].startsWith(QString::fromLatin1("0x")) ){ //fix uid if incorrect
		params[2].insert( 0, QString::fromLatin1("0x"));
	}

    printf("xqecom:\n");
	runRSSFileGenerator(params);
	runXQPkgGenerator(params);
	runXQIbyGenerator(params);
	runXQStubGenerator(params);
	
	printf("\n");	
	return 0;
}
