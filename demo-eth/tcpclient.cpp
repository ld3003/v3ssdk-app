#include "tcpclient.h"
#include "widget.h"
#include <QByteArray>
tcpclient::tcpclient(QObject *parent) : QObject(parent)
{

}

void tcpclient::setclient(QTcpSocket *skt)
{
    m_pClient=skt;
    this->m_pClient->setParent(this);
}

void tcpclient::readmsg()
{
    printf("read msg..");
    QByteArray ba=m_pClient->readAll();
    myW->wirtetext(ba);

    m_pClient->write("hello,this is the answer!");
}

void tcpclient::ondisconnected()
{
    deleteLater();
}
