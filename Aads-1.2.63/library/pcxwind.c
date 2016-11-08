/*
 *			GEM Window Manager Sub-Library:
 *			Now include a fully local menu processing
 *
 *
 *	Author  : FunShip
 *	File	: PCXwind.c
 *	Date    : 28 November 1993
 *	Release : 08 December 1998
 *	Version : 1.20
 *	Language: Pure C, Version du 3 Fevrier 1992 
 *	Country : FRANCE
 *	
 *
 *						- Copyright Atari FunShip (c) 1991, 1998 -
 *							  - ATARI ST/STE/TT & Falcon -
 * -----------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * -----------------------------------------------------------------------------------------
 *			Original include from Aads library
 */

#include "..\Include\PCXApp.h"
#include "..\Include\PCXwind.h"
#include "..\Include\PCXMouse.h"
#include "..\Include\PCPile.h"

#include "Tools.h"
#include "Window.h"

/*
 * -----------------------------------------------------------------------------------------
 *				Publics Datas
 */

int		tabulation	=	8;																/* Tabulate length in text char */

int		DesktopMenu,																	/* Lines height reserved for menu */
		DesktopW,																		/* Width of window desktop */
		DesktopH;																		/* Heigh of window desktop */

int		Red_x, Red_y, Red_w, Red_h;														/* Current are redrawing with rectangles list */

/*
 * -----------------------------------------------------------------------------------------
 *				Privates Defines
 */

#define CLIPON		1				/* Clipping VDI On */
#define CLIPOFF		0				/* Clipping VDI Off */

#define ESPACE		2				/* pixels between two windows */
#define ICONWINDH	80				/* size of window icon */
#define ICONWINDW	50


#define	DFLTATTRIBUTE	NAME|CLOSER|FULLER|MOVER|INFO|SIZER|UPARROW|DNARROW|VSLIDE|LFARROW|RTARROW|HSLIDE|SMALLER

#define	MOTHER		2				/* Is global Icon Application */
#define	CHILD		3				/* Is hidden cause global iconify */

#define	X_START		0					/* Origine of growing box */
#define	Y_START		0
#define	W_START		50
#define	H_START		50

#define	X_END		0					/* Destination of shrinking box */
#define	Y_END		0
#define	W_END		50
#define	H_END		50

#define	LEFT		1					/* mouse left button */
#define	RIGHT		2

/*
 * -----------------------------------------------------------------------------------------
 *				Privates types definitions
 */

/*typedef struct		
				{
					int	x;
					int	y;
				} Type_Coord;	*/
		
typedef	struct	{
					int	AutoEnable;					/* {True,False} = Mode On/Off */
					int	OnlyWorkArea;				/* If mouse into work area */
					int	NoMouseMovement;			/* Don't top if mouse-movement */
		  
					int	PreviousMouseX;				/* Previous mouse position for */
					int	PreviousMouseY;				/* mouve-movement analysis */
				} Type_AutoWind;

/*
 * -----------------------------------------------------------------------------------------
 *				Privates Datas structures
 */
 			
static	Type_Pile		*Pile_Window;				/* windows's list */
static	int				Flag_Redraw;				/* 1: Under Redraw control */
													/* 0: Without clipping control */

static	Type_AutoWind	AutoTopWind;				/* X-Window like: At mouse pos. TOP */
													/* the window or not */
static	int				TreeNameIconApp;			/* TreeName application icon */
static	int				GlobalIcon;					/* Show if global icon enable */
static	int				GrowShrinkBox;				/* Visual effect allowed */

/*
 * -----------------------------------------------------------------------------------------
 *				Low level Primitives VDI
 */

void W_OnClip(int pxyarray[])
/*
	Set On the VDI clipping for a graphic area of x1,y1,x2,y2 position placed 
	into a table
*/
{
	vs_clip(VdiHandle,CLIPON,pxyarray);
}

void W_OffClip(int pxyarray[])
/*
	Set off clipping for a graphic area of x1,y1,x2,y2 position 
*/
{
	vs_clip(VdiHandle,CLIPOFF,pxyarray);
}

/*
 * -----------------------------------------------------------------------------------------
 *				Datas Structures Procedures
 */

#define	WindInsert(window)		Empiler(Pile_Window, window)

static int WindRemove(int handle)
/*
	Delete a window data structure from the stack.
	
	Entree: handle
	Sortie: retourne 1 si ok
*/
{
	Type_Window		*window;
	Type_PilePoste 	*Ptr_poste;


	window = PileSommet(Pile_Window);

	if( window == NULL )
		return	FALSE;
	else if( window->handle == handle )
		return( Depiler(Pile_Window) );
	else
	{
		Ptr_poste	=	PileFirst(Pile_Window);
		window		=	Ptr_poste->PtrDonnee;
		while( Ptr_poste != NULL && window->handle != handle )
		{
			Ptr_poste	=	PileNext(Ptr_poste);
			window		=	( Ptr_poste != NULL) ? Ptr_poste->PtrDonnee : NULL;
		}
		/*
		 * Sortie:	- On retombe sur le premier poste vu, on n'a donc pas trouv‚
		 *		le bon poste de la fenetre 'handle'
		 *		- On a trouv‚ la fenetre		
		 */
		if(Ptr_poste != NULL && window->handle == handle)
			return(PileDelete(Pile_Window, Ptr_poste));
		else
			return	FALSE;
	}
}

Type_Window *WindSearch(int handle)
/*
	searching a window with its handle identification into the list.
	return its data structure.
*/
{
	Type_Window		*window;
	Type_PilePoste 	*Ptr_poste;


	window = PileSommet(Pile_Window);

	if(window == NULL)
		return(NULL);
	else if(window->handle == handle)					/* It's not this wind */
		return( window );								/* return wind address */
	else
	{
		Ptr_poste	=	PileFirst(Pile_Window);
		window		=	Ptr_poste->PtrDonnee;
		while( Ptr_poste != NULL && window->handle != handle )
		{
			Ptr_poste	=	PileNext(Ptr_poste);		
			window		=	( Ptr_poste != NULL) ? Ptr_poste->PtrDonnee : NULL;
		}

		if(Ptr_poste != NULL)
		{
			if(window->handle == handle)				/* good handle found */
				return(Ptr_poste->PtrDonnee);			/* return wind address */
			else
				return NULL;
		}
		else
			return(NULL);
	}
}

/*
 * -----------------------------------------------------------------------------------------
 *			Privates Icons Management Procedures
 */

static void WIcn_Moved(int handle)
/*
	Proc‚dure appel‚e losrque la fenetre iconifi‚e est d‚pla‡‚e. Dans ce cas
	il faut ajuster la position du formulaire au niveau de sa structure pour qu'il
	adopte ces changements.
	
	Entr‚es:	Handle = handle de la fenetre porteuse
	Sorties:	rien
*/
{
	int		Dummy;
	Type_Window	*Window;
	OBJECT	*Arbre;
	
	Window = WindSearch(handle);				/* Searching the window descriptor */
	if(Window != NULL)					/* if does exist */
	{	
		rsrc_gaddr(R_TREE,Window->treename,&Arbre);		/* Tree objects address */
		wind_get(handle,WF_WORKXYWH,&Arbre->ob_x,&Arbre->ob_y,&Dummy,&Dummy);	/* draw */
	}    			
}

static void WIcn_Redraw(int handle)
/*
	Called when a redraw event occurs. We must redraw Icon formular.
	Formular will be redrawing with Red_xx variables applied by
	PCXWind.
*/
{
	Type_Window	*Window;
	OBJECT	*Arbre;
	  
	Window = WindSearch(handle);				/* Searching the window descriptor */
	if(Window != NULL)					/* If does exist */
	{							/* Tout redessiner dans le clipping */
		rsrc_gaddr(R_TREE,Window->treename,&Arbre);		/* Tree object address */
		objc_draw(Arbre,ROOT,DEPTH,Red_x,Red_y,Red_w,Red_h);/* Draw tree objects */
	}
}

static void WDrawIcon(Type_Window *Window)
/*
	Dessine le formuaire contenant l'icone pour la fenetre iconifi‚e.
	En r‚allit‚, le formulaire est adapt‚ aux dimensions de la fenetre icone, et 
	sera dessin‚ sur ‚v‚nement message REDRAW que l'on envoit.
*/
{
	OBJECT	*Arbre;
	int		X,Y,W,H;
	
	rsrc_gaddr(R_TREE, Window->treename, &Arbre);			/* Tree Object address */	
	wind_get(Window->handle, WF_WORKXYWH, &X, &Y, &W, &H);	/* Get windows position and size */
	
	Arbre->ob_x     = X;									/* Set formular position and size */
	Arbre->ob_y     = Y;
	Arbre->ob_width = W;
	Arbre->ob_height= H; 
	
	W_Redraw( Window->handle );								/* Send a Redraw message */
}

/*
 * -----------------------------------------------------------------------------------------
 *			Privates Miscallenous Procedures
 */

static int WindIsFull(Type_Window *window)
/*
	Return TRUE if the window is already fulled.
*/
{
	int dummy,curw,curh;
	
	wind_get(window->handle,WF_CURRXYWH,&dummy,&dummy,&curw,&curh);
	return(curw==DesktopW && curh==DesktopH);
}

static void WindClear(int pxyarray[])
/*
	Filled with background color the pxyarray[] defined region.
	
	Entree: (x1,y1,x2,y2) area
	Sortie: background filled
*/
{
	vsf_interior(VdiHandle,FIS_HOLLOW);		/* Filled with background color */
	vsf_perimeter(VdiHandle,FALSE);		/* No perimeter drawing */
	v_bar(VdiHandle,pxyarray);			/* Draw a pxyarray defined rectangle */
	vsf_perimeter(VdiHandle,TRUE);		/* Perimeter drawing */
}

int W_ClrScr(int handle)
/*
	Clear the window work area.
	
	Input:	Handle of window
	Output:	TRUE if clear done else FALSE.
*/
{
	int		pxyarray[4];			/* (x1,y1,x2,y2) area */
	Type_Window	*window;
	
	window = WindSearch(handle);
	if(window == NULL)
		return(FALSE); 
	if(Flag_Redraw)				/* ClrScr under Redraw control */
	{						/* Not need of rectangles list */
		pxyarray[0] = Red_x;
		pxyarray[1] = Red_y;
		pxyarray[2] = Red_w + Red_x -1;
		pxyarray[3] = Red_h + Red_y -1;
		WindClear(pxyarray);
		return(TRUE);
	}
	else
	{
		wind_update(BEG_UPDATE); 			/* prevenir l'AES ! */
		v_hide_c(VdiHandle); 			/* Mouse cursor disable */
		
		wind_get(handle,WF_FIRSTXYWH,&pxyarray[0],&pxyarray[1],
							 &pxyarray[2],&pxyarray[3]);
		while(pxyarray[3] != 0)			/* Tant qu'il y a rectangles */
		{
			pxyarray[2] += pxyarray[0] - 1;
			pxyarray[3] += pxyarray[1] - 1;
			W_OnClip(pxyarray);			/* Clip On Vdi */
			WindClear(pxyarray);
			W_OffClip(pxyarray);			/* Clip Off Vdi */
			wind_get(handle,WF_NEXTXYWH,&pxyarray[0],&pxyarray[1], &pxyarray[2],&pxyarray[3]);
		}
		wind_update(END_UPDATE); 			/* process complete */
		v_show_c(VdiHandle,1);			/* Mouse cursor enable */
		return(TRUE);
	}
}

static void WindRedraw(int handle,int x,int y,int w,int h)
/*
	Redraw a partial window section
	
	Entree:handle =	fenetre
			x,y,w,h = position (x,y) et taille de la zone a redessiner
			global Flag_Redraw used
	Sortie: rien
*/	
{
	int table[4];					/* Intersection section */
	int x_rec,y_rec,w_rec,h_rec;			/* One rectangle from list */
	Type_Window *window;

	/*
	 *	First part: Normal redraw onto the window work area
	 */
	window = WindSearch(handle);			/* search its structure */
	wind_update(BEG_UPDATE); 			/* prevenir l'AES ! */
	v_hide_c(VdiHandle); 				/* Curseur Off */
	wind_get(handle,WF_FIRSTXYWH,&x_rec,&y_rec,&w_rec,&h_rec);
	Flag_Redraw = TRUE;
	while(h_rec != 0)				/* While exist rectangles */
	{
		if( CalculIntersect(x,y,w,h,x_rec,y_rec,w_rec,h_rec,table) )
		{
			W_OnClip(table);				/* Clip On for VDI */
			Red_x = table[0];				/* for redraw without */
			Red_y = table[1];							/* VDI functions, example */
			Red_w = table[2] - table[0] + 1;					/* AES obj_draw */
			Red_h = table[3] - table[1] + 1;
			W_ClrScr(handle);							/* before, clear window */
			
			if(window->isiconify && window->treename != NOICON)		/* If is iconify */
				WIcn_Redraw(handle);						/* Redraw formular icon */
			else
			{
				if(*window->op_redraw != NULL && !(window->isiconify))
					(*window->op_redraw)(handle);					/* Call S/P User */
				if(window->WithMenu)
					RefreshMenu(window);						/* Menu redraw */
			}  
		  	W_OffClip(table);							/* Clip Off for VDI */
		}  
		wind_get(handle,WF_NEXTXYWH,&x_rec,&y_rec,&w_rec,&h_rec);
	}
	Flag_Redraw = FALSE;
	wind_update(END_UPDATE); 						/* all done! */
	v_show_c(VdiHandle,1);						/* Cursor enable */
}

static void WindKill(int handle)
/*
	Kill a window OR call an another procedure defined by the user.
*/
{
	Type_Window *window;
	
	window = WindSearch(handle);
	if(window->op_close!=NULL)						/* User procedure defined */
		(*window->op_close)(handle);					/* call it */
	else									/* else, kill window */
	{
		wind_close(handle);							/* graphical close */
		wind_delete(handle);						/* delete data structure */
		W_MenuClose(handle);						/* remove menu and its data structure */
		WindRemove(handle);							/* delete my own structure */
	}
}

static void WindFull(int handle)
/* 
	Change the size window into maximal position and size OR
	into previous position and size if it was already fulled.
*/
{
	Type_Window *window;
	int	  dummy;				/* unused parameters */
	int 	  hslide,vslide;			/* position of sliders */
	
	window = WindSearch(handle);			/* search window structure */
	wind_get(handle,WF_HSLIDE,&hslide,&dummy,&dummy,&dummy);
	wind_get(handle,WF_VSLIDE,&vslide,&dummy,&dummy,&dummy);  
	if(WindIsFull(window))			/* if window fulled */
	{						/* set old position and size */
		wind_set(handle,WF_CURRXYWH,window->prev_x,window->prev_y,
				 window->prev_w,window->prev_h);
	}
	else						/* else set full size */
	{
		wind_get(handle,WF_CURRXYWH,&(window->prev_x),&(window->prev_y),
				 &(window->prev_w),&(window->prev_h) );
		wind_set(handle,WF_CURRXYWH,0,DesktopMenu,DesktopW,DesktopH);
	}
	wind_set(handle,WF_HSLIDE,hslide,0,0,0);	/* set positions */
	wind_set(handle,WF_VSLIDE,vslide,0,0,0);
	
	if(window->op_full != NULL) 			/* Si existe procedure User  */
		(*window->op_full)(handle);			/* Appeler la procedure User */
}

static void WindSize(int handle)
/* 
	Called when a window is resized
*/
{
	Type_Window *window;
	
	window = WindSearch(handle);	
	if(window->op_sized != NULL)			/* Si existe ! */
		(*window->op_sized)(handle);		/* Appeler la procedure User */
}

/*
 * ---------------------------------------------------------------------------------------
 *			Primitives de gestion d'iconification
 *			Appel‚es par le Event Manager
 */

static void WindIconify(int handle, int X, int Y, int Width, int Height)
/*
	This procedure is called when a window will be iconify. Well, we try to 
	draw an iconify icon and call an user procedure.
*/
{
	Type_Window		*window;
	int				x,y,w,h;
  
	window = WindSearch(handle);							/* Searching window */
	wind_get(handle, WF_CURRXYWH, &x, &y, &w, &h);			/* For copy its previous */ 
	window->prev_x = x;										/* position and size */
	window->prev_y = y;
	window->prev_w = w;
	window->prev_h = h;
	window->isiconify = TRUE;

	wind_set(handle, WF_ICONIFY, X, Y, Width, Height);		/* Set iconify window */

	/*	Second: Draw icon and call user procedure	*/
	if(window->op_iconify != NULL)							/* If exist User procedure */
		(*window->op_iconify)(handle);						/* Call it */
	
	if(window->treename != NOICON)							/* If Icon formular defined */
		WDrawIcon(window);									/* Go to draw it */
}

static void WAllIconify(int Handle, int X, int Y, int Width, int Height)
/*
	Inconify ALL windows from the windows list. The window responsable of this
	message will be the Global Icon Application and all other window will be hidden.
	If one or more was already iconify, they will be uniconify before to be hidden.
	
	Input:	Handle of window responsable of this message WM_ALLICONIFY

	Output:	Window of handle is iconify
		All other window are hidden
*/
{
  Type_Window		*window;
  int				Auxiliaire;
  Type_PilePoste	*Poste;

  /*
   *	First: Iconify really the window that had received the message Alliconify
   */
  if(!GlobalIcon)					/* If not previously done */
  {
    GlobalIcon = TRUE;					/* Global Iconfy done */
    window = WindSearch(Handle);			/* Searching this window */
    if(TreeNameIconApp != NOICON)			/* Exchange with global icon */
    {
      Auxiliaire = window->treename;			/* Save its treename icon */
      window->treename = TreeNameIconApp;		/* Set global treename icon */
      TreeNameIconApp = Auxiliaire;
    }
    window->globalicon = MOTHER;			/* Become the global icon application */
    WindIconify(window->handle, X, Y, Width, Height);			/* Iconify only the first one */
  }
  else
    window = PileSommet(Pile_Window);			/* take the first one */
  /*
   *	Second: Iconify all other window
   */    

  Poste	=	PileFirst(Pile_Window);
  do
  {
    if(window->isiconify && window->globalicon != MOTHER)/* If already iconify => UNICONIFY */
    {
      wind_set(window->handle, WF_UNICONIFY, window->prev_x, window->prev_y, window->prev_w, window->prev_h);
      window->isiconify = FALSE;
      /* !! */
      if(window->op_uniconify != NULL)			/* If user procedure exist */
        (*window->op_uniconify)(window->handle);	/* call it */

	}

	if(window->globalicon != CHILD && window->globalicon != MOTHER)/* If not previously done */
	{
		wind_get(window->handle, WF_CURRXYWH, &window->prev_x, &window->prev_y, &window->prev_w, &window->prev_h);
		window->globalicon = CHILD;  			/* Child window hidden */		       		 
		wind_close(window->handle);			/* Hidden all other windows */
	}

    Poste		=	PileNext(Poste);				/* Next window */
	if ( Poste != NULL )
	    window = Poste->PtrDonnee;			/* Read its descriptor */
  } 
  while(Poste != NULL);		/* for all windows */
}

static void WAllUnIconify(void)
/*
	Uninconify all windows from the windows list that are marked CHILD.
	In fact, we make a system wind_open() call to display them.
*/
{
	Type_Window		*window;
	Type_PilePoste	*Poste;
	
	window	=	PileSommet(Pile_Window);				/* take first window */
	Poste	=	PileFirst(Pile_Window);
	do
	{
		if(window->globalicon == CHILD)					/* If is a child window */
		{
			wind_open(window->handle,window->prev_x,window->prev_y,window->prev_w,window->prev_h);
			window->globalicon = FALSE;					/* clear flag */
		}
		
		if( ( Poste = PileNext(Poste) ) != NULL )
			window 		=	Poste->PtrDonnee;			/* Read its descriptor */
	}
	while(Poste != NULL);								/* for all windows */

	GlobalIcon = FALSE;									/* Global iconify cancelled */
}

static void WindUnIconify( int handle )
/* 
	Window UnIconify with previous position and size.
*/
{
	Type_Window		*window;
	int				Auxiliaire;

  /*
   *	Normal uniconify processing
   */  
	window = WindSearch(handle);							/* searching its structure */
	wind_set(handle, WF_UNICONIFY, window->prev_x, window->prev_y, window->prev_w, window->prev_h);
	window->isiconify = FALSE;

	/*
	 *	If it was the Global Icon Application 
	 */
	if(window->globalicon == MOTHER)						/* It was the App. icon */
	{
		window->globalicon = FALSE;							/* Delete this flag */
		if(TreeNameIconApp != NOICON)						/* Restore its real icon */
		{
			Auxiliaire = window->treename;
			window->treename = TreeNameIconApp;				/* Restore its icon */
			TreeNameIconApp = Auxiliaire;					/* Restore global icon */
		}
		WAllUnIconify();									/* Uniconify all other windows */
	}
	/*	
	 *	Call user procedure for this window
	 */
	if(window->op_uniconify != NULL)						/* If user procedure exist */
		(*window->op_uniconify)(handle);					/* call it */
}

/*
 * ---------------------------------------------------------------------------------------
 * 				Text Window Output Procedures
 */

int W_GMaxLine(int handle)
/*
  retourne le nombre maximum de ligne texte sur la fenetre 'handle'.
*/
{
  int		dummy,ligne;
  Type_Window	*window;
   
  window = WindSearch(handle);
  if(window != NULL)
  {
    W_WorkXYWH(handle,&dummy,&dummy,&dummy,&ligne);
    return( (ligne-1) / A_GCellHeigh()); 
  }
  else
    return(FALSE);
}

int W_GMaxColum(int handle)
/*
  retourne le nombre maximum de colonne sur la fenetre 'handle'.
*/
{
  int dummy,colonne;
  Type_Window	*window;
   
  window = WindSearch(handle);
  if(window != NULL)
  {
    wind_get(handle,WF_WORKXYWH,&dummy,&dummy,&colonne,&dummy);
    return(colonne / A_GCellWidth() );
  }
  else
    return(FALSE);
}

void W_HomeCursor(int handle)
/*
	Ramene les coordonnees des sorties textes sur la premiere ligne
	de la fenetre, mais laisse la colonne graphique courante 
	tel quelle.
*/
{
  Type_Window	*window;
  
  window=WindSearch(handle);
  if(window!=NULL)
  {
    window->ligne=A_GFontHeigh();				/* Premiere ligne texte */
  }
}

int W_GLine(int handle)
/*
  Retourne la ligne courante graphique de la prochainne sortie texte
  sur la fenetre 'handle';
*/
{
  Type_Window *window;
  int         dummy,ligne_origine;
  
  W_WorkXYWH(handle,&dummy,&ligne_origine,&dummy,&dummy);
  window=WindSearch(handle);
  if(window!=NULL)
    return(window->ligne+ligne_origine);
  else
    return(ligne_origine);
}

int W_GColum(int handle)
/*
  Retourne la colonne courante graphique de la prochainne sortie texte
  sur la fenetre 'handle';
*/
{
  Type_Window *window;
  int         dummy,col_origine;
  
  wind_get(handle,WF_WORKXYWH,&col_origine,&dummy,&dummy,&dummy);
  window=WindSearch(handle);
  if(window!=NULL)
    return(window->colonne+col_origine);
  else
    return(col_origine);
}

void W_SLine(int handle,int ligne)
/*
  Positionne le curseur … la ligne texte 'ligne' de la fenetre 'handle'.
*/
{
  Type_Window *window;
  
  window=WindSearch(handle);
  if(window!=NULL)
    window->ligne=(ligne-1)*A_GCellHeigh()+A_GFontHeigh();
}

void W_SColum(int handle,int colum)
{
  Type_Window *window;
  
  window=WindSearch(handle);
  if(window!=NULL)
    window->colonne = A_GCellWidth()*colum;
}

static int NextTab(int col_debut,size_t longueur)
/*
	Calcul la prochainne colonne graphique tabul‚e de sortie texte en fonction
	de:
	'col_debut' = colonne d'affichage de la chaine pr‚c‚dente
	'longueur'  = longueur de la chaine affich‚e
	Retourne la prochainne colonne graphique tabul‚e.
*/
{
	int 	col_graph, col_tab, taille_cell, lg_tab;
	
	taille_cell	= A_GCellWidth();
	col_graph	= col_debut+(int)longueur*taille_cell;
	col_tab		= col_debut;
	lg_tab		= tabulation*taille_cell;
	while(col_tab <= col_graph)
		col_tab+=lg_tab;
	
	return(col_tab);
}

/*
 * ---------------------------------------------------------------------------------------
 * 				W_Printf() Procedure Implementation
 */

static void WindEcrire(int handle,char *string)
/*
  Fonction equivalente printf pour les fenetres en incluant un CR/NL
  et gerant les tabulations '\t' dans le texte.
*/
{
	Type_Window		*window;
	char			*adr, *adr_fin;	
	
	window		=	WindSearch(handle);
	adr_fin		=	string+strlen(string);
	adr			=	string;
	while(adr != adr_fin)
	{
		while(*adr != '\t' && *adr != '\0')					/* Recherche les \t \n \r */
			adr++;
		/* 
			Sortie: adr pointe sur une 'tab' ou '\0' 
		*/
		if(*adr == '\t')
		{
			*adr = '\0';
			v_gtext(VdiHandle, W_GColum(handle), W_GLine(handle), string);
			window->colonne = NextTab(window->colonne, strlen(string));
			string = adr+1;
			*adr = '\t';
			adr  = string;
		}
		else												/* Pas de tabulation */
		{
			v_gtext(VdiHandle,W_GColum(handle),W_GLine(handle),string);
			adr = adr_fin;	  
		}
	}
}

int W_Writeln(int handle,char *string)
/*
  Fonction equivalente printf pour les fenetres en INCLUANT un CR/NL
  et gerant les tabulations '\t' dans le texte.
*/
{
	int				pxyarray[4], ColonneDepart;		
	Type_Window		*window;
	
	window  = WindSearch(handle);
	if(window == NULL)														/* wind doesn't exit */
		return(FALSE);
	else if(Flag_Redraw)													/* W_Printf called by */
	{
		ColonneDepart	=	window->colonne;								
		WindEcrire(handle,string);											/* REDRAW message */
		window->colonne = ColonneDepart;
	}
	else																	/* W_Printf called by User */
	{																		/* Must be under rectangle list control */
		wind_update(BEG_UPDATE);											/* prevenir l'AES ! */
		v_hide_c(VdiHandle);												/* Curseur Off */
		
		ColonneDepart	=	window->colonne;								/* sera modifiee ! */
		wind_get(handle, WF_FIRSTXYWH, &pxyarray[0], &pxyarray[1], &pxyarray[2], &pxyarray[3]);
		while(pxyarray[3] != 0)												/* Tant qu'il y a rectangles */
		{
			pxyarray[2] += pxyarray[0] - 1;
			pxyarray[3] += pxyarray[1] - 1;
			W_OnClip(pxyarray);												/* Clip On */
			
			WindEcrire(handle, string);
			window->colonne = ColonneDepart;									/* restitution */

			W_OffClip(pxyarray);											/* Clip Off */
			wind_get(handle, WF_NEXTXYWH, &pxyarray[0], &pxyarray[1], &pxyarray[2], &pxyarray[3]);
		}
		wind_update(END_UPDATE);											/* C'est fini */
		v_show_c(VdiHandle,1);												/* Curseur actif */
	}

	/*	Retour Chariot et Nouvelle ligne automatique */
	window->ligne  += (A_GCellHeigh() - A_GFontHeigh() ) + A_GFontHeigh();
	return(TRUE);
}

int W_Write(int handle, char *string)
/*
	Fonction affichant une chaine de carateres dans une fenetre SANS CR/Lf a la fin
	et gerant les tabulations '\t' dans le texte.
*/
{
	int				pxyarray[4], ColonneDepart;		
	Type_Window		*window;
	
	window  = WindSearch(handle);
	if(window == NULL)														/* wind doesn't exit */
		return(FALSE);
	else if(Flag_Redraw)													/* W_Printf called by */
	{
		ColonneDepart	=	window->colonne;								/* sera modifiee ! */
		WindEcrire(handle,string);											/* REDRAW message */
		window->colonne = ColonneDepart;
	}
	else																	/* W_Printf called by User */
	{																		/* Must be under rectangle list control */
		wind_update(BEG_UPDATE);											/* prevenir l'AES ! */
		v_hide_c(VdiHandle);												/* Curseur Off */
		
		ColonneDepart	=	window->colonne;							/* Sauvegarde colonne de depart */
		wind_get(handle, WF_FIRSTXYWH, &pxyarray[0], &pxyarray[1], &pxyarray[2], &pxyarray[3]);
		while(pxyarray[3] != 0)												/* Tant qu'il y a rectangles */
		{
			pxyarray[2] += pxyarray[0] - 1;
			pxyarray[3] += pxyarray[1] - 1;
			W_OnClip(pxyarray);												/* Clip On */

			WindEcrire(handle, string);
			window->colonne =	ColonneDepart;								/* Reprendre au debut */

			W_OffClip(pxyarray);											/* Clip Off */
			wind_get(handle, WF_NEXTXYWH, &pxyarray[0], &pxyarray[1], &pxyarray[2], &pxyarray[3]);
		}
		wind_update(END_UPDATE);											/* C'est fini */
		v_show_c(VdiHandle,1);												/* Curseur actif */
	}

	/*	La colonne finale est a la fin de la chaine ecrite */
	window->colonne +=	A_GCellWidth() * (int)strlen(string);	
	return(TRUE);
}

/*
 * ---------------------------------------------------------------------------------------
 * 				Main Publics Procedures to use windows
 */
  
int W_Init(void)
/*
  	Initialise le gestionnaire de fenetres
*/
{
/*  Type_Coord	CoupleXY;*/
	int		dummy, MouseX, MouseY;
    
   /*	Initialise la liste des fenetres … vide	*/

	Pile_Window = CreerPile(sizeof(Type_Window));
	if(Pile_Window == NULL)
		return(FALSE);

	/*	Compute width,heigh of desktop.	*/
	wind_get(DESKTOP,WF_WORKXYWH,&dummy,&DesktopMenu,&DesktopW,&DesktopH);

	/*	Special variables.	*/

	Flag_Redraw		= FALSE;
	TreeNameIconApp = NOICON;
	GlobalIcon		= FALSE;						/* Global iconify cancelled */
  
	/*	X_Window like: at the mouse's position TOP the window or not	*/

	AutoTopWind.AutoEnable = FALSE;					/* Not topped the window */
	AutoTopWind.OnlyWorkArea = FALSE;
	AutoTopWind.NoMouseMovement = FALSE;
	graf_mkstate(&MouseX,&MouseY,&dummy,&dummy);
	AutoTopWind.PreviousMouseX = MouseX;			/* For next execution */
	AutoTopWind.PreviousMouseY = MouseY;
  
	/*	Visual "clude".	*/
	GrowShrinkBox = TRUE;							/* If visual "clue" are allowed by the main program */
  
	return(TRUE);
}

int W_Exit(void)
/*
	Termine une session avec la sous-librairie
	
	Entr‚es:	rien
	Sorties:	True si bien ex‚cut‚ sinon False
*/
{
	KillPile(Pile_Window);
	return	TRUE;
}

Type_Parameter_Window *W_GetParameter(void)
/*
	Allocate a block parameter to create a window after, and
	initialize it.
*/
{
  Type_Parameter_Window *parameter;

  parameter = (Type_Parameter_Window *)malloc((long)sizeof(Type_Parameter_Window));
  if(parameter != NULL)
  {
    parameter->attributs = DFLTATTRIBUTE;
    
    strcpy(parameter->titre,"Without Title");
    strcpy(parameter->info,"Without Information"); 
    
	parameter->type			=	MODELESS;										/*	Par defaut non modal */
	
    parameter->treename		= NOICON;
    parameter->isiconify	= FALSE;

    /* Window's mouse definition */
    parameter->Mouse		= ARROW;
    parameter->MouseForm	= NULL;
    
    parameter->op_close		= 0L;
    parameter->op_redraw	= 0L;
    parameter->op_full		= 0L;
    parameter->op_sized		= 0L;
    parameter->op_uppage	= 0L;
    parameter->op_downpage	= 0L;
    parameter->op_upline	= 0L;
    parameter->op_downline	= 0L;
    parameter->op_rightpage	= 0L;
    parameter->op_leftpage	= 0L;
    parameter->op_rightcolum	= 0L;
    parameter->op_leftcolum	= 0L;
    parameter->op_vslider	= 0L;
    parameter->op_hslider	= 0L;
    parameter->op_iconify	= 0L;
    parameter->op_uniconify	= 0L;
    parameter->op_topped	= 0L;
    parameter->op_moved		= 0L;

		/*	My callbacks */
		parameter->ClicLeft		= NULL;
		parameter->ClicRight	= NULL;
	}
	return(parameter);
}

int W_Open(Type_Parameter_Window *parameter)
/*
	Creer et Ouvre un nouvelle fenetre a l'ecran.
	
	Entree: position,taille,attributs,Ptr sur un titre et une ligne info
        	(comme pour les fcts GEM)
	        Liste de fonctions pour les traitements propre aux fenetres
	Sortie: Retourne le handle (GEM) de la fenetre
*/
{
	Type_Window *window;
	int         handle;

	window = (Type_Window *)malloc((long)sizeof(*window));

	window->attributs	=	parameter->attributs;											/* Copy the bits field attribute */
	window->type		=	parameter->type;
  
	strcpy(window->titre,parameter->titre);													/* Window's title */
	strcpy(window->info,parameter->info); 													/* Window's information ligne */

	/*	Window's icon */
	window->treename		=	parameter->treename;										/* Copy menu tree name */
	window->isiconify		=	parameter->isiconify;										/* Show if window is already iconified as the open */
	window->globalicon		=	FALSE;														/* Show if the window take the global icon when iconfied */
 
	/*	Window's menu */
	window->WithMenu		=	FALSE;														/* No menu at begining */
	window->MenuModelName	=	FALSE;														/* Name of menu */
	window->MenuAddress		=	NULL;														/* Objects tree's address */
	window->MenuListEvent	=	NULL;														/* Event's list for each menu's item */
	window->NextGTitle		=	FALSE;														/* Next G_Title to hide, the last before to display all others G_Title */
  
	/* 	Window's mouse definition */
	window->Mouse		= parameter->Mouse;													/* Icon's number or USER_DEF (255)  */ 
	window->MouseForm	= parameter->MouseForm;												/* Contains an user mouse cursor shape if Mouse == USER_DEF */

	/*	Callback definition	*/  
	window->op_close	= parameter->op_close;
	window->op_redraw	= parameter->op_redraw;
	window->op_full	= parameter->op_full;
	window->op_sized	= parameter->op_sized;
	window->op_uppage	= parameter->op_uppage;
	window->op_downpage	= parameter->op_downpage;
	window->op_upline	= parameter->op_upline;
	window->op_downline	= parameter->op_downline;
	window->op_rightpage	= parameter->op_rightpage;
	window->op_leftpage	= parameter->op_leftpage;
	window->op_rightcolum	= parameter->op_rightcolum;
	window->op_leftcolum	= parameter->op_leftcolum;
	window->op_vslider	= parameter->op_vslider;
	window->op_hslider	= parameter->op_hslider;
	window->op_iconify	= parameter->op_iconify;
	window->op_uniconify	= parameter->op_uniconify;
	window->op_topped	= parameter->op_topped;
	window->op_moved	= parameter->op_moved;

	/*	My callbacks */
	window->ClicLeft	= parameter->ClicLeft;
	window->ClicRight	= parameter->ClicRight;
  
	handle = wind_create(window->attributs,parameter->x,parameter->y,parameter->w,parameter->h);
	if(handle > 0)																			/* No errors, window is now created for GEM but not visible */
	{
		wind_set(handle, 2, window->titre, 0, 0);												/* Set the window's title */
		wind_set(handle, 3, window->info, 0, 0);  												/* Set the window's information */

		if(GrowShrinkBox)																	/* If visual "clue" are allowed by the main program */
      		form_dial(FMD_GROW , X_START, Y_START, W_START, H_START, parameter->x, parameter->y, parameter->w, parameter->h);
     
    	if( wind_open(handle,parameter->x,parameter->y,parameter->w,parameter->h) == 0 )	/* Now we're attempting to display REALLY the window */
    	{
      		free(window);																	/* failled open */
      		wind_delete(handle);															/* Remove it from GEM */
      		handle = -1;
    	}
    	else																				/* Open successfull */
    	{
      		window->ligne = A_GFontHeigh();													/* First graphics line position to the next VDI text output */
      		window->colonne = 0;															/* At the left window's border */
      		window->handle = handle;														/* Copy the GEM Aes window's handle */
      		if(!WindInsert(window))															/* Not enough memory into AADS's data structure */
      		{
        		free(window);																/* Free the window AADS's structure */
        		wind_delete(handle);														/* Remove it from GEM */
        		handle = -1;
      		}
    	}
  	}
  	return(handle);    
}

int W_Close(int Handle)
/*
	Close and kill the window showed by the parameter handle.
*/
{
	Type_Window			*Window;
	int					WindX, WindY, WindW, WindH;
  
	Window = WindSearch(Handle);
	if(Window != NULL)																		/* if window found */
	{
		if(GrowShrinkBox)																	/* If visual "clue" are allowed by the main program */
    	{
      		wind_get( Window->handle, WF_CURRXYWH, &WindX, &WindY, &WindW, &WindH );				/* Get window's size and position */
      		form_dial(FMD_SHRINK, X_END, Y_END, W_END, H_END, WindX, WindY, WindW, WindH);
    	}
    	wind_close(Handle);																	/* Close... */
    	wind_delete(Handle);																/* GEM kill */
    	return(WindRemove(Handle));															/* AADS kill */
  	}
  	else																					/* Window not found...*/
    	return(FALSE);
}

void W_Redraw(int handle)
/*
	Envoyer une Message REDRAW a notre propre fenetre 'handle'.
*/
{
	int				message[8], x, y, w, h;
  	Type_Window		*window;

	window = WindSearch(handle);
	if(window != NULL)
	{
    	wind_get(handle, WF_WORKXYWH, &x, &y, &w, &h);

    	message[0]	=	WM_REDRAW;										/* Send a REDRAW message to himself */
    	message[1]	=	AesHandle;										/* Application source's handle is the target application */
		message[2]	=	0;
    	message[3]	=	handle;											/* target window's handle */
    	message[4]	=	x;												/* Window's Work area to readraw */
    	message[5]	=	y;
    	message[6]	=	w;
    	message[7]	=	h;
    	appl_write(AesHandle, 16, message);								/* Send message... */
  	}
}

void W_SetIconApp(int TreeName)
/*
	Indique un nom de formulaire icone qui sera utilis‚ lors d'une iconification
	g‚n‚rale des fenetres de l'application.
	
	Entr‚e:	TreeName = Nom du formulaire
	Sortie:	rien
*/
{
	TreeNameIconApp = TreeName;															/* Assign the application global icon tree name */
}

int W_NewMouse(int WindowHandle,int Mouse, MFORM *MouseForm)
/*
	Change the current mouse cursor shape for a target window.
*/
{
	Type_Window	*Window;
	
	Window = WindSearch(WindowHandle);
	if(Window != NULL)
	{
		Window->Mouse	= Mouse;					/* New icon number or USER_DEF */
		Window->MouseForm	= MouseForm;					/* Users mouse cursor shape */
		return(MS_Mouse(Mouse,MouseForm));					/* Call our mouse AADS's function */
	}
	return(FALSE);
}


int W_AutoTop(Type_AutoTop *AutoTop)
/*
	Switch On or Off the Auto Window TOP mode. When the mouse is on a window we top it
	automaticaly if this mode is allows.
*/
{
	AutoTopWind.AutoEnable 		= AutoTop->AutoEnable;			/* On/Off Auto window TOP mode */
	AutoTopWind.OnlyWorkArea  	= AutoTop->OnlyWorkArea;		/* If mouse into work area */
	AutoTopWind.NoMouseMovement = AutoTop->NoMouseMovement;		/* Top if mouse-mouvement */
	return(TRUE);
}

void W_VisualClue(int OnOff)
/*
	Turn On or Off the visual clue. They are the standard GEM Growbox and shrinkbox at the open or close window. 
*/
{
	GrowShrinkBox = OnOff;						/* Assign our global flag */
}

/*
 * ---------------------------------------------------------------------------------------
 *				Gestionnaire d'‚v‚nement Souris
 */

static void MSEvent(void)
/*
	Check where the mouse is and change the shape for each window under the (x,y)
	location or if the mouse is on the desktop's window.
*/
{
	int			MouseX,MouseY,Dummy,WorkX,WorkY,WorkW,WorkH;
	int			WindowHandle;
	Type_Window	*Window;
	
	graf_mkstate(&MouseX, &MouseY, &Dummy, &Dummy);
	WindowHandle = wind_find(MouseX, MouseY);
	
	if(WindowHandle > DESKTOP)
	{
		if( (Window = WindSearch(WindowHandle)) != NULL )			/* Structure fenetre trouv‚e */
		{
			wind_get( WindowHandle, WF_WORKXYWH, &WorkX, &WorkY, &WorkW, &WorkH);
			if(MouseX > WorkX && MouseX < WorkX + WorkW - 1 && MouseY > WorkY && MouseY < WorkY + WorkH - 1)
				MS_Mouse(Window->Mouse,Window->MouseForm);
			else													/* Application's mouse */
				MS_Mouse(A_Mouse,A_MouseForm);
		}
	}
	else															/* Application's mouse */
		MS_Mouse(A_Mouse,A_MouseForm);
}

/*
 * ---------------------------------------------------------------------------------------
 *			Gestion de la mise en TOP automatique des fenetres
 */

static void AutoWindTop(void)
/*
	At the mouse's position if a window exist, we send a top message if it allows.
	and if the current top window isn't modal.
*/
{
	int				Handle, HandleTop, MouseX, MouseY, Dummy;
	int				WorkX, WorkY, WorkW, WorkH;
	Type_Window		*WindowTop;

  
	graf_mkstate(&MouseX,&MouseY,&Dummy,&Dummy);					/*	Mouse's position */
	Handle = wind_find(MouseX,MouseY);								/*	window at this position */
	wind_get( DESKTOP, WF_TOP, &HandleTop, &Dummy, &Dummy, &Dummy);	/*	what's current TOP window */

	if( WindSearch( Handle ) == NULL )								/*	Si fenetre a TOper pas … nous */
		return;
		
	if(Handle != HandleTop && Handle > DESKTOP && AutoTopWind.AutoEnable)
	{
		WindowTop	=	WindSearch(HandleTop);							/*	Fenetre TOP actuelle */
		if( WindowTop != NULL)											/*	De notre application */
		{
			if( !(WindowTop->type & MODAL) )							/*	Si l'actuelle n'est pas modale */
			{
				if(AutoTopWind.OnlyWorkArea)							/* Only work area */
				{
					wind_get( Handle, WF_WORKXYWH, &WorkX, &WorkY, &WorkW, &WorkH);
					if(	MouseX >= WorkX && MouseX <= WorkX + WorkW && 
								MouseY >= WorkY && MouseY <= WorkY + WorkH )
						wind_set(Handle, WF_TOP, 0, 0, 0, 0);			
					return;
				}
			
				if(AutoTopWind.NoMouseMovement)							/* don't top during mose-movement */
				{
					AutoTopWind.PreviousMouseX = MouseX;				/* For next execution */
					AutoTopWind.PreviousMouseY = MouseY;
					if( MouseX == AutoTopWind.PreviousMouseX && MouseY == AutoTopWind.PreviousMouseY )
						wind_set(Handle, WF_TOP, 0, 0, 0, 0);
					return;
				}
				wind_set(Handle, WF_TOP, 0, 0, 0, 0);
			}
		}
	}
} 

/* --------------------------------------------------------------------------------------- */
void WClic( void)
/*	Appel‚ quand un clic sur la fenetre est apparu. */
{
	int			WindowHandle, MouseButton, Dummy;
	int			MouseX, MouseY;
	int			Area_X, Area_Y, Area_W, Area_H;
	Type_Window	*Window;
	
	if( A_Event & MU_BUTTON )
	{
		graf_mkstate( &MouseX, &MouseY, &MouseButton, &Dummy );			/* Position (x,y) and button */

		WindowHandle = wind_find( MouseX, MouseY );						/* Handle de la fenetre */
		Window = WindSearch( WindowHandle );    						/* Structure fenetre */
		
		if( Window != NULL && !Window->isiconify )						/*	Si existe et non iconifi‚e */
		{
			W_WorkXYWH( WindowHandle, &Area_X, &Area_Y, &Area_W, &Area_H);
			/*	Si souris dans zone cliente */
			if( MouseX >= Area_X && MouseX <= Area_X + Area_W &&	
				MouseY >= Area_Y && MouseY <= Area_Y + Area_H )
			{
				if( MouseButton & LEFT )
				{
					if( Window->ClicLeft != NULL )
						(*Window->ClicLeft)(Window->handle);
				} 
				else if( MouseButton & RIGHT )
				{
					if( Window->ClicRight != NULL )
						(*Window->ClicRight)(Window->handle);
				}
			}
		}
	}
}

/*
 * ---------------------------------------------------------------------------------------
 *				Gestionnaire d'‚v‚nements
 */

void W_Event(void)
/*
	  Procedure de traitement des messages fenetres uniquement.
	  Cette proc‚dure est non bloquante, on en sort ! 

	  Entree: event = EvŠnement … traiter, ici devra etre un message
  		  message = message relatif … l'‚vŠnement si c'est un message GEM fenetre
	  Sortie: Retourne le message GEM trait‚ pour pr‚venir l'application de ce que c'‚tait.
*/
  
{
	char			texte[80];
	Type_Window		*window;
	int				Handle, Dummy;
  
    /* 
     *	First part: If it's an event message and not destinate to Menu
     */
    if( (A_Event & MU_MESAG) && (A_Message[0] != MN_SELECTED) )
    {
      switch(A_Message[0])			/* case this message */
      {                         
        /* ----------- Traitements automatiques, sans proc‚dures user */
        
        case WM_REDRAW:	WindRedraw(A_Message[3], A_Message[4], A_Message[5], A_Message[6], A_Message[7]);
                       	break;    
        case WM_SIZED: 	wind_set(A_Message[3],WF_CURRXYWH, A_Message[4], A_Message[5], A_Message[6], A_Message[7]);
                       	WindSize(A_Message[3]);
                       	CheckSize(A_Message[3]);
                       	break;    
		case WM_FULLED:	WindFull(A_Message[3]);
						CheckSize(A_Message[3]);
                       	break;    
		case WM_NEWTOP:					/* AES 3.3 but never used ! */
						wind_set(A_Message[3],WF_TOP,0,0,0,0);
                       	break;    

        case WM_UNTOPPED:				/* AES 4.0 Information purpose only */
       					break;

        case WM_ONTOP:					/* AES 4.0 Information purpose only */
        	     		break;

	/* ------------ Traitements automatiques avec ‚ventuellement proc‚dures user */
		
		case WM_TOPPED:	wind_get(0,WF_TOP, &Handle, &Dummy, &Dummy, &Dummy);						/* Fenetre actuellement TOP */
						window = WindSearch(Handle);	
						if(window != NULL)											/*	Fenetre de notre application */
						{
							if(!(window->type & MODAL))								/* Si la fenetre courante n'est pas MODALE */
							{	
								window = WindSearch(A_Message[3]);
								wind_set(A_Message[3],WF_TOP,0,0,0,0);
	                			if(window->op_topped != NULL)	
  			  						(*window->op_topped)(A_Message[3]);		
  			  				}
  			  				else													/* Refuser le TOP et d‚truire l'event */
  			  				{
  			 					A_Event 		&=	(~MU_MESAG | ~MU_BUTTON | ~MU_KEYBD);
  			  				}
  			  			}
						else														/*	Pas de notre application */
						{
							window = PileSommet(Pile_Window);						/*	Chercher la derniere ouverte */
							if( window->type & MODAL )								/*	de type modal, reactiver celle-ci */
							{
								wind_set(window->handle,WF_TOP,0,0,0,0);
	                			if(window->op_topped != NULL)	
  			  						(*window->op_topped)(window->handle);
							}
							else													/*	Sinon reactiver celle demandee */
							{
								wind_set(A_Message[3],WF_TOP,0,0,0,0);
	                			if(window->op_topped != NULL)	
  			  						(*window->op_topped)(A_Message[3]);
							}
						}

                       	break;    

			case WM_MOVED:		wind_set(	A_Message[3],WF_CURRXYWH, 
											A_Message[4],A_Message[5], A_Message[6],A_Message[7] );
		                		window = WindSearch(A_Message[3]);
        	     				if(window->op_moved != NULL)	
		  					  		(*window->op_moved)(A_Message[3]);		
								if(window->isiconify && window->treename != NOICON)
  			   						WIcn_Moved(A_Message[3]);
  								if(window->WithMenu)				/* If have got a menu */
			  	  					AdjustPosition(window);
								break;    

			case WM_CLOSED:		WindKill(A_Message[3]);
    		                   	break;    
        
			case WM_ARROWED:	window = WindSearch(A_Message[3]);
								switch(A_Message[4])		/* Case sub-A_Message */
								{
									case WA_UPPAGE:	if(window->op_uppage != NULL)	
  					  									(*window->op_uppage)(A_Message[3]);		
	  				 								break;
	  				 				case WA_DNPAGE:	if(window->op_downpage != NULL)
								  					   (*window->op_downpage)(A_Message[3]);
													break;
	  				 				case WA_UPLINE:	if(window->op_upline	!= NULL)
								  						(*window->op_upline)(A_Message[3]);		
									  				 break;
									case WA_DNLINE:	if(window->op_downline != NULL)	
								  					   (*window->op_downline)(A_Message[3]);		
									  				 break;
									case WA_LFPAGE:	if(window->op_leftpage != NULL)	
								  					   (*window->op_leftpage)(A_Message[3]);		
									  				 break;
									case WA_RTPAGE:	if(window->op_rightpage != NULL)
								  					   (*window->op_rightpage)(A_Message[3]);		
									  				 break;
									case WA_LFLINE:	if(window->op_leftcolum != NULL)
								  					   (*window->op_leftcolum)(A_Message[3]);		
									  				break;
									case WA_RTLINE:	if(window->op_rightcolum != NULL)	
								  						(*window->op_rightcolum)(A_Message[3]);		
													break;
									default:	sprintf(texte,"[0][| WM_ARROWED error: | Unknow action needed n'%d ][ ok ]",A_Message[0]);
												form_alert(1,texte);
					        	}
								break;    
        
			case WM_HSLID: 
		       					wind_set(A_Message[3],WF_HSLIDE,A_Message[4],0,0,0);
						       	window = WindSearch(A_Message[3]);
		       					if(window->op_hslider != NULL)
						        	(*window->op_hslider)(A_Message[3],A_Message[4]);
                		       	break;    
        
			case WM_VSLID: 
    			   		       	wind_set(A_Message[3],WF_VSLIDE,A_Message[4],0,0,0);
       		    			   	window = WindSearch(A_Message[3]);
			       		       	if(window->op_vslider != NULL)
									(*window->op_vslider)(A_Message[3],A_Message[4]);
				       			break;    
        
			case WM_ICONIFY:				/* AES 4.1 */		     	
		    				 	WindIconify( A_Message[3],	A_Message[4], A_Message[5], 
		     												A_Message[6], A_Message[7]);
								break;
			
			case WM_ALLICONIFY:				/* AES 4.1 */
								WAllIconify( A_Message[3],	A_Message[4], A_Message[5], 
					     									A_Message[6], A_Message[7]);
								break;
			
			case WM_UNICONIFY:				/* AES 4.1 */
	    			            WindUnIconify( A_Message[3] );
						        break;
		        
			case WM_BOTTOMED:				/* AES 4.1 */
								wind_set(A_Message[3],WF_BOTTOM,0,0,0,0);
								break;
	
			case WM_TOOLBAR:	sprintf(texte,"[0][| WM_TOOLBAR received ][ ok ]");
								form_alert(1,texte);
								
			default:/* it's an unknow A_Message window or no used simply ! */
					break;
     		
		}
	}/*EndIf pour Event Fenetre */

    /*
     *	Second part: We are testing if it's an event mouse button was occured
     *	for a window-menu...
     */
    WMnEvent();

	/*
	 *	Check for any clic onto the work area.
	 */    
	 
	WClic();
	
    /*
     *	Third part: We are testing the mouse if it's the good mouse shape according to the window...
     */
    MSEvent();
    /*
     *	Auto Window Topping
     */
    AutoWindTop();
    
}
