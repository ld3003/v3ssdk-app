#
# (C) Copyleft 2011
# Late Lee from http://www.latelee.org
# 
# A simple Makefile for *ONE* project(c or/and cpp file) in *ONE*  directory
#
# note: 
# you can put head file(s) in 'include' directory, so it looks 
# a little neat.
#
# usage: $ make
#        $ make debug=y
###############################################################################

# cross compile...
CROSS_COMPILE = arm-buildroot-linux-uclibcgnueabihf-

CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AR = $(CROSS_COMPILE)ar

ARFLAGS = cr
RM = -rm -rf
MAKE = make

CFLAGS = -Wall
#debug = y

ifeq ($(debug), y)
CFLAGS += -g
else
CFLAGS += -O2 -s
endif

DEFS = 
LIBS = -L../media-codec/lib/

CFLAGS += $(DEFS)

LDFLAGS = $(LIBS)

INCDIRS = ./ -I../media-codec/inc/

CFLAGS += -I$(INCDIRS)

LDFLAGS += -lpthread -ljpeg -lyuv -lzbar -lopencv_core -lopencv_videoio -lopencv_imgcodecs -lopencv_objdetect -lopencv_imgproc -lopencv_imgcodecs -lcurl -lm 
LDFLAGS += -lcedar_base    -lcedar_omxadapter      -lcedar_plugin_vd_mjpeg  -lcedar_plugin_vd_mpeg4  -lcedar_vencoder -lcedar_common  -lcedar_plugin_vd_h264  -lcedar_plugin_vd_mpeg2  -lcedar_vdecoder
# source file(s), including c file(s) cpp file(s)
# you can also use $(wildcard *.c), etc.
SRC_C   := $(wildcard *.c)
SRC_CPP := $(wildcard *.cpp)

# object file(s)
OBJ_C   := $(patsubst %.c,%.o,$(SRC_C))
OBJ_CPP := $(patsubst %.cpp,%.o,$(SRC_CPP))


# executable file
target = demo-codec

###############################################################################

all: $(target)

$(target): $(OBJ_C) $(OBJ_CPP)
	@echo "Generating executable file..." $(notdir $(target))
	@$(CXX) $(CFLAGS) $^ -o $(target) $(LDFLAGS)

# make all .c or .cpp
%.o: %.c
	@echo "Compling: " $(addsuffix .c, $(basename $(notdir $@)))
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	@echo "Compling: " $(addsuffix .cpp, $(basename $(notdir $@)))
	@$(CXX) $(CFLAGS) -c $< -o $@

clean:
	@echo "cleaning..."
	@$(RM) $(target)
	@$(RM) *.o *.back *~

.PHONY: all clean
