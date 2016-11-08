;	------------------------------------------------------------------------------
;	Project JAGUAR:	Basic Input/Ouput routines and frames communication INCLUDES.
;
;	File:		Trames.Inc
;	Author:		FAVARD Laurent
;	Date:		20 January 1998
;	Release:	31 February 1999
;	Language:	HISOFT DEVPAC 3-680x0
;
;	Funship - Atari Jaguar France / Copyright 1997-1998
;	------------------------------------------------------------------------------
;	This file contains somes defintions usefull for JagOS
;	They have no relation with the Jaguar hardware


; ---------------------------------------------------------------------------

NULL					equ			0

; ---------------------------------------------------------------------------
;	FRAME structure:	<SPID><TPID><CODE><LEN>CHECKSUM><DATA>
;						  4     4     2     2     2       n	

OFFSET_SPID				equ			0
OFFSET_TPID				equ			OFFSET_SPID+4
OFFSET_CODE				equ			OFFSET_TPID+4
OFFSET_LEN				equ			OFFSET_CODE+2
OFFSET_CHECK			equ			OFFSET_LEN+2
OFFSET_DATA				equ			OFFSET_CHECK+2

; ---------------------------------------------------------------------------
;	Frames types definitions (CODE)

FRAME_NACK				equ			'N'
FRAME_ACK				equ			'A'
FRAME_DATA				equ			'D'

;	These codes are view from the host computer and jaguar

FRAME_INQUIRE			equ			$0001								;	ask the jaguar's identification
FRAME_PRGSEND			equ			$0002								;	send a program to the jaguar
FRAME_MEMSEND			equ			$0003								;	send a memory block from the host to the jaguar
FRAME_MEMGET			equ			$0004								;	get a memory block from the jaguar to the host
FRAME_DBMSG				equ			$0005								;	jaguar send a string to the host
FRAME_PRGRUN			equ			$0006
FRAME_EEPROMGET			equ			$0007
FRAME_EEPROMSET			equ			$0008
FRAME_EXCEPTION			equ			$0009
FRAME_USER1				equ			$000A

MAX_OPCODE				equ			10		

SIZE_JAGID				equ			16
SIZE_JAGMEMDESC			equ			8
SIZE_JAGPROGRAM			equ			32
SIZE_JAGEEPROM			equ			128

; ---------------------------------------------------------------------------
;	Errors

ERR_NO_FRAME			equ			-1
ERR_CHEKSUM				equ			-2

; ---------------------------------------------------------------------------

HEADER_FRAME_LEN_GET	equ			14									;	number of bytes to get
HEADER_FRAME_LEN_SEND	equ			14									;	number of bytes to send

MAX_LEN_DATAFRAME		equ			32767								;	MAX Data block for any frame type
;MAX_CMD_DATAFRAME		equ			MAX_LEN_DATAFRAME 					;	MAX Data block for Command Frame

;	used in HandleServices

BIG_AREA				equ			1024								;	USED FOR STORE DATA BLOCK FOR HandleService !
FRAME_AREA				equ			12                                  ;	USED FOR STORE FRAMES'S INFOS (hedaer) FOR GETFRAME !

;	------------------------------------------------------------------------------
;	Objects positions into the main list object

OFF_TOOLSBAR			equ			0
OFF_WINDOW				equ			2
OFF_JAGOS				equ			4
OFF_XTENSION			equ			8			;	link
OFF_CDROM				equ			10			;	Extension
OFF_JOYSTICK			equ			12			;	LYNX
OFF_ICONPRG				equ			14			;	eyes
OFF_MOUSE				equ			16			:	Mouse pointer

;	------------------------------------------------------------------------------
;	THESE DEFINITIONS CAME FROM LAURENT FAVARD

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
;	Emplacement bits carte JAGUAR & FALCON

SBUF_HALF		   		equ			0	; LOW Sensitivity
SBUF_FULL		   		equ			1	; LOW Sensitivity
SBUF_EMPTY		   		equ			2	; LOW Sensitivity
RBUF_HALF		   		equ			3	; LOW Sensitivity
RBUF_FULL		   		equ			4	; LOW Sensitivity
RBUF_EMPTY		   		equ			5	; LOW Sensitivity
LINK_PRESENT	   		equ			6	; LOW Sensitivity
HARD_PLUS		   		equ			7	; LOW Sensitivity

;	------------------------------------------------------------------------------

StatusBuffer	   		equ			$E1F000
ReceiveBuffer	   		equ			$E1F400
ControlBuffer	   		equ			$E1F800
SendBuffer		   		equ			$E1FC00
