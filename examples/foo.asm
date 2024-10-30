.org #0xD000

_start:
	mov .word [0x0020], #5356
	mov r2, .word [0x0020]
	sub r3, r2, r2

	jz _start
	hlt