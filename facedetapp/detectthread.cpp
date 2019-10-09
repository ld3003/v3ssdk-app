#include "detectthread.h"
#include "../libfacedetection/src/facedetectcnn.h"
//#include <zbar.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <curl/curl.h>
#include "faceregrequest.h"
#include <QThreadPool>
#include "mobilefacenet.h"
#include "Communiction.h"
#include "Common.h"
#include "mainwindow.h"
#include <QDebug>
using namespace std;
using namespace cv;


#define DETECT_BUFFER_SIZE 0x20000


#ifdef x86_64
#define USE_MTCNN
#define RESET_VAL 1
#endif

#ifdef arm
#define USE_MTCNN
#define RESET_VAL 4
#endif

#define RNU_TEST printf("RUN-TEST %s : %d \n",__FILE__,__LINE__);

DetectThread::DetectThread(CamThread *ct)
{
#ifdef USE_MTCNN
    mtcnn = new MTCNN("./models");
#endif

    pBuffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
    mCt = ct;
    tp.setMaxThreadCount(5);
}


void DetectThread::run()
{

    char checkflag = 0;

    for(;;)
    {
        int i;
        int *pResults;

        std::vector<Bbox> finalBbox;

        cv::Mat image1 = mCt->getImage();
        cv::Mat image2;
        cv::Mat image3;
        cv::Mat image_roi;

        if (image1.empty())
        {

            continue;
        }


#ifdef USE_MTCNN


        cv::resize(image1, image3, cv::Size(image1.cols/RESET_VAL, image1.rows/RESET_VAL),0,0);

        ncnn::Mat ncnn_img = ncnn::Mat::from_pixels(image3.data, ncnn::Mat::PIXEL_BGR2RGB, image3.cols, image3.rows);

        struct timeval gTpstart ,gTpend;
        //time_consuming_start(&gTpstart,&gTpend);
         qDebug()<<"44444";
        mtcnn->detectMaxFace(ncnn_img, finalBbox);
        //time_consuming_print("detect time",&gTpstart,&gTpend);
         qDebug()<<"55555";
        if (finalBbox.size() == 1)
        {

            int x = finalBbox[0].x1*RESET_VAL;
            int y = finalBbox[0].y1*RESET_VAL;
            int w = (finalBbox[0].x2 - finalBbox[0].x1)*RESET_VAL;
            int h = (finalBbox[0].y2 - finalBbox[0].y1)*RESET_VAL;

            mCt->setDetRect(x,y,w,h);

            if (((x+w) > image1.cols) || ((y+h) > image1.rows) || (x<0) || (y<0))
                continue;

            Rect rect(x, y, w, h);
            image_roi = image1(rect);
            //image_roi = image1;
            if (image_roi.empty())
                ;
            else
            {
               // MainWindow::screenshort=image_roi;
                MainWindow::realshort=image_roi;
                MainWindow::needcmp=1;
                qDebug()<<"i have get a picture!";
            }
 //           if(MainWindow::screenshort.empty())
//                qDebug()<<"there is no picture to compare!";
//            else
//                qDebug()<<"there is one picture to compare!";

            if (tp.activeThreadCount() < 1 )
            {
                FaceRegRequest *FR = new FaceRegRequest(image_roi);
                if(!FR->autoDelete()) {
                    qDebug()<<"QRunnable's autoDelete default value is not true";
                    FR->setAutoDelete(true);
                }
                tp.start(FR);
            }else{
            }

            //FR->run();
            //FR->deleteLater();



        }

        continue;
#endif


#ifdef USE_LIBDET


        cv::resize(image1, image3, cv::Size(image1.cols/RESET_VAL, image1.rows/RESET_VAL),0,0);

        struct timeval gTpstart ,gTpend;
        time_consuming_start(&gTpstart,&gTpend);

        pResults = facedetect_cnn(pBuffer, (unsigned char*)(image3.ptr(0)), image3.cols, image3.rows, (int)image3.step);


        //print the detection results
        for(i = 0; i < (pResults ? *pResults : 0); i++)
        {
            short * p = ((short*)(pResults+1))+142*i;
            int x = p[0]*RESET_VAL;
            int y = p[1]*RESET_VAL;
            int w = p[2]*RESET_VAL;
            int h = p[3]*RESET_VAL;
            int confidence = p[4];
            int angle = p[5];

            mCt->setDetRect(x,y,w,h);

#if 1
            printf("ROI X %d Y %d W %d H %d\n",x,y,w,h);
            if (((x+w) > image1.cols) || ((y+h) > image1.rows) || (x<0) || (y<0))
                continue;

            Rect rect(x, y, w, h);
            image_roi = image1(rect);

            if (tp.activeThreadCount() < 3 )
            {
                FaceRegRequest *FR = new FaceRegRequest(image_roi);
                if(!FR->autoDelete()) {
                    qDebug()<<"QRunnable's autoDelete default value is not true";
                    FR->setAutoDelete(true);
                }
                tp.start(FR);
            }
#endif
        }

        time_consuming_print("detect time",&gTpstart,&gTpend);
#endif
    }

}
