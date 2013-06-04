; HOHO BOOT LOADER
FLASH_START	.equ 0x64000000
CODE_START	.equ 0x00000400
CODE_SIZE	.equ 0x00000400

	.global _c_int00

	.sect ".boot_load"
	.global _boot_start
_boot_start:
	MVKL	FLASH_START,B4	; Flash start address -> B4
	MVKH	FLASH_START,B4

	MVKL	CODE_START,A4	; Apps code start address -> A4
	MVKH	CODE_START,A4
	ZERO	A1

_boot_loop1:
	LDB		*B4++,B5
	MVKL	CODE_SIZE-4,B6
	ADD		1,A1,A1
||	MVKH	CODE_SIZE-4,B6
	CMPLT	A1,B6,B0
	NOP
	STB		B5,*A4++
	[B0]	B	_boot_loop1
	NOP		5
	
	MVKL	.S2 _c_int00, B0
	MVKH	.S2	_c_int00, B0
	B		.S2	B0
	NOP		5

	