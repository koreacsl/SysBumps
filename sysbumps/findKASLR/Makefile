CC = clang
TARGET = attacker

VPATH = $(CURDIR) $(COMDIR)

COMDIR = ../common

CFLAGS = -arch arm64 -w -I./ -I$(COMDIR)

#SRCS = $(foreach dir, ./ $(COMDIR), $(wildcard $(dir)/*.c))
#SRCS := $(notdir $(SRCS))
#OBJS = $(SRCS:.c=.o)
OBJS = main.o memory.o timing.o

all : $(TARGET)
	rm -f $(OBJS)
	
$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

.c.o :
	$(CC) $(CFLAGS) -c -o $@ $< 

clean : 
	rm -f $(OBJS) $(TARGET)


