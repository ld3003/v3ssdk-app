#ifndef CONNTHREAD_H
#define CONNTHREAD_H

#include <QObject>
#include <QThread>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

class connthread : public QThread
{
    Q_OBJECT
public:
    connthread(QObject *obj);
    void run();
};

#endif // SERVERTHREAD_H
