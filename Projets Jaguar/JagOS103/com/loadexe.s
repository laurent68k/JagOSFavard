
; ---------------------------------------------------------------------------

						XDEF		PrgExe

                        TEXT
                        EVEN

; ---------------------------------------------------------------------------
;	Macro to start a jaguar program and save all context

EXEC_PRG				MACRO

						move.l		#WordStatus,a0								;	get save area word status
						
						movem.l		d0-d7/a0-a6,-(a0)
						move.l		sp,SaveSSP
						move.l		usp,a0
						move.l		a0,SaveUSP
						move.w		sr,SaveSR
						
						move.l		#.Terminate,SavePC
						
           				move.l		L_OFF_ADDRSTART(a1),a0		   				;	get the Code address and start
                      	jmp			(a0)

.Terminate:																
						move.w		SaveSR,sr
						move.l		SaveUSP,a0
						move.l		a0,usp
						move.l		SaveSSP,sp								
						
						move.l		#SaveArea,a0								;	get save area word status
						movem.l		(a0)+,d0-d7/a0-a6

						ENDM
						
; ---------------------------------------------------------------------------

; ---------------------------------------------------------------------------
;		void cdecl PrgExe(void)
;
;		This function is called when the HOST computer ask to the jaguar to received
;		a program and execute it.
;
;		SPECIAL PARAMETERS:	Called by handleServices()
;				d0	=	CODE x 4 received
;				a1	=	Address of DATA block received
;				a2	=	Address of FRAME'sinformations received (SPID, TPID, etc...)
; ---------------------------------------------------------------------------


PrgExe:

L_OFF_ADDRTARGET		set         0
L_OFF_ADDRSTART	 		set         4
L_OFF_LENGHT			set         8

L_OFF_TPID				set			8											;	source PID will be our target PID

;	First we send a ACK frame to accept the transfer

						move.l		a1,-(sp)									;	no data
                        move.w		#SIZE_JAGPROGRAM,-(sp) 						;	lenght
                        move.w		#FRAME_PRGSEND,-(sp)  	                    ;	Frame's CODE
                        move.l		L_OFF_TPID(a2),-(sp)						;	Target PID is the source PID from host
                        move.l		#KERNEL_PID,-(sp)							;	Source PID is the kernel itself
		   				jsr			SendFrame
                        add.l		#16,sp

;	Second we get the memory block from the host computer
;	long cdecl GetBinaries(long SourcePID, long TargetPID, unsigned long Lenght, void *MemoryBlock)

						move.l		L_OFF_ADDRTARGET(a1),-(sp)					;	Target address to copy
						move.l		L_OFF_LENGHT(a1),-(sp)						;	Lenght to get
                        move.l		L_OFF_TPID(a2),-(sp)						;	Target PID is the source PID from host
                        move.l		#KERNEL_PID,-(sp)							;	Source PID is the kernel itself
						jsr         GetBinaries
                        add.l       #16,sp

;	If upload successfull, start this program

                        tst.l		d0
                        ble.s		.ExitWithError

;	Save Status Program Word and execute

						move.w		#RunningEnd-RunningBegin,-(sp)
						move.l		#RunningBegin,-(sp)
						move.w		#SETICONPRG,-(sp)
						trap		#SYSTEM
						add.l		#8,sp
						
						EXEC_PRG	.ExitNoError

						move.w		#NoPrgEnd-NoPrgBegin,-(sp)
						move.l		#NoPrgBegin,-(sp)
						move.w		#SETICONPRG,-(sp)
						trap		#SYSTEM
						add.l		#8,sp

;	program with a correctly end

.ExitNoError:
						rts

;	error during the program transfer

.ExitWithError:
						rts

; ---------------------------------------------------------------------------
;		void cdecl RunExe(void)
;
;		This function is called when the HOST computer ask to the jaguar to
;		run a program at a specific address.
;
;		SPECIAL PARAMETERS:	Called by handleServices()
;				d0	=	CODE x 4 received
;				a1	=	Address of DATA block received
;				a2	=	Address of FRAME'sinformations received (SPID, TPID, etc...)
; ---------------------------------------------------------------------------


RunExe:

L_OFF_ADDRTARGET		set         0
L_OFF_ADDRSTART	 		set         4
L_OFF_LENGHT			set         8

L_OFF_TPID				set			8											;	source PID will be our target PID

;	First we send a ACK frame to accept the transfer

						move.l		a1,-(sp)									;	no data
                        move.w		#SIZE_JAGPROGRAM,-(sp) 						;	lenght
                        move.w		#FRAME_PRGRUN,-(sp)  	                    ;	Frame's CODE
                        move.l		L_OFF_TPID(a2),-(sp)						;	Target PID is the source PID from host
                        move.l		#KERNEL_PID,-(sp)							;	Source PID is the kernel itself
		   				jsr			SendFrame
                        add.l		#16,sp

;	Save Status Program Word and execute

						move.w		#RunningEnd-RunningBegin,-(sp)
						move.l		#RunningBegin,-(sp)
						move.w		#SETICONPRG,-(sp)
						trap		#SYSTEM
						add.l		#8,sp
						
						EXEC_PRG	.ExitNoError
						
						move.w		#NoPrgEnd-NoPrgBegin,-(sp)
						move.l		#NoPrgBegin,-(sp)
						move.w		#SETICONPRG,-(sp)
						trap		#SYSTEM
						add.l		#8,sp
						
;	program with a correctly end

.ExitNoError:			
						rts

; ---------------------------------------------------------------------------
						