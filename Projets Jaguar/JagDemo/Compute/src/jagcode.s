;
;	THIS SIMPLE PROGRAM WORK ONLY WITH THE JAGOS FROM FAVARD LAURENT
;
;	Business:	Compute
;
;	Copyright (c) FAVARD Laurent 1998-2000
;	DEVPAC 3
;	------------------------------------------------------------------------------

			include		"sysapi.inc"

			XDEF		C_Start
			XDEF		C_Len
			
;	------------------------------------------------------------------------------

SIZE_PARAM	equ         6
SIZE_RESULT	equ         4

RESULT		equ			1

;	------------------------------------------------------------------------------

			DATA
			EVEN

C_Start:
			pea.l		Hello(pc)
			move.w		#DBMSG,-(sp)
			trap		#SYSTEM
			addq.l		#6,sp

;	------------------------------------------------------------------------------

Loop:       move.l		#SIZE_PARAM,-(sp)
			pea.l		Buffer(pc)
			move.l		#TOOLBOX_PID,-(sp)
			move.w		#GETDATA,-(sp)
			trap		#SYSTEM
			add.l		#14,sp

			tst.l		d0							;	If not accepted => quit
			beq.s		.End
			
            move.w      Buffer(pc),d0				;	Check if command == 0
            beq.s		.End						;	So end of program

			cmpi.w		#2,d0
			beq.s		.Substract
			
;	------------------------------------------------------------------------------
;	Get parameters for addition

.Add:       lea.l		Buffer(pc),a0
            addq.l		#2,a0

            move.w		(a0)+,d0        	        ;	get a parameter
            move.w		(a0),d1             	    ;	get b parameter
            add.w		d1,d0
			bra.s		.Result
			
;	------------------------------------------------------------------------------
;	Get parameters for substract

.Substract: lea.l		Buffer(pc),a0
            addq.l		#2,a0

            move.w		(a0)+,d0        	        ;	get a parameter
            move.w		(a0),d1             	    ;	get b parameter
            sub.w		d1,d0
			
;	------------------------------------------------------------------------------
;	Store result and send it

.Result:    lea.l		Buffer(pc),a0
            move.w		#RESULT,(a0)+
            move.w		d0,(a0)

            move.l		#SIZE_RESULT,-(sp)
			pea.l		Buffer(pc)
			move.l		#TOOLBOX_PID,-(sp)
			move.w		#SENDDATA,-(sp)
			trap		#SYSTEM
			add.l		#14,sp

            bra			Loop

;	------------------------------------------------------------------------------

.End:		pea.l		Bye(pc)
			move.w		#DBMSG,-(sp)
			trap		#SYSTEM
			addq.l		#6,sp

			move.w		#PTERM,-(sp)
			trap		#SYSTEM
			illegal

;	------------------------------------------------------------------------------

			DATA
			EVEN

Hello:		dc.b		"Compute Jaguar hello !",0
Bye:		dc.b		"Compute Jaguar bye bye!",0

;	------------------------------------------------------------------------------

			EVEN
            
Buffer:		dc.w		0		;	command
			dc.w		0		;	a parameter
            dc.w        0		;	b parameter

C_End:		dc.l		1
			
C_Len:		dc.l		C_End - C_Start

			END

