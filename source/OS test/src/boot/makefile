# MOS - Markus Operating System version 1.0.0
# Makefile
AS=as
CC=gcc
INCLUDE=..\include
CFLAGS=--freestanding -I$(INCLUDE) -Wall
CPPFLAGS = -I$(INCLUDE)
LDFLAGSboot = -Ttext 0000 --oformat binary
ASFLAGS = --Wall --traditional-format --freestanding -I$(INCLUDE)

all :	mosboot.o kernelasm.o
	$(LD) $(LDFLAGSboot) -o mosboot mosboot.o kernelasm.o
.c.o:
	$(CC) -c $(CFLAGS) $<

.S.o:
	$(CC) -c $(ASFLAGS) $<

clean:
	del mosboot
	del *.o
