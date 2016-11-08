;	------------------------------------------------------------------------------
;	Project JAGUAR:	Basic Input/Ouput Routines. No debug possible, only for kernel
;
;	File:		TramLow.S
;	Author:		FAVARD Laurent
;	Date:		20 January 1998
;	Release:	17 June 1998
;	Language:	HISOFT DEVPAC 3-680x0
;
;	Funship - Atari Jaguar France / Copyright 1997-1998
;	------------------------------------------------------------------------------

;	Export symboles

						XDEF		GetPack
						XDEF		SendPack
						XDEF		GetByte
						XDEF		SendByte

						TEXT
                        EVEN
                        
; ---------------------------------------------------------------------------
;		long cdecl GetPack(void *Buffer, int Expected)
;
;		Get a block from the input buffer which the lenght will be of 'Expected'
;		bytes.
;
;		return the count of bytes received or zero if no bytes.
; ---------------------------------------------------------------------------

OFF_BLOCKGET			equ			4+2*4
OFF_EXPECBLKGET			equ			8+2*4


GetPack:				movem.l		d1/a0,-(sp)
						move.l		OFF_BLOCKGET(sp),a0

;	Check if one byte is ready at least (If < 0 then no bytes)

;	-----CHECK WHAT PORT TO USE: Serial or Cartridge
						tst.w		_UseComLynx
						beq.s		.UseCarte

						jsr			LynxGetByte
						bra.s		.Next

.UseCarte:				jsr			GetByte			
.Next:
;	-----END

						tst.l		d0											;	d0 < 0 ?
						blt.s		.EndWithError								;	yes => no byte
						
						move.b		d0,(a0)+									;	store the byte
						move.l		#1,d1										;	one byte received
;	Sending loop

.loop:					cmp.w		OFF_EXPECBLKGET(sp),d1						;	check the count expected
						bge.s		.EndNoError									;	too bytes received

;	-----CHECK WHAT PORT TO USE: Serial or Cartridge
						tst.w		_UseComLynx
						beq.s		.UseCarte2

						jsr			LynxGetByte
						bra.s		.Next2

.UseCarte2:				jsr			GetByte			
.Next2:
;	-----END

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

OFF_BLOCKSEND			equ			4+2*4
OFF_LENBLKSEND			equ			8+2*4


SendPack:

						movem.l		d1/a0,-(sp)
						move.l		OFF_BLOCKSEND(sp),a0
						move.w		OFF_LENBLKSEND(sp),d1

;	Sending loop

.loop:					tst.w		d1											;	while d1 > 0
						beq.s		.EndNoError									;	d1 = 0 terminate

						move.b		(a0)+,d0									;	get byte to send
						andi.l		#$FF,d0

						move.w		d0,-(sp)									;	push byte to send

;	-----CHECK WHAT PORT TO USE: Serial or Cartridge
						tst.w		_UseComLynx
						beq.s		.UseCarte

						jsr			LynxSendByte
						bra.s		.Next

.UseCarte:				jsr			SendByte			
.Next:
;	-----END

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

; ---------------------------------------------------------------------------
;		long cdecl GetByte(void)
;
;		Get a byte from the input buffer if a byte is ready.
;
;		Return the byte read into d0 or -1 if no byte.
; ---------------------------------------------------------------------------

GetByte:
                        btst.b		#RBUF_EMPTY,StatusBuffer	; tant que vide..
						beq.s		.NoByte
                        move.b		ReceiveBuffer,d0
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

OFF_SBYTE				equ			4+5*4
OFF_SBYTE2				equ			4

SendByte:

                        move.l		a0,-(sp)

.Wait:                  btst.b		#SBUF_FULL,StatusBuffer						; Attendre que vide
					   	beq.s		.Wait

					  	move.l		#SendBuffer,a0
                        move.w		OFF_SBYTE2(sp),d0							; caractere a envoyer
					  	move.b		(a0,d0.w),d0								; envoyer...

                        move.l		(sp)+,a0
                        move.l		#TRUE,d0
						rts

; ---------------------------------------------------------------------------

