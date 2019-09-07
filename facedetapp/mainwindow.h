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

#include <QLabel>
#include <QTimer>

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
    ~MainWindow();
public slots:
    void imgFlush(QImage img);
    void tipmsg(QString);
    void tipmsgTimeout();
    void syschecktimeout();
    void openDoor();

private:
    Ui::MainWindow *ui;
    CamThread *camThread;
    DetectThread *detThread;

    QLabel *topBar;
    QLabel *tipLable;
    QTimer *tipMsgtimer;
    QTimer *syschecktimer;

    int a;


};

extern MainWindow *mw;

#endif // MAINWINDOW_H
