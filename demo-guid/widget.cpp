#include "widget.h"
#include "ui_widget.h"
#include <stdlib.h>
#include <QDebug>
#include <QFont>
#include <QPalette>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QFontDatabase>
#include "processmgr.h"
#define KEY1 16777220   
#define KEY2 0
#define KEY3 16777328
#define KEY4 16777330  //next

#define KEY_HOME 16777330
#define KEY_ENTER 16777220
#define KEY_NEXT 16777328
#define KEY_PREV 65

#include <QtGui/QApplication>
#include <QTextCodec>   /**1.添加此头文件**/
 
#if 0
int main(int argc, char *argv[])
{
    /**2.添加下面三句话**/ 
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
 
    QApplication a(argc, argv);
    FindDialog *dialog = new FindDialog;
    /**3.添加要使用的字库，和字体大小，wpy-zenhei即为字库名**/
    dialog->setFont(QFont("wqy-zenhei",14,QFont::Normal));
    dialog->show();
 
    return a.exec();
}

#endif


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

    if(event->key()==KEY_NEXT)
    {
        key_index++;
        key_index%=6;
    }

    if(event->key()==KEY_PREV)
    {
        key_index--;
        key_index%=6;
    }



    qDebug()<<key_index;

    setlabeltext(key_index);

    /*
    if(key_index%2==0)
        ui->pushButton_2->setStyleSheet("border-image: url(:/pic/images/sml.jpg);");
    else
        ui->pushButton_2->setStyleSheet("border-image: url(:/pic/images/smlxz.jpg);");
    */

    if(event->key()==KEY_ENTER)
    {

	    killprocess("udhcpc");
	    killprocess("hostapd");
	    killprocess("udhcpd");


        if(key_index==1)
            system("/root/bin/demo-qrcode -qws");
        else if(key_index==0)
            system("/root/bin/demo-facenet -qws");
        else if(key_index==2)
            system("/root/bin/demo-router -qws");
        else if(key_index==3)
            system("/root/bin/demo-mjpgsrv -qws");
        else if(key_index==4)
            system("/root/bin/demo-cloud -qws");
        else if(key_index==5)
            system("/root/bin/demo-eth -qws");

    }
    update();

}

void Widget::setlabeltext(unsigned int index)
{
    QString str="";
    switch(index)
    {
    case 0:
        str="niti呵呵";
        ui->pushButton->setStyleSheet("border-image: url(:/pic/images/facexz.jpg);");
        ui->pushButton_2->setStyleSheet("border-image: url(:/pic/images/sml.jpg);");
        ui->pushButton_3->setStyleSheet("border-image: url(:/pic/images/trans.jpg);");
        ui->pushButton_4->setStyleSheet("border-image: url(:/pic/images/camera.jpg);");
        ui->pushButton_5->setStyleSheet("border-image: url(:/pic/images/cloud.jpg);");
        ui->pushButton_6->setStyleSheet("border-image: url(:/pic/images/eth.jpg);");
        break;
    case 1:
        str="Sweep code";
        ui->pushButton->setStyleSheet("border-image: url(:/pic/images/face.jpg);");
        ui->pushButton_2->setStyleSheet("border-image: url(:/pic/images/smlxz.jpg);");
        ui->pushButton_3->setStyleSheet("border-image: url(:/pic/images/trans.jpg);");
        ui->pushButton_4->setStyleSheet("border-image: url(:/pic/images/camera.jpg);");
        ui->pushButton_5->setStyleSheet("border-image: url(:/pic/images/cloud.jpg);");
        ui->pushButton_6->setStyleSheet("border-image: url(:/pic/images/eth.jpg);");
        break;
    case 2:
        str="transmission";
        ui->pushButton->setStyleSheet("border-image: url(:/pic/images/face.jpg);");
        ui->pushButton_2->setStyleSheet("border-image: url(:/pic/images/sml.jpg);");
        ui->pushButton_3->setStyleSheet("border-image: url(:/pic/images/transxz.jpg);");
        ui->pushButton_4->setStyleSheet("border-image: url(:/pic/images/camera.jpg);");
        ui->pushButton_5->setStyleSheet("border-image: url(:/pic/images/cloud.jpg);");
        ui->pushButton_6->setStyleSheet("border-image: url(:/pic/images/eth.jpg);");
        break;
    case 3:
        str="Camera";
        ui->pushButton->setStyleSheet("border-image: url(:/pic/images/face.jpg);");
        ui->pushButton_2->setStyleSheet("border-image: url(:/pic/images/sml.jpg);");
        ui->pushButton_3->setStyleSheet("border-image: url(:/pic/images/trans.jpg);");
        ui->pushButton_4->setStyleSheet("border-image: url(:/pic/images/cameraxz.jpg);");
        ui->pushButton_5->setStyleSheet("border-image: url(:/pic/images/cloud.jpg);");
        ui->pushButton_6->setStyleSheet("border-image: url(:/pic/images/eth.jpg);");
        break;
    case 4:
        str="Cloud";
        ui->pushButton->setStyleSheet("border-image: url(:/pic/images/face.jpg);");
        ui->pushButton_2->setStyleSheet("border-image: url(:/pic/images/sml.jpg);");
        ui->pushButton_3->setStyleSheet("border-image: url(:/pic/images/trans.jpg);");
        ui->pushButton_4->setStyleSheet("border-image: url(:/pic/images/cameraxz.jpg);");
        ui->pushButton_5->setStyleSheet("border-image: url(:/pic/images/cloudyy.jpg);");
        ui->pushButton_6->setStyleSheet("border-image: url(:/pic/images/eth.jpg);");
        break;
    case 5:
        str="Ethernet interface";
        ui->pushButton->setStyleSheet("border-image: url(:/pic/images/face.jpg);");
        ui->pushButton_2->setStyleSheet("border-image: url(:/pic/images/sml.jpg);");
        ui->pushButton_3->setStyleSheet("border-image: url(:/pic/images/trans.jpg);");
        ui->pushButton_4->setStyleSheet("border-image: url(:/pic/images/cameraxz.jpg);");
        ui->pushButton_5->setStyleSheet("border-image: url(:/pic/images/cloud.jpg);");
        ui->pushButton_6->setStyleSheet("border-image: url(:/pic/images/ethyy.jpg);");
        break;
    }
#if 0
	int fontId = QFontDatabase::addApplicationFont("/root/");
QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
qDebug() << "font list " << fontFamilies;
// 创建字体
QFont font;
font.setFamily(fontFamilies.at(0)); //FontAwesome
font.setPointSize(20);
#endif
    /**2.添加下面三句话**/ 
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
 
    /**3.添加要使用的字库，和字体大小，wpy-zenhei即为字库名**/
    ui->labelmsg->setFont(QFont("simhei",14,QFont::Normal));


    ui->labelmsg->setText(str);

}
