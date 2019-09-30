#include "serialreadthread.h"
#include <QDebug>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "widget.h"
serialreadthread::serialreadthread(QObject *obj)
{

}

void serialreadthread::run()
{
    int num;
    char *h;
    while (1)
    {
        while ((num = read(Widget::serial, rxbuf, 512)) > 0)
        {
            qDebug()<<"read num: "<<num;
            rxbuf[num + 1] = '\0';
            printf("[%s]\n", rxbuf);

             if (strstr(rxbuf, "+CGATT: 1"))
             {
                online=1;
                qDebug()<<1111;
             }
             if (strstr(rxbuf, "+CGATT: 0"))
             {
                online=0;
                qDebug()<<2222;
             }
             if (strstr(rxbuf, "+CSQ: "))
             {
                 h=strstr(rxbuf, "+CSQ: ");
                 QString str=QString(QLatin1String(h));

                 qDebug()<<str;
                 csq=atoi(str.mid(6,2).toLatin1().data());

             }


             QMetaObject::invokeMethod(mwgt,"update4gs",Q_ARG(int,online),Q_ARG(int,csq),Q_ARG(QString,QString("ddddd")));
             memset(rxbuf,0,512);

        }

       if (num < 0)
       {
           printf("quit");
           quit();
           wait();
       }
       qDebug()<<"read thread running!!!!";
    }
    quit();
    wait();

}
