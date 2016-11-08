;	------------------------------------------------------------------------------
;	OBJCPU Revision 2.00 - JAGUAR's Processor object routines.
;
;	Date:		06 october 1997
;	Revision:	20 April 1997
;
;	Funship - Atari jaguar (c) France - 1997
;	------------------------------------------------------------------------------


;	------------------------------------------------------------------------------
;	long cdecl InitOP(void *DisplayableList_Ptr, void *ReferenceList_Ptr)
;	------------------------------------------------------------------------------
;	Copy the reference list and start the Object Processor with the displayable
;	objects list.
;	------------------------------------------------------------------------------
;	Parameters:		6(sp)	=	Reference Object List
;					2(sp)	=	Displayable object list
;	Returns:		Nothing
;
;	Register Usage:	All Registers Preserved without d0.
;	Externals:		VarVde, DisplayableList_Ptr, vblank
;
;	FONCTION:	Number 8
;
;	WARNINGS:	don't foget to restore d1-d7/a0-a6 from the stack
;       a0 = Address of this routine
;		a1 = Address of input parameters
;		d0 = will contain return value (as we want)
; ---------------------------------------------------------------------------

DISPLAY_OFFSET				set			2
REFERENCE_OFFSET			set			6

InitOP:			
							
; Copy the reference list to the displayable object list

							move.l		DISPLAY_OFFSET(a1),a0
							move.l		a0,d2
							move.l		REFERENCE_OFFSET(a1),a1

;.cpylp:						move.l		(a1),(a0)+						; Copy Object list to display space
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

; Initialize VI for interupts

.endcpy:					move.w  	VarVde,d0						; thats the downside border of the screen
							btst.l		#0,d0							; check for odd
							bne.s		.uneq							; in odd thats fine
							subq.w  	#1,d0							; else use one line before as end
.uneq:	
							move.w  	d0,VI

							swap		d2								; THIS SWAP IS REQUIRED (Register is designed English)
							move.l		d2,OLP							; Point to finished Object List

							move.w		#INT1_VALUE,INT1
							move.w		#0,INT2							; CPU Interrupt Resume Register
							move.l		#vblank,$100.w					; Install our own VBLANK Interrupt
						
.End:						movem.l		(sp)+,d1-d7/a0-a6
							RTE

;	------------------------------------------------------------------------------
;	long WAITSYNC(void)   
;	------------------------------------------------------------------------------
;	Wait the VBL interrupt and then exit of this call.
;	------------------------------------------------------------------------------
;
;	Parameters:			Nothing
;	Returns:			Nothing
;	Register Usage:		All Registers Preserved whitout d0
;	Externals:			Use external FrameCount
;
;	FONCTION:	Number 9
;
;	WARNINGS:	don't foget to restore d1-d7/a0-a6 from the stack
;       a0 = Address of this routine
;		a1 = Address of input parameters
;		d0 = will contain return value (as we want)
; ---------------------------------------------------------------------------

WaitVBL:					move.l		FrameCount,d0

.wait:						cmp.l		FrameCount,d0
							beq			.wait
					
.End:						movem.l		(sp)+,d1-d7/a0-a6
							RTE

							
