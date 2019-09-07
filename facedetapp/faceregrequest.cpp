#include "faceregrequest.h"
#include "Communiction.h"
#include <opencv2/opencv.hpp>
#include <QThread>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDebug>
#include <QMetaObject>
#include "mainwindow.h"
#include <QApplication>

using namespace std;
using namespace cv;

static unsigned char runcount;


inline bool DeleteFileOrFolder( const QString& strPath )
{
    if( strPath.isEmpty() || !QDir().exists( strPath ) )
        return false;

    QFileInfo fileInfo( strPath );

    if( fileInfo.isFile() )
        QFile::remove( strPath );
    else if( fileInfo.isDir() )
    {
        QDir qDir( strPath );
        qDir.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );
        QFileInfoList fileInfoLst = qDir.entryInfoList();
        foreach( QFileInfo qFileInfo, fileInfoLst )
        {
            if( qFileInfo.isFile() )
                qDir.remove( qFileInfo.absoluteFilePath() );
            else
            {
                DeleteFileOrFolder( qFileInfo.absoluteFilePath() );
                qDir.rmdir( qFileInfo.absoluteFilePath() );
            }
        }
        qDir.rmdir( fileInfo.absoluteFilePath() );
    }

    return true;
}

FaceRegRequest::FaceRegRequest(Mat face)
{
    mFace = face;
    communiction_init(&mCOMMUNICTION);

}

FaceRegRequest::~FaceRegRequest()
{
}

void FaceRegRequest::setFace(cv::Mat face)
{
    mFace = face;
}
void FaceRegRequest::run()
{
    char filename[64];

    struct timeval gTpstart ,gTpend;
    time_consuming_start(&gTpstart,&gTpend);
    //time_consuming_print("detect time",&gTpstart,&gTpend);

    snprintf(filename,64,"/tmp/img%d.jpg",runcount++);
    COMMUNICTION_RESULT resu;
    PICTURE pic;
    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(50);
    cv::imwrite(filename, mFace, compression_params);
    pic.path = (nh_u8*)filename;
    communiction_pushpic(&mCOMMUNICTION,&pic,&resu);

    DeleteFileOrFolder(QString(filename));

    QMetaObject::invokeMethod(mw,"tipmsg",Q_ARG(QString,QString((char*)resu.resp)));

}
