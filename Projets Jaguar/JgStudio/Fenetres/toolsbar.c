/*
 * ----------------------------------------------------------------------------------------------
 *								Fenetres Barre d'outils.
 *
 * 	Author  : 	FAVARD Laurent
 *	File    : 	Toolsbar.C
 *	Date    : 	18 December 1996
 *	Release : 	18 August 1998
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
#include	"..\modules\tools.h"
#include	"..\modules\mainmenu.h"

#include	"..\fenetres\About.h"
#include	"..\fenetres\Options.h"
#include	"..\fenetres\bureau.h"
#include	"..\fenetres\sender.h"
#include	"..\fenetres\Fuji.h"

/*
 * ----------------------------------------------------------------------------------------------
 *		Description de la Barre d'outils de l'application
 * ----------------------------------------------------------------------------------------------
 */

void ToolBarAction(void);
void ToolBar(void);
void MouseEnter(void);
void MouseExit(void);

static	Type_Form_List
FormToolBox[]=	{
					{ROOT,		ToolBar,	NULL, NULL, NULL, MouseEnter, NULL},								/*	Root object Close window */
					{BN_100,ToolBarAction,	NULL, NULL, NULL, MouseEnter, MouseExit},							/*	*/
					{BN_101,ToolBarAction,	NULL, NULL, NULL, MouseEnter, MouseExit},							/*	Open executable */
					{BN_102,ToolBarAction,	NULL, NULL, NULL, MouseEnter, MouseExit},							/*	*/
					{BN_103,ToolBarAction,	NULL, NULL, NULL, MouseEnter, MouseExit},							/*	*/
					{BN_104,ToolBarAction,	NULL, NULL, NULL, MouseEnter, MouseExit},							/*	*/
					{BN_105,ToolBarAction,	NULL, NULL, NULL, MouseEnter, MouseExit},							/*	*/
					{BN_106,ToolBarAction,	NULL, NULL, NULL, MouseEnter, MouseExit},							/*	*/
					{BN_107,ToolBarAction,	NULL, NULL, NULL, MouseEnter, MouseExit},							/*	*/
					{BN_108,ToolBarAction,	NULL, NULL, NULL, MouseEnter, MouseExit},							/*	EEPROM */
					{BN_109,ToolBarAction,	NULL, NULL, NULL, MouseEnter, MouseExit},							/*	NEW */
					{BN_110,ToolBarAction,	NULL, NULL, NULL, MouseEnter, MouseExit},							/*	Changement affichage */
					{BN_111,ToolBarAction,	NULL, NULL, NULL, MouseEnter, MouseExit},							/*	OPEN source  */

					FORM_END
				};

typedef struct	{
					int	ob_x;
					int	ob_y;
					int	ob_height;
					int	ob_width;
				} TPos;
				
/*
 * ----------------------------------------------------------------------------------------------
 *			Variables
 */

int					WHToolBar;
int					ActiveToolBar;										/* ToolBar de l'application active */
int					G_BulleJagBar;

#define				DISPLAY_TOOLBOX		1
#define				DISPLAY_TOOLBAR		2
#define				N_OBJETS			11

static int			G_Display = DISPLAY_TOOLBOX;
static int			G_PrecedentDisplay = DISPLAY_TOOLBOX;
static TPos 		G_position[N_OBJETS + 1 ];
static int			PremiereFois = TRUE;

/* ----------------------------------------------------------------------------------------------*/
static void SavePosition( void )
{
	OBJECT	*Arbre;
	int		Indice;
	int		ListObjects[N_OBJETS+1] = {	R_TREE, BN_109, BN_111, BN_101, BN_103, BN_102, BN_104, 
										BN_105, BN_106, BN_107, BN_108, BN_100};

	if( PremiereFois )
	{	
		PremiereFois	=	FALSE;	
		rsrc_gaddr( R_TREE, TOOLBOX, &Arbre );

		/*	M‚moriser la taille de la form */
		G_position[ 0 ].ob_height	=	Arbre[ ListObjects[0] ].ob_height;
		G_position[ 0 ].ob_width	=	Arbre[ ListObjects[0] ].ob_width;	

		/*	M‚moriser les positions des objets */
		for(Indice = 1; Indice <= N_OBJETS; Indice++)
		{
			G_position[ Indice ].ob_x	=	Arbre[ ListObjects[Indice] ].ob_x;
			G_position[ Indice ].ob_y	=	Arbre[ ListObjects[Indice] ].ob_y;	
		}	
	}
}
/* ---------------------------------------------------------------------------------------------- */
static void DrawToolBar( void )
/*	Change l'apparence de la toolbox en version ToolBar	*/
{
	OBJECT	*Arbre;
	int		ListObjects[N_OBJETS] = {	BN_109, BN_111, BN_101, BN_103, BN_102, BN_104, 
										BN_105, BN_106, BN_107, BN_108, BN_100};
	int		Indice, x_courant, y_courant, Line1, Line2;
	int		PositionX, OffsetX;
	
	/*	Si l'affichage courant est deja en toolbar => Rien faire */
	if( G_PrecedentDisplay == G_Display )
		return;
					
	rsrc_gaddr( R_TREE, TOOLBOX, &Arbre );
	Arbre[R_TREE].ob_height	=	Arbre[R_TREE].ob_height / 2;
	Arbre[R_TREE].ob_width	=	Arbre[R_TREE].ob_width * 2;
	
	/*	Espacement en largeur entre les objets */
	OffsetX		=	Arbre[BN_111].ob_x - Arbre[BN_109].ob_x;
	PositionX	=	Arbre[BN_109].ob_x;
	
	/*	Coordonn‚es y de la premiŠre ligne */
	Line1	=	Arbre[BN_109].ob_y;
	/*	Coordonn‚es y de la deuxiŠme ligne */
	Line2	=	Arbre[BN_102].ob_y;	
	
	/*	Cr‚ation premiere ligne: */
	x_courant	=	PositionX + OffsetX;
	y_courant	=	Line1;
	for(Indice = 1; Indice < N_OBJETS; Indice++)
	{
		Arbre[ ListObjects[Indice] ].ob_x	=	x_courant;
		Arbre[ ListObjects[Indice] ].ob_y	=	y_courant;		
		
		x_courant	+=	OffsetX;
		
		if( Indice == (N_OBJETS / 2) ) 
		{
			x_courant	=	PositionX;
			y_courant	=	Line2;			
		}
	}	
}
/* ---------------------------------------------------------------------------------------------- */
static void DrawToolBox( void )
{
	OBJECT	*Arbre;
	int		Indice;
	int		ListObjects[N_OBJETS+1] = {	R_TREE, BN_109, BN_111, BN_101, BN_103, BN_102, BN_104, 
										BN_105, BN_106, BN_107, BN_108, BN_100};
		
	if( !PremiereFois && (G_PrecedentDisplay != G_Display) )
	{	
		rsrc_gaddr( R_TREE, TOOLBOX, &Arbre );

		/*	restaurer la taille de la form */
		Arbre[ ListObjects[0] ].ob_height	=	G_position[ 0 ].ob_height;
		Arbre[ ListObjects[0] ].ob_width	=	G_position[ 0 ].ob_width;	

		/*	restaurer les positions */
		for(Indice = 1; Indice <= N_OBJETS; Indice++)
		{
			Arbre[ ListObjects[Indice] ].ob_x	=	G_position[ Indice ].ob_x;
			Arbre[ ListObjects[Indice] ].ob_y	=	G_position[ Indice ].ob_y;	
		}	
	}
	
}

/*
 * ----------------------------------------------------------------------------------------------
 *			Ouverture et fermeture de la toolbar.
 * ----------------------------------------------------------------------------------------------
 */

void ToolBar(void)
/*
	Ouvre la fenetre Barre d'outils de l'application.
*/
{
	Type_Formular_Parameter		*Parameter;
	int							L_inX, L_inY, dummy;
	
	/*
	 *	Si la toolbar est d‚j… active => La d‚sactiver 
	 */
	if(ActiveToolBar)
	{
	  	wind_get(WHToolBar, WF_CURRXYWH, &L_inX, &L_inY, &dummy, &dummy);
  	
		SetLongProfile(G_APP_INF, G_WIND_TOOLBAR, "position_x", (long)L_inX);
		SetLongProfile(G_APP_INF, G_WIND_TOOLBAR, "position_y", (long)L_inY);
		SetLongProfile(G_APP_INF, G_WIND_TOOLBAR, "DisplayMode", (long)G_Display);

    	ActiveToolBar = FALSE;						
	    M_UnChecked(MAINMENU, O_BARRE);
    	F_Close(TOOLBOX);
	    return;
	}

	/*
	 *	toolbar non active => L'activer 
	 */
	Parameter = F_GetParameter();
	if(Parameter != NULL)
	{
		Parameter->TreeName =	TOOLBOX;								/* Indice de l'arbre de l'objet */
		Parameter->Title	=	G_TITLE_TOOLBAR;							/* Titre de la fenetre */
		Parameter->Mode		=	F_WIND;									/* Opening mode */
		Parameter->FirstEdit=	NOTEXT;									/* First Editable Text */
		Parameter->List		=	FormToolBox;								/* Event list */
		Parameter->FormIcon =	IAPPLI;									/* Icone d'iconification */
		Parameter->Mouse	=	MS_POINTFLATHAND;

		if(!G_ResolutionChanged)
		{
			Parameter->x	=	(int)GetLongProfile(G_APP_INF, G_WIND_TOOLBAR, "position_x", 0);
			Parameter->y	=	(int)GetLongProfile(G_APP_INF, G_WIND_TOOLBAR, "position_y", 0);
		}
		
		SavePosition();		

		G_PrecedentDisplay	=	G_Display;
		G_Display			=	(int)GetLongProfile(G_APP_INF, G_WIND_TOOLBAR, "DisplayMode", DISPLAY_TOOLBOX);
		if( G_Display == DISPLAY_TOOLBAR )
			DrawToolBar();
		else
			DrawToolBox();
		
		if((WHToolBar = F_XOpen(Parameter)) != FALSE)
		{
			M_Checked(MAINMENU, O_BARRE);
			ActiveToolBar = TRUE;										/* Compl‚mente SaveOnExit */
		}

		free(Parameter);
	}
}

/* ---------------------------------------------------------------------------------------------- */
static void MouseEnter(void)
/*
	This procedure is called when the mouse enter onto an object.
*/
{
	OBJECT	*Arbre;
	
	rsrc_gaddr(R_TREE, TOOLBOX, &Arbre);	

	switch( F_NameObject )
	{
		/*	Bouton QUITTER */
		case	BN_100:	
		case	BN_101:	
		case	BN_102:	
		case	BN_103:	
		case	BN_104:
		case	BN_105:
		case	BN_106:	
		case	BN_107:
		case	BN_108:
		case	BN_109:
		case	BN_110:
		case	BN_111:
						Arbre[F_NameObject].ob_state |= SELECTED;
						F_RedrawObject( Arbre, F_NameObject );
						break;
								
		default:		
						break;
	}

	if( G_BulleJagBar )
	{
		switch(F_NameObject)
		{
			/*	Bouton QUITTER */
			case	BN_100:	STG_Bubble( BUBBLE1, -1, -1);
							break;

			/*	OPEN file */
			case	BN_101:	STG_Bubble( BUBBLE2, -1, -1);
							break;
						
			/*	Load binaries */
			case	BN_102:	STG_Bubble( BUBBLE3, -1, -1);
							break;

			/*	Inquire command */
			case	BN_103:	STG_Bubble( BUBBLE4, -1, -1);
							break;

			/*	Run program  */
			case	BN_104:	STG_Bubble( BUBBLE5, -1, -1);
							break;

			/*	Load and Run after */
			case	BN_105:	STG_Bubble( BUBBLE6, -1, -1);
							break;

			/*	Bouton HELP ST-Guide sommaire */
			case	BN_106:	STG_Bubble( BUBBLE7, -1, -1);
							break;

			/*	Get jaguar memory block */
			case	BN_107:	STG_Bubble( BUBBLE8, -1, -1);
							break;

			/*	Serial Eprom */
			case	BN_108:	STG_Bubble( BUBBLE9, -1, -1);
							break;
		
			case	BN_109:	STG_Bubble( BUBBLE50, -1, -1);
							break;
							
			case	BN_111:	STG_Bubble( BUBBLE49, -1, -1);
							break;

			default:		SpeechSmiley( BUBBLE10 );
							break;
		}
	}
}

/* ---------------------------------------------------------------------------------------------- */
static void MouseExit(void)
/*
	This procedure is called when the mouse exit from an object.
*/
{
	OBJECT	*Arbre;
	
	rsrc_gaddr(R_TREE, TOOLBOX, &Arbre);	
	switch( F_NameObject )
	{
		/*	Bouton QUITTER */
		case	BN_100:	
		case	BN_101:	
		case	BN_102:	
		case	BN_103:	
		case	BN_104:
		case	BN_105:
		case	BN_106:	
		case	BN_107:
		case	BN_108:
		case	BN_109:
		case	BN_110:
		case	BN_111:
						Arbre[F_NameObject].ob_state &= ~SELECTED;
						F_RedrawObject( Arbre, F_NameObject );
						break;
								
		default:		
						break;
	}
}

/* ---------------------------------------------------------------------------------------------- */
static void ToolBarAction(void)
/*
	Prise en compte des actions sur les boutons de la JagBarre.Certains boutons appelent directement des proc‚dures dans
	la d‚claration des liste AADS.
*/
{
	int		inDisplay;
	
	switch(F_NameObject)
	{
		/*	Bouton QUITTER */
		case	BN_100:	M_Entry	=	F_QUIT;
						Fichier();	
						break;

		/*	OPEN Executable file */
		case	BN_101:	M_Entry	=	F_OPENE;
						Fichier();	
    					break;
						
		/*	Load binaries */
		case	BN_102:	M_Entry	=	L_LOAD;
						Connect();	
    					break;

		/*	Inquire command */
		case	BN_103:	M_Entry	=	L_PING;
						Connect();	
						break;

		/*	Run program  */
		case	BN_104:	M_Entry	=	L_RUN;
						Connect();
						break;

		/*	Load and Run after */
		case	BN_105:	M_Entry	=	L_LDRUN;
						Connect();
   						break;

		/*	Bouton HELP ST-Guide sommaire */
		case	BN_106:	M_Entry	=	AIDE;
						Aide();
						break;

		/*	Get jaguar memory block */
		case	BN_107:	M_Entry	=	L_GET;
						Connect();
						break;

		/*	Serial Eprom */
		case	BN_108:	M_Entry	=	T_EPROM;
						Outils();
						break;
						
		/*	NEW item */
		case	BN_109:	M_Entry	=	F_NEW;
						Fichier();
						break;
						
		/*	OPEN item */
		case	BN_111:	M_Entry	=	F_OPEN;
						Fichier();
						break;

		case	BN_110:	inDisplay	=	G_Display;
						if( G_Display == DISPLAY_TOOLBOX )
							G_Display = DISPLAY_TOOLBAR;
						else
							G_Display = DISPLAY_TOOLBOX;						
						ToolBar();			/*	Provoque la fermeture en enregistrant le nouvel affichage */
						G_Display	=	inDisplay;
						/*ToolBar();	*/		/*	Provoque l'ouverture avec le nouvel affichage enregistr‚ */							
						break;
						
		default:		break;
	}
}

