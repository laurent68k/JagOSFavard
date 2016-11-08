;	******************************************************************************
;	Revision 3.00 - Boot strap code for the jaguar system
;
;	Date	:	09 May 1997
;	Revision:	18 December 1997
;	File	:	INTROFJ.S	(Intro Fuji)
;
;	******************************************************************************
;	THIS SOURCE IS COPYRIGHT BY FAVARD LAURENT AND KEPT MY ENTIRE PROPRIETY.
;
;                                  COPYRIGHT 1997 FUNSHIP
;                  UNAUTHORIZED REPRODUCTION, ADAPTATION, DISTRIBUTION,
;               PERFORMANCE IS STRICTLY PROHIBITED WITHOUT ANY AUTHORSATION
;               FROM THE AUTHOR. ALL RIGHTS RESERVED.
;	******************************************************************************
;	Special thanks to Bastian schick (He knows why !).
;	******************************************************************************

							include		"include\jag.inc"

;	------------------------------------------------------------------------------

DISPBUF1					equ			ENDRAM-(160*58*2)+8		; Jaguar logo
DISPBUF2					equ			DISPBUF1-(48*48*2)		; Fuji logo
DISPBUF3					equ			DISPBUF2-(96*96*2)		; Computer bitmap

OBJSIZE						equ			8*2						; Object List length in Longs
BASEVARS					equ			$1000

TRUE						equ			1
FALSE						equ			0

;	------------------------------------------------------------------------------

PILE						equ			BASEVARS

;	RAM variables for Object processor

							RSINIT BASEVARS

							RSW FujiOffset,1
							RSW	FujiSens,1

							RSL JoyEdge,2						; Joypad 1&2 buttons's edges
							RSL JoyCur,2						; Joypad 1&2 buttons's status
							RSW VarVde,1						
							RSL FrameCount,1					; Count of Video Frames
							RSL DisplayableList_Ptr,1			; Ptr to Active Mungable ObjList
							RSL DisplayableList,OBJSIZE+16		; Storage for Mungable ObjList
							RSL ReferenceList,OBJSIZE 			; Storage for Master ObjList

							RSW Header,18
							RSW	ComputerReady,1							
							
;	------------------------------------------------------------------------------
;	ROM 680x0 Header
							TEXT

							dc.b		"BOOT"						; Just to pass the GEMDOS's header
							dc.l		PILE						; Reset SSP
							dc.l		StartBoot		 			; Reset PC 
;	------------------------------------------------------------------------------

							ORG			ADR_CODE

StartBoot:					move.w      #$1879,MEMCON1				; RAM wait state, ROM n clock cycles	
							move        #QUIET,sr					; keep quiet

							move.w		#$0,BG						; background color (16 bit RGB or CRY color)
							move.l      #$0,BORD1					; border color (green,red,unused,blue)
							
							lea			$F02110,A0
							moveq		#0,D0
							move.l		D0,4(A0)

							lea			$F1A110,A0
							move.l		D0,4(A0)
							 
;	------------------------------------------------------------------------------
;	Get memory size (2 or 4 Mb ?)

							jsr			InitSizeMemory
											
;	------------------------------------------------------------------------------
;	Video Initialization: PAL or NTSC display ?
									
							move.l		#NTSC_TABLE,-(sp)
							move.l		#PAL_TABLE,-(sp)
							jsr			InitVideo
							addq.l		#8,sp
							
;	------------------------------------------------------------------------------
							
							moveq		#0,d0
							move.l		d0,OLP						; set OP to STOP-OBJECT
							move.w		d0,OBF						; clear OP-Flag
							move.w		d0,$F00056					; undocumented register
							move.l		d0,$F00050					; stop PIT
							move.l		d0,$F10000					; stop JPIT1
							move.l		d0,$F10004					; stop JPIT2
								
;	------------------------------------------------------------------------------
;	I try to initialiaze the object processor

; The address of DisplayableList must be a multiple of 16 (Phrase aligned)

							move.l		#DisplayableList,d0
							add.l		#15,d0
							and.l		#$FFFFFFF0,d0				; Adjust ptr to 2*Phrase Boundary
							move.l		d0,DisplayableList_Ptr		; DisplayableList_Ptr = Ptr to Displayable Object List

; Install
							move.l		#ObjListROM,a0
							move.l		#ReferenceList,a1
							move.l		DisplayableList_Ptr,a2
							jsr			MakeObjList					; Initialize Object List

							jsr			InitOP						; Initialize Object processor
							move.w		#$4C1,VMODE					; Enable now the video processor
		
;	------------------------------------------------------------------------------
;	This a small intro, just for fun !

							jsr			PlayIt							
crash:						jmp			ResetJaguar				
							
							include		"play\play.inc"

;	------------------------------------------------------------------------------

							include		"system\upload.inc"
							include		"device\kit.inc"
							include		"system\memory.inc"
							include		"system\video.inc"
							include		"system\tools.inc"
							include 	"object\makeobj.inc"
							include		"object\objcpu.inc"
							include		"device\joypad.inc"

;	------------------------------------------------------------------------------

							DATA  
							EVEN

							include		"datas\video.dat"
							include		"datas\objlist.dat"
							
							EVEN

Message:					dc.b	"JAGUAR monitor 1.0 Ready",13,10,0
							EVEN
MessageStart:				dc.b	"Upload Jag header",13,10,0
							EVEN
MessageLoad:				dc.b	"Upalod Jag program...",13,10,0
							EVEN
MessageOk:					dc.b	"JAG program loaded start it !",13,10,0

;	------------------------------------------------------------------------------

						end
							