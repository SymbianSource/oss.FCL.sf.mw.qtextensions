include(../staticconfig.pri)

TEMPLATE = subdirs

#BearerManagement examples
contains(mobility_modules,bearer) {
    SUBDIRS += bearermonitor bearercloud
}
#BLD_INF_RULES.prj_exports += "./rom/qtmobilityexamples.iby $$CUSTOMER_VARIANT_APP_LAYER_IBY_EXPORT_PATH(qtmobilityexamples.iby)"

