#-------------------------------------------------
#
# Project created by QtCreator 2019-09-11T13:42:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = demo-guid
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
	processmgr.cpp\
	gpio.cpp

HEADERS  += widget.h \
	 processmgr.h \
	gpio.h

FORMS    += widget.ui

RESOURCES += \
    my.qrc
