/*
 * ----------------------------------------------------------------------------------------------
 *					Lancement des applications en parallŠle de JagStudio grace a Shel_write().
 *
 * 	Author  : 	FAVARD Laurent
 *	File    : 	Lunch.C
 *	Date    : 	19 January 1999
 *	Release : 	19 January 1999
 *	Version : 	1.0
 *	Country : 	FRANCE
 *			
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * ----------------------------------------------------------------------------------------------
 */


#include <stdio.h>

/* ----------------------------------------------------------------------------------------------*/

#include "MT_AES.h"				/*	MagiC OS	*/
/*#include <PcAads.h>*/

/* ----------------------------------------------------------------------------------------------*/

#include	"..\modules\Jaguar.h"

/* ----------------------------------------------------------------------------------------------*/

/*
 *	Ces proc‚dures permettent de lancer en parallŠles des applications externes. Elles ne
 *	savent g‚rer uniquement que MagiC OS.
 */
 
/* ----------------------------------------------------------------------------------------------*/
int EditSource( void )
{
#ifdef	__MAGIC_OS__
	int		retour;
	char	Message[100];
	
	
	retour	=	shel_write( TRUE, TRUE, SHW_PARALLEL, External.EditPathName , Projet.SrcPathName );
	if( !retour )
	{
		sprintf( Message, "[3][ Error while opening editor application | error = %d][Ok]", retour );
		form_alert(1, Message );
	}
	return	retour;		
#endif

#ifdef	__NAES_OS__
	form_alert(1, "[3][ JagStudio doesn't | support parallel start | Under N.AES][Ok]" );
	return	FALSE;
#endif

#ifdef	__TOS_OS__
	form_alert(1, "[3][ JagStudio doesn't | support parallel start | Under TOS/MiNT OS][Ok]" );
	return	FALSE;
#endif
}

/* ----------------------------------------------------------------------------------------------*/
int AsmSource( void )
{
#ifdef	__MAGIC_OS__
	int		retour;
	char	Message[100];
	
	retour	=	shel_write( TRUE, TRUE, SHW_PARALLEL, External.AsmPathName , Projet.SrcPathName );
	if( !retour )
	{
		sprintf( Message, "[3][ Error while opening Devpac application | error = %d][Ok]", retour );
		form_alert(1, Message );
	}
	return	retour;
#endif
		
#ifdef	__NAES_OS__
	form_alert(1, "[3][ JagStudio doesn't support parallel start | Under N.AES][Ok]" );
	return	FALSE;
#endif

#ifdef	__TOS_OS__
	form_alert(1, "[3][ JagStudio doesn't support parallel start | Under TOS/GEM OS][Ok]" );
	return	FALSE;
#endif
}


