/*
		WMDEMO.C: Demo program of Window-Menu with AADS.
		
		(Eh! Bill Gates, what do you think if i name it AADS-96...)
		
		06-20 December 1995 - Version 1.0
		Copyright Atari Funship (c) 1995		
*/

#include <stdio.h>
#include <stdlib.h>
#include <tos.h>
#include <string.h>

#include <PcAads.h>

#include "WMDemo_F.h"

/*
 * ---------------------------------------------------------------------------------------
 *				
 */
 
void Operation(int WindowHandle);

static Type_Window_Menu_List
WMenu1[]=	{	{ABOUT	,INITIAL,Operation},
			{OPEN	,INITIAL,Operation},
			{SAVE	,INITIAL,Operation},
			{QUITTER,INITIAL,Operation},
			{ATARI	,INITIAL,Operation},
			{AMIGA	,INITIAL,Operation},
			{APPLE	,INITIAL,Operation},
			{NEXT	,INITIAL,Operation},
			{UNIX	,INITIAL,Operation},
			{OPENW2	,DISABLED,Operation},
			{OPENM2	,DISABLED,Operation},
			{INSTALL,INITIAL,Operation},
			WMENU_END
		};

/*
 * ---------------------------------------------------------------------------------------
 *				
 */

#define		WATTRIBUTS		CLOSER|NAME|FULLER|MOVER|SIZER|INFO

/*
 * ---------------------------------------------------------------------------------------
 *				
 */

static	int				Sortir;
static  Type_Parameter_Window		*Window_Class;
static	int				WHandle1,WHandle2;
   

/*
 * ---------------------------------------------------------------------------------------
 *				
 */

void WDisplay(int Handle)
{
  W_HomeCursor(Handle);
  W_Printf(Handle,"it's a Gem window");
  W_Printf(Handle,"it's a Gem window");
  W_Printf(Handle,"it's a Gem window");
  W_Printf(Handle,"it's a Gem window");
}

void WClose(int Handle)
{
  if(form_alert(1,"[0][ Do you want to exit ][Yes|No]")==1)
  {
    W_Close(Handle);
    Sortir = TRUE;
  }
}

void Operation(int WindowHandle)
{
  if(W_MenuModel == MAINMENU)						/* Window 1's menu */
  {
    switch(W_MenuItem)
    {
    case OPEN : case SAVE:
    	form_alert(1,"[0][ Open/Save is processing][ok]");
    	break;
    case QUITTER:
    	WClose(WindowHandle);
    	break;
    case AMIGA: case ATARI: case APPLE: case NEXT: case UNIX:
    	form_alert(1,"[0][ Atari Amiga Apple NeXT UNIX ][ok]");
    	break;
    case OPENW2:
    	break;
    case OPENM2:
    	break;    
    case INSTALL:
    	form_alert(1,"[0][ Einstellugen ][ok]");
    	break;
    case ABOUT:
       	form_alert(1,"[0][ AADS and Window Menu ][ok]");
    	break;
    default:
       	form_alert(1,"[0][ Unknow Item selected ][ok]");
    	break;
    }
  }
  else
    ;
}

/*
 * ---------------------------------------------------------------------------------------
 *				Corps du programme principal
 */

int main(void)
{
  if(!A_Open("WMDemo_F.Rsc"))
  {
    form_alert(1,"[1][ | A_Open() Error ][ Ok ]");
    A_Close();
    return(0);							/* Quit immediatly */
  }

  W_Init();							/* Init Window  */
  F_Init();							/* Init Form    */
  M_Init();
  graf_mouse(ARROW,NULL);
  
  /*
   *		Ouverture des ‚l‚ments Menu, Bureau et fenetre
   */
  Window_Class = W_GetParameter();				/* Get a block parameter */

  strcpy(Window_Class->titre,"AADS and the Window-Menu (1)");
  strcpy(Window_Class->info ,"Look it's merveillous !");
  Window_Class->attributs	= WATTRIBUTS;
  Window_Class->op_redraw	= WDisplay;
  Window_Class->op_close	= WClose;
  Window_Class->x		= 15;
  Window_Class->y		= 50;
  Window_Class->w		= 300;
  Window_Class->h		= 200;
  WHandle1  = W_Open(Window_Class);				/* Opening window */

  strcpy(Window_Class->titre,"AADS and the Window-Menu (2)");
  strcpy(Window_Class->info ,"How are you ?");
  Window_Class->attributs	= WATTRIBUTS;
  Window_Class->op_redraw	= WDisplay;
  Window_Class->op_close	= WClose;
  Window_Class->x		= 35;
  Window_Class->y		= 70;
  Window_Class->w		= 300;
  Window_Class->h		= 200;
  WHandle2  = W_Open(Window_Class);				/* Opening window */

  
  /*
   *	Ajoute un menu … la fenetre
   */
  W_MenuOpen(WHandle1,MAINMENU ,WMenu1);			/* Opening the menu 1 */

  /*
   *		Event loop
   */
  Sortir = FALSE;
  do
  {
    A_WaitEvent();						/* Waiting an event	*/
    F_Event();				
    M_Event();
    W_Event();
  }
  while(!Sortir);

  free(Window_Class);

  W_MenuClose(WHandle1);					/* Close the window's menu */

  W_Close(WHandle1);
  W_Close(WHandle2);

  W_Exit();
  F_Exit();
  M_Exit();
  
  A_Close();							/* Close Application	*/
  return(0);							/* for the shell 	*/
}

