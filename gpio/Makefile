TARGET        = myGPIO    #可执行文件名称

########################编译参数############################

CROSS_COMPILE = arm-buildroot-linux-uclibcgnueabihf-

CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AR = $(CROSS_COMPILE)ar

DEFINES       = 
CFLAGS        = -pipe -g -Wall -W -fPIE $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W -fPIE $(DEFINES)
INCPATH       = -I. 


########################编译文件############################
SOURCES       = ./main.c ./gpio.c
OBJECTS       = main.o gpio.o

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)
	main.o : main.c gpio.h
	$(CC) $(include) $(CFLAGS) -c main.c 
	gpio.o : gpio.c gpio.h 
	$(CC) $(include) $(CFLAGS) -c gpio.c 
clean :
	rm  $(OBJECTS) $(TARGET)
