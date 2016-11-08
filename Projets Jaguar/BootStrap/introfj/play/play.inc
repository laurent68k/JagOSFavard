;	------------------------------------------------------------------------------
;	Revision 3.00 - Heart of the boostrap code: main loop
;
;	Date:		09 May 1997
;	Revision:	18 December 1997
;
;	Funship - Atari jaguar (c) France - 1997
;
;	------------------------------------------------------------------------------

FUJIOFFSET					equ			5
WAITVBL						equ			50*5

PlayIt:						move.l		#0,d4									; offset to zoom in
							move.l		#WAITVBL,d5								; wait before auto start
							
							move.w		#TRUE,FujiSens
							move.w		#FALSE,ComputerReady

;	------------------------------------------------------------------------------
;	Initialisation KIT
						
							btst.b		#LINK_PRESENT,StatusBuffer				; Host computer ready ( = 1) ?
							beq.s		Agrandir								; No computer => Object List ok

							move.w		#TRUE,ComputerReady
									
							move.l		#ControlBuffer,a0	 					; Remise a zero des deux buffers
							move.b		(a0),d0

							jsr			Change									; Computer OK 

;	------------------------------------------------------------------------------

Agrandir:					jsr			WaitSync
							cmpi.l		#32,d4									; 32 fois atteinte
							bgt.s		Attente									; oui => attente

							addq.l		#1,d4									; 1 fois de plus
							jsr			Zoom									; jagaur zoom in

							bra			Agrandir
												
;	------------------------------------------------------------------------------
;	No Host computer => Awaiting a key to know what to do...

Attente:					jsr			WaitSync	
							jsr			PlayFuji								; move the jaguar logo
														
							jsr			ReadJoyPads								; read joypads's state
							move.l		JoyCur,d0	

;	------------------------------------------------------------------------------
;	Check what's to do

ck_a:						btst.l		#FIRE_A,d0								; START cardridge
							beq.s		ck_b
							jmp			JmpCartridge

ck_b:						btst.l		#FIRE_B,d0								; Software RESET
							beq.s		ck_c
							jmp			ResetJaguar				

ck_c:						btst.l		#FIRE_C,d0								; Send string anyway
							beq.s		ck_option
							movea.l		#Message,a0           					; Send to host
							bsr			SendString

ck_option:					btst.l		#KEY_OPTION,d0							; Upload program
							beq.s		ck_none
							move.w		ComputerReady,d0						; Host computer connected ?
							bne.s		Load									; Yes (=1) => Try to upload

ck_none:					move.w		ComputerReady,d0						; Host computer connected ?
							bne.s		Attente									; Yes (=1) => no automatic jmp

							sub.w		#1,d5									; dec the wait delay
							bne			Attente									; if > 0 don't jump now
							jmp			JmpCartridge							; else jump cartridge
							
;	------------------------------------------------------------------------------
;	Check if something is received to upload a program

Load:						jmp			Upload									; upload a program and start it

;	------------------------------------------------------------------------------
;	CHANGE:	Change image to display 'Computer OK' otherwise Computer no present
;	------------------------------------------------------------------------------

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
;	ZOOM:	Make a Zoom on the jaguar logo object and move fuji in the same time
;	------------------------------------------------------------------------------

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
;	PLAYFUJI:	Move the fuji logo to left and the right and again, and again...
;	------------------------------------------------------------------------------

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

							sub.w		#FUJIOFFSET,d1
							bra.s		fgo

MinOk:						move.w		#1,FujiSens
							add.w		#FUJIOFFSET,d1
							bra.s		fgo

; right direction (increment)

FujiUp:						cmpi.w		#144+160-48,d1
							bge.s		MaxOk

							add.w		#FUJIOFFSET,d1
							bra.s		fgo							
	
MaxOk:						move.w		#0,FujiSens
							sub.w		#FUJIOFFSET,d1							
														
; inc Xposition							
fgo:						or.l		d1,d0
							move.l		d0,12(a0)

							movem.l		(sp)+,d0-d1/a0
							rts

;	------------------------------------------------------------------------------
