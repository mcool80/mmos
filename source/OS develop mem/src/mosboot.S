// File: mosboot.asm
// This file contains the boot up sequence
// it reads the kernel from the disk
// sets up protected mode and jumps to
// kernel
// This BOOT is written with tributes from PolyOS BOOT
// Created: 2003-04-06 Markus Svensson
// Modified:
//

// *****
// MOS boot
// *****

// The kernel code begins at 4000 in memory
KERNELOFFSET = 0x4000

// The bios starts out in 16-bit real mode
.code16
.text
.org 0, 0

jmp _start       	//skip over our data and functions

// *****
// Predefined messages given to user at boot
// *****

_bootdrv:
        .byte 0
_bootmsg:
        .string "MOS version 0.01\n\r\0"
_loadmsg:
        .string "Loading OS\n\r\0"
_rebootmsg:
        .string "Press any key to reboot\n\r\0"
_readmsg:
	.string "Reading OS disk\n\r\0"
_readmsg2:
	.string "OS disk read\n\r\0"
_pmodemsg:
        .string "Starting MOS kernel...\n\r\0"

// *****
// Functions
// *****
// Function: Message 
//  writes data in ds:si to screen
//  End character is NULL
message:          
   //load byte at ds:si into al
   lodsb        
   //test if character is 0 (end)  
   orb   %al,%al
   jz    done
   movb  $0x0e, %ah        //put character
   movw  $7, %bx           //attribute
   int   $0x10             //call BIOS
   jmp   message
done:
   ret

// Function: Getkey
//  holds processor until user press a key on keyboard

getkey:
   // wait for key using bios function ah at interrupt 16
   movb  $0, %ah
   int   $0x16
   ret

// Function: Reboot
//  Reboots the system when the user pressed a key system   
reboot:
   // report that a reboot is going to happen
   movw   $_rebootmsg, %si
   call   message
   // Wait for user action
   call   getkey
   
   //machine language to jump to FFFF:0000 (reboot)
   .byte 0xEA               
   .word 0x0000
   .word 0xFFFF
   // no ret required, rebooting

// Function: read_from_disc
//  Reads data from disc
//  Input must be es:bx which is the destination address
//  cx is from sector
//  al is number of sectors to read
read_from_disc:
   //Retry to read for 5 times
   movw	$5, %di		

   // I could condense a few of these high/low 8-bit movs into one 16-bit
   // mov, but, for simplicity, I'll leave it as is, unless necessary.

   // load two block (one sectors)
   movb 	$0x2, %ah       
   // head = 0 = s ide one  
   movb 	$0, %dh         
   // disk = what we booted from
   movb 	(_bootdrv), %dl  
   
   //Save retry counter
   pushw	%di		

   // Try to read (bios call)
   int 	$0x13
   
   // Restore retry counter
   popw	%di		
   
   // On no errors return from read_me
   jnb 	read_me_ok	

   xorb 	%al, %al        
   int	$0x13    	
   decw 	%di

   // if there's an error then we'll try again.
   // Often there is not error but requires a few
   // tries.  Ofcourse, this may end up as an 
   // infinite loop... but only on a bad disk...
   jnz	read_from_disc	
   jmp	reboot

.org 0x150

read_me_ok:
   ret

// Function: kill_motor
//  This procedure turns off the floppy drive motor, so
//  that we enter the kernel in a known state, and
//  don't have to worry about it later.
//  From linux
kill_motor:
   pushw %dx
   movw $0x3f2, %dx
   movb $0, %al
   //out
   popw %dx
   ret 

// *****
// Boot code
// *****
_start:
   // BIOS puts us at 0:07C00h, so set DS accordinly
   movl   $0x7c0,%eax
   
   // Therefore, we don't have to add 07C00h to all our data
   movw   %ax, %ds

   // quickly save what drive we booted from
   movb   %dl, (_bootdrv)

   // clear interrupts while we setup a stack
   cli
   movw   $0x9000, %ax     //put stack on 0x90000
   movw   %ax, %ss
   movw   $0xffff, %sp     //let's use the whole segment.  Why not?  We can :)
   sti                     //put our interrupts back on
        
   // Interestingly enough, apparently the processor will disable 
   // interupts itself when you directly access the stack segment!
   // At least it does in protected mode, I'm not sure about real mode.
        
   movw $_bootmsg, %si       //display our startup message
   call message

   movw   $_loadmsg, %si     //tell the user we're loading the kernel
   call   message


	//then load in the MOS kernel at the kernel offset
   movw 	$KERNELOFFSET/0x10-0x20, %ax 

   movw   %ax, %es
   xorw   %bx, %bx        //set bx 0 bx is the offset in the segment
   movb   $0x10, %al      //Number of sectors to read

   movw   $0x02, %cx      //Read from sector 2
   call   read_from_disc  //Read from driver

   movw 	$_readmsg, %si
   call 	message

   cli

   //set A20 line
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


   movl $0xfff, %ecx	//Sätter en räknare till loopen

enloop:	
   loop enloop		//Bochs hänger inte med så man får vänta lite 

        // jump to the kernel that we've loaded in...
	// ljmp KERNELOFFSET
   ljmp	$0, $KERNELOFFSET
stop:
   jmp	stop

   //The boot sector is supposed to have to have 0xAA55 at the end of 
   //the sector (the word at 510 bytes) to be loaded by the BIOS...
.org 510, 0
.word 0xAA55
