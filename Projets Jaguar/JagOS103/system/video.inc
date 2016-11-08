;	------------------------------------------------------------------------------
;	VIDEO.INC - Revision 3.00 - JAGUAR's Video routines.
;
;	Date:		06 october 1997
;	Revision:	23 March 1999
;
;	Funship - Atari jaguar (c) France, 1997-1999
;	------------------------------------------------------------------------------

;	------------------------------------------------------------------------------
;	INITVIDEO:		Video Initialization: PAL or NTSC display ?
;					BIG Endian and video processor off.
;	------------------------------------------------------------------------------
;	Version 1.0 - 09 December 1997 - FunShip
;
;	Given:					4(sp) = PAL Table
;							8(sp) = NSC Table
;	Returns:				Refrence list ready to been copied
;	Register Usage:			All Registers Preserved
;	Externals:				Nothings
;	------------------------------------------------------------------------------

OFS_PAL						equ			8+4
OFS_NTSC					equ			8+8
									
InitVideo:					movem.l		d0/a0,-(sp)

							move.l		OFS_PAL(sp),a0					; a0 <= PAL Table values
							move.w		CONFIG,d0					; get current mode
							
							btst		#PALFLAG,d0					; if flag set you have a NTSC else it's a PAL Jag
							beq.s		PAL_MODE
							move.l		OFS_NTSC(sp),a0				; a0 <= NTSC Table values

PAL_MODE:    				move.w		(a0)+,CLK2					; Jerry

							move.w      (a0)+,HP					; and video registers
							move.w      (a0)+,HBB
							move.w      (a0)+,HBE	
							move.w      (a0)+,HS
							move.w      (a0)+,HVS
							move.w      (a0)+,HDB1
							move.w      (a0)+,HDB2
							move.w      (a0)+,HDE
							move.w      (a0)+,VP
							move.w      (a0)+,VBB
							move.w      (a0)+,VBE
							move.w      (a0)+,VS
							move.w      (a0)+,VDB

							move.w      (a0),VDE		
							move.w		#$ffff,$0048(a0)   			; due to a bug in Jaguar Console !
							move.w		(a0)+,VarVde

							move.w      (a0)+,VEB
							move.w      (a0)+,VEE
							move.w      (a0)+,HEQ

;	Somes initialization: Big endian and video off

							move.l      #BIG_ENDIAN,$F1A10C			; big endian value
							move.l      #BIG_ENDIAN,$F0210C			; big endian value
							clr.w		VMODE
							move.w		#QUIET,sr

							movem.l		(sp)+,d0/a0
							rts
														