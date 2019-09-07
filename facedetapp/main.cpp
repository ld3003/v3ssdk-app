#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QDebug>
#include "Common/Common.h"

#include "gpio.h"


int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    MainWindow w;


#ifdef arm
    QGraphicsScene scene;
    QGraphicsView view;
    scene.addWidget(&w);//添加到场景
    view.setScene(&scene);//设置当前场景为视图
    view.setWindowFlags(Qt::FramelessWindowHint);//隐藏标题栏
    view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏滚动条
    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏滚动条
    view.resize(DEV_WINDOWS_H, DEV_WINDOWS_W);
    view.show();
    view.rotate(90);
#endif

#ifdef x86_64
    w.show();
#endif

    QApplication::setOverrideCursor(Qt::BlankCursor);
    return a.exec();
}
