							TEXT
							EVEN

;	------------------------------------------------------------------------------
;	long cdecl WindGet( int Windhandle, int Command, int *x, int *y, int *w, int *h)
;	------------------------------------------------------------------------------
;	Version 1.0 - 09 July 1998 - FunShip
;
;	Given:					
;							18(sp).L	=	Address of h
;							14(sp).L	=	Address of w
;							10(sp).L	=	Address of y
;							6(sp).L		=	Address of x
;							4(sp).L		=	Command
;							2(sp).l		=	Window handle
;
;	return:					d0.l = True, False, or address to display buffer
;
;	FONCTION:	Number 21
;
;	This function get somes windows informations depending of the command given in parameters.
;
;	WARNINGS:	don't forget to restore d1-d7/a0-a6 from the stack
;       a0 = Address of this routine
;		a1 = Address of input parameters
;		d0 = will contain return value (as we want)
; ---------------------------------------------------------------------------

OFF_H						set			18
OFF_W						set			14
OFF_Y						set			10
OFF_X						set			6
OFF_CMD						set			4
OFF_HANDLE					set			2

WITDH						set			128
N_LINES						set			10

WindGet:					move.w		OFF_HANDLE(a1),d0						; Get the command word
							cmpi.w		#W_CONSOLE,d0
							bne			.EndWithError

							move.w		OFF_CMD(a1),d0							; Get the command word

;	Address of the window object

							move.l		ReferenceList_Ptr,a0					; start list 
							add.l		#8*2,a0									; offset to window object

;	What's command required ?

							cmpi.w		#W_CURRXYWH,d0							; Client window area 
							beq.s		.CurrXYXH
							
							cmpi.w		#W_WORKXYWH,d0							; Window window area 
							beq.s		.WorkXYXH

							cmpi.w		#W_CURRBUFF,d0							; Client RAM buffer address 
							beq.s		.CurrBuff

							cmpi.w		#W_WORKBUFF,d0							; Window RAM buffer address 
							beq.s		.WorkBuff

							cmpi.w		#W_ADROBJ,d0							; Window address object
							beq.s		.AdrObj

							bra.s		.EndWithError
							
;	Compute the X window position

							move.l		12(a0),d0
							andi.l		#$00000FFF,d0							; d0 = X position
							
;	Compute the Y window position

							move.l		4(a0),d1
							lsr.l		#3,d1
							andi.l		#$7FF,d1								; d0 = Y position

.CurrXYXH:					move.l		d0,OFF_X(a1)
							move.l		d1,OFF_Y(a1)
							move.l		#128,OFF_W(a1)
							move.l		#64,OFF_H(a1)

							move.l		#TRUE,d0
							bra.s		.End

.WorkXYXH:					move.l		d0,OFF_X(a1)

							add.l		#N_LINES,d1
							move.l		d1,OFF_Y(a1)

							move.l		#128,OFF_W(a1)
							move.l		#64-N_LINES,OFF_H(a1)

							move.l		#TRUE,d0
							bra.s		.End

.CurrBuff:					move.l		#DISPBUF3,d0
							bra.s		.End

.WorkBuff:					move.l		#DISPBUF3,d0							; Address of buffer
							add.l		#WITDH*2*N_LINES,d0						; plus number of lines reserved for the "mover"
							bra.s		.End

.AdrObj:					move.l		a0,d0

.End:						movem.l		(sp)+,d1-d7/a0-a6
							RTE
							

.EndWithError:				move.l		#FALSE,d0
							movem.l		(sp)+,d1-d7/a0-a6
							RTE
							