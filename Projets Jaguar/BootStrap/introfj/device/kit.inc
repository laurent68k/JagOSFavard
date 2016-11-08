;	Hardware Jaguar KIT 
;	
;	File:		KIT.INC
;
;	Date:		09/05/97
;	Revision:	18/12/97
;
;	Funship Atari jaguar (c) France - 1997
;	------------------------------------------------------------------------------


;	------------------------------------------------------------------------------
;	Send a string of characters through the Hardware Kit port.
;   (Wait hard FIFO is ready)
;
;	a0 = @ of the string to send.
;	------------------------------------------------------------------------------

SendString:					movem.l		d1/a0-a1,-(sp)
							move.l		#SendBuffer,a1
							moveq.l		#0,d1

Attendre1:					btst.b		#SBUF_FULL,StatusBuffer	; Attendre a que vide
							beq.s		Attendre1

Envoit:
							move.b		(a0)+,d1				; caractere a envoyer
							beq.s		SendStringEnd

							move.b		(a1,d1.w),d1			; envoyer...
							bra.s		Envoit					; suite de la chaine

SendStringEnd:              movem.l		(sp)+,d1/a0-a1
							rts

;	------------------------------------------------------------------------------
;	Wait and get a byte from the Hardware Kit port.
;
;	d0 = octet arrivee
;	------------------------------------------------------------------------------

GetByte:					btst.b		#RBUF_EMPTY,StatusBuffer	; tant que vide..
							beq.s		GetByte
                            move.b		ReceiveBuffer,d0
							rts


							
