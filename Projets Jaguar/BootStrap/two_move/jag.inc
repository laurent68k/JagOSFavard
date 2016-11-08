;	------------------------------------------------------------------------------
;	BOOTROM Revision 0.00 - Include for the jaguar
;
;	Date:		06 October 1997
;	Revision:	06 November 1997
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

CARD_BUS_WIDTH				equ			$800400
CARD_GET_ADDRESS			equ			$800404

ROMINIT						equ			$1861					; Bus width
PALFLAG						equ			4
SIZERAM						equ			5

;	
QUIET						equ			$2700
ADR_CODE					equ			$E00008					; Start of this code

;	------------------------------------------------------------------------------
;	Object contants

BITMAP_OBJECT				equ			%000					; Bitmap object (non-scaled)
STOP_OBJECT					equ			%100					; Stop object
SCALED_OBJECT				equ			%001					; Scaled object
EGPU_OBJECT					equ			%010					; Elusive GPU-object

;	Clear all interrupts pending and allow video int.
INT1_VALUE					equ			$1F01
