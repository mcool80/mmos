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

;	mov	ecx,	[ebp+_n]	;Ladda r�knaren
;	mov	eax,	[ebp+_src]	;H�mta pekaren till k�llan
;	mov	edx,	[ebp+_dest]	;H�mta pekaren till m�let
;.AGAIN:	
;	mov	[ebp-4],	ecx	;Spara undan r�knaren
;	mov	cl, byte [eax]		;H�mta v�rdet fr�n k�llan
;	mov	byte [edx],	cl		;Skicka till m�let
;	mov	ecx,	[ebp-4]		;H�mta tillbaka r�knaren
;	inc	eax			;Flytta fram pekare f�r m�l
;	inc	edx			;Flytta fram pekare f�r k�lla
;	loop	.AGAIN			;G�r om
;
;	mov	eax,	[ebp+_dest]	;Returnera pekare till dest

;	mov esp, ebp
;	pop ebp
;	ret
