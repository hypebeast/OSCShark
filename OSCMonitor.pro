#-------------------------------------------------
#
# Project created by QtCreator 2013-10-13T16:26:52
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD = 0

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
            "VERSION_MINOR=$$VERSION_MINOR"\
            "VERSION_BUILD=$$VERSION_BUILD"

VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}

TARGET = OSCShark
TEMPLATE = app

SOURCES += \
    src/mainwindow.cpp \
    src/main.cpp \
    src/osclistenerworker.cpp \
    src/osclistenercontroller.cpp \
    exportdialog.cpp


HEADERS  += \
    src/mainwindow.h \
    src/osclistenerworker.h \
    src/osclistenercontroller.h \
    src/oscpkt/udp.hh \
    src/oscpkt/oscpkt.hh \
    common.h \
    exportdialog.h

FORMS    += src/mainwindow.ui

OTHER_FILES += \
    src/stylesheet.qss

RESOURCES += \
    src/resources.qrc

macx {
    ICON = $${PWD}/src/data/icons/app_icon.icns
}
