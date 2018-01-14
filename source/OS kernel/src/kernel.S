/*
; File: kernel.asm
; The asm kernel att offset 0x0000 int the system ( the system starts at 0x90000
; so it is acctually at that position)
; Created: 2001-11-11 Markus Svensson
; Modified:
; # 1.001  2001-11-29 Markus Svensson
; # 1.002  2001-11-29 Markus Svensson
; # 1.003  2001-12-02 Markus Svensson  Eternal loop ends asm kernel. 
;				       C kernel is responible for the system
; # 0.004  2001-04-23 Markus Svensson  Changing asm dialect. 
; --------------------------------------------------------------------------------
*/
#include <mos.h>
#include <linkage.h>
#include <kernel.h>
#include <keyboard.h>
#include <idt.h>
#include <pic.h>
#include <pages.h>
#define ASSEMBLY
KERNEL_PAGE_DIR_BASE = 0x0000  
KERNEL_PAGE_TABLE_BASE = 0x0001
KERNEL_STACK = 0x1000
KERNEL_IDT =0x2000


.code32
.text
gdt:
	.word 0			/*; limit 15:0 */
	.word 0			/*; base 15:0		*/
	.byte 0			/*; base 23:16		*/
	.byte 0			/*; type				*/
	.byte 0			/*; limit 19:16, flags	*/
	.byte 0			/*; base 31:24		*/

/*; linear data segment descriptor */
	.word 0xFFFF		/* ; limit 0xFFFFF*/
	.word 0				/*; base 0		*/
	.byte 0
	.byte 0x92			/*; present, ring 0, data, expand-up, writable */
   .byte 0x4F        /*         ; page-granular, 32-bit */
	.byte 0

/*; code segment descriptor for the OS kernel */
gdt2:
	.word 0xffff	   /*     ; limit 0xFFFFF */
	.word 0x0000		/*; (base gets set above)*/
	.byte 0x00			/*	; 00000000h*/
	.byte 0x9D 	/*; present, ring 0, code, non-conforming, readable */
   .byte 0x5F	/*; page-granular, 32-bit*/
	.byte 0x00

/*; data segment descriptor */
gdt3:	
	.word	0xFFFF		/*; limit: 0xfffff*/
	.word	0x0000			/*; base:  0x00001000*/
	.byte	0x00				/*;[philippo] I think this should start at a.quadress 0*/
	.byte	0x92
	.byte	0x4F
	.byte 0x00
.org 0x1000
cli	
//#mov	ax, 0x18	; Select stack to selector 0x18
movl	$0x18, %eax

movl	%eax, %ss

/*
mov ax, 0x18		; That same selector is chossen for */

movl	$0x18, %eax
movl	%eax,%ds
movl	%eax,%fs
movl	%eax,%gs
movl	%eax,%es


movl $(KERNEL_STACK), %ebp
movl $(KERNEL_STACK), %esp 

call	_init_gdt
call	_pages_init
call	_idt_init

movl $0x18, %eax
movl	%eax,%ds
movl	%eax,%fs
movl	%eax,%gs
movl	%eax,%es


movl %cr0, %eax
andl $0x80000011, %eax
movl %eax, %cr0




call	_setup_paging

call	_pic_init

call	_keyboard_init
sti
call	_kernel_init	/* Calls the kernel written i c-code */

endloop:		/*; Hangs the system because its the c kernal
			; that takes care of the system */
jmp endloop 

.globl gdt

gdtr:
   .word 0x200		/*; GDT limit */
	.long gdt 	/*;+0x1000           ; (GDT base gets set above) */
/*; null descriptor */
 

_setup_paging:
	/*lea eax, [_unhand] */
   lea _unhand, %eax
   /*mov cr2, eax */
	movl %eax, %cr2
	/*mov eax, 0x0000		; pg_dir is at 0x7000  */
	movl $(KERNEL_PAGE_DIR_BASE), %eax
	/*mov cr3, eax		; cr3 - page directory start  */
	movl %eax, %cr3
	/*mov eax, cr0 */
	movl %cr0, %eax
	/*or  eax,0x80000000*/
	orl $0x80000000, %eax
	/*mov cr0, eax		; set paging (PG) bit  */
	movl %eax, %cr0
	ret		/*	; this also flushes prefetch-queue  */

_init_gdt:
	lgdt gdtr
	ret


/*; Unhandled interrupt funcion */
.globl _unhand
_unhand:
	/*mov byte [es:0xB8000+0], 'U' */
	iret
/*; Interrupt 0	-  Divide Error */
.globl _isr0
_isr0:
	/*mov byte [es:0xB8000+0], '0'*/
	iret
/*; Interrupt 1	-  Debug */
.globl _isr1
_isr1:
	/*mov byte [es:0xB8000+0], '1'*/
	iret

/*; Interrupt 2	-  NMI Interrupt */
.globl _isr2
_isr2:
	/*mov byte [es:0xB8000+0], '2' */
	iret

/*; Interrupt 3	-  Breakpoint*/
.globl _isr3
_isr3:
	/*mov byte [es:0xB8000+0], '3'*/
	iret

/*; Interrupt 4	-  Overflow*/
.globl _isr4
_isr4:
	/*mov byte [es:0xB8000+0], '4'*/
	iret

/*; Interrupt 5	-  BOUND Range Exceeded*/
.globl _isr5
_isr5:
	/*mov byte [es:0xB8000+0], '5'*/
	iret

/*; Interrupt 6	-  Invalid Opcode*/
.globl _isr6
_isr6:
	/*mov byte [es:0xB8000+0], '6'*/
	iret

/*; Interrupt 7	-  Device Not Available*/
.globl _isr7
_isr7:
	/*mov byte [es:0xB8000+0], '7'*/
	iret

/*; Interrupt 8	-  Double Fault */
.globl _isr8
_isr8:
	/*mov byte [es:0xB8000+500], '8'*/
	iret
