#-------------------------------------------------
#
# Project created by QtCreator 2019-09-23T11:13:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = demo-mjpgsrv
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    MjpgServerClass.cpp \
    ServerClass.cpp \
    serverthread.cpp

HEADERS  += widget.h \
    MjpgServerClass.h \
    ServerClass.h \
    serverthread.h

FORMS    += widget.ui
LIBS    += -ljpeg -lcurl -lopencv_core -lopencv_objdetect -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio
