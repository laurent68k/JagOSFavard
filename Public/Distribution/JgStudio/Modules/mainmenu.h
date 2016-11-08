/*
 * ----------------------------------------------------------------------------------------------
 *					Fenetre quitter application
 *
 * 	Author  : 	FAVARD Laurent, FunShip (c)
 *	File    : 	Mainmenu.h
 *	Date    : 	10 December 1996
 *	Release : 	21 Mai 1997
 *	Version : 	1.0
 *	Country : 	FRANCE
 *			
 *
 *								- Copyright Atari FunShip (c) 1994-97 -
 *							- Atari ST, STE, TT, Falcon, C-LAB, Medusa, Hades -
 * ----------------------------------------------------------------------------------------------
 */

#ifndef		__BARRE_MENU__
#define		__BARRE_MENU__		__BARRE_MENU__

extern	int		G_SaveOnExit;
extern	char	G_HelpFile[];
extern	char	G_HelpOS98[];
extern	char	G_HelpToolBox98[];

extern	void Fichier(void);
extern	void Connect(void);
extern	void Outils(void);
extern	void Aide(void);

#endif
