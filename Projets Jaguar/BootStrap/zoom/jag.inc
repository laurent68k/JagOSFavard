;	------------------------------------------------------------------------------
;	Revision 0.00 - Include for the jaguar
;
;	Date:		06 October 1997
;	Revision:	22 November 1997
;
;	Funship - Atari jaguar (c) France - 1997
;	------------------------------------------------------------------------------

INT1						equ			$F000E0
INT2						equ			$F000E2

VI							equ			$F0004E

CLUT						equ			$F00400
OLP							equ			$F00020

ENDRAM						equ			$200000					; End of DRAM

VMODE						equ			$F00028
CLK2						equ			$F10012					; Jerry
CHRO_CLK					equ			$F10014					; Jerry

;	------------------------------------------------------------------------------
;	Constants to jump into the cardridge

CARD_BUS_WIDTH				equ			$800400
CARD_GET_ADDRESS			equ			$800404

MAGIC1						equ			$3D0DEAD
MAGIC2						equ			$30802251
MAGIC3						equ			$4ED14E71

;	------------------------------------------------------------------------------

ROMINIT						equ			$1861					; Bus width
PALFLAG						equ			4
SIZERAM						equ			5

;	
QUIET						equ			$2700
ADR_CODE					equ			$E00008					; Start of this code

;	------------------------------------------------------------------------------
;	Object contants

BITMAP_OBJECT				equ			0 ;%000					; Bitmap object (non-scaled)
STOP_OBJECT					equ			4 ;%100					; Stop object
SCALED_OBJECT				equ			1 ;%001					; Scaled object
EGPU_OBJECT					equ			2 ;%010					; Elusive GPU-object

;	Clear all interrupts pending and allow video int.
INT1_VALUE					equ			$1F01

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
OPTION						EQU	9
PAUSE						EQU	28

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
