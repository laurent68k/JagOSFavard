;	------------------------------------------------------------------------------
;	Title:programme test de g‚n‚ration code source;
;	Copyright (c) Laurent FAVARD 1998
;	Code generated for DEVPAC 3
;
;	Routine business:Exercice de g‚n‚ration par JagStudio
;	------------------------------------------------------------------------------

		include		".\Include\sysapi.inc"
		include		".\Include\jag.inc"

		XDEF	C_Start
		XDEF	C_Len


;	------------------------------------------------------------------------------
;	SZ_PARAM_FROM_HOST = Lenght of user data block await from the host
;	SZ_RESULT_TO_HOST = Lenght of user data block await from the host

SZ_PARAM_FROM_HOST	equ	0
SZ_RESULT_TO_HOST	equ	0
;	------------------------------------------------------------------------------

		TEXT


C_Start:
		pea.l		Welcome(pc)
		move.w		#DBMSG,-(sp)
		trap		#SYSTEM
		addq.l		#6,sp
;	------------------------------------------------------------------------------

		move.w		#EARTHEnd-EARTHBegin,-(sp)
		move.l		#EARTHBegin,-(sp)
		move.w		#SETICONPRG,-(sp)
		trap		#SYSTEM
		addq.l		#8,sp
;	------------------------------------------------------------------------------

		move.w		#APPLEEnd-APPLEBegin,-(sp)		; Lenght of packed datas
		move.l		#APPLEBegin,-(sp)		; Address of start packed datas
		move.w		#USER_MOUSE,-(sp)		; User define cursor
		move.w		#SETPOINTER,-(sp)
		trap		#SYSTEM
		addq.l		#4,sp
;	------------------------------------------------------------------------------

;	Wait for a block which contains parameters you wants
Await:		move.l		#SZ_PARAM_FROM_HOST,-(sp)
		pea.l		ParamIn(pc)
		move.l		#TOOLBOX_PID,-(sp)
		move.w		#GETDATA,-(sp)
		trap		#SYSTEM
		add.l		#14,sp

		tst.l		d0		;	equal 0 => Host didn't accept to send any block

		beq.s		Pterm		;	In this case what can we do !

		move.w		Param(pc),d0		;	Get first parameter...
;	------------------------------------------------------------------------------
;	 Code here the way to get your parameters and what to perform...

;	------------------------------------------------------------------------------

;	Send to the host the results

Results:	

		move.l		#SZ_PARAM_FROM_HOST,-(sp)
		pea.l		ParamOut(pc)
		move.l		#TOOLBOX_PID,-(sp)
		move.w		#SENDDATA,-(sp)
		trap		#SYSTEM
		add.l		#14,sp
		bra		Await
;	------------------------------------------------------------------------------

		pea.l		Goodbye(pc)
		move.w		#DBMSG,-(sp)
		trap		#SYSTEM
		addq.l		#6,sp
;	------------------------------------------------------------------------------

Pterm:	move.w		#PTERM,-(sp)
		trap		#SYSTEM
		illegal

;	------------------------------------------------------------------------------
		DATA
		EVEN
		; Icon program' datas
		include		".\Datas\EARTH.cci"

		DATA
		EVEN
		; Cursor program' datas
		include		".\Datas\APPLE.cci"

		DATA
		EVEN

Welcome:dc.b	"d‚but ex‚cution routine",0

		DATA
		EVEN

Goodbye:dc.b	"fin ex‚cution routine",0

;	------------------------------------------------------------------------------
		DATA
		EVEN

;	Input buffer host to Jagauar
ParamIn:

		DATA
		EVEN

;	Output buffer Jagauar to host
ParamOut:

;	------------------------------------------------------------------------------
C_End	dc.l	1

C_Len:	dc.l	C_End - C_Start
		END
