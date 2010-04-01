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

# All paths used in this file end with the path delimiter '/'

include $(EPOCROOT)epoc32/tools/shell/$(notdir $(basename $(SHELL))).mk

EPOC_ROOT := $(subst \,/,$(EPOCROOT))
QT_ROOT := $(EPOC_ROOT)epoc32/tools/qt/
QTMOBILITY_ROOT := $(subst group,,$(subst \,/,$(EXTENSION_ROOT)qtmobility/))

# Determine which platform we are building on
ifeq ($(OSTYPE),unix)
DOTBAT :=
else
QTMOBILITY_ROOT:= $(subst /,\,$(QTMOBILITY_ROOT))
QT_ROOT:= $(subst /,\,$(QT_ROOT))
DOTBAT := .bat
endif

QTMOBILITY:
	echo Configuring QtMobility
	echo EPOCROOT $(EPOCROOT)
	echo EPOC_ROOT $(EPOC_ROOT)
	echo EXTENSION_ROOT $(EXTENSION_ROOT)
	echo QT_ROOT $(QT_ROOT)
	echo QTMOBILITY_ROOT $(QTMOBILITY_ROOT)
	echo DOTBAT $(DOTBAT)
	echo MODULES $(MODULES)
	echo TESTS $(TESTS)
	echo TESTS $(TESTS)
	echo DOCS $(DOCS)
	cd $(QTMOBILITY_ROOT) && $(QTMOBILITY_ROOT)configure$(DOTBAT) -qt $(QT_ROOT) -prefix $(QTMOBILITY_ROOT) -modules "$(MODULES)" $(TESTS) $(EXAMPLES) $(DOCS)
	
do_nothing:
	
MAKMAKE : do_nothing

BLD : QTMOBILITY
	
FINAL : do_nothing

CLEAN : do_nothing

RELEASABLES :  do_nothing 

SAVESPACE : BLD

FREEZE : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : do_nothing



