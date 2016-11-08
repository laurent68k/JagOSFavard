/*
 *			Window-Menu Sub-Library of Window:
 *
 *			This new feature for AADS work with all Atari Computer Serie.
 *
 *			It seems work under MultiTos without any problems...
 *
 *
 *	Author  : FunShip
 *	File	: PCXWMenu.c
 *	Date    : 06 December 1995
 *	Release : 30 Juillet 1997
 *	Version : 1.20
 *	Language: Pure C, Version du 3 Fevrier 1992 
 *	Country : FRANCE
 *	
 *
 *			- Copyright Atari FunShip (c) 1995,1996 -
 *			    - ATARI ST/STE/Mega/TT & Falcon -
 * -----------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * -----------------------------------------------------------------------------------------
 *			Original include from Aads library
 */

#include "..\Include\PcXApp.h"
#include "..\Include\PCXwind.h"
#include "Tools.h"
#include "Window.h"

/*
 * ---------------------------------------------------------------------------------------
 *				
 */
 
#define		TIME_ITEM	50				/* 50 Ms clignotement */
#define		NB_CLIGNOTE	7				/* 3 Enabled + 4 Disabled */

/*
 *	knowing classical menu's objects
 */
 
#define		MN_ROOT		0
#define		MN_BAR		1
#define		MN_ACTIVE	2
#define		MN_FIRST_GTITLE	3

#define		MU_NULL		0				


#define		ERROR_1		"[0][ PcXWMenu Warning Code #1: | No Code To Execute ][Ok]"
#define		ERROR_2		"[0][ PcXWMenu Warning Code #2: | Code To Execute = NULL ][Ok]"

/*
 * ---------------------------------------------------------------------------------------
 *				Publics datas for Menu management
 */

int		W_MenuItem;					/* Item selected */
int		W_MenuModel;					/* Menu Model Tree Name */
OBJECT		*W_MenuAddress;					/* Menu Instance Tree Address */

/*
 * ---------------------------------------------------------------------------------------
 *				New Type
 */

typedef	struct	{
				  int		Entry;				/* Menu Item selected */
				  int		Key;				/* Short key */
				  void (*Procedure)(int Handle);		/* Procedure to call */
				} Type_MenuEvent;
		

/*
 * ---------------------------------------------------------------------------------------
 *				Limited Private Procedures
 */

void RefreshMenu(Type_Window *Window)
/*
	Called when a redraw is required for a window. Well, we're redrawing the
	menu showed by MenuTreeName for the window WhindHandle under rectangles list
	supplied by PcXWind with its Red_xx clipping variables.
*/
{
  int		Dummy;
  
  wind_get(Window->handle,WF_WORKXYWH,&Window->MenuAddress->ob_x,&Window->MenuAddress->ob_y,&Dummy,&Dummy);
  objc_draw(Window->MenuAddress,1,DEPTH,Red_x,Red_y,Red_w,Red_h);
}

void AdjustPosition(Type_Window *Window)
/*
	When a window is moved we must recalculte the new position of the menu tree into
	the window, but we don't redraw it.
*/
{
  int		Dummy;
  
  wind_get(Window->handle,WF_WORKXYWH,&Window->MenuAddress->ob_x,&Window->MenuAddress->ob_y,&Dummy,&Dummy);
}

int CheckSize(int WindowHandle)
/*
	When a window was resized we check if the menu is or isn't biggest than the 
	window's width. 
	In this case, we display the two Scroller BoxChar or we hide them.
	
	return TRUE if the situation has been changed.
*/
{
  int		Indice,Somme,Object,WorkW,Dummy;
  Type_Window	*Window;
  
    /*
     *	If the window's width cannot contains all menu: Unhide the scroller BoxChar
     */
  Window = WindSearch(WindowHandle);

  if(Window == NULL || !Window->WithMenu)			/* Window doesn't found or haven't a menu */
    return(FALSE);
   
  wind_get(Window->handle,WF_WORKXYWH,&Dummy,&Dummy,&WorkW,&Dummy);
  Object = 3;
  Somme  = 0;
  while(Window->MenuAddress[Object].ob_type == G_TITLE)
  {
    Somme += Window->MenuAddress[Object].ob_width;
    Object = Window->MenuAddress[Object].ob_next;
  }
  
  if((Somme > WorkW) && 								/* Menu's width > Window's width */
     (Window->MenuAddress[Window->MenuAddress[MN_ACTIVE].ob_head].ob_flags & HIDETREE))
  {
    for(Indice=3;!(Window->MenuAddress[Indice].ob_flags & LASTOB);Indice++)
    {
      if(Window->MenuAddress[Indice].ob_type == G_TITLE)
        Window->MenuAddress[Indice].ob_x += Window->MenuAddress[Window->MenuAddress[MN_ACTIVE].ob_head].ob_width + Window->MenuAddress[Window->MenuAddress[MN_ACTIVE].ob_head].ob_width;
      if(Window->MenuAddress[Indice].ob_type == G_BOX)
        Window->MenuAddress[Indice].ob_x += Window->MenuAddress[Window->MenuAddress[MN_ACTIVE].ob_head].ob_width + Window->MenuAddress[Window->MenuAddress[MN_ACTIVE].ob_head].ob_width;
    }
    Window->MenuAddress[Window->MenuAddress[MN_ACTIVE].ob_head].ob_flags &= ~HIDETREE;
    Window->MenuAddress[Window->MenuAddress[Window->MenuAddress[2].ob_head].ob_next].ob_flags &= ~HIDETREE;
    W_Redraw(Window->handle);  
    return(TRUE);
  }
  else if((Somme < WorkW) && 								/* Window's width > Menu's width */
          !(Window->MenuAddress[Window->MenuAddress[MN_ACTIVE].ob_head].ob_flags & HIDETREE) &&
          Window->NextGTitle == MN_FIRST_GTITLE)							
  {
    for(Indice=MN_FIRST_GTITLE; !(Window->MenuAddress[Indice].ob_flags & LASTOB); Indice++)
    {
      if(Window->MenuAddress[Indice].ob_type == G_TITLE)
        Window->MenuAddress[Indice].ob_x -= Window->MenuAddress[Window->MenuAddress[MN_ACTIVE].ob_head].ob_width + Window->MenuAddress[Window->MenuAddress[MN_ACTIVE].ob_head].ob_width;
      if(Window->MenuAddress[Indice].ob_type == G_BOX)
        Window->MenuAddress[Indice].ob_x -= Window->MenuAddress[Window->MenuAddress[MN_ACTIVE].ob_head].ob_width + Window->MenuAddress[Window->MenuAddress[MN_ACTIVE].ob_head].ob_width;
    }
    Window->MenuAddress[Window->MenuAddress[MN_ACTIVE].ob_head].ob_flags |= HIDETREE;
    Window->MenuAddress[Window->MenuAddress[Window->MenuAddress[MN_ACTIVE].ob_head].ob_next].ob_flags |= HIDETREE;
    return(TRUE);
  }
  else
    return(FALSE);
}

/*
 * ---------------------------------------------------------------------------------------
 *				Scrolling Window Menu procedures
 */
static void MoveRight(Type_Window *Window,int Scroller,int WorkX,int WorkY,int WorkW,int WorkH)
/*
*/
{
  int	Indice;

  if(Window->MenuAddress[Window->NextGTitle].ob_type == G_TITLE && 			/* Current is a G_Title and next too */
     Window->MenuAddress[Window->MenuAddress[Window->NextGTitle].ob_next].ob_type == G_TITLE)
  {
    Window->MenuAddress[Window->NextGTitle].ob_flags |= HIDETREE;			/* Hide current G_Title */
    for(Indice=Window->NextGTitle+1;!(Window->MenuAddress[Indice].ob_flags & LASTOB);Indice++)
    {											/* Move to left all Title and Boxmenu */
    if(Window->MenuAddress[Indice].ob_type == G_TITLE || Window->MenuAddress[Indice].ob_type == G_BOX)
      Window->MenuAddress[Indice].ob_x -=Window->MenuAddress[Window->NextGTitle].ob_width;
    }

    Window->MenuAddress[Window->MenuAddress[MN_ACTIVE].ob_head].ob_state &= ~DISABLED;

    objc_draw(Window->MenuAddress,1,DEPTH,WorkX,WorkY,WorkW,WorkH);
    Window->NextGTitle++;								/* Next G_Title to hide */
  }
  /*
   *	D‚sactive si le l'objet suivant le prochain  G_Title n'est pas un titre
   */
  if(Window->MenuAddress[Window->MenuAddress[Window->NextGTitle].ob_next].ob_type != G_TITLE)
  {
    Window->MenuAddress[Scroller].ob_state |= DISABLED;
    objc_draw(Window->MenuAddress,Scroller,1,WorkX,WorkY,WorkW,WorkH);
  }
}

static void MoveLeft(Type_Window *Window,int Scroller,int WorkX,int WorkY,int WorkW,int WorkH)	/* Redraw all menu scrolled */
/*
*/
{
  int	Indice;
  
  
  if(Window->MenuAddress[Window->NextGTitle-1].ob_type == G_TITLE)			/* If previous is a G_TITLE */
  {
    Window->NextGTitle--;								/* Go to previous */
    Window->MenuAddress[Window->NextGTitle].ob_flags &= ~HIDETREE;			/* Unhide it */
    for(Indice=Window->NextGTitle+1;!(Window->MenuAddress[Indice].ob_flags & LASTOB);Indice++)
    {											/* Move to right all Title and Boxmenu */
      if(Window->MenuAddress[Indice].ob_type == G_TITLE  || Window->MenuAddress[Indice].ob_type == G_BOX)
        Window->MenuAddress[Indice].ob_x +=Window->MenuAddress[Window->NextGTitle].ob_width;
    }
    Window->MenuAddress[Window->MenuAddress[Scroller].ob_next].ob_state &= ~DISABLED;

    objc_draw(Window->MenuAddress,1,DEPTH,WorkX,WorkY,WorkW,WorkH);			/* Redraw all menu scrolled */
  }
  /*
   *	D‚sactive si le l'objet pr‚cedent le prochain  G_Title n'est pas un titre
   */
  if(Window->MenuAddress[Window->NextGTitle-1].ob_type != G_TITLE)
  {
    Window->MenuAddress[Scroller].ob_state |= DISABLED;
    objc_draw(Window->MenuAddress,Scroller,1,WorkX,WorkY,WorkW,WorkH);
  }
}

static void SlideMenu(Type_Window *Window,int Object)
/*
	This routine call the procedures required for each of two arrow BOXCHAR
	to perform the scrolling.
*/
{
  int	Etat,WorkX,WorkY,WorkW,WorkH;
  	
  if(!(Window->MenuAddress[Object].ob_state & DISABLED))
  {
    wind_get(Window->handle,WF_WORKXYWH,&WorkX,&WorkY,&WorkW,&WorkH);				/* Window work area */

    Etat = Window->MenuAddress[Object].ob_state | SELECTED;
    objc_change(Window->MenuAddress,Object,0,WorkX,WorkY,WorkW,WorkH,Etat,TRUE);		/* Titre selected */

    if((Window->MenuAddress[Object].ob_spec.index & 0xFF000000L) == 0x03000000L)		/* Right arrow */
      MoveRight(Window,Object,WorkX,WorkY,WorkW,WorkH);
    else											/* Left arrow */
    {
      MoveLeft(Window,Object,WorkX,WorkY,WorkW,WorkH);
      if(CheckSize(Window->handle))
        W_Redraw(Window->handle);
    }

    Etat = Window->MenuAddress[Object].ob_state & ~SELECTED;
    objc_change(Window->MenuAddress,Object,0,WorkX,WorkY,WorkW,WorkH,Etat,TRUE);		/* Titre selected */
    evnt_timer(250,0);										/* Wait a moment */
  }
}

/*
 * ---------------------------------------------------------------------------------------
 *				Limited Private Procedure: CallBack Code Handler
 */

static int MenuProcess(Type_Window *Window,int Item)
/*
	This procedure call the code to execute when an item is selected into
	a window menu.
*/
{ 
  Type_MenuEvent	*Premier,*Courant;
  
  Premier = FileSommet(Window->MenuListEvent);
  if(Premier->Entry != Item)
  {
    FileTourne(Window->MenuListEvent);
    Courant = FileSommet(Window->MenuListEvent);
    while(Courant != Premier && Courant->Entry != Item)
    {
      FileTourne(Window->MenuListEvent);
      Courant = FileSommet(Window->MenuListEvent);
    }
    Premier = Courant;
  }

  if(Premier->Entry == Item)						/* If Item's event found */
  {
    W_MenuItem 		= Item;						/* Global variables assigned */
    W_MenuModel		= Window->MenuModelName;
    W_MenuAddress 	= Window->MenuAddress;
    if(Premier->Procedure != NULL)
      (*Premier->Procedure)(Window->handle);				/* Call user code */
    else
      form_alert(1,ERROR_2);						/* Code == NULL => warning */
    return(TRUE);
  }
  form_alert(1,ERROR_1);
  return(FALSE);
}

static void WMnKbd(void)
/*
	This procedure handle the keyboard event. Because, menu have may be short key
	we must check when a keybaord event occurs if it's destinated to own menu into
	the TOP window.
*/
{
  int			Touche,WindowHandle,Dummy;
  Type_Window		*Window;
  Type_MenuEvent	*Premier,*Courant;
  
  Touche = ((A_Key & 0x00FF) == 0) ? A_Key & 0xFF00 : A_Key & 0x00FF;
  wind_get(DESKTOP,WF_TOP,&WindowHandle,&Dummy,&Dummy,&Dummy);		/* Search window Handle TOP */
  Window = WindSearch(WindowHandle);					/* If this window is own */

  if(Window != NULL && Window->WithMenu)				/* It's a window with menu */
  {
    if(!EstFileVide(Window->MenuListEvent))
    {
      Premier = FileSommet(Window->MenuListEvent);
      if(Premier->Key != Touche)					/* Not this Key */
      {
        FileTourne(Window->MenuListEvent);
        Courant = FileSommet(Window->MenuListEvent);			/* Read next */
        while(Premier != Courant && Courant->Key != Touche)
        {						
          FileTourne(Window->MenuListEvent);
          Courant = FileSommet(Window->MenuListEvent);
        }
        Premier = Courant;
      }
      /*
       *	We have found the short key and not disabled.
       */
      if(Premier->Key == Touche && !(Window->MenuAddress[Premier->Entry].ob_state & DISABLED))
      {
        W_MenuItem 	= Premier->Entry;
        W_MenuModel 	= Window->MenuModelName;
        W_MenuAddress 	= Window->MenuAddress;
        if(Premier->Procedure != NULL)
          (*Premier->Procedure)(Window->handle);
        else
          form_alert(1,ERROR_2);
        A_Event = MU_NULL;
      }
    }
  }
}

/*
 * ---------------------------------------------------------------------------------------
 *				Private Procedures: Event Manager
 */

void WMnEvent(void)
/*
	This procedure handle entirely the window-menu. It does the graphic management and
	call the code manager when an entry is selected.
*/
{
	int					MouseX,MouseY,MouseButton,Dummy,Etat;
	int					WorkX,WorkY,WorkW,WorkH,RegionX,RegionY;
	int					WindowHandle,Object,BoxMenu,OldEntry,OldTitre;
	Type_Buffer_Screen	*RegionBoite;
	Type_Window			*Window;
    
	graf_mkstate(&MouseX,&MouseY,&MouseButton,&Dummy);						/* Reading (x,y) and button */
	if((A_Event & MU_BUTTON) && (MouseButton & 0x01))						/* If Left Mouse Button pressed */
	{
		WindowHandle = wind_find(MouseX,MouseY);							/* Find window at this position */
		Window = WindSearch(WindowHandle);    							/* If this window is own */
		
		/*
		 *	If the window is found and not inconified and contains a menu
		 */

		if(Window != NULL && !Window->isiconify && Window->WithMenu)
		{
			Object = objc_find(Window->MenuAddress,2,DEPTH,MouseX,MouseY);				/* What's object at (x,y) */

			if(Object <= 0)
				return;
   
			if(Window->MenuAddress[Object].ob_type == G_BOXCHAR)
			{
				SlideMenu(Window,Object);
				return;
			}
			else if(Window->MenuAddress[Object].ob_type == G_TITLE && !(Window->MenuAddress[Object].ob_state & DISABLED))	/* It's a menu title not disabled */
			{
				BoxMenu = CalculBoite(Window->MenuAddress,Object);					/* N d'ordre de la boite */
				wind_get(WindowHandle,WF_WORKXYWH,&WorkX,&WorkY,&WorkW,&WorkH);				/* Window work area */

				Etat = Window->MenuAddress[Object].ob_state | SELECTED;					/* New Titre selected */
				objc_change(Window->MenuAddress,Object,0,WorkX,WorkY,WorkW,WorkH,Etat,TRUE);		/* Titre selected */
				OldTitre = Object;

				objc_offset(Window->MenuAddress,BoxMenu,&RegionX,&RegionY);				/* (x,y) region to save */
				RegionBoite = ScreenSave(VdiHandle,RegionX-1,RegionY-1,Window->MenuAddress[BoxMenu].ob_width+2,Window->MenuAddress[BoxMenu].ob_height+2);
				objc_draw(Window->MenuAddress,BoxMenu,DEPTH,WorkX,WorkY,WorkW,WorkH);			/* Draw menu box */
        
				while((MouseButton & 0x01) == TRUE)							/* Wait Mouse released */
					graf_mkstate(&MouseX,&MouseY,&MouseButton,&Dummy);

				/*
				 *	Window Menu Management Loop
				 */        
		 
				graf_mkstate(&MouseX,&MouseY,&MouseButton,&Dummy);
				OldEntry = BoxMenu;
				while( !(MouseButton & 0x01))								/* Left button pressed */
				{	
					Object = objc_find(Window->MenuAddress,BoxMenu,DEPTH,MouseX,MouseY);			/* Search into box menu */
					Object = (MouseX >= WorkX && MouseX <= WorkX+WorkW-1 && MouseY >= WorkY && MouseY <= WorkY+WorkH-1) ? Object : -1;
					if( Object > 0 && (Window->MenuAddress[Object].ob_type == G_STRING) && !(Window->MenuAddress[Object].ob_state & DISABLED))
					{
						if(Object != OldEntry)
						{
							Etat = Window->MenuAddress[Object].ob_state | SELECTED;				/* New Entry selected */
							objc_change(Window->MenuAddress,Object,0,WorkX,WorkY,WorkW,WorkH,Etat,TRUE);
							Etat = Window->MenuAddress[OldEntry].ob_state & ~SELECTED;			/* Old Entry cleared */
							objc_change(Window->MenuAddress,OldEntry,0,WorkX,WorkY,WorkW,WorkH,Etat,TRUE);
							OldEntry = Object;								/* Copy current object */
						}
					}
					else 
					{
						Object = objc_find(Window->MenuAddress,1,DEPTH,MouseX,MouseY);
						if( Object > 0 && (Window->MenuAddress[Object].ob_type == G_TITLE) && !(Window->MenuAddress[Object].ob_state & DISABLED))
						{
							if(Object != OldTitre)								/* It's another title */
							{
								Etat = Window->MenuAddress[OldEntry].ob_state & ~SELECTED;			/* Old Entry cleared */
								objc_change(Window->MenuAddress,OldEntry,0,WorkX,WorkY,WorkW,WorkH,Etat,FALSE);

								Etat = Window->MenuAddress[Object].ob_state | SELECTED;				/* New Titre selected */
								objc_change(Window->MenuAddress,Object,0,WorkX,WorkY,WorkW,WorkH,Etat,TRUE);
								Etat = Window->MenuAddress[OldTitre].ob_state & ~SELECTED;			/* Old Titre cleared */
								objc_change(Window->MenuAddress,OldTitre,0,WorkX,WorkY,WorkW,WorkH,Etat,TRUE);
	      
								ScreenRestore(RegionBoite);
								BoxMenu = CalculBoite(Window->MenuAddress,Object);				/* N d'ordre de la boite */
								wind_get(WindowHandle,WF_WORKXYWH,&WorkX,&WorkY,&WorkW,&WorkH);
								objc_offset(Window->MenuAddress,BoxMenu,&RegionX,&RegionY);
								RegionBoite = ScreenSave(VdiHandle,RegionX-1,RegionY-1,Window->MenuAddress[BoxMenu].ob_width+2,Window->MenuAddress[BoxMenu].ob_height+2);
								objc_draw(Window->MenuAddress,BoxMenu,DEPTH,WorkX,WorkY,WorkW,WorkH);
	
								OldTitre = Object;								/* New active title */
								OldEntry = BoxMenu;								/* Old entry selected */
							}
						}
						else										/* Clic Mouse outside */
						{
							if((Window->MenuAddress[OldEntry].ob_state & SELECTED) == TRUE)			/* Souris sortie du Menu d‚roulant */
							{
								Etat = Window->MenuAddress[OldEntry].ob_state & ~SELECTED;			/* Old Entry cleared */
								objc_change(Window->MenuAddress,OldEntry,0,WorkX,WorkY,WorkW,WorkH,Etat,TRUE);
								OldEntry = BoxMenu;
							}
						}
					}
					graf_mkstate(&MouseX,&MouseY,&MouseButton,&Dummy);					/* Next mouse location */
				}
			}/* EndWhile */
	
			/*
			 *	Fait clignoter l'item du menu s‚lectionn‚ si il existe (!), juste pour faire jolie !!
			 */
	 
			if(OldEntry != BoxMenu && OldEntry > 2)
			{	
				for(Dummy=1;Dummy<= NB_CLIGNOTE;Dummy++)
				{
					Etat = (Window->MenuAddress[OldEntry].ob_state & SELECTED) ? Window->MenuAddress[OldEntry].ob_state & ~SELECTED : Window->MenuAddress[OldEntry].ob_state | SELECTED;	/* Old Entry cleared */
					objc_change(Window->MenuAddress,OldEntry,0,WorkX,WorkY,WorkW,WorkH,Etat,TRUE);
					evnt_timer(TIME_ITEM,0);
				}
			}
			ScreenRestore(RegionBoite);							/* Restore graphic area */
	
			/*
			 *	Call the procedure defined for the selected item and disabled the G_TITLE
			 */
		 
			Etat = Window->MenuAddress[OldTitre].ob_state & ~SELECTED;			/* New Titre selected */
			objc_change(Window->MenuAddress,OldTitre,0,WorkX,WorkY,WorkW,WorkH,Etat,TRUE);/* Titre selected */
			
			if(OldEntry != BoxMenu && OldEntry > 2)						/* if item selected */
	        	MenuProcess(Window,OldEntry);							/* Call users code */
		}
		else
		{
    	
		}  
	}
	else if(A_Event & MU_KEYBD)								/* If is a keyboard event */
	{
			WMnKbd();
	}
	else
		;
}

/*
 * ---------------------------------------------------------------------------------------
 *				Publics Procedures
 */
		

OBJECT *W_MenuOpen(int WindowHandle, int MenuTreeName, Type_Window_Menu_List *List)
/*
	Open a menu into a target window with a behavioural list for each all item.
	The menu will be display under control of REDRAW message sended at the end
	of this function.
	
	Return the Menu instance's address if created else NULL.
*/
{
  int			WorkW,WorkH;
  Type_Window		*Window;  
  Type_MenuEvent	Event;
  unsigned long		Indice;
    
  /*
   *	Install datas structures to manage the window-menu.
   */
  Window = WindSearch(WindowHandle);
  if(Window != NULL)
  {
    Window->WithMenu		= TRUE;					/* Have got a menu */
    Window->MenuModelName	= MenuTreeName;				/* Name of Menu Tree Model */
    Window->MenuListEvent	= CreerFile((unsigned long)sizeof(Type_MenuEvent));
    Window->NextGTitle		= MN_FIRST_GTITLE;				/* First G_Title to begin menu display */
       
    /*	WARNING: Create a new instance of the menu tree passed in parameter ! */
    if(!rsrc_gaddr(R_TREE,MenuTreeName,&Window->MenuAddress))
      return(NULL);
    Window->MenuAddress = CpMenu(Window->MenuAddress);
       
    /*
     *	Insert into MenuEventList all object that call process when an event occurs.
     */
    Indice = 0L;							/* First list's atom */
    while(!(List[Indice].Item      == 0    &&
    	    List[Indice].Procedure == NULL &&
  	    List[Indice].State     == 0))
    {
      Event.Entry 	= List[Indice].Item;				/* Copy indicie entry */
      Event.Procedure	= List[Indice].Procedure;			/* Address of procedure */
      Event.Key 	= CalculKey(Window->MenuAddress[Event.Entry].ob_spec.free_string);	/* Short-key */	

      if(!Emfiler(Window->MenuListEvent,&Event))		     	/* Insert new data */
        return(NULL);
      Window->MenuAddress[Event.Entry].ob_state |= List[Indice].State;	/* Entry initial State */ 		
      Indice++;
    }
  
    /*
     *	Now, we allows the menu display into the target window with the next redraw.
     */
    wind_get(WindowHandle,WF_WORKXYWH,&Window->MenuAddress->ob_x,&Window->MenuAddress->ob_y,&WorkW,&WorkH);
    Window->MenuAddress[MN_BAR].ob_spec.index = 0xFF1101L;
 
    /*
     *	check if the window's width can or not contains all menu.
     */
    CheckSize(Window->handle);

    W_Redraw(WindowHandle);											/* Redessiner */
    return(Window->MenuAddress); 
  }  
  else
   return(NULL);
}

int W_MenuClose(int WindowHandle)
/*
	Remove a menu from the target window of handle WindowHandle.
	We're delete the list MenuListEvent for each menu's item and assign the
	flag WithMenu to FALSE. A Redraw message is sended to refresh the window's contains.
	
*/
{
  Type_Window	*Window;  

  Window = WindSearch(WindowHandle);
  if(Window != NULL && Window->WithMenu)							/* If window exist and have got a menu */
  {
    Window->WithMenu		= FALSE;								/* Without Menu */
    KillFile(Window->MenuListEvent);								/* Clear Menu Event List */
    free(Window->MenuAddress);										/* Free menu instance */
    W_Redraw(Window->handle);										/* Redessiner */
    return(TRUE);
  }
  else
    return(FALSE);
}

int W_MenuExist(int WindowHandle)
/*
	Return TRUE if the window of handle WindowHandle contains a menu.
*/
{
  Type_Window	*Window;
  
  Window = WindSearch(WindowHandle);
  if(Window != NULL)
    return(Window->WithMenu);
  return(FALSE);
}

int W_WorkXYWH(int WindowHandle,int *Area_X,int *Area_Y,int *Area_W,int *Area_H)
/*
	Compute and return the real work area for a target window WindowHandle.
	If the window have got a menu, we substract the menu's height from work area.
*/
{
  Type_Window	*Window;
  
  Window = WindSearch(WindowHandle);								/* Search in own window list */
  if(Window != NULL)												/* If found */
  {
    wind_get(Window->handle,WF_WORKXYWH,Area_X,Area_Y,Area_W,Area_H);
    if(Window->WithMenu)											/* Have got a built-in menu bar */
    {
      *Area_Y	=	*Area_Y + Window->MenuAddress[1].ob_height + 1;
      *Area_H	=	*Area_H - Window->MenuAddress[1].ob_height - 2;
    }
    return(TRUE);
  }
  return(FALSE);
}

int W_MnDisable(int WindowHandle,int Object)
/*
	Change the object's state with the DISABLED bit set to zero.
*/
{
  Type_Window	*Window;
  
  Window = WindSearch(WindowHandle);
  if(Window != NULL)
  {
    Window->MenuAddress[Object].ob_state |= DISABLED;
    return(TRUE);
  }
  return(FALSE);
}

int W_MnEnable(int WindowHandle,int Object)
/*
	Change the object's state with the DISABLED bit set to one.
*/
{
  Type_Window	*Window;
  
  Window = WindSearch(WindowHandle);
  if(Window != NULL)
  {
    Window->MenuAddress[Object].ob_state &= ~DISABLED;
    return(TRUE);
  }
  return(FALSE);
}

int W_MnCheck(int WindowHandle,int Object)
/*
	Change the object's state with the CHECKED bit set to one.
*/
{
  Type_Window	*Window;
  
  Window = WindSearch(WindowHandle);
  if(Window != NULL)
  {
    Window->MenuAddress[Object].ob_state |= CHECKED;
    return(TRUE);
  }
  return(FALSE);
}

int W_MnUnCheck(int WindowHandle,int Object)
/*
	Change the object's state with the CHECKED bit set to zero.
*/
{
  Type_Window	*Window;
  
  Window = WindSearch(WindowHandle);
  if(Window != NULL)
  {
    Window->MenuAddress[Object].ob_state &= ~CHECKED;
    return(TRUE);
  }
  return(FALSE);
}