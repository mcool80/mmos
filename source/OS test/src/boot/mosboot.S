//* File: mosboot.asm
//* This file contains the boot up sequence
//* it reads the kernel from the disk
//* jumps to kernel
//* This BOOT is written with tributes from PolyOS BOOT
//* Created: 2003-04-06 Markus Svensson
//* Modified:
//------------------------------------------------------------------------
//MOS boot
//------------------------------------------------------------------------
.extern _initkernel

IDTOFFSET    = 0x2000
KERNELSEG    = 0x4e0
STARTUP      = 0x4 // nr of starup block including this

// MUST BE CHANGED
KERNELSIZE   = 0x12
INITSEG      = 0x7c00         // Initial segment that the mbr is read into in memory
MOSBOOTSEG   = 0x1000         // Segment the mos-boot is moved to
BOOTSIZE     = 0x200          // Size of the boot
.code16                       //the bios starts out in 16-bit real mode
.text
.org 0

//------------------------------------------------------------------------
//SECTOR ONE: THE BOOT LOADER
//------------------------------------------------------------------------

jmp _start       	//skip over our data and functions

//------------------------------------------------------------------------
//Data used in the boot-loading process
//------------------------------------------------------------------------
        //User messages
_bootdrv:
        .byte 0
_bootmsg:
        .string "MOS version 1.0.0\n\r\0"
_loadmsg:
        .string "Loading OS\n\r\0"
_rebootmsg:
        .string "Press any key to reboot\n\r\0"
_readmsg:
	.string "Reading OS disk\n\r\0"
_readmsg2:
	.string "OS disk read\n\r\0"

//------------------------------------------------------------------------
//Functions used in the boot-loading process
//------------------------------------------------------------------------
        //Message writes data in ds:si to screen
        //End character is NULL
        message:                        //Dump ds:si to screen.
                push  %edi
                movl   $(MOSBOOTSEG), %edi
                lodsb                   //load byte at ds:si into al
                pop   %edi
                orb   %al,%al           //test if character is 0 (end)
                jz    done
                movb  $0x0e, %ah        //put character
                movw  $7, %bx           //attribute
                int   $0x10             //call BIOS
                jmp   message
        done:
                ret

	//Getkey
        //holds processor until user press a key on keyboard
        getkey:
                movb  $0, %ah           //wait for key
                int   $0x16
                ret

//      ------------------------------------------------------------------     
        //Reboot
        //Reboots the system when the user pressed a key system   
        reboot:
                movw   $_rebootmsg, %si    //be polite, and say we're rebooting
                call   message
                call   getkey            //and even wait for a key :)

                .byte 0xEA               //machine language to jump to FFFF:0000 (reboot)
                .word 0x0000
                .word 0xFFFF
                // Rebooting, no ret required
//      ------------------------------------------------------------------     
        //read_from_disc
        //Reads data from disc
        //Input must be es:bx which is the destination address
        //cx is from sector
        //al is number of sectors to read
	read_from_disc:
		movw	$5, %di		//Retry to read for 5 times

	        //I could condense a few of these high/low 8-bit movs into one 16-bit
	        //mov, but, for simplicity, I'll leave it as is, unless necessary.
	        movb 	$0x02, %ah      //load two block (one sectors)
	        movb 	$0, %dh         //head = 0 = s	ide one
	        movb 	(_bootdrv), %dl  //disk = what we booted from
	
//    	        pushw	%di		//Save retry counter
		int 	$0x13
//		popw	%di		//Restore retry counter
        	jnb 	read_me_ok	//On no errors return from read_me
		xorb 	%ah, %ah        //if there's an error then we'll try again.
                int	$0x13    	//Often there is not error but requires a few
		decw 	%di
                jnz	read_from_disc	//tries.  Ofcourse, this may end up as an 
		jmp	reboot

                               //infinite loop... but only on a bad disk...
read_me_ok:
		ret
//      ------------------------------------------------------------------     
	// This procedure turns off the floppy drive motor, so
	// that we enter the kernel in a known state, and
	// don't have to worry about it later.
	// From linux

	kill_motor:
		pushw %dx
		movw $0x3f2, %dx
		movb $0, %al
		//out
		popw %dx
		ret 

//------------------------------------------------------------------------
//The actual code of our boot loading process
//------------------------------------------------------------------------
_start:
       // Flytta första bootsektorn till plats 0x1000
       // Läs sedan in resten av bootsektorn
        movw   $0x7c0, %ax
        movw   %ax, %ds
        movb   %dl, (_bootdrv)   //quickly save what drive we booted from

        // No fetch the rest of the startup code
	movw 	$(MOSBOOTSEG/0x10), %ax 
	movw   %ax, %es
        movw   $0x200, %bx
	movb   $(STARTUP-1), %al    //Number of sectors to read
	movw   $0x02, %cx           //Read from sector 2
	call   read_from_disc       //Read from driver        

        // Move the boot to an other location
        xor    %eax, %eax       
        movw   %ax, %ds         
        movw   %ax, %es
        movl   $(BOOTSIZE), %ecx
        movl   $(INITSEG), %esi   
        movl   $(MOSBOOTSEG), %edi
        rep movsb
        movl   $(MOSBOOTSEG/0x10), %eax
        movw   %ax, %ds
        ljmp   $(MOSBOOTSEG/0x10), $(ready_to_start)
ready_to_start: 
        
        // STACK
        cli                     //clear interrupts while we setup a stack
        movw   $0x9000, %ax     //put stack on 0x90000
        movw   %ax, %ss
        movw   $0xffff, %sp     //let's use the whole segment.  Why not?  We can :)
        sti                     //put our interrupts back on
        
        movw $_bootmsg, %si       //display our startup message
        call message

//.code32 //use 386 instructions from now on (I don't want to manually include
        //operand-size(66h) or address-size(67h) prefixes... it's annoying :)

        movw   $_loadmsg, %si     //tell the user we're loading the kernel
        call   message
        jmp    read_kernel
.org    0x150
read_kernel:
        // KERNEL
	//then load in the MOS kernel at the kernel offset
	movw   $KERNELSEG, %ax 

	movw   %ax, %es
	xorw   %bx, %bx             //set bx 0 bx is the offset in the segment
	movb   $KERNELSIZE, %al     //Number of sectors to read

	movw   $STARTUP, %cx        //Read from sector after startup

	call   read_from_disc       //Read from disc

	movw 	$_readmsg, %si
	call 	message

        //set A20 line, to be able to use all of memory
        xorw   %cx, %cx
clear_buf:
        inb    $0x64, %al	//get input from keyboard status port
        testb  $0x02, %al	//test the buffer full flag
        loopnz clear_buf        //loop until buffer is empty
        movb   $0xd1, %al 	//keyboard: write to output port
        outb   %al, $0x64	//output command to keyboard
clear_buf2:
        inb    $0x64, %al	//wait 'till buffer is empty again
        testb  $0x02, %al
        loopnz clear_buf2
        movb   $0xdf, %al	//keyboard: set A20
        outb   %al, $0x60	//send it to the keyboard controller
        movw   $0x14, %cx
wait_kbc:                       //this is approx. a 25uS delay to wait
        outb   %al, $0xed       //for the kb controler to execute our 
        loop   wait_kbc		//command.


	// WAIT
        movl $0xfff, %ecx	//Sätter en räknare till loopen
enloop:	loop enloop		//Bochs hänger inte med så man får vänta lite 

	//JUMP to kernel
ljmp	$0x100, $(_initkernel)

// Hang program just in case
stop:
        jmp	stop

        //The boot sector is supposed to have to have 0xAA55 at the end of 
        //the sector (the word at 510 bytes) to be loaded by the BIOS...
        .org 510, 0
        .word 0xAA55
