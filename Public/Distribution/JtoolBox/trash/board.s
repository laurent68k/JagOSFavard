;	------------------------------------------------------------------------------
;	Project JAGUAR:	JAGUAR PROTOCOLE COMMUNICATIONS.
;	LOW LEVEL CODE FOR TOS MACHINE FOR ENHANCED BOARD JAGUAR.
;
;	File:		Board.s
;	Author:		FAVARD Laurent
;	Date:		26 March 1999
;	Release:	26 March 1999
;	Language:	HISOFT DEVPAC 3-680x0
;
;	Funship - Atari Jaguar France / Copyright 1997-1998
;	------------------------------------------------------------------------------

						include		".\JProtolo.inc"

; ---------------------------------------------------------------------------
;	Export symboles

						XDEF		_InitB
						XDEF		_GetB
						XDEF		_SendB
						XDEF		_StatB

; ---------------------------------------------------------------------------
;	Import symboles
				

						TEXT
; ---------------------------------------------------------------------------

_InitB:

; ---------------------------------------------------------------------------

_StatB:					move.b		STATUS_BUFFER,d0
						btst.b		#CARTDRIDGE_SET,d0
						beq.s		.Detected
						
						move.w		#FALSE,d0
						rts
.Detected:				move.w		#TRUE,d0
						rts


; ---------------------------------------------------------------------------

_GetB:	              	btst.b		#RBUF_EMPTY,STATUS_BUFFER					;	FIFO empty ?
						beq.s		.NoByte										;	no bytes so wait

						move.b		RECEIVE_BUFFER,d0
						andi.l		#$FF,d0
						rts

.NoByte:                move.l		#-1,d0
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

_SendB:             move.l		a0,-(sp)

.WaitSendBoard:			btst.b		#SBUF_FULL,STATUS_BUFFER					;	test status register
						beq.s		.WaitSendBoard								;	FIFO full, wait

						movea.l		#SEND_BUFFER,a0								;	a0 <- Send register
						moveq.l		#0,d0										;	d0 <- 0.l
						move.w		BYTE(sp),d0									;	d0 <- data.w
						move.b		0(a0,d0.w),d0								;	d0 <- Read(a0 OR d0)

						move.l		(sp)+,a0
                        move.l		#TRUE,d0
						rts

; ---------------------------------------------------------------------------

				;		End
