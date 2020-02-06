#ifndef __GPIO_H__
#define __GPIO_H__
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PIO_BASE_ADDRESS 0x01C20800


//unsigned int 4字节 一个PIO_Struct占36字节,对应十六进制就是0x24，正好是一个offset值。
typedef struct
{
    unsigned int CFG[4];
    unsigned int DAT;
    unsigned int DRV0;
    unsigned int DRV1;
    unsigned int PUL0;
    unsigned int PUL1;
} PIO_Struct;

typedef struct
{
    PIO_Struct Pn[7];
} PIO_Map;

typedef enum {
    PA = 0,
    PB = 1,
    PC = 2,
    PD = 3,
    PE = 4,
    PF = 5,
    PG = 6,
} PORT;

typedef enum {
    IN = 0x00,
    OUT = 0x01,
    AUX = 0x02,
    INT = 0x06,
    DISABLE = 0x07,
} PIN_MODE;

extern PIO_Map *PIO;

void GPIO_Init(void);
void GPIO_ConfigPin(PORT port, unsigned int pin, PIN_MODE mode);
void GPIO_SetPin(PORT port, unsigned int pin, unsigned int level);
unsigned int GPIO_GetPin(PORT port, unsigned int pin);
int GPIO_Free(void);
#endif
