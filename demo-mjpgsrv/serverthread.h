#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QObject>
#include <QThread>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include "MjpgServerClass.h"
class serverthread : public QThread
{
    Q_OBJECT
public:
    serverthread(QObject *obj);
    MjpgServerClass g_MjpgServer;

    unsigned int g_Img1Size;
    unsigned char * g_Img1Data;
    unsigned int g_Img2Size;
    unsigned char * g_Img2Data;
    cv::VideoCapture *g_cap;

    void Load_File(const char * name,unsigned char ** out_data, unsigned int * out_data_size);
    void run();
};

#endif // SERVERTHREAD_H
