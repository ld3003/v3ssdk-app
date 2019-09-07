

#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <error.h>
#include <net/route.h>
 
int SetIfAddr(char *ifname, char *Ipaddr, char *mask,char *gateway)
{
    int fd;
    int rc;
    struct ifreq ifr; 
    struct sockaddr_in *sin;
    struct rtentry  rt;
    
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0)
    {
            perror("socket   error");     
            return -1;     
    }
    memset(&ifr,0,sizeof(ifr)); 
    strcpy(ifr.ifr_name,ifname); 
    sin = (struct sockaddr_in*)&ifr.ifr_addr;     
    sin->sin_family = AF_INET;  
   
    //ipaddr
    if(inet_aton(Ipaddr,&(sin->sin_addr)) < 0)   
    {     
        perror("inet_aton   error");     
        return -2;     
    }    
    
    if(ioctl(fd,SIOCSIFADDR,&ifr) < 0)   
    {     
        perror("ioctl   SIOCSIFADDR   error");     
        return -3;     
    }

    //netmask
    if(inet_aton(mask,&(sin->sin_addr)) < 0)   
    {     
        perror("inet_pton   error");     
        return -4;     
    }    
    if(ioctl(fd, SIOCSIFNETMASK, &ifr) < 0)
    {
        perror("ioctl");
        return -5;
    }

    //gateway
    memset(&rt, 0, sizeof(struct rtentry));
    memset(sin, 0, sizeof(struct sockaddr_in));
    sin->sin_family = AF_INET;
    sin->sin_port = 0;
    if(inet_aton(gateway, &sin->sin_addr)<0)
    {
       printf ( "inet_aton error\n" );
    }
    memcpy ( &rt.rt_gateway, sin, sizeof(struct sockaddr_in));
    ((struct sockaddr_in *)&rt.rt_dst)->sin_family=AF_INET;
    ((struct sockaddr_in *)&rt.rt_genmask)->sin_family=AF_INET;
    rt.rt_flags = RTF_GATEWAY;
    if (ioctl(fd, SIOCADDRT, &rt)<0)
    {
        printf( "ioctl(SIOCADDRT) error in set_default_route\n");
        close(fd);
        return -1;
    }
    close(fd);
    return rc;
}


int net_detect(char* net_name)
{
        int skfd = 0;
        struct ifreq ifr;

        skfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(skfd < 0) {
                printf("%s:%d Open socket error!\n", __FILE__, __LINE__);
                return -1;
        }

        strcpy(ifr.ifr_name, net_name);


        if(ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0 ) {
                printf("%s:%d IOCTL error!\n", __FILE__, __LINE__);
                printf("Maybe ethernet inferface %s is not valid!", ifr.ifr_name);
                close(skfd);
                return -2;
        }

        if(ifr.ifr_flags & IFF_RUNNING) {
                return 1;
        } else {
                return 0;
        }

		return 0;


			

}


char *net_up(char* net_name)
{
	int skfd = 0;
	struct ifreq ifr;

	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(skfd < 0) {
		printf("%s:%d Open socket error!\n", __FILE__, __LINE__);
		return NULL;
	}

	strcpy(ifr.ifr_name, net_name);

	//ifr.ifr_flags &= ~IFF_UP;  // ifconfig   donw 
	ifr.ifr_flags |= IFF_UP;
	ioctl(skfd, SIOCSIFFLAGS, &ifr); 


	if(ioctl(skfd, SIOCGIFFLAGS, &ifr) <0 ) {
		printf("%s:%d IOCTL error!\n", __FILE__, __LINE__);
		printf("Maybe ethernet inferface %s is not valid!", ifr.ifr_name);
		close(skfd);
		return NULL;
	}

	if(ifr.ifr_flags & IFF_RUNNING) {
		printf("UP");
	} else {
		printf("DOWN");
	}

	close(skfd);
	return 0;



}
