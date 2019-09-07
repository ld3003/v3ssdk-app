#include "Communiction.h"
#include "curl/curl.h"
#include "Common.h"

#include <stdio.h>
#include <curl/curl.h>

#define  REQURL "http://demo.zxhtong.com/camera_upload.html"

static unsigned char reqbuffer[512];

size_t  write_data(void *ptr, size_t size, size_t nmemb, void *stream) {

    printf("\n\n\n[%s]\n\n\n\n",ptr);
    //int written = fwrite(ptr, size, nmemb, (FILE *)fp);
    snprintf(reqbuffer,sizeof(reqbuffer),"%s",ptr,nmemb);
    return nmemb;
}


NH_ERRCODE communiction_init(COMMUNICTION *comm)
{
    return NH_ERR_NOERR;
}

//

NH_ERRCODE communiction_pushpic2(COMMUNICTION *comm, PICTURE *pic, COMMUNICTION_RESULT *result)
{

    char *url= "http://demo.zxhtong.com/camera_upload.html";
    CURL *pCurl = NULL;
    CURLcode res;

    struct curl_slist *headerlist = NULL;

    struct curl_httppost *post = NULL;
    struct curl_httppost *last = NULL;

    curl_formadd(&post, &last,CURLFORM_COPYNAME, "file", //此处表示要传的参数名
            CURLFORM_FILE, "/tmp/outImage.jpg",                               //此处表示图片文件的路径
            CURLFORM_CONTENTTYPE, "image/jpeg",
            CURLFORM_END);

    pCurl = curl_easy_init();

    if (NULL != pCurl)
    {
        curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 5);
        curl_easy_setopt(pCurl, CURLOPT_URL, url);
        curl_easy_setopt(pCurl, CURLOPT_HTTPPOST, post);

        curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, write_data);

        res = curl_easy_perform(pCurl);

        if (res != CURLE_OK)
        {
            printf("curl_easy_perform() failed，error code is:%s\n", curl_easy_strerror(res));
        }
        printf("\n");

        curl_easy_cleanup(pCurl);

        printf("%s\n",reqbuffer);

    }

    //result->tmp = 0;
    if (strstr(reqbuffer,"faceIndex"))
    {
        printf("!!!!!!!!!!!!!!!!OK\n");
        //result->tmp = 1;
    }


    return NH_ERR_NOERR;
}

NH_ERRCODE communiction_pushpic(COMMUNICTION *comm, PICTURE *pic, COMMUNICTION_RESULT *result)
{

    char *url= REQURL;
    CURL *pCurl = NULL;
    CURLcode res;

    struct curl_slist *headerlist = NULL;

    struct curl_httppost *post = NULL;
    struct curl_httppost *last = NULL;

    curl_formadd(&post, &last,CURLFORM_COPYNAME, "file", //此处表示要传的参数名
            CURLFORM_FILE, pic->path,                               //此处表示图片文件的路径
            CURLFORM_CONTENTTYPE, "image/jpeg",
            CURLFORM_END);

    pCurl = curl_easy_init();

    if (NULL != pCurl)
    {
        curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 5);
        curl_easy_setopt(pCurl, CURLOPT_URL, url);
        curl_easy_setopt(pCurl, CURLOPT_HTTPPOST, post);

        //curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, write_data);

        res = curl_easy_perform(pCurl);

        if (res != CURLE_OK)
        {
            printf("curl_easy_perform() failed，error code is:%s\n", curl_easy_strerror(res));
        }
        printf("\n");

        curl_easy_cleanup(pCurl);

        printf("%s\n",reqbuffer);

    }

    return NH_ERR_NOERR;

    result->tmp = 0;
    if (strstr(reqbuffer,"faceIndex"))
    {
        printf("!!!!!!!!!!!!!!!!OK\n");
        result->tmp = 1;
    }


    return NH_ERR_NOERR;
}
