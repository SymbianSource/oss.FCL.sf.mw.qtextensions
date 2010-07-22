#######################################################################
# Automatically generated by qmake (2.01a) pe 27. helmi 08:49:52 2009
#######################################################################

TEMPLATE = app
TARGET = 
QT += testlib xml
include(../tsrc.pri)

DEFINES += PBK_UNIT_TEST
DEPENDPATH += .
INCLUDEPATH += .

symbian:
{ 
#    load(data_caging_paths)
    INCLUDEPATH += $$SYMBIAN_PATHS
    INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE
    
    # Input
    HEADERS += $$SYMBIAN_HEADERS \
    		   testrunner.h \
               ut_cntsymbianengine.h \
               ut_cntfiltering.h \
               ut_matchphonenumber.h \
               ut_cntrelationship.h \
               ut_cntsymbiandatabase.h \
               ut_transformcontactdata.h \
               ut_cntsqlsearch.h \
               mt_cntsqlsort.h
    
    SOURCES += $$SYMBIAN_SOURCES \
    		   testrunner.cpp \
    		   main.cpp \
               ut_cntsymbianengine.cpp \
               ut_cntfiltering.cpp \
               ut_matchphonenumber.cpp \
               ut_cntrelationship.cpp \
               ut_cntsymbiandatabase.cpp \
               ut_transformcontactdata.cpp \
               ut_cntsqlsearch.cpp \
               mt_cntsqlsort.cpp

    CONFIG += mobility
    MOBILITY = contacts

    TARGET.CAPABILITY = ReadUserData \
                        WriteUserData

    LIBS += $$SYMBIAN_LIBS
    
    BLD_INF_RULES.prj_exports += \
        "test_data.txt \epoc32\winscw\c\filtering\test_data.txt"
}