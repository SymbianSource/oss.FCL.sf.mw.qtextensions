include(../staticconfig.pri)

TEMPLATE = subdirs

#ServiceFramework examples
contains(mobility_modules,serviceframework) {
    SUBDIRS += filemanagerplugin \
            bluetoothtransferplugin \
            notesmanagerplugin \
            servicebrowser
#            todotool
    
    contains(QT_CONFIG, declarative) {
        SUBDIRS += declarative
    }
}

#BearerManagement examples
contains(mobility_modules,bearer) {
    SUBDIRS += bearermonitor bearercloud
}

#Location examples
contains(mobility_modules,location) {
    SUBDIRS += logfilepositionsource \
		satellitedialog
    contains(mobility_modules,bearer) {
    	SUBDIRS += flickrdemo \
		    weatherinfo \
		    lightmaps
        contains(QT_CONFIG, webkit) {
            SUBDIRS += fetchgooglemaps
        }
    }		
}

#Contacts examples
contains(mobility_modules,contacts) {
    SUBDIRS += samplephonebook
    contains(mobility_modules,versit):contains(QT_CONFIG, declarative) {
        SUBDIRS += qmlcontacts
    }
}

#Publish and Subscribe examples
contains(mobility_modules,publishsubscribe) {
    !wince*{
        SUBDIRS += publish-subscribe
        contains(QT_CONFIG, declarative) {
            SUBDIRS += battery-charge
        }
    }
}

#System Information
contains(mobility_modules,systeminfo): SUBDIRS += sysinfo

#Multimedia
contains(mobility_modules,multimedia) {
    #disabled on Symbian due to missing backend
    SUBDIRS += \
        radio \
        player \
        cameracapture \
        slideshow \
        streamplayer \
        audiorecorder

    contains (QT_CONFIG, declarative) {
        SUBDIRS += \
            declarativemusic \
            declarativevideo
    }
}


#Messaging examples
contains(qmf_enabled,yes)|wince*|win32|symbian|maemo5 {
    contains(mobility_modules,messaging) {
        !win32-g++ {
	    SUBDIRS += \
                querymessages \
                writemessage \
                serviceactions

                contains(mobility_modules,contacts) {
                    SUBDIRS += keepintouch
                }
         }
    }
}

# Sensors API examples
contains(mobility_modules,sensors) {
    SUBDIRS += sensors
}

BLD_INF_RULES.prj_exports += "./rom/qtmobilityexamples.iby $$CUSTOMER_VARIANT_APP_LAYER_IBY_EXPORT_PATH(qtmobilityexamples.iby)"

