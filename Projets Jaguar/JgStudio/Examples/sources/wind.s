;
;	THIS SIMPLE PROGRAM WORK ONLY WITH THE JAGOS FROM FAVARD LAURENT
;
;	Copyright (c) FAVARD Laurent 1997-98
;	DEVPAC 3
;
;	THIS PROGRAM DEMONSTRATE HOW WE CAN USE THE SMALL WINDOW.
;	CALL GET_JOYPAD.
;	This program is terminated with a clean call system.(PTERM)
;	------------------------------------------------------------------------------

			include		".\include\JagOS.inc"
			include		".\include\jaguar.inc"

			OUTPUT		Wind.Jag

			OPT	D-						;	No debugging

			dc.b		'JAGR'			;	Magic to skip Devpac Header.
			dc.w		3				;	JagOS 3 - Receive & Run.
			dc.l		$5000			;	Upload adress.
			dc.l		C_END-C_Start	;	Lenght of Jaguar Code.
			dc.l		$5000			;	Start address code
	
	
			TEXT
			Org			$5000

X_STEP		equ			2
Y_STEP		equ			2

C_Start:	
			pea.l		String(pc)		
			move.w		#DBMSG,-(sp)
			trap		#SYSTEM
			addq.l		#6,sp

;	------------------------------------------------------------------------------
;	Get the work address area

			move.w		#W_WORKBUFF,-(sp)						; Ask to get the address of work area
			move.w		#W_CONSOLE,-(sp)						; Handle window ALWAYS this value !
			move.w		#WINDGET,-(sp)							; System call number
			trap		#SYSTEM
			addq.l		#6,sp		

			move.l		#WINDDEMBegin,-(sp)						; Source packed bitmap
			move.l		#WINDDEMEnd-WINDDEMBegin,-(sp)			; packed datas's size
			move.l		d0,-(sp)								; Target to unpack
			move.w		#UNPACK,-(sp)
			trap		#SYSTEM
			add.l		#14,sp

;	------------------------------------------------------------------------------
;	Get the object address (from the object list)

			move.w		#W_ADROBJ,-(sp)							; Ask to get the address object window
			move.w		#W_CONSOLE,-(sp)						; Handle window ALWAYS this value !
			move.w		#WINDGET,-(sp)							; System call number
			trap		#SYSTEM
			addq.l		#6,sp		

			tst.l		d0
			beq.s		.End

			move.l		d0,a0									; a0 = address object window

;	------------------------------------------------------------------------------
;	call read joypad

.Read:		;	Wait interrupt VBL

			move.w		#WAITSYNC,-(sp)
			trap		#SYSTEM
			addq.l		#2,sp

			;	Read Joypad#1 state
										
			move.l		#JoyPad1State,-(sp)
			move.l		#JoyPad1Edge,-(sp)							
			move.w		#GET_JOYPAD,-(sp)
			trap		#SYSTEM
			add.l		#10,sp
					
			move.l		JoyPad1State,d0	
			clr.l		d2
			clr.l		d3

;	------------------------------------------------------------------------------
;	check what's button is activated

.ck_b:		btst.l		#FIRE_B,d0
			beq.s		.ck_up
			bra.s		.End

.ck_up:		btst.l		#JOY_UP,d0
			beq.s		.ck_down
			move.l		#-1*Y_STEP,d2							; UP => 1 pixels to substract
			
.ck_down:	btst.l		#JOY_DOWN,d0
			beq.s		.ck_left
			move.l		#Y_STEP,d2								; DOWN  => 1 pixels to add

.ck_left:	btst.l		#JOY_LEFT,d0
			beq.s		.ck_right
			move.l		#-1*X_STEP,d3							; LEFT => 1 pixels to substract

.ck_right:	btst.l		#JOY_RIGHT,d0
			beq.s		.Next
			move.l		#X_STEP,d3								; RIGHT => 1 pixels to add

;	------------------------------------------------------------------------------
			
.Next:		move.l		a0,-(sp)								; Object address
			move.l		d2,-(sp)								; Y position
			move.l		d3,-(sp)								; X position
			move.w		#MOVE_OBJECT,-(sp)
			trap		#SYSTEM
			add.l		#14,sp

			bra.s		.Read
			
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

String:			dc.b		"Wind: Move the window - Press B to quit",0
GoodBye:		dc.b		"program demo terminated",0

;	------------------------------------------------------------------------------
;	128 x 54
;	Cry Compressed Image WINDDEM.CRY
			DATA
			EVEN

WINDDEMHeader:	dc.b	"_CCI"		; compression type
		dc.w	$0010		; version number
WINDDEMBegin:
		dc.w	781,$78C0,101,$7880,2,$78FF,25,$78C0,101,$7880,2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF
		dc.w	2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF
		dc.w	33,$F0FF,2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880,32,$00FF
		dc.w	34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880
		dc.w	32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0
		dc.w	2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF
		dc.w	25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF
		dc.w	2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF
		dc.w	33,$F0FF,2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880,32,$00FF
		dc.w	34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880
		dc.w	32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0
		dc.w	2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF
		dc.w	25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF
		dc.w	2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF
		dc.w	33,$F0FF,2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880,32,$00FF
		dc.w	34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880
		dc.w	32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0
		dc.w	2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF
		dc.w	25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF
		dc.w	2,$78FF,25,$78C0,2,$7880,32,$00FF,34,$78FF,33,$F0FF,2,$78FF,25,$78C0,2,$7880,101,$78FF,25,$78C0
		dc.w	103,$78FF
WINDDEMEnd:	dc.w	1164,$78C0

;	------------------------------------------------------------------------------

				BSS
				EVEN

JoyPad1Edge:	ds.l	1						; Joypad 1 buttons's edges
JoyPad2Edge:	ds.l	1						; Joypad 2 buttons's edges
JoyPad1State:	ds.l	1						; Joypad 1 buttons's status						
JoyPad2State:	ds.l	1						; Joypad 2 buttons's status
	
			

C_END:		
			End

