/*
 * ----------------------------------------------------------------------------------------------
 *								Fenetres Barre d'outils.
 *
 * 	Author  : 	FAVARD Laurent
 *	File    : 	NEWFORM.C
 *	Date    : 	19 December 1998
 *	Release : 	23 December 1998
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
#include	"..\modules\Bulles.h"
#include	"..\modules\jaguar.h"
#include	"..\modules\const.h"
#include	"..\modules\codegen.h"
#include	"..\modules\mainmenu.h"

#include	"..\fenetres\genform.h"

/*
 * ----------------------------------------------------------------------------------------------
 *		Description de la Barre d'outils de l'application
 * ----------------------------------------------------------------------------------------------
 */

void Action(void);
void DbAction(void);
void CloseNew(void);
void MouseEnter(void);

static	Type_Form_List
ActionList[]=	{
					{ROOT,	CloseNew,	NULL, NULL, NULL, MouseEnter, NULL},				/*	Root object Close window */
					{BN_38,	Action,	NULL, NULL, NULL, MouseEnter, NULL},					/*	*/
					{BN_39,	Action,	NULL, NULL, NULL, MouseEnter, NULL},					/*	*/
					{PRG1,	Action,	DbAction, NULL, NULL, MouseEnter, NULL},				/*	*/
					{PRG2,	Action,	DbAction, NULL, NULL, MouseEnter, NULL},				/*	*/
					{PRG3,	Action,	DbAction, NULL, NULL, MouseEnter, NULL},				/*	*/
					{PRG4,	Action,	DbAction, NULL, NULL, MouseEnter, NULL},				/*	*/
					{HELP6,	Action,	DbAction, NULL, NULL, MouseEnter, NULL},				/*	*/

					FORM_END
				};


/*
 * ----------------------------------------------------------------------------------------------
 *			Variables
 */

int					WHNewForm;
static	int			TypeObjet;

/*
 * ----------------------------------------------------------------------------------------------
 *			Ouverture et fermeture de la toolbar.
 * ----------------------------------------------------------------------------------------------
 */

void NewForm(void)
/*
	Ouvre la fenetre Barre d'outils de l'application.
*/
{
	Type_Formular_Parameter		*Parameter;

	
	Parameter = F_GetParameter();
	if(Parameter != NULL)
	{
		Parameter->TreeName =	NEWFORM;								/* Indice de l'arbre de l'objet */
		Parameter->Title	=	G_TITLE_NEWFORM;							/* Titre de la fenetre */
		Parameter->Mode		=	F_WIND;									/* Opening mode */
		Parameter->FirstEdit=	NOTEXT;									/* First Editable Text */
		Parameter->List		=	ActionList;								/* Event list */
		Parameter->FormIcon =	IAPPLI;									/* Icone d'iconification */
		Parameter->Mouse	=	MS_POINTFLATHAND;
		Parameter->Mode		=	MODAL;
		
		if(!G_ResolutionChanged)
		{
			Parameter->x	=	(int)GetLongProfile(G_APP_INF, G_WIND_NEWFORM, "position_x", 0);
			Parameter->y	=	(int)GetLongProfile(G_APP_INF, G_WIND_NEWFORM, "position_y", 0);
		}
		
		if((WHNewForm = F_XOpen(Parameter)) != FALSE)
		{
		}
		free(Parameter);
	}
}

/*
 * ----------------------------------------------------------------------------------------------
 *			Prise en compte des actions de la ToolBar
 * ----------------------------------------------------------------------------------------------
 */

static void MouseEnter(void)
/*
	This procedure is called when the mouse enter onto an object.
*/
{
	if( G_BulleAide )
	{
		switch(F_NameObject)
		{
			/*	Bouton QUITTER */
			case	PRG1:	STG_Bubble( "", -1, -1);
							break;

			/*	OPEN file */
			case	PRG2:	STG_Bubble( "", -1, -1);
							break;
						
			/*	Load binaries */
			case	PRG4:	STG_Bubble( "", -1, -1);
							break;

			default:		
							break;
		}
	}
}

void CloseNew(void)
{
	int							L_inX, L_inY, dummy;

  	wind_get(WHNewForm, WF_CURRXYWH, &L_inX, &L_inY, &dummy, &dummy);
  	
	SetLongProfile(G_APP_INF, G_WIND_NEWFORM, "position_x", (long)L_inX);
	SetLongProfile(G_APP_INF, G_WIND_NEWFORM, "position_y", (long)L_inY);

   	F_Close(NEWFORM);
}

/* ---------------------------------------------------------------------------------------------- */
static void DbAction(void)
{
	switch(F_NameObject)
	{
		case	PRG1:	Action();
						break;
						
		case	PRG2:	Action();
						break;
						
		case	PRG3:	Action();
						break;
						
		case	PRG4:	Action();
						break;
	}
	GenWindow( TypeObjet );
}

/* ---------------------------------------------------------------------------------------------- */
static void Action(void)
/*
	Prise en compte des actions sur les boutons.
*/
{

	switch(F_NameObject)
	{
		/*	Bouton OK */
		case	BN_38:	if( TypeObjet )
							GenWindow( TypeObjet );
						break;

		/*	Bouton Cancel */
		case	BN_39:	CloseNew();
    					break;
						
		case	PRG1:	TypeObjet	=	COOPERANT;
						F_WriteText( NEWFORM, TEXTINF2, "Jaguar program for host" );
						break;
						
		case	PRG2:	TypeObjet	=	SOURCEASM;
						F_WriteText( NEWFORM, TEXTINF2, "Source program" );
						break;
						
		case	PRG3:	TypeObjet	=	FREEPROGRAM;
						F_WriteText( NEWFORM, TEXTINF2, "Free program without JagOS" );
						break;
						
		case	PRG4:	TypeObjet	=	FREEWITHOS;
						F_WriteText( NEWFORM, TEXTINF2, "Free program which use JagOS" );
						break;

		case	HELP6:	STG_Open(G_HelpFile, SECTION_HELP_NEWFORM);
						break;
												
		default:		break;
	}
}

