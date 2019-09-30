#include <opencv2/opencv.hpp>
#include <iostream>
#include <pthread.h>

static cv::VideoCapture *cap;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int opencam()
{
}

int getpic2(void)
{

#if 0
    cv::Mat pic;

    for (int i = 0; i < 3; i++)
    {
        pthread_mutex_lock(&mutex);
        *cap >> pic;
        pthread_mutex_unlock(&mutex);
    }

#endif

    return 1;
}

bool save_to_file(const std::vector<uchar> &content)
{

    FILE *fp = fopen("/tmp/tmp.jpg", "wb+");
    if (!fp)
        return false;

    size_t len = content.size();

    bool save_success = (len == fwrite(&content[0], 1, len, fp));
    if (!save_success)
    {
        printf("write errpor !!!\n");
    }

    fflush(fp);
    fclose(fp);

    return save_success;
}

#if 0
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

void Jpegcompress(const cv::Mat &src, cv::Mat &dest, int quality)
{
    std::vector<uchar> buff;
    std::vector<int> params;
    /*IMWRITE_JPEG_QUALITY For JPEG, it can be a quality from 0 to 100 
	(the higher is the better). Default value is 95 */
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(quality);
    //将图像压缩编码到缓冲流区域
    cv::imencode(".jpg", src, buff, params);
    //将压缩后的缓冲流内容解码为Mat，进行后续的处理
    dest = cv::imdecode(buff, -1);
    cv::imshow("src", src);
    cv::imshow("dst", dest);
}

int main()
{
    std::string fileName = "test.jpg";
    cv::Mat src = cv::imread(fileName, -1);
    if (src.empty())
    {
        std::cerr << " image open error!\n";
        return 0;
    }
    cv::Mat dest;
    Jpegcompress(src, dest, 50);
    cvWaitKey(0);
    return 0;
}
#endif
#include "gpio.h"
int getpic(void)
{
    int loop = 30;
    cv::Mat pic;

    GPIO_SetPin(PE, 17, 1);
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

    printf("opencamera success1\n");
    for (int i = 0; i < loop; i++)
    {
        pthread_mutex_lock(&mutex); 
        *cap >> pic;
        pthread_mutex_unlock(&mutex);
    }

    printf("opencamera success4\n");

    GPIO_SetPin(PE, 17, 0);

    if (pic.empty())
        return -1;
    printf("opencamera success5\n");
    std::vector<uchar> buff;
    std::vector<int> params;
    /*IMWRITE_JPEG_QUALITY For JPEG, it can be a quality from 0 to 100 
	(the higher is the better). Default value is 95 */
    params.push_back(cv::IMWRITE_JPEG_QUALITY); //IMWRITE_JPEG_RST_INTERVAL
    params.push_back(90);
    //将图像压缩编码到缓冲流区域
    cv::imencode(".jpg", pic, buff, params);

    save_to_file(buff);

#if 0
    vector<byte> string_from_filename(const char * filename_)
{
    vector<byte> bytes;
   if (!filename_ || filename_[0] == '\0') return bytes;
    FILE* fp = fopen( filename_, "rb" );
    if (!fp)
    {
        return bytes;
    }
    
    // Get file length
    fseek(fp, 0, SEEK_END);
    int len = ftell( fp );
    fseek(fp, 0, SEEK_SET);
    
    bytes.resize(len);
    fread(&bytes[0], len, 1, fp);
    fclose(fp);
    
    return bytes;
}
#endif

    std::vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(100);
    cv::imwrite("/tmp/tmp.jpg", pic, compression_params);

    //exit(0);

    return 1;
}
