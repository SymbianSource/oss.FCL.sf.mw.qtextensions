TEMPLATE = subdirs

isEmpty(QT_LIBINFIX):symbian {
    include(../../staticconfig.pri)
    load(data_caging_paths)
    include($$QT_MOBILITY_BUILD_TREE/config.pri)

    SUBDIRS =
    TARGET = "QtMobility"
    TARGET.UID3 = 0x2002AC89

    VERSION = 1.0.1

    vendorinfo = \
        "; Localised Vendor name" \
        "%{\"Nokia\"}" \
        " " \
        "; Unique Vendor name" \
        ":\"Nokia\"" \
        " "
    qtmobilitydeployment.pkg_prerules += vendorinfo

    epoc31 = $$(EPOCROOT31)
    epoc32 = $$(EPOCROOT32)
    epoc50 = $$(EPOCROOT50)

    # default to EPOCROOT if EPOCROOTxy not defined
    isEmpty(epoc31) {
        EPOCROOT31 = $${EPOCROOT}
    } else {
        EPOCROOT31 = $$(EPOCROOT31)
    }
    isEmpty(epoc32) {
        EPOCROOT32 = $${EPOCROOT}
    }else {
        EPOCROOT32 = $$(EPOCROOT32)
    }
    isEmpty(epoc50) {
        EPOCROOT50 = $${EPOCROOT}
    } else {
        EPOCROOT50 = $$(EPOCROOT50)
    }
    
    contains(mobility_modules, bearer) {
        bearer = \
            "IF package(0x1028315F)" \
            "   \"$${EPOCROOT50}epoc32/release/$(PLATFORM)/$(TARGET)/QtBearer.dll\" - \"!:\\sys\\bin\\QtBearer.dll\"" \
            "ELSEIF package(0x102752AE)" \
            "   \"$${EPOCROOT50}epoc32/release/$(PLATFORM)/$(TARGET)/QtBearer.dll\" - \"!:\\sys\\bin\\QtBearer.dll\"" \
            "ELSEIF package(0x102032BE)" \
            "   \"$${EPOCROOT31}epoc32/release/$(PLATFORM)/$(TARGET)/QtBearer.dll\" - \"!:\\sys\\bin\\QtBearer.dll\"" \
            "ELSE" \
            "   \"$${EPOCROOT50}epoc32/release/$(PLATFORM)/$(TARGET)/QtBearer.dll\" - \"!:\\sys\\bin\\QtBearer.dll\"" \
            "ENDIF"

        qtmobilitydeployment.pkg_postrules += bearer
    }
    
    !isEmpty(pluginstubs):qtmobilitydeployment.pkg_postrules += pluginstubs

    qtmobilitydeployment.path = /sys/bin

    DEPLOYMENT += qtmobilitydeployment
    
    #BLD_INF_RULES.prj_exports += "./qtmobility.iby           $$CORE_MW_LAYER_IBY_EXPORT_PATH(qtmobility.iby)"

} else {
    message(Deployment of infixed library names not supported)
}
