INCLUDEPATH+=../../../src/sensors

include(version.pri)
include(grueplugin.pri)
include(../../../common.pri)

TEMPLATE = lib
CONFIG += plugin
TARGET = $$qtLibraryTarget(sensors_grueplugin)
symbian:TARGET.EPOCALLOWDLLDATA = 1

QT=core
CONFIG+=mobility
MOBILITY+=sensors

CONFIG+=strict_flags

DESTDIR = $$OUTPUT_DIR/bin/examples/sensors
target.path = $$SOURCE_DIR/plugins/sensors
INSTALLS += target

