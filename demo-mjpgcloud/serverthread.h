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
#include <QTcpSocket>
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
    QTcpSocket* m_pSkt;
    bool m_connected;
    double m_scaled;
    void initskt();

    void Load_File(const char * name,unsigned char ** out_data, unsigned int * out_data_size);
    void run();
public slots:
    void on_connected();
    void on_conerror();
};

#endif // SERVERTHREAD_H
