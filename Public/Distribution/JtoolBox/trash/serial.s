;	------------------------------------------------------------------------------
;	Project JAGUAR:	JAGUAR PROTOCOLE COMMUNICATIONS.
;	LOW LEVEL CODE FOR TOS MACHINE FOR SERIAL PORT.
;
;	File:		Serial.s
;	Author:		FAVARD Laurent
;	Date:		26 March 1999
;	Release:	26 March 1999
;	Language:	HISOFT DEVPAC 3-680x0
;
;	Funship - Atari Jaguar France / Copyright 1997-1998
;	------------------------------------------------------------------------------

TRUE					equ			1
FALSE					equ			0

DEBUG					equ			1
NORMAL					equ			2

MODE					equ			DEBUG
;MODE					equ			NORMAL

; ---------------------------------------------------------------------------
;	FOR DEBUG under TOS machines

BIOS					equ			13

DEV_AUX					equ			1
DEV_STDOUT				equ			2

BCONSTAT				equ			1
BCONIN					equ			2
BCONOUT					equ			3
BCOSTAT					equ			8

CRAWCIN					equ			7
CCONWS					equ			9
GEMDOS					equ			1

; ---------------------------------------------------------------------------
;	Export symboles

						XDEF		_InitS
						XDEF		_GetS
						XDEF		_SendS
						XDEF		_StatS

; ---------------------------------------------------------------------------
;	Import symboles
					

						TEXT

; ---------------------------------------------------------------------------
_InitS:
						rts

; ---------------------------------------------------------------------------
_StatS:
						rts

; ---------------------------------------------------------------------------
;		long cdecl GetByte(void)
;
;		Try to get a byte.
;
;		return byte in d0.l or -1L if no one.
; ---------------------------------------------------------------------------

_GetS:              	movem.l		d1-d2/a0-a2,-(sp)

.WaitToGet:				move.w		#DEV_AUX,-(sp)								;	A byte is ready ?
						move.w		#BCONSTAT,-(sp)
						trap		#BIOS
						addq.l		#4,sp

						tst.w		d0											;	d0 = 0 => no byte
						beq.s		.EndWithError

						move.w		#DEV_AUX,-(sp)								;	Yes, get it
						move.w		#BCONIN,-(sp)
						trap		#BIOS
						addq.l		#4,sp

.EndNoError:			movem.l		(sp)+,d1-d2/a0-a2
						andi.l		#$FF,d0
						rts

.EndWithError:			movem.l		(sp)+,d1-d2/a0-a2
						move.l		#-1,d0
						rts

; ---------------------------------------------------------------------------
;		long cdecl SendByte(int byte)
;
;		Wait and send a byte to the output buffer as soon as possible.
;
;		return always TRUE because it wait to send the byte.
; ---------------------------------------------------------------------------

OFF_SBYTE				equ			4+5*4										;	when via serial port
BYTE					equ			8											;	when via cartridge

_SendS:             	movem.l		d1-d2/a0-a2,-(sp)

.WaitToSend:			move.w		#DEV_AUX,-(sp)								;	ready to send ?
						move.w		#BCOSTAT,-(sp)
						trap		#BIOS
						addq.l		#4,sp

						tst.w		d0											;	not ready ?
						beq.s		.WaitToSend									;	so wait to send

						move.w		OFF_SBYTE(sp),-(sp)							;	get the byte
						move.w		#DEV_AUX,-(sp)
						move.w		#BCONOUT,-(sp)
						trap		#BIOS
						addq.l		#6,sp

				IF	MODE == DEBUG

						move.l		#7,d1
.Slow1:					move.l		#$FFFFFFFF,d0
.Slow2:					dbra		d0,.Slow2
						dbra		d1,.Slow1
						
				ENDIF
               
.EndNoError:			movem.l		(sp)+,d1-d2/a0-a2
						move.l		#TRUE,d0
						rts
; ---------------------------------------------------------------------------

				;		End
