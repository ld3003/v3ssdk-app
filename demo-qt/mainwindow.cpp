#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    vector<cv::Rect> eyeRect;
    vector<cv::Rect> faceRect;
    cv::Mat image,image2,image3,image_gray;

    QImage Img;

    ui->setupUi(this);

    cv::CascadeClassifier eye_Classifier;  //载入分类器
    cv::CascadeClassifier face_cascade;    //载入分类器

    //加载分类训练器，OpenCv官方文档提供的xml文档，可以直接调用
    //xml文档路径  opencv\sources\data\haarcascades
    if (!eye_Classifier.load("./haarcascade_eye.xml"))  //需要将xml文档放在自己指定的路径下
    {
        cout << "Load haarcascade_eye.xml failed!" << endl;
        //return 0;
    }

    if (!face_cascade.load("./haarcascade_frontalface_alt.xml"))
    {
        cout << "Load haarcascade_frontalface_alt failed!" << endl;
        //return 0;
    }

    cap =  new cv::VideoCapture(0);
    if(cap->isOpened())
    {
        cout<<"success"<<endl;
    }
    else
    {
        cout << "errr" << endl;
    }

    *cap >> image;

    cv::resize(image, image2, cv::Size(image.cols/4, image.rows/4),0,0);

    cv::cvtColor(image2, image3, CV_BGR2RGB);//颜色空间转换

    Img = QImage((const uchar*)(image3.data), image3.cols, image3.rows, image3.cols * image3.channels(), QImage::Format_RGB888);


    cv::cvtColor(image2, image_gray, CV_BGR2GRAY);//转为灰度图
    cv::equalizeHist(image_gray, image_gray);//直方图均衡化，增加对比度方便处理

    //检测关于眼睛部位位置
    eye_Classifier.detectMultiScale(image_gray, eyeRect, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
    for (size_t eyeIdx = 0; eyeIdx < eyeRect.size(); eyeIdx++)
    {
        rectangle(image, eyeRect[eyeIdx], cv::Scalar(0, 0, 255));   //用矩形画出检测到的位置
    }

    //检测关于脸部位置
    face_cascade.detectMultiScale(image_gray, faceRect, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
    for (size_t i = 0; i < faceRect.size(); i++)
    {
        rectangle(image, faceRect[i], cv::Scalar(0, 0, 255));      //用矩形画出检测到的位置

    }




    ui->label->setPixmap(QPixmap::fromImage(Img));

}

MainWindow::~MainWindow()
{
    delete ui;
}
