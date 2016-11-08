/*
		MousDemo.C: Demo program of AADS's Window-Menu.
		
		In this example, we display some mouse cursor shape.
		
		13-20 December 1995 - Version 1.0
		Copyright Atari Funship (c) 1995		
*/

#include <stdio.h>
#include <stdlib.h>
#include <tos.h>
#include <string.h>

#include <PcAads.h>

#include "MousDemF.h"

/*
 * ---------------------------------------------------------------------------------------
 *				
 */
 
void Operation(int WindowHandle);

static Type_Window_Menu_List
WMenu1[]=	{	{ABOUT	 ,INITIAL,Operation},
			{QUITTER ,INITIAL,Operation},
			{NEXT	 ,INITIAL,Operation},
			{PREVIOUS,INITIAL,Operation},
			WMENU_END
		};

void ObjectForm(void);

static Type_Form_List						/* Main Form Setup */
FormMain[]=	{	
			{OK		,ObjectForm,NULL},
			{BNEXT		,ObjectForm,NULL},
			{BPREVIOUS	,ObjectForm,NULL},
			FORM_END
		};

/*
 * ---------------------------------------------------------------------------------------
 *				
 */

static	int				Sortir;
static	int				WHandle1;

static	unsigned int			Actuelle;

#define	MAXX	20
static	int	Souris[] = {	ARROW,
				TEXT_CRSR,
				HOURGLASS ,
				BUSYBEE,
				POINT_HAND,
				FLAT_HAND,
				THIN_CROSS,
				THICK_CROSS,
				OUTLN_CROSS,
				
				MS_ATARI,
				MS_APPLE,
				MS_AMIGA,
				MS_MAC,
				MS_HOURGLASS,
				MS_BOMB,
				MS_DISK,
				MS_SLEEP,
				MS_POINTFLATHAND,
				MS_HELP,
				MS_MOUSE
			   };

/*
 * ---------------------------------------------------------------------------------------
 *				Processing Procedure Formular
 */

void ObjectForm(void)
{
  switch(F_NameObject)
  {
    case BNEXT:	W_NewMouse(WHandle1,Souris[++Actuelle % MAXX],NULL);
    		break;
    case BPREVIOUS:
    		W_NewMouse(WHandle1,Souris[--Actuelle % MAXX],NULL);
    		break;
    case OK:	Sortir = TRUE;
    		break;
  }
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
    	Sortir = TRUE;
    	break;
    case NEXT:
    	W_NewMouse(WindowHandle,Souris[++Actuelle % MAXX],NULL);
    	break;
    case PREVIOUS:
    	W_NewMouse(WindowHandle,Souris[--Actuelle % MAXX],NULL);
    	break;
    case ABOUT:
       	form_alert(1,"[0][ AADS Mouse demo ][ok]");
    	break;
    default:
       	form_alert(1,"[0][ Unknow Item selected ][ok]");
    	break;
    }
  }
}

/*
 * ---------------------------------------------------------------------------------------
 *				Corps du programme principal
 */

int main(void)
{
  Type_Formular_Parameter	*Formulaire_Class;
  
  
  if(!A_Open("MousDemF.Rsc"))
  {
    form_alert(1,"[1][ | A_Open() Error ][ Ok ]");
    A_Close();
    return(0);							/* Quit immediatly */
  }

  W_Init();							/* Init Window  */
  F_Init();							/* Init Form    */
  M_Init();
  A_SetMouse(MS_ATARI,NULL);					/* Application's mouse */
  Actuelle = ARROW;
  
  /*
   *	Ouverture des formulaires et fenetres
   */
  Formulaire_Class = F_GetParameter();
  Formulaire_Class->Closer	= FALSE;
  Formulaire_Class->TreeName	= FMAIN;
  Formulaire_Class->Title	= "AADS and the mouse";		/* Formular's title */
  Formulaire_Class->Mode	= F_WIND;			/* Opening mode */
  Formulaire_Class->FirstEdit	= NOTEXT;			/* First Editable Text */
  Formulaire_Class->List	= FormMain;			/* Event list */
  Formulaire_Class->FormIcon	= W1ICON;			/* window's Icon  */
  Formulaire_Class->Mouse	= Actuelle;
  WHandle1 = F_XOpen(Formulaire_Class);
  

  /*
   *	Install two menus to windows
   */
  W_MenuOpen(WHandle1,MAINMENU ,WMenu1);			/* Opening the menu 1 */

  /*
   *	Set the icon application (AES 4.1 and above)
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
  W_MenuClose(WHandle1);					/* Close the window's menu */
  F_Close(FMAIN);
  
  W_Exit();
  F_Exit();
  M_Exit();
  
  A_Close();							/* Close Application	*/
  return(0);							/* for the shell 	*/
}

