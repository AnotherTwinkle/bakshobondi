[org 0x8000]

; Important ^ org is set to 0x8000

jmp setup

VBA_MODE_INFO:
	times 256 db 1


%include "src/boot/disk.asm"
%include "src/boot/gdt.asm"
%include "src/boot/util.asm"

BOOT_DRIVE: db 0
DISPLAY_FAIL_MSG : db "VESA failed", 0
GREETINGS_16BIT: db "Hello, 16 bits!", 0

KERNEL_START equ 10   	  ; 8 sectors were loaded for stage 2 bootloader
						  ; So kernel starts at sector 10

KERNEL_OFFSET equ 0xa000 ; WARNING : make sure kernel is at 0xa000 while building

[bits 16]
setup:
	mov [BOOT_DRIVE], dl

	; Greetings
	mov bx, GREETINGS_16BIT
	call print16bit
	call newline

	; mov dx, KERNEL_OFFSET
	; call print_hex

	; Enable a20... It is enabled by default on qemu though
	in al, 0x92
	or al, 2
	out 0x92, al

	call setup_display

	call load_kernel 
	call load_kernel_next 	; for some reason, cannot load more than 128 bytes on the first run
	call load_kernel_next2
	call load_kernel_next3

	jmp switch_to_protected_mode  ; we are not returning

setup_display:
	mov ah, 0x0
	mov al, 0x13   			; 320*200 256 coors
	int 0x10
	
; Kernel must be loaded in 16 bit mode using bios routines
load_kernel:
	mov dh, 128          	; Load 128*512 bytes = 64kb for kernel
	mov cl, KERNEL_START  	; Starting from this sector

	mov bx, KERNEL_OFFSET 	; Load into ES:BX
	mov dl, [BOOT_DRIVE]  	; Target drive

	call disk_load

	ret

; Load the next 128 sectors after the kernel
load_kernel_next:
    mov dh, 127               ; 2  more
    mov cl, KERNEL_START + 128 ; Starting sector after first 128

    ; Load into memory immediately after the first 64 KB
    mov bx, KERNEL_OFFSET + 0x10000 ; 64KB offset

    mov dl, [BOOT_DRIVE]      ; Target drive
    call disk_load

    ret

; Load the next 128 sectors after the kernel
load_kernel_next2:
    mov dh, 127               ; 2  more
    mov cl, KERNEL_START + 128*2 ; Starting sector after first 128

    ; Load into memory immediately after the first 64 KB
    mov bx, KERNEL_OFFSET + 0x10000*2 ; 64KB offset

    mov dl, [BOOT_DRIVE]      ; Target drive
    call disk_load

    ret

; Load the next 128 sectors after the kernel
load_kernel_next3:
    mov dh, 127               ; 2  more
    mov cl, KERNEL_START + 128*2 ; Starting sector after first 128

    ; Load into memory immediately after the first 64 KB
    mov bx, KERNEL_OFFSET + 0x10000*3 ; 64KB offset

    mov dl, [BOOT_DRIVE]      ; Target drive
    call disk_load

    ret

switch_to_protected_mode:

	cli 				  ; Clear interrupts
	lgdt [gdt_descriptor] ; Pass gdt table

	mov eax, cr0
	or eax, 0x1           ; First bit = 1 for 32 bit mode
	mov cr0, eax

	jmp CODE_SEG:pm_init  ; Far jump to 32 bits!

[bits 32]
pm_init:

	; Redefine segments
	mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax  
	mov es, ax
	mov fs, ax
	mov gs, ax

	; New stack
	mov ebp, 0x800000 ; At 8 megabytes. 
	mov esp, ebp

	jmp main

main:
	call KERNEL_OFFSET  ; Hand control over to kernel.

	jmp $