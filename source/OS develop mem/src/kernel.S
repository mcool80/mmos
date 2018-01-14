##//
#; File: kernel.S
#; The asm kernel at offset 0x0000 int the system ( the system starts at 0x90000
#; so it is acctually at that position)
#; Created: 2003-04-07 Markus Svensson
#; Modified:
#; --------------------------------------------------------------------------------
#*/
#define ASSEMBLY

KERNEL_PAGE_DIR_BASE = 0x0000  
KERNEL_PAGE_TABLE_BASE = 0x0001
KERNEL_STACK = 0x1000
KERNEL_IDT =0x0000


.code16
.text
.data
.bss
.text

gdt:
	.word 0			#//; limit 15:0 */
	.word 0			#//; base 15:0		*/
	.byte 0			#//; base 23:16		*/
	.byte 0			#//; type				*/
	.byte 0			#//; limit 19:16, flags	*/
	.byte 0			#//; base 31:24		*/

#//; linear data segment descriptor */
	.word 0xFFFF		#// ; limit 0xFFFFF*/
	.word 0				#//; base 0		*/
	.byte 0
	.byte 0x92			#//; present, ring 0, data, expand-up, writable */
	.byte 0x4F        #//         ; page-granular, 32-bit */
	.byte 0

#// code segment descriptor for the OS kernel
gdt2:
	.word 0xffff		#// limit 0xFFFFF 
	.word 0x0000		#// (base gets set above)
	.byte 0x00		#// 00000000h
	.byte 0b10011110	#// present, ring 0, code, non-conforming, readable 
	.byte 0b11011111	#// page-granular, 32-bit
	.byte 0x00

#//; data segment descriptor */
gdt3:	
	.word	0xFFFF		#// limit: 0xfffff*/
	.word	0x0000		#// base:  0x00001000*/
	.byte	0x00		#// 
	.byte	0x92
	.byte	0b11011111
	.byte 	0x00
gdt_end:

.org 0x200
cli
movw	%cs, %ax
movw	%ax, %ds

call 	_init_gdt

// movl	%eax,%ds
// movl	%eax,%fs
// movl	%eax,%gs
// movl	%eax,%es

// movl $(KERNEL_STACK), %ebp
// movl $(KERNEL_STACK), %esp  */

movl 	%cr0, %eax
orl 	$0x1, %eax
movl 	%eax, %cr0


ljmp 	$0x10, $enter_32protected
enter_32protected:

.code32

movl	$0x18, %eax
movl	%eax, %ss
movl	%eax,%ds
movl	%eax,%fs
movl	%eax,%gs
movl	%eax,%es
sti
// movl $(KERNEL_STACK), %ebp
// movl $(KERNEL_STACK), %esp  */


endloop:		#// Hangs the system because its the c kernal
#			   that takes care of the system */
jmp endloop 

 
#//
#_setup_paging:
	#// lea eax, [_unhand] 
#   	lea _unhand, %eax
   	#// mov cr2, eax 
#	movl %eax, %cr2
	#// mov eax, 0x0000		; pg_dir is at 0x7000 
#	movl $(KERNEL_PAGE_DIR_BASE), %eax
	#// mov cr3, eax		; cr3 - page directory start  
#	movl %eax, %cr3
	#// mov eax, cr0 
#	movl %cr0, %eax
	#// or  eax,0x80000000
#	orl $0x80000000, %eax
	#// mov cr0, eax		; set paging (PG) bit
#	movl %eax, %cr0
#	ret		#//	; this also flushes prefetch-queue 
.code16
.globl gdtr
_gdtr:
   .word gdt_end-gdt-1 	#// GDT limit
   .long gdt 		#// GDT base gets set above

_init_gdt:
	lgdt 	_gdtr
	ret


#// Unhandled interrupt funcion 
#.globl _unhand
#_unhand:
	#//mov byte [es:0xB8000+0], 'U' 
#	iret
#// Interrupt 0	-  Divide Error 
#.globl _isr0
#_isr0:
	#// mov byte [es:0xB8000+0], '0'
#	iret
#// Interrupt 1	-  Debug 
#.globl _isr1
#_isr1:
	#//mov byte [es:0xB8000+0], '1'
#	iret

#// Interrupt 2	-  NMI Interrupt 
#.globl _isr2
#_isr2:
	#// mov byte [es:0xB8000+0], '2' 
#	iret

#// Interrupt 3	-  Breakpoint
#.globl _isr3
#_isr3:
	#// mov byte [es:0xB8000+0], '3'
#	iret

#// Interrupt 4	-  Overflow
#.globl _isr4
#_isr4:
	#// mov byte [es:0xB8000+0], '4'
#	iret

#// Interrupt 5	-  BOUND Range Exceeded
#.globl _isr5
#_isr5:
	#// mov byte [es:0xB8000+0], '5'
#	iret

#// Interrupt 6	-  Invalid Opcode
#.globl _isr6
#_isr6:
	#// mov byte [es:0xB8000+0], '6'
#	iret

#// Interrupt 7	-  Device Not Available
#.globl _isr7
#_isr7:
	#// mov byte [es:0xB8000+0], '7'
#	iret

#// Interrupt 8	-  Double Fault 
#.globl _isr8
#_isr8:
	#// mov byte [es:0xB8000+500], '8'
#	iret

