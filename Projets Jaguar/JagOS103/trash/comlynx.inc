;	------------------------------------------------------------------------------
;	Revision 1.00 - Somes sub-routines to drive the ComLynx port.
;
;	Date:		13 May 1998
;	Revision:	27 May 1998
;
;	Funship - Atari jaguar (c) France - 1997-98
;
;	------------------------------------------------------------------------------

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

ASIDATA				equ		$F10030
ASICTRL				equ		$F10032
ASISTAT				equ		$F10032
ASICLK				equ		$F10034

;	------------------------------------------------------------------------------
;	InitComLynx( void ):		speed rate for a RS232: xxx Bauds,8,no parity
;								Install Circular FIFO and enable Receive IT

UART_CONTROL		equ			RXDIRQON+RXDHI+TXDHI+NOPARITY

UART_9600			equ			172				;	26.593.900 / ( 9600 << 4 ) - 1
UART_600			equ			2769			;	26.593.900 / ( 600 << 4 ) - 1 

UART_SPEED			equ			UART_9600

InitComLynx:		move.l		d1,-(sp)

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
					
					move.l		(sp)+,d1
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
;	Send a string to the ComLynx port

LynxSendString:		movem.l		d0/a0,-(sp)

					move.l		12(sp),a0
					clr.l		d0
				
.Send:				move.w		ASISTAT,d0
					btst		#TE,d0
					beq.b		.Send

					move.b		(a0)+,d0
					move.w		d0,ASIDATA

					tst.b		d0
					bne.s		.Send

					movem.l		(sp)+,d0/a0
					rts
												
;	------------------------------------------------------------------------------
;	read a byte from ASIDATA register, return it in D0

_LynxGetByte:
.wait:				move.w		ASISTAT,d0
					andi.w		#$80,d0
					lsr.w		#7,d0
					tst.w		d0
					beq.w		.nobyte

					move.w		ASIDATA,d0
					andi.l		#$FF,d0
					rts

.nobyte:			move.l		#-1,d0
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
