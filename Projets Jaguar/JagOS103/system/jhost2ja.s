
  						XDEF		JHost2Jag

						TEXT
						EVEN

; ---------------------------------------------------------------------------
;		void cdecl JHost2Jag(long TargetPID, void *Address, unsigned long Lenght)
;
;		This function is called when the JAGUAR want to receive a memory block
;		 from the HOST
;
;	FONCTION:	Number 1
;
;	WARNINGS:	don't foget to restore a0, a1 from the stack
;       a0 = Address of this routine
;		a1 = Address of input parameters
;		d0 = will contain return value (as we want)
; ---------------------------------------------------------------------------

OFF_LENGHT				set         10
OFF_ADDRSTART			set         6
OFF_TPID				set			2
OFF_OPCODE				set			0

L_OFF_BLENGETBUFFER		set 		0
L_OFF_BCODE				set			2+L_OFF_BLENGETBUFFER
L_OFF_BTPID				set			2+L_OFF_BCODE
L_OFF_BSPID				set			4+L_OFF_BTPID

SB_LOCAL				set			12


JHost2Jag:	
						sub.l		#SIZE_JAGMEMDESC,sp 						;	reserve a location for a TJagMemDesc
						move.l		sp,a0
                        move.l      OFF_ADDRSTART(a1),(sp)+						;	write address of data to send
						move.l      OFF_LENGHT(a1),(sp)							;	write lenght to send
						move.l		a0,sp

;	Send a frame to initiate this service to the host

						move.l		a0,-(sp)									;	Address of TJagMemDesc
                        move.w		#SIZE_JAGMEMDESC,-(sp)						;	Lenght of TJagMemDesc
                        move.w		#FRAME_MEMGET,-(sp) 	                    ;	Frame's CODE
                        move.l		OFF_TPID(a1),-(sp)   						;	Target PID is the source PID from host
                        move.l		#KERNEL_PID,-(sp)							;	Source PID is the kernel itself
		   				jsr			SendFrame
                        add.l		#16,sp
                        add.l       #SIZE_JAGMEMDESC,sp							;	remove the location (TJagMemDesc)

;	Reserve a small location to get the ACK/NACK response

						sub.l		#SB_LOCAL,sp
						move.l		sp,a2

;	Wait the ACK/NACK response

.Wait:					move.l		#NULL,-(sp)										;	BUFFER parameter
                        move.l		a2,-(sp)                     					;	LENGHT parameter

                        addq.l		#2,a2
                        move.l		a2,-(sp)                     					;	CODE parameter

                        addq.l		#2,a2
						move.l		a2,-(sp)                     					;	TARGET PID parameter

                        addq.l		#4,a2
						move.l		a2,-(sp)                     					;   SOURCE PID parameter

.WaitFrame:             jsr			GetFrame										;	get a frame
                        cmpi.w		#ERR_NO_FRAME,d0
						beq.s		.WaitFrame

						add.l		#20,sp											;	remove parameters (5*4bytes)
						sub.l		#8,a2											;	first location

                        cmpi.w		#FRAME_NACK,L_OFF_BCODE(a2)						;	Code = ACK ?
                        beq.s       .NackRecu

;	Remove the small location and start the binaries transfer

						add.l		#SB_LOCAL,sp

						move.l		OFF_ADDRSTART(a1),-(sp)
						move.l		OFF_LENGHT(a1),-(sp)
						move.l		OFF_TPID(a1),-(sp)
						move.l		#KERNEL_PID,-(sp)					
						jsr			GetBinaries
						add.l		#16,sp

.Exit:        			movem.l		(sp)+,d1-d7/a0-a6
                        RTE

.NackRecu:				add.l		#SB_LOCAL,sp
						movem.l		(sp)+,d1-d7/a0-a6
                        RTE

; ---------------------------------------------------------------------------

