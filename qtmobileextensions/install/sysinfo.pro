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

TEMPLATE = subdirs
SUBDIRS = ../src/sysinfo
    
symbian: {
    load(data_caging_paths)
    VERSION=0.1.0

    qtmobileextension.sources = xqsysinfo.dll
    qtmobileextension.path = /sys/bin
    qtmobileextension.depends = "(0x2001e61c), 4, 5, 0, {\"QtLibs pre-release\"}"

    license.depends = "\"install.txt\" - \"\", FILETEXT, TEXTEXIT"
    DEPLOYMENT += license qtmobileextension
}