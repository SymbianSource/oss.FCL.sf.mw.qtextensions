build:
  qmake -platform win32-g++
  make relese

clean: 
  make clean

The application is generated directly to directory pointed by DESTDIR variable in xqecom.pro file.

Note: to compile one need full include directory placed in /sf/mw/qt directory. Usualy it means
that qt sources need to be configured by running eg.:

  configure -platform win32-g++ -make make -xplatform symbian-abld