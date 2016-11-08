;	------------------------------------------------------------------------------
;	VIDEO.INC - Revision 1.00 - JAGUAR's Video routines.
;
;	Date:		06 october 1997
;	Revision:	09 December 1997
;
;	Funship - Atari jaguar (c) France - 1997
;	------------------------------------------------------------------------------

;	------------------------------------------------------------------------------
;	INITSIZEMEMORY:			Memroy size Initialization 2 or 4 Mb
;	------------------------------------------------------------------------------
;	Version 1.0 - 09 December 1997 - FunShip
;
;	Given:					no parameters
;	Returns:				Nothing
;	Register Usage:			All Registers Preserved
;	Externals:				Nothings
;	------------------------------------------------------------------------------

InitSizeMemory:				movem.l		d0-d1,-(sp)

							move.w      #$35DD,d0					; default value for 4MB
							move.w      CONFIG,d1
							btst        #SIZERAM,d1					; bit set for 4MB ?
							beq.s       _4MB
							move.w      #$35CC,d0					; value for 2MB

_4MB:						move.w      d0,MEMCON2					; set value 
							move.w      #8,CHRO_CLK					; Default value

							movem.l		(sp)+,d0-d1
							rts

;	------------------------------------------------------------------------------
							