#-------------------------------------------------
#
# Project created by QtCreator 2019-01-29T20:15:17
#
#-------------------------------------------------

QT       += core gui

DEFINES += $${QT_ARCH}


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
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
CONFIG += fopenmp

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -fopenmp

SOURCES += \
        Common/Common.c \
        Common/Common_nh_type.c \
        Communiction/Communiction.c \
        cJSON.c \
        faceregrequest.cpp \
        gpio.c \
        main.cpp \
        mainwindow.cpp \
        camthread.cpp \
        detectthread.cpp

HEADERS += \
        Common/Common.h \
        Common/Common_nh_type.h \
        Communiction/Communiction.h \
        cJSON.h \
        faceregrequest.h \
        gpio.h \
        mainwindow.h \
        camthread.h \
        detectthread.h \
        mobilefacenet.h \
        ui_mainwindow.h

FORMS += \
        mainwindow.ui


INCLUDEPATH += $${PWD}/Common/
INCLUDEPATH += $${PWD}/Communiction/

LIBS    += -ljpeg -lcurl -lopencv_core -lopencv_objdetect -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio





contains(DEFINES,x86_64){

        INCLUDEPATH += ../ncnn_x64/src/
        INCLUDEPATH += ../ncnn_x64/build/src/

        LIBS    += ../libfacedetection_x64/build/libfacedetection.a
        LIBS    += ../ncnn_x64/build/src/libncnn.a
        LIBS    += -lGL -ltiff
        LIBS    += -fopenmp
        message(platform x86_64)

        SOURCES += mtcnn.cpp
        SOURCES += mobilefacenet.cpp
        HEADERS += mtcnn.h

}else{
        INCLUDEPATH += ../ncnn/src/
        INCLUDEPATH += ../ncnn/build/src/

        LIBS    += ../libfacedetection/build/libfacedetection.a
        LIBS    += ../ncnn/build/src/libncnn.a
        LIBS    += -fopenmp
        message(platform arm)

        SOURCES += mtcnn.cpp
        SOURCES += mobilefacenet.cpp
        HEADERS += mtcnn.h

}


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
