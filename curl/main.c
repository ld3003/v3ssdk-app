#if 0
#include <stdio.h>
#include <curl/curl.h>
//linker options: -lcurl -lcurldll

size_t write_function(void *buff, size_t size, size_t nmemb, FILE *fp){
    //回调函数，下载的数据通过这里写入本地文件
    fwrite(buff, size, nmemb, fp);
    return size*nmemb;
}

int main(int argc, char* argv[]){
    CURL *curl = NULL;
    CURLcode code = 0;
    char url[] = "http://www.lolhelper.cn/rank/rank.php";
    char formdata[] = "daqu=%E7%94%B5%E4%BF%A1%E4%B8%80&nickname=%E4%BC%A0%E5%A5%87%E8%8B%B1%E9%9B%84";
    char filename[] = "c:\\post.html";
    FILE *fp = fopen(filename, "w");


    curl = curl_easy_init();
    if(curl){
        //设置POST协议、URL和FORM_DATA
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, formdata);
        //设置数据回调
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_function);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        code = curl_easy_perform(curl);

        if(code == CURLE_OK){
            ;;
        }

        curl_easy_cleanup(curl);
    }

    fclose(fp);
    return 0;
}

#endif


#include <stdio.h>
#include <curl/curl.h>
//linker options: -lcurl -lcurldll
static unsigned char reqbuffer[512];


size_t write_data(void *buff, size_t size, size_t nmemb, FILE *fp){
    //回调函数，下载的数据通过这里写入本地文件
    fwrite(buff, size, nmemb, fp);
    return size*nmemb;
}


#define  REQURL "http://39.98.235.120:18080/rest/face/faceDevice/distinguish"

int main(int argc, char* argv[]){
	char *url= REQURL;
	CURL *pCurl = NULL;
	CURLcode res;

	struct curl_slist *headerlist = NULL;

	struct curl_httppost *post = NULL ;
	struct curl_httppost *last = NULL;

	curl_formadd(&post, &last, CURLFORM_COPYNAME, "busiDeviceId",
			CURLFORM_COPYCONTENTS, "280941447020552",
			CURLFORM_END);

	curl_formadd(&post, &last,CURLFORM_COPYNAME, "file", //此处表示要传的参数名
			CURLFORM_FILE, "1.jpg",                               //此处表示图片文件的路径
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

	return 0;



}

#if 0
CURL *hnd = curl_easy_init();

curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
curl_easy_setopt(hnd, CURLOPT_URL, "http://39.98.235.120:18080/rest/face/faceDevice/distinguish");

struct curl_slist *headers = NULL;
headers = curl_slist_append(headers, "cache-control: no-cache");
headers = curl_slist_append(headers, "Connection: keep-alive");
headers = curl_slist_append(headers, "Content-Length: 51344");
headers = curl_slist_append(headers, "Content-Type: multipart/form-data; boundary=--------------------------829742580188631806746898");
headers = curl_slist_append(headers, "Accept-Encoding: gzip, deflate");
headers = curl_slist_append(headers, "Cookie: com.jeespring.session.id=8ecf50f4bfdd4d1b85134d816bc1d28d");
headers = curl_slist_append(headers, "Host: 39.98.235.120:18080");
headers = curl_slist_append(headers, "Postman-Token: 6704d69d-d949-4ff0-9680-7a5d902bf2f6,b0276c41-f138-48d4-9216-2736df4d3043");
headers = curl_slist_append(headers, "Cache-Control: no-cache");
headers = curl_slist_append(headers, "Accept: */*");
headers = curl_slist_append(headers, "User-Agent: PostmanRuntime/7.15.2");
headers = curl_slist_append(headers, "content-type: multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW");
curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"busiDeviceId\"\r\n\r\n768755230312225\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"file\"; filename=\"66-3.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW--");

CURLcode ret = curl_easy_perform(hnd);
#endif
