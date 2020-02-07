#-------------------------------------------------
#
# Project created by QtCreator 2019-09-23T11:13:38
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = demo-cloud
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    MjpgServerClass.cpp \
    ServerClass.cpp \
    serverthread.cpp \
    connthread.cpp

HEADERS  += widget.h \
    MjpgServerClass.h \
    ServerClass.h \
    serverthread.h \
    connthread.h

FORMS    += widget.ui
LIBS    += -ljpeg -lcurl -lopencv_core -lopencv_objdetect -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio
