CC=gcc
INCLUDE=..\include
CFLAGS=--freestanding -I$(INCLUDE)
BIN_DIR=..\bin
NASM=..\bin\nasm.exe
LD=ld
PARTCOPY=..\bin\partcopy.exe
PCOPY = pcopy.exe
CPPFLAGS = -I$(INCLUDE)
LDFLAGSboot = -Ttext 0000 --oformat binary 
LDFLAGS = -Ttext 3E00 --oformat binary -e begtext --traditional-format
ASFLAGS = --traditional-format

objects = kernel.o


all :	kernel.bin mosboot.o
	$(LD) $(LDFLAGSboot) -o mosboot mosboot.o
	copy mosboot $(BIN_DIR)
	copy kernel.bin $(BIN_DIR)
	$(PARTCOPY) $(BIN_DIR)\mosboot 0 200 $(BIN_DIR)\mos.img 0
	$(PARTCOPY) $(BIN_DIR)\kernel.bin 0 600 $(BIN_DIR)\mos.img 200

.c.o:
	$(CC) -c $(CFLAGS) $<

.S.o:
	$(CC) -c $(CFLAGS) $<

kernel.bin:	$(objects)
	$(LD) $(LDFLAGS) -o kernel.bin $(objects)
clean:
	del mosboot
	del *.o
	del ..\bin\mosboot