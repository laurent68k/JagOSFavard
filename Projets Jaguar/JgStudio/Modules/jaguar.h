/*
 * ----------------------------------------------------------------------------------------------
 *					include file form the main file.
 *
 * 	Author  : 	FAVARD Laurent, FunShip (c)
 *	File    : 	Jaguar.h
 *	Date    : 	10 December 1996
 *	Release : 	26 April 2000
 *	Version : 	1.0
 *	Country : 	FRANCE
 *			
 *
 *								- Copyright Atari FunShip (c) 1994-97 -
 *							- Atari ST, STE, TT, Falcon, C-LAB, Medusa, Hades -
 * ----------------------------------------------------------------------------------------------
 */

#ifndef		__JaguarH
#define		__JaguarH

#include	"..\modules\const.h"


#define		TRUE	1
#define		FALSE	0

extern	char		ApplicationHelp[];

extern	TProjet		Projet;
extern	TExternal	External;
					
extern	int			G_ResolutionChanged;
extern	int			G_Sortir;
extern	int			G_BulleAide;						/*	Activation ou pas des bulles d'aides */
extern	int			G_SizeFont;
extern	int			G_ColorFont;

extern	char		*G_Laurent, *G_Sebastien;

#endif
