#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <iostream>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "camthread.h"
#include "detectthread.h"
#include "comparethread.h"
#include <QLabel>
#include <QTimer>
#include <QKeyEvent>
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void detectFace();
    cv::Mat QImageToMat(QImage image);
    static cv::Mat screenshort;
    static cv::Mat realshort;
    static std::vector<float> feature1;
    static int sampneedinit;
    static int needcmp;
    ~MainWindow();
public slots:
    void imgFlush(QImage img);
    void tipmsg(QString);
    void tipmsgTimeout();
    void syschecktimeout();
    void openDoor();
    void keyPressEvent(QKeyEvent *event);
private:
    Ui::MainWindow *ui;
    CamThread *camThread;
    DetectThread *detThread;
    CompareThread* cmpThread;

    QLabel *topBar;
    QLabel *tipLable;
    QLabel *screenshortLable;
    QLabel *resultLable;
    QLabel *more;
    QTimer *tipMsgtimer;
    QTimer *syschecktimer;

    int a;


};

extern MainWindow *mw;

#endif // MAINWINDOW_H
