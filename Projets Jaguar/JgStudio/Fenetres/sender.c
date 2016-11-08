/*
 * ----------------------------------------------------------------------------------------------
 *					Fentre emission fichier
 *
 * 	Author  : 	FAVARD Laurent
 *	File    : 	Sender.C
 *	Date    : 	10 December 1996
 *	Release : 	25 April 2000
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

/* ----------------------------------------------------------------------------------------------*/

#include	"..\modules\const.h"
#include	"..\modules\tools.h"

#ifdef	__MAGIC_OS__
#include	"..\magic.rsc\jaguar_F.H"
#endif

#ifdef	__NAES_OS__
#include	"..\naes.rsc\jaguar_F.H"
#endif

#ifdef	__TOS_OS__
#include	"..\Tos.rsc\jaguar_F.H"
#endif

/* ----------------------------------------------------------------------------------------------*/

#include	"..\modules\const.h"
#include	"..\modules\messages.h"
#include	"..\modules\Bulles.h"
#include	"..\modules\jaguar.h"
#include	"..\modules\mainmenu.h"

#include	"..\fenetres\console.h"

/* ----------------------------------------------------------------------------------------------*/

void	SenderAction(void);
void	MouseEnter(void);

static Type_Form_List
FormSender[]=	{	
					{BN_10,	SenderAction,	NULL, NULL, NULL, MouseEnter, NULL},
					{BN_11,	SenderAction,	NULL, NULL, NULL, MouseEnter, NULL},
					{HELP2,	SenderAction,	SenderAction},
					FORM_END
				};

/* ----------------------------------------------------------------------------------------------*/

int								WHSender;

static unsigned	long			G_BytesCount;						/* Nbr Octets en cours */
static unsigned long			G_BytesTotal;						/* Nbr total d'octets */

static	TYPE_SENDER_PARAM		G_SenderParameters;

/* ----------------------------------------------------------------------------------------------*/
void OpenSender(TYPE_SENDER_PARAM	*SenderParameters)
/*
	Ouvre la fenetre d'emission d'un fichier avec la barre de progression.
	Si mode = TRUE alors on envoit immediatement le fichier sans attendre le bouton envoyer.
*/
{
	Type_Formular_Parameter		*Parameter;
	OBJECT						*Arbre;
	char						L_texte[50];
	DTA							*L_Dta;
	TEDINFO		*Ted;
  

	Fsfirst(Projet.ExePathName,0);																	/* Pour fixer une zone DTA du fichier */
	L_Dta		=	Fgetdta();																/* Lecture de la DTA */		
	sprintf(L_texte, "%s (%ld octets)", Projet.ExeFileName, L_Dta->d_length);
	
	G_SenderParameters	=	*SenderParameters;

	Parameter = F_GetParameter();
	if(Parameter != NULL)
	{
		if( G_SenderParameters.Mode == L_LOAD)
			Parameter->Title		=	G_TITLE_SENDER1;														/* Titre de la fenetre */
		else
			Parameter->Title		=	G_TITLE_SENDER2;														/* Titre de la fenetre */

		Parameter->TreeName =	SENDER;															/* Indice de l'arbre de l'objet */
		Parameter->Mode		=	F_WIND;																/* Opening mode */
		Parameter->FirstEdit=	TARGET;																/* First Editable Text */
		Parameter->List		=	FormSender;															/* Event list */
		Parameter->FormIcon =	IAPPLI;																/* Icone d'iconification */
		Parameter->Type		=	MODAL;	
		Parameter->Mouse 	=	ARROW;			
		if(!G_ResolutionChanged)
		{
			Parameter->x	=	(int)GetLongProfile(G_APP_INF, G_WIND_SENDER, "position_x", 0);
			Parameter->y	=	(int)GetLongProfile(G_APP_INF, G_WIND_SENDER, "position_y", 0);
		}

		if((WHSender = F_XOpen(Parameter)) != FALSE)
		{
			rsrc_gaddr(R_TREE,SENDER,&Arbre);
			Arbre[JAUGE].ob_width = 0;
				
			Arbre[BN_11].ob_state &= ~DISABLED;
				
			/*	V‚rifi la taille de la chaine pour l'icone */
			if( strlen(Projet.ExeFileName) > 33)
			{
				memcpy( L_texte, Projet.ExeFileName, 29);
				L_texte[30]	=	0;
				strcat( L_texte, "...");
			}	
			else
				strcpy( L_texte, Projet.ExeFileName );
			F_WriteText(SENDER, FILEINFO, L_texte);
				
			sprintf(L_texte, "%lX", G_SenderParameters.StartAdr);	
			F_WriteText(SENDER, TARGET, L_texte);
			sprintf(L_texte, "%ld", G_SenderParameters.OffsetStart);	
			F_WriteText(SENDER, OFFSET, L_texte);
			sprintf(L_texte, "%lX", G_SenderParameters.StartCode);	
			F_WriteText(SENDER, START, L_texte);
			sprintf(L_texte, "%ld", G_SenderParameters.Length);	
			F_WriteText(SENDER, LENGHT, L_texte);

			Ted=(TEDINFO *) Arbre[POURCENT].ob_spec.tedinfo;
			strcpy (Ted->te_ptext,"   ");

		}
		free(Parameter);
	}
}
/* ----------------------------------------------------------------------------------------------*/
static void MouseEnter(void)
/*

*/
{
	if( !G_BulleAide )
		return;

	switch( F_NameObject )
	{
		/*	Button ANNULER */
		case	BN_10:	STG_Bubble( BUBBLE11, -1, -1);
							break;

		/*	Button VALIDER */
		case	BN_11: 	STG_Bubble( BUBBLE12, -1, -1);
							break;
	}
}
/* ----------------------------------------------------------------------------------------------*/
static void Jauge(int x,int y, int w,int h)
/*
	Incr‚mente la jauge de progession en fonction du nombre de la valeur G_BytesCount par rapport a G_BytesTotal.
	
	Les valeurs (x,y,w,h) sont le clipping AES fournie par AADS pour effectuer un Redraw sous la liste des rectangles.
*/
{
	OBJECT		*Arbre;
	TEDINFO		*Ted;
	int			Pourcent;
	char		Texte[3];
	
	rsrc_gaddr(R_TREE,SENDER,&Arbre);
  
	Arbre[JAUGE].ob_width = (unsigned int)( (unsigned long)(Arbre[BJAUGE].ob_width) * G_BytesCount / G_BytesTotal);

	Pourcent = (unsigned int)((G_BytesCount * 100L) / G_BytesTotal);
	ltoa(Pourcent,Texte, BASE10);
	Ted=(TEDINFO *) Arbre[POURCENT].ob_spec.tedinfo;
	strcpy (Ted->te_ptext,Texte);

	objc_draw(Arbre,JAUGE,2,x,y,w,h);
	objc_draw(Arbre,POURCENT,2,x,y,w,h);
}
/* ----------------------------------------------------------------------------------------------*/
static	void ToIncrementeJauge(unsigned long CountToDo, unsigned long CountPerfomed)
{
	G_BytesTotal	=	CountToDo;
	G_BytesCount	=	CountPerfomed;
	F_FreeDraw(WHSender, Jauge);
}
/* ----------------------------------------------------------------------------------------------*/
static	void SenderAction(void)
/*
	R‚ponse … l'action sur les boutons de la fenetre d'emission...
*/
{
	int				L_inX, L_inY, dummy, Ret;
	char			chTexte[10], *chStopper;
	OBJECT			*Arbre;
	unsigned long	lOffsetStart;
    TJagProgram		JagProgram; 
	TJagMemDesc		JagMemory;

	switch(F_NameObject)
	{
    	/*	Bouton ANNULER => Fermer fenetre */
    	case	BN_10:	
					  	wind_get(WHSender, WF_CURRXYWH, &L_inX, &L_inY, &dummy, &dummy);
  	
						SetLongProfile(G_APP_INF, G_WIND_SENDER, "position_x", (long)L_inX);
						SetLongProfile(G_APP_INF, G_WIND_SENDER, "position_y", (long)L_inY);
  						F_Close(SENDER);
    					break;
    
    	/* Bouton VALIDER ‚mission fichier */			
		case	BN_11:
						F_ReadText(SENDER, TARGET, chTexte);
						JagMemory.StartAddress	=	(void *)strtol(chTexte, &chStopper, BASE16); 

						F_ReadText(SENDER, OFFSET, chTexte);
						lOffsetStart	=	strtol(chTexte, &chStopper, BASE10); 

						F_ReadText(SENDER, START, chTexte);
						JagProgram.StartAddress	=	(void *)strtol(chTexte, &chStopper, BASE16); 
						JagMemory.StartAddress	=	(void *)G_SenderParameters.StartAdr;	

						graf_mouse(BUSYBEE, NULL);
						Ret	=	DiskToJag(Projet.ExePathName, lOffsetStart, &JagMemory, ToIncrementeJauge);
   						graf_mouse(ARROW, NULL);
						if( Ret != JTB_OK )
    					{
    						DispErr( Ret );
							return;
    					}

				 		/*	Griser le bouton envoyer */
				 		rsrc_gaddr(R_TREE,SENDER,&Arbre);
				 		Arbre[BN_11].ob_state |= DISABLED;
				 		F_RedrawObject(Arbre, BN_11);

						if( G_SenderParameters.Mode == L_LDRUN)
						{
	     					JagProgram.TargetAddress	=	NULL;
							JagProgram.Lenght			=	0L;

							graf_mouse(BUSYBEE, NULL);							
           		    		Ret	=	RunPRG( &JagProgram );
           		    		graf_mouse(ARROW, NULL);
           		    		
							if( Ret != JTB_OK )
								DispErr( Ret );
							else
								form_alert( 1, GetMessage( RUNOK ) );
						}
						break;

		/*	Bouton Aide contextuelle */
		case	HELP2:	STG_Open(G_HelpFile, SECTION_HELP_SENDER);
						break;
					
		default:
						break;
	}
}

