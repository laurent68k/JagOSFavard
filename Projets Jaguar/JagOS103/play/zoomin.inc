; ---------------------------------------------------------------------------
;	ZoomLogoJagIn()
; ---------------------------------------------------------------------------
;	ZOOM:	Make a Zoom IN on the jaguar logo object and change its X position
; ---------------------------------------------------------------------------
;	Version 1.0 - 02 May 1998 - FunShip
;
;	Given:				Nothing
;	Return:				Nothing
;
; ---------------------------------------------------------------------------

ZoomLogoJagIn:				movem.l		d0-d1/a0,-(sp)

							movea.l		ReferenceList_Ptr,a0
							adda.l		#16+16+16+4,a0
							
							move.l		0(a0),d0
							move.l		d0,d1
							andi.l		#$FFFFFF00,d0
							andi.l		#$FF,d1
							
							; HSCALE						
							addq.l		#1,d1						; Ajoute offset X 
							andi.l		#$FF,d1
							or.l		d1,d0
							move.l		d0,0(a0)

							; VSACLE
							move.l		0(a0),d0
							move.l		d0,d1
							andi.l		#$FFFF00FF,d1

							andi.l		#$FF00,d0
							lsr.l		#8,d0
							
							addq.l		#1,d0						; Ajoute offset Y 
							andi.l		#$FF,d0
							lsl.l		#8,d0
							or.l		d0,d1
							move.l		d1,0(a0)

;	Move the Jaguar logo to get always a center position

							movea.l		ReferenceList_Ptr,a0
							adda.l		#16+16,a0

							move.l		12(a0),d0
							move.l		d0,d1
							andi.l		#$FFFFF000,d0
							andi.l		#$00000FFF,d1
							
							; inc Xposition							
							sub.w		#2,d1
							or.l		d1,d0
							move.l		d0,12(a0)

							movem.l		(sp)+,d0-d1/a0
							rts

