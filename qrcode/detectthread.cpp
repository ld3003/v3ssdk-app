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

#include "Communiction.h"
#include "Common.h"
#include <zbar.h>
#include "mainwindow.h"
#include <QMetaObject>


using namespace std;
using namespace cv;
using namespace zbar;

//zbar接口
string ZbarDecoder(Mat img)
{
    string result;
    ImageScanner scanner;
    const void *raw = (&img)->data;
    // configure the reader
    scanner.set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1);
    // wrap image data
    Image image(img.cols, img.rows, "Y800", raw, img.cols * img.rows);
    // scan the image for barcodes
    int n = scanner.scan(image);
    // extract results
    result = image.symbol_begin()->get_data();
    image.set_data(NULL, 0);
    return result;
}

//对二值图像进行识别，如果失败则开运算进行二次识别
string GetQRInBinImg(Mat binImg)
{
    string result = ZbarDecoder(binImg);
    if(result.empty())
    {
        Mat openImg;
        Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
        morphologyEx(binImg, openImg, MORPH_OPEN, element);
        result = ZbarDecoder(openImg);
    }
    return result;
}

//main function
string GetQR(Mat img)
{
    Mat binImg;
    //在otsu二值结果的基础上，不断增加阈值，用于识别模糊图像
    int thre = threshold(img, binImg, 0, 255, cv::THRESH_OTSU);
    string result;
    while(result.empty() && thre<255)
    {
        threshold(img, binImg, thre, 255, cv::THRESH_BINARY);
        result = GetQRInBinImg(binImg);
        thre += 20;//阈值步长设为20，步长越大，识别率越低，速度越快
    }
    return result;
}



void Jpegcompress(const cv::Mat& src, cv::Mat& dest, int quality)
{
    std::vector<uchar> buff;
    std::vector<int> params;
    /*IMWRITE_JPEG_QUALITY For JPEG, it can be a quality from 0 to 100
    (the higher is the better). Default value is 95 */
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(quality);
    //将图像压缩编码到缓冲流区域
    cv::imencode(".jpg", src, buff, params);
    //将压缩后的缓冲流内容解码为Mat，进行后续的处理
    dest = cv::imdecode(buff, -1);
    //cv::imshow("src", src);
    //cv::imshow("dst", dest);
}

DetectThread::DetectThread(CamThread *ct)
{
    mCt = ct;
    tp.setMaxThreadCount(5);
}


void DetectThread::run()
{

    char checkflag = 0;

#define DETECT_BUFFER_SIZE 0x20000
    unsigned char * pBuffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
    string result;

    qDebug() << "####################################\n";
    for(;;)
    {
        int i;
        cv::Mat image1 = mCt->getImage();

        if (image1.empty())
            continue;
         cv::cvtColor(image1,image1,CV_BGR2GRAY);

        result = GetQR(image1);
        std::cout << result <<endl;

        QMetaObject::invokeMethod(mw,"tipmsg",Q_ARG(QString,QString(result.data())));
    }


}
