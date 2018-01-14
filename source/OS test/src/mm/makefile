# MOS - Markus Operating System version 1.0.0
# Makefile
AS=as
CC=gcc
INCLUDE=..\include
CFLAGS=--freestanding -I$(INCLUDE)
CPPFLAGS = -I$(INCLUDE)
LDFLAGSboot = -Ttext 0000 --oformat binary 
LDFLAGS = -Ttext AE00 --oformat binary -e begtext --traditional-format
ASFLAGS = --traditional-format --freestanding -I$(INCLUDE)
AFLAGS = -I$(INCLUDE) -D__ASSEMBLY__ -as -L 

objects = mm.o

all :	$(objects)

.c.o:
	$(CC) -c $(CFLAGS) $<

.S.o:
	$(CC) -c $(ASFLAGS) $<

clean:
	del *.o
