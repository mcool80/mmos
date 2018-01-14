AS=as
CC=gcc
INCLUDE=..\include
CFLAGS=--freestanding -I$(INCLUDE)
KERNELOFFSET = 0x3000
BIN_DIR=..\bin
NASM=..\bin\nasm.exe
LD=ld
PARTCOPY=..\bin\partcopy.exe
BOCHS_DIR=D:\PROGRA~1\emu\Bochs-1.3\pagestest
PCOPY = pcopy.exe
CPPFLAGS = -I$(INCLUDE)

AFLAGS = -I$(INCLUDE) -D__ASSEMBLY__ -as -L 

objects= kernel.o keyboard.o io.o idt.o pic.o mem.o pages.o \
	string.o stdio.o kernelc.o


all :	kernel.bin mosboot.
	copy kernel.bin $(BIN_DIR)
	copy mosboot $(BIN_DIR)
	$(PARTCOPY) $(BIN_DIR)\mosboot 0 200 $(BIN_DIR)\mos.img 0
#	$(PARTCOPY) $(BIN_DIR)\kernel.bin 0 1200 $(BIN_DIR)\mos.img 200
	$(PCOPY) $(PARTCOPY) $(BIN_DIR)\kernel.bin $(BIN_DIR)\mos.img 200

.c.o:
	$(CC) -c $(CFLAGS) $<

.S.o:
	$(AS) -traditional -o $@ $< 

kernel.bin:	$(objects)
	$(LD) -Ttext $(KERNELOFFSET) --oformat binary -o kernel.bin kernel.o kernelc.o keyboard.o io.o idt.o pic.o mem.o pages.o stdio.o string.o

mosboot.:
	$(NASM) -o mosboot mosboot.asm
io.o:
	$(NASM) -f coff -o io.o io.asm	
mem.o:	
	$(NASM) -f coff -o mem.o mem.asm	
string.o:	
	$(NASM) -f coff -o string.o string.asm	
keyboard.o :
clean:
	del mosboot
	del *.o
	del *.bin
	del ..\bin\kernel.bin
	del ..\bin\mosboot