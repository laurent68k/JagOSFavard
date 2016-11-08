;	------------------------------------------------------------------------------
;	Revision 3.00 - Uplaod routine
;
;	Date:		09 May 1997
;	Revision:	12 December 1997
;
;	Funship - Atari jaguar (c) France - 1997
;
;	------------------------------------------------------------------------------


;	------------------------------------------------------------------------------
;	Lit l'entete de programme recu. JAGR et les 3 infos du PRG.
;
;	a1 = Adresse ou stoker l'entete
;	d1 = Nombre d'octets a recevoir
;

Upload:						movea.l		#MessageStart,a0		;	Envoit un petit message
							bsr			SendString

							movea.l		#Header,a1			  	;	Adresse stocker l'entete
							move.l		#LEN_HEADER,d1		  	;	Longueur entete

LireEntete:					cmpi.l		#0,d1				  	;	Lire octet/octet
							beq.s		LireProgramme    		;	finie ? charger PRG
							bsr			GetByte
							move.b		d0,(a1)+
							sub.l		#1,d1
							bra.s		LireEntete

;	------------------------------------------------------------------------------
;	Telechargement du programme puis execution.
;
;	a1 = Adresse ou stoker le programme
;	a2 = Adresse ou sauter pour executer
;	d1 = Nombre d'octets a recevoir
;

LireProgramme:				movea.l		#MessageLoad,a0	 		;	Envoit un petit message
							bsr			SendString

							movea.l		#Header,a0
							
							movea.l		Target(a0),a1		 	;	adresse de stockage
							move.l		Length(a0),d1			;	longueur
							movea.l		StartCode(a0),a2	 	;	executer PRG

Charger:					cmpi.l		#0,d1			 		;	telecharger
							beq.s		Terminee
							bsr			GetByte
							move.b		d0,(a1)+
							sub.l		#1,d1
							bra.s		Charger

Terminee:					movea.l		MessageOk,a0		 		;	Envoit un petit message
							bsr			SendString

							jmp			(a2)
