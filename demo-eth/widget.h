#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTimer>
#define local_port 7676
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QTcpServer* m_ptcpserver;

    void initserver();
    QString m_strLanip;
    void getlanip();
    QTimer* m_pinittimer;

    void wirtetext(QByteArray ba);

    void keyPressEvent(QKeyEvent *event);
private:
    Ui::Widget *ui;
public slots:
    void on_inittimeout();
    void newconnect();
};
extern Widget* myW;
#endif // WIDGET_H
