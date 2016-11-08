; ---------------------------------------------------------------------------
;	ZoomLogoJagOut()
; ---------------------------------------------------------------------------
;	ZOOM:	Move the jaguar logo object to the left bottom of screen
;			left top of screen.
; ---------------------------------------------------------------------------
;	Version 1.0 - 02 May 1998 - FunShip
;
;	Given:				Nothing
;	Return:				Nothing
;
; ---------------------------------------------------------------------------

ZoomLogoJagOut:				movem.l		d0-d1/a0,-(sp)

;	Move the Jaguar logo

							movea.l		ReferenceList_Ptr,a0
							adda.l		#16+16,a0

							move.l		12(a0),d0
							move.l		d0,d1
							andi.l		#$FFFFF000,d0
							andi.l		#$00000FFF,d1
							
							; Modify Xposition							
							sub.w		#9,d1
							or.l		d1,d0
							move.l		d0,12(a0)

							; Modify Yposition
							move.l		4(a0),d0
							move.l		d0,d1
							andi.l		#$FFFFC000,d1

							lsr.l		#3,d0
							andi.l		#$7FF,d0

							sub.l		#12,d0								; Ajoute offset Y 
							andi.l		#$7FF,d0
							lsl.l		#3,d0
							or.l		d0,d1
							move.l		d1,4(a0)

							movem.l		(sp)+,d0-d1/a0
							rts

