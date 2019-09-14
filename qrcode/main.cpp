#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow w;
    w.show();    

    QApplication::setOverrideCursor(Qt::BlankCursor);

    return a.exec();
}
