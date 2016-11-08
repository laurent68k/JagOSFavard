;	------------------------------------------------------------------------------
;	Project JAGUAR:	JAGUAR PROTOCOLE COMMUNICATIONS.
;	LOW LEVEL CODE FOR TOS MACHINE VIA CARTRIDGE AND SERIAL PORT.
;
;	File:		JProtolo.s
;	Author:		FAVARD Laurent
;	Date:		20 January 1998
;	Release:	26 March 1999
;	Language:	HISOFT DEVPAC 3-680x0
;
;	Funship - Atari Jaguar France / Copyright 1997-1999
;	------------------------------------------------------------------------------

						include		".\JProtolo.inc"

; ---------------------------------------------------------------------------
;	Export symboles

						XDEF		GetPack
						XDEF		SendPack
						XDEF		SetPort
						XDEF		GetPort
					;	XDEF		CheckJaguar
						XDEF		ClearBuffer

; ---------------------------------------------------------------------------
;	Import symboles

						XREF		_InitB										;	board
						XREF		_GetB										;	board
						XREF		_SendB										;	board
						XREF		_StatB											;	board

						XREF		_InitS										;	serial
						XREF		_GetS										;	serial
						XREF		_SendS										;	serial
						XREF		_StatS											;	serial

						XREF		InitC										;	centronics
						XREF		GetC										;	centronics
						XREF		PutC										;	centronics
						XREF		StatC										;	centronics

; ---------------------------------------------------------------------------

O_INIT					equ			0
O_READ					equ			4
O_WRITE					equ			8
O_STATUS				equ			12

						TEXT

; ---------------------------------------------------------------------------
;		long cdecl GetPack(void *Buffer, int Expected)
;
;		Get a block from the input buffer which the lenght will be of 'Expected'
;		bytes.
;
;		return the count of bytes received or zero if no bytes.
; ---------------------------------------------------------------------------

OFF_BLOCKGET			equ			4+3*4
OFF_EXPECBLKGET			equ			8+3*4


GetPack:				movem.l		d1/a0-a1,-(sp)
						move.l		OFF_BLOCKGET(sp),a0

;	Compute address of current driver's GetByte

						lea			_tabdriver,a1								;	get tabdriver base address
						move.w		_currdriver,d0
						lsl.w		#4,d0										;	get base address of current driver
						addq.w		#O_READ,d0									;	offset to read function
						adda.w		d0,a1										;	add to base
						move.l		(a1),a1
						
;	Check if one byte is ready at least (If < 0 then no bytes)

						
						jsr			(a1)										;	call GetByte
						tst.l		d0											;	d0 < 0 ?
						blt.s		.EndWithError								;	yes => no byte

						move.b		d0,(a0)+									;	store the byte
						move.l		#1,d1										;	one byte received

;	Sending loop

.loop:					cmp.w		OFF_EXPECBLKGET(sp),d1						;	check the count expected
						bge.s		.EndNoError									;	too bytes received

						jsr			(a1)										;	call GetByte
						tst.l		d0
						blt.s		.EndNoError									;	no more bytes, finish

						move.b		d0,(a0)+									;	store the byte
						addq.l		#1,d1										;	one more byte received

						bra.s		.loop

.EndNoError:			
						move.l		d1,d0										;	return count of bytes
						movem.l		(sp)+,d1/a0-a1
						rts

.EndWithError:			movem.l		(sp)+,d1/a0-a1
						move.l		#0,d0										;	0 bytes received
						rts

; ---------------------------------------------------------------------------
;		long cdecl SendPack(void *Buffer, int Lenght)
;
;		Send the contains of the buffer to the output buffer. 'Lenght' bytes
;		will be send.
;
;		return TRUE if all bytes could been send or FALSE else.
; ---------------------------------------------------------------------------

OFF_BLOCKSEND			equ			4+3*4
OFF_LENBLKSEND			equ			8+3*4


SendPack:				movem.l		d1/a0-a1,-(sp)
						move.l		OFF_BLOCKSEND(sp),a0
						move.w		OFF_LENBLKSEND(sp),d1

;	Compute address of current driver's GetByte

						lea			_tabdriver,a1								;	get tabdriver base address
						move.w		_currdriver,d0
						lsl.w		#4,d0										;	get base address of current driver
						addq.w		#O_WRITE,d0									;	offset to read function
						adda.w		d0,a1										;	add to base
						move.l		(a1),a1

;	Sending loop

.loop:					tst.w		d1											;	while d1 > 0
						beq.s		.EndNoError									;	d1 = 0 terminate

						move.b		(a0)+,d0									;	get byte to send
						andi.l		#$FF,d0

						move.w		d0,-(sp)
						jsr			(a1)										;	call SendByte current driver
						addq.l		#2,sp										;	clear stack

						tst.w		d0
						beq.s		.EndWithError

						subi.w		#1,d1
						bra.s		.loop
						
.EndNoError:			movem.l		(sp)+,d1/a0-a1
						move.l		#TRUE,d0
						rts

.EndWithError:			movem.l		(sp)+,d1/a0-a1
						move.l		#FALSE,d0
						rts

; ---------------------------------------------------------------------------
;		int cdecl GetPort(void)

GetPort:				move.w		_currdriver,d0
						rts
; ---------------------------------------------------------------------------
;		void cdecl SetPort(int Port)

PORT					equ			4

SetPort:				move.w		PORT(sp),_currdriver
						movem.l		d0/a1,-(sp)

						;	Compute address of current driver's GetByte

						lea			_tabdriver,a1								;	get tabdriver base address
						move.w		_currdriver,d0
						lsl.w		#4,d0										;	get base address of current driver
						addi.w		#O_INIT,d0									;	offset to init function
						adda.w		d0,a1										;	add to base
						move.l		(a1),a1

						jsr			(a1)										;	call init function
						movem.l		(sp)+,d0/a1
						rts
; ---------------------------------------------------------------------------

ClearBuffer:			movem.l		d0/a0,-(sp)

;	Compute address of current driver's GetByte

						lea			_tabdriver,a0								;	get tabdriver base address
						move.w		_currdriver,d0
						lsl.w		#4,d0										;	get base address of current driver
						addq.w		#O_READ,d0									;	offset to read function
						adda.w		d0,a0										;	add to base
						move.l		(a0),a0
						
.Clear:					jsr			(a0)										;	call read function
						tst.l		d0											;	d0 < 0 ?
						bge.s		.Clear
						
						movem.l		(sp)+,d0/a0
						rts

; ---------------------------------------------------------------------------
						DATA
						EVEN

_currdriver:			dc.w		PORT_SERIAL									;	contents current port to use

_tabdriver:				dc.l		_InitB										;	board
						dc.l		_GetB										;	board
						dc.l		_SendB										;	board
						dc.l		_StatB										;	board

						dc.l		_InitS										;	serial
						dc.l		_GetS										;	serial
						dc.l		_SendS										;	serial
						dc.l		_StatS											;	serial

						dc.l		InitC										;	centronics
						dc.l		GetC										;	centronics
						dc.l		PutC										;	centronics
						dc.l		StatC										;	centronics
; ---------------------------------------------------------------------------
						END
