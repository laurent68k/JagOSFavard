;	------------------------------------------------------------------------------
;	Revision 3.00 - Heart of the boostrap code: main loop
;
;	Date:		09 May 1997
;	Revision:	02 June 1998
;
;	Funship - Atari jaguar (c) France - 1997-1998
;
;	------------------------------------------------------------------------------

WAITVBL						equ			50*30
Y_STEP						equ			3
X_STEP						equ			2

;O_BOARD						equ			0
;O_LYNX						equ			4
;O_CENTR						equ			8

PlayIt:						move.l		#WAITVBL,d5								; wait before auto start
							clr.l		d4
							
;	------------------------------------------------------------------------------
;	Initialisation KIT and harwdare detection

							move.l		ReferenceList_Ptr,a0
							add.l		#OFF_XTENSION*8,a0		;#8*8,a0
							
							move.l		a0,-(sp)								; Object's address
							move.w		#DISABLE_IMAGE,-(sp)					; Function opcode
							trap		#SYSTEM
							addq.l		#6,sp
							
;	------------------------------------------------------------------------------
;	Check if a CDROM is plugged: Currently we consider no CDROM !

.ChkCDROM:					move.l		ReferenceList_Ptr,a0
							add.l		#OFF_CDROM*8,a0
							
							move.l		a0,-(sp)								; Object's address
							move.w		#DISABLE_IMAGE,-(sp)					; Function opcode
							trap		#SYSTEM
							addq.l		#6,sp

;	------------------------------------------------------------------------------
;	FIRST STEP:	JagOS Logo Zoom in and keep the logo centered

ZoomLogoIn:					move.l		#30,d4

.Loop:						move.w		#WAITSYNC,-(sp)
							trap		#SYSTEM
							trap		#SYSTEM
							addq.l		#2,sp
							
							jsr			ZoomLogoJagIn
							dbf			d4,.Loop
							
;	------------------------------------------------------------------------------
;	SECOND STEP:	Wait a few seconds...

							
FewSeconds:					move.w		#2*60,d4

.Wait:						move.w		#WAITSYNC,-(sp)
							trap		#SYSTEM
							addq.l		#2,sp
							dbf			d4,.Wait							
												
;	------------------------------------------------------------------------------
;	THIRD STEP:	JagOS Logo Zoom Out and move to the left top

ZoomLogoOut:				move.l		#13,d4

.Loop						move.w		#WAITSYNC,-(sp)
							trap		#SYSTEM
							addq.l		#2,sp

							jsr			ZoomLogoJagOut					; jaguar zoom in
							dbf			d4,.Loop
							
;	------------------------------------------------------------------------------
;	"Shell":	We are awaiting for a key pressed or a frame received

.MainLoop:					move.w		#WAITSYNC,-(sp)
							trap		#SYSTEM
							addq.l		#2,sp
							
							move.l		#JoyPad1State,-(sp)
							move.l		#JoyPad1Edge,-(sp)							
							move.w		#GET_JOYPAD,-(sp)
							trap		#SYSTEM
							add.l		#10,sp
							
							move.l		JoyPad1State,d0	
							move.l		JoyPad1Edge,d1
							clr.l		d2										; clear Y offset
							clr.l		d3										; clear X offset

;	Check BUTTON A
.ck_a:						btst.l		#FIRE_A,d1								; START cardridge
							beq.s		.ck_b
							jmp			JmpCartridge

;	Check BUTTON B
.ck_b:						btst.l		#FIRE_B,d1								; RESET jaguar
							beq.s		.ck_c
							jmp			ResetJaguar				

;	Check BUTTON C
.ck_c:						btst.l		#FIRE_C,d1								; MESSAGE to the host
							beq.s		.ck_up

							pea.l		MSG_WELCOME
							move.w		#DBMSG,-(sp)
							trap		#SYSTEM
        	                addq.l		#6,sp
							bra			.MainLoop
							
.ck_up:						btst.l		#JOY_UP,d0
							beq.s		.ck_down
							move.l		#-1*Y_STEP,d2							; UP => 1 pixels to substract
							
.ck_down:					btst.l		#JOY_DOWN,d0
							beq.s		.ck_left
							move.l		#Y_STEP,d2								; DOWN  => 1 pixels to add
			
.ck_left:					btst.l		#JOY_LEFT,d0
							beq.s		.ck_right
							move.l		#-1*X_STEP,d3							; LEFT => 1 pixels to substract

.ck_right:					btst.l		#JOY_RIGHT,d0
							beq.s		.ck_none
							move.l		#X_STEP,d3								; RIGHT => 1 pixels to add

.ck_none:					move.l		ReferenceList_Ptr,a0
							add.l		#OFF_MOUSE*8,a0
							
							move.l		a0,-(sp)
							move.l		d2,-(sp)
							move.l		d3,-(sp)
							move.w		#MOVE_OBJECT,-(sp)
							trap		#SYSTEM
							add.l		#14,sp

					

							jsr			HandleServices							; Test if a frame is present
							bra			.MainLoop
	
;	------------------------------------------------------------------------------
;	Special code for Intro only

							include		".\Play\ZoomIn.inc"
							include		".\Play\ZoomOut.inc"

							