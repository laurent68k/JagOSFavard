/*
 *		Module de gestion du bureau (DESKTOP) propre de l'application.
 *		Ce module est un sous-ensemble de PCXFORM.C
 *
 *
 *	Author  : FunShip
 *	File	: PCXDesk.c
 *	Date    : 06 October 1995
 *	Revision: 21 Juillet 1997
 *	Version : 1.20
 *	Release : 2
 *	Language: Pure C, Version du 3 Fevrier 1992 
 *	Origine : FRANCE
 *	
 *		Copyright (c) FunShip, ATARI 1991-1996
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "..\Include\PCXApp.h"
#include "..\Include\PCFile.h"
#include "..\Include\PCXForm.h"
#include "..\Include\PCXWind.h"

/*
 * -----------------------------------------------------------------------------------------
 *				Privates global defines
 */

#define	TRUE		1
#define	FALSE		0

#define	DESKTOP		0

#define	GAUCHE		1					/* mouse left button */
#define	DROIT		2

#define	DEPTH		999					/* Obj_draw Depth level drawing */
#define	ONELEVEL	0					/* Obj_draw One level drawing   */
#define	FIRSTCHILD	1					/* 1st children of Object tree  */

#define	ERROR_ICON		"[0][ PCXForm warning: | No Icon found ][ok]"
#define	ERROR_ICONMOVE	"[0][ PCXForm warning: | Unknow Move Icon ][ok]"

#define	ICONMOVED	0					/* Icon moved only, on desktop */

/*
 * -----------------------------------------------------------------------------------------
 *				Privates Datas Types
 */


typedef	struct	{
					int		IconName;							/* Order number name object */
					void (*ProcClic)(void);						/* When icon clic */
					void (*ProcDbClic)(void);					/* When icon double clic */
					void (*ProcClicDroit)(void);				/* When icon double clic */
					void (*ProcDbClicDroit)(void);				/* When icon double clic */
					void (*Move2Icon)(int movment,int Object);	/* When Icon moved to Icon */
					void (*Move2Wind)(int movment,int Handle);	/* When Icon moved to Wind */
				} Type_Icon;

/*
 * -----------------------------------------------------------------------------------------
 *				Privates Datas Structures
 */

static	Type_File	*File_Icon;		/* Icon list */

/*
 * -----------------------------------------------------------------------------------------
 *				Miscallenous privates procedures
 */

static Type_Icon *IconSearch(int Object)
/*
	Recherche dans la liste des icones d‚clar‚s, le pointeur sur le poste 
	d‚finissant l'icone portant le num‚ro d'objet Object.	
*/
{
	Type_Icon	*Icon,*Initial_Icon;
	
	Icon = FileSommet(File_Icon);
	if( Icon == NULL)
		return(NULL);
	else if(Icon->IconName == Object)
		return(Icon);
	else
	{
		Initial_Icon = Icon;
		FileTourne(File_Icon);
		Icon = FileSommet(File_Icon);
		while(!(Icon->IconName == Object || Icon == Initial_Icon))
		{
			FileTourne(File_Icon);
			Icon = FileSommet(File_Icon);
		}
		if(Icon->IconName == Object)
			return(Icon);
		else
			return(NULL);
	}
}

static int MoveIcon(Type_Icon *Icon,int *WindOrObject)
/*
	Effectue le mouvement d'un icone d'aprŠs ce qui sa passe … la souris.
*/
{
	int	finishx,finishy;
	int	ancx,ancy;
	int	Handle,Object;  
	int	mouse_x,mouse_y,dummy;
  
	graf_dragbox(	DesktopAddress[Icon->IconName].ob_width,DesktopAddress[Icon->IconName].ob_height,
					DesktopAddress[Icon->IconName].ob_x,DesktopAddress[Icon->IconName].ob_y+DesktopAddress->ob_y,
					0,DesktopMenu,DesktopW,DesktopH,
					&finishx,&finishy);
  
	/*
	 *	Read mouse position (x,y) in order to detect an eventuelly object under it
	 */
	graf_mkstate(&mouse_x,&mouse_y,&dummy,&dummy);
	/*
	 *	Check where icon is dragged (its ghost rectangle).
	 *	First: Window couldn't be under new position
	 */
	Handle	= wind_find(finishx,finishy);
	if(Handle <= DESKTOP)
	{
		Handle	= wind_find(finishx + DesktopAddress[Icon->IconName].ob_width ,finishy);
		if(Handle <= DESKTOP)
		{
			Handle	= wind_find(finishx + DesktopAddress[Icon->IconName].ob_width ,finishy+DesktopAddress[Icon->IconName].ob_height);
			if(Handle <= DESKTOP)
			{
				Handle	= wind_find(finishx,finishy+DesktopAddress[Icon->IconName].ob_height);
			}
		}
	}      
	/*
	 *	Second: Object couldn't be under new position
	 */
	Object	= objc_find(DesktopAddress,ROOT,DEPTH,mouse_x,mouse_y);
	if(Handle <= DESKTOP && (Object == 0 || Object == Icon->IconName))
	{
		ancx = DesktopAddress[Icon->IconName].ob_x;
		ancy = DesktopAddress[Icon->IconName].ob_y;
		DesktopAddress[Icon->IconName].ob_x = finishx;
		DesktopAddress[Icon->IconName].ob_y = finishy - DesktopAddress->ob_y;
		/*
		 *	Restore previous section icon
		 */
		form_dial(FMD_FINISH,0,0,0,0,ancx,
					 ancy,
					 DesktopAddress[Icon->IconName].ob_width * 2 ,
						 DesktopAddress[Icon->IconName].ob_height * 2);
		/*
		 *	Draw icon at new position
		 */
		objc_draw(DesktopAddress,Icon->IconName,ONELEVEL,
					DesktopAddress[Icon->IconName].ob_x,
						DesktopAddress[Icon->IconName].ob_y,
					DesktopAddress[Icon->IconName].ob_width,
						DesktopAddress[Icon->IconName].ob_height);
		*WindOrObject = 0;   			    	/* No window or object */
		return(ICONMOVED);
	}
	else
	{
		if(Handle > DESKTOP)				/* There is a window */
		{
			*WindOrObject = Handle;				/* Return handle of window */
			return(ICON2WIND);				/* return operation type */
		}
		else if(Object != 0 && Object != Icon->IconName &&	/* There is a NEW object */
			DesktopAddress[Object].ob_flags & SELECTABLE)
		{
			*WindOrObject = Object;				/* Return order number object */
			return(ICON2ICON);				/* return operation type */
		}
		else
			return(ICONMOVED);
	}
}

static void CancelIcon(int Icone)
/*
	Parcours toute l'arborescence Bureau et d‚sactive tous les Icones rencontr‚
	sauf celui pass‚ en paramŠtre.

	Entrees:	Icone … ne pas d‚s‚lectionner
	Sorties:	rien
*/
{
  int	Object;

  if(DesktopAddress != NULL)				/* Si un bureau est en place */
  {  
    Object=1;						/* A partir du premier objet */
    while(DesktopAddress[Object].ob_flags != LASTOB)	/* Tant que non dernier */
    {
      if((DesktopAddress[Object].ob_type == G_ICON || DesktopAddress[Object].ob_type == G_CICON) && 
         Object != Icone)
      {
        if(DesktopAddress[Object].ob_state & SELECTED)	
        {
          DesktopAddress[Object].ob_state &=~SELECTED;	/* D‚selectionner */
          F_RedrawObject(DesktopAddress,Object);	/* redessiner */
        }
      }
      Object++;						/* Objet suivant */
    }
  }
}

/*
 * -----------------------------------------------------------------------------------------
 *				Publics Procedures for PcXForm.C module ONLY
 */

int _InitDesk(void)
/*
	Initialize private structure to manage desktop'icons.
*/
{
  File_Icon	= CreerFile(sizeof(Type_Icon));		/* Liste des icones du bureau */
  return(File_Icon != NULL);
}

void _ExitDesk(void)
/*
	Initialize private structure to manage desktop'icons.
*/
{
  while(Defiler(File_Icon));
  KillFile(File_Icon);
}

void _DeskEvent(void)
/*
	Proc‚dure de gestion des ‚vŠnements souris sur un Icone ou le bureau lui meme.
	
	Entrees:	A_Clic
	Sorties:	
*/
{
	Type_Icon	*Icon;
	int			Object, movment, WindOrObject, dummy, bouton;
	
	
	Object	=	objc_find(DesktopAddress, ROOT, FIRSTCHILD, A_MouseX, A_MouseY);

	if(Object < ROOT)
		return;
		
	Object	&=	0x7FFF;										/* Clear double clic object */
	Icon   = IconSearch(Object);

	if(Icon != NULL)										/* Un icone trouve */
	{
		if( !(DesktopAddress[Object].ob_flags & DISABLED) && ( (DesktopAddress[Object].ob_flags & SELECTABLE) || (DesktopAddress[Object].ob_flags & TOUCHEXIT) ) )
		{
			evnt_timer(100,0);									/* Wait because we are so fast */
			graf_mkstate(&dummy,&dummy,&bouton,&dummy);			/* Relire bouton souris */
			if( bouton & GAUCHE )								/* Si bouton maintenue c'est un d‚placement */
			{
				/*	Si l'objet n'est pas la racine du dialog (Ici bureau) */
				if(Object > DESKTOP)
				{
					DesktopAddress[Object].ob_state |=SELECTED;
					F_RedrawObject(DesktopAddress,Object);
					movment = MoveIcon(Icon,&WindOrObject);
					I_Name	= Icon->IconName;
					switch(movment)
					{
						case ICONMOVED:								/* No effect */
										break;
			
						case ICON2ICON:	if(Icon->Move2Icon != NULL)
											(*Icon->Move2Icon)(movment,WindOrObject);
										break;
						case ICON2WIND:	if(Icon->Move2Wind != NULL)
											(*Icon->Move2Wind)(movment,WindOrObject);
										break;
						default:
										form_alert(1,ERROR_ICONMOVE);
					}
					DesktopAddress[Object].ob_state &=~SELECTED;
					F_RedrawObject(DesktopAddress,Object);
				}
				else
				{
					while(bouton & 0x03)
						graf_mkstate(&dummy,&dummy,&bouton,&dummy);

				}
			}
			else															/* C'est un x Clic */
			{
				I_Name	= Icon->IconName;
				if( A_MouseButton & GAUCHE)
				{
					if(A_Clic == 1 && Icon->ProcClic != NULL)				/* Clic souris */
					{
						if(Object > DESKTOP)
						{
							if(DesktopAddress[Object].ob_state & SELECTED)		/* Inverser son ‚tat */
								DesktopAddress[Object].ob_state &=~SELECTED;
							else
								DesktopAddress[Object].ob_state |=SELECTED;  
							F_RedrawObject(DesktopAddress,Object);
						}						
						(*Icon->ProcClic)();								/* User code */
					}

					if(A_Clic == 2 && Icon->ProcDbClic != NULL)				/* Double Clic */
					{
						if(Object > DESKTOP)
						{
							DesktopAddress[Object].ob_state |=SELECTED;			/* S‚lect‚ */
							F_RedrawObject(DesktopAddress,Object);
						}
						
						(*Icon->ProcDbClic)();								/* User Code */
						
						if(Object > DESKTOP)
						{
							DesktopAddress[Object].ob_state &=~SELECTED;		/* D‚select‚ */
							F_RedrawObject(DesktopAddress,Object);
						}
					}	

				}
				else if( A_MouseButton & DROIT)				
				{
					if(A_Clic == 1 && Icon->ProcClicDroit != NULL)			/* Clic souris */
					{
						if(Object > DESKTOP)
						{
							if(DesktopAddress[Object].ob_state & SELECTED)		/* Inverser son ‚tat */
								DesktopAddress[Object].ob_state &=~SELECTED;
							else
								DesktopAddress[Object].ob_state |=SELECTED;  
							F_RedrawObject(DesktopAddress,Object);
						}
						(*Icon->ProcClicDroit)();							/* User code */
					}

					if(A_Clic == 2 && Icon->ProcDbClicDroit != NULL)		/* Double Clic */
					{
						if(Object > DESKTOP)
						{
							DesktopAddress[Object].ob_state |=SELECTED;			/* S‚lect‚ */
							F_RedrawObject(DesktopAddress,Object);
						}
						
						(*Icon->ProcDbClicDroit)();							/* User Code */
						
						if(Object > DESKTOP)
						{						
							DesktopAddress[Object].ob_state &=~SELECTED;		/* D‚select‚ */
							F_RedrawObject(DesktopAddress,Object);
						}
					}	
				}
			}
      		CancelIcon(Icon->IconName);			/* D‚sactiver les autres */
		}
		CancelIcon(Object);
	}
	else	 						/* Clic sur bureau */
	{
		CancelIcon(Object);
	}
}

/*
 * -----------------------------------------------------------------------------------------
 *				Publics Aads users procedures
 */
 
int D_Open(int TreeName,Type_Bureau_List *List)
/*
	Ouvre un nouveau bureau pour l'application courante.
	
	Entr‚es:
	Sorties:
*/
{
  OBJECT		*arbre;
  int			desk_x,desk_y,desk_w,desk_h;
  unsigned long		Indice;
  Type_Icon		Icon;
  
  rsrc_gaddr(R_TREE,TreeName,&arbre);
  /*
   *	Calulate new desktop coordinate 
   */
  wind_get(DESKTOP,WF_WORKXYWH,&desk_x,&desk_y,&desk_w,&desk_h);
  arbre->ob_x		= desk_x;
  arbre->ob_y		= desk_y;
  arbre->ob_width	= desk_w;
  arbre->ob_height	= desk_h;
  DesktopAddress	= arbre;
  
  /*
   *	Place new Object (Icon) at coordinate (x,y) and assign its procedures
   */
  if(List != NULL)
  {
    Indice = 0L;
    while(!(List[Indice].Object			== 0 &&
    	    List[Indice].x				== 0 &&
    	    List[Indice].y				== 0 &&
    	    List[Indice].Titre			== NULL &&
    	    List[Indice].ProcClic		== NULL &&
    	    List[Indice].ProcDbClic		== NULL &&
    	    List[Indice].ProcClicDroit		== NULL &&
			List[Indice].ProcDbClicDroit	== NULL &&
    	    List[Indice].Move2Icon			== NULL &&
    	    List[Indice].Move2Wind		== NULL))
    {
    	/*	Si ce n'est pas le desktop lui meme ! */
    	if(List[Indice].Object > 0)
    	{
			arbre[List[Indice].Object].ob_x = List[Indice].x;
			arbre[List[Indice].Object].ob_y = List[Indice].y;
      		if(List[Indice].Titre != NULL)
        		strcpy(arbre[List[Indice].Object].ob_spec.iconblk->ib_ptext,List[Indice].Titre);
        }
      Icon.IconName		= List[Indice].Object;
      Icon.ProcClic		= List[Indice].ProcClic;
      Icon.ProcDbClic	= List[Indice].ProcDbClic;
      Icon.ProcClicDroit	= List[Indice].ProcClicDroit;
      Icon.ProcDbClicDroit	= List[Indice].ProcDbClicDroit;
      Icon.Move2Icon	= List[Indice].Move2Icon;
      Icon.Move2Wind	= List[Indice].Move2Wind;
      if(!Emfiler(File_Icon,&Icon))
        return(FALSE);
      Indice++;
    }
  }
  wind_set(DESKTOP,WF_NEWDESK,arbre,ROOT,0);
  form_dial(FMD_FINISH,0,0,0,0,desk_x,desk_y,desk_w,desk_h);
  return(TRUE);
}

void D_Close(void)
/*
	Ferme le bureau personnel de l'application et restaure le bureau officiel
	du GEM.
	
	Entr‚es:
	Sorties:
*/
{
  DesktopAddress = NULL;				/* Plus d'adresse de desktop */
  while(Defiler(File_Icon));				/* Vider la file des icones */
  wind_set(DESKTOP,WF_NEWDESK,0,0,ROOT,0);		/* Bureau initial du GEM */
}
