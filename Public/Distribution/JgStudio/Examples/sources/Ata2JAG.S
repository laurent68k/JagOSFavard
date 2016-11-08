;
;	THIS SIMPLE PROGRAM WORK ONLY WITH THE JAGOS FROM FAVARD LAURENT
;
;	Copyright (c) FAVARD Laurent 1997-98
;	DEVPAC 3
;
;	SHOW HOW TO PERFORM A DATA TRANSFER FROM THE JAGUAR TO THE HOST.
;	USE THE SEND DATA SYSTEM CALL.
;	This program is terminated with a clean call system.(PTERM)
;	------------------------------------------------------------------------------

			include		".\include\JagOS.inc"
			include		".\include\Jaguar.inc"

			OUTPUT		Ata2jag.Jag

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

;	------------------------------------------------------------------------------

			move.l		#128,-(sp)										;	Lenght of datas to get from the host
			move.l		#DATABBegin,-(sp)								;	Address of datas to store them
			move.l		#TOOLBOX_PID,-(sp)								;	ST process addressed
			move.w		#GETDATA,-(sp)
			trap		#SYSTEM
			add.l		#14,sp

;	------------------------------------------------------------------------------

			pea.l		Goodbye(pc)		
			move.w		#DBMSG,-(sp)
			trap		#SYSTEM
			addq.l		#6,sp

			move.w		#PTERM,-(sp)
			trap		#SYSTEM
			
;	------------------------------------------------------------------------------

			DATA
			EVEN 			

String:		dc.b		"Host2jag: demonstrate a datas transfer ST to Jaguar",0
Goodbye:	dc.b		"program terminated",0

			BSS
			EVEN

DATABBegin:	ds.b		128

C_END:		
			End

