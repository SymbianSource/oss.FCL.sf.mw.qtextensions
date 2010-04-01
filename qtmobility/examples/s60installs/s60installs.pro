!symbian:error(This example is for Symbian packaging purposes only.)

TEMPLATE = app
TARGET = S60Examples

include(../../staticconfig.pri)

TEMPLATE = subdirs

load(data_caging_paths)

#BearerManagement examples
contains(mobility_modules, bearer) {
    SUBDIRS += ../bearermonitor \
        ../../tests/bearerex
    executables.sources += \
        bearermonitor.exe \
        BearerEx.exe

    reg_resource.sources += \
         $${EPOCROOT}$$HW_ZDIR$$REG_RESOURCE_IMPORT_DIR/bearermonitor_reg.rsc \
         $${EPOCROOT}$$HW_ZDIR$$REG_RESOURCE_IMPORT_DIR/BearerEx_reg.rsc
    resource.sources += \
         $${EPOCROOT}$$HW_ZDIR$$APP_RESOURCE_DIR/bearermonitor.rsc \
         $${EPOCROOT}$$HW_ZDIR$$APP_RESOURCE_DIR/BearerEx.rsc
}

executables.path = /sys/bin
reg_resource.path = $$REG_RESOURCE_IMPORT_DIR
resource.path = $$APP_RESOURCE_DIR

DEPLOYMENT += executables \
    pluginstubs \
    reg_resource \
    resource

mifs.sources = \
    $${EPOCROOT}$$HW_ZDIR$$APP_RESOURCE_DIR/0xA000C611.mif
mifs.path = $$APP_RESOURCE_DIR


# ensure that dependency to QtMobility sis package is added
CONFIG+=mobility
