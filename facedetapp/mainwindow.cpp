#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Common/Common.h"
#include "mtcnn.h"
#include "mobilefacenet.h"
#include <QImage>
#include <QPixmap>
#include <QTextFormat>
#include "gpio.h"
#include <QDebug>

cv::Mat MainWindow::screenshort=cv::imread("noway.jpg", CV_LOAD_IMAGE_COLOR);
cv::Mat MainWindow::realshort=cv::imread("noway.jpg", CV_LOAD_IMAGE_COLOR);

int MainWindow::sampneedinit=0;
int MainWindow::needcmp=0;
MainWindow *mw;
#define KEY1 16777220
#define KEY2 0
#define KEY3 16777328
#define KEY4 16777330

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    mw = this;
    //showFullScreen();

    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowOpacity(0.7);

   // setGeometry(0,0,DEV_WINDOWS_W,DEV_WINDOWS_H);
    ui->label->setGeometry(0,-40,DEV_WINDOWS_W,DEV_WINDOWS_H);
    //ui->label->hide();


    camThread = new CamThread(0);
    detThread = new DetectThread(camThread);
    //cmpThread = new CompareThread(0);

    connect(camThread,SIGNAL(imgReady(QImage)),this,SLOT(imgFlush(QImage)));
    connect(detThread,SIGNAL(tipmsg(QString)),this,SLOT(tipmsg(QString)));

    camThread->start();
    detThread->start();
   // cmpThread->start();
    topBar = new QLabel(this);
    topBar->setGeometry(0,0,DEV_WINDOWS_W,50);

    screenshortLable=new QLabel(this);

    screenshortLable->setGeometry(5,DEV_WINDOWS_W-35,108,108);
    screenshortLable->show();



    tipLable = new QLabel(this);
    tipLable->setGeometry(0,100,DEV_WINDOWS_W,DEV_WINDOWS_H-50);
    tipLable->hide();

    QFont ft;
    ft.setPointSize(18);
    tipLable->setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    tipLable->setPalette(pa);
    tipLable->setAlignment(Qt::AlignCenter);

    resultLable=new QLabel(this);
    resultLable->setGeometry(113+10,DEV_WINDOWS_W-25,108,20);
    resultLable->setText("");
    resultLable->setPalette(pa);
    QFont ftf;
    ftf.setPointSize(14);
    resultLable->setFont(ftf);

    more=new QLabel(this);
    more->setGeometry(113+30,DEV_WINDOWS_W-5,64,64);

    //QPalette palette;
    //palette.setBrush(QPalette::Background, QBrush(QPixmap(":/pic/img/right.png")));
  //  more->setPalette(palette);



    tipMsgtimer = new QTimer(this);
    connect(tipMsgtimer,SIGNAL(timeout()),this,SLOT(tipmsgTimeout()));

    syschecktimer = new QTimer(this);
    connect(syschecktimer,SIGNAL(timeout()),this,SLOT(syschecktimeout()));

    syschecktimer->start(1000);

    GPIO_Init();
    GPIO_ConfigPin(PG,0,OUT);
    GPIO_ConfigPin(PB,0,OUT);
    GPIO_ConfigPin(PB,1,OUT);
    GPIO_ConfigPin(PB,2,OUT);
    GPIO_SetPin(PB,0,1);
    GPIO_SetPin(PB,1,1);
    GPIO_SetPin(PB,2,1);
    a = 0;


}

void MainWindow::imgFlush(QImage img)
{
    ui->label->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::tipmsg(QString str)
{


    resultLable->setText(str);
    if (str.contains("score:0.9"))
    {
         more->setPixmap(QPixmap(":/pic/img/right.jpg"));
    }
    else
        more->setPixmap(QPixmap(":/pic/img/wrong.jpg"));
    /*
    if (!tipLable->text().isEmpty())
        return;

    qDebug() << "Tip msg : " << str;
    tipLable->show();
    tipLable->setText(str);
    //tipLable->setTextFormat()
    //




    tipMsgtimer->start(1000);

    if (str.contains("SUCCESS"))
    {
        GPIO_SetPin(PG,0,a=~a);
    }
    */

}
void MainWindow::tipmsgTimeout()
{
    tipLable->setText("");


}

void MainWindow::syschecktimeout()
{
    char mac[32] = "";
    char ip[16] = "";

    long long devid;
    get_local_ip(ETH_NAME,ip);
    get_mac(mac,sizeof(mac),&devid,ETH_NAME);

    topBar->setText(QString("DEVICE ID:").append(QString(mac).append("\n")
                                                 .append("IPADDRESS:").append(QString(ip).append("\n")
                                                                              .append("NETSTATUS:").append(QString(net_detect(ETH_NAME)).append("\n"))

                                                                              )));

    //topBar->setText(QString("DEVICE ID:").append(QString(mac).append("\n")));

}

void MainWindow::openDoor()
{

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<event->key();

    if(event->key()==KEY3)
        exit(0);

    if(!MainWindow::realshort.empty())
    {
        MainWindow::screenshort=MainWindow::realshort;
        imwrite("sample.jpg", MainWindow::realshort);

        QImage pic;
        pic.load("/root/sample.jpg");
        screenshortLable->setPixmap(QPixmap::fromImage(pic.scaled(screenshortLable->size())));

        sampneedinit=1;

   //这里是加上测试是否死机的
 //       char *model_path = "/root";
  //      Recognize recognize(model_path);

 //       std::vector<float> feature1;

 //       recognize.start(MainWindow::screenshort, feature1);


//        qDebug()<<"EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEe";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
