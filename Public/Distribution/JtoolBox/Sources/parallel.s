; ---------------------------------------------------------------------------
;	FAVARD Laurent copyright (c) 16 March 1999
;	Jaguar development kit project
;	JagStudio and JagOS are software part of this JDK
; ---------------------------------------------------------------------------

						XDEF		_initC
						XDEF		_getbusyC
												
                        TEXT
                        EVEN

; ---------------------------------------------------------------------------
;		void cdecl _initC(void)
;
;		Centronics port Initialization: Disable MFP-ST interrupt, pending IT
;		for busy line and set busy as input.(We want to be sure !)
; ---------------------------------------------------------------------------

_initC:					movem.l		d0/a0,-(sp)

						lea			$FFFA09,a0
						move.b		(a0),d0						;	no interrupt with Busy
						andi		#$FE,d0
						move.b		d0,(a0)

						lea			$FFFA0D,a0
						move.b		(a0),d0						;	no interrupt pending with Busy
						andi		#$FE,d0
						move.b		d0,(a0)

						lea			$FFFA05,a0
						move.b		(a0),d0						;	Busy as input line
						andi		#$FE,d0
						move.b		d0,(a0)

						movem.l		(sp)+,d0/a0
						rts

; ---------------------------------------------------------------------------
;		int cdecl _getbusyC(void)
;
;		Get current value of GPIP (MFP-ST) register to check busy line status
; ---------------------------------------------------------------------------

_getbusyC:				move.w		$FFFA00,d0
						andi.w		#$FF,d0
						rts
