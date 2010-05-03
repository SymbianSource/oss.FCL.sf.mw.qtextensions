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

INCPATH += $$QTINCLUDEDIR $$QTINCLUDEDIR/QtCore

SOURCES += main.cpp 

include(../bootstrap/bootstrap.pri)
      
DEFINES *= QT_NO_QOBJECT
    