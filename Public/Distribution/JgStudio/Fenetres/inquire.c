/*
 * ----------------------------------------------------------------------------------------------
 *
 *
 * Author  : 	FAVARD Laurent *	File    : 	Transfer.C
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
#include	<JToolBox.h>

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
#include	"..\modules\const.h"
#include	"..\modules\jaguar.h"

#include	"..\fenetres\console.h"

/*
 * ----------------------------------------------------------------------------------------------
 *		Description de la fenetre "Quitter l'application"
 * ----------------------------------------------------------------------------------------------
 */

void	Action(void);

static Type_Form_List
FormAction[]=	{

					{BN_30,	Action,	NULL},							/* Bouton HELP */
					FORM_END
				};

/*
 * ----------------------------------------------------------------------------------------------
 *			Variables
 */

static	int						WHInquire;

/*
 * ----------------------------------------------------------------------------------------------
 *			Fenetre 
 * ----------------------------------------------------------------------------------------------
 */

void OpenInquire(TJagId *JagIdentification)
/*
	Open window for display JagOS version.
	
*/
{
	Type_Formular_Parameter	*Parameter;
	char					chTexte[20];
  
	Parameter = F_GetParameter();
	if(Parameter != NULL)
	{
		Parameter->Title	=	G_TITLE_INQUIRE;											/* Titre de la fenetre */
		Parameter->TreeName =	INQUIRE;															/* Indice de l'arbre de l'objet */
		Parameter->Mode		=	F_WIND;																/* Opening mode */
		Parameter->FirstEdit=	NOTEXT;																/* First Editable Text */
		Parameter->List		=	FormAction;															/* Event list */
		Parameter->FormIcon =	IAPPLI;
		Parameter->Type 	=	MODELESS;																		/* Icone d'iconification */
		Parameter->Mouse 	=	ARROW;			
		if(!G_ResolutionChanged)
		{
			Parameter->x	=	(int)GetLongProfile(G_APP_INF, G_WIND_INQUIRE, "position_x", 0);
			Parameter->y	=	(int)GetLongProfile(G_APP_INF, G_WIND_INQUIRE, "position_y", 0);
		}

		if((WHInquire = F_XOpen(Parameter)) != FALSE)
		{
			sprintf(chTexte, "%X", JagIdentification->_SystemVersion);
			F_WriteText(INQUIRE, OSV, chTexte);

			sprintf(chTexte, "%X", JagIdentification->_HardwareVersion);
			F_WriteText(INQUIRE, HV, chTexte);

			sprintf(chTexte, "%lX", JagIdentification->_SystemDate);
			F_WriteText(INQUIRE, OSD, chTexte);

			sprintf(chTexte, "%lX", JagIdentification->_HardwareDate);
			F_WriteText(INQUIRE, HD, chTexte);
			
		}
		free(Parameter);
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
						
	wind_get(WHInquire, WF_CURRXYWH, &L_inX, &L_inY, &dummy, &dummy);
	
	SetLongProfile(G_APP_INF, G_WIND_INQUIRE, "position_x", (long)L_inX);
	SetLongProfile(G_APP_INF, G_WIND_INQUIRE, "position_y", (long)L_inY);

	F_Close(INQUIRE);
	graf_mouse(ARROW, NULL);
}
