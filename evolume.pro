#-------------------------------------------------
#
# Project created by QtCreator 2013-09-29T10:04:37
#
#-------------------------------------------------

QT       += core gui x11extras dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



PKGCONFIG += alsa

TEMPLATE = app

TARGET = evolume
DESTDIR = usr/bin
VERSION		= 0.1
CONFIG += qt \
          release
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
INCLUDEPATH +=build

SOURCES += main.cpp\
     audioengine.cpp \
    audiodevice.cpp \
  alsaengine.cpp \
    alsadevice.cpp \
       elokabvolume.cpp \
    widgetvolume.cpp \
    widgetpopup/widgetpopup.cpp \
    volume_adaptor.cpp

HEADERS  += \
    audioengine.h \
    audiodevice.h \
  alsaengine.h \
    alsadevice.h \
    elokabvolume.h \
    widgetvolume.h \
    widgetpopup/widgetpopup.h \
    volume_adaptor.h \
    notificationutill.h

FORMS +=


LIBS += $(SUBLIBS)   -lasound
RESOURCES += \
    icons.qrc



# install





#INSTALL
 target.path = /usr/bin
 INSTALLS +=   target


