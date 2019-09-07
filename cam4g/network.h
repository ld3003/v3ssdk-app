#ifndef __network_h__
#define __network_h__

int SetIfAddr(char *ifname, char *Ipaddr, char *mask,char *gateway);
int net_detect(char* net_name);
char *net_up(char* net_name);
int get_system_output(char *cmd, char *output, int size);

#endif


