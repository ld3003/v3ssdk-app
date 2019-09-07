#include "Communiction.h"
#include "curl/curl.h"
#include "Common.h"
#include "cJSON.h"

#include <stdio.h>
#include <curl/curl.h>

static unsigned char reqflag = 0;

#define  REQURL2 "http://39.98.235.120:18080/rest/face/faceDevice/distinguish"
#define REQURL "http://182.61.60.162:9080/rest/checkFace"
static unsigned char reqbuffer[512];

//7E010100003200000000000000000000000000000000000000000D


size_t  write_data(void *ptr, size_t size, size_t nmemb, void *stream) {

    printf("\n\n\nHTTP[%s]\n\n\n\n",ptr);
    snprintf(reqbuffer,sizeof(reqbuffer),"%s",ptr,nmemb);
    return nmemb;
}


NH_ERRCODE communiction_init(COMMUNICTION *comm)
{
    return NH_ERR_NOERR;
}


NH_ERRCODE communiction_pushpic(COMMUNICTION *comm, PICTURE *pic, COMMUNICTION_RESULT *result)
{

    char mac[32];
    long long devid;
    char *url= REQURL;
    CURL *pCurl = NULL;
    CURLcode res;

    if (reqflag == 1)
	    url = REQURL2;

    printf("REQURL : %s \n",url);

    struct curl_slist *headerlist = NULL;

    struct curl_httppost *post = NULL;
    struct curl_httppost *last = NULL;

    reqbuffer[0] = 0;
    result->resp[0] = 0;


    get_mac(mac,sizeof(mac),&devid,ETH_NAME);




    curl_formadd(&post, &last, CURLFORM_COPYNAME, "busiDeviceId",
                 CURLFORM_COPYCONTENTS, mac,
                 CURLFORM_END);


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

        curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, write_data);

        res = curl_easy_perform(pCurl);

        if (res != CURLE_OK)
        {
            printf("curl_easy_perform() failed，error code is:%s\n", curl_easy_strerror(res));
            snprintf(result->resp,512,"%s",curl_easy_strerror(res));

        }else {


            if (strstr(reqbuffer,"{\""))
            {

                char *in_json = strstr(reqbuffer,"{\"");
                cJSON * pSub;
                cJSON * pJson;

                if(NULL != in_json){

                    pJson = cJSON_Parse(in_json);
                    if(NULL != pJson){

                        pSub = cJSON_GetObjectItem(pJson, "message");
                        if(NULL != pSub)
                        {
                            printf("MESSAGE:%s\n", pSub->valuestring);
                            snprintf(result->resp,512,"%s",pSub->valuestring);

                        }



                        pSub = cJSON_GetObjectItem(pJson, "code");
                        if(NULL != pSub)
                        {
                            printf("CODE:%d\n", pSub->valueint);
                            result->code = pSub->valueint;
			    if (result->code == 200)
			    {
				    reqflag = 1;
			    }

                        }




                        cJSON_Delete(pJson);
                    }
                }


            }

        }


        curl_easy_cleanup(pCurl);


    }



    return NH_ERR_NOERR;
}
