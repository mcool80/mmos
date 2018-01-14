GLOBAL _memcpy

_dest	equ	08h
_src	equ	0Ah
_n	equ	0Ch
t	equ	4

;void* memcpyt (void* dest, const void * src, int n)
_memcpy:
	mov edi, eax
	push ebp 
        mov ebp,esp 
	mov	eax, [ebp+8]
	mov	esi, eax
	mov	eax, [ebp+12]
	mov	edi, eax
	mov	eax, [ebp+16]	
	mov	ecx, eax
	rep 	movsw
	leave
	ret

;	push ebp
;	mov ebp, esp

;	mov	ecx,	[ebp+_n]	;Ladda räknaren
;	mov	eax,	[ebp+_src]	;Hämta pekaren till källan
;	mov	edx,	[ebp+_dest]	;Hämta pekaren till målet
;.AGAIN:	
;	mov	[ebp-4],	ecx	;Spara undan räknaren
;	mov	cl, byte [eax]		;Hämta värdet från källan
;	mov	byte [edx],	cl		;Skicka till målet
;	mov	ecx,	[ebp-4]		;Hämta tillbaka räknaren
;	inc	eax			;Flytta fram pekare för mål
;	inc	edx			;Flytta fram pekare för källa
;	loop	.AGAIN			;Gör om
;
;	mov	eax,	[ebp+_dest]	;Returnera pekare till dest

;	mov esp, ebp
;	pop ebp
;	ret
