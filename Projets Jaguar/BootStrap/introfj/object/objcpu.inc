;	------------------------------------------------------------------------------
;	OBJCPU Revision 2.00 - JAGUAR's Processor object routines.
;
;	Date:		06 october 1997
;	Revision:	17 November 1997
;
;	Funship - Atari jaguar (c) France - 1997
;	------------------------------------------------------------------------------


;	------------------------------------------------------------------------------
;	VBLANK   Vertical Sync Interrupt Handler
;
;	Given:				Nothing
;	Returns:			Object List restored for next frame
;	Register Usage:		All Registers Preserved
;	Externals:			FrameCount, DisplayBitMapPtr, DisplayableList_Ptr, ReferenceList
;

vblank:
							movem.l		d0-d1/a0-a1,-(sp)
							
							btst.b		#0,$00F000E1
							beq.s		.endit

							move.l		#OBJSIZE-1,d1
							move.l		DisplayableList_Ptr,a0
							lea			ReferenceList,a1

.itcpy:						move.l		(a1)+,(a0)+						; Copy Object list to display space
							dbra		d1,.itcpy
										
							addq.l		#1,FrameCount					; Count a video frame
							
.endit:						move.w		#INT1_VALUE,INT1				; CPU Interrupt Control Register
							move.w		#0,INT2							; CPU Interrupt Resume Register
														
							movem.l		(sp)+,d0-d1/a0-a1
							rte


;	------------------------------------------------------------------------------
;	INITOP:   Start the Object processor.
;
;	Parameters:		Nothing
;	Returns:		Nothing
;	Register Usage:	All Registers Preserved
;	Externals:		VarVde, DisplayableList_Ptr, vblank

InitOP:						movem.l		d0/a0-a1,-(sp)

; Copy the reference list to the displayable object list

							move.l		#OBJSIZE-1,d1
							
							move.l		DisplayableList_Ptr,a0
							lea			ReferenceList,a1

.cpylp:						move.l		(a1)+,(a0)+						; Copy Object list to display space
							dbra		d1,.cpylp

; Initialize VI for interupts

							move.w  	VarVde,d0						; thats the downside border of the screen
							btst.l		#0,d0							; check for odd
							bne.s		.uneq							; in odd thats fine
							subq.w  	#1,d0							; else use one line before as end
.uneq:	
							move.w  	d0,VI

							move.l		DisplayableList_Ptr,d0
							swap		d0								; THIS SWAP IS REQUIRED (Register is designed English)
							move.l		d0,OLP							; Point to finished Object List

							move.w		#INT1_VALUE,INT1
							move.w		#0,INT2							; CPU Interrupt Resume Register
							move.l		#vblank,$100.w					; Install our own VBLANK Interrupt
						

							move.w		#$2000,sr						; Enable Interrupts
							movem.l		(sp)+,d0/a0-a1
							rts

;	------------------------------------------------------------------------------
;
;	WAITSYNC   Wait for VSYNC Interrupt
;
;	Parameters:			Nothing
;	Returns:			Nothing
;	Register Usage:		All Registers Preserved
;	Externals:			FrameCount

WaitSync:					move.l		d0,-(sp)					
							move.l		FrameCount,d0

.wait:						cmp.l		FrameCount,d0
							beq			.wait
					
							move.l		(sp)+,d0
							rts

;	------------------------------------------------------------------------------
;
;	COPYBITMAP:   Copy the source bitmap to the destination address display buffer.
;
;	This is very usefull to copy the bitmap to an aligned phrase address given in parameter.
;	Parameters:		Destination, Source and Length
;	Returns:		Nothing
;	Register Usage:	All Registers Preserved
;	Externals:		BitMapROM, BitMapCountROM

CopyBitMap:					movem.l		d0/a0-a1,-(sp)

							move.l		16(sp),d0						; length
							move.l		20(sp),a1						; source
							move.l		24(sp),a0						; target

							lsr.l		#2,d0							; bitmap's size in long
.loop						move.l		(a1)+,(a0)+		
							dbra		d0,.loop						; do while size not equal 0

							movem.l		(sp)+,d0/a0-a1
							rts

							
