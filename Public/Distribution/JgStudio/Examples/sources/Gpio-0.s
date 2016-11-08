;
;	THIS SIMPLE PROGRAM WORK ONLY WITH THE JAGOS FROM FAVARD LAURENT
;
;	Copyright (c) FAVARD Laurent 1997-98
;	DEVPAC 3
;
;	THIS PROGRAM DEMONSTRATE HOW TO THE GENERAL PURPOSE I/O, GPIO0-5
;	
;	This program is terminated with a clean call system.(PTERM)
;	------------------------------------------------------------------------------

			include		".\include\JagOS.inc"
			include		".\include\jaguar.inc"

			OUTPUT		Gpio-0.Jag

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

			addq.w		#1,Value
			
		;	move.b		Value,$F14801							; GPIO/0
		;	move.b		Value,$F15001							; GPIO/1
		;	move.b		Value,$F16001							; GPIO/2
		;	move.b		Value,$F17001							; GPIO/3
		;	move.b		Value,$F17801							; GPIO/4
			move.b		Value,$F17C01							; GPIO/5 Used for ADC embedded

.ck_b:		btst.l		#FIRE_B,d0								; Finish ?
			beq.s		.Next
			bra.s		.End

.Next:		bra			.Read
			
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

String:		dc.b		"Gpio-0: Press B to quit",0
GoodBye:	dc.b		"Gpio-0: Terminated",0

;	------------------------------------------------------------------------------

			BSS
			EVEN

JoyPad1Edge:	ds.l	1						; Joypad 1 buttons's edges
JoyPad2Edge:	ds.l	1						; Joypad 2 buttons's edges
JoyPad1State:	ds.l	1						; Joypad 1 buttons's status						
JoyPad2State:	ds.l	1						; Joypad 2 buttons's status
	
Value:		ds.w	1
			

C_END:		
			End

