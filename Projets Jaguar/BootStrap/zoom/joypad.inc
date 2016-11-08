;	------------------------------------------------------------------------------
;	READJOYPADS   
;
;	Given:				Nothing
;	Returns:			JoyEdge, JoyCur modified
;	Register Usage:		All Registers Preserved
;	Externals:			JoyEdge, JoyCur
;

JOYSTICK			EQU			$F14000 		; TOM Internal Register Base

ReadJoyPads:

;	------------------------------------------------------------------------------
;	scan for player 1

					movem.l		d0-d2,-(sp)

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
					
					move.l		JoyCur,d0			; old JoyCur needed for determining the new JoyEdge
					move.l		d1,JoyCur			; Current joypad reading stored into JoyCur
					eor.l		d1,d0
					and.l		d1,d0
					move.l		d0,JoyEdge			;joypad, buttons, keys that were just pressed
					
;	------------------------------------------------------------------------------
;	scan for player 2

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
					eor.l		d2,d1				; d1 = xxAPxxBx RLDU741* xxCxxxOx 2580369# <== now inputs active high
					
					move.l		JoyCur+4,d0			; old JoyCur needed for determining the new JoyEdge
					move.l		d1,JoyCur+4			; Current joypad reading stored into JoyCur
					eor.l		d1,d0
					and.l		d1,d0
					move.l		d0,JoyEdge+4		;joypad, buttons, keys that were just pressed
					
					movem.l		(sp)+,d0-d2
					rts
