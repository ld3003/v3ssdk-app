#ifndef CAMTHREAD_H
#define CAMTHREAD_H

#include <QThread>
#include <QDebug>
#include <QImage>
#include <QMutexLocker>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>


class CamThread : public QThread
{
    Q_OBJECT

public:
    CamThread(QObject *obj);
    void run();
    cv::Mat getImage();
    void setDetRect(int x, int y , int w , int h);
    QString name;

signals:
    void imgReady(QImage img);

private:
    cv::VideoCapture *mCap;

    QMutex mImglocker;
    cv::Mat mImageData1;
    cv::Mat mImageData2;

    QMutex mDetlocker;
    int mDetRect[4];



};

#endif // CAMTHREAD_H
