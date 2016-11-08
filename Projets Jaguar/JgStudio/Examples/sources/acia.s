;
;	THIS SIMPLE PROGRAM WORK ONLY WITH THE JAGOS FROM FAVARD LAURENT
;
;	Copyright (c) FAVARD Laurent 1998
;	DEVPAC 3
;
;	THIS PROGRAM DEMONSTRATE HOW TO THE ACIA Motorla 6850 on GPIO0-5
;	
;	This program is terminated with a clean call system.(PTERM)
;	------------------------------------------------------------------------------

			include		".\include\JagOS.inc"
			include		".\include\jaguar.inc"

			OUTPUT		Acia.Jag

			OPT	D-						;	No debugging

			dc.b		'JAGR'			;	Magic to skip Devpac Header.
			dc.w		3				;	JagOS 3 - Receive & Run.
			dc.l		$5000			;	Upload adress.
			dc.l		C_END-C_Start	;	Lenght of Jaguar Code.
			dc.l		$5000			;	Start address code

DATA_ACIA	equ			$F17C02
STATUS_ACIA	equ			$F17C00
CTRL_ACIA	equ			$F17C00
	
;	------------------------------------------------------------------------------

			TEXT
			Org			$5000
			
C_Start:	
			pea.l		String(pc)		
			move.w		#DBMSG,-(sp)
			trap		#SYSTEM
			addq.l		#6,sp

			jsr			AciaInit
			move.w		#'A',Value
			move.l		#Buffer,a1
			
;	------------------------------------------------------------------------------
;	call read joypad

.Loop:		addq.w		#1,Value

			move.w		Value,-(sp)
			jsr			AciaSendByte							
			addq.l		#2,sp

.Wait:		jsr			AciaGetByte
			tst.l		d0
			blt.s		.Wait
			move.b		d0,(a1)+

.Next:		cmpi.w		#'Z',Value
			bne			.Loop
			move.b		#0,(a1)
			
;	------------------------------------------------------------------------------

.End:		pea.l		Buffer
			move.w		#DBMSG,-(sp)
			trap		#SYSTEM
			addq.l		#6,sp

			pea.l		GoodBye(pc)		
			move.w		#DBMSG,-(sp)
			trap		#SYSTEM
			addq.l		#6,sp

			move.w		#PTERM,-(sp)
			trap		#SYSTEM
			illegal			

;	------------------------------------------------------------------------------

AciaSendByte:
.wait:		move.w		STATUS_ACIA,d0
			btst		#1,d0
			beq.b		.wait

			move.w		#'A',DATA_ACIA
			rts
			
;	------------------------------------------------------------------------------

AciaGetByte:
.wait:		move.w		STATUS_ACIA,d0
			btst		#0,d0
			beq.w		.nobyte

			move.w		DATA_ACIA,d0
			andi.l		#$FF,d0
			rts

.nobyte:	move.l		#-1,d0
			rts

			
;	------------------------------------------------------------------------------

AciaInit:	move.w	#%11,CTRL_ACIA
			move.w	#%10100001,CTRL_ACIA
			rts
			
;	------------------------------------------------------------------------------
			
			DATA
			EVEN 			

String:		dc.b		"Acia-0: Press B to quit",0
GoodBye:	dc.b		"Acia-0: Terminated",0

;	------------------------------------------------------------------------------

Value:			ds.w	1
			
Buffer:			ds.b	64

C_END:		
			End

			
		;	move.b		Value,$F14801							; GPIO/0
		;	move.b		Value,$F15001							; GPIO/1
		;	move.b		Value,$F16001							; GPIO/2
		;	move.b		Value,$F17001							; GPIO/3
		;	move.b		Value,$F17801							; GPIO/4
		;	move.b		Value,$F17C01							; GPIO/5 Used for ADC embedded
