;	------------------------------------------------------------------------------
;	Revision 2.00 - INTRO Boot code for the jaguar
;
;	Date:		09 May 1997
;	Revision:	29 November 1997
;
;	Funship - Atari jaguar (c) France - 1997
;
;	------------------------------------------------------------------------------
;	Special thanks to Bastian schick (He knows why !).
;	------------------------------------------------------------------------------
;	This code should replace the original Atari code.
;	------------------------------------------------------------------------------

							include	"jag.inc"

;	------------------------------------------------------------------------------

DISPBUF1					equ			ENDRAM-(160*58*2)+8		; Jaguar logo
DISPBUF2					equ			DISPBUF1-(48*48*2)		; Fuji logo
DISPBUF3					equ			DISPBUF2-(96*96*2)		; Computer bitmap

OBJSIZE						equ			8*2						; Object List length in Longs
BASEVARS					equ			$1000

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
;	Get memory size (2 or 4 MB)

							move.w      #$35DD,d0					; default value for 4MB
							move.w      CONFIG,d1
							btst        #SIZERAM,d1					; bit set for 4MB ?
							beq.s       _4MB
							move.w      #$35CC,d0					; value for 2MB

_4MB:						move.w      D0,MEMCON2					; set value 
							move.w      #8,CHRO_CLK					; Default value
				
;	------------------------------------------------------------------------------
;	Video Initialization: PAL or NTSC display ?
									
							lea         PAL_TABLE,A0
							btst        #PALFLAG,D1					; if flag set you have a NTSC else it's a PAL Jag
							beq.s       PAL
							lea         NTSC_TABLE,A0

PAL:	    				move.w      (A0)+,CLK2					; Jerry

							move.w      (A0)+,HP					; and video registers
							move.w      (A0)+,HBB
							move.w      (A0)+,HBE	
							move.w      (A0)+,HS
							move.w      (A0)+,HVS
							move.w      (A0)+,HDB1
							move.w      (A0)+,HDB2
							move.w      (A0)+,HDE
							move.w      (A0)+,VP
							move.w      (A0)+,VBB
							move.w      (A0)+,VBE
							move.w      (A0)+,VS
							move.w      (A0)+,VDB

							move.w      (a0),VDE		
							move.w		(a0)+,VarVde

							move.w      (A0)+,VEB
							move.w      (A0)+,VEE
							move.w      (A0)+,HEQ

							move.l      #$70007,$F1A10C				; big endian value
							move.l      #$70007,$F0210C				; big endian value

							clr.w		VMODE
							move.w		#QUIET,sr
							
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

Install:					move.l		#DisplayableList,d0
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
;	just for fun

							include		"Play.inc"

;	------------------------------------------------------------------------------
;	Jump cardridge code (Never return)

JmpCardridge:				move.w		#$1861,d0
							or.b		CARD_BUS_WIDTH,d0
							
							move.l		#MAGIC1,$F03000
							lea			$1FB000,a5
							lea			$250(A5),A2
							move.l		#MAGIC2,(A2)
							move.l		#MAGIC3,4(A2)
							
							move.w		#$8100,$F14000
							lea			$F00000,a0
							lea			CARD_GET_ADDRESS,a1
							move.w		#$FFFF,$4E(a0)
							ori.w		#$700,sr
							jmp			(a2)

;	------------------------------------------------------------------------------
;	Software Reset, to re initialize the jaguar system

ResetJag:					clr.l		d0
							move.l		d0,OLP					; set OP to STOP-OBJECT
							move.w		d0,OBF					; clear OP-Flag
							reset
							jmp			StartBoot

;	------------------------------------------------------------------------------
							include		"objcpu.inc"
							include		"joypad.inc"
;	------------------------------------------------------------------------------

							DATA  
							EVEN                              

;	------------------------------------------------------------------------------

NTSC_TABLE:					dc.w		$00B5
							dc.w		$034C
							dc.w		$06B1
							dc.w		$007D
							dc.w		$06CD
							dc.w		$028B
							dc.w		$00A6
							dc.w		$00A6
							dc.w		$06A0
							dc.w		$020B
							dc.w		$01F4
							dc.w		$0018
							dc.w		$0205
							dc.w		$002E
							dc.w		$01F0
							dc.w		$01FF
							dc.w		$0006
							dc.w		$0310

;	------------------------------------------------------------------------------
PAL_TABLE:					dc.w		$00E2						; CLK2
							dc.w		$0352						; HP
							dc.w		$06AF						; HBB
							dc.w		$009E						; HBE
							dc.w		$06D5						; HS
							dc.w		$0259						; HVS
							dc.w		$00A6						; HDB1
							dc.w		$00A6						; HDB2
							dc.w		$06A0						; HDE
							dc.w		$0271						; VP
							dc.w		$0258						; VBB
							dc.w		$0022						; VBE
							dc.w		$026A						; VS
							dc.w		$002E						; VDB
							dc.w		$01AD						; VDE
							dc.w		$0265						; VEB
							dc.w		$0006						; VEE
							dc.w		$0313						; HEQ

;	------------------------------------------------------------------------------
							include		"objlist.inc"
;	------------------------------------------------------------------------------
							EVEN

Message1:					dc.b	"BootRom Version 1.00 FunShip 1997",0
Message2:					dc.b	"Joypad C button pressed...",0

;	------------------------------------------------------------------------------

						end
							