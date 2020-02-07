#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "serverthread.h"
#include "connthread.h"

#include <QTimer>
#define SERVERPORT 7888
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    serverthread* sthd;
    connthread* connthd;

     void keyPressEvent(QKeyEvent *event);
     QString m_strip;
     quint16 m_port;
     void getconfig();
     QTimer* m_pinitTimer;
     QString m_strlocalip;
     void startserver();

public slots:
    void updateclientcnt(QString str);
    void inittimeout();
private:
    Ui::Widget *ui;
};

extern Widget* myW;
#endif // WIDGET_H
