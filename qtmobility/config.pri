build_unit_tests = no 
build_examples = no 
build_docs = no 
build_tools = no
qmf_enabled = no 
isEmpty($$QT_MOBILITY_INCLUDE):QT_MOBILITY_INCLUDE=$$QT_MOBILITY_PREFIX/include 
isEmpty($$QT_MOBILITY_LIB):QT_MOBILITY_LIB=$$QT_MOBILITY_PREFIX/lib 
isEmpty($$QT_MOBILITY_BIN):QT_MOBILITY_BIN=$$QT_MOBILITY_PREFIX/bin 
isEmpty($$QT_MOBILITY_PLUGINS):QT_MOBILITY_PLUGINS=$$QT_MOBILITY_PREFIX/plugins 
mobility_modules =  bearer   
maemo5|maemo6:mobility_modules -= systeminfo 
contains(mobility_modules,versit): mobility_modules *= contacts  
lbt_enabled = yes 
snap_enabled = yes 
occ_enabled = yes 
symbiancntsim_enabled = yes 
MOBILITY_SD_MCL_BUILD = yes
sensors_symbian_enabled = yes
audiorouting_s60_enabled = yes
radioutility_s60_enabled = yes
openmaxal_symbian_enabled = yes
surfaces_s60_enabled = yes
