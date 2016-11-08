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


						XREF		GetFrame
						XREF		SendFrame

; ---------------------------------------------------------------------------
;	Export symboles

						XDEF		GetBinaries
                        XDEF		SendBinaries
						;XDEF		GetFrame
						;XDEF		SendFrame

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


; ---------------------------------------------------------------------------
;		Int CheckSum(a1 = Data buffer Address)
;
;		Compute the checksum and return it in d0 register.
; ---------------------------------------------------------------------------

CheckSum:			   	move.w		#'C'<<8,d0
                        move.b      #'K',d0
						rts

; ---------------------------------------------------------------------------
;		long GetFrame(long *SourcePID, long *TargetPID, int *Code, int *Lenght, void *Buffer);
;
;		Get a frame from the host computer.
; ---------------------------------------------------------------------------

N_REGS_GT				set			4												;	number of reg. saved
OFF_CHECKSUM           	set			12												;	into the reserved location

OFF_SPID				set			04+N_REGS_GT*4
OFF_TPID				set			08+N_REGS_GT*4
OFF_CODE				set			12+N_REGS_GT*4
OFF_LENGETBUFFER		set			16+N_REGS_GT*4
OFF_GETBUFFER			set			20+N_REGS_GT*4

L_OFF_SPID				set			0
L_OFF_TPID				set			4
L_OFF_CODE				set			8
L_OFF_LENGETBUFFER		set 		10
L_OFF_CHECKSUM			set			12


GetFra:					movem.l		d1-d2/a0-a1,-(sp)

						move.l      #HEADER_FRAME_LEN_GET,d1						;	number of bytes awaiting
						sub.l		d1,sp     										;	reserve a header's location
                        movea.l		sp,a0											;	a0 = start of location
                        movea.l		sp,a1											;	a0 = start of location too

;	Au moins une premiere partie arrivee de la trame

   						move.w		d1,-(sp)
						move.l		a1,-(sp)
                        jsr			GetPack
                        addq.l		#6,sp

                        tst.l		d0												;	number = 0 ?
                        beq			.ErrNoFrame										;	no datas available

                        sub.l		d0,d1											;	d1 <= d1 - d0
                        add.l		d0,a1											;	a1 <= a1 + d0

;	handle frame header reception

.GetHeader:             tst.l		d1												;	number = number expected
                        beq.s		.ProcessData                                    ;	go to get datas

						move.w		d1,-(sp)
						move.l		a1,-(sp)
                        jsr			GetPack
                        addq.l		#6,sp

                        sub.l		d0,d1											;	d1 <= d1 - d0
                        add.l		d0,a1											;	a1 <= a1 + d0

						bra.s       .GetHeader

;	handle datas reception

.ProcessData:          
						add.l       #HEADER_FRAME_LEN_GET,sp

						move.l      OFF_CODE(sp),a1
                        move.w		L_OFF_CODE(a0),(a1)

						move.l      OFF_TPID(sp),a1
                        move.l      L_OFF_TPID(a0),(a1)

						move.l      OFF_SPID(sp),a1
                        move.l      L_OFF_SPID(a0),(a1)

						move.l      OFF_LENGETBUFFER(sp),a1
                        move.w		L_OFF_LENGETBUFFER(a0),(a1)

                        move.w		(a1),d1											;	keep the data lenght
                        move.w		L_OFF_CHECKSUM(a0),d2       					;	keep the checksum received

						move.l      OFF_GETBUFFER(sp),a0   							;	keep the data buffer
                        move.l		a0,a1											;	keep original address
						
.GetData:				tst.l		d1                                              ;	while d1 > 0
                        ble.s		.End

                        move.w		d1,-(sp)
                        move.l		a0,-(sp)
                        jsr			GetPack
                        addq.l		#6,sp

                        sub.l		d0,d1											;	substract number received
                        add.l		d0,a0											;	adjust buffer address

                        bra.s		.GetData

;	Now verify the checksum and it's all done:	d0 = checksum
;											  	d2 = checksum received
;                                               a1 = data buffer address

.End:                  
						  jsr			CheckSum
						cmp.w		d0,d2
                        bne.s		.ErrCheckSum

.EndNoError:			movem.l		(sp)+,d1-d2/a0-a1
						move.l		#TRUE,d0
						rts

;	End with error because no data header available

.ErrNoFrame:            add.l       #HEADER_FRAME_LEN_GET,sp
						movem.l		(sp)+,d1-d2/a0-a1
						move.l		#ERR_NO_FRAME,d0
						rts

;	End with error because invalid checksum

.ErrCheckSum:			add.l       #HEADER_FRAME_LEN_GET,sp
						movem.l		(sp)+,d1-d2/a0-a1
						move.l		#ERR_CHEKSUM,d0
						rts

; ---------------------------------------------------------------------------
;		int cdecl SendFrame(long SourcePID, long TargetPID, int Code, int Lenght, void *Buffer);
;
;		Send a frame to the host computer or jaguar.
; ---------------------------------------------------------------------------

N_REGS_ST				set			3												;	number of reg. saved

OFF_SPIDTRAMESEND		set			04+N_REGS_ST*4
OFF_TPIDTRAMESEND		set			08+N_REGS_ST*4
OFF_CODETRAMESEND		set			12+N_REGS_ST*4
OFF_LENTRAMESEND		set			14+N_REGS_ST*4
OFF_BUFFERTRAMESEND		set			16+N_REGS_ST*4

SendFra:				movem.l		d1/a0-a1,-(sp)

						
;	Compute the checksum:	Currently no checksum computed, we just put the 'CK' text.

						move.w		#'C'<<8,d0
                        move.b      #'K',d0

;	Build the frame's header:	we save the data buffer adress to use this location for the checksum !!

						movea.l		OFF_BUFFERTRAMESEND(sp),a1						;	save the buffer address
						move.w		d0,OFF_BUFFERTRAMESEND(sp)						;	push the checksum		

;	Now we have from the stack a complete frame's header ready to send

						move.w		#HEADER_FRAME_LEN_SEND,-(sp)					;	block's lenght to send

						movea.l		sp,a0											;	compute the block address
						adda.l		#N_REGS_ST*4+4+2,a0 								;	+2 cause HEADER_FRAME_LEN pushed
						move.l		a0,-(sp)										;	a0 is the header address !

						jsr			SendPack
						addq.l		#6,sp

						tst.l		d0
						beq.s		.EndWithError
				
;	Restore the data buffer address, destroyed before with the checksum

						move.l		a1,OFF_BUFFERTRAMESEND(sp)						;	save the buffer address
					
;	Now we can send the DATA buffer


						move.w		OFF_LENTRAMESEND(sp),-(sp)						;	push block's lenght
						move.l		a1,-(sp)										;	push data address
						jsr			SendPack
						addq.l		#6,sp

						tst.l		d0						
						beq.s		.EndWithError
	
.EndNoError:			movem.l		(sp)+,d1/a0-a1
						move.l		#TRUE,d0
						rts

.EndWithError:			movem.l		(sp)+,d1/a0-a1
						move.l		#FALSE,d0
						rts

; ---------------------------------------------------------------------------

PRINTF:					movem.l		d0-d7/a0-a6,-(sp)

						move.l		d0,-(sp)
                        move.w		#9,-(sp)
                        trap		#1
                        addq.l		#6,sp

						movem.l		(sp)+,d0-d7/a0-a6
                        rts

						END