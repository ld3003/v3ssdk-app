#-------------------------------------------------
#
# Project created by QtCreator 2019-09-24T13:54:52
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = demo-router
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    serialport.cpp \
    serialreadthread.cpp \
    error-log.cpp \
    net_detect.cpp \
    processmgr.cpp

HEADERS  += widget.h \
    serialport.h \
    serialreadthread.h \
    error-log.h \
    net_detect.h \
    processmgr.h

FORMS    += widget.ui

RESOURCES += \
    img.qrc
