#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>


int mySystem(const char *cmd) {
    pid_t pid;
    if(cmd == NULL)    return 1;
    int status;
    if((pid = fork()) < 0)   status = -1;
    else if(0 == pid) {
        execl("/system/bin/sh","sh","-c",cmd,(char*)0);
        _exit(127);
    }
    else {
        while(waitpid(pid, &status, 0) < 0)
            if(errno != EINTR)   return -1;
    }
    return status;
}
