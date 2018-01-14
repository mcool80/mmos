; * File: mosboot.asm
; * This file contains the boot up sequence
; * it reads the kernel from the disk
; * sets up protected mode and jumps to
; * kernel
; * This BOOT is written with tributes from PolyOS BOOT
; * Created: 2001-11-13 Markus Svensson
; * Modified:
; * # 1.001  2001-12-02 Markus Svensson Getkey remarked
; * # 1.002  2001-12-02 Markus Svensson IDTR offset changed
; ------------------------------------------------------------------------
; MOS boot
; ------------------------------------------------------------------------
IDTOFFSET	equ	0x2000
KERNELOFFSET 	equ	0x4000
[BITS 16]       ; the bios starts out in 16-bit real mode
[ORG 0]

; ------------------------------------------------------------------------
; SECTOR ONE: THE BOOT LOADER
; ------------------------------------------------------------------------

jmp start       ; skip over our data and functions

; ------------------------------------------------------------------------
; Data used in the boot-loading process
; ------------------------------------------------------------------------
        ; User messages
        bootdrv         db 0
        bootmsg         db 'MOS version 0.01',13,10,0
        loadmsg         db 'Loading OS',13,10,0
        rebootmsg       db 'Press any key to reboot',13,10,0
	readmsg		db 'Reading OS disk',13,10,0

        ;a20msg          db '',13,10,0
        pmodemsg        db 'Starting MOS kernel...',13,10,0

        ; Here's the locations of my IDT and GDT.  Remember, Intel's are
        ; little endian processors, therefore, these are in reversed order.
        ; Also note that lidt and lgdt accept a 32-bit address and 16-bit 
        ; limit, therefore, these are 48-bit variables.
; # 1.002
        idtr            dw 07ffh         ; limit of 256 IDT slots
                        dd IDTOFFSET     ; starting at IDTOFFSET
; # 1.002

; ------------------------------------------------------------------------
; Functions used in the boot-loading process
; ------------------------------------------------------------------------
        ; Message writes data in ds:si to screen
        ; End character is NULL
        message:                        ; Dump ds:si to screen.
                lodsb                   ; load byte at ds:si into al
                or al,al                ; test if character is 0 (end)
                jz done
                mov ah,0eh              ; put character
                mov bx,0007             ; attribute
                int 0x10                ; call BIOS
                jmp message
        done:
                ret

;       ------------------------------------------------------------------
;	putc:
;                mov ah,0eh              ; put character
;                mov bx,0007             ; attribute
;                int 0x10                ; call BIOS
;		ret
;       ------------------------------------------------------------------
	; Getkey
        ; holds processor until user press a key on keyboard
        getkey:
                mov ah, 0               ; wait for key
                int 016h
                ret

;       ------------------------------------------------------------------     
        ; Reboot
        ; Reboots the system when the user pressed a key system   
        reboot:
                mov si, rebootmsg       ; be polite, and say we're rebooting
                call message
                call getkey             ; and even wait for a key :)

                db 0EAh                 ; machine language to jump to FFFF:0000 (reboot)
                dw 0000h
                dw 0FFFFh
                ; no ret required; we're rebooting! (Hey, I just saved a byte :)
;       ------------------------------------------------------------------     
        ; read_from_disc
        ; Reads data from disc
        ; Input must be es:bx which is the destination address
        ; cx is from sector
        ; al is number of sectors to read
	read_from_disc:
		mov	di, 05h		; Retry to read for 5 times

	        ; I could condense a few of these high/low 8-bit movs into one 16-bit
	        ; mov, but, for simplicity, I'll leave it as is, unless necessary.
	        mov 	ah,0x02         ; load two block (one sectors)
	        mov 	dh,0            ; head = 0 = s	ide one
	        mov 	dl,[bootdrv]    ; disk = what we booted from
	
    	        push	di		; Save retry counter
		int 	13h
		pop	di		; Restore retry counter
        	jnb 	read_me_ok	; On no errors return from read_me
		xor 	ax,ax	        ; if there's an error then we'll try again.
                int	13h    	        ; Often there is not error but requires a few
		dec 	di
                jnz	read_from_disc	; tries.  Ofcourse, this may end up as an 
		jmp	reboot
	                                ; infinite loop... but only on a bad disk...
read_me_ok:
		ret
;       ------------------------------------------------------------------     
	;/* This procedure turns off the floppy drive motor, so
	; * that we enter the kernel in a known state, and
	; * don't have to worry about it later.
	; */ snodd från linux

	kill_motor:
		push dx
		mov dx,0x3f2
		mov al,0
		outb
		pop dx
		ret 

; -------------------------------------------
; The actual code of our boot loading process
; ------------------------------------------------------------------------
start:
        mov ax,0x7c0    ; BIOS puts us at 0:07C00h, so set DS accordinly
        mov ds,ax       ; Therefore, we don't have to add 07C00h to all our data

        mov [bootdrv], dl ; quickly save what drive we booted from

        cli             ; clear interrupts while we setup a stack
        mov ax,0x9000   ; this seems to be the typical place for a stack
        mov ss,ax
        mov sp,0xffff   ; let's use the whole segment.  Why not?  We can :)
        sti             ; put our interrupts back on
        
        ; Interestingly enough, apparently the processor will disable 
        ; interupts itself when you directly access the stack segment!
        ; Atleast it does in protected mode, I'm not sure about real mode.
        
        mov si,bootmsg  ; display our startup message
        call message

.386    ; use 386 instructions from now on (I don't want to manually include
        ; operand-size(66h) or address-size(67h) prefixes... it's annoying :)

        mov 	si,loadmsg  ; tell the user we're loading the kernel
        call 	message


	; then load in the MOS kernel
	mov 	ax, KERNELOFFSET/0x10-0x100          ; The kernel starts at this segment

	mov 	es, ax
	xor 	bx, bx		; set bx 0 bx is the offset in the segment
	mov	al, 20h		; Number of sectors to read

	mov	cx, 0002h	; Read from sector 2

	call	read_from_disc  ; Read from driver


	mov 	si, readmsg
	call 	message
	call 	kill_motor

	; Point to gdtr in the boot
	mov 	bx,ds                       ; BX=segment
	shl 	ebx,4                       ; BX="linear" address of segment base
	lea 	eax,[gdt+ebx]
	mov	[gdtr+2], eax

        ; the A20 line is on now.  Let's load in our IDT and GDT tables...
        ; Ideally, there will actually be data in their locations (by loading 
        ; the kernel)
	cli

        ; set A20 line
        xor cx, cx
clear_buf:
        in al, 64h              ; get input from keyboard status port
        test al, 02h            ; test the buffer full flag
        loopnz clear_buf        ; loop until buffer is empty
        mov al, 0D1h            ; keyboard: write to output port
        out 64h, al             ; output command to keyboard
clear_buf2:
        in al, 64h              ; wait 'till buffer is empty again
        test al, 02h
        loopnz clear_buf2
        mov al, 0dfh            ; keyboard: set A20
        out 60h, al             ; send it to the keyboard controller
        mov cx, 14h
wait_kbc:                       ; this is approx. a 25uS delay to wait
        out 0edh, ax            ; for the kb controler to execute our 
        loop wait_kbc           ; command.

        lgdt [gdtr]


        ; now let's enter pmode...
        mov si, pmodemsg
        call message

; # 1.001
;        call getkey
	mov	ecx, 0xfff	; Sätter en räknare till loopen
enloop:	loop enloop		; Booch hänger inte med så man får vänta lite
; # 1.001

	
	mov eax, cr0            ; load the control register in
        or  al, 1               ; set bit 1: pmode bit
        mov cr0, eax            ; copy it back to the control register
	
        lidt [idtr]		; Set up the IDTR

	jmp flush
flush:


        ; jump to the kernel that we've loaded in...
	jmp 	DWORD SYS_CODE_SEL:KERNELOFFSET
stop:
jmp	stop
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	global descriptor table (GDT)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
gdtr:	dw gdt_end - gdt - 1	; GDT limit
	dd gdt+7c00h            ; (GDT base gets set above)

; null descriptor
gdt:	dw 0			; limit 15:0
	dw 0			; base 15:0
	db 0			; base 23:16
	db 0			; type
	db 0			; limit 19:16, flags
	db 0			; base 31:24

; linear data segment descriptor
LINEAR_SEL	equ	$-gdt
	dw 0xFFFF		; limit 0xFFFFF
	dw 0			; base 0
	db 0
	db 0x92			; present, ring 0, data, expand-up, writable
        db 0x4F                 ; page-granular, 32-bit
	db 0
; code segment descriptor for the OS kernel
SYS_CODE_SEL	equ	$-gdt
gdt2:   dw 0xffff	        ; limit 0xFFFFF
	dw 0x0000		; (base gets set above)
	db 00000000b		; 00000000h
	db 10011101b 	; present, ring 0, code, non-conforming, readable
        db 01011111b	; page-granular, 32-bit

	db 0x00

; data segment descriptor
SYS_DATA_SEL	equ	$-gdt
gdt3:	dw	0xFFFF		; limit: 0xfffff
	dw 	0000h		; base:  0x00001000
	db	00h		;[philippo] I think this should start at address 0
	db	10010010b
	db	01001111b
	db 	00h

gdt_end:
        times 0x1be-($-$$) db 0
        ; The boot sector is supposed to have to have 0xAA55 at the end of 
        ; the sector (the word at 510 bytes) to be loaded by the BIOS...
        times 510-($-$$) db 0
        dw 0xAA55
