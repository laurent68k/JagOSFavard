/*
 * ----------------------------------------------------------------------------------------------
 *								Fenetre OPTIONS.
 *
 * 	Author  : 	FAVARD Laurent
 *	File    : 	Options.C
 *	Date    : 	18 December 1996
 *	Release : 	30 Juillet 1997
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

#ifdef	__TOS_OS__
#include	"..\Tos.rsc\jaguar_F.H"
#endif
	
/*
 * ----------------------------------------------------------------------------------------------
 *			Fichiers sp‚cifiques … inclure
 */

#include	"..\modules\jaguar.h"
#include	"..\modules\messages.h"
#include	"..\modules\bulles.h"
#include	"..\modules\const.h"
#include	"..\modules\mainmenu.h"

#include	"..\fenetres\Fuji.h"
#include	"..\fenetres\Console.h"

/*
 * ----------------------------------------------------------------------------------------------
 *		Description de la fenetre "Param‚trage..."
 * ----------------------------------------------------------------------------------------------
 */

void	OptionsAction(void);
void	DoubleClic(void);
void	MouseEnter(void);

static	Type_Form_List
FormOptions[]=	{
			{ROOT,		OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},
			{BN_5,		OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/* Bouton FERMER */
			{BN_6,		OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/* Auto TOP fenetre */
			{BN_7,		OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/* Sur souris fixe */
			{BN_8,		OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/* Sur toute la fenetre */
			{BN_9,		OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/* Effets visuels fenetres */

			/*	Bouton correspondant aux trames choisies */
			{TRAME_1,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/* Trame du bureau */
			{TRAME_2,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */
			{TRAME_3,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */
			{TRAME_4,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */
			{TRAME_5,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */
			{TRAME_6,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */
			{TRAME_7,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */
			{TRAME_8,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */

			/*	Bouton correspondant aux couleurs choisies */
			{COLOR_1,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*	Couleur du bureau */
			{COLOR_2,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */
			{COLOR_3,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */
			{COLOR_4,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */
			{COLOR_5,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */
			{COLOR_6,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */
			{COLOR_7,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */
			{COLOR_8,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */
			{COLOR_9,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */
			{COLOR_10,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */
			{COLOR_11,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */
			{COLOR_12,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */
			{COLOR_13,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */
			{COLOR_14,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */
			{COLOR_15,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */
			{COLOR_16,	OptionsAction,	NULL, NULL, NULL, MouseEnter, NULL},						/*  */

			/*	Gestion boutons HAUT et BAS temps */
			{BN_15,		OptionsAction,	DoubleClic, NULL, NULL, MouseEnter, NULL},					
			{BN_16,		OptionsAction,	DoubleClic, NULL, NULL, MouseEnter, NULL},					
			{HELP1,		OptionsAction,	DoubleClic, NULL, NULL, MouseEnter, NULL},					
			
			FORM_END
		};


static Type_Parameter_PopAttach											/* Popup menu */
ListPopAttach[]={
		  			{POPUP4, FNTCL	,COWHITE, COLGREY, TRUE},
					POPATTACH_END
	        	};

void Popup4( void );
static Type_Parameter_PopEvent												/* Code for each item */
ListPopEvent[]=	{	
		  			{POPUP4, COWHITE,	Popup4},						/* */
					{POPUP4, COBLACK,	Popup4},						/* */
					{POPUP4, CORED,		Popup4},						/* */
					{POPUP4, COGREEN,	Popup4},						/* */
					{POPUP4, COBLUE,	Popup4},						/* */
					{POPUP4, COCYAN,	Popup4},						/* */
					{POPUP4, COYELLOW,	Popup4},						/* */
					{POPUP4, COMAGENT,	Popup4},						/* */
					{POPUP4, COLGREY,	Popup4},						/* */
					{POPUP4, CODGREY,	Popup4},						/* */
					{POPUP4, CODRED,	Popup4},						/* */
					{POPUP4, CODGREEN,	Popup4},						/* */
					{POPUP4, CODBLUE,	Popup4},						/* */
					{POPUP4, CODCYAN,	Popup4},						/* */
					{POPUP4, CODYELLO,	Popup4},						/* */
					{POPUP4, CODMAGEN,	Popup4},						/* */
  		  			POPEVENT_END  			
  				};  				   

/*
 * ----------------------------------------------------------------------------------------------
 *			Variables
 */

int					WHOptions;												/*	window handle */

Type_AutoTop		AutoTop;												/* Auto top fenetre d‚finitions actif*/
int					Effets;													/* Effets visuels sur les fenetres */
static	long		Trame;
static	int			AncienBtnColor	=	0;
static	int			AncienBtnTrame	=	0;

/*
 * ----------------------------------------------------------------------------------------------
 *			Fenetre "Param‚trages" du menu option l'application
 * ----------------------------------------------------------------------------------------------
 */

void Options(void)
/*
	Ouvre la fenetre "Param‚trage..." de l'application.
*/
{
	Type_Formular_Parameter	*Parameter;
	int						L_inButton;
	OBJECT					*Arbre;
	char		L_sTexte[40];

	  
	rsrc_gaddr(R_TREE, FOPTIONS, &Arbre);
  
  Parameter = F_GetParameter();
  if(Parameter != NULL)
  {
    Parameter->TreeName =	FOPTIONS;											/* Indice de l'arbre de l'objet */
    Parameter->Title	=	G_TITLE_PARM;										/* Titre de la fenetre */
    Parameter->Mode		=	F_WIND;											/* Opening mode */
    Parameter->FirstEdit=	NOTEXT;											/* First Editable Text */
    Parameter->List		=	FormOptions;										/* Event list */
    Parameter->FormIcon =	IAPPLI;											/* Icone d'iconification */
    Parameter->Mouse	=	MS_MOUSE;											/* Curseur souris */
    
	if(!G_ResolutionChanged)
	{
   		Parameter->x	=	(int)GetLongProfile(G_APP_INF, G_WIND_OPTIONS, "position_x", 0);
		Parameter->y	=	(int)GetLongProfile(G_APP_INF, G_WIND_OPTIONS, "position_y", 0);
	}

    if((WHOptions = F_XOpen(Parameter)) != FALSE)
    {
  		Trame					=	DesktopAddress[0].ob_spec.index;
  		
  		/*	Re-check dialog's buttons */
		AncienBtnColor = (int)(0x0000000FL & Trame) + COLOR_1;
		Arbre[AncienBtnColor].ob_state	|=	CHECKED;					/*	Color selected */


		AncienBtnTrame = (int)((0x00000070L & Trame) >> 4) + TRAME_1;
		Arbre[AncienBtnTrame].ob_state	|=	CHECKED;					/*	Color selected */
		
		/* window auto top */
		F_Selected(FOPTIONS, BN_6, AutoTop.AutoEnable);			

		/*	no mouse movment */
		F_Selected(FOPTIONS, BN_7, AutoTop.NoMouseMovement);			

		/* work area only */
		F_Selected(FOPTIONS, BN_8, AutoTop.OnlyWorkArea);			

		/* visual clude effects */
		F_Selected(FOPTIONS, BN_9, Effets);		

		/*	cacher les boutons de couleurs suivant la resolution courante */	
		switch(VdiInfo.NombreCouleurs)
		{
			case	2:	for(L_inButton = COLOR_3; L_inButton <= COLOR_16; L_inButton++)
							Arbre[L_inButton].ob_flags	|=	HIDETREE;
						break;

			case	4:	for(L_inButton = COLOR_5; L_inButton <= COLOR_16; L_inButton++)
							Arbre[L_inButton].ob_flags	|=	HIDETREE;
						break;

			default:
						break;
		}

		sprintf(L_sTexte, "%d", G_SizeFont); 
		F_WriteText(FOPTIONS, SZFNT,   L_sTexte);

		ListPopAttach[0].StartEntry	=	G_ColorFont + COWHITE;
		
		/*	Attacher le menu popup */	
		F_PopAttach(FOPTIONS, ListPopAttach);
		F_PopEvent(ListPopEvent);
    }
    free(Parameter);
  }
}

/*
 * ----------------------------------------------------------------------------------------------
 *			Action sur la fenetre
 * ----------------------------------------------------------------------------------------------
 */

static void DoubleClic(void)
{
	char		L_sTexte[40];
	int			dummy;
	
	switch(F_NameObject)
	{
		/*	Bouton BAS */
		case BN_15:
					G_SizeFont = 8;
					sprintf(L_sTexte, "%d", G_SizeFont); 
					F_WriteText(FOPTIONS, SZFNT,   L_sTexte);

					vst_point( VdiHandle, G_SizeFont, &dummy, &dummy, &dummy, &dummy );
					if( ActiveConsole )
						W_Redraw(WHandleConsole);					
					break;

		/*	Bouton HAUT */
		case BN_16:
					G_SizeFont = 24;
					sprintf(L_sTexte, "%d", G_SizeFont); 
					F_WriteText(FOPTIONS, SZFNT,   L_sTexte);

					vst_point( VdiHandle, G_SizeFont, &dummy, &dummy, &dummy, &dummy );
					if( ActiveConsole )
						W_Redraw(WHandleConsole);					
					break;

		default:	break;

	}
}

void OptionsAction(void)
/*
	R‚ponse … l'action sur les boutons de la fenetre "Param‚trage..."
*/
{
  	OBJECT		*Arbre;
  	int			L_inX, L_inY, dummy;
	char		L_sTexte[40];

	rsrc_gaddr(R_TREE, FOPTIONS, &Arbre);

	switch(F_NameObject)
	{
		/*	Bouton BAS */
		case BN_15:
						if(G_SizeFont - 1 >= 8)
						{
							G_SizeFont--;
							sprintf(L_sTexte, "%d", G_SizeFont); 
							F_WriteText(FOPTIONS, SZFNT, L_sTexte);

							vst_point( VdiHandle, G_SizeFont, &dummy, &dummy, &dummy, &dummy );
							if( ActiveConsole )
								W_Redraw(WHandleConsole);					
						}
						break;
		
		/*	Bouton HAUT */
		case BN_16:
		
						if(G_SizeFont + 1 <= 24)
						{
							G_SizeFont++;
							sprintf(L_sTexte, "%d", G_SizeFont); 
							F_WriteText(FOPTIONS, SZFNT, L_sTexte);

							vst_point( VdiHandle, G_SizeFont, &dummy, &dummy, &dummy, &dummy );
							if( ActiveConsole )
								W_Redraw(WHandleConsole);					
						}
						break;
		
		 /*	Bouton FERMER => Fermer fenetre */
		 case	BN_5:	
						wind_get(WHOptions, WF_CURRXYWH, &L_inX, &L_inY, &dummy, &dummy);
		
						SetLongProfile(G_APP_INF, G_WIND_OPTIONS, "position_x", (long)L_inX);
						SetLongProfile(G_APP_INF, G_WIND_OPTIONS, "position_y", (long)L_inY);
						WHOptions	=	0;
						F_Close(FOPTIONS);
						break;
					
		 case	BN_6:	AutoTop.AutoEnable = 0x0001 & ~(AutoTop.AutoEnable);
						F_Selected(FOPTIONS, BN_6, AutoTop.AutoEnable);
						W_AutoTop(&AutoTop);
						break;
		
		 case	BN_7:	AutoTop.NoMouseMovement = 0x0001 & ~(AutoTop.NoMouseMovement);
						F_Selected(FOPTIONS, BN_7, AutoTop.NoMouseMovement);
						W_AutoTop(&AutoTop);
						break;
		
		 case	BN_8:	AutoTop.OnlyWorkArea = 0x0001 & ~(AutoTop.OnlyWorkArea);
						F_Selected(FOPTIONS, BN_8, AutoTop.OnlyWorkArea);
						W_AutoTop(&AutoTop);
						break;
		 case	BN_9:	Effets = 0x0001 & ~(Effets);
						F_Selected(FOPTIONS, BN_9, Effets);
						W_VisualClue(Effets);											/* Set effets visuels des fenetres */
						break;
	
		case	TRAME_1: case TRAME_2: case TRAME_3: case TRAME_4:
		case	TRAME_5: case TRAME_6: case TRAME_7: case TRAME_8:
						Trame = (0xFFFFFF8FL & Trame ) | ((0x0000000FL & (F_NameObject - TRAME_1)) << 4);
						DesktopAddress[0].ob_spec.index = Trame;
						
						if( AncienBtnTrame  )
							Arbre[AncienBtnTrame].ob_state	&=	~CHECKED;
							
						Arbre[F_NameObject].ob_state	|=	CHECKED;
						AncienBtnTrame	=	F_NameObject;	
						
						form_dial(FMD_FINISH,0,0,0,0,DesktopAddress[0].ob_x,DesktopAddress[0].ob_y,DesktopAddress[0].ob_width,DesktopAddress[0].ob_height);
						break;
	
		case	COLOR_1: case	COLOR_2: case	COLOR_3: case	COLOR_4:
		case	COLOR_5: case	COLOR_6: case	COLOR_7: case	COLOR_8:
		case	COLOR_9: case	COLOR_10: case	COLOR_11: case	COLOR_12:
		case	COLOR_13: case	COLOR_14: case	COLOR_15: case	COLOR_16:
	
						Trame = (0xFFFFFFF0L & Trame) | (0x0000000FL & (F_NameObject - COLOR_1));
						if( AncienBtnColor  )
							Arbre[AncienBtnColor].ob_state	&=	~CHECKED;
							
						Arbre[F_NameObject].ob_state	|=	CHECKED;
						AncienBtnColor	=	F_NameObject;	
						
						DesktopAddress[0].ob_spec.index = Trame;
						form_dial(FMD_FINISH,0,0,0,0,DesktopAddress[0].ob_x,DesktopAddress[0].ob_y,DesktopAddress[0].ob_width,DesktopAddress[0].ob_height);
						
						break;
				
		case	HELP1:	STG_Open(G_HelpFile, SECTION_HELP_OPTIONS);
						break;
							
	 	default:		break;
	}
}

void Popup4( void )
{
	if(F_PopupName == POPUP4)
	{
		G_ColorFont	=	F_PopupEntry - COWHITE;
		vst_color( VdiHandle, G_ColorFont );
		if( ActiveConsole )
			W_Redraw(WHandleConsole);					
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
		/*	Button Trames */
		case	TRAME_1: 
						STG_Bubble( BUBBLE17, -1, -1);
						break;

		case TRAME_2: case TRAME_3: case TRAME_4:
		case	TRAME_5: case TRAME_6: case TRAME_7: case TRAME_8:
						break;
						
		/*	Button COULEURS */
		case	COLOR_1: 
						STG_Bubble( BUBBLE18, -1, -1);
						break;
						
		case	COLOR_2: case	COLOR_3: case	COLOR_4:
		case	COLOR_5: case	COLOR_6: case	COLOR_7: case	COLOR_8:
		case	COLOR_9: case	COLOR_10: case	COLOR_11: case	COLOR_12:
		case	COLOR_13: case	COLOR_14: case	COLOR_15: case	COLOR_16:
						break;
						
		/* Bouton FERMER */
		case	BN_5:		
						STG_Bubble( BUBBLE19, -1, -1);
						break;
						
		/* Auto TOP fenetre */
		case	BN_6:	
						STG_Bubble( BUBBLE20, -1, -1);
						break;
		case	BN_7:						/* Sur souris fixe */
						STG_Bubble( BUBBLE21, -1, -1);
						break;
		case	BN_8:
						STG_Bubble( BUBBLE22, -1, -1);
						break;
		case	BN_9:
						STG_Bubble( BUBBLE23, -1, -1);
						break;
			
		case	BN_15:			
						STG_Bubble( BUBBLE24, -1, -1);
						break;
		case	BN_16:					
						STG_Bubble( BUBBLE25, -1, -1);
						break;

		default:		
						break;

	}
}
