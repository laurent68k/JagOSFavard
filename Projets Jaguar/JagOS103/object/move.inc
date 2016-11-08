;	------------------------------------------------------------------------------
;	Revision 1.00 - Somes usefull fonctions's object
;
;	Date:		09 May 1997
;	Revision:	02 May 1998
;
;	Funship - Atari jaguar (c) France - 1997-1998
;
;	------------------------------------------------------------------------------

; ---------------------------------------------------------------------------
;		void cdecl Disable(void *ObjectAddress)
; ---------------------------------------------------------------------------
;		Change an image to an disable image.
; ---------------------------------------------------------------------------
;	Version 1.0 - 02 May 1998 - FunShip
;
;	Given:				4(sp) = Object Address to disable
;	Return:				Always TRUE
;
;	FONCTION:	Number 4
;
;	WARNINGS:	don't foget to restore a0, a1 from the stack
;       a0 = Address of this routine
;		a1 = Address of input parameters
;		d0 = will contain return value (as we want)
; ---------------------------------------------------------------------------

OBJECT_OFFSET				set			2
NEXT_IMAGE					set			1024									; next images in pixels

Disable:					move.l		OBJECT_OFFSET(a1),a0
							move.l		0(a0),d0
							andi.l		#$7FF,d0
							
							move.l		0(a0),d1
							move.l		#11,d2
							lsr.l		d2,d1
							lsl.l		#3,d1
							
							move.l		#NEXT_IMAGE*2,d2
							add.l		d2,d1
							lsl.l		#8,d1
							
							or.l		d1,d0
							move.l		d0,0(a0)

.End:						move.l		#TRUE,d0
							movem.l		(sp)+,d1-d7/a0-a6					
                        	RTE

; ---------------------------------------------------------------------------
;		void cdecl Enable(void *ObjectAddress)
; ---------------------------------------------------------------------------
;		Change an image to an disable image.
; ---------------------------------------------------------------------------
;	Version 1.0 - 02 May 1998 - FunShip
;
;	Given:				4(sp) = Object Address to disable
;	Return:				Always TRUE
;
;	FONCTION:	Number 22
;
;	WARNINGS:	don't foget to restore a0, a1 from the stack
;       a0 = Address of this routine
;		a1 = Address of input parameters
;		d0 = will contain return value (as we want)
; ---------------------------------------------------------------------------

OBJECT_OFFSET				set			2
NEXT_IMAGE					set			1024									; next images in pixels

Enable:					move.l		OBJECT_OFFSET(a1),a0
							move.l		0(a0),d0
							andi.l		#$7FF,d0
							
							move.l		0(a0),d1
							move.l		#11,d2
							lsr.l		d2,d1
							lsl.l		#3,d1
							
							move.l		#NEXT_IMAGE*2,d2
							sub.l		d2,d1
							lsl.l		#8,d1
							
							or.l		d1,d0
							move.l		d0,0(a0)

.End:						move.l		#TRUE,d0
							movem.l		(sp)+,d1-d7/a0-a6					
                        	RTE


; ---------------------------------------------------------------------------
;		void cdecl MoveObject(signed long X_Offset, signed long Y_Offset, void *Object)
; ---------------------------------------------------------------------------
;		This function Move an object with the two X,Y offset given in parameters.
; ---------------------------------------------------------------------------
;	Version 1.0 - 02 May 1998 - FunShip
;
;	Given:				10(sp)	=	Object Address to move
;						6(sp)	=	Y offset to add
;						2(sp)	=	X offset to add
;
;	Return:				Always TRUE
;
;	FONCTION:	Number 5
;
;	WARNINGS:	don't foget to restore a0, a1 from the stack
;       a0 = Address of this routine
;		a1 = Address of input parameters
;		d0 = will contain return value (as we want)
; ---------------------------------------------------------------------------

OBJECT_OFFSET				set			10
Y_OFFSET					set			6
X_OFFSET					set			2

MoveObject:					move.l		OBJECT_OFFSET(a1),a0				; Get object address
							move.l		Y_OFFSET(a1),d2						; Get Y offset
							move.l		X_OFFSET(a1),d3						; Get X offset
							
							move.l		12(a0),d0
							move.l		d0,d1
							andi.l		#$FFFFF000,d0
							andi.l		#$00000FFF,d1
							
							; inc Xposition							
							add.l		d3,d1								; Ajoute offset X 
							andi.l		#$00000FFF,d1
							or.l		d1,d0
							move.l		d0,12(a0)

							; inc Yposition
							move.l		4(a0),d0
							move.l		d0,d1
							andi.l		#$FFFFC000,d1

							lsr.l		#3,d0
							andi.l		#$7FF,d0

							add.l		d2,d0								; Ajoute offset Y 
							andi.l		#$7FF,d0
							lsl.l		#3,d0
							or.l		d0,d1
							move.l		d1,4(a0)
			
.end:						movem.l		(sp)+,d1-d7/a0-a6
                        	RTE
							
