/*
 *				Miscallenous Procedures Modules
 *
 *
 *	Author  : FunShip
 *	File	: PCTools.c
 *	Date    : 18 December 1995
 *	Release : 26 January  1996
 *	Version : 1.20
 *	Language: Pure C, Version du 3 Fevrier 1992 
 *	Country : FRANCE
 *	
 *
 *			  - Copyright Atari FunShip (c) 1991-1995 -
 *			      - ATARI ST/STE/Mega/TT & Falcon -
 * -----------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vdi.h>
#include <aes.h>

#include "Tools.h"
#include "Keyboard.h"

#define		ERROR_1		"[0][ AADS: Tools Error #1 | Not Enough Memory ][Ok]"
#define		ERROR_2		"[0][ AADS: Tools Error #2 | Unknow object into menu ][Ok]"

/*
 *	Movment menu button
 */
 
#define		BOXCHAR_STYLE	0x00FE1401L
#define		ARROW_LEFT	0x04000000L
#define		ARROW_RIGHT	0x03000000L

/*
 *	knowing classical menu's objects
 */
 
#define		MN_ROOT		0
#define		MN_BAR		1
#define		MN_ACTIVE	2
#define		MN_FIRST_GTITLE	3

#define		NB_ACCESSORY	6							/* GEM allows 6 standard accessory */

/*
 * ---------------------------------------------------------------------------------------
 *			Used by PcXWind
 */

int CalculIntersect(int x1,int y1,int w1,int h1,
		    int x2,int y2,int w2,int h2,int tablexywh[])
/*
	Compute intersection area between both rectangles and
	place its position and size in tablexywh[].
	return 1 if two ractangles have intersection section.
*/
{
  int x,y,w,h;

  x = (x2 > x1) ? x2 : x1;
  y = (y2 > y1) ? y2 : y1;
  w = (x2+w2 < x1+w1) ? x2+w2 : x1+w1;
  h = (y2+h2 < y1+h1) ? y2+h2 : y1+h1;
  tablexywh[0] = x;
  tablexywh[1] = y;
  tablexywh[2] = w-1;
  tablexywh[3] = h-1;

  return(w>x && h>y);
}

/*
 * ---------------------------------------------------------------------------------------
 *			Used by PcXPopup
 */

int GetPere(OBJECT *Arbre,int fils)
/*
	Compute what is the father object of 'fils' object.
*/
{
  int pere;
    
  pere = Arbre[fils].ob_next;
  while(Arbre[pere].ob_tail != fils)
  {
    fils = pere;
    pere = Arbre[fils].ob_next;
  }
  return(pere);
}

void SumXY(Type_CoordXY *CoordXY,OBJECT *Arbre,int Id_Object)
/*
	Calcul les coordonn‚es ABSOLUE ‚cran d'un objet identifi‚ Id_Object
*/
{
  Type_CoordXY	CoordXYprime;
  
  if(Arbre[Id_Object].ob_next == -1)				/* Is Root Object */
  {
    CoordXY->X = Arbre[Id_Object].ob_x;
    CoordXY->Y = Arbre[Id_Object].ob_y;
  }
  else
  {
    SumXY(&CoordXYprime,Arbre,GetPere(Arbre,Id_Object));
    CoordXY->X = CoordXYprime.X + Arbre[Id_Object].ob_x;
    CoordXY->Y = CoordXYprime.Y + Arbre[Id_Object].ob_y;
  }
}

/*
 * ---------------------------------------------------------------------------------------
 *			Used by PcXWMenu
 *			Copy and Restore Screen Area Procedures
 */

void *ScreenSave(int VdiHandle,int x,int y,int w,int h)
/*
	Save into a buffer the graphic area showed by (x,y,w,h).
	If success, return a pointer to a struture that contains the picture.
*/
{
  int			workout[57];
  Type_Buffer_Screen	*buffer;
  long			size;
  int			planes,wdwidth;

  vq_extnd(VdiHandle,1,workout);
  planes = workout[4];
  wdwidth = (int)((w + 15) / 16);
  size = sizeof(Type_Buffer_Screen) + (long)planes*(long)h*2L*(long)wdwidth;
  if ((buffer = malloc(size)) == 0)
    return(NULL);
  else 
  {
    buffer->VdiHandle		= VdiHandle;
    buffer->mfdb.fd_addr 	= ((void *) &buffer->data);
    buffer->mfdb.fd_w		= w;
    buffer->mfdb.fd_h		= h;
    buffer->mfdb.fd_wdwidth	= wdwidth;
    buffer->mfdb.fd_stand	= 0;
    buffer->mfdb.fd_nplanes	= planes;
    buffer->scr.fd_addr		= (long)(0L);		/* Data source in video memory */
    buffer->scr.fd_w		= 0;
    buffer->scr.fd_h		= 0;
    buffer->scr.fd_wdwidth	= 0;
    buffer->scr.fd_stand	= 0;
    buffer->scr.fd_nplanes	= 0;
    buffer->pxy[0]		= x;			/* (x1,y1) source area to copy */
    buffer->pxy[1]		= y;
    buffer->pxy[2]		= x + w - 1;		/* (x2,y2) source area to copy */
    buffer->pxy[3]		= y + h - 1;
    buffer->pxy[4] 		= 0;			/* (x1,y1) destination area */
    buffer->pxy[5] 		= 0;
    buffer->pxy[6] 		= w-1;			/* (x2,y2) destination area */
    buffer->pxy[7] 		= h-1;
    graf_mouse(M_OFF,0);
    vro_cpyfm(VdiHandle,S_ONLY,buffer->pxy,&buffer->scr,&buffer->mfdb);
    graf_mouse(M_ON,0);
    return((void *)buffer); 
  }
}

void ScreenRestore(void *buffer)
/*
	Replace at the old position the picture saved into the structure pointed by buffer.
*/
{
  int		pxy[8];
  int		*s,*d;

  if (((Type_Buffer_Screen *)buffer) != NULL) 
  {
    d = pxy; 
    s = ((Type_Buffer_Screen *)buffer)->pxy+4;
    *d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
    d = pxy+4; 
    s = ((Type_Buffer_Screen *)buffer)->pxy;
    *d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
    graf_mouse(M_OFF,0);
    vro_cpyfm(((Type_Buffer_Screen *)buffer)->VdiHandle,S_ONLY,pxy,
		&((Type_Buffer_Screen *)buffer)->mfdb,&((Type_Buffer_Screen *)buffer)->scr
	     );
    graf_mouse(M_ON,0);
    free(((Type_Buffer_Screen *)buffer)); 
  }
  return;
}

/*
 * ---------------------------------------------------------------------------------------
 *			Used by PcXMenu and PcXWMenu
 */

int CalculKey(char *EntryText)
/*
	Recherche la raccourcie clavier d‚finie dans le texte d'un entr‚e de Menu.
	
	Entr‚es:	Texte de l'entr‚e de Menu
	Sortie:		
*/
{
  char	*string;
  char	Ctrl,
  	Alt;
  int	Indice;

  /*
   *	D‚finition des codes CTRL + xx et ALT + xx
   */
  Ctrl	= '^';
  Alt	= 0x07;
  
  /*
   *	Cas du CTRL + xx ?
   */
  string = strrchr(EntryText,Ctrl);		/* Rechercher le CTRL */
  if(string != NULL)				/* Si trouv‚ */
  {
    ++string;					/* Pointer le caractŠre xx */
    Indice = 0;
    						/* Rechercher le code correspondant */
    while(!(CTRL_[Indice].Lettre == *string || CTRL_[Indice].Lettre == '\0'))
      Indice++;
    if(CTRL_[Indice].Lettre == *string)		/* Si trouv‚ */
      return(CTRL_[Indice].Touche);		/* Le retourner */
    else
      return(NO_KEY);				/* Sinon pas de raccourcie */
  }
  /*
   *	Cas du ALT + xx ?
   */
  string = strrchr(EntryText,Alt);		/* Rechercher ALT */
  if( string != NULL )				/* Si trouv‚ */
  {
    ++string;					/* Pointer le caractŠre */
    Indice = 0;
    						/* Rechercher le code */
    while(!(ALT_[Indice].Lettre == *string || ALT_[Indice].Lettre == '\0'))
      Indice++;
    if(ALT_[Indice].Lettre == *string)		/* Si trouv‚ */
      return(ALT_[Indice].Touche);		/* Le retourner */
    else
      return(NO_KEY);				/* Sinon pas de raccourcie */
  }
  /*
   *	Pas de CTRL ou ALT: donc on ignore, pas de raccourcie clavier
   */
  return(NO_KEY);
}

/*
 * ---------------------------------------------------------------------------------------
 *			Used by PcXWMenu
 *			This code perform a copy from a menu tree source
 */
 
int CalculBoite (OBJECT *MenuAddress,int object)
/*
	Compute what is the menu box's order number for the order number 
	of the G_TITLE selected.
*/
{
  int		boxMenu, Indice;
  
  boxMenu= MenuAddress->ob_tail;						/* Box's backgroung */
  boxMenu= MenuAddress[boxMenu].ob_head;					/* First box */
  for(Indice=3;Indice<object;Indice++)
    boxMenu= MenuAddress[boxMenu].ob_next;					/* Next box menu */
  return(boxMenu);								/* Return ordre number */
}

OBJECT *CpMenu(OBJECT *ArbreMenu)
/*
	This code perform a tree objects copy from a source menu to a new menu.
	In addition, two new G_Title (In fact two BoxChar objects) to allow the 
	scrolling are created into the new menu. This feature will be handle by 
	the Menu Window management.
	
	Return the array's address of the new menu dupplicated.
*/
{
  unsigned long		Size;
  unsigned int		NbObject,Indice;
  OBJECT		*Destination;
  int			BoxMenu,Item;						/* used for hide accessory item */
    
  /*
   *	Count how many objects there are into the tree object array source
   */
  NbObject	= 0;
  while(!(ArbreMenu[NbObject].ob_flags & LASTOB))
    NbObject++;
  NbObject++;
  
  /*
   *	Get a new type object array 
   */
  Size = sizeof(OBJECT);
  Destination = (OBJECT *)malloc(Size * (NbObject+2));
  if(Destination == NULL)
  {
    form_alert(1,ERROR_1);
    return(NULL);
  }
 
  /*
   *	Making now the real copy to the destination 
   */
  for(Indice = 0; Indice < NbObject; Indice++)
  {
    switch(ArbreMenu[Indice].ob_type)
    {
    	case G_IBOX:
    	case G_BOX:	Destination[Indice] = ArbreMenu[Indice];
    			break;
    	case G_TITLE:
    	case G_STRING:	
    			Destination[Indice] = ArbreMenu[Indice];
    			Destination[Indice].ob_spec.free_string = malloc(strlen(ArbreMenu[Indice].ob_spec.free_string)+1);
    			if(Destination[Indice].ob_spec.free_string == NULL)
    			{
			  form_alert(1,ERROR_1);
			  return(NULL);
    			}
    			strcpy(Destination[Indice].ob_spec.free_string,ArbreMenu[Indice].ob_spec.free_string);
    			break;
    	default:	form_alert(1,ERROR_2);
    			break;
    }
  }
  /*
   *	Create two new Objects: G_BoxChar will be our Scroller "button"
   */
  Destination[NbObject].ob_type  = G_BOXCHAR;						/* Object's type */
  Destination[NbObject].ob_flags = 0;
  Destination[NbObject].ob_state = DISABLED;						/* At the begining the menu is no scrolled */
  Destination[NbObject].ob_spec.index = ARROW_LEFT | BOXCHAR_STYLE;			/* Left arrow */
  Destination[NbObject].ob_x	 = 0;
  Destination[NbObject].ob_y	 = 0;
  Destination[NbObject].ob_width = 2;
  Destination[NbObject].ob_height= 1;
  Destination[NbObject].ob_head	 = -1;							/* No children */
  Destination[NbObject].ob_tail	 = -1;
  Destination[NbObject].ob_next  = NbObject+1;						/* To other arrow G_TITLE */
  
  Destination[NbObject+1].ob_type  = Destination[NbObject].ob_type;
  Destination[NbObject+1].ob_flags = Destination[NbObject].ob_flags;
  Destination[NbObject+1].ob_state = 0;
  Destination[NbObject+1].ob_spec.index = ARROW_RIGHT | BOXCHAR_STYLE;			/* Right arrow */
  Destination[NbObject+1].ob_x	   = Destination[NbObject].ob_width;
  Destination[NbObject+1].ob_y	   = Destination[NbObject].ob_y;
  Destination[NbObject+1].ob_width = Destination[NbObject].ob_width;
  Destination[NbObject+1].ob_height= Destination[NbObject].ob_height;
  Destination[NbObject+1].ob_head  = -1;
  Destination[NbObject+1].ob_tail  = -1;
  Destination[NbObject+1].ob_next  = MN_FIRST_GTITLE;					/* First real G_TITLE */

  Destination[MN_ACTIVE].ob_head   = NbObject;						/* Insert into the list the new both BoxChar */

  rsrc_obfix(Destination,NbObject);							/* Caracters to pixels */
  rsrc_obfix(Destination,NbObject+1);
  
  /*
   *	Compute the new menu position with the new both G_TITLE
   */
   
  Destination[MN_BAR].ob_x = Destination[MN_ROOT].ob_x;					/* Caler le menu tout … gauche */
  Destination[MN_ACTIVE].ob_x = Destination[MN_ROOT].ob_x;
  											/* Shift the G_TITLE to right */
  for(Indice=3;Indice<NbObject;Indice++)						/* And the BoxMenu */
  {
    if(Destination[Indice].ob_type == G_TITLE)
      Destination[Indice].ob_x += Destination[NbObject].ob_width + Destination[NbObject+1].ob_width;
    if(Destination[Indice].ob_type == G_BOX)
      Destination[Indice].ob_x += Destination[NbObject].ob_width;
  }
  
  /*
   *	Now we can hide all accessory item, because it won't be used with the built-menu.
   */
  BoxMenu = CalculBoite(Destination,MN_FIRST_GTITLE);					/* Box order number of first G_TITLE */
  Item = Destination[BoxMenu].ob_head;							/* First child G_STRING ("About me...")*/
  Item = Destination[Item].ob_next;							/* Second child G_STRING: We hide from it */
  for(Indice = 0; Indice <= NB_ACCESSORY; Indice++)						
  {
    Destination[Item].ob_flags |= HIDETREE;						/* Each accessory item is hiden */
    Destination[BoxMenu].ob_height -= Destination[Item].ob_height;			/* Substract the G_STRING's height */ 
    Item++;										/* Next G_STRING object */
  } 
  
  return(Destination);
}
