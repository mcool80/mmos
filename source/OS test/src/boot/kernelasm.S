//
// File: kernel.S
// The asm kernel at offset 0x0000 int the system ( the system starts at 0x90000
// so it is acctually at that position)
// Created: 2003-04-07 Markus Svensson
// Modified:
// --------------------------------------------------------------------------------
//

.extern _kernel
.globl _initkernel
.globl _main
#define ASSEMBLY

STACK_POINTER = 0x007FFFFF
KERNEL_STACK_POINTER = 0x007fffff
KERNEL_IDT =0x0000

KERNEL_PAGE_DIR_BASE = 0x1000

GDTOFFSET = 0x00004D00

.code16
.text
.data
.bss
.text
.org 0x0
gdt:
	.word 0			// limit 15:0
	.word 0			// base 15:0
	.byte 0			// base 23:16
	.byte 0			// type
	.byte 0			// limit 19:16, flags
	.byte 0			// base 31:24

// linear data segment descriptor 
	.word 0xFFFF		// limit 0xFFFFF
	.word 0			// base 0
	.byte 0
	.byte 0x92		// present, ring 0, data, expand-up, writable
	.byte 0x4F        	// page-granular, 32-bit
	.byte 0

// Kernel code segment descriptor for the OS kernel, same area as the data
gdt2:
	.word 	0xffff		// limit 0xFFFFF 
	.word 	0x0000		// (base gets set above)
	.byte 	0x00		// 00000000h
	.byte 	0b10011110	// present, ring 0, code, non-conforming, readable 
	.byte 	0b11011111	// page-granular, 32-bit
	.byte 	0x00

// Kernel data segment descriptor 
gdt3:	
	.word	0xFFFF		// limit: 0xfffff
	.word	0x0000		// base:  0x00000000
	.byte	0x00		// base
	.byte	0x92		// 0b10010010, present, DPL=00, S=1, Read/Write
	.byte	0b11011111
	.byte 	0x00

// Kernel stack segment
gdt4:
	.word	0xFFFF		// limit: 0xffff
	.word	0x0000		// Base 0:15 = 0x0000
	.byte	0x00		// Base 16:23 = 0x70
	.byte	0x92		// 0b10010010, present, DPL=00, S=1, Read/Write
	.byte	0b11011111	// page-granular, 32-bit, totallimit 0xfffff
	.byte	0x00		// Base 24:31

// User code segment descriptor 
gdt5:
	.word 	0xffff		// limit 0xFFFFF 
	.word 	0x0000		// (base gets set above)
	.byte 	0xC0		// 00000000h
	.byte 	0b10011110	// present, ring 0, code, non-conforming, readable 
	.byte 	0b11011111	// page-granular, 32-bit
	.byte 	0x00

// User data segment descriptor 
gdt6:	
	.word	0xFFFF		// limit: 0xfffff
	.word	0x0000		// base:  0x00000000
	.byte	0xC0		// base
	.byte	0x92		// 0b10010010, present, DPL=00, S=1, Read/Write
	.byte	0b11011111
	.byte 	0x00
gdt_end:

.org 0x200
//Init the segement and paging mechanism before calling kernel
_main:
_initkernel:
movw	%cs, %ax
movw	%ax, %ds
movw	%ax, %es
movw    %ax, %ss

cli
movw    $(GDTOFFSET-0x1000), %ax
movw    %ax, %di

movw    $gdt, %ax
movw    %ax, %si

movl    $(gdt_end-gdt-1), %ecx
rep  movsb

call 	_init_gdt

jmp set_cr
//set up paging
start_page_data:
.long   0x00000193
.long   0x00400193
set_cr:
        xorl    %eax, %eax
        movl    %eax, %es
        movl    %eax, %ds
        
        movl    $(start_page_data+0x1000), %eax
        movl    %eax, %esi
        movl    $(KERNEL_PAGE_DIR_BASE), %edi
        movl    $0x8, %ecx
        rep     movsb
cli
        movl    %cr3, %eax
	orl     $(KERNEL_PAGE_DIR_BASE), %eax  // cr3 - page directory start  
	movl    %eax, %cr3
        movl    %cr0, %eax
//	orl     $0x80000000, %eax   // set paging (PG) bit
        orl 	$0x1, %eax          // Set Protection Enabled (PE) bit
	movl    %eax, %cr0 
movl	$0x18, %eax

movl	%eax,%ds
movl	%eax,%fs
movl	%eax,%gs
movl	%eax,%es

//stack
movl	$0x20, %eax
movl	%eax,%ss
movl	$0x007FFFFF, %eax
movl	%eax, %esp 

// start interrupts - hope it goes well
.org 0x300
sti
// This code is now in protected mode....
// Lets continue and 
// Jump to the kernel
ljmp    $0x10, $0x00005000


endloop:		// Hangs the system because its the c kernal
			// that takes care of the system 
jmp endloop 
 
.code16
.globl gdtr
_gdtr:
//   .word gdt_end-gdt-1 	#// GDT limit
   .word 0x300                  #// GDT limit
   .long GDTOFFSET 		#// GDT base gets set above

_init_gdt:
	lgdt 	_gdtr
	ret




