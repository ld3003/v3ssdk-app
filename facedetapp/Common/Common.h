#ifndef __Common_h__
#define __Common_h__

#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "Common_nh_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CAM_VIDEO_W         640
#define CAM_VIDEO_H         480

#define DEV_WINDOWS_W       272
#define DEV_WINDOWS_H       480
#ifdef arm
#define ETH_NAME            "eth0"
#endif
#ifdef x86_64
#define ETH_NAME            "enp0s3"
#endif


int get_mac(char * mac, int len_limit , long long *devid , char *ethname);
char *net_detect(char* net_name);
int get_local_ip(const char *eth_inf, char *ip);

inline void time_consuming_start(struct timeval *gTpstart , struct timeval *gTpend)
{
    memset(gTpstart,0,sizeof(struct timeval));
    memset(gTpend,0,sizeof(struct timeval));
    gettimeofday(gTpstart,NULL); // 开始时间
}

inline float time_consuming_print(char *strPuts , struct timeval *gTpstart , struct timeval *gTpend)
{
    float timeuse;
    gettimeofday(gTpend,NULL); // 结束时间
    timeuse=1000000*(gTpend->tv_sec-gTpstart->tv_sec)+gTpend->tv_usec-gTpstart->tv_usec;
    timeuse/=1000000;
    printf("@ %s -----> Used Time:%f  S\n",strPuts,timeuse);

    return timeuse;

}



#ifdef __cplusplus
}
#endif


#endif
