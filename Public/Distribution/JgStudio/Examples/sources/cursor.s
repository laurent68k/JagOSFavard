;
;	THIS SIMPLE PROGRAM WORK ONLY WITH THE JAGOS FROM FAVARD LAURENT
;
;	Copyright (c) FAVARD Laurent 1997-98
;	DEVPAC 3
;
;	THIS PROGRAM DEMONSTRATE HOW TO USE
;	CALL GET_JOYPAD.
;	This program is terminated with a clean call system.(PTERM)
;	------------------------------------------------------------------------------

			include		".\include\JagOS.inc"
			include		".\include\jaguar.inc"

			OUTPUT		Cursor.Jag

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

;	------------------------------------------------------------------------------
;	call read joypad

.Read:		move.l		#JoyPad1State,-(sp)
			move.l		#JoyPad1Edge,-(sp)							
			move.w		#GET_JOYPAD,-(sp)
			trap		#SYSTEM
			add.l		#10,sp
					
			move.l		JoyPad1State,d0	

;	------------------------------------------------------------------------------
;	check what's button is activated

.ck_a:		btst.l		#FIRE_A,d0								; START cardridge
			beq.s		.ck_b
	
			move.w		#ARROW,-(sp)
			move.w		#SETPOINTER,-(sp)
			trap		#SYSTEM
			addq.l		#4,sp

			bra.s		.Read

.ck_b:		btst.l		#FIRE_B,d0								; START cardridge
			beq.s		.ck_c

			move.w		#BUSY,-(sp)
			move.w		#SETPOINTER,-(sp)
			trap		#SYSTEM
			addq.l		#4,sp

			bra.s		.Read

.ck_c:		btst.l		#FIRE_C,d0								; START cardridge
			beq.s		.ck_option

			move.w		#APPLEEnd-APPLEBegin,-(sp)				; Lenght of packed datas
			move.l		#APPLEBegin,-(sp)						; Address of start packed datas
			move.w		#USER_MOUSE,-(sp)						; Command to say it is an user cursor
			move.w		#SETPOINTER,-(sp)
			trap		#SYSTEM
			addq.l		#4,sp

			bra.s		.Read

.ck_option:	btst.l		#OPTION,d0
			beq.s		.Read


;	------------------------------------------------------------------------------

.End:		pea.l		GoodBye(pc)		
			move.w		#DBMSG,-(sp)
			trap		#SYSTEM
			addq.l		#6,sp


			move.w		#PTERM,-(sp)
			trap		#SYSTEM
			illegal			

;	------------------------------------------------------------------------------

				DATA
				EVEN 			

String:			dc.b		"Cursor: Demonstrate SetPointer - A-Arrow B-Busy C-free Option-Quit",0
GoodBye:		dc.b		"program demo terminated",0

;	------------------------------------------------------------------------------
;24 x 24
;	Cry Compressed Image APPLE.CRY
			DATA
			EVEN

APPLEHeader:	dc.b	"_CCI"		; compression type
		dc.w	$0010		; version number
APPLEBegin:
		dc.w	110,$0000,1,$F07F,22,$0000,1,$F07F,1,$7FF8,1,$F07F,20,$0000,1,$F07F,1,$7FF8,1,$F07F,18,$0000
		dc.w	8,$F07F,15,$0000,1,$F07F,3,$F0FF,2,$F0BF,3,$F0FF,1,$F07F,13,$0000,1,$F07F,1,$F0BF,1,$D3FF
		dc.w	1,$77EF,1,$D3FF,5,$F0FF,1,$F0DF,1,$F07F,12,$0000,1,$F07F,1,$F0FF,1,$D3FF,1,$B5FF,7,$F0FF
		dc.w	1,$F07F,12,$0000,1,$F07F,8,$F0FF,1,$F0DF,1,$F0FF,1,$F07F,12,$0000,1,$F07F,8,$F0FF,2,$F0BF
		dc.w	1,$F07F,12,$0000,1,$F07F,8,$F0FF,1,$F09F,1,$F0FF,1,$F07F,12,$0000,1,$F07F,1,$F0DF,6,$F0FF
		dc.w	1,$F09F,1,$F0BF,1,$F0FF,1,$F07F,13,$0000,1,$F07F,5,$F0FF,2,$F0DF,1,$F0FF,1,$F07F,14,$0000
		dc.w	2,$F07F,1,$F0FF,3,$F07F,1,$F09F,1,$F0FF,2,$F07F,15,$0000,3,$F07F,2,$0000,3,$F07F
APPLEEnd:	dc.w	151,$0000
;	------------------------------------------------------------------------------

				BSS
				EVEN

JoyPad1Edge:	ds.l	1						; Joypad 1 buttons's edges
JoyPad2Edge:	ds.l	1						; Joypad 2 buttons's edges
JoyPad1State:	ds.l	1						; Joypad 1 buttons's status						
JoyPad2State:	ds.l	1						; Joypad 2 buttons's status
	
			

C_END:		
			End

