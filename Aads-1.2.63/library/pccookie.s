;		Code de la routine d'effacement de la
;		variable systäme "_cookie"
;
;
; Auteur	: FunShip
; Fichier	: Cookrst.s
; Date		: 8 DÇcembre 1993
; Revision	: 15 Mai 1994
; Version	: 1.00
; Release	: 1.00
; Langage	: GenDev ou Assembleur Pure-C
;
; -----------------------------------------------------------------------
;
; # Routine d'installation de la routine CookieReset 
; Elle installe la routine CookieReset en tant que S/P Reset TOS
;
; # Routine Reset de la variable 0x5A0 pour les TOS < 1.6
; Cette routine doit etre appelÇe lors d'un Reset par le TOS, elle efface
; le contenu de la variable _cookie qui n'existait pas Ö "l'Çpoque"
; et supprime sa propre installation ensuite.
; -----------------------------------------------------------------------

		XDEF	Install_Reset
		XDEF	CookieReset
		
; les defines...
		
_cookie		equ	$5A0
resvalid	equ	$426
resvector	equ	$42A

; Les codes
;		section TEXT

Install_Reset:	
		move.l	resvalid,anc_resvalid
		move.l	resvector,anc_resvector
		
		move.l	#CookieReset,resvector	; @ de la routine
		move.l	#$31415926,resvalid	; valeur magique
		rts
				
CookieReset:	move.l	#0,_cookie		; clear of _cookie
		move.l	anc_resvalid,resvalid	; restore old value
		move.l	anc_resvector,resvector
		jmp	(a6)

; Quelques donnÇes
				
;		section DATA
		
texte:		dc.b	"Routine Reset d'effacement du Cookie Jar pour ST"
		dc.b	13,10,0
		
;		section BSS
		
anc_resvalid:	ds.l	1
anc_resvector:	ds.l	1

		end		
