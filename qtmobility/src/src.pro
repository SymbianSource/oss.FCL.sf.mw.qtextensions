include(../staticconfig.pri)

TEMPLATE = subdirs
SUBDIRS += global

contains(mobility_modules,bearer): SUBDIRS += bearer

symbian {
    SUBDIRS += s60installs
}

