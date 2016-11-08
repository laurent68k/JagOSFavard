/*
 * ----------------------------------------------------------------------------------------------
 *			Module de gestion des s‚lecteurs de fichiers et d'appel un PRG externe
 *
 * 	Author  : 	FAVARD Laurent, FunShip (c)
 *	File    : 	TOOLS.H
 *	Date    : 	6  Avril 1994
 *	Release : 	08 September 1997
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

#include	"..\Modules\Const.h"

extern	void 			DispErr( int Code );
extern	void			CallHTMLApplication( char *AppPathName, char *Parametre );
extern	signed int		GetHeader( char *PathName, TYPE_SENDER_PARAM *SenderParameters );
extern	void 			ReOpenFile( char *CmdLine );
extern	void			HideOthers( void );

#endif