# makefile test
# MOS - Markus Operating System version 1.0.0
# Makefile
#
AS=as
CC=gcc
INCLUDE=..\include
CFLAGS= -I$(INCLUDE) -DTEST
LD=ld
CPPFLAGS = -I$(INCLUDE)

OBJECTS = test.o ../pages/page.o ../process/process_memory.o ../pages/intel/intel_pages.o


all:	$(OBJECTS)
	gcc -o test.exe	$(OBJECTS)

clean:	
	del test.o
	del ../pages/page/page.o