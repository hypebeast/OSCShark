#-------------------------------------------------
#
# Project created by QtCreator 2013-10-13T16:26:52
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OSCMonitor
TEMPLATE = app


SOURCES += src/osclistener.cpp \
    src/mainwindow.cpp \
    src/main.cpp \
    src/oscpack/ip/IpEndpointName.cpp \
    src/oscpack/osc/OscTypes.cpp \
    src/oscpack/osc/OscReceivedElements.cpp \
    src/oscpack/osc/OscPrintReceivedElements.cpp \
    src/oscpack/osc/OscOutboundPacketStream.cpp \
    src/osclistenerworker.cpp \
    src/osclistenercontroller.cpp


HEADERS  += src/osclistener.h \
    src/mainwindow.h \
    src/oscpack/ip/UdpSocket.h \
    src/oscpack/ip/TimerListener.h \
    src/oscpack/ip/PacketListener.h \
    src/oscpack/ip/NetworkingUtils.h \
    src/oscpack/ip/IpEndpointName.h \
    src/oscpack/osc/OscTypes.h \
    src/oscpack/osc/OscReceivedElements.h \
    src/oscpack/osc/OscPrintReceivedElements.h \
    src/oscpack/osc/OscPacketListener.h \
    src/oscpack/osc/OscOutboundPacketStream.h \
    src/oscpack/osc/OscHostEndianness.h \
    src/oscpack/osc/OscException.h \
    src/oscpack/osc/MessageMappingOscPacketListener.h \
    src/osclistenerworker.h \
    src/osclistenercontroller.h \
    src/oscpkt/udp.hh \
    src/oscpkt/oscpkt.hh \
    common.h

win32 {
    SOURCES += src/oscpack/ip/win32/UdpSocket.cpp \
        src/oscpack/ip/win32/NetworkingUtils.cpp

   LIBS += C:\Qt\Tools\mingw48_32\i686-w64-mingw32\lib\libws2_32.a

    #INCLUDEPATH += c:\Qt\Tools\mingw48_32\i686-w64-mingw32\include
}

unix {
    SOURCES += src/oscpack/ip/posix/UdpSocket.cpp \
        src/oscpack/ip/posix/NetworkingUtils.cpp
}

FORMS    += src/mainwindow.ui

INCLUDEPATH += src/oscpack
