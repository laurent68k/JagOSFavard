;	------------------------------------------------------------------------------
;	Revision 3.00 - Tools routines.
;
;	Date:		09 May 1997
;	Revision:	09 December 1997
;
;	Funship - Atari jaguar (c) France - 1997
;
;	------------------------------------------------------------------------------


;	------------------------------------------------------------------------------
;	Jump cardridge code (Never return)

JmpCartridge:				move.w		#$1861,d0
							or.b		CARD_BUS_WIDTH,d0
							
							move.l		#MAGIC1,$F03000
							lea			$1FB000,a5
							lea			$250(A5),A2
							move.l		#MAGIC2,(A2)
							move.l		#MAGIC3,4(A2)
							
							move.w		#$8100,$F14000
							lea			$F00000,a0
							lea			CARD_GET_ADDRESS,a1
							move.w		#$FFFF,$4E(a0)
							ori.w		#$700,sr
							jmp			(a2)

;	------------------------------------------------------------------------------
;	Software Reset, to re initialize the jaguar system

ResetJaguar:				move.w		#WAITSYNC,-(sp)
							trap		#SYSTEM
							addq.l		#2,sp

QuickReset:					moveq		#0,d0
							move.l		d0,$00F02114
							move.l		d0,$00F1A114
							move.l		d0,$00F10000
							move.l		#$FF00FF00,$00F00020
							move.l		d0,$00F1A154
							move.l		d0,$00F1A150
							move.l		d0,$00F00050
							move.l		d0,$00F03FF4
							move.w		#$7fff,$f0004e
							move.w		#$1861,$00F00000
							move.l		$00E00004,-(sp)
							rts
                  
                 