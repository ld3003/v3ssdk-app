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
