#ifndef __Common_nh_type_h__
#define __Common_nh_type_h__


#ifdef __cplusplus
extern "C" {
#endif


typedef unsigned char nh_u8;
typedef char nh_s8;

typedef unsigned short nh_u16;
typedef short nh_s16;

typedef unsigned int nh_u32;
typedef int nh_s32;

typedef struct _NH_TIME
{

    nh_s32 second;
    nh_s32 usecond;

} NH_TIME;

typedef struct _NH_DATE
{
    nh_s32 year;
    nh_s32 mon;
    nh_s32 day;
    nh_s32 week;
} NH_DATE;

typedef struct _NH_DATETIME
{
    NH_DATE date;
    NH_TIME time;

} NH_DATETIME;

typedef enum
{
    NH_ERR_NOERR,   /*< 没有错误*/
    NH_ERR_MEM,     /*< 内存错误 */
    NH_ERR_TIMEOUT, /*< 超时错误*/
    NH_ERR_IO,      /*< IO错误*/

} NH_ERRCODE;

typedef enum
{
    PIC_JPEG,
    PIC_BMP,

} PICTYPE;

typedef enum
{
    PIC_QVGA,
    PIC_VGA,
    PIC_720P,
    PIC_1080P

} PICSIZE;

typedef struct _PICTURE
{
    nh_u8 *path;
    nh_u8 *imgdat;
    nh_s32 imgdatlen;
    PICTYPE imgtype;
} PICTURE;

typedef struct _RECT
{
    nh_u32 x;
    nh_u32 y;
    nh_u32 w;
    nh_u32 h;

} RECT;

#ifdef __cplusplus
}
#endif


#endif
