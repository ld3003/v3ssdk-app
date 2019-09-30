#include "comparethread.h"
#include "Common/Common.h"
#include "mainwindow.h"

#include "mobilefacenet.h"
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <QDebug>

CompareThread::CompareThread(QObject *obj)
{

}
/*


        Recognize recognize(model_path);
        recognize.start(img1, feature1);
        qDebug()<<feature1.size();
*/
void CompareThread::run()
{
    for(;;)
    {//计算实时特征值，对比历史特征值


        if(MainWindow::needcmp==0)
        {
           // qDebug("RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR");
            usleep(1);
            continue;
        }

        if((!MainWindow::realshort.empty())&&(!MainWindow::screenshort.empty()))
        {//先拷贝出来
            qDebug()<<"zzzzzzzzzzzzzzz";
            clock_t start_time = clock();
             qDebug()<<"bbbbbbbbbbbbbbbb";
             char *model_path = "/root";
             Recognize recognize(model_path);
            qDebug()<<"ddddddddddddddddd";

             std::vector<float> feature2;

             if(MainWindow::sampneedinit==1)
             {
                recognize.start(MainWindow::screenshort, feature1);
                MainWindow::sampneedinit=0;
             }

             qDebug()<<"ffffffffffffffffffffffff";
             recognize.start(MainWindow::realshort, feature2);
             clock_t finish_time = clock();
             double total_time = (double)(finish_time - start_time) / CLOCKS_PER_SEC;
             double similar = calculSimilar(feature1, feature2);
            // double euc_d=euclidean_distance(feature1, feature2);

             clock_t duibi_time = clock();
             double totaldb_time = (double)(duibi_time - finish_time) / CLOCKS_PER_SEC;

             qDebug()<<"*********************************"<<similar<<total_time<<totaldb_time;

            QString str= QString("score:%1").arg(similar);
#if 1
            cv::String name=cv::format("%f.jpg",similar);


            imwrite(name, MainWindow::realshort);
#endif
             QMetaObject::invokeMethod(mw,"tipmsg",Q_ARG(QString,QString(str)));

             MainWindow::needcmp=0;
        }

    }
}
