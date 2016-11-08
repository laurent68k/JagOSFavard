;
;	THIS SIMPLE PROGRAM WORK ONLY WITH THE JAGOS FROM FAVARD LAURENT
;
;	Copyright (c) FAVARD Laurent 1997-98
;	DEVPAC 3
;
;	SEND A SIMPLE TEXT MESSAGE WITH THE DBMSG CALL SYSTEM.
;	This program is terminated with a clean call system.(PTERM)
;	------------------------------------------------------------------------------

			include		".\include\JagOS.inc"

			OUTPUT		Dbmessag.Jag

			OPT	D-						;	No debugging

;	------------------------------------------------------------------------------

			dc.b		'JAGR'			;	Magic to skip Devpac Header.
			dc.w		3				;	JagOS 3 - Receive & Run.
			dc.l		$5000			;	Upload adress.
			dc.l		C_END-C_Start	;	Lenght of Jaguar Code.
			dc.l		$5000			;	Start address code
	
	
			TEXT
			Org			$5000
			
C_Start:	
			pea.l		String(pc)		
			move.w		#DBMSG,-(sp)
			trap		#SYSTEM
			addq.l		#6,sp

			move.w		#PTERM,-(sp)
			trap		#SYSTEM
			
;	------------------------------------------------------------------------------

			DATA
			EVEN 			

String:		dc.b		"Jag PRG: Hi!! it is me and i was run",0

C_END:		
			End

