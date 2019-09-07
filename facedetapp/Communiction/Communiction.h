#ifndef __communiction_h__
#define __communiction_h__

#include "Common.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _COMMUNICTION_RESULT 
{
    nh_u8 tmp;
    nh_s32 code;
    nh_s32 action;
    nh_s32 zhixinlv;
    nh_u8 resp[512];

}COMMUNICTION_RESULT;

typedef struct _COMMUNICTION
{
    nh_u8 tmp;



}COMMUNICTION;

NH_ERRCODE communiction_init(COMMUNICTION *comm);
NH_ERRCODE communiction_pushpic(COMMUNICTION *comm, PICTURE *pic, COMMUNICTION_RESULT *result);
NH_ERRCODE communiction_pushpic2(COMMUNICTION *comm, PICTURE *pic, COMMUNICTION_RESULT *result);

#ifdef __cplusplus
};
#endif

#endif
