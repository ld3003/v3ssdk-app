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
#define KEY1 16777220
#define KEY2 0
#define KEY3 16777328
#define KEY4 16777330
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

    system("sh /root/bin/hostap.sh");

  //  system("udhcpc -i eth0");

    foreach(const QHostAddress& hostAddress,QNetworkInterface::allAddresses())
    if ( hostAddress != QHostAddress::LocalHost && hostAddress.toIPv4Address() )
    {
        ui->textEdit->append(hostAddress.toString());
    }

    QFont ft;
    ft.setPointSize(14);
    ui->labelport->setFont(ft);
    ui->labelport_2->setFont(ft);
    ui->labelport_3->setFont(ft);
    ui->labelport_4->setFont(ft);
    ui->labelport_5->setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    ui->labelport->setPalette(pa);
    ui->labelport_2->setPalette(pa);
    ui->labelport_3->setPalette(pa);
    ui->labelport_4->setPalette(pa);
    ui->labelport_5->setPalette(pa);
    sthd=new serverthread(this);
    sthd->start();

    ui->labelport->setText("server port:"+QString("%1").arg(SERVERPORT));
    if(ui->textEdit->toPlainText()!="")
    {
        QStringList list = ui->textEdit->toPlainText().split("\n");
        QString str=QString("%1:%2").arg(list[0]).arg(SERVERPORT);
        ui->labelport_5->setText(str);
    }

    QFont ftt;
    ftt.setPointSize(18);
    QPalette paa;
    paa.setColor(QPalette::WindowText,Qt::red);
    ui->labelstat->setFont(ftt);
    ui->labelstat->setPalette(paa);
    ui->labelstat->setText("Client:0");


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

void Widget::updateclientcnt(QString str)
{
    ui->labelstat->setText("Client:"+str);
}


