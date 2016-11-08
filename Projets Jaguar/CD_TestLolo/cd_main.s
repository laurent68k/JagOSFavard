;**************************************************************************
; (C)1993 ATARI CORP.       SECRET & CONFIDENTIAL       ALL RIGHTS RESERVED
;
;
;                                cd_asamp.s
;
;               This code shows how to run the Jaguar CD subsystem to play audio thru Jerry

;                                REVISION HISTORY
;
; REV.  DATE       BY            DESCRIPTION OF EDIT
; """"  """"       ""            """""""""""""""""""
; 1.00  20 Apr 94  LJT             First release
; 2.00   4 Jun 96  Bastian Schick  ported to BJL
; 0.00   6 May 2000 Laurent Favard ported to just support JAGR header
;****************************************************************************

				OUTPUT		cdmain.jag

                include		".\include\jaguar.inc"
                include		".\include\jagcd.inc"
				include		".\include\JagOS.inc"
				
				
JERI_B			equ	 	$00F1B020
VOLUME			equ		$00F1B0A0

CODE_ADDR		equ		$5000


PRINTF          MACRO
				pea.l		\1(pc)		
				move.w		#DBMSG,-(sp)
				trap		#SYSTEM
				addq.l		#6,sp
                ENDM

				OPT	D-						;	No debugging

				dc.b		'JAGR'			;	Magic to skip Devpac Header.
				dc.w		3				;	JagOS 3 - Receive & Run.
				dc.l		CODE_ADDR		;	Upload adress.
				dc.l		C_END-C_Start	;	Lenght of Jaguar Code.
				dc.l		CODE_ADDR		;	Start address code

				TEXT
				ORG			CODE_ADDR
			
;**************************************************************************

C_Start:		
				PRINTF		Welcome

				move    #$2700,sr
                  lea     $1000.w,sp
                  lea     $F00000,a0
                  move.l  #$070007,d0           ; big endian
                  move.l  d0,$210C(a0)
                  move.l  d0,$F1A10C
                  moveq #0,d0
                  move.l d0,$2114(a0)           ; stop gpu
                  move.l d0,$f1a114             ; stop dsp
                  move.l d0,$2100(a0)           ; disable GPU-IRQs
                                                ; disable DSP-IRQs
                  move.l #%10001111100000000,$f1a100
                  move.l #%111111<<8,$f10020    ; clear and disable IRQs

                  move.l  d0,0.w
                  moveq   #4,d0
                  move.l  d0,4.w
                  moveq   #0,d0
                  move.l  d0,$20(a0)            ; set OP to STOP-OBJECT
                  move.w  d0,$26(a0)            ; clear OP-Flag
                  move.l  d0,$2A(a0)            ; border black
                  move.w  d0,$56(a0)            ; set CRY mode to color/not BW
                  move.w  d0,$58(a0)            ; background black
                  move.l  d0,$50(a0)            ; stop PIT
                  move.l  d0,$f10000            ; stop JPIT1
                  move.l  d0,$f10004            ; stop JPIT2
                  move.l  #$1F00<<16,$E0(a0)    ; clear pending irqs
                  move.w  #$7fff,$4e(a0)        ; no VI

                  lea     dummy_irq(pc),a0
                  move.l  a0,$0100.w
                  bra.s   INIT1
dummy_irq:        move.l #$1f00<<16,$f000e0
                  rte

INIT1:          nop
                nop
                move.w #$1865,$f00000
                move.l #0,$dfff00

;**************************************************************************
;	Install the CD Rom BIOS

                bsr			init_CDBIOS

;	Initialize the CD Rom BIOS

                jsr         CD_setup
                
;	Set the CD mode

				move.w      #0,d0                           ; Go to single speed, Audio mode
               	jsr         CD_mode

;	Load the DSP Code to use

Do_DSP:
                moveq		#(DSPEND-DSPSTART)>>2,d0
                lea			DSPSTART+12(pc),a0
                move.l		#D_RAM,a1
xferloop:
                move.l		(a0)+,(a1)+
                dbra    	d0,xferloop

				
;	Initialize the DSP

DSP_init:
                move.l      #JERI_B,D_PC    ; Set DSP PC to start of SRAM
                move.l      #1,D_CTRL       ; Set DSP GO bit to start running

; Set up external clock I2S mode
                move.l      #$14,SMODE

; Set up volume
                move.l      #$7fff,VOLUME

Jeri_on:
                move.w      #1,d0
                jsr         CD_jeri

; Set up 4x oversampling (See CD_osamp documentation)

                move.w      #2,d0
                jsr         CD_osamp

; Turn off mute
                move.w      #$100,JOYSTICK

; DSP's running.  Now PLAY!

Play_it: 
                            
; This starts the disk but does not send data to RAM
; Play from 0 minutes; 2 seconds; 0 frames
; The data comes in via Jerry

                move.l      #$80000200,d0   ;Start play from 00:02:00

                jsr         CD_read
                
exit:			moveq		#0,d0
forever         move.w		d0,BORD1
                addq.w		#1,d0
                bra.s		forever

;	------------------------------------------------------------------------------

init_CDBIOS::   lea			CDBIOS(pc),a0
                lea 		$3000.w,a1
                move.w 		#(CDBIOSEND-CDBIOS)>>2,d0
xferlp1         move.l 		(a0)+,(a1)+
                dbra 		d0,xferlp1
                rts

;	------------------------------------------------------------------------------

DSPSTART
				EVEN
                incbin "cd_dsp.o"
DSPEND
;	------------------------------------------------------------------------------

                EVEN
CDBIOS          incbin ".\bios\cdbios43.bin"
CDBIOSEND:

;	------------------------------------------------------------------------------

				DATA
				EVEN 			

Welcome:		dc.b		"JagCD demo: Now i'm playing your audio CD",0

C_END:
				END

