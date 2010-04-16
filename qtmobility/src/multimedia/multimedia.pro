TEMPLATE = lib

# distinct from QtMultimedia
TARGET = QtMedia

include (../../common.pri)
INCLUDEPATH+= .

QT += network multimedia

contains(QT_CONFIG, opengl): QT += opengl

!static:DEFINES += QT_MAKEDLL
DEFINES += QT_BUILD_MEDIA_LIB

PRIVATE_HEADERS += \
    qmediacontrol_p.h \
    qmediaobject_p.h \
    qmediaservice_p.h  \
    qmediaplaylist_p.h \
    qmediaplaylistprovider_p.h \
    qmediaimageviewerservice_p.h \
    qvideowidget_p.h \
    qmediapluginloader_p.h \
    qpaintervideosurface_p.h

PUBLIC_HEADERS += \
    qmediacontrol.h \
    qmediaobject.h \
    qmediaservice.h \
    qlocalmediaplaylistprovider.h \
    qmediaimageviewer.h \
    qmediaplayer.h \
    qmediaplayercontrol.h \
    qmediaplaylist.h \
    qmediaplaylistnavigator.h \
    qmediaplaylistprovider.h \
    qmediaplaylistioplugin.h \
    qmediacontent.h \
    qmediaresource.h \
    qmediarecorder.h \
    qmediaencodersettings.h \
    qmediarecordercontrol.h \
    qmediaserviceprovider.h \
    qmediaserviceproviderplugin.h \
    qmetadatacontrol.h \
    qmediastreamscontrol.h \
    qradiotuner.h \
    qradiotunercontrol.h \
    qtmedianamespace.h \
    qvideooutputcontrol.h \
    qvideowidget.h \
    qvideowindowcontrol.h \
    qvideowidgetcontrol.h \
    qaudioencodercontrol.h \
    qvideoencodercontrol.h \
    qimageencodercontrol.h \
    qaudiocapturesource.h \
    qmediacontainercontrol.h \
    qmediaplaylistcontrol.h \
    qaudioendpointselector.h \
    qvideodevicecontrol.h \
    qgraphicsvideoitem.h \
    qvideorenderercontrol.h \
    qmediatimerange.h

SOURCES += qmediacontrol.cpp \
    qmediaobject.cpp \
    qmediaservice.cpp \
    qlocalmediaplaylistprovider.cpp \
    qmediaimageviewer.cpp \
    qmediaimageviewerservice.cpp \
    qmediaplayer.cpp \
    qmediaplayercontrol.cpp \
    qmediaplaylist.cpp \
    qmediaplaylistioplugin.cpp \
    qmediaplaylistnavigator.cpp \
    qmediaplaylistprovider.cpp \
    qmediarecorder.cpp \
    qmediaencodersettings.cpp \
    qmediarecordercontrol.cpp \
    qmediacontent.cpp \
    qmediaresource.cpp \
    qmediaserviceprovider.cpp \
    qmetadatacontrol.cpp \
    qmediastreamscontrol.cpp \
    qradiotuner.cpp \
    qradiotunercontrol.cpp \
    qvideooutputcontrol.cpp \
    qvideowidget.cpp \
    qvideowindowcontrol.cpp \
    qvideowidgetcontrol.cpp \
    qaudioencodercontrol.cpp \
    qvideoencodercontrol.cpp \
    qimageencodercontrol.cpp \
    qaudiocapturesource.cpp \
    qmediacontainercontrol.cpp \
    qmediaplaylistcontrol.cpp \
    qaudioendpointselector.cpp \
    qvideodevicecontrol.cpp \
    qmediapluginloader.cpp \
    qpaintervideosurface.cpp \
    qvideorenderercontrol.cpp \
    qmediatimerange.cpp

contains(QT_CONFIG, declarative) {
    QT += declarative

    PRIVATE_HEADERS += \
        qmetadatacontrolmetaobject_p.h \
        qmlaudio_p.h \
        qmlgraphicsvideo_p.h \
        qmlmediabase_p.h \
        qsoundeffect_p.h \
        wavedecoder.h

    SOURCES += \
        qmetadatacontrolmetaobject.cpp \
        qmlaudio.cpp \
        qmlgraphicsvideo.cpp \
        qmlmediabase.cpp \
        qsoundeffect.cpp \
        wavedecoder.cpp

   maemo5: DEFINES += QT_MULTIMEDIA_MAEMO5
   system(pkg-config --exists \'libpulse >= 0.9.10\') {
       DEFINES += QT_MULTIMEDIA_PULSEAUDIO
       PRIVATE_HEADERS += qsoundeffect_pulse_p.h
       SOURCES += qsoundeffect_pulse_p.cpp
       LIBS_PRIVATE += -lpulse
   } else:x11 {
       DEFINES += QT_MULTIMEDIA_QMEDIAPLAYER
       PRIVATE_HEADERS += qsoundeffect_qmedia_p.h
       SOURCES += qsoundeffect_qmedia_p.cpp
   } else {
       PRIVATE_HEADERS += qsoundeffect_qsound_p.h
       SOURCES += qsoundeffect_qsound_p.cpp
   }
}

maemo5 {
    HEADERS += qxvideosurface_maemo5_p.h
    SOURCES += qxvideosurface_maemo5.cpp
    SOURCES += qgraphicsvideoitem_maemo5.cpp
    LIBS += -lXv
} else {
    SOURCES += qgraphicsvideoitem.cpp
}

include (experimental/experimental.pri)
HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

symbian {
    load(data_caging_paths)
    QtMediaDeployment.sources = QtMedia.dll
    QtMediaDeployment.path = /sys/bin
    DEPLOYMENT += QtMediaDeployment
    TARGET.UID3=0x2002AC77
    MMP_RULES += EXPORTUNFROZEN
    TARGET.CAPABILITY = ALL -TCB
}

CONFIG += middleware
include(../../features/deploy.pri)