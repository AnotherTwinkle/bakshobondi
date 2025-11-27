; GDT
gdt_start:

gdt_init_null:
	times 2 dd 0x0 ; 8 bytes bits of 0

gdt_code:
	dw 0xffff       ; Segment Limit (bits 0 - 15)
	dw 0x0          ; Base (bits 0 - 15)
	db 0x0          ; Base (bits 16 - 23)

	; 1st flags  : (present) 1 (privillege [highest]) 00 (descriptor type) 1
	; type flags : (code) 1 (conforming) 0 (readable) 1 (accessed) 0 
	db 10011010b

	; 2nd flags : (granularity)1 (32-bit default)1 (64-bit seg)0  (AVL)0
	; Limit (bits 16-19)
	db 11001111b

	db 0x0         ; Base (bits 24-31)

gdt_data:
	dw 0xffff      ; Segment Limit 
	dw 0x0         ; Base
	db 0x0         ; Base

	; 1st flags
	; type flags : (code) 0 (expand down) 0 (writable) 1 (accessed) 0
	db 10010010b

	; 2nd flags
	; Limit (bits 16 -19)
	db 11001111b

	db 0x0

gdt_end:

gdt_descriptor:
	dw gdt_end - gdt_start - 1
	dd gdt_start

; Offset registers should be set to
; 0x0 -> NULL
; 0x08 -> CODE 
; 0x10 -> DATA

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start