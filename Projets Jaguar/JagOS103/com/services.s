;	------------------------------------------------------------------------------
;	Project JAGUAR:	Communications Services.
;
;	File:		Services.s
;	Author:		FAVARD Laurent
;	Date:		23 March 1998
;	Release:	25 March 1998
;	Language:	HISOFT DEVPAC 3-680x0
;
;	Funship - Atari Jaguar France / Copyright 1997-1998
;	------------------------------------------------------------------------------

; ---------------------------------------------------------------------------
;	Export symboles

                        XDEF		HandleServices

						TEXT
                        EVEN
                        
; ---------------------------------------------------------------------------
;		void cdecl HandleServices(void)
;
;		MAIN function which handle all services required by the host computer
;		via all frame command received from the extended board.
; ---------------------------------------------------------------------------

HandleServices:

L_OFF_SPID				set			8
L_OFF_TPID				set			4
L_OFF_CODE				set			2
L_OFF_LENGETBUFFER		set 		0

SB_LOCAL				set			FRAME_AREA+BIG_AREA

						movem.l		d0-d7/a0-a6,-(sp)

;	reserve the big area and frame area

						sub.l       #BIG_AREA,sp								   		;	reserve a Data area location
						move.l		sp,a1

						sub.l       #FRAME_AREA,sp								   		;	reserve a Frame's info location
						move.l		sp,a0
						move.l		sp,a2

;	reserve a location for the header and data block

                        move.l		a1,-(sp)											;	BUFFER address parameter

                        move.l		a0,-(sp)                     						;	LENGHT parameter

                        addq.l		#2,a0
                        move.l		a0,-(sp)                     						;	CODE parameter

                        addq.l		#2,a0
						move.l		a0,-(sp)                     						;	TARGET PID parameter

                        addq.l		#4,a0
                        move.l		a0,-(sp)                     						;   SOURCE PID parameter

.WaitCommand:           jsr			GetFrame 											;	get a Command Frame
                        add.l		#20,sp												;	remove parameters

                        tst.l		d0                                                  ;	d0 <= 0 ?
						ble.s		.Exit												;	exit

;	Change pointer to busybee

						move.w		#BUSY,-(sp)
						move.w		#SETPOINTER,-(sp)
						trap		#SYSTEM
						addq.l		#4,sp				

;	Command frame analysis

                        move.w		L_OFF_CODE(a2),d0									;	get the command CODE
                        andi.l		#$FFFF,d0

                        cmpi.l		#MAX_OPCODE,d0
			  			bgt.s		.UnknowCodeOp

                        lsl.l		#2,d0												;	CODE x 4

;	Get the sub-routine address:    These parameters will be given to the sub-routine
;									d0 = CODEx4
;									a1 = Data block received
;									a2 = Frame's informations

                        movea.l		#TableCommand,a0
                        adda.l		d0,a0
						move.l		(a0),a0												;	get sub-routine address
						
                        jsr			(a0)

						move.w		#ARROW,-(sp)
						move.w		#SETPOINTER,-(sp)
						trap		#SYSTEM
						addq.l		#4,sp

;	No frame available

.Exit:   				add.l       #SB_LOCAL,sp										;	remove the big location
						movem.l		(sp)+,d0-d7/a0-a6
						rts

;	an illegal code op was send in this frame

.UnknowCodeOp:          add.l       #SB_LOCAL,sp										;	remove the big location
						movem.l		(sp)+,d0-d7/a0-a6

						rts

* ---------------------------------------------------------------------------

						DATA
                        EVEN

TableCommand:			dc.l		DummyFunction					;	Code Frame 0
						dc.l		JagInquire						;	Code Frame 1
						dc.l		PrgExe							;	Code Frame 2
						dc.l		Host2Jag						;	Code Frame 3
						dc.l		Jag2Host						;	Code Frame 4
						dc.l		DummyFunction					;	Code Frame 5
						dc.l		RunExe							;	Code Frame 6
						dc.l		GetEEPROM						;	Code Frame 7
						dc.l		SetEEPROM						;	Code Frame 8

; ---------------------------------------------------------------------------
;		void cdecl DummyFunction(void)
;
;		Dummy function which is called when no code is implemented on a
;		CODE number.
; ---------------------------------------------------------------------------

                       TEXT
                       EVEN

DummyFunction:		   rts


