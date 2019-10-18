#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QTime>
#include <QCoreApplication>
#include <QFont>
#include <QGroupBox>
#include <QKeyEvent>
#include <QNetworkInterface>

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

    sendindex=0;
    m_4gip="";
    m_lanip="";


    QFont ft;
    ft.setPointSize(14);
    ui->labelonline->setFont(ft);
    ui->labelonlinecsq->setFont(ft);
    ui->labelonlinecops->setFont(ft);
    ui->labelband->setFont(ft);

    ui->groupBox->setFont(ft);
    ui->groupBox_2->setFont(ft);
    ui->label->setFont(ft);
    ui->label_2->setFont(ft);
    ui->label_3->setFont(ft);









    system("/root/bin/usb-set-hostmode.sh");
    qDebug()<<"***************************************** run 4g";


    system("/root/bin/hostap.sh");

    qDebug()<<"***************************************** run wifi";
    delay(5000);

    //system("udhcpc -i eth0 &");
    system("udhcpc -i eth1 &");

    delay(5000);
    getip();



    isworking=islanworking();
    qDebug()<<"****************************************4g ip="<<m_4gip<<",lan ip="<<m_lanip<<"Lan working:"<<isworking;


    hastabled=FALSE;
  //  p1=new QProcess();
  // p2=new QProcess();

    configserial();


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


        qDebug()<<"start ask timer!";
        m_pTimer=new QTimer;

        connect(m_pTimer,SIGNAL(timeout()),this,SLOT(ask_timeout()));
        m_pTimer->start(2000);
    }
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==KEY3)
    {
      exit(0);
    }
}

void Widget::getip()
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
                    m_4gip=entry.ip().toString();
            }
            else  if(interface.name()=="eth0")
            {
                if(entry.ip().protocol()==QAbstractSocket::IPv4Protocol)
                    m_lanip=entry.ip().toString();
            }

        }
   }
}

bool Widget::islanworking()
{
    QString network_cmd = "ping www.baidu.com -c 1 -w 500";
    QString result;
    QProcess *network_process;
    network_process = new QProcess();    //不要加this
    network_process->start(network_cmd);   //调用ping 指令
    network_process->waitForFinished();    //等待指令执行完毕
    result = network_process->readAll();   //获取指令执行结果
    if(result.contains(QString("ttl=")))   //若包含TTL=字符串则认为网络在线
    {
       return TRUE;
    }
    else
    {
        return FALSE;
    }

}

void Widget::iptable(QString desip)
{
    system("iptables -t nat -F");
    system("iptables -t nat -X");
    system("iptables -t nat -Z");
    QString str=QString("iptables -t nat -A POSTROUTING -s 192.168.10.0/24 -j SNAT --to %1").arg(desip);
    qDebug()<<str;
    system(str.toLocal8Bit().data());

    //192.168.2.138
}

void Widget::delay(int ms)
{
    QTime dieTime= QTime::currentTime().addMSecs(ms);
     while( QTime::currentTime() < dieTime )
     QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void Widget::ask_timeout()
{
    unsigned int hasack=0;

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

          hasack=1;
     }


     update4gs(online,csq,cops,band);
     update();

     if(hastabled==FALSE)
     {
         //p1->start("udhcpc -i eth0");
        // p2->start("udhcpc -i eth1");


         if(islanworking())
         {
            getip();
            qDebug()<<"****************************************4g ip="<<m_4gip<<",lan ip="<<m_lanip<<"Lan working:"<<islanworking();

            if(hasack==1)
            {
                if(online==1)
                {//4g
                    if(m_4gip!="")
                    {
                        iptable(m_4gip);
                        hastabled=TRUE;
                        ui->label_3->setText("Internet:use 4G");
                        qDebug()<<"<<<<<<<<<4G router>>>>>>>>";
                    }
                }
                else
                {//lan
                    if(m_lanip!="")
                    {
                        hastabled=TRUE;
                        iptable(m_lanip);
                        ui->label_3->setText("Internet:use Lan");
                        qDebug()<<"<<<<<<<<<LAN router>>>>>>>>";
                    }
                }
            }
            else
            {
                if(m_lanip!="")
                {
                    hastabled=TRUE;
                    iptable(m_lanip);
                    ui->label_3->setText("Internet:use Lan");

                }
            }
        }
     }

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

