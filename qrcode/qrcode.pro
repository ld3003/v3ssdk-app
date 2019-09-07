#-------------------------------------------------
#
# Project created by QtCreator 2019-01-29T20:15:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qrcode
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        Common/Common.c \
        Common/Common_nh_type.c \
        Communiction/Communiction.c \
        faceregrequest.cpp \
        main.cpp \
        mainwindow.cpp \
        camthread.cpp \
        detectthread.cpp

HEADERS += \
        Common/Common.h \
        Common/Common_nh_type.h \
        Communiction/Communiction.h \
        faceregrequest.h \
        mainwindow.h \
        camthread.h \
        detectthread.h \
        ui_mainwindow.h

FORMS += \
        mainwindow.ui


INCLUDEPATH += $${PWD}/Common/
INCLUDEPATH += $${PWD}/Communiction/

LIBS    += -ljpeg -lcurl -lopencv_core -lopencv_objdetect -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio -lyuv -lzbar
LIBS    += ../libfacedetection/build/libfacedetection.a

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
