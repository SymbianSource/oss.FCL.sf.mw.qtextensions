#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description: 
#
#

TEMPLATE = app
TARGET = test_xqkeycapture


INCLUDEPATH += . 
INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE \
               /epoc32/include/mw/qt \
               /epoc32/include/mw/qttest

TARGET.CAPABILITY = ALL -TCB

# Input
# LIBS += 

DEFINES += _XQKEYCAPTURE_UNITTEST_

SOURCES += test_xqkeycapture.cpp \
            mytestwindowgroup.cpp

HEADERS += mytestwindowgroup.h

HEADERS += \
  ../xqkeycapture.h \
  ../keycapture_p.h \
  ../txlogger.h

SOURCES += \
  ../xqkeycapture.cpp
  
symbian { 
    HEADERS += ../keycapture_s60_p.h \
            ../capturerequest_s60.h
    SOURCES +=../keycapture_s60_p.cpp \
            ../capturerequest_s60.cpp
} else { 
    HEADERS += ../keycapture_stub_p.h
    SOURCES += ./keycapture_stub_p.cpp
}


CONFIG += qtestlib console

