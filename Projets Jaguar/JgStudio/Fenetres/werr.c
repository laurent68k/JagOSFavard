/*
 * ----------------------------------------------------------------------------------------------
 *								Fenetres affichage d'erreurs
 *
 * 	Author  : 	FAVARD Laurent
 *	File    : 	Werr.c
 *	Date    : 	26 April 2000
 *	Release : 	26 April 2000
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
 *		Description de la fenetre A propos...
 * ----------------------------------------------------------------------------------------------
 */

void	ErrClose(void);

static Type_Form_List
FormErrEvent[]=	{
					{BN_53, ErrClose, NULL},
					FORM_END
				};

/*
 * ----------------------------------------------------------------------------------------------
 *			Fichiers sp‚cifiques … inclure
 */

#include	"..\modules\messages.h"
#include	"..\modules\jaguar.h"
#include	"..\modules\const.h"

/*
 * ----------------------------------------------------------------------------------------------
 *			Variables
 */

int					WHErr;

/*
 * ----------------------------------------------------------------------------------------------
 *				Gestion de fenetre la fenetre Err
 * ----------------------------------------------------------------------------------------------
 */

void WError( int ErrorCode, char *Message )
/*
	Ouvre la fenetre Erreur.
*/
{
	Type_Formular_Parameter		*Parameter;
  	char						Text[ 50 ];
  	
	Parameter = F_GetParameter();
	if(Parameter != NULL)
	{
    	Parameter->TreeName =	WERR;														/* Indice de l'arbre de l'objet */
		Parameter->Title	=	G_TITLE_WERR;													/* Titre de la fenetre */
    	Parameter->Mode		=	F_WIND;														/* Opening mode */
    	Parameter->Type		=	MODAL;
    	Parameter->FirstEdit=	NOTEXT;														/* First Editable Text */
    	Parameter->List		=	FormErrEvent;															/* Event list */
    	Parameter->FormIcon =	IAPPLI;														/* Icone d'iconification */
		Parameter->Mouse	=	MS_BOMB;

		if(!G_ResolutionChanged)
		{
			Parameter->x	=	(int)GetLongProfile(G_APP_INF, G_WIND_WERR, "position_x", 0);
			Parameter->y	=	(int)GetLongProfile(G_APP_INF, G_WIND_WERR, "position_y", 0);
		}

    	if( (WHErr = F_XOpen( Parameter )) != FALSE)
    	{
    		sprintf( Text, "Error code = 0x%X (%d)", ErrorCode, ErrorCode );
    		F_WriteText( WERR, ERRTEXT1, Text );
    		F_WriteText( WERR, ERRTEXT2, Message );
    	}
    	free(Parameter);
	}
}

static void ErrClose(void)
{
  	int							X, Y, dummy;

  	wind_get( WHErr, WF_CURRXYWH, &X, &Y, &dummy, &dummy );
  	
	SetLongProfile(G_APP_INF, G_WIND_WERR, "position_x", (long)X);
	SetLongProfile(G_APP_INF, G_WIND_WERR, "position_y", (long)Y);

	WHErr	=	0;
	F_Close( WERR );
}
