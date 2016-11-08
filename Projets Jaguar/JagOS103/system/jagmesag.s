

; ---------------------------------------------------------------------------

						XDEF		JagMessage

                        TEXT
                        EVEN

; ---------------------------------------------------------------------------
;		void cdecl JagMessage(char *String)
;
;		Send to the host computer a string for debug purpose.
;
;	FONCTION:	Number 0
;
;	WARNINGS:	don't foget to restore a0, a1 from the stack
;       a0 = Address of this routine
;		a1 = Address of input parameters
;		d0 = will contain return value (as we want)
; ---------------------------------------------------------------------------

OFF_STRING				set			2											;	String's Address
OFF_CODEOP				set			0   										;	Function's number

JagMessage:             move.l		OFF_STRING(a1),a0							;	String's address
                        clr.l		d0
						move.w		#1,d0										;	always 1 byte cause the '\0'

.Loop:					tst.b		(a0)+										;	count number of bytes
                        beq.s		.Test
                        addq.w		#1,d0
                        bra.s		.Loop

.Test:					move.l		OFF_STRING(a1),-(sp)						;	String's address
                        move.w		d0,-(sp)									;	String's lenght
                        move.w		#FRAME_DBMSG,-(sp)							;	Frame's CODE
                        move.l		#TOOLBOX_PID,-(sp)							;	Target PID
                        move.l		#KERNEL_PID,-(sp)							;	Source PID, the KERNEL
		 				jsr			SendFrame
                        add.l		#16,sp

.Exit:                  movem.l		(sp)+,d1-d7/a0-a6
                        RTE
; ---------------------------------------------------------------------------


