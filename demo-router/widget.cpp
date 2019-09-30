#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QTime>
#include <QCoreApplication>
#include <QFont>
#include <QGroupBox>
#include <QKeyEvent>
int Widget::serial=-1;
#define MAX_AT_CNT 6
#define KEY1 16777220
#define KEY2 0
#define KEY3 16777328
#define KEY4 16777330
Widget* mwgt;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    mwgt=this;
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowOpacity(0.7);
    configserial();
    sendindex=0;


    QFont ft;
    ft.setPointSize(14);
    ui->labelonline->setFont(ft);
    ui->labelonlinecsq->setFont(ft);
    ui->labelonlinecops->setFont(ft);
    ui->labelband->setFont(ft);

    ui->groupBox->setFont(ft);
    ui->groupBox_2->setFont(ft);
//iptables -t nat -A POSTROUTING -s 192.168.10.0/24 -j SNAT --to-source 192.168.2.1 --privileged
//iptables -t nat -A POSTROUTING -s 192.168.10.0/24  -o eth0  -j MASQUERADE
    //post-up iptables -t nat -A POSTROUTING -s '192.168.10.0/24' -o eth0 -j MASQUERADE
    //post-down iptables -t nat -D POSTROUTING -s '192.168.10.0/24' -o eth0 -j MASQUERADE
    //system("/root/sdcard/./qrcode -qws");


    ui->label->setFont(ft);
    ui->label_2->setFont(ft);

    system("sh /root/sdcard/hostap.sh");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::getallserialport()
{

}

int Widget::open4gserial()
{
    char dev_name[32] = {0};
    strcpy(dev_name, "/dev/ttyS2");
    serial = open_port(dev_name); /* open the port */
    if (serial < 0)
    {
      qDebug()<<"open %s err\n";
       return OP_ERROR;
    }
    return OP_OK;
}

void Widget::configserial()
{
    if(open4gserial()==OP_OK)
    {
        int ret;
        ret=setup_port(serial, 115200, 8, 'N', 1);
        if(ret<0)
            return;

        memset(sendbuf,0,512);

       // qDebug()<<"start serial thread!";
     //   m_preadThread=new serialreadthread(this);
    //    m_preadThread->start();

        qDebug()<<"start ask timer!";
        m_pTimer=new QTimer;
       // m_pTimer->setInterval(1000);
        connect(m_pTimer,SIGNAL(timeout()),this,SLOT(ask_timeout()));
        m_pTimer->start(500);
    }
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==KEY3)
    {
      exit(0);
    }
}

void Widget::ask_timeout()
{

     qDebug()<<"ask timeout!";
      QString str;

     switch(sendindex)
     {
     case 0:
         str ="AT+CGATT?\r\n";
         break;
     case 1:
         str ="AT+CSQ\r\n";
         break;
     case 2:
         str ="AT+COPS?\r\n";
         break;
     case 4:
         str ="AT^SYSINFO\r\n";
         break;
     case 5:
         str="AT*BANDIND?\r\n";
         break;
     }


     qDebug()<<"<--"<<str;
     write(serial, str.toLatin1().data(), str.length());

     sendindex++;
     sendindex%=MAX_AT_CNT;

     QTime dieTime = QTime::currentTime().addMSecs(100);

     while( QTime::currentTime() < dieTime )

     QCoreApplication::processEvents(QEventLoop::AllEvents, 10);


     int num;
     char *h;
     while ((num = read(serial, rxbuf, 512)) > 0)
     {
         qDebug()<<"read num: "<<num;
         rxbuf[num + 1] = '\0';
         printf("[%s]\n", rxbuf);

          if (strstr(rxbuf, "+CGATT: 1"))
          {
             online=1;
             qDebug()<<1111;
          }
          if (strstr(rxbuf, "+CGATT: 0"))
          {
             online=0;
             qDebug()<<2222;
          }
          if (strstr(rxbuf, "+CSQ: "))
          {
              h=strstr(rxbuf, "+CSQ: ");
              QString str=QString(QLatin1String(h));

              qDebug()<<str;
              csq=atoi(str.mid(6,2).toLatin1().data());

          }
          if (strstr(rxbuf, "+COPS: "))
          {
              h=strstr(rxbuf, "\"");

              QString str=QString(QLatin1String(h+1));

              cops=str.mid(0,str.indexOf("\""));

              qDebug()<<"zzzzzzzzz"<<cops;

          }

          if (strstr(rxbuf, "*BANDIND: "))
          {
              h=strstr(rxbuf, "\,");
              QString str=QString(QLatin1String(h+1));
              band=str.mid(1,1);
              qDebug()<<band;
          }
     }


     update4gs(online,csq,cops,band);
     update();

}

void Widget::update4gs(int flag,int ceq,QString cops,QString band)
{
    mask^=1;
    if(flag==1)
    {
        ui->labelonline->setText("On Line");
        if(mask%2)
            ui->labelonlinepic->setPixmap(QPixmap(":/myp/images/green.jpg"));
        else
            ui->labelonlinepic->setPixmap(QPixmap(":/myp/images/greenb.jpg"));
    }
    else
    {
        ui->labelonline->setText("Off Line");
        if(mask%2)
            ui->labelonlinepic->setPixmap(QPixmap(":/myp/images/red.jpg"));
        else
           ui->labelonlinepic->setPixmap(QPixmap(":/myp/images/redb.jpg"));
    }

    QString temp="CSQ:"+QString::number(ceq);
    ui->labelonlinecsq->setText(temp);

    ui->labelonlinecops->setText("COPS:"+cops);
     ui->labelband->setText("BAND:"+band);
}

