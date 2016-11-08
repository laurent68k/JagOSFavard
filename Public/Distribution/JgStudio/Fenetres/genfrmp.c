/*
 * ----------------------------------------------------------------------------------------------
 *
 *
 * Author  : 	FAVARD Laurent
 *	File    : 	Genfrmp.C
 *	Date    : 	30 December 1998
 *	Release : 	30 December 1998
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

#include	"..\modules\Codegen.h"
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
#include	"..\modules\const.h"
#include	"..\modules\jaguar.h"
#include	"..\modules\mainmenu.h"

/*
 * ----------------------------------------------------------------------------------------------
 *		Description de la fenetre "Quitter l'application"
 * ----------------------------------------------------------------------------------------------
 */

void	Action(void);
void	DbClic(void);

static Type_Form_List
FormAction[]=	{

					{BN_48,		Action,	NULL, NULL, NULL, NULL, NULL},							/* Bouton Close */
					{BN_49,		Action,	NULL, NULL, NULL, NULL, NULL},							/* Bouton Ok */
					FORM_END
				};

/*
 * ----------------------------------------------------------------------------------------------
 *			Variables
 */

static	int						WHGenfrmp;
static	TSource 				*G_Definition;
/*
 * ----------------------------------------------------------------------------------------------
 *			Fenetre 
 * ----------------------------------------------------------------------------------------------
 */

void GenFrmP( TSource *Definition )
/*
	Ouvre la fenetre de configuration des programmes externes
	
*/
{
	Type_Formular_Parameter	*Parameter;
	char					chTexte[6]	=	"01024";
	
	G_Definition	=	Definition;
	Parameter = F_GetParameter();
	if(Parameter != NULL)
	{
		Parameter->Title	=	G_TITLE_GENFRMP;
		Parameter->TreeName =	GENFORMP;															/* Indice de l'arbre de l'objet */
		Parameter->Mode		=	F_WIND;																/* Opening mode */
		Parameter->FirstEdit=	ESZIN;																/* First Editable Text */
		Parameter->List		=	FormAction;															/* Event list */
		Parameter->FormIcon =	IAPPLI;																		/* Icone d'iconification */
		Parameter->Type		=	MODAL;
		Parameter->Mouse 	=	ARROW;			
		if(!G_ResolutionChanged)
		{
			Parameter->x	=	(int)GetLongProfile(G_APP_INF, G_WIND_GENFRMP, "position_x", 0);
			Parameter->y	=	(int)GetLongProfile(G_APP_INF, G_WIND_GENFRMP, "position_y", 0);
		}

		if((WHGenfrmp = F_XOpen(Parameter)) != FALSE)
		{
			if( Definition->SizeParamIn > 0 )
				sprintf( chTexte, "%d", Definition->SizeParamIn);
			F_WriteText( GENFORMP, ESZIN, chTexte);
			if( Definition->SizeParamOut > 0 )
				sprintf( chTexte, "%d", Definition->SizeParamOut);
			F_WriteText( GENFORMP, ESZOU, chTexte);
		}
		free(Parameter);
	}
}

static void Action(void)
/*
	Suivant le bouton cliqu‚ dans la fenetre.
*/
{
	int				L_inX, L_inY, dummy;
	char			chTexte[6];
	
	switch( F_NameObject )
	{
		/*	Button Close */
		case	BN_49:	F_ReadText( GENFORMP, ESZIN, chTexte);
						G_Definition->SizeParamIn	=	atoi( chTexte );
						F_ReadText( GENFORMP, ESZOU, chTexte);
						G_Definition->SizeParamOut	=	atoi( chTexte );

					
		case	BN_48: 	MS_Mouse(MS_SLEEP, NULL);
						
						wind_get(WHGenfrmp, WF_CURRXYWH, &L_inX, &L_inY, &dummy, &dummy);
  	
						SetLongProfile(G_APP_INF, G_WIND_GENFRMP, "position_x", (long)L_inX);
						SetLongProfile(G_APP_INF, G_WIND_GENFRMP, "position_y", (long)L_inY);

						F_Close(GENFORMP);
						MS_Mouse(ARROW, NULL);
						break;
						
		default:		break;
	}						
}
