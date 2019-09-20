#include "widget.h"
#include "ui_widget.h"
#include <stdlib.h>
#include <QDebug>
#include <QFont>
#include <QPalette>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#define KEY1 16777220
#define KEY2 0
#define KEY3 16777328
#define KEY4 16777330
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowOpacity(0.7);
    setAttribute(Qt::WA_TranslucentBackground, true);

    key_index=0;
   // ui->labelmsg->setText(tr("这是QLabel文本"));
    setlabeltext(0);
    QFont ft;
    ft.setPointSize(24);
    ui->labelmsg->setFont(ft);

    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    ui->labelmsg->setPalette(pa);
    ui->pushButton->setStyleSheet("border-image: url(:/pic/images/facexz.jpg);");

    setStyleSheet("background-image: url(:/pic/images/back.png);");

}

Widget::~Widget()
{
    delete ui;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<event->key();

    if(event->key()==KEY1)
    {
        key_index++;
        key_index%=4;
    }
    qDebug()<<key_index;

    setlabeltext(key_index);
    /*
    if(key_index%2==0)
        ui->pushButton_2->setStyleSheet("border-image: url(:/pic/images/sml.jpg);");
    else
        ui->pushButton_2->setStyleSheet("border-image: url(:/pic/images/smlxz.jpg);");
    */

    if(event->key()==KEY4)
    {
        if(key_index==1)
            system("./qrcode -qws");
        else if(key_index==0)
            system("./facenet -qws");
    }
    update();

}

void Widget::setlabeltext(unsigned int index)
{
    QString str="";
    switch(index)
    {
        case 0:
        str="face recognition";
        ui->pushButton->setStyleSheet("border-image: url(:/pic/images/facexz.jpg);");
        ui->pushButton_2->setStyleSheet("border-image: url(:/pic/images/sml.jpg);");
        ui->pushButton_3->setStyleSheet("border-image: url(:/pic/images/trans.jpg);");
        ui->pushButton_4->setStyleSheet("border-image: url(:/pic/images/camera.jpg);");
        break;
    case 1:
        str="Sweep code";
        ui->pushButton->setStyleSheet("border-image: url(:/pic/images/face.jpg);");
        ui->pushButton_2->setStyleSheet("border-image: url(:/pic/images/smlxz.jpg);");
        ui->pushButton_3->setStyleSheet("border-image: url(:/pic/images/trans.jpg);");
        ui->pushButton_4->setStyleSheet("border-image: url(:/pic/images/camera.jpg);");
        break;
    case 2:
        str="transmission";
        ui->pushButton->setStyleSheet("border-image: url(:/pic/images/face.jpg);");
        ui->pushButton_2->setStyleSheet("border-image: url(:/pic/images/sml.jpg);");
        ui->pushButton_3->setStyleSheet("border-image: url(:/pic/images/transxz.jpg);");
        ui->pushButton_4->setStyleSheet("border-image: url(:/pic/images/camera.jpg);");
        break;
    case 3:
        str="Camera";
        ui->pushButton->setStyleSheet("border-image: url(:/pic/images/face.jpg);");
        ui->pushButton_2->setStyleSheet("border-image: url(:/pic/images/sml.jpg);");
        ui->pushButton_3->setStyleSheet("border-image: url(:/pic/images/trans.jpg);");
        ui->pushButton_4->setStyleSheet("border-image: url(:/pic/images/cameraxz.jpg);");
        break;
    }
    ui->labelmsg->setText(str);

}
