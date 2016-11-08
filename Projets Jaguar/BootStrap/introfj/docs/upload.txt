;	Hardware Jaguar KIT 
;	
;	File:		KIT.INC
;
;	Date:		09/05/97
;	Revision:	08/10/97
;
;	Funship Atari jaguar (c) France - 1997
;	------------------------------------------------------------------------------


LEN_HEADER					equ			18

;	------------------------------------------------------------------------------
;	Emplacement bits carte DEFINITIVE et PROTOTYPE

SBUF_EMPTY					equ			2
SBUF_HALF					equ			0
SBUF_FULL					equ			1
RBUF_EMPTY					equ			5
RBUF_HALF					equ			3
RBUF_FULL					equ			4
LINK_PRESENT				equ			6

;	------------------------------------------------------------------------------
;	Attention aux adresses impaires !

StatusBuffer				equ			$E10001
ControlBuffer				equ			$E14001
SendBuffer					equ			$E16001
ReceiveBuffer				equ			$E12001

;	Zone de donnees definies par ces constantes

Header						equ			0
FunctionNumber				equ			Header+4				;	N fct kit roine (nu)
Target						equ			FunctionNumber+2		;	debut ou stocker le programme
Length						equ			Target+4				;	longueur du programme
StartCode					equ			Length+4				;	point d'entree

;	------------------------------------------------------------------------------
										
;	Initialisation KIT

							move.l		#ControlBuffer,a0	 	;	Remise a zero des deux buffers
							move.b		(a0),d0
						
							movea.l		#Message1,a0           	;	Envoyer un message
							bsr			SendString

;	Teste si un Atari est connecte sinon saute au debut de la cartouche

Teste:						btst.b		#LINK_PRESENT,StatusBuffer 	;	Bit Atari present = 1
							bne.s		Signale				   	;	Oui, alors telecharger

							movea.l		#Message2,a0           	;	Envoyer un message
							bsr			SendString

;	Envoit un message vers l'Atari

Signale:					movea.l		#Message3,a0           	;	Envoyer un message
							bsr			SendString

;	------------------------------------------------------------------------------
;	Lit l'entete de programme recu. JAGR et les 3 infos du PRG.
;
;	a1 = Adresse ou stoker l'entete
;	d1 = Nombre d'octets a recevoir
;
							movea.l		#Header,a1			  	;	Adresse stocker l'entete
							move.l		#LEN_HEADER,d1		  	;	Longueur entete

LireEntete:					cmpi.l		#0,d1				  	;	Lire octet/octet
							beq.s		LireProgramme    		;	finie ? charger PRG
							bsr			GetByte
							move.b		d0,(a1)+
							subq.l		#1,d1
							bra.s		LireEntete

;	------------------------------------------------------------------------------
;	Telechargement du programme puis execution.
;
;	a1 = Adresse ou stoker le programme
;	a2 = Adresse ou sauter pour executer
;	d1 = Nombre d'octets a recevoir
;

LireProgramme:				movea.l		#Message4,a0	 		;	Envoit un petit message
							bsr			SendString

							movea.l		Target,a1		 		;	adresse de stockage
							move.l		Length,d1		 		;	longueur
							movea.l		StartCode,a2	 		;	executer PRG

Charger:					cmpi.l		#0,d1			 		;	telecharger
							beq.s		Terminee
							bsr			GetByte
							move.b		d0,(a1)+
							sub.l		#1,d1
							bra.s		Charger

Terminee:					movea.l		Message5,a0		 		;	Envoit un petit message
							bsr			SendString

							jmp			(a2)


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

;	------------------------------------------------------------------------------
							DATA
														
							EVEN
Message1:					dc.b		"Jaguar BootROM loader ready",13,10
							dc.b		"Version 1.00 Atari Jaguar System",13,10
							dc.b		"--------------------------------",13,10,10,0

                            EVEN
Message2:					dc.b		"WARNING: No Atari, jmp CARTOUCHE",13,10,0

                            EVEN
Message3:					dc.b		"Atari detected, await a header",13,10,0

                            EVEN
Message4:					dc.b		"Header loaded, loading program code now...",13,10,0

							EVEN
Message5:					dc.b		"Program loaded, start execution...",13,10,0

;	------------------------------------------------------------------------------

							end
							