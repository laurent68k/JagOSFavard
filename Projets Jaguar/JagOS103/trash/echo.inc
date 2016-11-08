;	------------------------------------------------------------------------------
;
;

Echo:						move.l		d0,-(sp)
							
							move.w		#BUSY,-(sp)
							move.w		#SETPOINTER,-(sp)
							trap		#SYSTEM
							addq.l		#4,sp

.Wait:						jsr			LynxGetByte
							tst.l		d0
							blt.s		.Wait
											
							cmpi.w		#$1B,d0
							beq.s		.End

							move.w		d0,-(sp)
							jsr			LynxSendByte							
							addq.l		#2,sp
							bra.s		.Wait

.End:						move.w		#ARROW,-(sp)
							move.w		#SETPOINTER,-(sp)
							trap		#SYSTEM
							addq.l		#4,sp
		
							move.l		(sp)+,d0
							rts
