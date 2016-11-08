							TEXT
							EVEN

;	------------------------------------------------------------------------------
;	long cdecl SetIconPRG( void *BitmapAddress, int BitMapLenght)
;	------------------------------------------------------------------------------
;	Version 1.0 - 03 Jully 1998 - FunShip
;	Revision:	03 May 2000
;
;	Given:					6(sp).L	=	Lenght of datas packed
;							2(sp).l	=	Address of bitmap to display
;
;	FONCTION:	Number 18
;
;	This function set the icon program from a packed source (format .cci)
;
;	WARNINGS:	don't forget to restore d1-d7/a0-a6 from the stack
;       a0 = Address of this routine
;		a1 = Address of input parameters
;		d0 = will contain return value (as we want)
; ---------------------------------------------------------------------------

OFF_LENGHT					set			6
OFF_ADR						set			2

SetIconPRG:					move.l		OFF_ADR(a1),a0							; Icon start address
							move.w		OFF_LENGHT(a1),d0						; Lenght of packed datas icon
							
							move.l		a0,-(sp)								; Source packed bitmap
							move.l		d0,-(sp)								; packed datas's size
							move.l		#DISPBUF7,-(sp)							; Target to unpack (Object's bitmap address)
							move.w		#UNPACK,-(sp)
							trap		#SYSTEM
							add.l		#14,sp

.End:						movem.l		(sp)+,d1-d7/a0-a6
							RTE
							

;	------------------------------------------------------------------------------
;	void cdecl Pterm( void )
;	------------------------------------------------------------------------------
;	Version 1.0 - 06 Jully 1998 - FunShip
;
;	Given:					NONE
;
;	FONCTION:	Number 19
;
;	This function terminate the current jaguar program running.
;
;	WARNINGS:	don't forget to restore d1-d7/a0-a6 from the stack
;       a0 = Address of this routine
;		a1 = Address of input parameters
;		d0 = will contain return value (as we want)
; ---------------------------------------------------------------------------

Pterm:						

;	Restore VI
							move.w  	VarVde,d0						; thats the downside border of the screen
							btst.l		#0,d0							; check for odd
							bne.s		.uneq							; in odd thats fine
							subq.w  	#1,d0							; else use one line before as end
.uneq:	
							move.w  	d0,VI


;	Restore Objects list to the OP

							move.l		DisplayableList_Ptr,d0
							swap		d0								; THIS SWAP IS REQUIRED (Register is designed English)
							move.l		d0,OLP							; Point to finished Object List

;	Replace the Vblank vector

							move.w		#INT1_VALUE,INT1
							move.w		#0,INT2							; CPU Interrupt Resume Register
							move.l		#vblank,$100.w					; Install our own VBLANK Interrupt

;	Go back to the OS ( Restore PSW )

							movem.l		(sp)+,d1-d7/a0-a6
							move.l		SavePC,-(sp)					;	This will call ".Terminate" from EXEC_PRG macro code
							move.w		SaveSR,-(sp)

							RTE
							
;	------------------------------------------------------------------------------
;	void cdecl UserFrame( long TargetPID, void *Buffer, unsigned int Lenght )
;	------------------------------------------------------------------------------
;	Version 1.02 - 07 Jully 1998 - FunShip
;
;	Given:					10(sp).l	=	Lenght of datas packed
;							06(sp).l	=	Address of bitmap to display
;							02(sp).l	=	Target PID addressed
;							00(sp).w	=	Fonction number
;
;	FONCTION:	Number 20
;
;	This function send an user frame code for general purpose usage.
;
;	WARNINGS:	don't forget to restore d1-d7/a0-a6 from the stack
;       a0 = Address of this routine
;		a1 = Address of input parameters
;		d0 = will contain return value (as we want)
; ---------------------------------------------------------------------------

OFF_LENGHT					set         10
OFF_ADDRSTART				set         6
OFF_TPID					set			2
OFF_OPCODE					set			0

UserFrame:					cmpi.l		#TOOLBOX_PID,OFF_TPID(a1)
							bne.s		.EndWithError

							move.l		OFF_ADDRSTART(a1),-(sp)						;	Datas's address to send
                    	    move.w		OFF_LENGHT(a1),-(sp)						;	Datas lenght
	                        move.w		#FRAME_USER1,-(sp)							;	Frame's Code USER 1
    	                    move.l		OFF_TPID(a1),-(sp)							;	Target PID which we want to receive
        	                move.l		#KERNEL_PID,-(sp)							;	Source PID which want to send
		 					jsr			SendFrame
                	        add.l		#16,sp
	
.End:						movem.l		(sp)+,d1-d7/a0-a6
							move.l		#TRUE,d0
							RTE

.EndWithError:				movem.l		(sp)+,d1-d7/a0-a6
							move.l		#FALSE,d0
							RTE
