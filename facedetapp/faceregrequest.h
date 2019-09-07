#ifndef FACEREGREQUEST_H
#define FACEREGREQUEST_H

#include <QObject>
#include <QRunnable>
#include <opencv2/opencv.hpp>
#include "Communiction.h"

class FaceRegRequest : public QRunnable
{
public:
    FaceRegRequest(cv::Mat face);
    ~FaceRegRequest();
    void setFace(cv::Mat face);

public:
    void run();

private:
    cv::Mat mFace;
    COMMUNICTION mCOMMUNICTION;

};

#endif // FACEREGREQUEST_H
