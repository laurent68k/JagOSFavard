;	Hardware Jaguar KIT 
;	
;	File:		KIT.INC
;
;	Date:		09/05/97
;	Revision:	27/11/97
;
;	Funship & Gordh Atari jaguar (c) France - 1997
;	------------------------------------------------------------------------------


LEN_HEADER					equ			18

;	------------------------------------------------------------------------------
;	Emplacement bits carte JAGUAR & FALCON

SBUF_HALF					equ			0	; Actif BAS
SBUF_FULL					equ			1	; Actif BAS 
SBUF_EMPTY					equ			2	; Actif BAS 
RBUF_HALF					equ			3	; Actif BAS 
RBUF_FULL					equ			4	; Actif BAS 
RBUF_EMPTY					equ			5	; Actif BAS 
;LINK_PRESENT					equ			6	; Actif HAUT

;	------------------------------------------------------------------------------

;StatusBuffer				equ			$E1F000
ReceiveBuffer				equ			$E1F400
;ControlBuffer				equ			$E1F800
SendBuffer				equ			$E1FC00

;	Zone de donnees definies par ces constantes

Header						equ			0
FunctionNumber				equ			Header+4				;	N fct kit roine (nu)
Target						equ			FunctionNumber+2		;	debut ou stocker le programme
Length						equ			Target+4				;	longueur du programme
StartCode					equ			Length+4				;	point d'entree

;	------------------------------------------------------------------------------
;							Routines E/S sur Hardware Kit jaguar.
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

Attendre1:					btst.b		#SBUF_EMPTY,StatusBuffer	; Attendre a que vide
							bne.s		Attendre1


Envoit:						cmpi.b		#0,(a0)					; tant que chaine[i] != '\0'
							beq.s		SendStringEnd

							move.b		(a0)+,d1				; caractere a envoyer
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


							
