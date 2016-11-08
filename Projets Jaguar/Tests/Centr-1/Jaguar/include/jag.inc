;	------------------------------------------------------------------------------
;	Revision 3.00 - GLOBAL INCLUDE for the jaguar
;
;	Date:		06 October 1997
;	Revision:	09 December 1997
;
;	Funship - Atari jaguar (c) France - 1997
;	------------------------------------------------------------------------------

VMODE						equ			$F00028					; Video Mode

BORD1						equ			$F0002A					; Border Color (Red & Green)
BORD2						equ			$F0002C					; Border Color (Blue)

HP							equ			$F0002E					; Horizontal Period
HBB							equ			$F00030					; Horizontal Blanking Begin
HBE							equ			$F00032					; Horizontal Blanking End
HS							equ			$F00034					; Horizontal Sync
HVS							equ			$F00036					; Horizontal Vertical Sync
HDB1						equ			$F00038					; Horizontal Display Begin One
HDB2						equ			$F0003A					; Horizontal Display Begin Two
HDE							equ			$F0003C					; Horizontal Display End
VP							equ			$F0003E					; Vertical Period
VBB							equ			$F00040					; Vertical Blanking Begin
VBE							equ			$F00042					; Vertical Blanking End
VS							equ			$F00044					; Vertical Sync
VDB							equ			$F00046					; Vertical Display Begin
VDE							equ			$F00048					; Vertical Display End
VEB							equ			$F0004A					; Vertical EQUalization Begin
VEE							equ			$F0004C					; Vertical EQUalization End

VI							equ			$F0004E					; Vertical Interrupt
HEQ							equ			$F00054					; Horizontal EQUalization End
BG							equ			$F00058					; Background Color

MEMCON1 					equ			$F00000					; Memory Configuration Register One
BASEREGISTERS				equ			$F00000
MEMCON2						equ			$F00002

JOYSTICK					equ			$F14000 		; TOM Internal Register Base
JOYBUTS						equ			$F14002					; Joystick register
CONFIG						equ			$F14002					; Also has NTSC/PAL

INT1						equ			$F000E0
INT2						equ			$F000E2

CLUT						equ			$F00400
OLP							equ			$F00020
OBF							equ			$F00026

ENDRAM						equ			$200000					; End of DRAM

CLK2						equ			$F10012					; Jerry
CHRO_CLK					equ			$F10014					; Jerry

;	------------------------------------------------------------------------------
;	Constants to jump into the cardridge

CARD_BUS_WIDTH				equ			$800400
CARD_GET_ADDRESS			equ			$800404

MAGIC1						equ			$3D0DEAD
MAGIC2						equ			$30802251
MAGIC3						equ			$4ED14E71

BIG_ENDIAN					equ			$70007
;	------------------------------------------------------------------------------

PALFLAG						equ			4
SIZERAM						equ			5
	
QUIET						equ			$2700
ADR_CODE					equ			$E00008					; Start of this code

;	------------------------------------------------------------------------------
;	Object contants

BITMAP_OBJECT				equ			0 ;%000					; Bitmap object (non-scaled)
STOP_OBJECT					equ			4 ;%100					; Stop object
SCALED_OBJECT				equ			1 ;%001					; Scaled object
EGPU_OBJECT					equ			2 ;%010					; Elusive GPU-object

;	------------------------------------------------------------------------------
;     JERRY Joystick Equates

; Bits when LONGword is formatted as below.
; Format: xxApxxBx RLDU147* xxCxxxox 2580369#

JOY_UP						EQU	20						;joypad
JOY_DOWN					EQU	21
JOY_LEFT					EQU	22
JOY_RIGHT					EQU	23

FIRE_A						EQU	29						;fire buttons
FIRE_B						EQU	25
FIRE_C						EQU	13
KEY_OPTION					EQU	9
KEY_PAUSE					EQU	28

KEY_STAR					EQU	16						;keypad
KEY_7						EQU	17
KEY_4						EQU	18
KEY_1						EQU	19

KEY_0						EQU	4
KEY_8						EQU	5
KEY_5						EQU	6
KEY_2						EQU	7

KEY_HASH					EQU	0
KEY_9						EQU	1
KEY_6						EQU	2
KEY_3						EQU	3

ANY_JOY						EQU	$00F00000				; AND joyedge with this...													
														; joypad was pressed if result is not 0
ANY_FIRE					EQU	$32002200				; AND joyedge with this...
														; A,B C, Option or Pause was pressed
														; if result is not 0
ANY_KEY						EQU	$000F00FF				; AND joyedge with this... 123456789*0#
														; was pressed if result is not 0

;	------------------------------------------------------------------------------
; Masks for INT1 CPU Interrupt Control Register

C_VIDENA					EQU	$0001		; Enable Video time-base interrupts
C_GPUENA					EQU	$0002		; Enable GPU register-write interrupt
C_OPENA						EQU	$0004		; Enable OP stop-object interrupt
C_PITENA					EQU	$0008		; Enable PIT interrupt
C_JERENA					EQU	$0010		; Enable Jerry interrupt

C_VIDCLR					EQU	$0100		; Clear pending video interrupts
C_GPUCLR					EQU	$0200		; Clear pending GPU interrupts
C_OPCLR						EQU	$0400		; Clear pending OP interrupts
C_PITCLR					EQU	$0800		; Clear pending PIT interrupts
C_JERCLR					EQU	$1000		; Clear pending Jerry interrupts

;	Clear all interrupts pending and allow video int.
INT1_VALUE					equ			$1F01

;	------------------------------------------------------------------------------
;	Macros definitions (To reserve variables from the RAM memory)

RSCOUNT         SET 0

RSB             MACRO 							;label,count
\1              EQU RSCOUNT
RSCOUNT         SET RSCOUNT+\2
                ENDM

RSW             MACRO 							;label,count
\1              EQU RSCOUNT
RSCOUNT         SET RSCOUNT+\2*2
                ENDM

RSL             MACRO 							;label,count
\1              EQU RSCOUNT
RSCOUNT         SET RSCOUNT+\2*4
                ENDM

RSP             MACRO 							;label,count
\1              EQU RSCOUNT
RSCOUNT         SET RSCOUNT+\2*8
                ENDM

RSINIT          MACRO							;start address
RSCOUNT         SET \1
                ENDM

;	------------------------------------------------------------------------------
