include(../staticconfig.pri)

TEMPLATE = subdirs
SUBDIRS += auto

contains(mobility_modules,bearer) {
    symbian:SUBDIRS += bearerex
}

symbian {
    SUBDIRS += s60installs/s60installs.pro
}
