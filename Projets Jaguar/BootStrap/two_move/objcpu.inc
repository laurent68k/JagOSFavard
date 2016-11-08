;	------------------------------------------------------------------------------
;	OBJCPU Revision 0.00 - JAGUAR's Processor object routines.
;
;	Date:		06 october 1997
;	Revision:	06 November 1997
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
;	BUILD OBJECT		Build one object. link it with the next one, align its
;						display bitmap and copy the bitmap datas.
;
;	Given:
;	d0 = Object's address, aligned and displayable in the real list
;	a0 = Object address in the reference list
;	d2 = Display buffer
;	d3 = Bitmap address for this object
;	d4 = Length bitamp in bytes
;

BuildObject:				movem.l		d0-d4/a0,-(sp)

							add.l		#16,d0							; NEXT object's address (16 Bytes far)
							lsr.l		#3,d0

							move.l		d0,d1							; d0 = d1 = Link to Stop Object
							ror.l		#8,d0							; Split for Hardware
							and.l		#$ff000000,d0					; d0 = Lo Link
							lsr.l		#8,d1
							and.l		#$7ff,d1						; d1 = Hi Link

							;lea			ReferenceList,a0				; point to ReferenceList once again
							or.l		d0,4(a0)						; Patch-Up Bitmap Object

; Compute bitmap aligned address
							
							;move.l		#DISPBUF1,d2					; BitMap's address
							lsr.l		#3,d2							; Phrase aligned
							lsl.l		#3,d2
								
							move.l		d2,-(sp)							
							move.l		d3,-(sp)				; d0=target RAM and d1=source bitmap
							move.l		d4,-(sp)				; bitmap's size in bytes
							jsr			CopyBitMap						; Copy BitMap to target address
							add.l		#12,sp
							
							lsl.l		#8,d2
							or.l		d2,d1
							move.l		d1,(a0)							; Patch-Up Bitmap Object
							
							movem.l		(sp)+,d0-d4/a0
							rts

;	------------------------------------------------------------------------------
;	MakeObjList:   Build Object-List for just one bitmap object and stop object
;
;	Given:					Nothing
;	Returns:				ReferenceList = Master Object List
;							DisplayableList_Ptr = Ptr to a displayable Object List
;	Register Usage:			All Registers Preserved
;	Externals:				ObjListROM, ReferenceList, DisplayableList, DisplayableList_Ptr
;							DisplayBitMapPtr

MakeObjList:
							movem.l		d0-d2/a0-a1,-(sp)

; First, i copy source ROM object to the reference objects list in RAM

							move		#OBJSIZE-1,d1
							lea			ObjListROM,a1
							lea			ReferenceList,a0

.ocopy:						move.l		(a1)+,(a0)+						; Copy Raw Object list to Master ReferenceList
							dbra		d1,.ocopy


; The address of DisplayableList must be a multiple of 16 (Phrase aligned)

							move.l		#DisplayableList,d0
							add.l		#15,d0
							and.l		#$fffffff0,d0					; Adjust ptr to 2*Phrase Boundary
							move.l		d0,DisplayableList_Ptr			; DisplayableList_Ptr = Ptr to Displayable Object List

; We are building our object list, with two bitamp object 

							move.l		DisplayableList_Ptr,d0
							lea			ReferenceList,a0				; point to ReferenceList once again
							move.l		#DISPBUF1,d2					; BitMap's address
							move.l		#JagLogoROM,d3				; d0=target RAM and d1=source bitmap
							move.l		#JagLogoCountROM-JagLogoROM,d4	; bitmap's size in bytes
							jsr			BuildObject
							
;							move.l		DisplayableList_Ptr,d0
							add.l		#16,d0
;							lea			ReferenceList,a0				; point to ReferenceList once again
							add.l		#16,a0							
							move.l		#DISPBUF2,d2					; BitMap's address
							move.l		#FlagsROM,d3				; d0=target RAM and d1=source bitmap
							move.l		#FlagsCountROM-FlagsROM,d4	; bitmap's size in bytes
							jsr			BuildObject
							
; Copy the reference list to the displayable object list

							move		#OBJSIZE-1,d1
							
							move.l		DisplayableList_Ptr,a0
							lea			ReferenceList,a1

.cpylp:						move.l		(a1)+,(a0)+						; Copy Object list to display space
							dbra		d1,.cpylp
							
							movem.l		(sp)+,d0-d2/a0-a1
							rts

;	------------------------------------------------------------------------------
;	INITOP:   Start the Object processor.
;
;	Parameters:		Nothing
;	Returns:		Nothing
;	Register Usage:	All Registers Preserved
;	Externals:		VarVde, DisplayableList_Ptr, vblank

InitOP:						movem.l		d0,-(sp)

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
							movem.l		(sp)+,d0
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

;	------------------------------------------------------------------------------
;
;	SETPALETTE:   	Initialize the jaguar's palette.
;
;	Parameters:		Nothing
;
;	Returns:		Nothing
;
;	Register Usage:	All Registers Preserved
;
;	Externals:		Palette

SetPalette:					movem.l		d0/a0-a1,-(sp)

							move.l		#Palette,a0
							move.l		#CLUT,a1
							move.w		#$FF,d0

.cpyplt:					move.w		(a0)+,(a1)+
							dbra		d0,.cpyplt
				
							movem.l		(sp)+,d0/a0-a1
							rts

;	------------------------------------------------------------------------------
;
;	CONSTANT DATA

							DATA
							EVEN
;	------------------------------------------------------------------------------
;	Bitmap object

; Phrase 1/2, low long
YPOS						equ			68<<3 			; 68<<3		
HEIGHT						equ			58<<14	 		; Height in pixels

; Phrase 1/2, high long
LINK_ADR					equ			0				; will be computed
DATA_ADR					equ			0				; will be computed


; Phrase 2/2, low long
XPOS						equ			128
DEPTH						equ			4<<12 			; 4<<12	
PITCH						equ			1<<15 			; 1<<15		

; Phrase 2/2, high long
IWIDTH						equ			40				; width in phrases (Pixels / 4)		
DWIDTH						equ			IWIDTH<<18 		;IWIDTH<<18

IWIDTHhigh					equ			IWIDTH>>4		;IWIDTH>>4		
IWIDTHlow					equ			$80000000

;	------------------------------------------------------------------------------
;	Scaled BitMap object ROM

ObjListROM:					; Phrase 1
							dc.l	LINK_ADR+DATA_ADR
							dc.l	HEIGHT+YPOS+BITMAP_OBJECT

							; Phrase 2
							dc.l	IWIDTHhigh
							dc.l	IWIDTHlow+DWIDTH+PITCH+DEPTH+XPOS
							
;	------------------------------------------------------------------------------
;	64x64
							dc.l	LINK_ADR+DATA_ADR
							dc.l	$00100600
							dc.l	$00000001
							dc.l	$0040C0C0
														
;	------------------------------------------------------------------------------
;	STOP object
							dc.l	$00000000			; Stop Object		d63-d32
							dc.l	STOP_OBJECT			;					d31-d00

;	------------------------------------------------------------------------------
;	Bitmap source (CRY image from TGA source)
							
JagLogoROM:					include	"jaglogo.cry"
JagLogoCountROM:			dc.l	$00000000

FlagsROM:					include	"flags.cry"
FlagsCountROM:				dc.l	$00000000

;	------------------------------------------------------------------------------

Palette:
	dc.w	$0000,$01FF,$02FF,$03FF,$04FF,$05FF,$06FF,$07FF,$08FF,$09FF,$0AFF,$0BFF,$0CFF,$0DFF,$0EFF,$0FFF
	dc.w	$10FF,$11FF,$12FF,$13FF,$14FF,$15FF,$16FF,$17FF,$18FF,$19FF,$1AFF,$1BFF,$1CFF,$1DFF,$1EFF,$1FFF
	dc.w	$20FF,$21FF,$22FF,$23FF,$24FF,$25FF,$26FF,$27FF,$28FF,$29FF,$2AFF,$2BFF,$2CFF,$2DFF,$2EFF,$2FFF
	dc.w	$30FF,$31FF,$32FF,$33FF,$34FF,$35FF,$36FF,$37FF,$38FF,$39FF,$3AFF,$3BFF,$3CFF,$3DFF,$3EFF,$3FFF
	dc.w	$40FF,$41FF,$42FF,$43FF,$44FF,$45FF,$46FF,$47FF,$48FF,$49FF,$4AFF,$4BFF,$4CFF,$4DFF,$4EFF,$4FFF
	dc.w	$50FF,$51FF,$52FF,$53FF,$54FF,$55FF,$56FF,$57FF,$58FF,$59FF,$5AFF,$5BFF,$5CFF,$5DFF,$5EFF,$5FFF
	dc.w	$60FF,$61FF,$62FF,$63FF,$64FF,$65FF,$66FF,$67FF,$68FF,$69FF,$6AFF,$6BFF,$6CFF,$6DFF,$6EFF,$6FFF
	dc.w	$70FF,$71FF,$72FF,$73FF,$74FF,$75FF,$76FF,$77FF,$78FF,$79FF,$7AFF,$7BFF,$7CFF,$7DFF,$7EFF,$7FFF
	dc.w	$80FF,$81FF,$82FF,$83FF,$84FF,$85FF,$86FF,$87FF,$88FF,$89FF,$8AFF,$8BFF,$8CFF,$8DFF,$8EFF,$8FFF
	dc.w	$90FF,$91FF,$92FF,$93FF,$94FF,$95FF,$96FF,$97FF,$98FF,$99FF,$9AFF,$9BFF,$9CFF,$9DFF,$9EFF,$9FFF
	dc.w	$A0FF,$A1FF,$A2FF,$A3FF,$A4FF,$A5FF,$A6FF,$A7FF,$A8FF,$A9FF,$AAFF,$ABFF,$ACFF,$ADFF,$AEFF,$AFFF
	dc.w	$B0FF,$B1FF,$B2FF,$B3FF,$B4FF,$B5FF,$B6FF,$B7FF,$B8FF,$B9FF,$BAFF,$BBFF,$BCFF,$BDFF,$BEFF,$BFFF
	dc.w	$C0FF,$C1FF,$C2FF,$C3FF,$C4FF,$C5FF,$C6FF,$C7FF,$C8FF,$C9FF,$CAFF,$CBFF,$CCFF,$CDFF,$CEFF,$CFFF
	dc.w	$D0FF,$D1FF,$D2FF,$D3FF,$D4FF,$D5FF,$D6FF,$D7FF,$D8FF,$D9FF,$DAFF,$DBFF,$DCFF,$DDFF,$DEFF,$DFFF
	dc.w	$E0FF,$E1FF,$E2FF,$E3FF,$E4FF,$E5FF,$E6FF,$E7FF,$E8FF,$E9FF,$EAFF,$EBFF,$ECFF,$EDFF,$EEFF,$EFFF
	dc.w	$F0FF,$F1FF,$F2FF,$F3FF,$F4FF,$F5FF,$F6FF,$F7FF,$F8FF,$F9FF,$FAFF,$FBFF,$FCFF,$FDFF,$FEFF,$FFFF
							
