#include "connthread.h"
#include "widget.h"
#include <QHostAddress>
connthread::connthread(QObject *obj)
{
}



void connthread::run()
{
	for(;;)
	{

		system("sh /root/bin/usb-set-hostmode.sh");

		sleep(2);

		system("udhcpc -i eth1 &");
		sleep(5);


		printf("conn thread \n");
		sleep(1);
	}
}


