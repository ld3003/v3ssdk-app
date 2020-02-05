#include "ServerClass.h"
#include "MjpgServerClass.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <opencv2/opencv.hpp>

MjpgServerClass g_MjpgServer;

unsigned int g_Img1Size;
unsigned char * g_Img1Data;
unsigned int g_Img2Size;
unsigned char * g_Img2Data;
cv::VideoCapture *g_cap;


/*
//http://answers.opencv.org/question/6976/display-iplimage-in-webbrowsers/
// convert the image to JPEG ( in memory! )

std::vector<uchar>outbuf;
std::vector<int> params;
params.push_back(CV_IMWRITE_JPEG_QUALITY);
params.push_back(100);
cv::imencode(".jpg", frame, outbuf, params);
*/



void Load_File(const char * name,unsigned char ** out_data, unsigned int * out_data_size)
{
    unsigned char * data = NULL;
    unsigned int data_size = 0;
    unsigned int bytes_read = 0;

    FILE *f = fopen(name,"r");
    if (f != NULL)
    {
        // size of the file
        struct stat file_stats;
        stat(name,&file_stats);
        data_size = file_stats.st_size;

        // allocate memory and read in the bytes
        data = new unsigned char[data_size];
        memset(data,0,data_size);
        bytes_read = fread(data,1,data_size,f);
        fclose(f);
    }

    if (out_data != NULL)
    {
        *out_data = data;
    }
    if (out_data_size != NULL)
    {
        *out_data_size = data_size;
    }

    FILE * fout = fopen("Test.jpg","w");
    if (fout != NULL)
    {
        fwrite(data,1,data_size,fout);
        fclose(fout);
    }
}

int main(int argc,char ** argv)
{
    char dir[1024];
    getcwd(dir,sizeof(dir));
    printf("Current Directory: %s\r\n",dir);

    g_MjpgServer.Init(9870);

    // load two jpg files
    Load_File("Motorcycle.jpg",&g_Img1Data, &g_Img1Size);
    Load_File("Bart.jpg",&g_Img2Data, &g_Img2Size);


     g_cap = new cv::VideoCapture(1);

     if (g_cap->isOpened())
     {
	     printf("opencamera success\n");
     }
     else
     {
	     printf("opencamera error\n");
	     return -1;
     }

printf("release !!!!!!!!!!!!!!!!!!!!!!!!!!!!!1\n");
	g_cap->release();

     delete g_cap;

    sleep(1);

     g_cap = new cv::VideoCapture(1);

     if (g_cap->isOpened())
     {
	     printf("opencamera success\n");
     }
     else
     {
	     printf("opencamera error\n");
	     return -1;
     }



    for (;;)
    {


	    cv::Mat frame;
	    *g_cap >> frame;
	    if (frame.empty())
		    continue;

	    std::vector<unsigned char> outbuf;
	    std::vector<int> params;

	    params.push_back(CV_IMWRITE_JPEG_QUALITY);
	    params.push_back(50);
	    cv::imencode(".jpg", frame, outbuf, params);

	    printf("img len %d \n",outbuf.size());

	    g_MjpgServer.Send_New_Image(outbuf.data(),(unsigned int)outbuf.size());
	    g_MjpgServer.Process();
    }


    return 0;
}
