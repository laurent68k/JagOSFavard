;	------------------------------------------------------------------------------
;	Project JAGUAR:	Basic Input/Ouput routines and frames communication INCLUDES.
;
;	File:		Trames.Inc
;	Author:		FAVARD Laurent
;	Date:		20 January 1998
;	Release:	27 March 1998
;	Language:	HISOFT DEVPAC 3-680x0
;
;	Funship - Atari Jaguar France / Copyright 1997-1998
;	------------------------------------------------------------------------------


TRUE					equ			1
FALSE					equ			0

DEBUG					equ			1
NORMAL					equ			2

MODE					equ			DEBUG
;MODE					equ			NORMAL

; ---------------------------------------------------------------------------
;	FOR DEBUG under TOS machines

BIOS					equ			13

DEV_AUX					equ			1
DEV_STDOUT				equ			2

BCONSTAT				equ			1
BCONIN					equ			2
BCONOUT					equ			3
BCOSTAT					equ			8

CRAWCIN					equ			7
CCONWS					equ			9
GEMDOS					equ			1

; ---------------------------------------------------------------------------

PORT_CARTRIDGE		equ		0
PORT_SERIAL			equ		1
PORT_CENTRONICS		equ		2

; ---------------------------------------------------------------------------
;	For the cartridge port

RECEIVE_BUFFER				equ			$FB8001
SEND_BUFFER					equ			$FA0000
STATUS_BUFFER				equ			$FB0001

CARTDRIDGE_SET				equ			7			; Low sensitivity
COMPUTER_SET				equ			6			; Low sensitivity
SBUF_EMPTY					equ			5			; Actif BAS
SBUF_FULL					equ			4			; Actif BAS
SBUF_HALF					equ			3			; Actif BAS
RBUF_EMPTY					equ			2			; Actif BAS
RBUF_FULL					equ			1			; Actif BAS
RBUF_HALF					equ			0			; Actif BAS


MASK_SBUF_EMPTY				equ			%00100000
MASK_SBUF_HALF				equ			%00001000
MASK_SBUF_FULL				equ			%00010000
MASK_RBUF_EMPTY				equ			%00000100
MASK_RBUF_HALF				equ			%00000001
MASK_RBUF_FULL				equ			%00000010

; ---------------------------------------------------------------------------

