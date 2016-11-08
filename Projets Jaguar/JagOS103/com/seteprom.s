; ---------------------------------------------------------------------------

						XDEF		SetEEPROM

                        TEXT
                        EVEN

; ---------------------------------------------------------------------------
;		void cdecl SetEEPOM(char *chBuffer)
;
;		Write chBuffer into the EEPROM
;
;		SPECIAL PARAMETERS:	Called by handleServices()
;				d0	=	CODE x 4 received
;				a1	=	Address of DATA block received
;				a2	=	Address of FRAME'sinformations received (SPID, TPID, etc...)
; ---------------------------------------------------------------------------

SIZE_EPROM				set			128
ADR_START				set			0

SetEEPROM:				move.w		#SIZE_EPROM,-(sp)									; Bytes number to read
						move.w		#ADR_START,-(sp)									; Start address EEPROM							
						move.l		a1,-(sp)									; Address to store the bytes							
						move.w		#WRITEEPROM,-(sp)
						trap		#SYSTEM
						add.l		#10,sp

						move.l		#NULL,-(sp)										;	Identification struct address
                        move.w		#0,-(sp)									;	Identification struct lenght
                        move.w		#FRAME_EEPROMSET,-(sp)                      ;	Frame's CODE
                        move.l		#TOOLBOX_PID,-(sp)							;	Target PID
                        move.l		#KERNEL_PID,-(sp)							;	Source PID, the KERNEL
		 				jsr			SendFrame
                        add.l		#16,sp

                        rts

; ---------------------------------------------------------------------------


