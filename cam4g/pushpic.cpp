#include <stdio.h>
#include <curl/curl.h>

char reqbuffer[512];

#include <stdlib.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>
#include <cJSON.h>

int next_request_time = 30758400;
char geturl[512];

int get_mac(char *mac, int len_limit, long long *devid, char *ethname)
{
	char *tmp;
	struct ifreq ifreq;
	int sock;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return -1;
	}
	strcpy(ifreq.ifr_name, ethname); //Currently, only get eth0

	if (ioctl(sock, SIOCGIFHWADDR, &ifreq) < 0)
	{
		perror("ioctl");
		return -1;
	}
	*devid = 0;
	tmp = (char *)devid;
	tmp[0] = ifreq.ifr_hwaddr.sa_data[5];
	tmp[1] = ifreq.ifr_hwaddr.sa_data[4];
	tmp[2] = ifreq.ifr_hwaddr.sa_data[3];
	tmp[3] = ifreq.ifr_hwaddr.sa_data[2];
	tmp[4] = ifreq.ifr_hwaddr.sa_data[1];
	tmp[5] = ifreq.ifr_hwaddr.sa_data[0];
	tmp[6] = 0;
	tmp[7] = 0;

	printf("mac addr id [%15lld] \n", *devid);

	printf("%X:%X:%X:%X:%X:%X", (unsigned char)ifreq.ifr_hwaddr.sa_data[0], (unsigned char)ifreq.ifr_hwaddr.sa_data[1], (unsigned char)ifreq.ifr_hwaddr.sa_data[2], (unsigned char)ifreq.ifr_hwaddr.sa_data[3], (unsigned char)ifreq.ifr_hwaddr.sa_data[4], (unsigned char)ifreq.ifr_hwaddr.sa_data[5]);

	return snprintf(mac, len_limit, "%15lld", *devid);

	//return snprintf (mac, len_limit, "%X:%X:%X:%X:%X:%X", (unsigned char) ifreq.ifr_hwaddr.sa_data[0], (unsigned char) ifreq.ifr_hwaddr.sa_data[1], (unsigned char) ifreq.ifr_hwaddr.sa_data[2], (unsigned char) ifreq.ifr_hwaddr.sa_data[3], (unsigned char) ifreq.ifr_hwaddr.sa_data[4], (unsigned char) ifreq.ifr_hwaddr.sa_data[5]);
}

size_t write_data(void *buff, size_t size, size_t nmemb, FILE *fp)
{
	//回调函数，下载的数据通过这里写入本地文件
	//fwrite(buff, size, nmemb, fp);
	snprintf(reqbuffer,sizeof(reqbuffer),"%s",buff,nmemb);
	return size * nmemb;
}

#define REQURL "http://demo.zxhtong.com/camera_upload.html"


extern int vol;
extern int csq;
extern float jd;
extern float wd;




int pushpic()
{
	int run_count = 0;
	char mac[32];
	long long devid;
	char *url = REQURL;
	CURL *pCurl = NULL;
	CURLcode res;
	char tmpstr[32];
	char tmpstr2[32];



	next_request_time = 30758400;

	struct curl_slist *headerlist = NULL;

	struct curl_httppost *post = NULL;
	struct curl_httppost *last = NULL;

	get_mac(mac, sizeof(mac), &devid, "eth0");

	snprintf(tmpstr,sizeof(tmpstr),"%d",vol);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "BATVOL",
				 CURLFORM_COPYCONTENTS, tmpstr,
				 CURLFORM_END);

	snprintf(tmpstr2,sizeof(tmpstr2),"%d",csq);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "SIG",
				 CURLFORM_COPYCONTENTS, tmpstr2,
				 CURLFORM_END);

	curl_formadd(&post, &last, CURLFORM_COPYNAME, "DEVID",
				 CURLFORM_COPYCONTENTS, mac,
				 CURLFORM_END);

	curl_formadd(&post, &last, CURLFORM_COPYNAME, "file", //此处表示要传的参数名
				 CURLFORM_FILE, "/tmp/tmp.jpg",			  //此处表示图片文件的路径
				 CURLFORM_CONTENTTYPE, "image/jpeg",
				 CURLFORM_END);

	pCurl = curl_easy_init();

	if (NULL != pCurl)
	{
		curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 10);
		curl_easy_setopt(pCurl, CURLOPT_URL, url);
		curl_easy_setopt(pCurl, CURLOPT_HTTPPOST, post);
		curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, write_data);

		for (int i = 0; i < 3; i++)
		{
			res = curl_easy_perform(pCurl);

			if (res != CURLE_OK)
			{
				printf("curl_easy_perform() failed，error code is:%s\n", curl_easy_strerror(res));
			}
			else
			{
				if (strstr(reqbuffer, "{\""))
				{

					char *in_json = strstr(reqbuffer, "{\"");
					cJSON *pSub;
					cJSON *pJson;

					if (NULL != in_json)
					{

						pJson = cJSON_Parse(in_json);
						if (NULL != pJson)
						{

							pSub = cJSON_GetObjectItem(pJson, "message");
							if (NULL != pSub)
							{
								printf("MESSAGE:%s\n", pSub->valuestring);
							}

							pSub = cJSON_GetObjectItem(pJson, "next_request_time");
							if (NULL != pSub)
							{
								next_request_time = pSub->valueint;
								printf("CODE:%d\n", pSub->valueint);
							}


							//geturl

							cJSON_Delete(pJson);
						}
					}
				}
			}
			printf("\n");

			curl_easy_cleanup(pCurl);

			printf("%s\n", reqbuffer);

			break;
		}
	}

	return res;
}
