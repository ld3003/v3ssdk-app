#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Common/Common.h"

#include <QImage>
#include <QPixmap>
#include <QTextFormat>
#include "gpio.h"


MainWindow *mw;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    mw = this;
    //showFullScreen();

    ui->setupUi(this);


    setGeometry(0,0,DEV_WINDOWS_W,DEV_WINDOWS_H);
    ui->label->setGeometry(0,0,DEV_WINDOWS_W,DEV_WINDOWS_H);


    camThread = new CamThread(0);
    detThread = new DetectThread(camThread);

    connect(camThread,SIGNAL(imgReady(QImage)),this,SLOT(imgFlush(QImage)));
    connect(detThread,SIGNAL(tipmsg(QString)),this,SLOT(tipmsg(QString)));

    camThread->start();
    detThread->start();

    topBar = new QLabel(this);
    topBar->setGeometry(0,0,DEV_WINDOWS_W,100);

    tipLable = new QLabel(this);
    tipLable->setGeometry(0,100,DEV_WINDOWS_W,DEV_WINDOWS_H-100);
    tipLable->hide();

    QFont ft;
    ft.setPointSize(18);
    tipLable->setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    tipLable->setPalette(pa);
    tipLable->setAlignment(Qt::AlignCenter);


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

}

void MainWindow::openDoor()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}
