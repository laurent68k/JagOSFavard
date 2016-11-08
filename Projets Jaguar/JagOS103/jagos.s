;	******************************************************************************
;	Revision 5.01 - JagOS - Small Operating System for the jaguar.
;
;	Date	:	09 May 1997
;	Revision:	12 May 2000
;	File	:	JAGOS.S (Main file)
;	AUTHOR	:	FAVARD Laurent
;
;	******************************************************************************
;	THIS SOURCE IS COPYRIGHT BY FAVARD LAURENT AND KEEP MY ENTIRE PROPERSTIES.
;
;                        - COPYRIGHT 1997-99 FAVARD LAURENT -
;
;                  UNAUTHORIZED REPRODUCTION, ADAPTATION, DISTRIBUTION,
;               PERFORMANCE IS STRICTLY PROHIBITED WITHOUT ANY AUTHORISATION
;               FROM THE AUTHOR. ALL RIGHTS RESERVED.
;	******************************************************************************
;                           Special thanks to Bastian schick.
;                         And great think to my nice Laurence.
;	******************************************************************************

							include		"Include\OSdef.inc"
							include		"Include\jaguar.inc"
							include		"Include\JagOS.inc"

;	------------------------------------------------------------------------------

							OUTPUT		JagOS103.prg
							
;	------------------------------------------------------------------------------

DISPBUF1					equ			ENDRAM-(160*60*2)+8		; JagOS LOGO
DISPBUF2					equ			DISPBUF1-(64*64*2)		; TOOLSBAR 
DISPBUF3					equ			DISPBUF2-(128*64*2)		; WINDOW
DISPBUF4					equ			DISPBUF3-(32*64*2)		; XTENSION ISA
DISPBUF5					equ			DISPBUF4-(32*64*2)		; CDROM
DISPBUF6					equ			DISPBUF5-(32*64*2)		; JOYSTICK
DISPBUF7					equ			DISPBUF6-(32*32*2)		; ICON Program
DISPBUF8					equ			DISPBUF7-(24*24*2)		; MOUSE cursor


OBJLISTSIZE					equ			24						; Object List length in phrases
BASEVARS					equ			$1000

TRUE						equ			1
FALSE						equ			0

;	------------------------------------------------------------------------------

PILE						equ			BASEVARS

;	RAM variables for OS and Object Processor

							RSINIT BASEVARS

							RSP DisplayableList,OBJLISTSIZE*3		; Storage for Mungable ObjList
							RSP ReferenceList,OBJLISTSIZE*3			; Storage for Master ObjList
			
							RSL DisplayableList_Ptr,1				; Ptr to Active Mungable ObjList
							RSL	ReferenceList_Ptr,1

							RSL	ListObjectLenght,1					; List Object's lenght
							
							RSL FrameCount,1						; Count of Video Frames
							RSL SysCounter1,1						; System Counter #1
							RSL SysCounter2,1						; System Counter #2
	
							RSL _Stack,1							; To save the Stack Pointer when Exec program

							RSL JoyPad1Edge,1						; Joypad 1 buttons's edges
							RSL JoyPad2Edge,1						; Joypad 2 buttons's edges
							RSL JoyPad1State,1						; Joypad 1 buttons's status						
							RSL JoyPad2State,1						; Joypad 2 buttons's status

							RSW VarVde,1						
							
							RSL	PtrRead,1
							RSL	PtrWrite,1
							RSW	CountSerial,1
							RSW	FifoSerial,512
							RSW	FifoEnd,1
							
							RSW	SizeExcepData,1

							RSL	SaveArea,15							; 8 d0-7 + 7 a0-a6
							RSL	WordStatus,1
							RSL	SaveUSP,1
							RSL	SaveSSP,1
							RSL	SavePC,1
							RSW	SaveSR,1

							RSW	_CurrMainDriver,1					; current driver for host communication
							RSW	_LockJoyPad2,1						; lock/unlock joypad #2
							RSL	_CurrRead,1
							RSL	_CurrWrite,1
							
;	------------------------------------------------------------------------------
;	Mysterious: I don't know what is it...

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
;	ROM 680x0 Header
							TEXT

							dc.b		"BOOT"						; Just to pass the GEMDOS's header
							dc.l		PILE						; Reset SSP
							dc.l		StartBoot		 			; Reset PC 
;	------------------------------------------------------------------------------
;	Note:	Comment as UNDEFINED means that no documentations was found about that

							ORG			ADR_CODE

StartBoot:					move.w      #$1879,MEMCON1				; RAM wait state, ROM n clock cycles	
							move        #QUIET,sr					; keep quiet
							move.w		sr,sr_save.w				; UNDEFINED
							move.l		#$E00000,pc_save.w			; UNDEFINED

							clr.l		err_num.w					; UNDEFINED
							move.w		#$1F<<6,BG					; background color (16 bit RGB or CRY color)
							move.l      #0,BORD1					; border color (green,red,unused,blue)

							movem.l		D0-D7/A0-A7,reg_save.W		; UNDEFINED
							move.l		usp,A0						; UNDEFINED
							move.l		A0,usp_save.W				; UNDEFINED
							
							lea			$F02110,a0
							moveq		#0,d0
							move.l		4(A0),gpu_state.W			; UNDEFINED
							move.l		d0,4(a0)
							move.l		(A0),D1
							addq.l		#2,D1
							move.l		D1,gpu_pc.W					; UNDEFINED

							lea			$F1A110,a0
							move.l		4(A0),dsp_state.W			; UNDEFINED
							move.l		d0,4(a0)
							move.l		(A0),D1
							addq.l		#2,D1
							move.l		D1,dsp_pc.W					; UNDEFINED

							move.l		$100.W,VI_save.W			; UNDEFINED
							move.l		$F00400,clut0.W				; UNDEFINED

;	------------------------------------------------------------------------------
;	Get memory size (2 or 4 Mb ?)

							move.w      #$35DD,d0					; default value for 4MB
							move.w      CONFIG,d1
							btst        #SIZERAM,d1					; bit set for 4MB ?
							beq.s       _4MB
							move.w      #$35CC,d0					; value for 2MB

_4MB:						move.w      d0,MEMCON2					; set value 
							move.w      #8,CHRO_CLK					; Default value

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
;	Install	Kernel System Trap and all others exceptions

							jsr			InstallTrap

;	------------------------------------------------------------------------------
;	I try to initialiaze the object processor

; The address of DisplayableList must be a multiple of 32 (4*Phrase aligned)

							move.l		#DisplayableList,d0
							addi.l		#31,d0
							andi.l		#$FFFFFFE0,d0				; Adjust ptr to 4*Phrase Boundary
							move.l		d0,DisplayableList_Ptr		; DisplayableList_Ptr = Ptr to Displayable Object List

							move.l		#ReferenceList,d0
							addi.l		#31,d0
							andi.l		#$FFFFFFE0,d0				; Adjust ptr to 4*Phrase Boundary
							move.l		d0,ReferenceList_Ptr		; DisplayableList_Ptr = Ptr to Displayable Object List

; Now build the object list and enable the OP

							move.l		#ObjListROM,-(sp)
							move.l		ReferenceList_Ptr,-(sp)
							move.l		DisplayableList_Ptr,-(sp)
							move.w		#MAKEOBJLIST,-(sp)
							trap		#SYSTEM
							add.l		#14,sp
							
							move.l		d0,ListObjectLenght
										
							move.l		ReferenceList_Ptr,-(sp)
							move.l		DisplayableList_Ptr,-(sp)
							move.w		#INITOP,-(sp)
							trap		#SYSTEM
							add.l		#10,sp
							
							move.w		#$4C1,VMODE					; Enable now the video processor
		
;	------------------------------------------------------------------------------
;	Default use the ComLynx port to communicate

							move.w		#$FF,_CurrMainDriver		; set anything to force device change
							move.w		#D_CENTR,-(sp)
							move.w		#SETDRIVER,-(sp)
							trap		#SYSTEM
							addq.l		#4,sp
							
							clr.l		FrameCount
							clr.l		SysCounter1
							clr.l		SysCounter2
							
							move.w		#$2000,sr					; Enable Interrupts
							
;	------------------------------------------------------------------------------
;	Now start intro and main loop system

							jsr			PlayIt
crash:						jmp			ResetJaguar


; ---------------------------------------------------------------------------
;	System info. structure
JagStruct:					dc.b		"_JAG"						;	Start mark
							dc.w		$0103						;   _SystemVersion
                    	    dc.w		$0000						;	_HardwareVersion
                        	dc.l        $12052000					;	_SystemDate
                     		dc.l        $22061972					;	_HardwareDate
; ---------------------------------------------------------------------------

MSG_WELCOME:				dc.b		"JagOS 2000 (c) for Jaguar by FAVARD Laurent",0

							include		"play\play.inc"

;	------------------------------------------------------------------------------

							include		"system\video.inc"
							include		"system\tools.inc"
							include		"system\tools2.inc"
							include		"system\tools3.inc"
							include		"system\unpack.inc"

							include 	"Object\makeobj.inc"
							include 	"Object\Vblank.inc"
							include		"Object\objcpu.inc"
							include		"Object\move.inc"

							include		"device\joypad.inc"
							include		"device\eeprom.inc"
							include		"device\board.inc"
							include		"device\serial.inc"
							include		"device\centr.inc"

							include		"System\InstTrap.inc"
							include		"System\Trap-OS.inc"
							include		"System\Pointer.inc"
							include		"system\jagmesag.s"
							include		"system\JHost2ja.s"
							include		"system\Jjag2hos.s"
							include		"system\drivers.inc"
							
							include		"com\services.s"
							include		"com\inquire.s"
							include		"com\geteprom.s"
							include		"com\seteprom.s"
							include		"com\Host2jag.s"
							include		"com\jag2host.s"
							include		"com\loadexe.s"
							include		"com\TramHigh.s"

							include		"Wind\WindGet.inc"

							;	This code seems doesn't work...
							;include		"cd\jagcd.s"

;	------------------------------------------------------------------------------

							include		"datas\video.dat"
							include		"datas\objlist.dat"
							
;	------------------------------------------------------------------------------
							end
							
							