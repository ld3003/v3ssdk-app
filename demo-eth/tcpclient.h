#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
class tcpclient : public QObject
{
    Q_OBJECT
public:
    explicit tcpclient(QObject *parent = 0);

    QTcpSocket* m_pClient;
      void setclient(QTcpSocket *skt);
signals:

public slots:
    void readmsg();
    void ondisconnected();
};

#endif // TCPCLIENT_H
