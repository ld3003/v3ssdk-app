#ifndef COMPARETHREAD_H
#define COMPARETHREAD_H
#include <QThread>
#include "mtcnn.h"
class CompareThread : public QThread
{
    Q_OBJECT
public:
    CompareThread(QObject *obj);
    std::vector<float> feature1;
    void run();
};

#endif // COMPARETHREAD_H
