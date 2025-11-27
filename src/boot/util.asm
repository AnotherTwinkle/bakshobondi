[bits 32]
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

print:
	pusha    ; push all registers to stack
	mov edx, VIDEO_MEMORY

	.print_char:
		mov al, [ebx]
		mov ah, WHITE_ON_BLACK

		cmp al, 0 ; null character
		je .cleanup

		mov [edx], ax
		
		add ebx, 1
		add edx, 2
		jmp .print_char

	.cleanup:
		popa
		ret

cls:
	pusha
	mov edi, VIDEO_MEMORY
	mov ecx, 80*25
	mov eax , 0x0f00

	rep stosw ; write AX to [EDI] ECX times

	popa
	ret

[bits 16]
print16bit:
	pusha    ; push all registers to stack
	mov ah, 0x0e

	.print_char:
		mov al, [bx]
		cmp al, 0 ; null character
		je .cleanup

		int 0x10
		inc bx
		jmp .print_char

	.cleanup:
		popa
		ret

HEX_OUT:
	db "0x0000", 0 ; hex out is stored somewhere in memory

print_hex:
	pusha

	mov ah, 0x0e
	mov cx, 0

	.loop:
		cmp cx, 4
		je .finish

		mov ax, dx
		and ax, 0x000f

		; ord(c) ('0' to '9' - 48 to 57) ('A' to 'F' - 65 to 70)
		add ax, 0x30 ; 0x30 = 48
		cmp al, 0x39 ; is character numeric?
		jle .modify  ; if less than 0x40, yes
		add ax, 7

	.modify:
		mov bx, HEX_OUT + 5
		sub bx, cx ; we have our index
		mov [bx], al ; copy whas in al to the position stored in bx (HEX_OUT + 5 - i)
		ror dx, 4 ; rotate, at the end of the rotation we will be back to normal

		inc cx
		jmp .loop 

	.finish:
		mov bx, HEX_OUT
		call print16bit
		popa
		ret

newline:
	pusha
	mov ah, 0x0e

	mov al, 0x0a ; newline
	int 0x10
	mov al, 0x0d; carriage return
	int 0x10

	popa
	ret

space:
	pusha
	mov ah, 0x0e

	mov al, 0x20 ; space
	int 0x10

	popa
	ret

REG_MSG : db "(dabc)", 0

reg_debug:
	pusha

	mov ah, 0x0e

	call print_hex
	call space

	mov dx, ax
	call print_hex
	call space

	mov dx, bx
	call print_hex
	call space

	mov dx, cx
	call print_hex
	call newline

	mov bx, REG_MSG
	call print16bit
	call newline
 
	popa
	ret
