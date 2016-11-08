;
;
;	News AES definitions for version 3.3, 4.0 and 4.1
;	Assembly sub-routine to call aes
;
;		FunShip, (c) FRANCE 1994 - ATARI Falcon030
;
;		Version 1.00 from 5 November.
;		Turbo-C and Pure-C language
;
;
;

		XDEF	XAes
		

GEM		equ	2


; Sub-routine to call Aes

XAes:
		movem.l		d0-a6,-(sp)
		move.l		#200,d0
		move.l		a0,d1
		trap		#GEM
		movem.l		(sp)+,d0-a6
		rts
