;	------------------------------------------------------------------------------
;	GET_JOYPAD:	Read the data from the joypad 1 and 2.
;	------------------------------------------------------------------------------
;	Given:				Nothing
;	Returns:			JoyEdge, JoyCur modified
;	Register Usage:		All Registers Preserved
;	Externals:			JoyEdge, JoyCur
;	------------------------------------------------------------------------------

; ---------------------------------------------------------------------------
;		void cdecl Disable(void *ObjectAddress)
; ---------------------------------------------------------------------------
;		Change an image to an disable image.
; ---------------------------------------------------------------------------
;	Version 1.0 - 05 April 1999 - FunShip
;
;	Given:				8(sp)	=	Two long's address for JoyPadState
;						4(sp)	=	Two long's address for JoypadEdge
;	Return:				Always TRUE
;
;	FONCTION:	Number 6
;
;	WARNINGS:	don't foget to restore a0, a1 from the stack
;       a0 = Address of this routine
;		a1 = Address of input parameters
;		d0 = will contain return value (as we want)
; ---------------------------------------------------------------------------

JOYSTATE_OFFSET				set		6
JOYEDGE_OFFSET				set		2

JOYPAD1						set		0
JOYPAD2						set		4

ReadJoyPads:				move.l		JOYEDGE_OFFSET(a1),a0
							move.l		JOYSTATE_OFFSET(a1),a1

;	------------------------------------------------------------------------------
;	scan for player 1


							move.l		#$f0fffffc,d1		; d1 = Joypad data mask
							moveq.l		#-1,d2				; d2 = Cumulative joypad reading
					
							move.w		#$81fe,JOYSTICK
							move.l		JOYSTICK,d0			; Read joypad, pause button, A button
							or.l		d1,d0				; Mask off unused bits
							ror.l		#4,d0
							and.l		d0,d2				; d2 = xxAPxxxx RLDUxxxx xxxxxxxx xxxxxxxx
							move.w		#$81fd,JOYSTICK
							move.l		JOYSTICK,d0			; Read *741 keys, B button
							or.l		d1,d0				; Mask off unused bits
							ror.l		#8,d0
							and.l		d0,d2				; d2 = xxAPxxBx RLDU741* xxxxxxxx xxxxxxxx
							move.w		#$81fb,JOYSTICK
							move.l		JOYSTICK,d0			; Read 2580 keys, C button
							or.l		d1,d0				; Mask off unused bits
							rol.l		#6,d0
							rol.l		#6,d0
							and.l		d0,d2				; d2 = xxAPxxBx RLDU741* xxCxxxxx 2580xxxx
							move.w		#$81f7,JOYSTICK
							move.l		JOYSTICK,d0			; Read 369# keys, Option button
							or.l		d1,d0				; Mask off unused bits
							rol.l		#8,d0
							and.l		d0,d2				; d2 = xxAPxxBx RLDU741* xxCxxxOx 2580369# <== inputs active low
					
							moveq.l		#-1,d1
							eor.l		d2,d1				; d1 = xxAPxxBx RLDU741* xxCxxxOx 2580369# <== now inputs active high
					
							move.l		JOYPAD1(a1),d0			; old JoyCur needed for determining the new JoyEdge
							move.l		d1,JOYPAD1(a1)			; Current joypad reading stored into JoyCur
							eor.l		d1,d0
							and.l		d1,d0
							move.l		d0,JOYPAD1(a0)			;joypad, buttons, keys that were just pressed
					
;	------------------------------------------------------------------------------
;	scan for player 2

.Scan2:						tst.w		_LockJoyPad2			; check if joypad #2 is locked
							bne			.NoScan					; yes, no scan
							
							move.l		#$0ffffff3,d1		; d1 = Joypad data mask
							moveq.l		#-1,d2				; d2 = Cumulative joypad reading
							
							move.w		#$817f,JOYSTICK
							move.l		JOYSTICK,d0			; Read joypad, pause button, A button
							or.l		d1,d0				; Mask off unused bits
							rol.b		#2,d0				; note the size of rol
							ror.l		#8,d0
							and.l		d0,d2				; d2 = xxAPxxxx RLDUxxxx xxxxxxxx xxxxxxxx
							move.w		#$81bf,JOYSTICK
							move.l		JOYSTICK,d0			; Read *741 keys, B button
							or.l		d1,d0				; Mask off unused bits
							rol.b		#2,d0				; note the size of rol
							ror.l		#8,d0
							ror.l		#4,d0
							and.l		d0,d2				; d2 = xxAPxxBx RLDU741* xxxxxxxx xxxxxxxx
							move.w		#$81df,JOYSTICK
							move.l		JOYSTICK,d0			; Read 2580 keys, C button
							or.l		d1,d0				; Mask off unused bits
							rol.b		#2,d0				; note the size of rol
							rol.l		#8,d0
							and.l		d0,d2				; d2 = xxAPxxBx RLDU741* xxCxxxxx 2580xxxx
							move.w		#$81ef,JOYSTICK
							move.l		JOYSTICK,d0			; Read 369# keys, Option button
							or.l		d1,d0				; Mask off unused bits
							rol.b		#2,d0				; note the size of rol
							rol.l		#4,d0
							and.l		d0,d2				; d2 = xxAPxxBx RLDU741* xxCxxxOx 2580369# <== inputs active low
						
							moveq.l		#-1,d1
							eor.l		d2,d1					; d1 = xxAPxxBx RLDU741* xxCxxxOx 2580369# <== now inputs active high
					
							move.l		JOYPAD2(a1),d0			; old JoyCur needed for determining the new JoyEdge
							move.l		d1,JOYPAD2(a1)			; Current joypad reading stored into JoyCur
							eor.l		d1,d0
							and.l		d1,d0
							move.l		d0,JOYPAD2(a0)			;joypad, buttons, keys that were just pressed
					
.End:						move.l		#TRUE,d0
							movem.l		(sp)+,d1-d7/a0-a6
                        	RTE

.NoScan:					move.l		#0,JOYPAD2(a1)
							move.l		#0,JOYPAD2(a0)
							move.l		#TRUE,d0
							movem.l		(sp)+,d1-d7/a0-a6
                        	RTE
                        	

;	------------------------------------------------------------------------------

