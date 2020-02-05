#include "widget.h"
#include "ui_widget.h"
#include <QNetworkInterface>
#include <QHostAddress>
#include <QTcpSocket>
#include "tcpclient.h"
#include <QTextEdit>
#include <QKeyEvent>
#define KEY3 16777330
Widget* myW=NULL;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);
    setWindowOpacity(0.7);
    myW=this;
    m_strLanip="";

    system("udhcpc -i eth0 &");
    m_pinittimer=new QTimer;
    connect(m_pinittimer,SIGNAL(timeout()),this,SLOT(on_inittimeout()));
    m_pinittimer->start(1000);

    ui->textEdit_hex->setReadOnly(true);
    ui->textEdit_ascii->setReadOnly(true);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::initserver()
{
    m_ptcpserver=new QTcpServer;

    m_ptcpserver->listen(QHostAddress::Any,local_port);

    ui->label->setText("server : "+m_strLanip+" "+QString::number(local_port));

    connect(m_ptcpserver,SIGNAL(newConnection()),this,SLOT(newconnect()));

}

void Widget::wirtetext(QByteArray ba)
{
    ui->textEdit_hex->append(ba.toHex());
    ui->textEdit_ascii->append(QString(ba).toLocal8Bit());
}



void Widget::on_inittimeout()
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

            if(interface.name()=="eth0")
            {
                if(entry.ip().protocol()==QAbstractSocket::IPv4Protocol)
                    m_strLanip=entry.ip().toString();
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
   if(m_strLanip=="")
   {
       ui->label->setText("get lan ip failed!");
   }
   else
   {
       ui->label->setText("get lan ip successed!");
       m_pinittimer->stop();

       initserver();

   }
}


void Widget::newconnect()
{
    printf("new connection!");
    QTcpSocket* skt=m_ptcpserver->nextPendingConnection();

    tcpclient* newclt=new tcpclient;
    newclt->setclient(skt);

    connect(newclt->m_pClient,SIGNAL(readyRead()),newclt,SLOT(readmsg()));
    connect(newclt->m_pClient,SIGNAL(disconnected()),newclt,SLOT(ondisconnected()));

}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==KEY3)
    {
        exit(0);
    }
}
