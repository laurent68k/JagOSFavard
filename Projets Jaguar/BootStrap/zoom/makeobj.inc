
;	------------------------------------------------------------------------------
;	MakeObjList:   Build Object-List for just one bitmap object and stop object
;	------------------------------------------------------------------------------
;	Version 2
;	Given:					a0 = ObjListROM
;							a1 = ReferenceList
;							a2 = DisplayableList ALIGNED !
;	Returns:				Refrence list ready to been copied
;	Register Usage:			All Registers Preserved
;	Externals:				Nothings
;	------------------------------------------------------------------------------

;	Object's size for the OP (in bytes)

SIZE_BITMAPOBJ				equ			16
SIZE_SCALEDOBJ				equ			24

;	Object's offset into the ROM list (in bytes)

OFF_BITMAPOBJ				equ			32
OFF_SCALEDOBJ				equ			38

;	Offset to get the values from the object list ROM.

O_TYPE						equ			0
O_DISPLAY					equ			2
O_DRAW						equ			6
O_SIZE						equ			10
O_HEIGHT					equ			12
O_YPOS						equ			14

O_1STPIX					equ			16
O_FLAGS						equ			18
O_IDX						equ			20
O_IWIDTH					equ			22
O_DWIDTH					equ			24
O_PITCH						equ			26
O_DEPTH						equ			28
O_XPOS						equ			30

O_REMAIN					equ			32
O_VSCALE					equ			34
O_HSCALE					equ			36

;	------------------------------------------------------------------------------
;	Sub code for each objcets

TableCode:					dc.l		BitmapObject			; %000
							dc.l		BitmapObject			; %001
							dc.l		EgpuObject				; %010
							dc.l		EndMakeObjList			; %011 = Error cause not exist
							dc.l		StopObject				; %100
							dc.l		EndMakeObjList			; %101 = Error cause not exist
							dc.l		EndMakeObjList			; %110 = Error cause not exist
							dc.l		EndMakeObjList			; %111 = Error cause not exist
							
							
MakeObjList:				movem.l		d0-d7/a0-a6,-(sp)							
							
TypeObject:					lea.l		TableCode,a6					; Code for each object
							move.w		O_TYPE(a0),d0					; type object
							lsl.l		#2,d0
							adda.w		d0,a6
							move.l		(a6),a6
							jmp			(a6)

;	------------------------------------------------------------------------------
;	Build BITAMP OBJECT or SCALED OBJECT (just a small difference)

BitmapObject:	

;	First, make the lower long word of the upper phrase (Link address+height+Ypos+type object)

							adda.l		#SIZE_BITMAPOBJ,a2				; a2 <= next object's address

							move.w		O_TYPE(a0),d0					; get type object			
							cmpi.w		#BITMAP_OBJECT,d0				; BITMAP or SCALED ?
							beq.s		noscaled						; bitmap object						
							adda.l		#8,a2							; scaled => add one phrase

noscaled:					move.l		a2,d0							; d0 <= link address
							lsr.l		#3,d0							; skip lower 3 bits

							move.l		d0,d1							; d0 = d1 = Link to Stop Object
							ror.l		#8,d0							; Split for Hardware
							and.l		#$ff000000,d0					; d0 = Lo Link
							lsr.l		#8,d1
							and.l		#$7ff,d1						; d1 = Hi Link

							move.w		O_HEIGHT(a0),d2					; get HEIGHT's object
							andi.l		#$3FF,d2
							move.l		#14,d3
							lsl.l		d3,d2							; shift HEIGHT of 14 bits
							or.l		d2,d0							; d0 <= d0 OR Height
							
							move.w		O_YPOS(a0),d2					; get YPOS's object
							andi.l		#$7FF,d2
							lsl.l		#3,d2
							or.l		d2,d0							; d0 <= d0 OR Xpos

							move.w		O_TYPE(a0),d2					; get YPOS's object
							andi.l		#$7,d2
							or.l		d2,d0							; d0 <= d0 OR Xpos
							
							;ori.l		#BITMAP_OBJECT,d0				; d0 <= d0 OR Type object

							move.l		d0,4(a1)						; store into reference list
							
; Second, make the upper long word of the upper phrase (Data address + Upper link address)
							
							move.l		O_DISPLAY(a0),d2				; Display BitMap's address
							lsr.l		#3,d2							; Phrase aligned
							lsl.l		#3,d2

							move.l		O_DRAW(a0),d3					; source bitmap draw
							move.w		O_SIZE(a0),d4					; size source bitmap
							andi.l		#$FFFF,d4
							
							move.l		d2,-(sp)						; target display area	
							move.l		d3,-(sp)						; source bitmap
							move.l		d4,-(sp)						; bitmap's size in bytes
							jsr			CopyBitMap						; Copy BitMap to target address
							add.l		#12,sp
							
							lsl.l		#8,d2
							or.l		d2,d1
							move.l		d1,0(a1)						; store into reference list

; Third, make the upper long word of the lower phrase

							clr.l		d0

							move.w		O_IWIDTH(a0),d0					; d1‚.w <= IWIDTH
							lsr.l		#4,d0							; MSB [9..4] of Iwidth
							andi.l		#$3F,d0
							
							move.w		O_IDX(a0),d1					; get idx
							andi.l		#$7F,d1
							lsl.l		#6,d1
							or.l		d1,d0							; d0.l <= d0.l OR IDX

							move.w		O_FLAGS(a0),d1					; get flags
							andi.l		#$F,d1
							move.l		#13,d2
							lsl.l		d2,d1
							or.l		d1,d0							; d0.l <= d0.l OR FLAGS

							move.w		O_1STPIX(a0),d1					; get 1st pixel
							andi.l		#$3F,d1
							move.l		#17,d2
							lsl.l		d2,d1
							or.l		d1,d0							; d0.l <= d0.l OR 1STPIXEL

							move.l		d0,8(a1)						; store into reference list

; Four, make the lower long word of the lower phrase

							clr.l		d0

							move.w		O_XPOS(a0),d0
							andi.l		#$FFF,d0		

							move.w		O_DEPTH(a0),d1					; d1.w <= DEPTH
							andi.l		#$7,d1
							move.l		#12,d2
							lsl.l		d2,d1
							or.l		d1,d0							; d0.l <= d0.l OR 


							move.w		O_PITCH(a0),d1					; d1.w <= PITCH
							andi.l		#$7,d1
							move.l		#15,d2
							lsl.l		d2,d1
							or.l		d1,d0							; d0.l <= d0.l OR 

							move.w		O_DWIDTH(a0),d1					; d1.w <= DWIDTH
							andi.l		#$3FF,d1
							move.l		#18,d2
							lsl.l		d2,d1
							or.l		d1,d0							; d0.l <= d0.l OR 

							move.w		O_IWIDTH(a0),d1					; d1.w <= IWIDTH
							move.l		#28,d2							; LSB [3..0] of Iwidth
							lsl.l		d2,d1
							or.l		d1,d0							; d0.l <= d0.l OR 

							move.l		d0,12(a1)

; Terminate
							move.w		O_TYPE(a0),d0					; get type object			
							cmpi.w		#SCALED_OBJECT,d0				; BITMAP or SCALED ?
							beq.s		ScaledObject					; process extra code
							
							adda.l		#OFF_BITMAPOBJ,a0				; Next object from the ROM list
							adda.l		#SIZE_BITMAPOBJ,a1				; Next object into reference list
							bra			TypeObject
							
;	------------------------------------------------------------------------------
;	EXTRA CODE for the scaled object (in addition to the bitmap object)
;	Just build the phrase 3.

ScaledObject:				move.l		#0,16(a1)						; upperlong word is always 0
							clr.l		d0

							move.w		O_REMAIN(a0),d0					; d0.w <= REMAIN
							andi.l		#$FF,d0
							move.l		#16,d1
							lsl.l		d1,d0

							move.w		O_VSCALE(a0),d1					; d1.w <= VSCALE
							andi.l		#$FF,d1
							lsl.l		#8,d1
							or.l		d1,d0							; d0.l <= d0.l OR VSCALE

							move.w		O_HSCALE(a0),d1					; d1.w <= HSCALE
							andi.l		#$FF,d1
							or.l		d1,d0							; d0.l <= d0.l OR HSCALE

							move.l		d0,20(a0)						; store to lower long word
							
							adda.l		#OFF_SCALEDOBJ,a0				; Next object from the ROM list
							adda.l		#SIZE_SCALEDOBJ,a1				; Next object into reference list
							bra			TypeObject

;	------------------------------------------------------------------------------
;	NO IMPLEMENTED CODE
							
EgpuObject:					
							bra			EndMakeObjList

;	------------------------------------------------------------------------------
;	Build STOP OBJECT:	Just put the STOP_OBJECT code into reference list.

StopObject:
							move.l		#0,(a1)+
							move.l		#STOP_OBJECT,(a1)											
							
;	------------------------------------------------------------------------------

EndMakeObjList:				movem.l		(sp)+,d0-d7/a0-a6
							rts
										
														
;	------------------------------------------------------------------------------
;	LIST ROM DEFINITIONS:
;
;	BITMAP Structure object: (32 bytes)
;
;							; For the phrase 1/2
;							dc.w	BITMAP_OBJECT	0
;							dc.l	DISPLAY_BUFFER	+2
;							dc.l	BitmapDraw		+6
;							dc.w	SizeDraw		+10
;							dc.w	HEIGHT			+12
;							dc.w	YPOS			+14
;
;							; For the phrase 2/2
;							dc.w	1stpix			+16
;							dc.w	flags			+18
;							dc.w	idx				+20
;							dc.w	IWIDTH			+22
;							dc.w	DWIDTH			+24
;							dc.w	PITCH			+26
;							dc.w	DEPTH			+28
;							dc.w	XPOS			+30
;
;	BITMAP Structure object: (38 bytes)
;
;							; For the phrase 1/3
;							dc.w	SCALED_OBJECT	0
;							dc.l	DISPLAY_BUFFER	+2
;							dc.l	BitmapDraw		+6
;							dc.w	SizeDraw		+10
;							dc.w	HEIGHT			+12
;							dc.w	YPOS			+14
;
;							; For the phrase 2/3
;							dc.w	1STPIX			+16
;							dc.w	FLAGS			+18
;							dc.w	IDX				+20
;							dc.w	IWIDTH			+22
;							dc.w	DWIDTH			+24
;							dc.w	PITCH			+26
;							dc.w	DEPTH			+28
;							dc.w	XPOS			+30
;
;							; For the phrase 3/3
;							dc.w	REMAIN			+32
;							dc.w	VSCALE			+34
;							dc.w	HSCALE			+36
;
;	STOP structure object:  (2 bytes)
;
;							; For the only ONE phrase
;							dc.w	STOP_OBJECT
;
;	------------------------------------------------------------------------------

		