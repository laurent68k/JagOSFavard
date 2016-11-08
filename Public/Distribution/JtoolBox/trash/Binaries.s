;	------------------------------------------------------------------------------
;	Project JAGUAR:	Frames Protocoles Communications.
;
;	THIS FILE IS USED BY THE JAGUAR KERNEL AND THE CLIENT ST TOOLBOX, BECAUSE
;	THE ROUTINES ARE EXACTLY THE SAME AT THE PROTOCOLE LEVEL.
;
;	File:		TramHigh.S
;	Author:		FAVARD Laurent
;	Date:		20 January 1998
;	Release:	01 April 1998
;	Language:	HISOFT DEVPAC 3-680x0
;
;	Funship - Atari Jaguar France / Copyright 1997-1998
;	------------------------------------------------------------------------------

				include	"trame.inc"
				
TRUE		equ		1
FALSE		equ		0

;	------------------------------------------------------------------------------

						XREF		GetFrame
						XREF		SendFrame

; ---------------------------------------------------------------------------
;	Export symboles

						XDEF		GetBinaries
                        XDEF		SendBinaries

						TEXT
                        
; ---------------------------------------------------------------------------
;		long cdecl GetBinaries(long SourcePID, long TargetPID, unsigned long Lenght, void *MemoryBlock)
;
;		Get a memory block FROM the host computer TO the jaguar memory area
;		specified.
; ---------------------------------------------------------------------------

N_REGS_GB				set			6

OFF_GETBINBLK			set			16+N_REGS_GB*4
OFF_GETBINLEN			set			12+N_REGS_GB*4
OFF_GETBINTPID         	set			08+N_REGS_GB*4
OFF_GETBINSPID			set			04+N_REGS_GB*4

GB_LOCAL				set			12

L_OFF_BLENGETBUFFER		set 		0
L_OFF_BCODE				set			2+L_OFF_BLENGETBUFFER
L_OFF_BTPID				set			2+L_OFF_BCODE
L_OFF_BSPID				set			4+L_OFF_BTPID


GetBinaries:            movem.l		d1-d4/a0-a1,-(sp)
                        movea.l		OFF_GETBINBLK(sp),a0  							;	where store binaries datas
						move.l      OFF_GETBINSPID(sp),d1							;	source (host) PID
						move.l      OFF_GETBINTPID(sp),d2							;	target (jaguar) PID
                        move.l      OFF_GETBINLEN(sp),d3                            ;	lenght to receive

;	Reserve a small location for SPID, TPID, Code and Lenght
;	int GetFrame(long *SourcePID, long *TargetPID, int *Code, int *Lenght, void *Buffer);

						sub.l		#GB_LOCAL,sp							   		;	reserve a location
						move.l		sp,a1											;	a1 = start of reserved location
                        clr.l		d4												;	number received

;	Wait a frame

.WaitFrame:				cmp.l		d4,d3										 	;	while d4 <> d3 get datas
						beq       	.EndNoError										;	else end

						move.l		a0,-(sp)										;	BUFFER parameter

                        move.l		a1,-(sp)                     					;	LENGHT parameter

                        addq.l		#2,a1
                        move.l		a1,-(sp)                     					;   CODE parameter

                        addq.l		#2,a1
						move.l		a1,-(sp)                     					;   TARGET PID parameter

                        addq.l		#4,a1
						move.l		a1,-(sp)                     					;   SOURCE PID parameter

						jsr			GetFrame										;	get a frame
                        add.l		#20,sp											;
						sub.l		#8,a1

;	Frame received without any errors

                       	tst.w		d0   											;	d0 > 0 => OK
                        bgt.s		.FrameOK

;	No Frame available, so wait a frame again

                        cmpi.w		#ERR_NO_FRAME,d0
                        beq.s		.WaitFrame

;	Frame received but with a checksum error: We send a error frame to re-received it

                        move.l		#NULL,-(sp)
                        move.w		#0,-(sp)
                        move.w		#FRAME_NACK,-(sp)
                        move.l		d2,-(sp)
                        move.l		d1,-(sp)
			   			jsr			SendFrame
                        add.l		#16,sp

						bra.s		.WaitFrame

;	No problem, next frame if lenght not completed
;	int cdecl SendFrame(long SourcePID, long TargetPID, int Code, int Lenght, void *Buffer);

.FrameOK:               move.w		L_OFF_BLENGETBUFFER(a1),d0				  		;	get new lenght received
                        add.l		d0,d4									 		;	total received + new received
                        add.l		d0,a0									  		;	next location in the buffer

						move.l		#NULL,-(sp)
                        move.w		#0,-(sp)
                        move.w		#FRAME_ACK,-(sp)
                        move.l		d2,-(sp)
                        move.l		d1,-(sp)
						jsr			SendFrame
                        add.l		#16,sp

						bra			.WaitFrame

;	End with no error

.EndNoError:			add.l		#GB_LOCAL,sp					 		 		;	free location
						movem.l		(sp)+,d1-d4/a0-a1
                        move.l		#TRUE,d0
						rts

;	End with error

.EndWithError:			add.l		#GB_LOCAL,sp					 				;	free location
						movem.l		(sp)+,d1-d4/a0-a1
                        move.l		#FALSE,d0
						rts

; ---------------------------------------------------------------------------
;		long cdecl SendBinaries(long SourcePID, long TargetPID, unsigned long Lenght, void *MemoryBlock)
;
;		Send a binarie memory block FROM the jaguar TO the host computer.
; ---------------------------------------------------------------------------

N_REGS_SB				set			6

OFF_SENDBINBLK			set			16+N_REGS_SB*4
OFF_SENDBINLEN			set			12+N_REGS_SB*4
OFF_SENDBINTPID         set			08+N_REGS_SB*4
OFF_SENDBINSPID			set			04+N_REGS_SB*4

SB_LOCAL				set			12

SendBinaries:			movem.l		d1-d4/a0-a1,-(sp)
                        movea.l		OFF_SENDBINBLK(sp),a0  							;	memory block
						move.l      OFF_SENDBINSPID(sp),d1							;	source (jaguar) PID
						move.l      OFF_SENDBINTPID(sp),d2							;	target (host computer) PID
                        move.l      OFF_SENDBINLEN(sp),d3							;	lenght to send

;	Reserve a small location for SPID, TPID, Code and Lenght

						sub.l		#SB_LOCAL,sp									;	reserve a location
						move.l		sp,a1											;	a1 = start of reserved location
                        clr.l		d4
                        
.MainLoop:				cmpi.l		#MAX_LEN_DATAFRAME,d3                           ;	if d3 >= Max
						bge.s		.SendMax										;	send maximal

                        tst.l		d3                                              ;	if d3 = 0
                        beq	       .EndNoError										;	terminate

                        move.l		d3,d4											;
                        bra.s       .Send

.SendMax:				move.l		#MAX_LEN_DATAFRAME,d4

;	Send a maximal block of 64Ko

.Send:                 	move.l		a0,-(sp)
                        move.w		d4,-(sp)
                        move.w		#FRAME_DATA,-(sp)
                        move.l		d2,-(sp)
                        move.l		d1,-(sp)
						jsr			SendFrame
                        add.l		#16,sp

;	Now await the host's answer for accept or not the data frame

						move.l		#NULL,-(sp)										;	BUFFER parameter
                        move.l		a1,-(sp)                     					;	LENGHT parameter

                        addq.l		#2,a1
                        move.l		a1,-(sp)                     					;	CODE parameter

                        addq.l		#2,a1
						move.l		a1,-(sp)                     					;	TARGET PID parameter

                        addq.l		#4,a1
						move.l		a1,-(sp)                     					;   SOURCE PID parameter

.WaitFrame:             jsr			GetFrame										;	get a frame
                        cmpi.w		#ERR_NO_FRAME,d0
						beq.s		.WaitFrame

.FrameOk:               add.l		#20,sp											;	remove parameters (5*4bytes)
						sub.l		#8,a1											;	first location

                        cmpi.w		#FRAME_ACK,L_OFF_BCODE(a1)						;	Code = ACK ?
                        beq.s       .AckRecu

						bra.s       .MainLoop										;	no so re send it

.AckRecu:               sub.l		d4,d3											;	d3 <= d3 - MAX_LEN_DATAFRAME
                        add.l		d4,a0 				                     	    ;	next location
                        bra       .MainLoop

.EndNoError:			add.l		#SB_LOCAL,sp									;	free location
						movem.l		(sp)+,d1-d4/a0-a1
                        move.l		#TRUE,d0
						rts

						END