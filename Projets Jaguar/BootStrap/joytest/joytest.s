;	------------------------------------------------------------------------------
;	BOOTROM Revision 0.00 - Boot code for the jaguar
;
;	Date:		09 May 1997
;	Revision:	13 November 1997
;
;	Funship - Atari jaguar (c) France - 1997
;
;	------------------------------------------------------------------------------
;	Special thanks to Bastian schick (He knows why !).
;	------------------------------------------------------------------------------
;	This code should replace the original Atari code.
;	------------------------------------------------------------------------------

							include	"jag.inc"
							include	"Mac.inc"

;	------------------------------------------------------------------------------

DISPBUF1					equ			ENDRAM-(128*128*2)
DISPBUF2					equ			DISPBUF1-(64*64*2)

OBJSIZE						equ			5*2						; Object List length in Longs
BASEVARS					equ			$1000

;	------------------------------------------------------------------------------

PILE						equ			BASEVARS

;	RAM variables for Object processor

							RSINIT BASEVARS

							RSL JoyEdge,2
							RSL JoyCur,2
							RSW VarVde,1						
							RSL FrameCount,1					; Count of Video Frames
							RSL DisplayableList_Ptr,1			; Ptr to Active Mungable ObjList
							RSL DisplayableList,OBJSIZE+16		; Storage for Mungable ObjList
							RSL ReferenceList,OBJSIZE 			; Storage for Master ObjList

;	------------------------------------------------------------------------------

;	RAM variables in exception 68000 area !!

sr_save						equ			$250
pc_save						equ			$24C
err_num						equ			$26E
reg_save					equ			$208
usp_save					equ			$248
gpu_state					equ			$25A
dsp_state					equ			$262
dsp_pc						equ			$25E
gpu_pc						equ			$256
VI_save						equ			$252
clut0						equ			$26A

;	------------------------------------------------------------------------------

							TEXT

							dc.b		"BOOT"						; Just to pass the GEMDOS's header
							dc.l		PILE						; Reset SSP
							dc.l		StartBoot		 			; Reset PC 

;	------------------------------------------------------------------------------

							ORG			ADR_CODE

StartBoot:					move.w      #$1879,$F00000				; RAM wait state, ROM n clock cycles	
							move        #QUIET,sr					; keep quiet
							move.w		sr,sr_save.w
							move.l		#$E00000,pc_save.w

							clr.l		err_num.w
							move.w		#$0,$F00058						; background color (16 bit RGB or CRY color)
							move.l      #$FF,$F0002A				; border color (green,red,unused,blue)
							
							movem.L		D0-D7/A0-A7,reg_save.W
							move.L		usp,A0
							move.L		A0,usp_save.W
	
							lea			$F02110,A0
							moveq		#0,D0
							move.L		4(A0),gpu_state.W
							move.L		D0,4(A0)
							move.L		(A0),D1
							addq.L		#2,D1
							move.L		D1,gpu_pc.W

							lea			$F1A110,A0
							move.L		4(A0),dsp_state.W
							move.L		D0,4(A0)
							move.L		(A0),D1
							addq.L		#2,D1
							move.L		D1,dsp_pc.W
							
							move.L		$100.W,VI_save.W
							move.L		$F00400,clut0.W
 
;	------------------------------------------------------------------------------
;	Get memory size (2 or 4 MB)

							move.w      #$35DD,d0					; default value for 4MB
							move.w      $F14002,d1
							btst        #5,d1					; bit set for 4MB ?
							beq.s       _4MB
							move.w      #$35CC,d0					; value for 2MB

_4MB:						move.w      D0,$F00002					; set value 
							move.w      #8,CHRO_CLK					; Default value
				
;	------------------------------------------------------------------------------
;	Video Initialization: PAL or NTSC display ?
				
							lea         PAL_TABLE,A0
							btst        #4,D1					; if flag set you have a NTSC else it's a PAL Jag
							beq.s       PAL
							lea         NTSC_TABLE,A0

PAL:	    				move.w      (A0)+,CLK2					; Jerry

							lea			$F00000,a1
							move.w      (A0)+,$2E(a1)				; and video registers
							move.w      (A0)+,$30(a1)
							move.w      (A0)+,$32(a1)
							move.w      (A0)+,$34(a1)
							move.w      (A0)+,$36(a1)
							move.w      (A0)+,$38(a1)
							move.w      (A0)+,$3A(a1)
							move.w      (A0)+,$3C(a1)
							move.w      (A0)+,$3E(a1)
							move.w      (A0)+,$40(a1)
							move.w      (A0)+,$42(a1)
							move.w      (A0)+,$44(a1)
							move.w      (A0)+,$46(a1)

							move.w      (a0),$48(a1)		
							move.w		#$ffff,$0048(a0)   			; due to a bug in Jaguar Console !
							move.w		(a0)+,VarVde

							move.w      (A0)+,$4A(a1)
							move.w      (A0)+,$4C(a1)
							move.w      (A0)+,$54(a1)

							move.l      #$70007,$F1A10C				; big endian value
							move.l      #$70007,$F0210C				; big endian value

							clr.w		$F00028
							move.w		#QUIET,sr
							
;	------------------------------------------------------------------------------
							
							moveq		#0,d0
							move.l		d0,$20(a1)					; set OP to STOP-OBJECT
							move.w		d0,$26(a1)					; clear OP-Flag
							move.w		d0,$56(a1)					; set CRY mode to color/not BW
							move.l		d0,$50(a1)					; stop PIT
							move.l		d0,$f10000					; stop JPIT1
							move.l		d0,$f10004					; stop JPIT2
								
;	------------------------------------------------------------------------------
;	I try to initialiaze the object processor :-)

							jsr			MakeObjList					; Initialize Object List to single 320x200 Object
							jsr			InitOP						; Initialize Object processor
																		
;	------------------------------------------------------------------------------
;	Enable now the video processor

							move.w		#$4C1,$F00028
		
;	Read position joypad
Boucle:						jsr			WaitSync
							jsr			ReadJoyPads
							move.l		JoyCur,d0

							move.l		#0,d2
							move.l		#0,d3

ck_up:						btst.l		#JOY_UP,d0
							beq.s		ck_down
							move.l		#-1,d2						; UP => 1 pixels to substract
							bra.s		ck_left

ck_down:					btst.l		#JOY_DOWN,d0
							beq.s		ck_left
							move.l		#1,d2						; DOWN  => 1 pixels to add
			

ck_left:					btst.l		#JOY_LEFT,d0
							beq.s		ck_right
							move.l		#-1,d3						; LEFT => 1 pixels to substract
							bra			deplace

ck_right:					btst.l		#JOY_RIGHT,d0
							beq.s		deplace
							move.l		#1,d3						; RIGHT => 1 pixels to add


;	Move object
deplace:					lea.l		ReferenceList,a0
							move.l		12(a0),d0
							move.l		d0,d1
							andi.l		#$FFFFF000,d0
							andi.l		#$00000FFF,d1
							
							; inc Xposition							
							add.l		d3,d1						; Ajoute offset X 
							andi.l		#$000000FF,d1
							or.l		d1,d0
							move.l		d0,12(a0)

							; inc Yposition
							move.l		4(a0),d0
							move.l		d0,d1
							andi.l		#$FFFFC000,d1

							lsr.l		#3,d0
							andi.l		#$7FF,d0

							add.l		d2,d0						; Ajoute offset Y 
							andi.l		#$0FF,d0
							lsl.l		#3,d0
							or.l		d0,d1
							move.l		d1,4(a0)
						
							bra			Boucle



;	------------------------------------------------------------------------------
;	doesn't work currently :-(

JmpCardridge:				move.w		#0,INT1

							clr.w		d0
							move.b		CARD_BUS_WIDTH,d0
							or.w		#$1861,d0
							move.w		d0,$F00000
							
							move.l		CARD_GET_ADDRESS,a0
							move.w		#$2000,sr
							jmp			(a0)

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

PAL_TABLE:					dc.w		$00E2
							dc.w		$0352
							dc.w		$06AF
							dc.w		$009E
							dc.w		$06D5
							dc.w		$0259
							dc.w		$00A6
							dc.w		$00A6
							dc.w		$06A0
							dc.w		$0271
							dc.w		$0258
							dc.w		$0022
							dc.w		$026A
							dc.w		$002E
							dc.w		$01AD
							dc.w		$0265
							dc.w		$0006
							dc.w		$0313

;	------------------------------------------------------------------------------

						end
							