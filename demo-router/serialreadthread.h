#ifndef SERIALREADTHREAD_H
#define SERIALREADTHREAD_H

#include <QObject>
#include <QThread>

class serialreadthread : public QThread
{
    Q_OBJECT
public:
    serialreadthread(QObject *obj);
    char rxbuf[512];
    int online;
    int csq;
    QString strapn;
    void run();
};

#endif // SERIALREADTHREAD_H
