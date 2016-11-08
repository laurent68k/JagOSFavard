/*
 * ----------------------------------------------------------------------------------------------
 *
 *
 * Author  : 	FAVARD Laurent *	File    : 	Status.C
 *	Date    : 	09 may 2000
 *	Release : 	09 may 2000
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
#include <stdlib.h>
#include <string.h>
#include <tos.h>

#include	<PcAads.h>
/*
 * ----------------------------------------------------------------------------------------------
 */

#include	"..\modules\const.h"

#ifdef	__MAGIC_OS__
#include	"..\magic.rsc\jaguar_F.H"
#endif

#ifdef	__NAES_OS__
#include	"..\naes.rsc\jaguar_F.H"
#endif

#ifdef	__TOS_OS__
#include	"..\Tos.rsc\jaguar_F.H"
#endif

/*
 * ----------------------------------------------------------------------------------------------
 *			Fichiers sp‚cifiques … inclure
 */

#include	"..\modules\messages.h"
#include	"..\modules\bulles.h"
#include	"..\modules\const.h"
#include	"..\modules\jaguar.h"

#include	"..\fenetres\console.h"


/*
 * ----------------------------------------------------------------------------------------------
 *		Description de la fenetre "Quitter l'application"
 * ----------------------------------------------------------------------------------------------
 */

void	MouseEnter(void);

static Type_Form_List
FormAction[]=	{

					{ISTATUS,	NULL, NULL, NULL, NULL, MouseEnter, NULL},							/* Bitmap SMILEY */
					FORM_END
				};

/*
 * ----------------------------------------------------------------------------------------------
 *			Variables
 */

int		WHStatus;
int		ActiveStatus;

/* ---------------------------------------------------------------------------------------------- */
void OpenStatus( void )
/*
	Open window for display SMILEY
	
*/
{
	Type_Formular_Parameter		*Parameter;
	int							L_inX, L_inY, dummy;

	
	/*
	 *	Si Status est d‚j… actif => Le desactiver 
	*/
	if( ActiveStatus )
	{
	  	wind_get(WHStatus, WF_CURRXYWH, &L_inX, &L_inY, &dummy, &dummy);
  	
		SetLongProfile(G_APP_INF, G_WIND_WSTATUS, "position_x", (long)L_inX);
		SetLongProfile(G_APP_INF, G_WIND_WSTATUS, "position_y", (long)L_inY);

    	ActiveStatus = FALSE;						
	    M_UnChecked( MAINMENU, O_STATUS );
    	F_Close( WSTATUS );
	    return;
	}

  /*
   *	Smiley non actif => L'activer 
   */
	Parameter = F_GetParameter();
	if(Parameter != NULL)
	{
		Parameter->TreeName =	WSTATUS;								/* Indice de l'arbre de l'objet */
		Parameter->Title	=	G_TITLE_WSTATUS;							/* Titre de la fenetre */
		Parameter->Mode		=	F_WIND;									/* Opening mode */
		Parameter->FirstEdit=	NOTEXT;									/* First Editable Text */
		Parameter->List		=	FormAction;								/* Event list */
		Parameter->FormIcon =	IAPPLI;									/* Icone d'iconification */
		Parameter->Mouse	=	ARROW;
		Parameter->Smaller	=	FALSE;
		Parameter->Closer	=	FALSE;

		if(!G_ResolutionChanged)
		{
			Parameter->x	=	(int)GetLongProfile(G_APP_INF, G_WIND_WSTATUS, "position_x", 0);
			Parameter->y	=	(int)GetLongProfile(G_APP_INF, G_WIND_WSTATUS, "position_y", 0);
		}
		
		if( ( WHStatus = F_XOpen(Parameter) ) != FALSE)
		{
			M_Checked( MAINMENU, O_STATUS );
			ActiveStatus = TRUE;										/* Compl‚mente SaveOnExit */
		}

		free(Parameter);
	}
}
/* ---------------------------------------------------------------------------------------------- */
static void MouseEnter(void)
/*

*/
{
	switch( F_NameObject )
	{
		case	ISTATUS:	if( JTB_DevStatus() )
								STG_Bubble( "Jaguar is quiet", -1, -1);
							else
								STG_Bubble( "Jaguar is sending something", -1, -1);
								
							break;
	}
}
