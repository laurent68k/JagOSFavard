;	------------------------------------------------------------------------------
;	Fuji logo (48*48)

ObjListROM:					dc.w	BITMAP_OBJECT
							dc.l	DISPBUF2						; DISPLAY buffer
							dc.l	FujiROM							; DRAW source bitmap
							dc.w	FujiCountROM-FujiROM			; O_SIZE
							dc.w	48-1							; O_HEIGHT
							dc.w	192+128								; O_YPOS

							dc.w	0								; 1STPIX
							dc.w	0								; FLAGS
							dc.w	0								; IDX
							dc.w	48/4							; IWIDTH
							dc.w	48/4							; DWIDTH
							dc.w	1								; PITCH
							dc.w	4								; DEPTH
							dc.w	0								; XPOS	
;	------------------------------------------------------------------------------
;	Computers (96*96 - But contains two images => 2 x 96*48) 

							dc.w	BITMAP_OBJECT
							dc.l	DISPBUF3						; DISPLAY buffer
							dc.l	ComputerROM						; DRAW source bitmap
							dc.w	ComputerCountROM-ComputerROM	; O_SIZE
							dc.w	(96/2)-1						; O_HEIGHT
							dc.w	192+128							; O_YPOS

							dc.w	0								; 1STPIX
							dc.w	0								; FLAGS
							dc.w	0								; IDX
							dc.w	96/4							; IWIDTH
							dc.w	96/4							; DWIDTH
							dc.w	1								; PITCH
							dc.w	4								; DEPTH
							dc.w	320								; XPOS							
;	------------------------------------------------------------------------------
;	BitMap object ROM Jaguar logo (160x58)

							dc.w	SCALED_OBJECT
							dc.l	DISPBUF1						; DISPLAY buffer
							dc.l	JagLogoROM						; DRAW source bitmap
							dc.w	JagLogoCountROM-JagLogoROM 		; SIZE
							dc.w	58-1							; HEIGHT
							dc.w	68								; YPOS

							dc.w	0								; 1STPIX
							dc.w	0								; FLAGS
							dc.w	0								; IDX
							dc.w	40								; IWIDTH
							dc.w	40								; DWIDTH
							dc.w	1								; PITCH
							dc.w	4								; DEPTH
							dc.w	224-16							; XPOS

							dc.w	$10
							dc.w	$00
							dc.w	$00					
;	------------------------------------------------------------------------------
;	STOP object
							dc.w	STOP_OBJECT

;	------------------------------------------------------------------------------
;	Bitmap source (CRY image from TGA source)
							
JagLogoROM:					include	"jaglogo3.cry"
JagLogoCountROM:			dc.l	$00000000

FujiROM:					include	"stmag.cry"
FujiCountROM:				dc.l	$00000000

ComputerROM:				include	"computer.cry"
ComputerCountROM:			dc.l	$00000000
