/*
 * ----------------------------------------------------------------------------------------------
 *					Fentre emission fichier
 *
 * 	Author  : 	FAVARD Laurent
 *	File    : 	InfoFile.C
 *	Date    : 	21 July 1998
 *	Release : 	21 July 1998
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

#include	"..\modules\const.h"
#include	"..\modules\tools.h"
#include	"..\modules\messages.h"
#include	"..\modules\Bulles.h"
#include	"..\modules\jaguar.h"
#include	"..\modules\mainmenu.h"

#include	"..\fenetres\console.h"

/*
 * ----------------------------------------------------------------------------------------------
 *		Description de la fenetre d'‚mission
 * ----------------------------------------------------------------------------------------------
 */

void	Action(void);
void	MouseEnter(void);

static Type_Form_List
FormAction[]=	{	
					{BN_31,		Action,	NULL, NULL, NULL, MouseEnter, NULL},
/*					{B_SIZE,	NULL,	NULL, NULL, NULL, MouseEnter, NULL},
					{B_MARK,	NULL,	NULL, NULL, NULL, MouseEnter, NULL},
					{B_CODE,	NULL,	NULL, NULL, NULL, MouseEnter, NULL},
					{B_TARGET,	NULL,	NULL, NULL, NULL, MouseEnter, NULL},
					{B_START,	NULL,	NULL, NULL, NULL, MouseEnter, NULL},
					{B_OFFSET,	NULL,	NULL, NULL, NULL, MouseEnter, NULL},
					{B_LEN,		NULL,	NULL, NULL, NULL, MouseEnter, NULL},*/
					{I_SIZE,	NULL,	NULL, NULL, NULL, MouseEnter, NULL},
					{I_MARK,	NULL,	NULL, NULL, NULL, MouseEnter, NULL},
					{I_CODE,	NULL,	NULL, NULL, NULL, MouseEnter, NULL},
					{I_TARGET,	NULL,	NULL, NULL, NULL, MouseEnter, NULL},
					{I_START,	NULL,	NULL, NULL, NULL, MouseEnter, NULL},
					{I_OFFSET,	NULL,	NULL, NULL, NULL, MouseEnter, NULL},
					{I_LEN,		NULL,	NULL, NULL, NULL, MouseEnter, NULL},
					FORM_END
				};

/*
 * ----------------------------------------------------------------------------------------------
 *			Variables
 */

int								WHInfoFile;

/*
 * ----------------------------------------------------------------------------------------------
 *				Ouverture et gestion de la fenetre d'emission fichier
 * ----------------------------------------------------------------------------------------------
 */

void OpenInfoFile(void)
/*
	Ouvre la fenetre d'emission d'un fichier avec la barre de progression.
	Si mode = TRUE alors on envoit immediatement le fichier sans attendre le bouton envoyer.
*/
{
	Type_Formular_Parameter		*Parameter;
	char						L_texte[30];
	TYPE_SENDER_PARAM			SenderParameters;
	DTA							*L_Dta;
	long						LengthFile;
	int							inErreur;
	char						L_chFileName[FILENAME_LEN];

	Parameter = F_GetParameter();
	if(Parameter != NULL)
		{
			Parameter->Title	=	G_TITLE_INFOFILE;														/* Titre de la fenetre */
			Parameter->TreeName =	INFOFILE;															/* Indice de l'arbre de l'objet */
			Parameter->Mode		=	F_WIND;																/* Opening mode */
			Parameter->FirstEdit=	NOTEXT;																/* First Editable Text */
			Parameter->List		=	FormAction;															/* Event list */
			Parameter->FormIcon =	IAPPLI;																/* Icone d'iconification */
			Parameter->Mouse 	=	ARROW;			
			Parameter->Type 	=	MODELESS;
			if(!G_ResolutionChanged)
			{
				Parameter->x	=	(int)GetLongProfile(G_APP_INF, G_WIND_INFOFILE, "position_x", 0);
				Parameter->y	=	(int)GetLongProfile(G_APP_INF, G_WIND_INFOFILE, "position_y", 0);
			}

			Fsfirst(Projet.ExePathName,0);
			L_Dta		=	Fgetdta();
			LengthFile	=	L_Dta->d_length;

			inErreur	=	GetHeader(Projet.ExePathName, &SenderParameters);
			if( inErreur == JTB_ENOHEADER )
			{
				form_alert(1, GetMessage(ERROR_JAGHEADER));

				SenderParameters.StartAdr		= 	DEFAULT_TARGET_ADR;	
				SenderParameters.OffsetStart	=	DEFAULT_OFFSET;	
				SenderParameters.StartCode		=	DEFAULT_START_ADR;
			}
			else if( inErreur != JTB_EFILE )
			{
				if((WHInfoFile = F_XOpen(Parameter)) != FALSE)
				{
					/*	V‚rifi la taille de la chaine pour l'icone */
					if( strlen(Projet.ExeFileName) > 33)
					{
						memcpy( L_chFileName, Projet.ExeFileName, 29);
						L_chFileName[30]	=	0;
						strcat( L_chFileName, "...");
					}	
					else
						strcpy( L_chFileName, Projet.ExeFileName );
						
					F_WriteText(INFOFILE, I_NAME, L_chFileName);
	
					sprintf(L_texte, "%ld", LengthFile);	
					F_WriteText(INFOFILE, I_SIZE, L_texte);
					
					sprintf(L_texte, "%c%c%c%c", 	(char)(( SenderParameters.Header & 0xFF000000L) >> 24),
													(char)(( SenderParameters.Header & 0x00FF0000L) >> 16),
													(char)(( SenderParameters.Header & 0x0000FF00L) >> 8) ,
													(char)( SenderParameters.Header & 0x000000FFL) );	
					F_WriteText(INFOFILE, I_MARK, L_texte);

					sprintf(L_texte, "%d", SenderParameters.CodeOp);	
					F_WriteText(INFOFILE, I_CODE, L_texte);

					sprintf(L_texte, "$%lX", SenderParameters.StartAdr);	
					F_WriteText(INFOFILE, I_TARGET, L_texte);
	
					sprintf(L_texte, "%ld", SenderParameters.OffsetStart);	
					F_WriteText(INFOFILE, I_OFFSET, L_texte);
	
					sprintf(L_texte, "$%lX", SenderParameters.StartCode);	
					F_WriteText(INFOFILE, I_START, L_texte);
	
					sprintf(L_texte, "%ld", SenderParameters.Length);	
					F_WriteText(INFOFILE, I_LEN, L_texte);
				}
			}
			else
			{
				form_alert(1, GetMessage(ERROR_NOFILE));
			}
			free(Parameter);
		}
}

/*
 * ----------------------------------------------------------------------------------------------
 *				
 * ----------------------------------------------------------------------------------------------
 */

static void MouseEnter(void)
/*

*/
{
	if( !G_BulleAide )
		return;

	switch( F_NameObject )
	{
		case	B_SIZE:
		case	I_SIZE:		STG_Bubble( BUBBLE32, -1, -1);
							break;

		case	B_MARK:
		case	I_MARK:		STG_Bubble( BUBBLE33, -1, -1);
							break;

		case	B_TARGET:
		case	I_TARGET:	STG_Bubble( BUBBLE34, -1, -1);
							break;

		case	B_START:
		case	I_START:	STG_Bubble( BUBBLE35, -1, -1);
							break;

		case	B_OFFSET:
		case	I_OFFSET:	STG_Bubble( BUBBLE36, -1, -1);
							break;

		case	B_LEN:
		case	I_LEN:		STG_Bubble( BUBBLE37, -1, -1);
							break;

		case	B_CODE:
		case	I_CODE:		STG_Bubble( BUBBLE38, -1, -1);
							break;

		default:			break;
	}
}

/*
 * ----------------------------------------------------------------------------------------------
 *				
 * ----------------------------------------------------------------------------------------------
 */

static	void Action(void)
/*
	R‚ponse … l'action sur les boutons de la fenetre d'emission...
*/
{
	int				L_inX, L_inY, dummy;

	switch(F_NameObject)
	{
    	/*	Bouton CLOSE => Fermer fenetre */
    	case	BN_31:	
					  	wind_get(WHInfoFile, WF_CURRXYWH, &L_inX, &L_inY, &dummy, &dummy);
  	
						SetLongProfile(G_APP_INF, G_WIND_INFOFILE, "position_x", (long)L_inX);
						SetLongProfile(G_APP_INF, G_WIND_INFOFILE, "position_y", (long)L_inY);
  						F_Close(INFOFILE);
    					break;
		default:
						break;
	}
}

