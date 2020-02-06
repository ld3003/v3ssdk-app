#include "widget.h"
#include <QApplication>
#include "gpio.h"
int main(int argc, char *argv[])
{

	GPIO_Init();
	GPIO_ConfigPin(PB, 2, OUT);
	GPIO_SetPin(PB,2,0);

    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
