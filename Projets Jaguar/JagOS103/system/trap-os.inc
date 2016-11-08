;	------------------------------------------------------------------------------
;	Project JAGUAR:	HANDLER Trap system Communications Services.
;
;	File:		Services.s
;	Author:		FAVARD Laurent
;	Date:		26 March 1998
;	Release:	03 Jully 1998
;	Language:	HISOFT DEVPAC 3-680x0
;
;	Funship - Atari Jaguar France / Copyright 1997-1998
;	------------------------------------------------------------------------------

                        
; ---------------------------------------------------------------------------
;	Export symboles

                        TEXT
                        EVEN

; ---------------------------------------------------------------------------
;		void cdecl TrapSystem(void)
;
;		This function is the Trap handler for communications services.
;
;		Parameters:	passed in the stack
;		Return:		d0 = always modified
;                   ALL others registers saved 
; ---------------------------------------------------------------------------


JagOS:

OFF_FCTNUMBER			equ			0
OFF_BEGINPARAM			equ			14+(12*4)									;	start of parameters in teh stack

MAX_CODE				equ			25											;	maximal function number

						movem.l		d1-d7/a0-a6,-(sp)

						move.l		sp,a1
                        add.l		#OFF_BEGINPARAM,a1	 						;	a1 pointer to start of parameters area

						move.w		OFF_FCTNUMBER(a1),d0
                        andi.l		#$FFFF,d0

                        cmpi.w		#MAX_CODE,d0
                        bgt.s		.UnknowNumber

						lsl.l		#2,d0
                        lea.l		TableRoutine,a0
                        add.l		d0,a0

; ---------------------------------------------------------------------------
;	Now we call the sub-routine: 	a0 is always saved
;									a1 pointer to the input parameters area
;                                   d0 can be used for given a return value
;
;	Ths sub-routine MUST resored itself the d0-d7/a0-a6 registers and perform a RTE.
; ---------------------------------------------------------------------------

						move.l		(a0),a0
						jmp			(a0)

; ---------------------------------------------------------------------------
;	If the number is to great

.UnknowNumber:			movem.l		(sp)+,d1-d7/a0-a6
                        RTE

; ---------------------------------------------------------------------------

						DATA
                        EVEN

TableRoutine:          	dc.l		JagMessage						;	function 0
						dc.l		JHost2Jag					 	;	function 1
						dc.l		JJag2Host					 	;	function 2
						dc.l		Unpack							;	function 3
						dc.l		Disable							;	function 4
						dc.l		MoveObject						;	function 5
						dc.l		ReadJoyPads						;	function 6
						dc.l		MakeObjList						;	function 7
						dc.l		InitOP							;	function 8
						dc.l		WaitVBL							;	function 9
						dc.l		SetCursor						;	function 10
						dc.l		WriteEEPROM						;	function 11
						dc.l		ReadEEPROM						;	function 12
						dc.l		ClearEEPROM						;	function 13
						dc.l		VideoRate						;	function 14
						dc.l		MemSize							;	function 15
						dc.l		HostConnected					;	function 16
						dc.l		GetVersion						;	function 17
						dc.l		SetIconPRG						;	function 18
						dc.l		Pterm							;	function 19
						dc.l		UserFrame						;	function 20
						dc.l		WindGet							;	function 21
						dc.l		Enable							;	function 22
						dc.l		ResetJaguar						;	function 23
						dc.l		JmpCartridge					;	function 24
						dc.l		SetDriver						;	function 25
						
; ---------------------------------------------------------------------------

