/*

		Atari Application Developpment Service:		A simple program Demo II
		
		30 December 1995
		05 January  1996
		Copyright Atari Funship (c) 1995 FRANCE. Atari ST and Falcon30
*/

#include <stdio.h>
#include <stdlib.h>
#include <tos.h>
#include <string.h>

#include <PcAads.h>

#include "DemoII_F.h"
#include "DemoDef.h"

/*
 * ---------------------------------------------------------------------------------------
 *				
 */

#define		HAUTEURMIN		399
#define		LARGEURMIN		639

#define		NBITEM			10
#define		WATTRIBUTS		CLOSER|NAME|FULLER|MOVER|SIZER|INFO|SMALLER

static	int				Sortir;
static	Type_Formular_Parameter		*FormMain_Class,*FormInfo_Class;
static  Type_Parameter_Window		*MWindow_Class,*Window_Class;
static	int				WDBHandle,WMHandle,WHandle; 
static	Type_AutoTop			AutoTop;
static	int				GrowShrink;

/*
 * ---------------------------------------------------------------------------------------
 *				Dialogs Box's events procedures
 */

void GoMainMenu(void)
/*
	Handle the entry of the main menu.
*/
{
  switch(M_Entry)
  {
    case ABOUT:		F_XOpen(FormInfo_Class);				/* Open Info dialog box */
    			break;	
    case QUITTER:	Sortir = TRUE;						/* Quit program */
    			break;	
    case OPENWDB:	WDBHandle = F_XOpen(FormMain_Class);			/* Open dialog box in window */
    			M_Enable(MAINMENU,OPENWMDB);
    			M_Disable(MAINMENU,OPENWDB);
    			break;	
    case OPENWMDB:	W_MenuOpen(WDBHandle,MENU1,WMenu1);			/* Opening the menu instance 1 */
    			M_Disable(MAINMENU,OPENWMDB);
    			break;	
    case OPENWM:	WMHandle  = W_Open(MWindow_Class);			/* Open standard GEM window */
    			W_MenuOpen(WMHandle,MENU2,WMenu2);
    			M_Enable(MAINMENU,FLIP);
    			break;	
    case OPENW:		WHandle  = W_Open(Window_Class);			/* Open window with menu built-in */
    			break;	
    case FLIP:		if(W_MenuExist(WMHandle))				/* Insert/Remove menu into the window GEM */
			  W_MenuClose(WMHandle);
    			else
    			  W_MenuOpen(WMHandle,MENU2,WMenu2);  
    			break;
    case MSAUTO:	if(!AutoTop.AutoEnable)					/* Auto top window On */
    			{
    			  AutoTop.AutoEnable = TRUE;
    			  M_Checked(MAINMENU,MSAUTO);
    			}
    			else							/* Or Off !! */
    			{
    			  AutoTop.AutoEnable = FALSE;
    			  M_UnChecked(MAINMENU,MSAUTO);
    			}
			W_AutoTop(&AutoTop);
    			break;
    case MSWORK:	if(!AutoTop.OnlyWorkArea)				/* Switch On/Off Work area only */
    			{
    			  AutoTop.OnlyWorkArea = TRUE;
    			  M_Checked(MAINMENU,MSWORK);
    			}
    			else
    			{
    			  AutoTop.OnlyWorkArea = FALSE;
    			  M_UnChecked(MAINMENU,MSWORK);
    			}
			W_AutoTop(&AutoTop);
    			break;
    case MSNOMOVE:	if(!AutoTop.NoMouseMovement)				/* Switch On/Off don't top during mouse-movement */
    			{
    			  AutoTop.NoMouseMovement = TRUE;
    			  M_Checked(MAINMENU,MSNOMOVE);
    			}
    			else
    			{
    			  AutoTop.NoMouseMovement = FALSE;
    			  M_UnChecked(MAINMENU,MSNOMOVE);
    			}
			W_AutoTop(&AutoTop);
    			break;
    case GROWSHRINK:	if(!GrowShrink)						/* GrwoShrink box was false before */
    			{
    			  GrowShrink = TRUE;
    			  W_VisualClue(TRUE);					/* Attempt to set On */
    			  M_Checked(MAINMENU,GROWSHRINK);
    			}
    			else							/* Was TRUE before */
    			{
    			  GrowShrink = FALSE;
    			  W_VisualClue(FALSE);					/* Set to FALSE */
    			  M_UnChecked(MAINMENU,GROWSHRINK);
    			}
    			break;
    default:		break;
  }
}

void GoMain(void)
/*
	This procedure handle the Main dialog box
*/
{
  switch(F_NameObject)
  {
    case CLOSE:	F_Close(FMAIN);							/* Exit button selected */
    		M_Disable(MAINMENU,OPENWMDB);
    		M_Enable(MAINMENU,OPENWDB);
    		break;
    default:	break;
  }
}

void GoInfo(void)
/*
	This procedure handle the Info dialog box.
*/
{
  switch(F_NameObject)
  {
    case OK:	F_Close(FINFO);							/* Close dialog box */
    		break;
    case MORE:	F_Open(FMORE,"What's more...",F_WIND,NOTEXT,FormMore);		/* Open dialog box more information */
    		break;
    default:	break;
  }
}

/*
 * ---------------------------------------------------------------------------------------
 *				Processing Procedure
 */

void GoPopup(void)
{
}

/*
 * ---------------------------------------------------------------------------------------
 *				Processing Procedure
 */

void IconClic(void)
/*
	This procedure handle the simple-click on any icon
*/
{
  switch(I_Name)
  {
    case IATARI:
    case IFLOPPY:
    case IMUSIC:
    case ICLIPBRD:
    case IECRAN:
    case ITELEPHONE:
    case ITRASH:	form_alert(1,"[0][ icon clicked: | I have nothing to do ][Ok]");
    			break;
    default:		break;
  }
}

void DbClic(void)
/*
	This procedure handle the double-click on any icon
*/
{
  char	Texte[10];
  
  switch(I_Name)
  {
    case IATARI:	F_Open(FSYS,"Atari Icon",F_WIND,NOTEXT,FormSys);
     			break;
    case IFLOPPY:	F_Open(FSYS,"Floppy Icon",F_WIND,NOTEXT,FormSys);
     			break;
    case IMUSIC:	F_Open(FSYS,"Music Icon",F_WIND,NOTEXT,FormSys);
     			break;
    case ICLIPBRD:	F_Open(FSYS,"ClipBoard Icon",F_WIND,NOTEXT,FormSys);
     			break;
    case IECRAN:	F_Open(FSYS,"Screen Icon",F_WIND,NOTEXT,FormSys);
     			break;
    case ITELEPHONE:	F_Open(FSYS,"Phone Icon",F_WIND,NOTEXT,FormSys);
     			break;
    case ITRASH:	F_Open(FSYS,"Trash Icon",F_WIND,NOTEXT,FormSys);
    			break;
    default:		break;
  }
  sprintf(Texte,"0x%X",MchInfo.TosVersion); 	F_WriteText(FSYS,FTOS,Texte);
  sprintf(Texte,"0x%X",MchInfo.AesVersion);	F_WriteText(FSYS,FAES,Texte);
  sprintf(Texte,"0x%X",MchInfo.GemdosVersion);	F_WriteText(FSYS,FGEMDOS,Texte);
  sprintf(Texte,"0x%lX",MchInfo.Machine);	F_WriteText(FSYS,FMACHINE,Texte);
  sprintf(Texte,"%d",MchInfo.Processor);	F_WriteText(FSYS,FPROCESSOR,Texte);
  sprintf(Texte,"%d",VdiInfo.LargeurStation);	F_WriteText(FSYS,FWIDTH,Texte);
  sprintf(Texte,"%d",VdiInfo.HauteurStation);	F_WriteText(FSYS,FHEIGHT,Texte);
  sprintf(Texte,"%d",VdiInfo.NombreCouleurs);	F_WriteText(FSYS,FCOLORS,Texte);
 }

void IconMove(int Move,int Target)
{
}

/*
 * ---------------------------------------------------------------------------------------
 *				Processing Procedure for the both built-in menu 
 */

void Operation(int WindowHandle)
/*
	Handle the both built-in menu for menu and dialog box.
*/
{
  if(W_MenuModel == MENU1)					/* Main dialog box's menu */
  {
    switch(W_MenuItem)
    {
      case APROPOS:	F_XOpen(FormInfo_Class);		/* Info dialog box */
      			break;
      case QUITTE:	
      case CLOSEW1:	F_Close(FMAIN);				/* Close Main dialog */
    			M_Disable(MAINMENU,OPENWMDB);
    			M_Enable(MAINMENU,OPENWDB);
		    	break;
      case CLOSEM1:	W_MenuClose(WindowHandle);		/* Remove its menu */
    			M_Enable(MAINMENU,OPENWMDB);
		    	break;    
      default:
		    	break;
    }
  }
  else if(W_MenuModel == MENU2)					/* Window's menu */
  {
     switch(W_MenuItem)
     {
       case COMPILE:	F_XOpen(FormInfo_Class);		/* Info dialog box */
      			break;
       case ENDE:
       case CLOSEW:	W_Close(WindowHandle);			/* Close window */
		    	break;
       case CLOSEM:	W_MenuClose(WindowHandle);		/* Remove its menu */
		    	break;    
       default:
			break;
      }
  }
  else
    ;
}

/*
 * ---------------------------------------------------------------------------------------
 *				Processing Procedure Windows
 */

void WMDisplay(int Handle)
/*
	Display procedure for window with built-in menu.
*/
{
  int	Indice;
  char	Texte[100];
  
  W_HomeCursor(Handle);
  for(Indice=1; Indice < 5; Indice++)
  {
    sprintf(Texte,"Line # %d, i love all lot GEM...",Indice);
    W_Printf(Handle,Texte);
  }
}

void WDisplay(int handle)
/*
	Display procedure for standard window. We're printing some global system informations.
*/
{
  char 	texte[50];
  
  W_HomeCursor(handle);	W_ClrScr(handle);
  W_Printf(handle,"");
  sprintf(texte,"Tos Version    : 0x%X",MchInfo.TosVersion);	W_Printf(handle,texte);
  sprintf(texte,"Aes Version    : 0x%X",MchInfo.AesVersion);	W_Printf(handle,texte);
  sprintf(texte,"Gemdos Version : 0x%X",MchInfo.GemdosVersion);	W_Printf(handle,texte);
  sprintf(texte,"Processor      : 0x%d",MchInfo.Processor);	W_Printf(handle,texte);
  sprintf(texte,"Machine        : %lX",MchInfo.Machine);	W_Printf(handle,texte);
}

void WClose(int Handle)
/*
	Close procedure for standard window and window with built-in menu.
*/
{
  W_MenuClose(Handle);						/* Remove the menu if exist */
  W_Close(Handle);						/* Close the window */
}

/*
 * ---------------------------------------------------------------------------------------
 *				Annexe du programme principal
 */

void MakeClass(void)
/*
	Cr‚e les classes qui seront utiles lors de l'ouverture de formulaires ou fenetres.
*/
{
  FormMain_Class 		= F_GetParameter();
  FormMain_Class->Smaller	= TRUE;
  FormMain_Class->TreeName	= FMAIN;
  FormMain_Class->Title		= "Main Dialog Box";		/* Formular's title */
  FormMain_Class->Mode		= F_WIND;			/* Opening mode */
  FormMain_Class->FirstEdit	= EDIT1;			/* First Editable Text */
  FormMain_Class->List		= FormMain;			/* Event list */
  FormMain_Class->FormIcon	= IMAIN;			/* window's Icon  */
  FormMain_Class->Mouse		= MS_POINTFLATHAND;		/* Aads's mouse shape */

  FormInfo_Class 		= F_GetParameter();
  FormMain_Class->Smaller	= TRUE;
  FormInfo_Class->TreeName	= FINFO;
  FormInfo_Class->Title		= "What's about...";		/* Formular's title */
  FormInfo_Class->Mode		= F_WIND;			/* Opening mode */
  FormInfo_Class->FirstEdit	= NOTEXT;			/* First Editable Text */
  FormInfo_Class->List		= FormInfo;			/* Event list */
  FormInfo_Class->FormIcon	= IINFO;			/* window's Icon  */
  FormInfo_Class->Mouse		= MS_HELP;			/* Aads's mouse shape */

  MWindow_Class = W_GetParameter();				/* Get a block parameter */
  strcpy(MWindow_Class->titre,"Window-Menu");
  strcpy(MWindow_Class->info ,"Built-in Menu");
  MWindow_Class->attributs	= WATTRIBUTS;
  MWindow_Class->op_redraw	= WMDisplay;
  MWindow_Class->op_close	= WClose;
  MWindow_Class->x		= 35;
  MWindow_Class->y		= 70;
  MWindow_Class->w		= 300;
  MWindow_Class->h		= 200;
  MWindow_Class->treename	= WMICON;			/* Window's icon */
  MWindow_Class->Mouse		= OUTLN_CROSS;

  Window_Class = W_GetParameter();				/* Get a block parameter */
  strcpy(Window_Class->titre,"Standart Window");
  strcpy(Window_Class->info ,"Without any addition");
  Window_Class->attributs	= WATTRIBUTS;
  Window_Class->op_redraw	= WDisplay;
  Window_Class->op_close	= WClose;
  Window_Class->x		= 55;
  Window_Class->y		= 90;
  Window_Class->w		= 300;
  Window_Class->h		= 200;
  Window_Class->treename	= WICON;			/* Window's icon */
  Window_Class->Mouse		= THIN_CROSS;
}

/*
 * ---------------------------------------------------------------------------------------
 *				Corps du programme principal
 */

int main(void)
{
  if(!A_Open("DemoII_F.Rsc"))
  {
    form_alert(1,"[1][ Application cannot | be lunched ][ Sorry ]");
    A_Close();
    return(-1);							/* Quit immediatly */
  }
  if(VdiInfo.HauteurStation < HAUTEURMIN || VdiInfo.LargeurStation < LARGEURMIN)		
  {
    form_alert(1,"[1][ Bad Screen Resolution | 640 x 400 At least ][ Ok ]");
    A_Close();
    return(-3);
  } 
  else
  {
    W_Init();							/* Init Window manager */
    F_Init();							/* Init Form manager */
    M_Init();							/* Init Menu manager */

    MakeClass();						/* Create all required class */
    
    A_SetMouse(MS_ATARI,NULL);					/* Application's mouse */
    W_SetIconApp(APPICON);					/* Global Application icon */

    M_Open(MAINMENU,MainMenu);					/* Create root menu */

    F_PopAttach(FMAIN,ListPopAttach);				/* Attach Pop-ups */
    F_PopEvent(ListPopEvent);					/* Install Pop-events */
    F_SetPopup(NBITEM);						/* number items displaying */

    D_Open(MYDESK,Bureau);					/* Open the application's desktop */
    
    GrowShrink = FALSE;
    Sortir = FALSE;
    do
    {
      A_WaitEvent();						/* Waiting an event */
      W_Event();						/* Window event manager */
      F_Event();						/* Formular (Dialog box) event manager */
      M_Event();						/* Menu event manager */
    }
    while(!Sortir);
  
    free(FormMain_Class);					/* free our blocks */
    free(FormInfo_Class);

    W_MenuClose(WDBHandle);					/* Close the window's menu */
    W_MenuClose(WMHandle);					/* Close the window's menu */

    W_Close(WMHandle);						/* Close window menu */
    W_Close(WHandle);						/* Close window */
 
    F_Close(FINFO);						/* Close all dialog box */
    F_Close(FMORE);
    F_Close(FMAIN);
    F_Close(FSYS);
    
    M_Close(MAINMENU);						/* Close main menu */
    D_Close();							/* Close desktop */
    
    W_Exit();							/* Close Aads sub parts */
    F_Exit();
    M_Exit();
    A_Close();							/* Close Application	*/
    return(0);							/* for the shell 	*/
  }
}

