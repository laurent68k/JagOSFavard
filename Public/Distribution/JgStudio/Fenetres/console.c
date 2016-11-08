/*
 * ----------------------------------------------------------------------------------------------
 *					APPLICATION JagXServ sous A.A.D.S pour Atari FALCON.
 *
 * 	Author  : 	FAVARD Laurent
 *	File    : 	Jaquar.C
 *	Date    : 	09 January 1997
 *	Release : 	12 September 1997
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
#include <tos.h>
#include <string.h>

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

#include	"..\modules\messages.h"															/*	Messages textes */
#include	"..\modules\jaguar.h"
#include	"..\modules\const.h"
#include	"..\modules\mainmenu.h"

#include	"..\fenetres\About.h"

#define	CLIPBOARD_WRITE		"C:\\clipbrd\\scrap.txt"
#define	CLIPBOARD_READ		"C:\\clipbrd\\scrap.txt" 

/*
 * ----------------------------------------------------------------------------------------------
 *		Description de la Barre de menu de la fenetre console.
 * ----------------------------------------------------------------------------------------------
 */

void MenuWind(int WindowHandle);

static Type_Window_Menu_List
MenuList[]=	{	
				{M_1	,INITIAL,	MenuWind},
				{M_2	,INITIAL,	MenuWind},
				{M_3	,INITIAL,	MenuWind},
				{M_4	,INITIAL,	MenuWind},
				{M_5	,INITIAL,	MenuWind},
				{M_6	,INITIAL,	MenuWind},
				{M_7	,INITIAL,	MenuWind},
				{M_8	,INITIAL,	MenuWind},
				{M_9	,INITIAL,	MenuWind},
				{M_10	,INITIAL,	MenuWind},
				WMENU_END
			};

/*
 * ----------------------------------------------------------------------------------------------
 */

#define RAM_ERROR      -1		/* Lire_Fichier 	*/
#define OK				1

/*
 * ----------------------------------------------------------------------------------------------
 */
 
long				G_LigneCouranteConsole	=	-1L;
long				G_NbreLigneConsole	=	0L;									/* nombre de lignes totales  */
int					G_NbreColonneConsole	=	0;									/* nombre de colonnes */
char				*G_TexteConsole[MAX_LINE_CONSOLE];								/* liste des lignes du texte */

int					tabulation 				=	8;
long				G_LigneDepart			=	0L;									/* ligne de depart ou afficher */
int					G_ColonneDepart			=	0;									/* colonne de depart ou afficher */

int					WHandleConsole;													/* La fenetre terminal */
int					ActiveConsole 			=	0;

int					G_ConsoleModifie = FALSE;

/*
 * ----------------------------------------------------------------------------------------------
 */
 
static void RedrawWorkArea(int handle)
{
	int	message[8];

	W_WorkXYWH(handle, &message[4], &message[5], &message[6], &message[7]);
	message[0]	=	WM_REDRAW;												/* Send a REDRAW message to himself */
	message[1]	=	AesHandle;												/* Application source's handle is the target application */
	message[2]	=	0;
	message[3]	=	handle;													/* target window's handle */
	appl_write(AesHandle, 16, message);									/* Send message... */
}

static void SetVSlider(int handle)
/*
  Procedure calculant la taille et la position du slider vertical:
  - Appelee par le gestionnaire lors d'un full window.
  - Par le procedures U/Down page/line.
*/
{
	if( (long)W_GMaxLine(handle) >= G_NbreLigneConsole )							/* Petit document */
		wind_set(handle,WF_VSLIDE,0,0,0,0);											/* inferieur fenetre */
	else																			/* Doc. sup. fenetre */
		wind_set(handle, WF_VSLIDE, (int)((long)1000 * ((long)G_LigneDepart) / ((long)G_NbreLigneConsole - (long)W_GMaxLine(handle))), 0, 0, 0);   
		
	if(G_NbreLigneConsole)
		wind_set(handle, WF_VSLSIZE, (int)((long)1000 * (long)W_GMaxLine(handle) / (long)G_NbreLigneConsole), 0, 0, 0);
	else
		wind_set(handle, WF_VSLSIZE, 1000 , 0, 0, 0);	
}

static void WUpline(int handle)
/*
	Fleche de Scrolling Vers le Haut
*/
{
	int 	table[8];
	int	x,y,w,h;
	long  dummy=0L;
	int	hauteur_cell;
	int	max_line;
	int	handle_actif;
	
	hauteur_cell = A_GCellHeigh();
	max_line     = W_GMaxLine(handle);
	wind_get(handle,WF_TOP,&handle_actif,0,0,0);
	if(G_LigneDepart > 0L && handle == handle_actif)
	{
		G_LigneDepart--;
		
		W_WorkXYWH(handle, &x, &y, &w, &h);
		table[0]=x;				/* Zone Source */
		table[1]=y;
		table[2]=x+w-1;
		table[3]=y+(max_line-1) * hauteur_cell;
		table[4]=x;				/* Destination */
		table[5]=y+hauteur_cell;
		table[6]=x+w-1;
		table[7]=y+max_line * hauteur_cell;
		vro_cpyfm(VdiHandle,3,table,(MFDB *)&dummy,(MFDB *)&dummy);
		
		table[0] = x;			/* Effacer premiere ligne */
		table[1] = y;
		table[2] = x+w-1;
		table[3] = y+hauteur_cell;
		vsf_interior(VdiHandle,0);
		vsf_perimeter(VdiHandle,0);
		v_bar(VdiHandle,table);
		vsf_perimeter(VdiHandle,1);

		W_HomeCursor(handle);			/* Cursueur en derniere ligne */
		table[0] = x;			
		table[1] = y; 
		table[2] = x+w-1;
		table[3] = y+hauteur_cell;
		W_OnClip(table); 
		W_Printf(handle,G_TexteConsole[G_LigneDepart]);
		W_OffClip(table); 
		SetVSlider(handle);
	}
}

static void WDnline(int handle)
/*
	Fleche de Scrolling Vers le Bas
*/
{
	int 	table[8];
	int	x,y,w,h;
	long  dummy=0L;
	long  ligne;
	int	hauteur_cell;
	int	max_line;
	int	handle_actif;
	
	hauteur_cell = A_GCellHeigh();
	max_line     = W_GMaxLine(handle);
	wind_get(handle,WF_TOP,&handle_actif,0,0,0);
	if(G_LigneDepart + W_GMaxLine(handle) < G_NbreLigneConsole && handle == handle_actif)
	{
		/*	Deplacer le bloc de texte vers le haut */
		wind_set(handle,WF_TOP,0,0,0,0);
		G_LigneDepart++;
		W_WorkXYWH(handle, &x, &y, &w, &h);
		table[0]=x;				/* Scroller les lignes */
		table[1]=y+hauteur_cell;
		table[2]=x+w-1;
		table[3]=y+ (max_line * hauteur_cell);
		table[4]=x;
		table[5]=y;
		table[6]=x+w-1;
		table[7]=y+(max_line-1) * hauteur_cell;
		vro_cpyfm(VdiHandle,3,table,(MFDB *)&dummy,(MFDB *)&dummy);

		/* Effacer derniere ligne */
		table[0] = x;															
		table[1] = y +(max_line-1) * hauteur_cell+1;
		table[2] = x+w-1;
		table[3] = y+h-1;
		vsf_interior(VdiHandle,0);
		vsf_perimeter(VdiHandle,0);
		v_bar(VdiHandle,table);
		vsf_perimeter(VdiHandle,1); 

		ligne = G_LigneDepart + max_line - 1;
		
		/* Clipping et ecrire nouvelle ligne */
		W_SLine(handle,max_line);
		table[0] = x;			
		table[1] = y + 1;
		table[2] = x+w-1;
		table[3] = y+h-1;
		W_OnClip(table);     
		W_Printf(handle,G_TexteConsole[ligne]);
		W_OffClip(table); 
		SetVSlider(handle);
	}
}

static void WUppage(int handle)
/*
  Scrolling Haut d'une page fenetre
*/
{
	if(G_LigneDepart-(long)W_GMaxLine(handle) > 0L)
	{
		G_LigneDepart-=(long)W_GMaxLine(handle);
		RedrawWorkArea(handle);
		SetVSlider(handle);
	}
	else if( G_LigneDepart > 0L)
	{
		G_LigneDepart = 0L;
		RedrawWorkArea(handle);
		SetVSlider(handle);
	}
}

static void WDnpage(int handle)
/*
  Scrolling Bas d'une page fenetre
*/
{
	if( G_NbreLigneConsole )
	{
  		if(G_LigneDepart+2L*(long)W_GMaxLine(handle) < G_NbreLigneConsole)
  		{
    		G_LigneDepart+=(long)W_GMaxLine(handle);
    		RedrawWorkArea(handle);
    		SetVSlider(handle);
  		}
  		else
  		{
    		G_LigneDepart=G_NbreLigneConsole-(long)W_GMaxLine(handle);
		   RedrawWorkArea(handle);
			SetVSlider(handle);
		}
	}
}

static void WVSlide(int handle,int position)
/*
	Gestion du mouvement du slider vertical
*/
{
	long		NewLigneDepart;
	
	if( G_NbreLigneConsole )
	{
		NewLigneDepart =(long)position * (long)( G_NbreLigneConsole-W_GMaxLine(handle) ) / 1000L;
		if( G_LigneDepart != NewLigneDepart )
		{
			G_LigneDepart	=	NewLigneDepart;
			RedrawWorkArea(handle);
		}
	}
}

/*
	Gestion des organes de la fenetre horizontaux
*/

static void SetHSlider(int handle)
/*
	Procedure calculant la taille et la position du slider horizontal:

	- Appelee par le gestionnaire lors d'un full window.
	- Par le procedures U/Down page/line.
*/
{
	/*	Fixer la position */
	if( W_GMaxColum(handle) >= G_NbreColonneConsole )						/* Petit document */
	{
		wind_set(handle,WF_HSLIDE, 0, 0, 0, 0);
		G_ColonneDepart			=	0;
	}
	else																	/* Doc. sup. fenetre */
		wind_set(handle,WF_HSLIDE, (int)((long)1000 * ((long)G_ColonneDepart) / ((long)G_NbreColonneConsole-(long)W_GMaxColum(handle))), 0, 0, 0);
	
	/*	Fixer la taille */
	if(G_NbreColonneConsole)												/* Si existe au moins une colonne */
		wind_set(handle,WF_HSLSIZE, (int)((long)1000 * (long)W_GMaxColum(handle) / (long)G_NbreColonneConsole), 0, 0, 0);
	else
		wind_set(handle,WF_HSLSIZE, 1000, 0, 0, 0);	
}

static void WLTColum(int handle)
/*
	Gestion de la fleche gauche 
*/
{
	if(G_ColonneDepart > 0)
	{
		G_ColonneDepart--;
		SetHSlider(handle);
		RedrawWorkArea(handle);
	}
}

static void WRTColum(int handle)
/*
	Gestion de la fleche droite
*/
{
	if(G_ColonneDepart+W_GMaxColum(handle)<G_NbreColonneConsole)
	{
		G_ColonneDepart++;
		SetHSlider(handle);
		RedrawWorkArea(handle);
	}
}

static void WLTPage(int handle)
/*
	Gestion clic page gauche
*/
{
	if(G_ColonneDepart-W_GMaxColum(handle) > 0)
	{
		G_ColonneDepart -=W_GMaxColum(handle);
		SetHSlider(handle);
		RedrawWorkArea(handle);
	}
	else if( G_ColonneDepart > 0)
	{
		G_ColonneDepart = 0;
		SetHSlider(handle);
		RedrawWorkArea(handle);
	}
}

static void WRTPage(int handle)
/*
	Gestion clic page droite
*/
{
	if(G_ColonneDepart+2*W_GMaxColum(handle) < G_NbreColonneConsole)
	{
		G_ColonneDepart +=W_GMaxColum(handle);
		SetHSlider(handle);
		RedrawWorkArea(handle);
	}
	else
	{
		G_ColonneDepart = G_NbreColonneConsole-W_GMaxColum(handle);
		SetHSlider(handle);
		RedrawWorkArea(handle);
	}
}

static void WHSlide(int handle,int position)
/*
	Gestion du mouvement du slider horizontal
*/
{
	int	NewColonneDepart;
	
	if( G_NbreColonneConsole )
	{
		NewColonneDepart =(int)(position * (long)(G_NbreColonneConsole-W_GMaxColum(handle))/1000L);
		if( G_ColonneDepart != NewColonneDepart)
		{
			G_ColonneDepart	=	NewColonneDepart;
			RedrawWorkArea(handle);
		}
	}
}

void SetVHSlider(int handle)
/*
	Recalcul les sliders H et V lors d'un Sized ou Full Window
	(taille et position)
*/
{
	SetVSlider(handle);
	SetHSlider(handle);
}

static void WFermer(int handle)
{
	int		L_inX, L_inY, L_inW, L_inH;
	
	ActiveConsole = 0;
	
 	wind_get(handle, WF_CURRXYWH, &L_inX, &L_inY, &L_inW, &L_inH);
	SetLongProfile(G_APP_INF, G_WIND_CONSOLE, "position_x", (long)L_inX);
	SetLongProfile(G_APP_INF, G_WIND_CONSOLE, "position_y", (long)L_inY);
	SetLongProfile(G_APP_INF, G_WIND_CONSOLE, "position_w", (long)L_inW);
	SetLongProfile(G_APP_INF, G_WIND_CONSOLE, "position_h", (long)L_inH);

	M_UnChecked(MAINMENU, O_CONSOL);

	W_Close(handle);
	W_MenuClose(WHandleConsole);
}

/*
 * ----------------------------------------------------------------------------------------------
 *						Gestion du menu de la fenetre console.
 * ----------------------------------------------------------------------------------------------
 */

static void WAffiche(int handle)
/*
	Affiche le nombre exacte de lignes possibles dans la fenetre
	'handle' du document G_TexteConsole courant.

	Entree:	'G_TexteConsole[]' = liste des lignes du G_TexteConsole
		'ligne_max'
		'G_LigneDepart'
		'G_ColonneDepart'
*/
{
	long L_Ligne, L_LigneMax;

	W_HomeCursor(handle);
	if( G_LigneDepart < 0)
		return;

	L_Ligne     =	G_LigneDepart;
	L_LigneMax	=	G_LigneDepart + W_GMaxLine(handle);

	W_SColum(handle, -1 * G_ColonneDepart);
	/*	Afficher toutes les lignes sauf la derniere, puisque elle est en cours */
	while( L_Ligne < G_NbreLigneConsole - 1 && L_Ligne < L_LigneMax)
	{
    	W_Writeln(handle, G_TexteConsole[L_Ligne++]);		
	}

	/*	derniere ligne, affichee sans CR/LF a la fin */
	if( G_NbreLigneConsole >= 1 )
	{
		if(strcmp(G_TexteConsole[L_Ligne], "") == 0)
			W_Write(handle, G_TexteConsole[L_Ligne]);	
		else
			W_Writeln(handle, G_TexteConsole[L_Ligne]);	
	}

	Vsync();
}

/*
 * ----------------------------------------------------------------------------------------------
 *						Gestion du menu de la fenetre console.
 * ----------------------------------------------------------------------------------------------
 */

static void PopAction( int Item )
{
	switch( Item )
	{
		case	ITEM40:	W_MenuItem = M_8;
						MenuWind( WHandleConsole);
						break;

		case	ITEM41:	W_MenuItem = M_9;
						MenuWind( WHandleConsole );
						break;

		case	ITEM42:	W_MenuItem = M_10;
						MenuWind( WHandleConsole );
						break;
	}
}

static void ClicLeft( int Handle )
{
}

static void ClicRight( int Handle )
{
	TPopup		Popup;
	
	Popup.Tree		=	POPUP5;
	Popup.routine	=	PopAction;
	
	F_PopupXY( &Popup, A_MouseX, A_MouseY );
}


void ResetConsole(void)
{
	unsigned long		Ligne;
	
	for(Ligne = 0; Ligne < G_NbreLigneConsole; Ligne++)
		free( G_TexteConsole[Ligne] );

	G_LigneDepart 			=	0L;
	G_ColonneDepart			=	0;
	G_NbreLigneConsole	=	0L;
	G_NbreColonneConsole	=	0;
	G_LigneCouranteConsole	=	-1L;

	SetVHSlider(WHandleConsole);
	RedrawWorkArea(WHandleConsole);
	G_ConsoleModifie = FALSE;
}

void UpdateConsole(void)
{
	SetVHSlider(WHandleConsole);
	RedrawWorkArea(WHandleConsole);
}

void SaveConsole(char *FileName)
/*
	Sauvegarde le tampon de la fenetre dans un fichier.
*/

{
	FILE			*Fichier;
	unsigned long	Ligne;

	graf_mouse(BUSYBEE,NULL);
  
	if((Fichier = fopen(FileName, "w")) != NULL)
	{
		for(Ligne = 0; Ligne < G_NbreLigneConsole; Ligne++)
			fprintf(Fichier, "%s\n", G_TexteConsole[Ligne]);

		fclose(Fichier);
		G_ConsoleModifie = FALSE;
	}
  
	graf_mouse(ARROW,NULL);
}

static void Load(char *FileName)
/*
	Sauvegarde le tampon de la fenetre dans un fichier.
*/

{
	FILE			*Fichier;
	long			NombreLigneAffichable;

	graf_mouse(BUSYBEE,NULL);
  	ResetConsole();

	if((Fichier = fopen(FileName, "r")) != NULL)
	{
		G_ConsoleModifie = FALSE;

		while(!feof(Fichier) && G_NbreLigneConsole < MAX_LINE_CONSOLE - 1)
		{
			G_TexteConsole[++G_LigneCouranteConsole]	=	calloc(SIZE_LINE_CONSOLE, 1);
			fgets(G_TexteConsole[G_LigneCouranteConsole], SIZE_LINE_CONSOLE, Fichier);
			/*	Supprimer le CR/LF en fin, car W_Writexx() ne les utilise pas */
			G_TexteConsole[G_LigneCouranteConsole][strlen(G_TexteConsole[G_LigneCouranteConsole]) - 1] = 0;

			if( G_NbreColonneConsole < strlen(G_TexteConsole[G_LigneCouranteConsole]) )
				G_NbreColonneConsole	=	(int)strlen(G_TexteConsole[G_LigneCouranteConsole]);
		}
		fclose(Fichier);
		G_TexteConsole[++G_LigneCouranteConsole]	=	calloc(SIZE_LINE_CONSOLE, 1);
		strcpy(G_TexteConsole[G_LigneCouranteConsole], "");
	}

  	if(G_LigneCouranteConsole)
		G_NbreLigneConsole	=	G_LigneCouranteConsole + 1;

	NombreLigneAffichable	=	W_GMaxLine(WHandleConsole);
	if( NombreLigneAffichable < G_NbreLigneConsole )
	{
		G_LigneDepart	=	G_NbreLigneConsole - 	NombreLigneAffichable;
	}		

	UpdateConsole();
	graf_mouse(ARROW,NULL);
}

void AddString(char *NewString)
/*
	Ajoute une nouvelle ligne de texte dans le tampon de la fenetre.
*/

{
	long			NombreLigneAffichable;
	
	G_ConsoleModifie = TRUE;

	if( G_NbreLigneConsole < MAX_LINE_CONSOLE - 1 )
	{
		G_TexteConsole[++G_LigneCouranteConsole]	=	calloc(SIZE_LINE_CONSOLE, 1);
		strcpy(G_TexteConsole[G_LigneCouranteConsole], NewString);
			                                    
		G_NbreLigneConsole++;
					                                    
		if( G_NbreColonneConsole < strlen(G_TexteConsole[G_LigneCouranteConsole]) )
			G_NbreColonneConsole	=	(int)strlen(G_TexteConsole[G_LigneCouranteConsole]);
	}
                                                                             
	NombreLigneAffichable	=	W_GMaxLine(WHandleConsole);
	if( NombreLigneAffichable < G_NbreLigneConsole )
	{
		G_LigneDepart	=	G_NbreLigneConsole - 	NombreLigneAffichable;
	}		

	UpdateConsole();		
}

void MenuWind(int Handle)
{
	unsigned long		NombreLigneAffichable;
	char				L_Path[2048]		= "\0"; 						/* Chemin seulement */
	char				L_PathName[2048]	= "\0";			/* Chemin + Nom de fichier */
	char				L_FileName[256]		= "\0"; 						/* Nom de fichier seulement */
	char				L_Extension[6]		= "*.txt\0";					/* Extension de fichiers */

	if(Handle == WHandleConsole)
 	{
    	switch(W_MenuItem)
    	{
			/*	Item ABOUT ME */
      		case M_1:	AboutMe();
      					break;

			/*	Item SAVE historique */
      		case M_2:	if(A_FileSelector(L_PathName, L_FileName, L_Extension, L_Path, G_SAVE_HIST)) 
						{									
							SaveConsole( L_PathName );
						}
      					break;

			/* Item CLEAR window */
      		case M_3:	ResetConsole();
      					break;

			/*	Item TOP  */
      		case M_4:	G_LigneDepart 		=	0L;
						G_ColonneDepart		=	0;
	
						RedrawWorkArea(WHandleConsole);																		/* s'envoyer un Redraw */
						SetVHSlider(WHandleConsole);
						break;
      		
			/*	Item BOTTOM */			
      		case M_5:	NombreLigneAffichable	=	W_GMaxLine(WHandleConsole);
						if( NombreLigneAffichable < G_NbreLigneConsole )
						{
							G_LigneDepart	=	G_NbreLigneConsole - 	NombreLigneAffichable;
						}

						RedrawWorkArea(WHandleConsole);																	/* s'envoyer un Redraw */
						SetVHSlider(WHandleConsole);
      					break;

			/*	Item LOAD historique */
			case M_6:	if(A_FileSelector(L_PathName, L_FileName, L_Extension, L_Path, G_OPEN_HIST)) 
						{									
							Load( L_PathName );
						}
						break;

			/*	Aide ST-Guide console window Help */
	    	case	M_7:STG_Open(G_HelpFile, SECTION_HELP_TERMINAL);							
    					break;

			/*	Cut	*/
			case	M_8:SaveConsole( CLIPBOARD_WRITE );
						ResetConsole();
						break;
						
			/*	Copy */
			case	M_9:SaveConsole( CLIPBOARD_WRITE );
						break;

			/*	Paste */
			case	M_10:Load( CLIPBOARD_READ );
						break;

      		default:	break;
    }
  }
}


/*
 * ----------------------------------------------------------------------------------------------
 *								Ouverture de la fenetre JagConsole
 */
 
void OpenConsole(void)
/*
	Ouvre la fenetre JagConsole, terminal des messages.
*/
{
	Type_Parameter_Window		*parameter;
	int							vingtieme;

	if( ActiveConsole )
	{
		WFermer(WHandleConsole);
		ActiveConsole	=	FALSE;
	}
	else
	{
		ActiveConsole	=	TRUE;
		M_Checked(MAINMENU, O_CONSOL);
				
	    vingtieme = (VdiInfo.HauteurStation	- DesktopMenu - 5) / 20;
    	parameter = W_GetParameter();
		strcpy(parameter->titre, G_TITLE_TERM);
		parameter->treename			=	IAPPLI;

	 	parameter->x				=	(int)GetLongProfile(G_APP_INF, G_WIND_CONSOLE, "position_x", 0L);
		parameter->y				=	(int)GetLongProfile(G_APP_INF, G_WIND_CONSOLE, "position_y", 0L);
	    parameter->w				=	(int)GetLongProfile(G_APP_INF, G_WIND_CONSOLE, "position_w", 0L);
	    parameter->h				=	(int)GetLongProfile(G_APP_INF, G_WIND_CONSOLE, "position_h", 0L);
		if( (parameter->x == 0 && parameter->y == 0 && parameter->w == 0 && parameter->h == 0 ) || G_ResolutionChanged)
		{
		    parameter->x			=	VdiInfo.LargeurStation * 2 / 10;
	    	parameter->y			=	(DesktopMenu)+(vingtieme);
		    parameter->w			=	VdiInfo.LargeurStation * 6 / 10;
	    	parameter->h			=	VdiInfo.HauteurStation - DesktopMenu - (2*vingtieme);
		}
	
	    parameter->attributs		=	NAME|CLOSER|FULLER|MOVER|SIZER|UPARROW|DNARROW|VSLIDE|LFARROW|RTARROW|HSLIDE|SMALLER;
    	parameter->op_close			=	WFermer;
    	parameter->op_redraw		=	WAffiche;
	    parameter->op_full			=	SetVHSlider;
	    parameter->op_sized			=	SetVHSlider;
	    parameter->op_uppage		=	WUppage;
	    parameter->op_downpage		=	WDnpage;
	    parameter->op_upline		=	WUpline;
	    parameter->op_downline		=	WDnline;
	    parameter->op_rightpage		=	WRTPage;
	    parameter->op_leftpage		=	WLTPage;
	    parameter->op_rightcolum	=	WRTColum;
	    parameter->op_leftcolum 	=	WLTColum;
	    parameter->op_vslider		=	WVSlide;
	    parameter->op_hslider		=	WHSlide;
	    
	    parameter->ClicLeft		=	ClicLeft;
	    parameter->ClicRight	=	ClicRight;
	    
		parameter->Mouse 			=	ARROW;

		WHandleConsole	=	W_Open(parameter);
		ResetConsole();
		SetVHSlider(WHandleConsole);
	
		W_MenuOpen(WHandleConsole, WINDMENU, MenuList);

		Mfree(parameter);
	}
}
