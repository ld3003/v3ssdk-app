#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>
#include <QPixmap>
#include <QTextFormat>
#include <QKeyEvent>
#define KEY3 16777330
MainWindow *mw;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    mw = this;
    ui->setupUi(this);

    showFullScreen();


    camThread = new CamThread(0);
    detThread = new DetectThread(camThread);

    connect(camThread,SIGNAL(imgReady(QImage)),this,SLOT(imgFlush(QImage)));
    connect(detThread,SIGNAL(tipmsg(QString)),this,SLOT(tipmsg(QString)));

    camThread->start();
    detThread->start();

    tipLable = new QLabel(this);
    tipLable->setGeometry(0,0,this->width(),400);
    tipLable->hide();

    QFont ft;
    ft.setPointSize(80);
    tipLable->setFont(ft);

    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    tipLable->setPalette(pa);

    tipLable->setAlignment(Qt::AlignCenter);
    tipLable->setWordWrap(true);
    tipLable->setAlignment(Qt::AlignTop);
    tipMsgtimer = new QTimer(this);
    connect(tipMsgtimer,SIGNAL(timeout()),this,SLOT(tipmsgTimeout()));

}

QString AutoFeed(QString text)
{
     QString strText = text;
     int AntoIndex = 1;
     if(!strText.isEmpty())
     {
 
         for(int i = 1;i<strText.size() + 1;i++)//25个字符换一行
         {
             if(i == 18*AntoIndex + AntoIndex -1)
             {
                 strText.insert(i,"\n");
                 AntoIndex ++;
             }
 
         }
     }
     return strText;
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
    tipLable->setText(AutoFeed(str));
    //tipLable->setTextFormat()
    //
    tipMsgtimer->start(1000);
}
void MainWindow::tipmsgTimeout()
{
    tipLable->setText(QString());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==KEY3)
    {
        exit(0);
    }
}
