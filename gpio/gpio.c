#include "gpio.h"

unsigned int fd;
PIO_Map *PIO = NULL;
unsigned int *gpio_map;
unsigned int addr_start, addr_offset;
unsigned int PageSize, PageMask;
void GPIO_Init(void)
{

    if ((fd = open("/dev/mem", O_RDWR)) == -1)
    {
        printf("open error\r\n");
        return;
    }

    PageSize = sysconf(_SC_PAGESIZE); //使用sysconf查询系统页面大小
    PageMask = (~(PageSize - 1));     //页掩码
    printf("PageSize:%d,PageMask:0x%.8X\r\n", PageSize, PageMask);

    addr_start = PIO_BASE_ADDRESS & PageMask;   //0x01C20800 & 0xfffff000 =  0x1C20000
    addr_offset = PIO_BASE_ADDRESS & ~PageMask; //0x01C20800 & 0x00000100 = 0x800
    printf("addr_start:%.8X,addr_offset:0x%.8X\r\n", addr_start, addr_offset);
    //mmap(系统自动分配内存地址，映射区长度“内存页的整数倍”，选择可读可写，MAP_SHARED=与其他所有映射到这个对象的进程共享空间，文件句柄，被映射内容的起点)
    //offest 映射物理内存的话，必须页对其!!!   所以这个起始地址应该是0x1000的整数倍，那么明显0x01C20800需要减去0x800才是整数倍！
    if ((gpio_map = mmap(NULL, PageSize * 2, PROT_READ | PROT_WRITE, MAP_SHARED, fd, addr_start)) == NULL)
    {
        printf("mmap error\r\n");
        close(fd);
        return;
    }
    printf("gpio_map:%.8X\r\n", gpio_map);
    //这里已经将0x1c20000的地址映射到了内存中，但是我们需要的地址是0x01C20800，所以要再加上地址偏移量～
    PIO = (PIO_Map *)((unsigned int)gpio_map + addr_offset);
    printf("PIO:0x%.8X\r\n", PIO);

    close(fd); //映射好之后就可以关闭文件？
}

void GPIO_ConfigPin(PORT port, unsigned int pin, PIN_MODE mode)
{
    if (gpio_map == NULL)
        return;
    PIO->Pn[port].CFG[pin / 8] &= ~((unsigned int)0x07 << pin % 8 * 4);
    PIO->Pn[port].CFG[pin / 8] |= ((unsigned int)mode << pin % 8 * 4);
    printf("struct PIO_Struct size : %d",sizeof(PIO->Pn[port]));
}

void GPIO_SetPin(PORT port, unsigned int pin, unsigned int level)
{
    if (gpio_map == NULL)
        return;
    if (level)
        PIO->Pn[port].DAT |= (1 << pin);
    else
        PIO->Pn[port].DAT &= ~(1 << pin);
}

int GPIO_Free(void)
{
    if ((munmap(gpio_map, PageSize * 2)) == 0)//取消映射
    {
        printf("unmap success!\r\n");
    }
    else
    {
        printf("unmap failed!\r\n");
    }
    return 0;
}
