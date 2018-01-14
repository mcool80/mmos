; * File: io.asm
; * This file contains the fundamental i/o communications
; * with ports
; * function outportb
; * function inportb
; * function outportw
; * function inportw
; * Created: 2001-11-13 Markus Svensson
; * Modified:
; * # 1.001  2001-12-02 Markus Svensson Test function is remarked
;-----------------------------------------------------------------------------------


GLOBAL _inportb
GLOBAL _outportb

GLOBAL _inportw
GLOBAL _outportw

GLOBAL _test
SECTION .text
_inportb:
	push ebp 
        mov ebp,esp 
	mov	edx, [ebp+8]
	in	al, dx
	mov	[ebp-12], eax
	leave
	ret

_outportb:
;	pusha
	push ebp 
        mov ebp,esp 
	mov	edx,[ebp+8]
	mov	eax,[ebp+12]
	out	dx, al
	leave
;	popa
	ret
; # 1.001
;_test:
;	push ebp 
;       mov ebp,esp 
;	mov	edx, [ebp+8]
;	mov 	bx, 0x495
;	mov	ds, bx
;	hlt
;	leave
;	ret
; # 1.001
_inportw:
	push ebp 
        mov ebp,esp 
	mov	edx, [ebp+8]
	in	ax, dx
	mov	[ebp-12], eax
	leave
	ret

_outportw:
;	pusha
	push ebp 
        mov ebp,esp 
	mov	edx,[ebp+8]
	mov	eax,[ebp+12]
	out	dx, ax
	leave
;	popa
	ret

