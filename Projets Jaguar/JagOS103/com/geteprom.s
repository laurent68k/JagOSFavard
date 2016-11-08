; ---------------------------------------------------------------------------

						XDEF		GetEEPROM

                        TEXT
                        EVEN

; ---------------------------------------------------------------------------
;		void cdecl JagInquire(void)
;
;		Send to the host computer the EEPROM's datas
;
;		SPECIAL PARAMETERS:	Called by handleServices()
;				d0	=	CODE x 4 received
;				a1	=	Address of DATA block received
;				a2	=	Address of FRAME'sinformations received (SPID, TPID, etc...)
; ---------------------------------------------------------------------------

SIZE_EPROM				set			128
ADR_START				set			0

LOCAL					set			SIZE_EPROM+4

GetEEPROM:				sub.l		#LOCAL,sp
						move.l		sp,a0
						
						move.w		#SIZE_EPROM,-(sp)									; Bytes number to read
						move.w		#ADR_START,-(sp)									; Start address EEPROM							
						move.l		a0,-(sp)									; Address to store the bytes							
						move.w		#READEPROM,-(sp)
						trap		#SYSTEM
						add.l		#10,sp

						move.l		a0,-(sp)										;	Identification struct address
                        move.w		#SIZE_EPROM,-(sp)									;	Identification struct lenght
                        move.w		#FRAME_EEPROMGET,-(sp)                      ;	Frame's CODE
                        move.l		#TOOLBOX_PID,-(sp)							;	Target PID
                        move.l		#KERNEL_PID,-(sp)							;	Source PID, the KERNEL
		 				jsr			SendFrame
                        add.l		#16,sp

						add.l		#LOCAL,sp
                        rts

; ---------------------------------------------------------------------------


