            .title  "Flash bootup utility for 6416 dsk"
            .option D,T
            .length 102
            .width  140

PAGE_SIZE     .equ    0x01000      ;flash page size in byte
BOOT_SIZE     .equ    0x800        ;bootup code size in byte
FLASH_START   .equ    0x64000000   ;flash start address
BOOT_START    .equ    0x00000000   ;L2 sram start address
CODE_SIZE     .equ    0xF800       ;application code size in byte
CODE_START    .equ    0x800        ;application code start address
FLASH_REG1    .equ    0x90000555   ;address of the flash control reg 1
FLASH_REG2    .equ    0x90000AAA   ;address of the flash control reg 2
FLASH_KEY1    .equ    0xAA
FLASH_KEY2    .equ    0x55
FLASH_KEY3    .equ    0xA0

;EMIFA Register Addresses
EMIFA_GCTL       .equ  0x01800000  ;EMIFA global control
EMIFA_CE1        .equ  0x01800004  ;address of EMIFA CE1 control reg.
EMIFA_CE0        .equ  0x01800008  ;EMIFA CE0control
EMIFA_CE2        .equ  0x01800010  ;EMIFA CE2control
EMIFA_CE3        .equ  0x01800014  ;EMIFA CE3control
EMIFA_SDRAMCTL   .equ  0x01800018  ;EMIFA SDRAM control
EMIFA_SDRAMTIM   .equ  0x0180001c  ;EMIFA SDRAM timer
EMIFA_SDRAMEXT   .equ  0x01800020  ;EMIFA SDRAM extension
EMIFA_CE1SECCTL  .equ  0x01800044  ;EMIFA CE1 secondary control
EMIFA_CE0SECCTL  .equ  0x01800048  ;EMIFA CE0 secondary control
EMIFA_CE2SECCTL  .equ  0x01800050  ;EMIFA CE2 secondary control
EMIFA_CE3SECCTL  .equ  0x01800054  ;EMIFA CE3 secondary control

; EMIFA Register Values for 6416 DSK
EMIFA_GCTL_V     .equ  0x00012070  ;
EMIFA_CE0_V      .equ  0xffffffd3  ;EMIFA SDRAM
EMIFA_CE2_V      .equ  0x22a28a22  ;EMIFA CE2 Daughtercard 32-bit async
EMIFA_CE3_V      .equ  0x22a28a22  ;EMIFA CE3 Daughtercard 32-bit async
EMIFA_SDRAMCTL_V .equ  0x47115000  ;EMIFA SDRAM control
EMIFA_SDRAMTIM_V .equ  0x00000618  ;SDRAM timing (refresh)
EMIFA_SDRAMEXT_V .equ  0x000a8529  ;SDRAM extended control

; EMIFB Register Addresses
EMIFB_GCTL       .equ  0x01A80000  ;EMIFB global control
EMIFB_CE1        .equ  0x01A80004  ;address of EMIFB CE1 control reg.
EMIFB_CE0        .equ  0x01A80008  ;EMIFB CE0control
EMIFB_CE2        .equ  0x01A80010  ;EMIFB CE2control
EMIFB_CE3        .equ  0x01A80014  ;EMIFB CE3control
EMIFB_SDRAMCTL   .equ  0x01A80018  ;EMIFB SDRAM control
EMIFB_SDRAMTIM   .equ  0x01A8001c  ;EMIFB SDRAM timer
EMIFB_SDRAMEXT   .equ  0x01A80020  ;EMIFB SDRAM extension
EMIFB_CE1SECCTL  .equ  0x01A80044  ;EMIFB CE1 secondary control
EMIFB_CE0SECCTL  .equ  0x01A80048  ;EMIFB CE0 secondary control
EMIFB_CE2SECCTL  .equ  0x01A80050  ;EMIFB CE2 secondary control
EMIFB_CE3SECCTL  .equ  0x01A80054  ;EMIFB CE3 secondary control

; EMIFB Register Values for the 6416 DSK
EMIFB_GCTL_V     .equ  0x00002070  ;EMIFB global control
EMIFB_CE0_V      .equ  0x22008800  ;CE0 CPLD 8-bit
EMIFB_CE1_V      .equ  0x22008800  ;CE1 Flash 8-bit

COPY_TABLE       .equ  0x64000400

            .sect ".boot_load"
            .global _boot

_boot:      
;************************************************************************
;* DEBUG LOOP -  COMMENT OUT B FOR NORMAL OPERATION
;************************************************************************

;            zero B1
;_myloop:  ; [!B1] B _myloop  
;            nop  5
;_myloopend: nop

;************************************************************************
;* CONFIGURE EMIF
;************************************************************************

        ;****************************************************************
        ; *EMIFA_GCTL = EMIFA_GCTL_V;
        ;****************************************************************

            mvkl  EMIFA_GCTL,A4    
      ||    mvkl  EMIFA_GCTL_V,B4

            mvkh  EMIFA_GCTL,A4
      ||    mvkh  EMIFA_GCTL_V,B4

            stw   B4,*A4

        ;****************************************************************
        ; *EMIFA_CE0 = EMIFA_CE0_V
        ;****************************************************************

            mvkl  EMIFA_CE0,A4       
      ||    mvkl  EMIFA_CE0_V,B4     

            mvkh  EMIFA_CE0,A4
      ||    mvkh  EMIFA_CE0_V,B4

            stw   B4,*A4

        ;****************************************************************
        ; *EMIFA_CE2 = EMIFA_CE2_V (setup for 32-bit async)
        ;****************************************************************

            mvkl  EMIFA_CE2,A4       
      ||    mvkl  EMIFA_CE2_V,B4

            mvkh  EMIFA_CE2,A4
      ||    mvkh  EMIFA_CE2_V,B4

            stw   B4,*A4

        ;****************************************************************
        ; *EMIFA_CE3 = EMIFA_CE3_V (setup for 32-bit async)
        ;****************************************************************

      ||    mvkl  EMIFA_CE3,A4    
      ||    mvkl  EMIFA_CE3_V,B4     ;

            mvkh  EMIFA_CE3,A4
      ||    mvkh  EMIFA_CE3_V,B4

            stw   B4,*A4

        ;****************************************************************
        ; *EMIFA_SDRAMCTL = EMIFA_SDRAMCTL_V
        ;****************************************************************
      ||    mvkl  EMIFA_SDRAMCTL,A4      
      ||    mvkl  EMIFA_SDRAMCTL_V,B4    ;

            mvkh  EMIFA_SDRAMCTL,A4
      ||    mvkh  EMIFA_SDRAMCTL_V,B4

            stw   B4,*A4

        ;****************************************************************
        ; *EMIFA_SDRAMTIM = EMIFA_SDRAMTIM_V
        ;****************************************************************
      ||    mvkl  EMIFA_SDRAMTIM,A4      
      ||    mvkl  EMIFA_SDRAMTIM_V,B4    ;

            mvkh  EMIFA_SDRAMTIM,A4
      ||    mvkh  EMIFA_SDRAMTIM_V,B4

            stw   B4,*A4

        ;****************************************************************
        ; *EMIFA_SDRAMEXT = EMIFA_SDRAMEXT_V
        ;****************************************************************
      ||    mvkl  EMIFA_SDRAMEXT,A4      
      ||    mvkl  EMIFA_SDRAMEXT_V,B4    ;

            mvkh  EMIFA_SDRAMEXT,A4
      ||    mvkh  EMIFA_SDRAMEXT_V,B4

            stw   B4,*A4

        ;****************************************************************
        ; *EMIFB_GCTL = EMIFB_GCTL_V
        ;****************************************************************
      ||    mvkl  EMIFB_GCTL,A4      
      ||    mvkl  EMIFB_GCTL_V,B4    ;

            mvkh  EMIFB_GCTL,A4
      ||    mvkh  EMIFB_GCTL_V,B4

            stw   B4,*A4

        ;****************************************************************
        ; *EMIFB_CE0 = EMIFB_CE0_V
        ;****************************************************************
      ||    mvkl  EMIFB_CE0,A4      
      ||    mvkl  EMIFB_CE0_V,B4    ;

            mvkh  EMIFB_CE0,A4
      ||    mvkh  EMIFB_CE0_V,B4

            stw   B4,*A4

        ;****************************************************************
        ; *EMIFB_CE1 = EMIFB_CE1_V
        ;****************************************************************
      ||    mvkl  EMIFB_CE1,A4      
      ||    mvkl  EMIFB_CE1_V,B4    ;

            mvkh  EMIFB_CE1,A4
      ||    mvkh  EMIFB_CE1_V,B4

            stw   B4,*A4

;****************************************************************************
; copy sections
;****************************************************************************
        mvkl  COPY_TABLE, a3 ; load table pointer
        mvkh  COPY_TABLE, a3

        ldw   *a3++, b1     ; Load entry point

copy_section_top:
        ldw   *a3++, b0     ; byte count 
        ldw   *a3++, a4     ; ram start address
        nop   3

 [!b0]  b copy_done         ; have we copied all sections?
        nop   5

copy_loop:
        ldb   *a3++,b5
        sub   b0,1,b0       ; decrement counter
 [ b0]  b     copy_loop     ; setup branch if not done
 [!b0]  b     copy_section_top
        zero  a1
 [!b0]  and   3,a3,a1
        stb   b5,*a4++
 [!b0]  and   -4,a3,a5      ; round address up to next multiple of 4
 [ a1]  add   4,a5,a3       ; round address up to next multiple of 4

;****************************************************************************
; jump to entry point
;****************************************************************************
copy_done:
        b    .S2 b1
        nop   5
