;
;	THIS SIMPLE PROGRAM WORK ONLY WITH THE JAGOS FROM FAVARD LAURENT
;
;	Copyright (c) FAVARD Laurent 1997-98
;	DEVPAC 3
;
;	THIS PROGRAM DEMONSTRATE HOW TO READ THE JOYPAD #1, #2 WITH THE SYSTEM
;	CALL GET_JOYPAD.
;	This program is terminated with a clean call system.(PTERM)
;	------------------------------------------------------------------------------

			include		".\include\JagOS.inc"
			include		".\include\jaguar.inc"

			OUTPUT		Joypad.Jag

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
					
			move.l		JoyPad1Edge,d0	

;	------------------------------------------------------------------------------
;	check what's button is activated

.ck_a:		btst.l		#FIRE_A,d0								; START cardridge
			beq.s		.ck_b
			move.l		#ButtonA,a0
			bra.s		.Next

.ck_b:		btst.l		#FIRE_B,d0								; START cardridge
			beq.s		.ck_1
			bra.s		.End

.ck_1:		btst.l		#KEY_1,d0								; Send a JAG -> HOST transfer
			beq.s		.ck_option
			move.l		#Button1,a0
			bra.s		.Next

.ck_option:	btst.l		#OPTION,d0
			beq.s		.Read
			move.l		#ButtonOpt,a0

.Next:		move.l		a0,-(sp)
			move.w		#DBMSG,-(sp)
			trap		#SYSTEM
			addq.l		#6,sp

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

String:			dc.b		"Test-4: JoyPad#1 Test - Press B to quit",0
GoodBye:		dc.b		"program demo terminated",0

ButtonA:		dc.b		"Button A pressed",0
Button1:		dc.b		"Button 1 pressed",0
ButtonOpt:		dc.b		"Button Option pressed",0

;	------------------------------------------------------------------------------

				BSS
				EVEN

JoyPad1Edge:	ds.l	1						; Joypad 1 buttons's edges
JoyPad2Edge:	ds.l	1						; Joypad 2 buttons's edges
JoyPad1State:	ds.l	1						; Joypad 1 buttons's status						
JoyPad2State:	ds.l	1						; Joypad 2 buttons's status
	
			

C_END:		
			End

