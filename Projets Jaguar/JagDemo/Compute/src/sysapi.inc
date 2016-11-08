;	------------------------------------------------------------------------------
;	Project JAGUAR:	Jaguar Kernel'API
;
;	File:		Sysapi.Inc
;	Author:		FAVARD Laurent
;	Date:		01 April 1998
;	Release:	22 December 1998
;	Language:	HISOFT DEVPAC 3-680x0
;
;	Funship - Atari Jaguar France / Copyright 1997-1998
;	------------------------------------------------------------------------------

;	general

;NULL					equ			0

KERNEL_PID				equ			$4A4A4A4A
TOOLBOX_PID				equ			$48484848

;	parameters for SETPOINTER function

ARROW					equ		0
BUSY					equ		1
USER_MOUSE				equ		255

;	returned by VIDEORATE

VIDEO_PAL				equ		50
VIDEO_NTSC				equ		60

;	returned by MEMSIZE

RAM_2MB					equ		2
RAM_4MB					equ		4

;	parameters for WindGet

;	Command
W_WORKXYWH				equ		1
W_CURRXYWH				equ		2
W_WORKBUFF				equ		3
W_CURRBUFF				equ		4
W_ADROBJ				equ		5

;	Handle
W_CONSOLE				equ		1

;	parameters for SETDRV
D_BOARD					equ		0
D_LYNX					equ		1
D_CENTR					equ		2

;	------------------------------------------------------------------------------

SYSTEM			equ		0				;	Trap #System


DBMSG			equ		0				;	Send a Debug String

GETDATA			equ		1				;	Receive datas from the host
SENDDATA		equ		2				;	Send datas to the host

UNPACK			equ		3				;	To unpack datas

DISABLE_IMAGE	equ		4				;	To change a bitmap
MOVE_OBJECT		equ		5				;	To move an object with two offsets
GET_JOYPAD		equ		6				;	Get the current state of JoyPad #1, #2
MAKEOBJLIST		equ		7				;	Build a new object list
INITOP			equ		8				;	Init and start the Object Processor
WAITSYNC		equ		9				;	Wait the next VBL interrupt
SETPOINTER		equ		10				;	Change the mouse pointer

WRITEEPROM		equ		11				;	Write 128 bytes into the serial Eeprom
READEPROM		equ		12				;	Read 128 bytes from the serial Eeprom
RESETEPROM		equ		13				;	Clear the Eeprom

VIDEORATE		equ		14				;	Get the currently video rate 
MEMSIZE			equ		15				;	Get the memory size
HOSTCONNECTED	equ		16				;	Check if a host computer is connected
GETVERSION		equ		17				;	Get the OS version

SETICONPRG		equ		18				;	Set a new icon program (at the right top screen)

PTERM			equ		19				;	Terminate a running jaguar program
USER1MSG		equ		20				;	Send an User 1 Message to the host

WINDGET			equ		21				;	Get informations about the execution window
ENABLE_IMAGE	equ		22

OSRESET			equ		23
EX_CARTRIDGE	equ		24

SETDRIVER		equ		25				;	Set current communication channel with host

