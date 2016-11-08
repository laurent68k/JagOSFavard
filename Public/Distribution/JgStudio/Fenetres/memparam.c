/*
 * ----------------------------------------------------------------------------------------------
 *
 *
 * Author  : 	FAVARD Laurent
 *	File    : 	Memparam.C
 *	Date    : 	12 June 1998
 *	Release : 	04 May 2000
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
#include	"..\modules\bulles.h"
#include	"..\modules\const.h"
#include	"..\modules\Tools.h"
#include	"..\modules\jaguar.h"
#include	"..\modules\mainmenu.h"

#include	"..\fenetres\console.h"


/*
 * ----------------------------------------------------------------------------------------------
 *		Description de la fenetre "Quitter l'application"
 * ----------------------------------------------------------------------------------------------
 */

void	Action(void);
void	MouseEnter(void);

static Type_Form_List
FormAction[]=	{	{BN_2,	Action,	NULL, NULL, NULL, MouseEnter, NULL},							/* Bouton Annuler */
					{BN_1,	Action,	NULL, NULL, NULL, MouseEnter, NULL},							/* Bouton OK */
					{HELP3,	Action,	NULL, NULL, NULL, MouseEnter, NULL},							/* Bouton HELP */
					FORM_END
				};

/*
 * ----------------------------------------------------------------------------------------------
 *			Variables
 */

static	int		WHMemParam, G_Mode;
/*
 * ----------------------------------------------------------------------------------------------
 *			Fenetre 
 * ----------------------------------------------------------------------------------------------
 */

void OpenMemParam(int Mode)
/*
	Ouvre la fenetre de configuration des paramŠtres m‚moire jaguar
	
*/
{
	Type_Formular_Parameter	*Parameter;
	OBJECT						*Arbre;
	char							L_texte[20];
  
	G_Mode	=	Mode;

	Parameter = F_GetParameter();
	if(Parameter != NULL)
	{
		if( G_Mode == L_RUN)
			Parameter->Title		=	G_TITLE_MEMPARAM1;											/* Titre de la fenetre */
		else
			Parameter->Title		=	G_TITLE_MEMPARAM2;											/* Titre de la fenetre */

		Parameter->TreeName =	MEMPARAM;															/* Indice de l'arbre de l'objet */
		Parameter->Mode		=	F_WIND;																/* Opening mode */
		Parameter->FirstEdit=	E_ADR;																/* First Editable Text */
		Parameter->List		=	FormAction;															/* Event list */
		Parameter->FormIcon =	IAPPLI;																		/* Icone d'iconification */
		Parameter->Type		=	MODAL;	
		Parameter->Mouse 	=	ARROW;			
		if(!G_ResolutionChanged)
		{
			Parameter->x	=	(int)GetLongProfile(G_APP_INF, G_WIND_MEMPARAM, "position_x", 0);
			Parameter->y	=	(int)GetLongProfile(G_APP_INF, G_WIND_MEMPARAM, "position_y", 0);
		}

		if((WHMemParam = F_XOpen(Parameter)) != FALSE)
		{
			switch( G_Mode )
			{
				case	L_RUN:

									rsrc_gaddr(R_TREE, MEMPARAM, &Arbre);
									Arbre[E_LEN].ob_flags |= HIDETREE;
									break;

				case	L_GET:
									rsrc_gaddr(R_TREE, MEMPARAM, &Arbre);
									Arbre[E_LEN].ob_flags &= ~HIDETREE;
					
									sprintf(L_texte, "%ld", DEFAULT_LEN);
									F_WriteText(MEMPARAM, E_LEN, L_texte);
									break;

				default:			form_alert(1, "[1][ unknow mode ][ok]");
			}

			sprintf(L_texte, "%lX", DEFAULT_START_ADR);	
			F_WriteText(MEMPARAM, E_ADR, L_texte);
		}
		free(Parameter);
	}
}

static void Action(void)
/*
	Suivant le bouton cliqu‚ dans la fenetre de fin d'application, assigne la variable 'Sortir'.
*/
{
	int				L_inX, L_inY, dummy, Ret;
	char			chTexte[10];
	char			*chStopper;
	char			L_chFileName[20]	=	"dump.bin";
	TJagProgram		JagProgram; 
	TJagMemDesc		JagMem;
	
	
	switch( F_NameObject )
	{
		case	BN_1:	switch( G_Mode )
						{
							case	L_RUN:F_ReadText(MEMPARAM, E_ADR, chTexte);

											JagProgram.StartAddress	=	(void *)strtol(chTexte, &chStopper, BASE16); 
											JagProgram.TargetAddress	=	NULL;
											JagProgram.Lenght			=	0L;

											graf_mouse(BUSYBEE, NULL);										
											Ret	=	RunPRG( &JagProgram );
											graf_mouse(ARROW, NULL);
											if( Ret != JTB_OK )
												DispErr( Ret );
											else
												form_alert( 1, GetMessage( RUNOK ) );
											break;			
								
							case	L_GET:F_ReadText(MEMPARAM, E_ADR, chTexte);
											JagMem.StartAddress	=	(void *)strtol(chTexte, &chStopper, BASE16); 

											F_ReadText(MEMPARAM, E_LEN, chTexte);
											JagMem.Lenght		=	strtol(chTexte, &chStopper, BASE10); 

											graf_mouse(BUSYBEE, NULL);
											Ret	=	JagToDisk(L_chFileName, &JagMem);
											graf_mouse(ARROW, NULL);
											if( Ret != JTB_OK )
												DispErr( Ret );
											else
												form_alert( 1, GetMessage( GETOK ) );
											break;

							default:		form_alert(1, "[1][ unknow mode ][ok]");
						}
						break;

		case	BN_2:  	wind_get(WHMemParam, WF_CURRXYWH, &L_inX, &L_inY, &dummy, &dummy);
  	
						SetLongProfile(G_APP_INF, G_WIND_MEMPARAM, "position_x", (long)L_inX);
						SetLongProfile(G_APP_INF, G_WIND_MEMPARAM, "position_y", (long)L_inY);

						F_Close(MEMPARAM);
						break;

		case	HELP3:switch( G_Mode )
						{
							case	L_RUN:	STG_Open(G_HelpFile, SECTION_HELP_MEMPARAM1);
											break;
							default:		STG_Open(G_HelpFile, SECTION_HELP_MEMPARAM2);
											break;
						}
						break;	
						
		default:		break;
	}						
}

static void MouseEnter(void)
/*
*/
{
	switch( F_NameObject )
	{
		/*	Bouton OK */
		case	BN_1:	switch( G_Mode )
						{
							case	L_RUN:	STG_Bubble( BUBBLE27, -1, -1);
											break;			
								
							case	L_GET:	STG_Bubble( BUBBLE28, -1, -1);
											break;
						}
						break;
						
		/*	Bouton Cancel */
		case	BN_2:	STG_Bubble( BUBBLE29, -1, -1);
						break;

						
		default:		break;
	}						
}
