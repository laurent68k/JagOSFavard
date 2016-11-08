LINK_PRESENT				equ			6
StatusBuffer				equ			$E1F000
ControlBuffer				equ			$E1F800


;	------------------------------------------------------------------------------
;	Init. Counter...
							move.l		#0,d4
							move.w		#0,FujiSens
							move.w		#6,FujiOffset
;	------------------------------------------------------------------------------
;	Initialisation KIT

						
							btst.b		#LINK_PRESENT,StatusBuffer				; Host computer ready ( = 1) ?
							beq.s		Agrandir								; No computer => Object List ok
		
							move.l		#ControlBuffer,a0	 	;	Remise a zero des deux buffers
							move.b		(a0),d0

							jsr			Change					;	Computer OK 

							movea.l		#Message1,a0           	;	Envoyer un message
							bsr			SendString

																																
;	------------------------------------------------------------------------------
Agrandir:					jsr			WaitSync
							cmpi.l		#32,d4
							bgt.s		Attente

							addq.l		#1,d4
							jsr			Zoom

							bra			Agrandir
												
;	------------------------------------------------------------------------------
;	No Host computer => Awaiting a key to know what to do...

Attente:					
							jsr			WaitSync
							jsr			PlayFuji

							jsr			ReadJoyPads
							move.l		JoyCur,d0

ck_a:						btst.l		#FIRE_A,d0								; START cardridge
							beq.s		ck_b
							jmp			JmpCardridge

ck_b:						btst.l		#FIRE_B,d0								; Software RESET
							beq.s		ck_c
							jsr			WaitSync
							jmp			ResetJag						

ck_c:						btst.l		#FIRE_C,d0								; Send string anyway
							beq.s		ck_up
							movea.l		#Message2,a0           					; Send to host
							bsr			SendString

ck_up:						btst.l		#JOY_UP,d0
					;		beq.s		ck_down
					;		addq.w		#1,FujiOffset									; UP => 1 pixels to substract

ck_down:					btst.l		#JOY_DOWN,d0
					;		beq.s		Attente
					;		sub.w		#1,FujiOffset							; DOWN  => 1 pixels to add

					;		andi.w		#$F,FujiOffset
																				
							bra			Attente

;	------------------------------------------------------------------------------
;	Subroutine : Change image to display 'Computer OK'

Change:						movem.l		d0-d2/a0,-(sp)

							lea.l		ReferenceList,a0						; computer ready => change bitmap
							add.l		#16,a0
							move.l		0(a0),d0
							andi.l		#$7FF,d0
							
							move.l		0(a0),d1
							move.l		#11,d2
							lsr.l		d2,d1
							lsl.l		#3,d1
							
							move.l		#9216,d2
							add.l		d2,d1
							lsl.l		#8,d1
							
							or.l		d1,d0
							move.l		d0,0(a0)

							movem.l		(sp)+,d0-d2/a0
							rts

;	------------------------------------------------------------------------------
;	Subroutine to make a Zoom from the jaguar logo object and move fuji

Zoom:						movem.l		d0-d1/a0,-(sp)

							lea.l		ReferenceList,a0

							adda.l		#16+16+16+4,a0
							
							move.l		0(a0),d0
							move.l		d0,d1
							andi.l		#$FFFFFF00,d0
							andi.l		#$FF,d1
							
							; LEFT/RIGHT button => HSCALE						
							addq.l		#1,d1										; Ajoute offset X 
							andi.l		#$FF,d1
							or.l		d1,d0
							move.l		d0,0(a0)

							; UP/DOWN button => VSACLE
							move.l		0(a0),d0
							move.l		d0,d1
							andi.l		#$FFFF00FF,d1

							andi.l		#$FF00,d0
							lsr.L		#8,d0
							
							addq.l		#1,d0						; Ajoute offset Y 
							andi.l		#$FF,d0
							lsl.l		#8,d0
							or.l		d0,d1
							move.l		d1,0(a0)

;	Move the Jaguar logo to get always a center position

							lea.l		ReferenceList,a0
							adda.l		#16+16,a0

							move.l		12(a0),d0
							move.l		d0,d1
							andi.l		#$FFFFF000,d0
							andi.l		#$00000FFF,d1
							
							; inc Xposition							
							sub.w		#2,d1
							or.l		d1,d0
							move.l		d0,12(a0)

;	Move the Atari Fuji logo to screen center						

							lea.l		ReferenceList,a0
							move.l		12(a0),d0
							move.l		d0,d1
							andi.l		#$FFFFF000,d0
							andi.l		#$00000FFF,d1
							
							; inc Xposition							
							addq.l		#6,d1
							andi.l		#$000000FF,d1
							or.l		d1,d0
							move.l		d0,12(a0)

							movem.l		(sp)+,d0-d1/a0
							rts

;	------------------------------------------------------------------------------
;	Subroutine to play fuji logo

PlayFuji:					movem.l		d0-d1/a0,-(sp)

							lea.l		ReferenceList,a0
							move.l		12(a0),d0
							move.l		d0,d1
							andi.l		#$FFFFF000,d0
							andi.l		#$00000FFF,d1

; What's direction
							cmpi.w		#1,FujiSens
							beq.s		FujiUp

; left direction (decrement)

							cmpi.w		#144,d1
							blt.s		MinOk

							sub.w		FujiOffset,d1
							bra.s		fgo

MinOk:						move.w		#1,FujiSens
							add.w		FujiOffset,d1
							bra.s		fgo

; right direction (increment)

FujiUp:						cmpi.w		#144+160-48,d1
							bge.s		MaxOk

							add.w		FujiOffset,d1
							bra.s		fgo							
	
MaxOk:						move.w		#0,FujiSens
							sub.w		FujiOffset,d1							
														
; inc Xposition							
fgo:						or.l		d1,d0
							move.l		d0,12(a0)

							movem.l		(sp)+,d0-d1/a0
							rts

;	------------------------------------------------------------------------------
							include		"kit.inc"
