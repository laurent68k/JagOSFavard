; ---------------------------------------------------------------------------

						XDEF		JagInquire

                        TEXT
                        EVEN

; ---------------------------------------------------------------------------
;		void cdecl JagInquire(void)
;
;		Send to the host computer an identification structure about the Hardware
;		and Operating System.
;
;		SPECIAL PARAMETERS:	Called by handleServices()
;				d0	=	CODE x 4 received
;				a1	=	Address of DATA block received
;				a2	=	Address of FRAME'sinformations received (SPID, TPID, etc...)
; ---------------------------------------------------------------------------

JagInquire:				pea.l		JagStruct									;	Identification struct address
                        move.w		#SIZE_JAGID,-(sp)							;	Identification struct lenght
                        move.w		#FRAME_INQUIRE,-(sp)                        ;	Frame's CODE
                        move.l		#TOOLBOX_PID,-(sp)							;	Target PID
                        move.l		#KERNEL_PID,-(sp)							;	Source PID, the KERNEL
		 				jsr			SendFrame
                        add.l		#16,sp

                        rts

; ---------------------------------------------------------------------------


