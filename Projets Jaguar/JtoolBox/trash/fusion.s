;	------------------------------------------------------------------------------
;	Project JAGUAR:	JAGUAR PROTOCOLE COMMUNICATIONS.
;	HIGH LEVEL JAGUAR PROTOCOLE FROM THE JagOS
;
;	File:		Fusion.s
;	Author:		FAVARD Laurent
;	Date:		20 January 1998
;	Release:	06 June 1998
;	Language:	HISOFT DEVPAC 3-680x0
;
;	Funship - Atari Jaguar France / Copyright 1997-1998
;	------------------------------------------------------------------------------
    
    
TRUE		equ		1
FALSE		equ		0
    
			include	"Trame.inc"    

			XREF	SendPack
			XREF	GetPack
			
			include	"TramHigh.s"

			END
