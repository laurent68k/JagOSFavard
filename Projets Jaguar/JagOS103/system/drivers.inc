							TEXT
							EVEN

;	------------------------------------------------------------------------------
;	long cdecl SetDriver( int NumDev )
;	------------------------------------------------------------------------------
;	Version 1.0 - 05 april 1999 - FunShip
;
;	Given:			2(sp).w	= Number of device.
;					0(sp).w	= Number of this function (25)
;	
;	FONCTION:	Number 25o
;
;	This function set a device as main communication channel. return 1 if
;	device changed or 0 if not (Device always set).
;
;	WARNINGS:	don't forget to restore d1-d7/a0-a6 from the stack
;       a0 = Address of this routine
;		a1 = Address of input parameters
;		d0 = will contain return value (as we want)
;
;	GLOABLS:	_CurrMainDriver =	Current device number used
;				_LockJoyPad2	=	Lock Joypad port #2 or not.
;				_KernDrv		=	System devices drivers
;				_CurrRead		=	Current device Read function
;				_CurrWrite		=	Current device Write function
; ---------------------------------------------------------------------------

O_NUMDEV					set			2

SetDriver:					move.w		O_NUMDEV(a1),d0					;	get device number
							andi.l		#$F,d0							;	clear uppers bits
							
							cmp.w		_CurrMainDriver,d0
							beq.s		.Error
		
							move.w		d0,_CurrMainDriver
							
							cmpi.w		#D_CENTR,d0
							bne.s		.Next
							
							move.w		#1,_LockJoyPad2					;	lock joypad #2
							lsl.l		#2,d0
							bra.s		.Continue
													
.Next:						clr.w		_LockJoyPad2					;	unlock joypad #2
							lsl.l		#2,d0							;	d0 <= Number * 4
							
.Continue:					lea			_KernDrv,a0						;	kernel devices
							add.l		d0,a0
							move.l		(a0),a0							;	driver address
							
							move.l		4(a0),_CurrRead					;	set current read block
							move.l		8(a0),_CurrWrite				;	set current write block
							move.l		(a0),a0							;	get init() address
							jsr			(a0)							;	initialize...
														
.End:						movem.l		(sp)+,d1-d7/a0-a6
							move.l		#TRUE,d0														
							RTE
							
.Error:						movem.l		(sp)+,d1-d7/a0-a6
							move.l		#FALSE,d0														
							RTE						
; ---------------------------------------------------------------------------
;	"Drivers" Structure

_KernDrv:					dc.l		_BoardDrv
							dc.l		_LynxDrv
							dc.l		_CentrDrv
							
