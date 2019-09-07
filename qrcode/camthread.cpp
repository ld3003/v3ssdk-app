#include "camthread.h"
#include "../libfacedetection/src/facedetectcnn.h"

#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>






CamThread::CamThread(QObject *obj)
{

    //GPIO_Init();

    mCap = new cv::VideoCapture(1);

    if( mCap->isOpened())
    {
        qDebug() <<"opencamera success" ;
    }
    else
    {
        qDebug() << "opencamera error" ;
    }


}


void CamThread::run()
{
    QImage Img;
    cv::Mat tmppic0,tmppic;

    for(;;)
    {
        mImglocker.lock();
        *mCap >> mImageData2;
        mImageData2 = mImageData2(cv::Rect(160 , 120 , 320 , 240));
        mImglocker.unlock();


        mImageData1 = mImageData2;

#define RESIZE_VAL 2
        //cv::resize(mImageData1, mImageData1, cv::Size(mImageData1.cols/RESIZE_VAL, mImageData1.rows/RESIZE_VAL),0,0);


        cv::cvtColor(mImageData1, tmppic, CV_BGR2RGB);
        //tmppic = mImageData1;

        mDetlocker.lock();
        //rectangle(tmppic, cv::Rect(mDetRect[0]/RESIZE_VAL, mDetRect[1]/RESIZE_VAL, mDetRect[2]/RESIZE_VAL, mDetRect[3]/RESIZE_VAL), cv::Scalar(0, 255, 0), 2);
        mDetlocker.unlock();

        Img = QImage((const uchar*)(tmppic.data), tmppic.cols, tmppic.rows, tmppic.cols * tmppic.channels(), QImage::Format_RGB888);
        emit imgReady(Img);

    }

}

cv::Mat CamThread::getImage()
{
    cv::Mat image;
    mImglocker.lock();
    image = mImageData2;
    mImglocker.unlock();
    return image;
}

void CamThread::setDetRect(int x, int y , int w , int h)
{
    mDetlocker.lock();
    mDetRect[0] = x;
    mDetRect[1] = y;
    mDetRect[2] = w;
    mDetRect[3] = h;
    mDetlocker.unlock();
}
