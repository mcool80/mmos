# MOS - Markus Operating System version 1.0.0
# Makefile
#
AS=as
CC=gcc
INCLUDE=..\include
CFLAGS=--freestanding -I$(INCLUDE)
BIN_DIR=..\bin
MAKE = make
LD=ld
PARTCOPY=..\bin\partcopy.exe
PCOPY = pcopy.exe
CPPFLAGS = -I$(INCLUDE)
LDFLAGSboot = -Ttext 1000 --oformat binary 
LDFLAGS = -Ttext 5000 --oformat binary -e kernel_startup --traditional-format
ASFLAGS = --traditional-format --freestanding -I$(INCLUDE)
AFLAGS = -I$(INCLUDE) -D__ASSEMBLY__ -as -L 

objects = kernel/kernel.obj kernel/*.o process/*.o io/*.o pages/*.o mm/*.o pages/intel/*.o process/intel/*.o
SUBDIRS = kernel mm boot io process pages pages\intel process\intel
SUBDIRSDEL = kernel\*.o mm\*.o boot\*.o io\*.o pages\*.o process\*.o pages\intel\*.o process\intel\*.o

all:	subdirs kernel.bin
	$(LD) $(LDFLAGS) -o kernel.bin $(objects)
	@echo Copy the MBR to Binary directory
	@copy boot\mosboot $(BIN_DIR)
	@echo Copy the kernel to Binary directory
	@copy kernel.bin $(BIN_DIR)
	@echo Copy the MBR and kernel to the virtual hard drive
	@$(PARTCOPY) $(BIN_DIR)\mosboot 0 800 $(BIN_DIR)\mos.img 0
	@$(PARTCOPY) $(BIN_DIR)\kernel.bin 0 1E00 $(BIN_DIR)\mos.img 800

.c.o:
	$(CC) -c $(CFLAGS) $<

.S.o:
	$(CC) -c $(ASFLAGS) $<

.PHONY:	subdirs $(SUBDIRS)
	
subdirs:	$(SUBDIRS)
	
$(SUBDIRS):	
	@echo Enter directory $@
	@$(MAKE) -C $@

.PHONYclean:	subdirsclean $(SUBDIRSDEL)
	
subdirsclean:	$(SUBDIRSDEL)

$(SUBDIRSDEL):	
	@del $@

kernel.bin:
	@echo Making kernel.bin
#	$(LD) $(LDFLAGS) -o kernel.bin $(objects)

clean:	subdirsclean
	@echo Removed all object files
	@echo Remove the compiled kernel
	@del kernel.bin
	@echo Remove the compiled MBR
	@del boot\mosboot
