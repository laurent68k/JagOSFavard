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

						XDEF		_LynxDrv

						TEXT
                        EVEN
      
; ---------------------------------------------------------------------------

BREAK 				equ			$80
CLRERROR			equ			$40
RXDIRQON			equ			$20
TXDIRQOFF			equ			$10
RXDLO				equ			$08
TXDLO				equ			$04
PARITY				equ			$02
NOPARITY			equ			$00
ODD					equ			$01
EVEN				equ			$00
RXDHI				equ			$00
TXDHI				equ			$00

RF					equ			7
TE					equ			8
PE					equ			9
FE					equ			10
OE					equ			11

;	------------------------------------------------------------------------------

;ASIDATA				equ		$F10030
;ASICTRL				equ		$F10032
;ASISTAT				equ		$F10032
;ASICLK				equ		$F10034

;	------------------------------------------------------------------------------
;	InitComLynx( void ):		speed rate for a RS232: xxx Bauds,8,no parity
;								Install Circular FIFO and enable Receive IT

UART_CONTROL		equ			RXDIRQON+RXDHI+TXDHI+NOPARITY

UART_9600			equ			172				;	26.593.900 / ( 9600 << 4 ) - 1
UART_600			equ			2769			;	26.593.900 / ( 600 << 4 ) - 1 

UART_SPEED			equ			UART_9600

InitComLynx:		movem.l		d0-d1,-(sp)

					move.l		#UART_SPEED,d0
					move.l		#UART_CONTROL,d1
					move.w		d0,ASICLK
					move.w		d1,ASICTRL
					move.w		d0,ASICLK
					move.w		d1,ASICTRL

					move.l		#FifoSerial,PtrRead
					move.l		#FifoSerial,PtrWrite
					move.w		#0,CountSerial
					
					
					move.w		#$10,$F10020			;	Enable ASI interrupt
					
					movem.l		(sp)+,d0-d1
					rts
						                  
; ---------------------------------------------------------------------------
;		long cdecl GetPack(void *Buffer, int Expected)
;
;		Get a block from the input buffer which the lenght will be of 'Expected'
;		bytes.
;
;		return the count of bytes received or zero if no bytes.
; ---------------------------------------------------------------------------

N_REGS_SAVED			set			2

OFF_BLOCKGET			set			4+N_REGS_SAVED*4
OFF_EXPECBLKGET			set			8+N_REGS_SAVED*4


LynxRDPack:				movem.l		d1/a0,-(sp)
						move.l		OFF_BLOCKGET(sp),a0

;	Check if one byte is ready at least (If < 0 then no bytes)

						jsr			LynxGetByte
						tst.l		d0											;	d0 < 0 ?
						blt.s		.EndWithError								;	yes => no byte
						
						move.b		d0,(a0)+									;	store the byte
						move.l		#1,d1										;	one byte received
;	Sending loop

.loop:					cmp.w		OFF_EXPECBLKGET(sp),d1						;	check the count expected
						bge.s		.EndNoError									;	too bytes received

						jsr			LynxGetByte			
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

N_REGS_SAVED			set			2

OFF_BLOCKSEND			set			4+N_REGS_SAVED*4
OFF_LENBLKSEND			set			8+N_REGS_SAVED*4


LynxWRPack:

						movem.l		d1/a0,-(sp)
						move.l		OFF_BLOCKSEND(sp),a0
						move.w		OFF_LENBLKSEND(sp),d1

;	Sending loop

.loop:					tst.w		d1											;	while d1 > 0
						beq.s		.EndNoError									;	d1 = 0 terminate

						move.b		(a0)+,d0									;	get byte to send
						andi.l		#$FF,d0

						move.w		d0,-(sp)									;	push byte to send
						jsr			LynxSendByte
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
;	Get a byte from the circular FIFO if one is available

LynxGetByte:		movem.l		a0,-(sp)
					
					tst.w		CountSerial
					beq.s		.NoByte

					move.l		PtrRead,a0
					move.b		(a0)+,d0
					andi.l		#$FF,d0
					move.l		a0,PtrRead
						
					sub.w		#1,CountSerial
					
					cmp.l		#FifoEnd,a0
					bne.s		.End
					
					move.l		#FifoSerial,PtrRead	

.End:				movem.l		(sp)+,a0
					rts

.NoByte:			move.l		#-1,d0
					movem.l		(sp)+,a0
					rts			

;	------------------------------------------------------------------------------
; send byte into the ASIDATA register

OFF_BYTE			set			4

LynxSendByte:
.wait:				move.w		ASISTAT,d0
					btst		#TE,d0
					beq.b		.wait

					move.w		OFF_BYTE(sp),ASIDATA
					rts

;	------------------------------------------------------------------------------
;	ITComLynx code

MAX					set			512

ITComLynx:			
					move.w		ASISTAT,d0
					andi.w		#$80,d0
					lsr.w		#7,d0
					tst.w		d0
					beq.w		.End

					move.w		#$1010,$F10020
		
					cmpi.w		#MAX,CountSerial
					beq.s		.Full
					
					move.w		ASIDATA,d0
					andi.l		#$FF,d0
					
					move.l		PtrWrite,a0
					move.b		d0,(a0)+
					move.l		a0,PtrWrite
					
					
					addi.w		#1,CountSerial
					

					cmp.l		#FifoEnd,a0
					bne.s		.End
					
					move.l		#FifoSerial,PtrWrite
.Full:								
.End:				
					RTS

; ---------------------------------------------------------------------------
;	Board driver descriptor 

						DATA
						EVEN
						
_LynxDrv:				dc.l		InitComLynx
						dc.l		LynxRDPack
						dc.l		LynxWRPack

