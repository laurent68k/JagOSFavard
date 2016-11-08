

; ---------------------------------------------------------------------------
						XDEF		Jag2Host

                        TEXT
                        EVEN

; ---------------------------------------------------------------------------
;		void cdecl Jag2Host(void *Address, unsigned long Lenght)
;
;		This function is called when the Atari computer ask to the jaguar to send
;		a memory block.
;
;		SPECIAL PARAMETERS:	Called by handleServices()
;				d0	=	CODE x 4 received
;				a0	=	address of this routine
;				a1	=	Address of DATA block received
;				a2	=	Address of FRAME'sinformations received (SPID, TPID, etc...)
; ---------------------------------------------------------------------------

Jag2Host:

L_OFF_ADDRSTART			set         0
L_OFF_LENGHT			set         4
L_OFF_TPID				set			8											;	source PID will be our target PID

;	First we send a ACK frame to accept the transfer

						move.l		a1,-(sp)									;	Address of DATA block received
                        move.w		#SIZE_JAGMEMDESC,-(sp)						;	Lenght
                        move.w		#FRAME_MEMGET,-(sp) 	                    ;	Frame's CODE
                        move.l		L_OFF_TPID(a2),-(sp)   					;	Target PID is the source PID from host
                        move.l		#KERNEL_PID,-(sp)							;	Source PID is the kernel itself
		   				jsr			SendFrame
                        add.l		#16,sp

;	Second we send the memory block to the host computer
;	long cdecl SendBinaries(long SourcePID, long TargetPID, unsigned long Lenght, void *MemoryBlock)

						move.l		L_OFF_ADDRSTART(a1),-(sp)					;	Start address
						move.l		L_OFF_LENGHT(a1),-(sp)						;	Lenght to send
                        move.l		L_OFF_TPID(a2),-(sp)						;	Target PID is the source PID from host
                        move.l		#KERNEL_PID,-(sp)							;	Source PID is the kernel itself
						jsr         SendBinaries
                        add.l       #16,sp

						rts

; ---------------------------------------------------------------------------


