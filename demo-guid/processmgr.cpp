#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "processmgr.h"

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>

static pthread_t processmgrthread;

#define BUF_SIZE 1024

struct pidlist
{
    int pid[32];
    int cnt;
};

void getPidByName(char *task_name, struct pidlist *plist)
{
    DIR *dir;
    struct dirent *ptr;
    FILE *fp;
    char filepath[50];      //大小随意，能装下cmdline文件的路径即可
    char cur_task_name[50]; //大小随意，能装下要识别的命令行文本即可
    char buf[BUF_SIZE];
    dir = opendir("/proc"); //打开路径
    plist->cnt = 0;
    if (NULL != dir)
    {
        while ((ptr = readdir(dir)) != NULL) //循环读取路径下的每一个文件/文件夹
        {
            //如果读取到的是"."或者".."则跳过，读取到的不是文件夹名字也跳过
            if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
                continue;
            if (DT_DIR != ptr->d_type)
                continue;

            sprintf(filepath, "/proc/%s/status", ptr->d_name); //生成要读取的文件的路径
            fp = fopen(filepath, "r");                         //打开文件
            if (NULL != fp)
            {
                if (fgets(buf, BUF_SIZE - 1, fp) == NULL)
                {
                    fclose(fp);
                    continue;
                }
                sscanf(buf, "%*s %s", cur_task_name);

                //如果文件内容满足要求则打印路径的名字（即进程的PID）
                if (!strcmp(task_name, cur_task_name))
                {
                    sscanf(ptr->d_name, "%d", &plist->pid[plist->cnt]);
                    plist->cnt++;
                    printf("PID:  %s %d %d\n", ptr->d_name, plist->pid[plist->cnt - 1], plist->cnt);
                }

                fclose(fp);
            }
        }
        closedir(dir); //关闭路径
    }
}

void getNameByPid(pid_t pid, char *task_name)
{
    char proc_pid_path[BUF_SIZE];
    char buf[BUF_SIZE];

    sprintf(proc_pid_path, "/proc/%d/status", pid);
    FILE *fp = fopen(proc_pid_path, "r");
    if (NULL != fp)
    {
        if (fgets(buf, BUF_SIZE - 1, fp) == NULL)
        {
            fclose(fp);
        }
        fclose(fp);
        sscanf(buf, "%*s %s", task_name);
    }
}


void killprocess(char *processname)
{
    int i = 0;

    char task_name[50];
    struct pidlist dhcp_server_plist;

    getPidByName(processname, &dhcp_server_plist);
 
    for (i = 0; i < dhcp_server_plist.cnt; i++)
    {
        int retval = kill(dhcp_server_plist.pid[i], SIGKILL);

        if (retval)
        {
            puts("kill media_server_plist failed.");
            perror("kill");
        }
    }

}

void killmediaprocess()
{
    int i = 0;

    char task_name[50];
    struct pidlist media_server_plist;
    struct pidlist ccdr_plist;

    getPidByName("mediaserver", &media_server_plist);
    getPidByName("ccdr", &ccdr_plist);

    for (i = 0; i < media_server_plist.cnt; i++)
    {
        int retval = kill(media_server_plist.pid[i], SIGKILL);

        if (retval)
        {
            puts("kill media_server_plist failed.");
            perror("kill");
        }
    }

    for (i = 0; i < ccdr_plist.cnt; i++)
    {
        int retval = kill(ccdr_plist.pid[i], SIGKILL);

        if (retval)
        {
            puts("kill ccdr_plist failed.");
            perror("kill");
        }
    }

    printf("start ccdr\n");
}

void *processmgr(void *arg)
{
    for (;;)
    {
        sleep(60 * 30);
        killmediaprocess();
    }
}

int startprocessmgr()
{
    int ret = pthread_create(&processmgrthread, NULL, processmgr, (void *)0);
    if (ret < 0)
        printf("Create tcprecv thread error.");
}
