#include "widget.h"
#include "ui_widget.h"
#include <QNetworkInterface>
#include <QHostAddress>
#include <QFont>
#include <QLabel>
#include <QPalette>
#include <QTextEdit>
#include <QKeyEvent>
#include <QProcess>
#include <QSettings>
#include <QNetworkInterface>
#define KEY3 16777330
Widget* myW;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    myW=this;
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowOpacity(0.7);
//
   // QProcess *process = new QProcess(); process->start("sh /root/sdcard/hostap.sh");

  //  QProcess *process1 = new QProcess(); process->start("udhcpc -i eth0");

    system("sh /root/bin/usb-set-hostmode.sh");

    sleep(2);

    system("udhcpc -i eth1 &");
    sleep(5);

    m_strlocalip="";
    m_pinitTimer=new QTimer;
    connect(m_pinitTimer,SIGNAL(timeout()),this,SLOT(inittimeout()));

    m_pinitTimer->start(2000);

    QFont ft;
    ft.setPointSize(14);
    ui->labelport->setFont(ft);
    ui->labelport_2->setFont(ft);
    ui->labelport->setFont(ft);
    ui->labelip->setFont(ft);
    ui->label->setFont(ft);
    ui->label_2->setFont(ft);
    ui->label_3->setFont(ft);

    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    ui->labelport->setPalette(pa);
    ui->labelport_2->setPalette(pa);
    ui->labelport->setPalette(pa);
    ui->labelip->setPalette(pa);
    ui->label->setPalette(pa);
    ui->label_2->setPalette(pa);
    ui->label_3->setPalette(pa);

    getconfig();



    QFont ftt;
    ftt.setPointSize(16);
    QPalette paa;
    paa.setColor(QPalette::WindowText,Qt::red);
    ui->labelstat->setFont(ftt);
    ui->labelstat->setPalette(paa);
    //ui->labelstat->setText("Client:0");


}

Widget::~Widget()
{
    delete ui;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==KEY3)
    {
        exit(0);
    }
}

void Widget::getconfig()
{
    QSettings user_cfg("/root/bin/cloud.cfg",QSettings::IniFormat);
    user_cfg.beginGroup("remote");
    m_strip=user_cfg.value("ip").toString();
    m_port=user_cfg.value("port").toInt();
    ui->labelip->setText("ip:"+m_strip);
    ui->labelport->setText("port:"+QString::number(m_port));

    printf("%s %d",m_strip,m_port);
    user_cfg.endGroup();

}

void Widget::startserver()
{
    sthd=new serverthread(this);
    sthd->start();
}

void Widget::updateclientcnt(QString str)
{
    ui->labelstat->setText("connecte to cloud:"+str);
}

void Widget::inittimeout()
{
    QList<QNetworkInterface> list=QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface interface,list)
    {
        qDebug()<<"Device:"<<interface.name();
        qDebug()<<"HardwareAddress:"<<interface.hardwareAddress();
        QList<QNetworkAddressEntry> entryList=interface.addressEntries();
        foreach(QNetworkAddressEntry entry,entryList)
        {//便利ip条目列表
            qDebug()<<"IP address:"<<entry.ip().toString();
            qDebug()<<"Netmask:"<<entry.netmask().toString();//获取子网掩码
            qDebug()<<"Broadcast:"<<entry.broadcast().toString();//获取广播地址

            if(interface.name()=="eth1")
            {
                if(entry.ip().protocol()==QAbstractSocket::IPv4Protocol)
                    m_strlocalip=entry.ip().toString();
            }
            /*
            else  if(interface.name()=="eth0")
            {
                if(entry.ip().protocol()==QAbstractSocket::IPv4Protocol)
                    m_lanip=entry.ip().toString();
            }
            */

        }
   }
   if((m_strlocalip=="")||(m_strlocalip=="0.0.0.0"))
   {
       ui->labelstat->setText("get lan ip failed!");
   }
   else
   {
       ui->labelstat->setText("get lan ip successed!");
       m_pinitTimer->stop();

       startserver();

   }
}


