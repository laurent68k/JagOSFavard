;	------------------------------------------------------------------------------
;	Project JAGUAR:
;	EEProm routines from Atari BootRom
;
;	File:		Eeprom.s
;	Author:		FAVARD Laurent
;	Date:		01 May 1998
;	Release:	01 May 1998
;	Language:	HISOFT DEVPAC 3-680x0
;
;	Funship - Atari Jaguar France / Copyright 1997-1998
;	------------------------------------------------------------------------------

;	I2C-Bus-Adressen

* $F14000.w  - Bit 0 = Datenstrom vom EEPROM ( Hi-Bit zuerst)
* $F14800.w  - lesen - CLK - schreiben => Bit 0 ausgeben und CLK
* $F15000.w  - lesen CS

;	EEPROM-Opcodes

eeREAD				equ		%110000000
eeWRITE				equ		%101000000
eeERASE				equ		%111000000
eeEWEN				equ		%100110000
eeEWDS				equ		%100000000
eeERAL				equ		%100100000
eeWRAL				equ		%100010000
eeCLK				equ		$0800
eeCS				equ		$1000
eeADR				equ		%111111

;	------------------------------------------------------------------------------
;	void WriteEEPROM():
;	------------------------------------------------------------------------------
;	Version 1.0 - 01 May 1998 - FunShip
;
;	Given:					d0.w	=	number of bytes to read
;							d1.w	=	start Eeprom address
;							a0.l	=	source address of bytes
;	Returns:				d4.w < 0 failure
;	Register Usage:			All Registers Preserved
;	Externals:				Nothings
;	------------------------------------------------------------------------------

NUMBER						set			8
ADR_EPROM					set			6
ADR_STORE					set			2

WriteEEPROM:				move.w		NUMBER(a1),d0
							move.w		ADR_EPROM(a1),d1
							move.l		ADR_STORE(a1),a0

.Write:						move.w		d0,-(sp)
			                move.w  	(a0)+,d0
            			    bsr		   	WriteWordEEPROM
			                bmi.s   	exit_WE
            			    addq.w  	#1,d1
			                subq.w  	#2,(sp)
			                bgt.s   	.Write+2
exit_WE:    			    addq.l  	#2,sp
			                tst.w   	d4

.End:						movem.l		(sp)+,d1-d7/a0-a6
							RTE

;	------------------------------------------------------------------------------
;	void ReadEEPROM():
;	------------------------------------------------------------------------------
;	Version 1.0 - 01 May 1998 - FunShip
;
;	Given:					08(sp).w	=	number of bytes to read
;							06(sp).w	=	start Eeprom address
;							02(sp).l	=	destination address to store
;							00(sp).w	=	Code op. of this function
;
;	FONCTION:	Number 12
;
;	This function read the any number of bytes at a start address in serial e2prom
;
;	WARNINGS:	don't foget to restore d1-d7/a0-a6 from the stack
;       a0 = Address of this routine
;		a1 = Address of input parameters
;		d0 = will contain return value (as we want)
;	------------------------------------------------------------------------------

NUMBER						set			8
ADR_EPROM					set			6
ADR_STORE					set			2

ReadEEPROM:					move.w		NUMBER(a1),d0
							move.w		ADR_EPROM(a1),d1
							move.l		ADR_STORE(a1),a0

.Read:						move.w		d0,-(sp)
							bsr.s		ReadWordEEPROM
							move.w		d0,(a0)+
							addq.w		#1,d1
							subq.w		#2,(sp)
							bpl.s		.Read+2
							addq.l		#2,sp
				  
.End:						movem.l		(sp)+,d1-d7/a0-a6
							RTE
                                    
;	------------------------------------------------------------------------------
;	void ClearEEPROM():
;	------------------------------------------------------------------------------
;	Version 1.0 - 01 May 1998 - FunShip
;
;	Given:					00(sp).w	=	Code op. of this function
;
;	FONCTION:	Number 13
;
;	This function clear the serial EPROM
;
;	WARNINGS:	don't foget to restore d1-d7/a0-a6 from the stack
;       a0 = Address of this routine
;		a1 = Address of input parameters
;		d0 = will contain return value (as we want)
;	------------------------------------------------------------------------------

ClearEEPROM:      			lea			$F14000,a0
                		  	tst.w		eeCS(a0)
        		          	move.w		#eeEWEN,d0
                			bsr			SendEEOpCode
        		          	tst.w		eeCS(a0)
		                  	move.w		#eeERAL,d0
                		  	bsr			SendEEOpCode
        		          	tst.w		eeCS(a0)
		                  	move.w		#eeEWDS,d0
		                  	bsr		    SendEEOpCode

.End:						movem.l		(sp)+,d1-d7/a0-a6
							RTE

***************************
* D0=ReadWordEEPROM(D1.w) *
***************************
ReadWordEEPROM:   movem.l d1-d3/a0,-(sp)
                  lea     $F14000,a0
                  tst.w   eeCS(a0)
                  andi.w  #%111111,d1      ; Adress-Bits maskieren
                  ori.w   #eeREAD,d1
                  move.w  d1,d2
                  bsr     SendEEOpCode
                  moveq   #0,d0
                  moveq   #$0F,d3
loop_RWE:         tst.w   eeCLK(a0)
                  nop
                  move.w  (a0),d1
                  lsr.w   #1,d1
                  addx.w  d0,d0
                  nop                      ; warten
                  nop                      ; ...
                  nop
                  nop
                  nop
                  nop
                  dbra    d3,loop_RWE
                  movem.l (sp)+,d1-d3/a0
                  rts
******************************
* WriteWordEEPROM(D1.w,D0.w) *
******************************
WriteWordEEPROM:  movem.l d0-d3/a0,-(sp)
                  lea     $F14000,a0
                  tst.w   eeCS(a0)
                  move.w  #eeEWEN,d2       ;
                  bsr.s   SendEEOpCode
                  tst.w   eeCS(a0)
                  andi.w  #$3F,d1
                  ori.w   #eeWRITE,d1
                  move.w  d1,d2
                  bsr.s   SendEEOpCode
                  move.w  d0,d2
                  bsr.s   SendEEWord       ; Wert senden
                  tst.w   eeCS(a0)
                  nop
                  nop
                  move.w  #1,d2
                  move.w  #$7FFF,d4
wait_WWE:         move.w  (a0),d3
                  and.w   d2,d3
                  bne.s   ok_WWE
                  dbra    d4,wait_WWE
ok_WWE:           move.w  #eeEWDS,d2
                  bsr.s   SendEEOpCode
                  tst.w   eeCS(a0)
                  movem.l (sp)+,d0-d3/a0
                  tst.w   d4
                  rts
*************************
* SendXBit2EEPROM(d2.w) *
*************************
SendEEWord:       rol.w   #1,d2
                  moveq   #$0F,d3
                  bra.s   loop_SAE
SendEEOpCode:     rol.w   #8,d2
                  moveq   #8,d3
loop_SAE:         move.w  d2,eeCLK(a0)
                  nop
                  nop
                  nop
                  nop
                  nop
                  nop
                  rol.w   #1,d2
                  dbra    d3,loop_SAE
                  rts

