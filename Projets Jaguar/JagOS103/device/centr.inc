;	------------------------------------------------------------------------------
;	Project JAGUAR:	Basic Input/Ouput Routines. No debug possible, only for kernel
;
;	File:		TramLow.S
;	Author:		FAVARD Laurent
;	Date:		05 April 1999
;	Release:	05 April 1999
;	Language:	HISOFT DEVPAC 3-680x0
;
;	Funship - Atari Jaguar France / Copyright 1999
;	------------------------------------------------------------------------------

;	Export symboles

						XDEF		_CentrDrv

						TEXT
                        EVEN
           
;	------------------------------------------------------------------------------
;
;
;		Joypad #2 initialisazition. Set STROBE High
;	------------------------------------------------------------------------------

J7_0_OUT	equ		$8000
J7_HIGH		equ		$81F0		;$80
J7_LOW		equ		$8100
B2			equ		2
B3			equ		3

JoypadInit:				move.l		a0,-(sp)
						lea			$F14000,a0
						move.w		#J7_HIGH,(a0)
						move.l		(sp)+,a0
						rts
             
; ---------------------------------------------------------------------------
;		long cdecl GetPack(void *Buffer, int Expected)
;
;		Get a block from the input buffer which the lenght will be of 'Expected'
;		bytes.
;
;		return the count of bytes received or zero if no bytes.
; ---------------------------------------------------------------------------

N_REGS					set			2

OFF_BLOCKGET			set			4+N_REGS*4
OFF_EXPECBLKGET			set			8+N_REGS*4


JoypadRDPack:			movem.l		d1/a0,-(sp)
						move.l		OFF_BLOCKGET(sp),a0

;	Check if one byte is ready at least (If < 0 then no bytes)

						jsr			JPGet
						tst.l		d0											;	d0 < 0 ?
						blt.s		.EndWithError								;	yes => no byte
						
						move.b		d0,(a0)+									;	store the byte
						move.l		#1,d1										;	one byte received
;	Sending loop

.loop:					cmp.w		OFF_EXPECBLKGET(sp),d1						;	check the count expected
						bge.s		.EndNoError									;	too bytes received

						jsr			JPGet
						tst.l		d0
						blt.s		.EndNoError									;	no more bytes, finish

						move.b		d0,(a0)+									;	store the byte
						addq.l		#1,d1										;	one more byte received

						bra.s		.loop

.EndNoError:			move.l		d1,d0										;	return count of bytes
						movem.l		(sp)+,d1/a0
						rts

.EndWithError:			movem.l		(sp)+,d1/a0
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

N_REGS					set			2

OFF_BLOCKSEND			set			4+N_REGS*4
OFF_LENBLKSEND			set			8+N_REGS*4

JoypadWRPack:

						movem.l		d1/a0,-(sp)
						move.l		OFF_BLOCKSEND(sp),a0
						move.w		OFF_LENBLKSEND(sp),d1

;	Sending loop

.loop:					tst.w		d1											;	while d1 > 0
						beq.s		.EndNoError									;	d1 = 0 terminate

						move.b		(a0)+,d0									;	get byte to send
						andi.l		#$FF,d0

						move.w		d0,-(sp)									;	push byte to send
						jsr			JPSend
						addq.l		#2,sp										;	clear stack

						tst.w		d0
						beq.s		.EndWithError

						subi.w		#1,d1
						bra.s		.loop

.EndNoError:			movem.l		(sp)+,d1/a0
						move.l		#TRUE,d0
						rts

.EndWithError:			movem.l		(sp)+,d1/a0
						move.l		#FALSE,d0
						rts

;	------------------------------------------------------------------------------
;
;
;		Byte is send with 3 bits blocks, with the LSBs to the MSBs.
;	------------------------------------------------------------------------------

OFF_BYTE	set		6*4

JPSend:	movem.l		d1-d3/a0-a1,-(sp)

		move.w		OFF_BYTE(sp),d0
		andi.w		#$FF,d0
		move.w		#2,d1								;	3 loop
		lea			$F14000,a0
		lea			3(a0),a1
		move.w		#J7_HIGH,d3							;	set BUSY line HIGH
		
.loop:	move.w		d3,(a0)

		move.w		d0,d2								;	take data to send
		andi.w		#$07,d2								;	Keep only 3 LSBs
		lsl.w		#4,d2								;	move to J4...J6 position
		ori.w		#J7_LOW,d2							;	ORed with BUSY line LOW
		move.w		d2,(a0)								;	put data and strobe

.wait1:	btst		#B3,(a1)							;	wait until STROBE line HIGH
;		beq			.wait1	
		bne			.wait1	
		
		move.w		d3,(a0)								;	set BUSY line high		

.wait2:	btst		#B3,(a1)							;	wait until STROBE line LOW
;		bne			.wait2
		beq			.wait2
			
		lsr.w		#3,d0								;	next 3 bits to send
		dbra		d1,.loop							;	go to main loop
				
.End:	move.w		d3,(a0)								
		movem.l		(sp)+,d1-d3/a0-a1
		move.l		#1,d0
		rts
		
;	------------------------------------------------------------------------------


JPGet:	movem.l		d1-d4/a0-a1,-(sp)

		move.w		#1,d1								;	2 loop
		lea			$F14000,a0
		lea			3(a0),a1
		move.w		#12,d4								;	initial left shift
		clr.l		d3									;	STROBE Jag High
		clr.l		d0									;	contains byte to get from host

		btst		#B3,(a1)							;	STROBE low => no data
;		beq.s		.NoData		
		bne.s		.NoData		

.loop:	btst		#B3,(a1)							;	STROBE low => wait STROBE HIGH
;		beq.s		.loop
		bne.s		.loop
		
		move.w		(a0),d2								;	get data from J15-J12
		andi.w		#$F000,d2
		lsr.w		d4,d2								;	move to d0-d3 position (and d4-d7 for the next one)
		or.w		d2,d0

		move.w		#J7_LOW,(a0)						;	set BUSY line LOW		
		
.wait1:	btst		#B3,(a1)							;	wait until STROBE is LOW
;		bne.s		.wait1
		beq.s		.wait1

		move.w		#J7_HIGH,(a0)						;	set BUSY line HIGH
		
		subi.w		#4,d4
		dbra		d1,.loop
		
.Ok		movem.l		(sp)+,d1-d4/a0-a1
		rts

.NoData:movem.l		(sp)+,d1-d4/a0-a1
		move.l		#-1,d0
		rts

;	------------------------------------------------------------------------------
;	Joypad 2/Centronics descriptor 

_CentrDrv:				dc.l		JoypadInit
						dc.l		JoypadRDPack
						dc.l		JoypadWRPack

