# MOS - Markus Operating System version 1.0.0
# Makefile for kernel files

AS=as
CC=gcc
INCLUDE=..\include
CFLAGS=--freestanding -I$(INCLUDE) -Wall
BIN_DIR=..\bin
CPPFLAGS = -I$(INCLUDE)
ASFLAGS = --traditional-format --freestanding -I$(INCLUDE)
AFLAGS = -I$(INCLUDE) -D__ASSEMBLY__ -as -L 

objects = kernel.o idt.o pic.o

all :	kernel
	copy kernel.o kernel.obj
	del kernel.o

.c.o:
	$(CC) -c $(CFLAGS) $<

.S.o:
	$(CC) -c $(ASFLAGS) $<

kernel:	$(objects)
	
clean:
	del *.o


