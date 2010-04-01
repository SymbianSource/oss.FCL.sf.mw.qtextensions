include(../staticconfig.pri)

TEMPLATE = subdirs
SUBDIRS += auto

contains(mobility_modules,bearer) {
    contains(QT_CONFIG,dbus) {
        !symbian:!mac:!maemo6:!maemo5:unix:SUBDIRS += networkmanager
    }
    symbian:SUBDIRS += bearerex
}

#BLD_INF_RULES.prj_exports += "./rom/qtmobilitytests.iby $$CUSTOMER_VARIANT_APP_LAYER_IBY_EXPORT_PATH(qtmobilitytests.iby)"
