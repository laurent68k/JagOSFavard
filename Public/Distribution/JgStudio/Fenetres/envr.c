/*
 * ----------------------------------------------------------------------------------------------
 *
 *
 * Author  : 	FAVARD Laurent
 *	File    : 	Envr.C
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
void	MouseEnter(void);

static Type_Form_List
FormAction[]=	{

					{BN_46,		Action,	NULL, NULL, NULL, MouseEnter, NULL},							/* Bouton Close */
					{EEDITER,	NULL,	DbClic, NULL, NULL, MouseEnter, NULL},							/* Bouton Close */
					{EASM,		NULL,	DbClic, NULL, NULL, MouseEnter, NULL},							/* Bouton Close */
					{HELP8,		Action,	NULL},															/* Bouton HELP */
					FORM_END
				};

/*
 * ----------------------------------------------------------------------------------------------
 *			Variables
 */

static	int						WHEnvr;

/*
 * ----------------------------------------------------------------------------------------------
 *			Fenetre 
 * ----------------------------------------------------------------------------------------------
 */

void Envr( void )
/*
	Ouvre la fenetre de configuration des programmes externes
	
*/
{
	Type_Formular_Parameter	*Parameter;

	Parameter = F_GetParameter();
	if(Parameter != NULL)
	{
		Parameter->Title	=	G_TITLE_ENVR;
		Parameter->TreeName =	ENVR;															/* Indice de l'arbre de l'objet */
		Parameter->Mode		=	F_WIND;																/* Opening mode */
		Parameter->FirstEdit=	EEDITER;																/* First Editable Text */
		Parameter->List		=	FormAction;															/* Event list */
		Parameter->FormIcon =	IAPPLI;																		/* Icone d'iconification */
		Parameter->Type		=	MODELESS;
		Parameter->Mouse 	=	ARROW;			
		if(!G_ResolutionChanged)
		{
			Parameter->x	=	(int)GetLongProfile(G_APP_INF, G_WIND_ENVR, "position_x", 0);
			Parameter->y	=	(int)GetLongProfile(G_APP_INF, G_WIND_ENVR, "position_y", 0);
		}

		if((WHEnvr = F_XOpen(Parameter)) != FALSE)
		{
			F_WriteText( ENVR, EEDITER, External.EditFileName );
			F_WriteText( ENVR, EASM, External.AsmFileName );
		}
		free(Parameter);
	}
}

static void MouseEnter(void)
/*

*/
{
	if( !G_BulleAide )
		return;

	switch( F_NameObject )
	{
		/*	Button ACCEPT */
		case	EEDITER:STG_Bubble( "", -1, -1);
						break;

		/*	Button Deny */
		case	EASM: 	STG_Bubble( "", -1, -1);
						break;
	}
}

static void DbClic(void)
/*
	Suivant l'objet double cliqu‚...
*/
{
	OBJECT	*Arbre;
	char	L_chFileName[FILENAME_LEN];

	rsrc_gaddr( R_TREE, FBUREAU, &Arbre);
	switch( F_NameObject )
	{
		/*	Chemin Editeur */
		case	EEDITER:if( A_FileSelector(	External.EditPathName, External.EditFileName, EXT_PRG, 
				    					External.EditPath, G_SELECT_EDITER) )
				    	{
							SetProfileString(G_APP_INF, EXTERNAL, "EditPathName", External.EditPathName);
							SetProfileString(G_APP_INF, EXTERNAL, "EditFileName", External.EditFileName);
							SetProfileString(G_APP_INF, EXTERNAL, "EditPath", External.EditPath);

							if( strlen( External.EditFileName ) > FILE_ICON_LEN)
							{
								memcpy( L_chFileName, External.EditFileName, FILE_ICON_LEN - 4);
								L_chFileName[FILE_ICON_LEN - 3]	=	0;
								strcat( L_chFileName, "...");
							}	
							else
								strcpy( L_chFileName, External.EditFileName );
								
							Arbre[IEDIT].ob_flags &= ~HIDETREE;
							strcpy(Arbre[IEDIT].ob_spec.iconblk->ib_ptext, L_chFileName);
							form_dial(FMD_FINISH,0,0,0,0,Arbre[IEDIT].ob_x,Arbre[IEDIT].ob_y,Arbre[IEDIT].ob_width,Arbre[IEDIT].ob_height*2);
							
							F_WriteText( ENVR, EEDITER, L_chFileName );
				    	}
						break;
						
		/*	Chemin Assembleur */
		case	EASM:	if( A_FileSelector(	External.AsmPathName, External.AsmFileName, EXT_PRG, 
				    					External.AsmPath, G_SELECT_ASM) )
				    	{
							SetProfileString(G_APP_INF, EXTERNAL, "AsmPathName", External.AsmPathName);
							SetProfileString(G_APP_INF, EXTERNAL, "AsmFileName", External.AsmFileName);
							SetProfileString(G_APP_INF, EXTERNAL, "AsmPath", External.AsmPath);

							if( strlen( External.AsmFileName ) > FILE_ICON_LEN)
							{
								memcpy( L_chFileName, External.AsmFileName, FILE_ICON_LEN - 4);
								L_chFileName[FILE_ICON_LEN - 3]	=	0;
								strcat( L_chFileName, "...");
							}	
							else
								strcpy( L_chFileName, External.AsmFileName );
								
							Arbre[IASM].ob_flags &= ~HIDETREE;
							strcpy(Arbre[IASM].ob_spec.iconblk->ib_ptext, L_chFileName);
							form_dial(FMD_FINISH,0,0,0,0,Arbre[IASM].ob_x,Arbre[IASM].ob_y,Arbre[IASM].ob_width,Arbre[IASM].ob_height*2);

							F_WriteText( ENVR, EASM, L_chFileName );
				    	}
				    	break;

		default:		break;
	}						
}

static void Action(void)
/*
	Suivant le bouton cliqu‚ dans la fenetre.
*/
{
	int				L_inX, L_inY, dummy;

	switch( F_NameObject )
	{
		/*	Button Close */
		case	BN_46: 	MS_Mouse(MS_SLEEP, NULL);
						
						wind_get(WHEnvr, WF_CURRXYWH, &L_inX, &L_inY, &dummy, &dummy);
  	
						SetLongProfile(G_APP_INF, G_WIND_ENVR, "position_x", (long)L_inX);
						SetLongProfile(G_APP_INF, G_WIND_ENVR, "position_y", (long)L_inY);

						F_Close(ENVR);
						MS_Mouse(ARROW, NULL);
						break;
						
		/*	Button HELP */
		case	HELP8:	STG_Open(G_HelpFile, SECTION_HELP_ENVR);	
						break;

		default:		break;
	}						
}
