;
;	THIS SIMPLE PROGRAM WORK ONLY WITH THE JAGOS FROM FAVARD LAURENT
;
;	Copyright (c) FAVARD Laurent 1997-98
;	DEVPAC 3
;
;	THIS PROGRAM DEMONSTRATE HOW TO USE THE SYSTEM CALL SETICONPRG
;	CALL GET_JOYPAD.
;	This program is terminated with a clean call system.(PTERM)
;	------------------------------------------------------------------------------

			include		".\include\JagOS.inc"
			include		".\include\jaguar.inc"

			OUTPUT		SetIconP.Jag

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
;

			move.w		#EARTHEnd-EARTHBegin,-(sp)							
			move.l		#EARTHBegin,-(sp)							
			move.w		#SETICONPRG,-(sp)
			trap		#SYSTEM
			addq.l		#8,sp

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

.ck_b:		btst.l		#FIRE_B,d0								; START cardridge
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

String:			dc.b		"SetIconP: Demonstrate SetIconPrg B-Quit",0
GoodBye:		dc.b		"program demo terminated",0

;	------------------------------------------------------------------------------
;32 x 32
;	Cry Compressed Image EARTH.CRY
			DATA
			EVEN

EARTHHeader:	dc.b	"_CCI"		; compression type
		dc.w	$0010		; version number
EARTHBegin:
		dc.w	36,$0000,4,$F0FF,3,$0000,1,$007F,7,$0000,1,$007F,15,$0000,7,$F0FF,1,$0000,1,$0E7F,1,$777F
		dc.w	1,$0FFF,1,$0E7F,1,$0FFF,1,$77BF,1,$0FFF,1,$777F,1,$0E7F,15,$0000,1,$F0FF,2,$78FF,5,$F0FF
		dc.w	7,$777F,1,$0000,1,$77BF,2,$0E7F,13,$0000,1,$F0FF,2,$78FF,1,$0000,5,$F0FF,2,$0000,1,$F07F
		dc.w	2,$7040,1,$777F,1,$77BF,1,$777F,1,$0E7F,1,$0FFF,1,$777F,1,$0E7F,11,$0000,1,$F0FF,1,$78FF
		dc.w	1,$0000,1,$0FFF,1,$78FF,5,$F0FF,1,$0000,1,$F0FF,2,$7040,2,$0E7F,1,$7040,1,$777F,1,$0E7F
		dc.w	1,$0FFF,1,$777F,1,$0E7F,12,$0000,1,$0FFF,1,$78FF,1,$0FFF,1,$78FF,6,$F0FF,6,$7040,1,$777F
		dc.w	1,$77BF,1,$0FFF,1,$777F,1,$0E7F,10,$0000,1,$0FFF,1,$78FF,1,$0FFF,1,$78FF,1,$0FFF,1,$777F
		dc.w	5,$F0FF,6,$7040,2,$777F,1,$0E7F,1,$0FFF,1,$777F,1,$0E7F,8,$0000,1,$777F,1,$78FF,1,$0FFF
		dc.w	1,$78FF,1,$0FFF,1,$78FF,2,$0000,4,$F0FF,6,$7040,1,$777F,1,$0E7F,1,$0FFF,2,$0E7F,1,$777F
		dc.w	1,$0E7F,7,$0000,1,$78FF,1,$0FFF,1,$78FF,1,$0FFF,1,$78FF,1,$777F,1,$F07F,5,$F0FF,4,$7040
		dc.w	1,$777F,1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF,2,$0E7F,1,$777F,6,$0000,1,$777F,1,$0FFF
		dc.w	1,$78FF,1,$0FFF,1,$78FF,1,$0FFF,1,$777F,10,$7040,2,$77BF,1,$0FFF,1,$0E7F,1,$0FFF,2,$0E7F
		dc.w	1,$777F,1,$0E7F,1,$777F,5,$0000,1,$0FFF,1,$78FF,1,$0FFF,1,$78FF,1,$0FFF,1,$78FF,1,$77BF
		dc.w	8,$7040,2,$777F,1,$77BF,1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF,2,$0E7F,1,$777F,1,$0E7F
		dc.w	3,$0000,1,$007F,1,$0E7F,1,$77BF,1,$0FFF,1,$78FF,1,$0FFF,1,$78FF,1,$0FFF,1,$78FF,1,$0FFF
		dc.w	4,$7040,1,$77BF,1,$777F,1,$7040,1,$777F,1,$77BF,1,$0FFF,1,$77BF,1,$0FFF,1,$0E7F,1,$0FFF
		dc.w	4,$0E7F,1,$777F,1,$0E7F,1,$007F,2,$0000,1,$777F,1,$0FFF,1,$77BF,1,$0FFF,1,$78FF,1,$0FFF
		dc.w	1,$78FF,1,$0FFF,1,$78FF,1,$777F,2,$7040,1,$78FF,1,$0FFF,1,$78FF,1,$7040,1,$77BF,1,$0FFF
		dc.w	1,$77BF,1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF,2,$0E7F,1,$777F,1,$0E7F,1,$777F,3,$0000
		dc.w	1,$0FFF,1,$77BF,1,$0FFF,1,$78FF,1,$0FFF,1,$78FF,1,$0FFF,1,$78FF,1,$0FFF,1,$77BF,2,$7040
		dc.w	1,$777F,1,$7040,2,$77BF,1,$0FFF,1,$77BF,1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF,4,$0E7F
		dc.w	1,$777F,1,$0E7F,3,$0000,1,$77BF,1,$0FFF,1,$77BF,1,$0FFF,1,$78FF,1,$0FFF,1,$78FF,1,$0FFF
		dc.w	1,$78FF,1,$0FFF,1,$777F,3,$7040,1,$777F,1,$0FFF,1,$77BF,1,$0FFF,1,$77BF,1,$0FFF,1,$0E7F
		dc.w	1,$0FFF,1,$0E7F,1,$0FFF,2,$0E7F,1,$777F,1,$0E7F,1,$777F,3,$0000,1,$0FFF,1,$77BF,1,$0FFF
		dc.w	1,$77BF,1,$0FFF,1,$77BF,1,$0FFF,1,$78FF,1,$0FFF,1,$78FF,1,$0FFF,1,$78FF,1,$0FFF,1,$77BF
		dc.w	1,$7040,1,$777F,1,$0FFF,1,$777F,1,$7040,2,$77BF,1,$0E7F,1,$0FFF,4,$0E7F,1,$777F,1,$0E7F
		dc.w	3,$0000,1,$0E7F,1,$0FFF,1,$77BF,1,$0FFF,1,$77BF,1,$0FFF,1,$78FF,1,$0FFF,1,$78FF,1,$0FFF
		dc.w	1,$77BF,1,$0FFF,1,$77BF,1,$0FFF,1,$77BF,1,$777F,5,$7040,1,$777F,1,$77BF,3,$0E7F,1,$777F
		dc.w	1,$0E7F,1,$777F,3,$0000,1,$0FFF,1,$77BF,1,$0FFF,1,$77BF,1,$0FFF,1,$77BF,1,$0FFF,1,$77BF
		dc.w	1,$0FFF,1,$77BF,1,$0FFF,1,$77BF,1,$0FFF,1,$77BF,1,$0FFF,1,$77BF,7,$7040,1,$777F,3,$0E7F
		dc.w	1,$777F,1,$0E7F,3,$0000,1,$0E7F,1,$0FFF,1,$77BF,1,$0FFF,1,$77BF,1,$0FFF,1,$77BF,1,$0FFF
		dc.w	1,$77BF,1,$0FFF,1,$77BF,1,$0FFF,1,$77BF,1,$0FFF,1,$77BF,9,$7040,1,$777F,1,$0E7F,1,$777F
		dc.w	1,$0E7F,1,$777F,2,$0000,1,$007F,2,$0E7F,1,$0FFF,1,$77BF,1,$0FFF,1,$77BF,1,$0FFF,1,$77BF
		dc.w	1,$0FFF,1,$77BF,1,$0FFF,1,$77BF,1,$0FFF,2,$77BF,10,$7040,1,$777F,1,$0E7F,1,$777F,1,$0E7F
		dc.w	1,$007F,3,$0000,1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF,1,$77BF,1,$0FFF,1,$77BF,1,$0FFF
		dc.w	1,$77BF,1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F,1,$77BF,5,$7040,1,$F07F,1,$F0FF,2,$7040,1,$0E7F
		dc.w	1,$777F,1,$0E7F,5,$0000,1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF
		dc.w	1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F,4,$7040,1,$F07F,3,$F0FF,1,$0E7F
		dc.w	1,$777F,1,$0E7F,1,$777F,6,$0000,1,$777F,1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F
		dc.w	1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF,1,$777F,3,$7040,5,$F0FF,1,$0E7F
		dc.w	1,$777F,7,$0000,1,$0E7F,1,$777F,1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF
		dc.w	1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF,1,$77BF,3,$7040,5,$F0FF,1,$0E7F,8,$0000
		dc.w	1,$0E7F,1,$777F,1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF
		dc.w	1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF,3,$7040,1,$777F,5,$F0FF,9,$0000,1,$0E7F,1,$777F,1,$0E7F
		dc.w	1,$777F,1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF,1,$0E7F,1,$0FFF,3,$0E7F,3,$7040,1,$777F
		dc.w	1,$0E7F,4,$F0FF,10,$0000,1,$0E7F,1,$777F,1,$0E7F,1,$777F,1,$0E7F,1,$777F,6,$0E7F,1,$777F
		dc.w	2,$7040,1,$77BF,1,$0E7F,1,$777F,1,$0E7F,4,$F0FF,10,$0000,1,$0E7F,1,$777F,1,$0E7F,1,$777F
		dc.w	1,$0E7F,1,$777F,1,$0E7F,1,$777F,1,$0E7F,1,$777F,1,$0E7F,2,$7040,1,$777F,1,$0E7F,1,$777F
		dc.w	1,$0E7F,2,$0000,3,$F0FF,12,$0000,2,$0E7F,1,$777F,1,$0E7F,1,$777F,1,$0E7F,1,$777F,1,$0E7F
		dc.w	1,$777F,1,$7040,1,$77BF,1,$0E7F,1,$777F,5,$0000,2,$F0FF,14,$0000,3,$0E7F,1,$777F,1,$0E7F
		dc.w	1,$777F,1,$0000,1,$777F,1,$0E7F,4,$0000,4,$F0FF,15,$0000,1,$007F,7,$0000,1,$007F
EARTHEnd:	dc.w	12,$0000
;	------------------------------------------------------------------------------

				BSS
				EVEN

JoyPad1Edge:	ds.l	1						; Joypad 1 buttons's edges
JoyPad2Edge:	ds.l	1						; Joypad 2 buttons's edges
JoyPad1State:	ds.l	1						; Joypad 1 buttons's status						
JoyPad2State:	ds.l	1						; Joypad 2 buttons's status
	
			

C_END:		
			End

