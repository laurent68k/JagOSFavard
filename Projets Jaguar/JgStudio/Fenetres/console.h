/*
 * ----------------------------------------------------------------------------------------------
 *			Module de gestion des entr‚es/sorties sur port cartouche.
 *
 * 	Author  : 	FAVARD Laurent, FunShip (c)
 *	File    : 	CONSOLE.C
 *	Date    : 	15 December 1996
 *	Release : 	11 Juillet 1997
 *	Version : 	1.0
 *	Country : 	FRANCE
 *			
 *
 *				- Copyright Atari FunShip (c) 1994-96 -
 *	 		     - ATARI ST,STE,TT, Falcon, Medusa et Hades-
 * ----------------------------------------------------------------------------------------------
 */

#ifndef		__CONSOLE__
#define		__CONSOLE__			__CONSOLE__

extern	char	*G_TexteConsole[];														/* liste des lignes du texte */
extern	long	G_NbreLigneConsole;
extern	int		G_NbreColonneConsole;
extern	long	G_LigneCouranteConsole;
extern	long	G_LigneDepart;
extern	int		G_ConsoleModifie;

extern	int		WHandleConsole;
extern	int		tabulation;
extern	int		WHandleConsole;													/* La fenetre terminal */
extern	int		ActiveConsole;

extern	void	SetVHSlider(int handle);

extern	void 	OpenConsole(void);
extern	void 	UpdateConsole(void);
extern	void 	ResetConsole(void);
extern	void	SaveConsole(void);
extern	void	AddString(char *NewString);
extern	void	MenuWind(int Handle);

#endif
