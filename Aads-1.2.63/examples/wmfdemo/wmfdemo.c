/*
		WMFDEMO.C: Demo program of AADS's Window-Menu.
		
		In this example, we display a dialog box and a window with Menu.
		
		06-20 December 1995 - Version 1.0
		Copyright Atari Funship (c) 1995		
*/

#include <stdio.h>
#include <stdlib.h>
#include <tos.h>
#include <string.h>

#include <PcAads.h>

#include "WMFDemoF.h"

/*
 * ---------------------------------------------------------------------------------------
 *				
 */
 
void Operation(int WindowHandle);

static Type_Window_Menu_List
WMenu1[]=	{	{ABOUT	,INITIAL,Operation},
			{QUITTER,INITIAL,Operation},
			{ATARI	,INITIAL,Operation},
			{AMIGA	,INITIAL,Operation},
			{APPLE	,INITIAL,Operation},
			{NEXT	,INITIAL,Operation},
			{UNIX	,INITIAL,Operation},
			{OPENW2	,DISABLED,Operation},
			{OPENM2	,DISABLED,Operation},
			WMENU_END
		};

static Type_Window_Menu_List
WMenu2[]=	{	{COMPILE,INITIAL,Operation},
			{CLOSEW2,INITIAL,Operation},
			{CLOSEM2,INITIAL,Operation},
			{ENDE	,INITIAL,Operation},
			WMENU_END
		};


void ObjectForm(void);

static Type_Form_List						/* Main Form Setup */
FormMain[]=	{	
			{OK		,ObjectForm,NULL},
			{CANCEL		,NULL,ObjectForm},
			{R1		,ObjectForm,NULL},
			{R2		,ObjectForm,NULL},
			{R3		,ObjectForm,NULL},
			{R4		,ObjectForm,NULL},
			FORM_END
		};

/*
 * ---------------------------------------------------------------------------------------
 *				
 */

#define		WATTRIBUTS		CLOSER|NAME|FULLER|MOVER|SIZER|INFO|SMALLER

/*
 * ---------------------------------------------------------------------------------------
 *				
 */

static	int				Sortir;
static  Type_Parameter_Window		*Window_Class;
static	int				WHandle1,WHandle2;
   
/*
 * ---------------------------------------------------------------------------------------
 *				Processing Procedure Windows
 */

void WDisplay(int Handle)
{
  int	Indice;
  
  W_HomeCursor(Handle);
  for(Indice=1; Indice < 5; Indice++)
    W_Printf(Handle,"Look, how GEM is very merveillous");
}

void WClose(int Handle)
{
  if(form_alert(1,"[0][ Do you want to exit ][Yes|No]")==1)
  {
    W_Close(Handle);
    Sortir = TRUE;
  }
}

/*
 * ---------------------------------------------------------------------------------------
 *				Processing Procedure Formular
 */

void ObjectForm(void)
{
  if(F_NameObject == OK || F_NameObject == CANCEL)
    WClose(WHandle1);
}

/*
 * ---------------------------------------------------------------------------------------
 *				Processing Procedure Menus
 */

void Operation(int WindowHandle)
{
  if(W_MenuModel == MAINMENU)						/* Window 1's menu */
  {
    switch(W_MenuItem)
    {
    case QUITTER:
    	WClose(WindowHandle);
    	break;
    case AMIGA:
    	form_alert(1,"[0][ Amiga ][ok]");
    	break;
    case ATARI:
    	form_alert(1,"[0][ Atari ][ok]");
    	break;
    case APPLE:
    	form_alert(1,"[0][ Apple ][ok]");
    	break;
    case NEXT:
    	form_alert(1,"[0][ NeXT ][ok]");
    	break;
    case UNIX:
    	form_alert(1,"[0][ UNIX ][ok]");
    	break;
    case OPENW2:
        WHandle2  = W_Open(Window_Class);				/* Opening window */
        W_MenuOpen(WHandle2,OTHERMENU,WMenu2);				/* Opening the menu 2 */
        W_MnDisable(WHandle1,OPENW2);
    	break;
    case OPENM2:
        W_MenuOpen(WHandle2,OTHERMENU,WMenu2);				/* Opening the menu 2 */
        W_MnDisable(WHandle1,OPENM2);
    	break;    
    case ABOUT:
       	form_alert(1,"[0][ AADS and Window Menu ][ok]");
    	break;
    default:
       	form_alert(1,"[0][ Unknow Item selected ][ok]");
    	break;
    }
  }
  else if(W_MenuModel == OTHERMENU)					/* Window 2's menu */
  {
     switch(W_MenuItem)
     {
       case COMPILE:
         form_alert(1,"[0][ AADS and Window Menu ][ok]");
    	 break;
       case CLOSEW2:
          W_MenuClose(WHandle2);
          W_Close(WHandle2);
          W_MnEnable(WHandle1,OPENW2);
          break;
       case CLOSEM2:
          W_MenuClose(WHandle2);
          W_MnEnable(WHandle1,OPENM2);
          break;
       case ENDE:
         WClose(WindowHandle);
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
  Type_Formular_Parameter	*Formulaire_Class;
  
  if(!A_Open("WMFDemoF.Rsc"))
  {
    form_alert(1,"[1][ | A_Open() Error ][ Ok ]");
    A_Close();
    return(0);							/* Quit immediatly */
  }

  W_Init();							/* Init Window  */
  F_Init();							/* Init Form    */
  M_Init();
  A_SetMouse(MS_ATARI,NULL);					/* Application's mouse */
  
  /*
   *	Ouverture des formulaires et fenetres
   */
  Formulaire_Class = F_GetParameter();
  Formulaire_Class->Closer	= FALSE;
  Formulaire_Class->TreeName	= FMAIN;
  Formulaire_Class->Title	= "Main Dialog Box";		/* Formular's title */
  Formulaire_Class->Mode	= F_WIND;			/* Opening mode */
  Formulaire_Class->FirstEdit	= EDIT1;			/* First Editable Text */
  Formulaire_Class->List	= FormMain;			/* Event list */
  Formulaire_Class->FormIcon	= W1ICON;			/* window's Icon  */
  Formulaire_Class->Mouse	= POINT_HAND;
  WHandle1 = F_XOpen(Formulaire_Class);
  

  Window_Class = W_GetParameter();				/* Get a block parameter */
  strcpy(Window_Class->titre,"AADS and the Window-Menu");
  strcpy(Window_Class->info ,"This is a usually window");
  Window_Class->attributs	= WATTRIBUTS;
  Window_Class->op_redraw	= WDisplay;
  Window_Class->op_close	= WClose;
  Window_Class->x		= 35;
  Window_Class->y		= 70;
  Window_Class->w		= 300;
  Window_Class->h		= 200;
  Window_Class->treename	= W2ICON;			/* Window's icon */
  Window_Class->Mouse		= OUTLN_CROSS;
  WHandle2  = W_Open(Window_Class);				/* Opening window */

  /*
   *	Install two menus's instance into the windows
   */
  W_MenuOpen(WHandle1,MAINMENU ,WMenu1);			/* Opening the menu instance 1 */
  W_MenuOpen(WHandle2,OTHERMENU,WMenu2);			/* Opening the menu instance 2 */

  /*
   *	Set the icon application (AES 4.1 and above only...)
   */
  W_SetIconApp(APPICON);

  Sortir = FALSE;
  do
  {
    A_WaitEvent();						/* Waiting an event	*/
    W_Event();
    M_Event();
    F_Event();				
  }
  while(!Sortir);

  free(Formulaire_Class);
  free(Window_Class);
  
  W_MenuClose(WHandle1);					/* Close the window's menu */
  W_MenuClose(WHandle2);					/* Close the window's menu */

  W_Close(WHandle2);
  F_Close(FMAIN);
  
  W_Exit();
  F_Exit();
  M_Exit();
  
  A_Close();							/* Close Application	*/
  return(0);							/* for the shell 	*/
}

