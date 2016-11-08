/*
 * ----------------------------------------------------------------------------------------------
 *
 *
 * Author  : 	FAVARD Laurent
 *	File    : 	Fuji.C
 *	Date    : 	12 June 1998
 *	Release : 	17 June 1998
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

void	Action(void);
void	MouseEnter(void);

static Type_Form_List
FormAction[]=	{

					{ISMILEY,	Action,	NULL, NULL, NULL, MouseEnter, NULL},							/* Bitmap SMILEY */
					FORM_END
				};

/*
 * ----------------------------------------------------------------------------------------------
 *			Variables
 */

int		WHSmiley;
int		ActiveSmiley;

/*
 * ----------------------------------------------------------------------------------------------
 */

void OpenSmiley(void)
/*
	Open window for display SMILEY
	
*/
{
	Type_Formular_Parameter		*Parameter;
	int							L_inX, L_inY, dummy;

	
	/*
	 *	Si Smiley est d‚j… actif => Le desactiver 
	*/
	if(ActiveSmiley)
	{
	  	wind_get(WHSmiley, WF_CURRXYWH, &L_inX, &L_inY, &dummy, &dummy);
  	
		SetLongProfile(G_APP_INF, G_WIND_SMILEY, "position_x", (long)L_inX);
		SetLongProfile(G_APP_INF, G_WIND_SMILEY, "position_y", (long)L_inY);

    	ActiveSmiley = FALSE;						
	    M_UnChecked(MAINMENU, O_SMILEY);
    	F_Close(SMILEY);
	    return;
	}

  /*
   *	Smiley non actif => L'activer 
   */
	Parameter = F_GetParameter();
	if(Parameter != NULL)
	{
		Parameter->TreeName =	SMILEY;								/* Indice de l'arbre de l'objet */
		Parameter->Title	=	G_TITLE_SMILEY;							/* Titre de la fenetre */
		Parameter->Mode		=	F_WIND;									/* Opening mode */
		Parameter->FirstEdit=	NOTEXT;									/* First Editable Text */
		Parameter->List		=	FormAction;								/* Event list */
		Parameter->FormIcon =	IAPPLI;									/* Icone d'iconification */
		Parameter->Mouse	=	ARROW;
		Parameter->Smaller	=	FALSE;
		Parameter->Closer	=	FALSE;

		if(!G_ResolutionChanged)
		{
			Parameter->x	=	(int)GetLongProfile(G_APP_INF, G_WIND_SMILEY, "position_x", 0);
			Parameter->y	=	(int)GetLongProfile(G_APP_INF, G_WIND_SMILEY, "position_y", 0);
		}
		
		if((WHSmiley = F_XOpen(Parameter)) != FALSE)
		{
			M_Checked(MAINMENU, O_SMILEY);
			ActiveSmiley = TRUE;										/* Compl‚mente SaveOnExit */
		}

		free(Parameter);
	}
}

/*
 * ----------------------------------------------------------------------------------------------
 */

static void MouseEnter(void)
/*

*/
{
	switch( F_NameObject )
	{
		case	ISMILEY:	STG_Bubble( BUBBLE16, -1, -1);
							break;
	}
}

static void Action(void)
/*
	Suivant le bouton cliqu‚ dans la fenetre de fin d'application, assigne la variable 'Sortir'.
*/
{
	int				L_inX, L_inY, dummy;

	/*	Button Close */
	graf_mouse(BUSYBEE, NULL);
						
	wind_get(WHSmiley, WF_CURRXYWH, &L_inX, &L_inY, &dummy, &dummy);
	
	SetLongProfile(G_APP_INF, G_WIND_SMILEY, "position_x", (long)L_inX);
	SetLongProfile(G_APP_INF, G_WIND_SMILEY, "position_y", (long)L_inY);

	F_Close(SMILEY);
	graf_mouse(ARROW, NULL);
}

/*
 * ----------------------------------------------------------------------------------------------
 */

void SpeechSmiley(char *chString)
{
	static	int	Bulle_X, Bulle_Y;
	OBJECT		*Arbre;

	if( ActiveSmiley )
	{	
		rsrc_gaddr(R_TREE, SMILEY, &Arbre);

		Bulle_X	=	Arbre[R_TREE].ob_x + Arbre[ISMILEY].ob_x + ( Arbre[ISMILEY].ob_width / 2);
		Bulle_Y	=	Arbre[R_TREE].ob_y + Arbre[ISMILEY].ob_y + ( Arbre[ISMILEY].ob_height /2);
		
		Bulle_Y	-=	6;
		STG_Bubble( chString, Bulle_X, Bulle_Y);
	}
}
