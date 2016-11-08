;
;	THIS SIMPLE PROGRAM WORK ONLY WITH THE JAGOS FROM FAVARD LAURENT
;
;	Copyright (c) FAVARD Laurent 1997-98
;	DEVPAC 3
;
;	SEND A USER 1 MESSAGE WITH ANY DATAS ATTACHED WITH THE SYSTEM CALL USER1MSG.
;	This program is terminated with a clean call system.(PTERM)
;	------------------------------------------------------------------------------

			include		".\include\Jaguar.inc"
			include		".\include\JagOS.inc"
		
			OUTPUT		User-1.Jag

			OPT	D-						;	No debugging

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

			move.w		#10,-(sp)
			pea.l		Data(pc)	
			move.l		#TOOLBOX_PID,-(sp)	
			move.w		#USER1MSG,-(sp)
			trap		#SYSTEM
			addq.l		#8,sp

			move.w		#PTERM,-(sp)
			trap		#SYSTEM
			

			DATA
			EVEN 			

String:		dc.b		"Test-3: Send a free data buffer (USERFRAME)",13,10,0
Data:		dc.b		0,1,2,3,4,5,6,7,8,9

C_END:		
			End

