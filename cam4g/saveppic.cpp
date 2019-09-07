#include <opencv2/opencv.hpp>
#include <iostream>
#include <pthread.h>

static cv::VideoCapture *cap;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int opencam()
{

    cv::Mat pic;

    cap = new cv::VideoCapture(0);

    //cap->set(CV_CAP_PROP_FRAME_WIDTH, 1920);
    //cap->set(CV_CAP_PROP_FRAME_HEIGHT, 1080);


    if (cap->isOpened())
    {
        printf("opencamera success\n");
    }
    else
    {
        printf("opencamera error\n");
        return -1;
    }

    return 0;
}

int getpic2(void)
{
    cv::Mat pic;

    for (int i = 0; i < 3; i++)
    {
        pthread_mutex_lock(&mutex);
        *cap >> pic;
        pthread_mutex_unlock(&mutex);
    }

    return 1;
}

int getpic(void)
{
   int loop = 3;
    cv::Mat pic;

    for(int i=0;i<loop;i++)
    {
	    pthread_mutex_lock(&mutex);
	    *cap >> pic;
	    pthread_mutex_unlock(&mutex);
	    if (!pic.empty())
		    break;
    }

    if (pic.empty())
	    return -1;

    std::vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(90);
    cv::imwrite("/tmp/tmp.jpg", pic, compression_params);

    return 1;
}
