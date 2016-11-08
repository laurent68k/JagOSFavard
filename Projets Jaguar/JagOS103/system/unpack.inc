;	------------------------------------------------------------------------------
;	UNPACK.INC - Revision 3.00 - Simple Unpack Routine
;
;	Date:		06 April 1998
;	Revision:	06 April 1998
;
;	Funship - Atari jaguar (c) France - 1997
;	------------------------------------------------------------------------------

;	------------------------------------------------------------------------------
;	void cdecl Unpack( void *AddressUnpack, unsigned long Lenght, void *AddressPack)
;	------------------------------------------------------------------------------
;	Version 1.0 - 07 April 1998 - FunShip
;
;	Given:					12(sp) = Source Address of Datas packed
;							8(sp) = Lenght in byte of datas packed
;							4(sp) = Destination Address where unpack
;	FONCTION:	Number 3
;
;	This function unpack the simple Cry Compressed Image format.
;
;	WARNINGS:	don't foget to restore d1-d7/a0-a6 from the stack
;       a0 = Address of this routine
;		a1 = Address of input parameters
;		d0 = will contain return value (as we want)
; ---------------------------------------------------------------------------

OFF_DATAPACK				set		10
OFF_LENGHT					set		6
OFF_DATAUNPACK				set		2
OFF_OPCODE					set		0


Unpack:						move.l		OFF_DATAUNPACK(a1),a0						;	where to unpack
							move.l		OFF_LENGHT(a1),d2							;	lenght
							move.l		OFF_DATAPACK(a1),a1						;	datas packed

							lsr.l		#2,d2										;	number of long to process (n bytes / 4)

.loop:						move.l		(a1)+,d0									;	d0.l <= <number,data>
							move.l		d0,d1										;	d1.l <= <number,data>
							lsr.l		#8,d1										
							lsr.l		#8,d1										;	d1.w <= <number> only
							sub.w		#1,d1

.write:						move.w		d0,(a0)+
							dbf			d1,.write

							dbf			d2,.loop

.end:						movem.l		(sp)+,d1-d7/a0-a6
                        	RTE
							