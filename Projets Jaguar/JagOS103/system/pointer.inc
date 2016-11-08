							TEXT
							EVEN

;	------------------------------------------------------------------------------
;	void cdecl SetPointer( int CursorCode, void *UserDefined, int Lenght)
;	------------------------------------------------------------------------------
;	Version 1.2 - 07 April 1998 - FunShip
;				- 07 Jully 1998
;
;	Given:					8(sp).l	=	User defined cursor lenght (datas are packed)
;							4(sp).l	=	User defined cursor start address	
;							2(sp).w =	Mouse cursor code
;
;	FONCTION:	Number 10
;
;	This function change the mouse cursor.
;
;	WARNINGS:	don't foget to restore d1-d7/a0-a6 from the stack
;       a0 = Address of this routine
;		a1 = Address of input parameters
;		d0 = will contain return value (as we want)
; ---------------------------------------------------------------------------

OFF_USER_LENGHT				set			8
OFF_USER_DEF				set			4
OFF_CODE_CURSOR				set			2

SetCursor:					move.w		OFF_CODE_CURSOR(a1),d1					; get code cursor
							
							move.l		#ArrowBegin,a0							; assume it will be a arrow cursor
							move.l		#ArrowEnd-ArrowBegin,d0					; packed datas's size
							tst.w		d1										; test if d1 = 0
							beq.s		.Ready									; if = 0 then this a arrow required

							cmpi.w		#USER_MOUSE,d1							; else check if it's a user defined cursor
							bne.s		.Busy									; if no then it's the busy bee cursor

							move.l		OFF_USER_DEF(a1),a0					; else take the start address
							clr.l			d0
							move.w		OFF_USER_LENGHT(a1),d0					; and the lenght of packed datas
							bra.s		.Ready									

.Busy:						move.l		#BusyBegin,a0
							move.l		#BusyEnd-BusyBegin,d0					; packed datas's size
							
.Ready:						move.l		a0,-(sp)								; Source packed bitmap
							move.l		d0,-(sp)								; packed datas's size
							move.l		#DISPBUF8,-(sp)							; Target to unpack
							move.w		#UNPACK,-(sp)
							trap		#SYSTEM
							add.l		#14,sp

.End:						movem.l		(sp)+,d1-d7/a0-a6
							RTE
							
