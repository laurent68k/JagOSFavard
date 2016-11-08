;	------------------------------------------------------------------------------
;	VBLANK interrupt Revision 2.00 - JAGUAR's Processor object routines.
;
;	Date:		06 october 1997
;	Revision:	01 June 1998
;
;	Funship - Atari jaguar (c) France - 1997
;	------------------------------------------------------------------------------


;	------------------------------------------------------------------------------
;	VBLANK   Vertical Sync Interrupt Handler
;
;	Given:				Nothing
;	Returns:			Object List restored for next frame
;	Register Usage:		All Registers Preserved
;	Externals:			FrameCount, SysCounter1, SysCounter2
;						DisplayBitMapPtr, DisplayableList_Ptr, ReferenceList
;

vblank:						movem.l		d0-d1/a0-a1,-(sp)
							
							btst.b		#0,$00F000E1
							beq.s		.EndIT

							move.l		DisplayableList_Ptr,a0
							move.l		ReferenceList_Ptr,a1

;.cpylp:					move.l		(a1),(a0)+						; Copy Object list to display space
;							
;							move.w		O_TYPE(a1),d0					; get current type object			
;							andi.l		#$7,d0							
;							cmpi.w		#STOP_OBJECT,d0					; STOP object ?
;							beq.s		.endcpy
;							
;							addq.l		#4,a1							
;							bra.s		.cpylp
					
;	---- New copy
							move.l		ListObjectLenght,d0
							lsr.l		#2,d0
.copy:						move.l		(a1)+,(a0)+
							dbra		d0,.copy
							
;	-------------
					
;	Video frame counter
.endcpy:					addq.l		#1,FrameCount					; Counter video frame

;	For general purpose usage

							addq.l		#1,SysCounter1					; System Counter #1
							addq.l		#1,SysCounter2					; System Counter #2

.EndIT:						jsr			ITComLynx

							move.w		#INT1_VALUE,INT1				; CPU Interrupt Control Register
							move.w		#0,INT2							; CPU Interrupt Resume Register
														
							movem.l		(sp)+,d0-d1/a0-a1
							rte
							
