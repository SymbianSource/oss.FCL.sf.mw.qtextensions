include(../staticconfig.pri)

TEMPLATE = subdirs
SUBDIRS += auto

contains(mobility_modules,bearer) {
    contains(QT_CONFIG,dbus) {
        !symbian:!mac:!maemo6:!maemo5:unix:SUBDIRS += networkmanager
    }
    symbian:SUBDIRS += bearerex
}

symbian {
    SUBDIRS += s60installs/s60installs.pro
}
