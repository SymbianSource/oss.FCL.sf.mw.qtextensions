#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
# All rights reserved.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, version 2.1 of the License.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, 
# see "http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html/".
#
# Description:
#

TARGET = xqecom
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

QT -= gui
QT -= core

QPRO_PWD   = $$PWD


DESTDIR = /epoc32/tools/
#$$[QT_INSTALL_BINS]$${DIR_SEPARATOR}
#QTSOURCEDIR = $$[QT_INSTALL_BINS]$${DIR_SEPARATOR}..$${DIR_SEPARATOR}src
#QTINCLUDEDIR = $$[QT_INSTALL_BINS]$${DIR_SEPARATOR}..$${DIR_SEPARATOR}include

QTSOURCEDIR = $${DIR_SEPARATOR}sf$${DIR_SEPARATOR}mw$${DIR_SEPARATOR}qt$${DIR_SEPARATOR}src
QTINCLUDEDIR = $${DIR_SEPARATOR}sf$${DIR_SEPARATOR}mw$${DIR_SEPARATOR}qt$${DIR_SEPARATOR}include


# QT_BUILD_QMAKE QT_BOOTSTRAPPED

DEFINES += 	QT_NO_TEXTCODEC QT_NO_UNICODETABLES QT_LITE_COMPONENT QT_NO_PCRE \
	       	  QT_NODLL QT_NO_STL QT_NO_COMPRESS HAVE_QCONFIG_CPP \
	       	  QT_NO_THREAD QT_NO_QOBJECT QT_NO_GEOM_VARIANT QT_NO_DATASTREAM \
		        QT_NO_LIBRARY QT_BOOTSTRAPPED

VPATH += $$QTSOURCEDIR/corelib/global \
         $$QTSOURCEDIR/corelib/tools \
         $$QTSOURCEDIR/corelib/codecs \
         $$QTSOURCEDIR/corelib/kernel \
         $$QTSOURCEDIR/corelib/plugin \
				 $$QTSOURCEDIR/corelib/io \
	       $$QTSOURCEDIR/corelib/xml \
	       $$QTSOURCEDIR/script \
		   c:/MinGW/include/

INCPATH += $$QTINCLUDEDIR $$QTINCLUDEDIR/QtCore

SOURCES += main.cpp 

#QT sources    			 
SOURCES+= \
      qbitarray.cpp \
      qbuffer.cpp \
      qbytearray.cpp \
      qbytearraymatcher.cpp \
      qcryptographichash.cpp \
      qdatetime.cpp \
      qdir.cpp \
      qdiriterator.cpp \
      qfile.cpp \
      qabstractfileengine.cpp \
      qfileinfo.cpp \
      qfsfileengine.cpp \
      qfsfileengine_win.cpp \
      qfsfileengine_iterator.cpp \
      qfsfileengine_iterator_win.cpp \
      qglobal.cpp \
      qnumeric.cpp \
      qhash.cpp \
      qiodevice.cpp \
      qlist.cpp \	  
	  qutfcodec.cpp \
      qlinkedlist.cpp \
      qlocale.cpp \
      qmalloc.cpp \
      qmap.cpp \
      qmetatype.cpp \
      qregexp.cpp \
      qstring.cpp \
      qstringlist.cpp \
      qtemporaryfile.cpp \
      qtextstream.cpp \
      qurl.cpp \
#      quuid.cpp \
      qvariant.cpp \
      qvector.cpp \
      qvsnprintf.cpp 

   HEADERS+= \
        qbitarray.h \
        qbytearray.h \
        qbytearraymatcher.h \
        qchar.h \
        qcryptographichash.h \
        qdatetime.h \
        qdatetime_p.h \
        qdir.h \
        qdiriterator.h \
        qfile.h \
        qabstractfileengine.h \
        qabstractfileengine_p.h \
	    qfsfileengine_p.h \
	    qfsfileengine_iterator_p.h \
        qfileinfo.h \
        qfileinfo_p.h \
        qglobal.h \
        qnumeric.h \
        qhash.h \
        qiodevice.h \
        qlist.h \
        qlinkedlist.h \
        qlocale.h \
        qmap.h \
        qmetatype.h \
        qregexp.h \
        qstring.h \
        qstringlist.h \
        qstringmatcher.h \
        qtemporaryfile.h \
        qtextstream.h \
        qurl.h \
        quuid.h \
        qvector.h \
      
DEFINES *= QT_NO_QOBJECT
    