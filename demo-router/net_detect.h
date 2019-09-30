#ifndef NET_DETECT_H
#define NET_DETECT_H

#include <QThread>
#include <QProcess>
class net_detect : public QThread
{
public:
    net_detect();
    void run();
};

#endif // NET_DETECT_H
