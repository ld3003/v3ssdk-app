#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "serverthread.h"

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
public slots:
    void updateclientcnt(QString str);

private:
    Ui::Widget *ui;
};
extern Widget* myW;
#endif // WIDGET_H
