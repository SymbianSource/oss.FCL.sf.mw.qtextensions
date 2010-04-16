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
CONFIG += ordered
SUBDIRS = src/settingsmanager \
          src/sysinfo \
          src/utils \
          src/keycapture

symbian {
BLD_INF_RULES.prj_exports += "./src/settingsmanager/settingsmanager_global.h               /epoc32/include/mw/settingsmanager_global.h"
BLD_INF_RULES.prj_exports += "./src/settingsmanager/xqcentralrepositorysearchcriteria.h    /epoc32/include/mw/xqcentralrepositorysearchcriteria.h"
BLD_INF_RULES.prj_exports += "./include/XQCentralRepositorySearchCriteria                  /epoc32/include/mw/XQCentralRepositorySearchCriteria"
BLD_INF_RULES.prj_exports += "./src/settingsmanager/xqcentralrepositoryutils.h             /epoc32/include/mw/xqcentralrepositoryutils.h"
BLD_INF_RULES.prj_exports += "./include/XQCentralRepositoryUtils                           /epoc32/include/mw/XQCentralRepositoryUtils"
BLD_INF_RULES.prj_exports += "./src/settingsmanager/xqpublishandsubscribesecuritypolicy.h  /epoc32/include/mw/xqpublishandsubscribesecuritypolicy.h"
BLD_INF_RULES.prj_exports += "./include/XQPublishAndSubscribeSecurityPolicy                /epoc32/include/mw/XQPublishAndSubscribeSecurityPolicy"
BLD_INF_RULES.prj_exports += "./src/settingsmanager/xqpublishandsubscribeutils.h           /epoc32/include/mw/xqpublishandsubscribeutils.h"
BLD_INF_RULES.prj_exports += "./include/XQPublishAndSubscribeUtils                         /epoc32/include/mw/XQPublishAndSubscribeUtils"
BLD_INF_RULES.prj_exports += "./src/settingsmanager/xqsettingskey.h                        /epoc32/include/mw/xqsettingskey.h"
BLD_INF_RULES.prj_exports += "./include/XQCentralRepositorySettingsKey                     /epoc32/include/mw/XQCentralRepositorySettingsKey"
BLD_INF_RULES.prj_exports += "./include/XQPublishAndSubscribeSettingsKey                   /epoc32/include/mw/XQPublishAndSubscribeSettingsKey"
BLD_INF_RULES.prj_exports += "./include/XQSettingsKey                                      /epoc32/include/mw/XQSettingsKey"
BLD_INF_RULES.prj_exports += "./src/settingsmanager/xqsettingsmanager.h                    /epoc32/include/mw/xqsettingsmanager.h"
BLD_INF_RULES.prj_exports += "./include/XQSettingsManager                                  /epoc32/include/mw/XQSettingsManager"
BLD_INF_RULES.prj_exports += "./src/sysinfo/sysinfo_global.h                               /epoc32/include/mw/sysinfo_global.h"
BLD_INF_RULES.prj_exports += "./src/sysinfo/xqsysinfo.h                                    /epoc32/include/mw/xqsysinfo.h"
BLD_INF_RULES.prj_exports += "./include/XQSysInfo                                          /epoc32/include/mw/XQSysInfo"
BLD_INF_RULES.prj_exports += "./src/utils/utils_global.h                                   /epoc32/include/mw/utils_global.h"
BLD_INF_RULES.prj_exports += "./src/utils/xqutils.h                                        /epoc32/include/mw/xqutils.h"
BLD_INF_RULES.prj_exports += "./include/XQUtils                                            /epoc32/include/mw/XQUtils"
BLD_INF_RULES.prj_exports += "./src/utils/xqconversions.h                                  /epoc32/include/mw/xqconversions.h"
BLD_INF_RULES.prj_exports += "./include/XQConversions                                      /epoc32/include/mw/XQConversions"
BLD_INF_RULES.prj_exports += "./src/keycapture/xqkeycapture.h                              /epoc32/include/mw/xqkeycapture.h"
BLD_INF_RULES.prj_exports += "./include/XQKeyCapture                                       /epoc32/include/mw/XQKeyCapture"
BLD_INF_RULES.prj_exports += "./rom/qtmobileextensions.iby                                 $$CORE_MW_LAYER_IBY_EXPORT_PATH(qtmobileextensions.iby)"
}
