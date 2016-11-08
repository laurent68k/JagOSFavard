;
;	THIS SIMPLE PROGRAM WORK ONLY WITH THE JAGOS FROM FAVARD LAURENT
;
;	Copyright (c) FAVARD Laurent 1997-98
;	DEVPAC 3
;
;	THIS PROGRAM WILL GENERATE A 68000 ADDRESS ERROR EXCEPTION.
;	This program is terminated with a clean call system.(PTERM)
;	------------------------------------------------------------------------------

;	------------------------------------------------------------------------------

			include		".\include\JagOS.inc"

			OUTPUT		ADR_ERR.JAG

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
			
			;	Now we'll generate an address error exception

			move.l		#$A0,a0
			move.l		#$A1,a1
			move.l		#$A2,a2
			move.l		#$A3,a3
			move.l		#$A4,a4
			move.l		#$A5,a5

			move.l		#$D0,d0
			move.l		#$D1,d1
			move.l		#$D2,d2
			move.l		#$D3,d3
			move.l		#$D4,d4
			move.l		#$D5,d5
			move.l		#$D6,d6
			move.l		#$D7,d7

			move.l		#$ABC,sp
			move.l		#$DEF,a6
			move.l		a6,usp
			move.l		#$A6,a6
			move.l		#$2000,sp
							
			move.l		(a1),d7						;	Oups !! Odd address...
			
			move.w		#PTERM,-(sp)
			trap		#SYSTEM
			rts

;	------------------------------------------------------------------------------

			DATA
			EVEN 			

Debut:		dc.b		"Adr_Err.Jag: Will generate an address error",0

C_END:		
			End

