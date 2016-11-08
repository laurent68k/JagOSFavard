;
;	THIS SIMPLE PROGRAM WORK ONLY WITH THE JAGOS FROM FAVARD LAURENT
;
;	Copyright (c) FAVARD Laurent 1997-98
;	DEVPAC 3
;
;	DO A SIMPLE TEST TO SEND SOMES CHARACTERS TROUGH THE EXPANDED BOARD 
;	OF SEBASTIEN FAVARD
;

; ---------------------------------------------------------------------------
;	Emplacement bits carte JAGUAR & FALCON

SBUF_HALF		   		equ			0	; LOW Sensitivity
SBUF_FULL		   		equ			1	; LOW Sensitivity
SBUF_EMPTY		   		equ			2	; LOW Sensitivity
RBUF_HALF		   		equ			3	; LOW Sensitivity
RBUF_FULL		   		equ			4	; LOW Sensitivity
RBUF_EMPTY		   		equ			5	; LOW Sensitivity
LINK_PRESENT	   		equ			6	; LOW Sensitivity
HARD_PLUS		   		equ			7	; LOW Sensitivity

;	------------------------------------------------------------------------------

StatusBuffer	   		equ			$E1F000
ReceiveBuffer	   		equ			$E1F400
ControlBuffer	   		equ			$E1F800
SendBuffer		   		equ			$E1FC00

;	------------------------------------------------------------------------------

			include		".\include\JagOS.inc"

			OUTPUT		Board.Jag

			OPT	D-						;	No debugging

;	------------------------------------------------------------------------------
;	YES ! I use the Roine jaguar header number 3

			dc.b		'JAGR'			;	Magic to skip Devpac Header.
			dc.w		3				;	JagOS 3 - Receive & Run.
			dc.l		$5000			;	Upload adress.
			dc.l		C_END-C_Start	;	Lenght of Jaguar Code.
			dc.l		$5000			;	Start address code
	
;	------------------------------------------------------------------------------
	
			TEXT
			Org			$5000
			
C_Start:	
			pea.l		Debut(pc)		
			move.w		#DBMSG,-(sp)
			trap		#SYSTEM
			addq.l		#6,sp
			

			move.l		#String,a1		
.Many:		
.Wait:      btst.b		#SBUF_FULL,StatusBuffer
		   	beq.s		.Wait

		  	move.l		#SendBuffer,a0
            move.b		(a1)+,d0
		  	move.b		(a0,d0.w),d1

			tst.b		(a1)
			bne.s		.Many
			
			
			pea.l		Fin(pc)		
			move.w		#DBMSG,-(sp)
			trap		#SYSTEM
			addq.l		#6,sp

			move.w		#PTERM,-(sp)
			trap		#SYSTEM
			
			rts

;	------------------------------------------------------------------------------

			DATA
			EVEN 			

Debut:		dc.b		"TEST-2.Jag: Try to send via expanded board",0
Fin:		dc.b		"TEST-2.Jag: End and return under JagOS",0

String:		dc.b		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ",0

C_END:		
			End

