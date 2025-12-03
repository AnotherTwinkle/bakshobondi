[org 0x7c00]

jmp bootloader_init

BOOT_DRIVE: db 0

%include "src/boot/disk.asm"
%include "src/boot/util.asm"

; Memory Diagram
; 0x0000  -  0x7c00  : Bios stuff
; 0x7c00  -  0x7fff  : Boot sector
; 0x8000  -  0x8fff  : Boot stage 2
; ?       -  0x9fff  : Initial stack
; 0xa000  -  ?       : Kernel
; ?       -  0x80000 : Stack (In protected mode)

; Important : It is crucial that setup.bin is padded to match its size (currently 4096 bytes)
;             setup.asm should have its org directive at 0x8000

[bits 16]
bootloader_init:
	; BIOS stores boot drive in dl
	mov [BOOT_DRIVE], dl

	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax

	; Setup stack 
	mov bp, 0x9fff
	mov sp, bp

	mov dl, [BOOT_DRIVE]
	mov dh, 8       ; Read 8 sectors
	mov cl, 0x2     ; From sector 2
	mov bx, 0x8000  ; Load stage2 at es:bx

	; So setup stage 2 is upto 0x9000

	call disk_load

	mov dl, [BOOT_DRIVE] ; This will be needed in stage 2
	jmp 0x0000:0x8000 ; jump to stage 2

end:
	times 510-($ - $$) db 0
	dw 0xaa55