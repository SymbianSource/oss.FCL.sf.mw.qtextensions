include(../staticconfig.pri)

TEMPLATE = subdirs
SUBDIRS += auto benchmarks

contains(mobility_modules,serviceframework): SUBDIRS += testservice2 sampleserviceplugin sampleserviceplugin2
contains(mobility_modules,bearer) {
    contains(QT_CONFIG,dbus) {
        !symbian:!mac:!maemo6:!maemo5:unix:SUBDIRS += networkmanager
    }
    symbian:SUBDIRS += bearerex
}

symbian {
    contains(mobility_modules,messaging): SUBDIRS += messagingex
    contains(mobility_modules,multimedia): SUBDIRS += cameracapture_s60 playerex_s60
    contains(mobility_modules,publishsubscribe): SUBDIRS += publishsubscribeex
}

BLD_INF_RULES.prj_exports += "./rom/qtmobilitytests.iby $$CUSTOMER_VARIANT_APP_LAYER_IBY_EXPORT_PATH(qtmobilitytests.iby)"
