// 80x86
// File: io.asm
// This file contains the fundamental i/o communications
// with ports
// function outportb
// function inportb
// function outportw
// function inportw
// Created: 2001-11-13 Markus Svensson
// Modified:
//-----------------------------------------------------------------------------------


.globl _inportb
.globl _outportb

.globl _inportw
.globl _outportw
//.globl _getmemsize
//.globl _memorysize
.text
//_memorysize:	.word 0x0000
//		.word 0x0000
_inportb:
	pushl 	%ebp 
        movl 	%esp, %ebp 
	movl	8(%ebp), %edx
	inb	%dx, %al
	movl	%eax, -12(%ebp)
	leave
	ret

_outportb:
	pushl 	%ebp 
        movl	%esp, %ebp 
	movl	8(%ebp), %edx
	movl	12(%ebp), %eax
	outb	%al, %dx
	leave
	ret
_inportw:
	pushl 	%ebp 
        movl	%esp, %ebp 
	movl	8(%ebp), %edx
	inw	%dx, %ax
	movl	%eax, -12(%ebp)
	leave
	ret

_outportw:
	pushl 	%ebp 
        movl 	%esp, %ebp 
	movl	8(%ebp), %edx
	movl	12(%ebp), %eax
	outw	%ax, %dx
	leave
	ret

/*_getmemsize:
	pushl 	%ebp 
        movl 	%esp, %ebp
	movl	$0x18, %eax
	movl	$0x70, %edx
	outb	%al, %dx
	movl	$0x71, %edx
	inb	%dx, %al
	movl	%edx, %ebx
	shll	$8, %ebx
	movl	$0x17, %edx
	movl	$0x70, %eax
	outb	%al, %dx
	movl	$0x71, %edx
	inb	%dx, %al
	orl	%edx, %ebx
	movl	%ebx, _memorysize
	movl	%ebx, %eax
	leave
	ret */
