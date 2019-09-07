/**
 *                      Copyleft (C) 2010  Late Lee
 *        This program is tested on LINUX PLATFORM, WITH GCC 4.x.
 *        The program is distributed in the hope that it will be
 *        useful, but WITHOUT ANY WARRANTY. Please feel free to 
 *        use the program, and I feel free to ignore the related
 *        issues. Any questions or suggestions, or bugs, please 
 *        contact me at
 *        <$ echo -n "aHR0cDovL3d3dy5sYXRlbGVlLm9yZwo=" | base64 -d>
 *        or e-mail to 
 *        <$ echo -n "bGF0ZWxlZUAxNjMuY29tCg==" | base64 -d>
 *        if you want to do this.
 *
 * @file   main.c
 * @author Late Lee
 * @date   Mon Jan 10 2011
 * 
 * @brief  A simple test of serial port writing data to the port.
 * @test   To compile the program, type <tt> make </tt>,
 *         then it will generate the executable binary @p a.out.
 *         To run it, type <tt> ./a.out</tt>.Make sure you have the permission
 *         to open the serial port.@n
 *         You can start a ternimal and type @p minicom(also make sure you can
 *         open the device). When everything is OK, you can see the data in 
 *         minicom using the same port if you connect pin 2 & pin 3 of the port
 *         (the male connector, see the picture below)(but I don't know the
 *         reason, isn't it blocked?).@n
 *         If you want to stop the program, just use 'Ctrl+c' or 'q'. @n
 *         In default, the program open the device '/dev/ttyUSB0'.@n
 *         Here are snapshots of the program:
 *         @image html serial-write.jpg "Writing to the port..."
 *         @image html serial-read-minicom.jpg "Reading the data in minicom..."
 *         @image html send_revc_self.jpg "One thread send, one thread read."
 *         @n And here comes the serial port connector:
 *         @image html com-male.jpg "A 9-pin male D-Sub connector(*)"
 *         @image html com-female.jpg "A 9-pin female D-Sub connector"
 *         (*)We use this connector in the test.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
//#include "debug-msg.h"
#include "serialport.h"
#include "error-log.h"
#include "saveppic.h"
#include "pushpic.h"
#include "network.h"
#include "common.h"
#include <curl/curl.h>

pthread_t write_tid; /**< write thread */
pthread_t read_tid;  /**< read thread */
pthread_t exit_tid;  /**< exit thread */
pthread_t netcheck_tid;
pthread_t getcamera_tid;

int vol = 0;
int csq = 0;
float jd = 0;
float wd = 0;


/** The data we write to the port. */
//char *buf = "Are you going to die?\r\n";
char *buf = "Are you going to die? To be or not to be, that is the question.\r\n";
/** data we receive */
char tmp[512];

static void split(char *src, const char *separator, char **dest, int *num)
{
    char *pNext;
    int count = 0;
    if (src == NULL || strlen(src) == 0)
        return;
    if (separator == NULL || strlen(separator) == 0)
        return;
    pNext = strtok(src, separator);
    while (pNext != NULL)
    {
        *dest++ = pNext;
        ++count;
        pNext = strtok(NULL, separator);
    }
    *num = count;
}

static void process_recv_serialport(int fd)
{
    if (strstr(tmp, "AT+GETPIC"))
    {
        if (getpic())
        {
            snprintf(tmp, sizeof(tmp), "OK\r\n");
            write(fd, tmp, strlen(tmp));
        }
        else
        {

            snprintf(tmp, sizeof(tmp), "ERROR NOPIC\r\n");
            write(fd, tmp, strlen(tmp));
        }
    }else
    if (strstr(tmp, "AT+GETURL"))
    {
	    extern char geturl[];
            extern char reqbuffer[512];

	    snprintf(tmp, sizeof(tmp), "OK=%s\r\n",reqbuffer);
	    write(fd, tmp, strlen(tmp));
	    printf("GETURL ############################ %s \n",tmp);
    }
    else if (strstr(tmp, "AT+PUSHPIC"))
    {
        char *buf = tmp;

        //用来接收返回数据的数组。这里的数组元素只要设置的比分割后的子字符串个数大就好了。
        char *revbuf[128] = {0};
        char *p;

        for(int i=0;i<strlen(buf);i++)
        {
                if (buf[i] == '=')
                        buf[i] = ',';
                if (buf[i] == '\r')
                        buf[i] = ',';
        }

        //分割后子字符串的个数
        int num = 0;

        split(buf,",",revbuf,&num);
        for (int i = 0; i < num; i++)
        {
                printf("[%s]\n", revbuf[i]);
        }

	if (num >= 4)
	{
		sscanf(revbuf[1],"%d",&vol);
		sscanf(revbuf[2],"%d",&csq);
		sscanf(revbuf[3],"%f",&jd);
		sscanf(revbuf[4],"%f",&wd);


		printf("!!!!!!!! %d %d %f %f \n",vol,csq,jd,wd);

	}


	int ret = pushpic();
	if (ret == 0)
	{
		if (next_request_time < 30758400)
                snprintf(tmp, sizeof(tmp), "OK=%d\r\n", next_request_time);
            else

                snprintf(tmp, sizeof(tmp), "ERROR TIMEERR%d\r\n", next_request_time);
        }
        else
        {
            snprintf(tmp, sizeof(tmp), "ERROR %s\r\n", curl_easy_strerror((CURLcode)ret));
        }

        printf("ATECHO @ %s \n", tmp);

        write(fd, tmp, strlen(tmp));
    }
    else if (strstr(tmp, "AT\r\n"))
    {
        snprintf(tmp, sizeof(tmp), "OK\r\n");
        write(fd, tmp, strlen(tmp));
    }
}

/** 
 * write_port_thread - A thread that writes data to the port
 * 
 * @param argc : Here means the port(specified by the fd).
 * 
 * @note
 * This is only a test, not the @e real one.
 */
void *write_port_thread(void *argc)
{
    int ret;
    int fd;
    static int cnt = 1;
    char send_buf[512] = {0};

    fd = (int)argc;

    while (1)
    {
        sprintf(send_buf, "%d %s", cnt, buf);
        debug_msg("writing time %d... ", cnt++);

        ret = write(fd, send_buf, strlen(send_buf));

        if (ret < 0)
            pthread_exit(NULL);
        debug_msg("write num---: %d\n", ret);
        sleep(2);
    }
    pthread_exit(NULL);
}

void *getpic_thread(void *argc)
{
    for (;;)
    {
        getpic2();
    }
}

/** 
 * read_port_thread - Thread that reads data from the port
 * 
 * @param argc : Here means the port(specified by the fd).
 * 
 */
void *read_port_thread(void *argc)
{
    int num;
    int fd;

    fd = (int)argc;
    while (1)
    {
        while ((num = read(fd, tmp, 512)) > 0)
        {
            debug_msg("read num: %d\n", num);
            tmp[num + 1] = '\0';
            printf("[%s]\n", tmp);
            process_recv_serialport(fd);
        }

        if (num < 0)
            pthread_exit(NULL);
    }
    pthread_exit(NULL);
}

void *netcheck_thread(void *argc)
{
    int num;
    int fd;
    for (;;)
    {
#if 1
        if (net_detect("eth1") >= 0)
        {
            //如果网卡未激活，则激活并配置网卡
            net_up("eth1");
            SetIfAddr("eth1", "192.168.0.100", "255.255.255.0", "192.168.0.1");
            //mySystem("setprop net.dns1 114.114.114.114");
        }
#endif

        sleep(1);
    }
}

/** 
 * sig_handle - Handle the INT signal.
 * 
 * @param sig_num : The signal.
 * @note
 * This function is not used.
 */
void sig_handle(int sig_num)
{
    debug_msg("catch signal %d\n", sig_num);
    exit(0);
}

/** 
 * exit_thread - Thread that exit the program when 'q' pressed
 * 
 * @param argc : Here means the port(specified by the fd).
 * 
 */
void *exit_thread(void *argc)
{
    while (1)
    {
        int c = getchar();
        if ('q' == c)
        {
            printf("You exit, not myself.\n");
            exit(0);
        }
        sleep(1);
    }
    pthread_exit(NULL);
}

/** 
 * main - main function
 * 
 */

#define RUN_TEST printf("RUN_TEST #################33 %s : %d \n",__FILE__,__LINE__);

int main(int argc, char *argv[])
{
    int fd;
    int ret;
    char dev_name[32] = {0};

    char tb[128];

    RUN_TEST;

    opencam();

    RUN_TEST;
    strcpy(dev_name, "/dev/ttyS2");
    if (argc == 2)
    {
        sprintf(dev_name, "%s", argv[1]);
    }

    RUN_TEST;

    //signal(SIGINT, sig_handle);
    fd = open_port(dev_name); /* open the port */
    if (fd < 0)
    {
        printf("open %s err\n", dev_name);
        exit(0);
    }
    RUN_TEST;
    ret = setup_port(fd, 115200, 8, 'N', 1); /* setup the port */
    if (ret < 0)
        exit(0);

#if 0
    ret = pthread_create(&write_tid, NULL, write_port_thread, (void*)fd);
    if (ret < 0)
        unix_error_exit("Create write thread error.");
#endif

    ret = pthread_create(&read_tid, NULL, read_port_thread, (void *)fd);
    if (ret < 0)
        unix_error_exit("Create read thread error.");
    RUN_TEST;
    ret = pthread_create(&netcheck_tid, NULL, netcheck_thread, (void *)fd);
    if (ret < 0)
        unix_error_exit("Create netcheck thread error.");
    RUN_TEST;
    ret = pthread_create(&getcamera_tid, NULL, getpic_thread, (void *)fd);
    if (ret < 0)
        unix_error_exit("Create netcheck thread error.");

#if 0
    ret = pthread_create(&exit_tid, NULL, exit_thread, NULL);
    if (ret < 0)
        unix_error_exit("Create exit thread error.");
#endif

    //pthread_join(write_tid, NULL);
    //pthread_join(read_tid, NULL);
    //pthread_join(exit_tid, NULL);
    //
    for(;;){
	    sleep(1);
    }


    return 0;
}
