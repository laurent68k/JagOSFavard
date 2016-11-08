/*
 * ----------------------------------------------------------------------------------------------
 *			Module de gestion des s‚lecteurs de fichiers et d'appel un PRG externe
 *
 * 	Author  : 	FAVARD Laurent, FunShip (c)
 *	File    : 	TOOLS.H
 *	Date    : 	6  Avril 1994
 *	Release : 	09 December 1996
 *	Version : 	1.0
 *	Country : 	FRANCE
 *			
 *
 *				- Copyright Atari FunShip (c) 1994-96 -
 *	 		     - ATARI ST,STE,TT, Falcon, Medusa et Hades-
 * ----------------------------------------------------------------------------------------------
 */

#ifndef		__TOOLS__
#define		__TOOLS__	__TOOLS__

#define		NAMELONG		2048
#define		EXTLONG			4
#define		PATHLONG		2048

extern	void GetPath(char *Chemin,char *Label);
extern	void CallApplication(char *AppPathName, char *Parametre);
extern	int GetProfileInf(char *FileNameINF);
extern	void SetProfileInf(char *FileNameINF);

#endif