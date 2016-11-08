;
;	THIS SIMPLE PROGRAM WORK ONLY WITH THE JAGOS FROM FAVARD LAURENT
;
;	Copyright (c) FAVARD Laurent 1997-98
;	DEVPAC 3
;
;	THIS PROGRAM DEMONSTRATE HOW TO START THE CARTRIDGE.
;	CALL GET_JOYPAD.
;	This program is terminated with a clean call system.(PTERM)
;	------------------------------------------------------------------------------

			include		".\include\JagOS.inc"
			include		".\include\jaguar.inc"

			OUTPUT		Jmp.Jag

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

.ck_b:		btst.l		#FIRE_B,d0								; START cardridge
			beq.s		.Read

;	------------------------------------------------------------------------------

.End:		pea.l		GoodBye(pc)		
			move.w		#DBMSG,-(sp)
			trap		#SYSTEM
			addq.l		#6,sp


			move.w		#EX_CARTRIDGE,-(sp)
			trap		#SYSTEM
			illegal			

;	------------------------------------------------------------------------------

				DATA
				EVEN 			

String:			dc.b		"Jmp Cartridge: Demonstrate how to reset JagOS (Press B)",0
GoodBye:		dc.b		"Jump now, i'll never return to JagOS",0

;	------------------------------------------------------------------------------

				BSS
				EVEN

JoyPad1Edge:	ds.l	1						; Joypad 1 buttons's edges
JoyPad2Edge:	ds.l	1						; Joypad 2 buttons's edges
JoyPad1State:	ds.l	1						; Joypad 1 buttons's status						
JoyPad2State:	ds.l	1						; Joypad 2 buttons's status
	
			

C_END:		
			End

