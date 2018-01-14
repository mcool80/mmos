; * File: mem.c
; * Memory functions
; * Created: 2001-11-20 Markus Svensson
; * Modified: 

[BITS 32]
GLOBAL _writemem
;GLOBAL _memcpy
GLOBAL _getdata
SECTION .text
_writemem:
	push ebp 
        mov ebp,esp 

	mov	eax, [ebp+12]
	mov	edi, eax
	mov	eax, [ebp+8]
;	push	ds
	mov	gs, ax
	mov	edx, [ebp+16]	
	mov     dword [gs:edi], edx
;	pop	ax
;	mov	gs, ax
	leave
	ret

_memcpy:
	push ebp 
        mov ebp,esp 
	
	mov	eax, [ebp+8]
	mov	edi, eax
	mov	eax, [ebp+12]
	mov	esi, eax
	mov	eax, [ebp+16]	
	mov	ecx, eax
        mov	ax, 0x463
	mov	ds, ax
	cld
	rep
	movsb
	leave
	ret

_getdata:
	mov edi, eax
	push ebp 
        mov ebp,esp 
	mov	ebx, [ebp+8]
;	mov	, eax
	mov	eax, [ebp+12]
	mov	es, ax
	mov	ax, word [es:ebx]
	mov	[ebp-12], eax
	mov	edx, 0x356
;	mov	ds, dx
	leave
	ret