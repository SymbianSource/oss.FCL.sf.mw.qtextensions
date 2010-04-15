include(../staticconfig.pri)

TEMPLATE = subdirs

#BearerManagement examples
contains(mobility_modules,bearer) {
    SUBDIRS += bearermonitor bearercloud
}

symbian {
    SUBDIRS += s60installs/s60installs.pro
}
