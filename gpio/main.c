#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "gpio.h"
#include <stdbool.h>
int main()
{
    int a=0,b=-1,i;
    GPIO_Init();
    GPIO_ConfigPin(PB,4,OUT);
    GPIO_ConfigPin(PB,5,OUT);
    for(i=0;i<20;i++)
    {
        GPIO_SetPin(PB,4,a=~a);//！！-1的取反是0 1的取反是-2....
        GPIO_SetPin(PB,5,b=~b);
        //usleep(100000);
        sleep(1);
    }
    GPIO_Free();
}
