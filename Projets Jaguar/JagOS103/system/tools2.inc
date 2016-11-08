							TEXT
							EVEN

;	------------------------------------------------------------------------------
;	long cdecl VideoRate( void )
;	------------------------------------------------------------------------------
;	Version 1.0 - 03 Jully 1998 - FunShip
;
;	Given:			NONE
;
;	FONCTION:	Number 14
;
;	This function return the video rate frequency 50 or 60 Hz
;
;	WARNINGS:	don't foget to restore d1-d7/a0-a6 from the stack
;       a0 = Address of this routine
;		a1 = Address of input parameters
;		d0 = will contain return value (as we want)
; ---------------------------------------------------------------------------

VideoRate:					move.l		#VIDEO_NTSC,d0				; d0 <= NTSC 60 Hz
							move.w		CONFIG,d1					; get current mode
							
							btst		#PALFLAG,d1					; if flag set you have a NTSC else it's a PAL Jag
							bne.s		.End

							move.l		#VIDEO_PAL,d0				; d0 <= PAL 50 Hz

.End:						movem.l		(sp)+,d1-d7/a0-a6
							RTE
							

;	------------------------------------------------------------------------------
;	long cdecl MemSize( void )
;	------------------------------------------------------------------------------
;	Version 1.0 - 03 Jully 1998 - FunShip
;
;	Given:			NONE
;
;	FONCTION:	Number 15
;
;	This function return the memory size 2 or 4 Mb
;
;	WARNINGS:	don't foget to restore d1-d7/a0-a6 from the stack
;       a0 = Address of this routine
;		a1 = Address of input parameters
;		d0 = will contain return value (as we want)
; ---------------------------------------------------------------------------

MemSize:					move.l	    #RAM_4MB,d0				; value for 4MB
							move.w      CONFIG,d1
							btst        #SIZERAM,d1				; if bit set so 4MB ?
							bne.s       .End

							move.l		#RAM_2MB,d0				; value for 4MB

.End:						movem.l		(sp)+,d1-d7/a0-a6
							RTE


;	------------------------------------------------------------------------------
;	long cdecl HostConnected( void )
;	------------------------------------------------------------------------------
;	Version 1.0 - 03 Jully 1998 - FunShip
;
;	Given:		NONE
;
;	FONCTION:	Number 16
;
;	This function return true if a host computer is connected through the extended board.
;
;	WARNINGS:	don't foget to restore d1-d7/a0-a6 from the stack
;       a0 = Address of this routine
;		a1 = Address of input parameters
;		d0 = will contain return value (as we want)
; ---------------------------------------------------------------------------

HostConnected:				move.l		#TRUE,d0				;	FIXME

							btst.b		#LINK_PRESENT,StatusBuffer				; Host computer ready ( = 0 ) ?
							beq.s		.End

							move.l		#FALSE,d0

.End:						movem.l		(sp)+,d1-d7/a0-a6
							RTE

;	------------------------------------------------------------------------------
;	long cdecl GetVersion( void )
;	------------------------------------------------------------------------------
;	Version 1.0 - 03 Jully 1998 - FunShip
;
;	Given:		NONE
;
;	FONCTION:	Number 17
;
;	This function return an address to a structure whish contains Soft and Hard system versions in d0.l
;
;	WARNINGS:	don't foget to restore d1-d7/a0-a6 from the stack
;       a0 = Address of this routine
;		a1 = Address of input parameters
;		d0 = will contain return value (as we want)
; ---------------------------------------------------------------------------

GetVersion:					move.l		#JagStruct,d0

.End:						movem.l		(sp)+,d1-d7/a0-a6
							RTE

