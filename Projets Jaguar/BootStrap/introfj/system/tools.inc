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

ResetJaguar:				jsr			WaitSync
							clr.l		d0
							move.l		d0,OLP					; set OP to STOP-OBJECT
							move.w		d0,OBF					; clear OP-Flag
							reset
							jmp			StartBoot
