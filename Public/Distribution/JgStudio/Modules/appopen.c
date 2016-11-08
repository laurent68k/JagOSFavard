/*
 * ----------------------------------------------------------------------------------------------
 *					Chargement des informations … partir du .INF
 *
 * 	Author  : 	FAVARD Laurent
 *	File    : 	Appopen.C
 *	Date    : 	18 July 1996
 *	Release : 	09 May 2000
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
#include	"..\tos.rsc\jaguar_F.H"
#endif

/*
 * ----------------------------------------------------------------------------------------------
 */

#include	"..\modules\Jaguar.h"
#include	"..\modules\messages.h"												/*	Messages textes */
#include	"..\modules\Tools.h"												/*	Tools !! */
#include	"..\modules\mainmenu.h"

#include	"..\fenetres\console.h"										/*	Gestion de la fenetre terminal Jaguar */
#include	"..\fenetres\Options.h"
#include	"..\fenetres\Toolsbar.h"
#include	"..\fenetres\bureau.h"
#include	"..\fenetres\Fuji.h"
#include	"..\fenetres\status.h"

/*
 * ----------------------------------------------------------------------------------------------
 */

int AppOpen(void)
/*
	Proc‚dure appel‚e lorsque tout l'initialisation de AADS est faite, pour coder ici les besoin
	de l'application.
	
	retourne le port d'E/S a utiliser.
*/
{
	char			ValeurChamp[20];
	unsigned int	ScreenResolutionX, ScreenResolutionY;
	int				Port;

	/*	Est ce que la resolution a changee depuis la derniere fois ? */
	ScreenResolutionX	=	(unsigned int)GetLongProfile(G_APP_INF, ECRAN, "ResolutionX", 0);
	ScreenResolutionY	=	(unsigned int)GetLongProfile(G_APP_INF, ECRAN, "ResolutionY", 0);
	if( ScreenResolutionX != 0 && ScreenResolutionY != 0 )
	{
		G_ResolutionChanged	= ( ScreenResolutionX != VdiInfo.LargeurStation || ScreenResolutionY != VdiInfo.HauteurStation);
		if(G_ResolutionChanged)
			G_ResolutionChanged	=	(form_alert(1, GetMessage(WRNG_SCREEN)) == 1 ) ? TRUE : FALSE;
			
	}
	else
		G_ResolutionChanged	=	TRUE;


	/*	d‚termine si un pr‚c‚dent fichier executable est enregistr‚ dans le .INF */
	GetProfileString(G_APP_INF, PROJET, EXE_NAME, Projet.ExeFileName, "");	
	GetProfileString(G_APP_INF, PROJET, EXE_PATHNAME, Projet.ExePathName, "");
	GetProfileString(G_APP_INF, PROJET, EXE_EXTENSION, Projet.ExeExtension, EXT_JAG);
	GetProfileString(G_APP_INF, PROJET, EXE_PATH, Projet.ExePath, "");

	/*	d‚termine si un pr‚c‚dent fichier source est enregistr‚ dans le .INF */
	GetProfileString(G_APP_INF, PROJET, SRC_NAME, Projet.SrcFileName, "");	
	GetProfileString(G_APP_INF, PROJET, SRC_PATHNAME, Projet.SrcPathName, "");
	GetProfileString(G_APP_INF, PROJET, SRC_EXTENSION, Projet.SrcExtension, EXT_SRC);
	GetProfileString(G_APP_INF, PROJET, SRC_PATH, Projet.SrcPath, "");

	if(!G_ResolutionChanged)
	{

		DesktopAddress[IPRG].ob_x	=	(int)GetLongProfile(G_APP_INF, BUREAU, "Exe_x", DesktopAddress[IPRG].ob_x);
		DesktopAddress[IPRG].ob_y	=	(int)GetLongProfile(G_APP_INF, BUREAU, "Exe_y", DesktopAddress[IPRG].ob_y);
	
		DesktopAddress[ITRASH].ob_x	=	(int)GetLongProfile(G_APP_INF, BUREAU, "Trash_x", DesktopAddress[ITRASH].ob_x);
		DesktopAddress[ITRASH].ob_y	=	(int)GetLongProfile(G_APP_INF, BUREAU, "Trash_y", DesktopAddress[ITRASH].ob_y);
	
		DesktopAddress[IDISK].ob_x	=	(int)GetLongProfile(G_APP_INF, BUREAU, "Disk_x", DesktopAddress[IDISK].ob_x);
		DesktopAddress[IDISK].ob_y	=	(int)GetLongProfile(G_APP_INF, BUREAU, "Disk_y", DesktopAddress[IDISK].ob_y);

		DesktopAddress[IJAG].ob_x	=	(int)GetLongProfile(G_APP_INF, BUREAU, "Jag_x", DesktopAddress[IJAG].ob_x);
		DesktopAddress[IJAG].ob_y	=	(int)GetLongProfile(G_APP_INF, BUREAU, "Jag_y", DesktopAddress[IJAG].ob_y);

		DesktopAddress[ISRC].ob_x	=	(int)GetLongProfile(G_APP_INF, BUREAU, "Src_x", DesktopAddress[ISRC].ob_x);
		DesktopAddress[ISRC].ob_y	=	(int)GetLongProfile(G_APP_INF, BUREAU, "Src_y", DesktopAddress[ISRC].ob_y);

		DesktopAddress[IEDIT].ob_x	=	(int)GetLongProfile(G_APP_INF, BUREAU, "Edit_x", DesktopAddress[IEDIT].ob_x);
		DesktopAddress[IEDIT].ob_y	=	(int)GetLongProfile(G_APP_INF, BUREAU, "Edit_y", DesktopAddress[IEDIT].ob_y);

		DesktopAddress[IASM].ob_x	=	(int)GetLongProfile(G_APP_INF, BUREAU, "Asm_x", DesktopAddress[IASM].ob_x);
		DesktopAddress[IASM].ob_y	=	(int)GetLongProfile(G_APP_INF, BUREAU, "Asm_y", DesktopAddress[IASM].ob_y);
	}

	/*	Lecture des programmes externes */
	GetProfileString(G_APP_INF, EXTERNAL, "AsmPathName", External.AsmPathName, "");
	GetProfileString(G_APP_INF, EXTERNAL, "AsmFileName", External.AsmFileName, "");
	GetProfileString(G_APP_INF, EXTERNAL, "AsmPath", External.AsmPath, "");

	GetProfileString(G_APP_INF, EXTERNAL, "EditPathName", External.EditPathName, "");
	GetProfileString(G_APP_INF, EXTERNAL, "EditFileName", External.EditFileName, "");
	GetProfileString(G_APP_INF, EXTERNAL, "EditPath", External.EditPath, "");

   	/*	re-assign global variables */
   	GetProfileString(G_APP_INF, APPLICATION, "WindowAutoActivate", ValeurChamp, "no");
   	AutoTop.AutoEnable 		= ( strcmp(ValeurChamp, "yes") == 0 || strcmp(ValeurChamp, "YES" ) == 0) ? TRUE : FALSE;

   	GetProfileString(G_APP_INF, APPLICATION, "NoMouseMovment", ValeurChamp, "no");
	AutoTop.NoMouseMovement	= ( strcmp(ValeurChamp, "yes") == 0 || strcmp(ValeurChamp, "YES" ) == 0) ? TRUE : FALSE;

   	GetProfileString(G_APP_INF, APPLICATION, "WindowWorkArea", ValeurChamp, "no");
	AutoTop.OnlyWorkArea	= ( strcmp(ValeurChamp, "yes") == 0 || strcmp(ValeurChamp, "YES" ) == 0) ? TRUE : FALSE;
	W_AutoTop(&AutoTop);
	
   	GetProfileString(G_APP_INF, APPLICATION, "VisualEffects", ValeurChamp, "no");
	Effets					= ( strcmp(ValeurChamp, "yes") == 0 || strcmp(ValeurChamp, "YES" ) == 0) ? TRUE : FALSE;			
 	W_VisualClue(Effets);
 
	/*	Smiley ouverte ? */
	GetProfileString(G_APP_INF, APPLICATION, "ActiveSmiley", ValeurChamp, "yes");
	if( strcmp(ValeurChamp, "yes") == 0 )
	{
		ActiveSmiley	=	0;															/*	Mettre smiley inactif pour l'activer ! */
		OpenSmiley();																		/*	Appeler sa m‚thode */		
	}

	/*	Status window open ? */
	GetProfileString(G_APP_INF, APPLICATION, "ActiveStatus", ValeurChamp, "yes");
	if( strcmp(ValeurChamp, "yes") == 0 )
	{
		ActiveStatus	=	0;															/*	Mettre smiley inactif pour l'activer ! */
		OpenStatus();																		/*	Appeler sa m‚thode */		
	}

	/*	Fenetre console ouverte ? */
	GetProfileString(G_APP_INF, APPLICATION, "ActiveConsole", ValeurChamp, "yes");
	if( strcmp(ValeurChamp, "yes") == 0 )
	{
		ActiveConsole	=	0;															/*	Mettre console inactive pour l'activer ! */
		OpenConsole();																	/*	Ouvre fenetre console */
	}

	/*	Toolbar ouverte ? */
	GetProfileString(G_APP_INF, APPLICATION, "ActiveToolBar", ValeurChamp, "yes");
	if( strcmp(ValeurChamp, "yes") == 0 )
	{
		ActiveToolBar	=	0;															/*	Mettre tool bar inactive pour l'activer ! */
		ToolBar();																		/*	Appeler sa m‚thode */		
	}

	/*	ParamŠtres fontes ecran */
	G_SizeFont	=	(int)GetLongProfile(G_APP_INF, FONTES, "SizeFont", 8L);
	G_ColorFont	=	(int)GetLongProfile(G_APP_INF, FONTES, "ColorFont", 4L);
	
	/*	Save on exit ? */

	GetProfileString(G_APP_INF, APPLICATION, "SaveOnExit", ValeurChamp, "yes");
	if( strcmp(ValeurChamp, "yes") == 0)
	{
		M_Checked(MAINMENU, O_SAVEQT);
		G_SaveOnExit	=	1;
	}
	else
	{
		M_UnChecked(MAINMENU, O_SAVEQT);
		G_SaveOnExit	=	0;
	}

	/*	Lecture du port de communication */
	GetProfileString(G_APP_INF, PORTCOM, "Port", ValeurChamp, "cartouche");
	if( strcmp(ValeurChamp, P_SERIAL) == 0 )
	{
		M_Checked(MAINMENU, L_SERIE);
		Port	=	PSERIAL;
	}
	else if( strcmp(ValeurChamp, P_CENTR) == 0 )
	{
		M_Checked(MAINMENU, L_CENTR);
		Port	=	PCENTRONICS;
	}
	else
	{
		form_alert(1, ERROR_NOJAGUAR);
		M_Checked(MAINMENU, L_CENTR);
		Port	=	PCENTRONICS;
	}

	/*	Bulles d'aides global a l'application */
	GetProfileString(G_APP_INF, APPLICATION, BULLE, ValeurChamp, "yes");
	if( strcmp(ValeurChamp, "yes") == 0)
	{
		M_Checked(MAINMENU, BAIDE);
		G_BulleAide	=	TRUE;
	}
	else
	{
		M_UnChecked(MAINMENU, BAIDE);
		G_BulleAide	=	FALSE;
	}

	/*	Bulles d'aides sur la fenetre JagBar */
	GetProfileString(G_APP_INF, APPLICATION, BULLEJAGBAR, ValeurChamp, "yes");
	if( strcmp(ValeurChamp, "yes") == 0)
	{
		M_Checked(MAINMENU, BONJB);
		G_BulleJagBar	=	TRUE;
	}
	else
	{
		M_UnChecked(MAINMENU, BONJB);
		G_BulleJagBar	=	FALSE;
	}
	/*	Fichier d'aides ST-Guide */
	GetProfileString(G_APP_INF, APPLICATION, HELPFILE, G_HelpFile, G_DEFAULT_HELPFILE);

	/*	Fichier d'aides ST-Guide JAGOS*/
	GetProfileString(G_APP_INF, APPLICATION, HELPOS98, G_HelpOS98, G_DEFAULT_HELPOS98);


	/*	Fichier d'aides ST-Guide JToolbox*/
	GetProfileString(G_APP_INF, APPLICATION, HELPTB98, G_HelpToolBox98, G_DEFAULT_TOOLBOX98);
	
	return	Port;
}

/* ---------------------------------------------------------------------------------------------- */
void SaveOptions(void)
/*
	Sauvegarde les options lors du clic dans l'item de menu "Save options"
*/
{
	int	L_inX, L_inY, L_inW, L_inH;

	graf_mouse(BUSYBEE, NULL);
		
	/*	sauvegarder ToolBar */	

	if( ActiveToolBar )	
	{
		SetProfileString(G_APP_INF, "Application", "ActiveToolBar", "yes");
	  	wind_get(WHToolBar, WF_CURRXYWH, &L_inX, &L_inY, &L_inW, &L_inH);
  	
		SetLongProfile(G_APP_INF, "Window_ToolBar", "position_x", (long)L_inX);
		SetLongProfile(G_APP_INF, "Window_ToolBar", "position_y", (long)L_inY);
	}
	else
		SetProfileString(G_APP_INF, "Application", "ActiveToolBar", "no");
	
	/*	Sauvegarder position et taille fenetre console si active*/
	
	if( ActiveConsole )	
	{
		SetProfileString(G_APP_INF, "Application", "ActiveConsole", "yes");
		
 	 	wind_get(WHandleConsole, WF_CURRXYWH, &L_inX, &L_inY, &L_inW, &L_inH);
		SetLongProfile(G_APP_INF, "Window_Terminal", "position_x", (long)L_inX);
		SetLongProfile(G_APP_INF, "Window_Terminal", "position_y", (long)L_inY);
		SetLongProfile(G_APP_INF, "Window_Terminal", "position_w", (long)L_inW);
		SetLongProfile(G_APP_INF, "Window_Terminal", "position_h", (long)L_inH);
	}
	else
		SetProfileString(G_APP_INF, "Application", "ActiveConsole", "no");

	/*	Smiley	*/
	if( ActiveSmiley )	
	{
		SetProfileString(G_APP_INF, APPLICATION, "ActiveSmiley", "yes");
		
 	 	wind_get(WHSmiley, WF_CURRXYWH, &L_inX, &L_inY, &L_inW, &L_inH);
		SetLongProfile(G_APP_INF, G_WIND_SMILEY, "position_x", (long)L_inX);
		SetLongProfile(G_APP_INF, G_WIND_SMILEY, "position_y", (long)L_inY);
	}
	else
		SetProfileString(G_APP_INF, "Application", "ActiveSmiley", "no");
	
	/*	Status	*/
	if( ActiveStatus )	
	{
		SetProfileString(G_APP_INF, APPLICATION, "ActiveStatus", "yes");
		
 	 	wind_get(WHStatus, WF_CURRXYWH, &L_inX, &L_inY, &L_inW, &L_inH);
		SetLongProfile(G_APP_INF, G_WIND_WSTATUS, "position_x", (long)L_inX);
		SetLongProfile(G_APP_INF, G_WIND_WSTATUS, "position_y", (long)L_inY);
	}
	else
		SetProfileString(G_APP_INF, "Application", "ActiveStatus", "no");

	/*	sauvegardes paramŠtres couleur et taille fonte */
	SetLongProfile(G_APP_INF, FONTES, "SizeFont", G_SizeFont);
	SetLongProfile(G_APP_INF, FONTES, "ColorFont", G_ColorFont);
	
	/*	Sauvegarder le fichier executable actuel */
	SetProfileString(G_APP_INF, PROJET, EXE_NAME, Projet.ExeFileName);
	SetProfileString(G_APP_INF, PROJET, EXE_PATHNAME, Projet.ExePathName);
	SetProfileString(G_APP_INF, PROJET, EXE_EXTENSION, Projet.ExeExtension);
	SetProfileString(G_APP_INF, PROJET, EXE_PATH, Projet.ExePath);

	/*	Sauvegarder le fichier source actuel */
	SetProfileString(G_APP_INF, PROJET, SRC_NAME, Projet.SrcFileName);
	SetProfileString(G_APP_INF, PROJET, SRC_PATHNAME, Projet.SrcPathName);
	SetProfileString(G_APP_INF, PROJET, SRC_EXTENSION, Projet.SrcExtension);
	SetProfileString(G_APP_INF, PROJET, SRC_PATH, Projet.SrcPath);

	/*	Sauvegarder la couleur et trame du bureau */
	
	SetLongProfile(G_APP_INF, BUREAU, "ColourTrame", DesktopAddress[0].ob_spec.index);

	SetLongProfile(G_APP_INF, BUREAU, "Exe_x", DesktopAddress[IPRG].ob_x);
	SetLongProfile(G_APP_INF, BUREAU, "Exe_y", DesktopAddress[IPRG].ob_y);

	SetLongProfile(G_APP_INF, BUREAU, "Trash_x", DesktopAddress[ITRASH].ob_x);
	SetLongProfile(G_APP_INF, BUREAU, "Trash_y", DesktopAddress[ITRASH].ob_y);

	SetLongProfile(G_APP_INF, BUREAU, "Disk_x", DesktopAddress[IDISK].ob_x);
	SetLongProfile(G_APP_INF, BUREAU, "Disk_y", DesktopAddress[IDISK].ob_y);

	SetLongProfile(G_APP_INF, BUREAU, "Jag_x", DesktopAddress[IJAG].ob_x);
	SetLongProfile(G_APP_INF, BUREAU, "Jag_y", DesktopAddress[IJAG].ob_y);

	SetLongProfile(G_APP_INF, BUREAU, "Edit_x", DesktopAddress[IEDIT].ob_x);
	SetLongProfile(G_APP_INF, BUREAU, "Edit_y", DesktopAddress[IEDIT].ob_y);

	SetLongProfile(G_APP_INF, BUREAU, "Src_x", DesktopAddress[ISRC].ob_x);
	SetLongProfile(G_APP_INF, BUREAU, "Src_y", DesktopAddress[ISRC].ob_y);
	
	SetLongProfile(G_APP_INF, BUREAU, "Asm_x", DesktopAddress[IASM].ob_x);
	SetLongProfile(G_APP_INF, BUREAU, "Asm_y", DesktopAddress[IASM].ob_y);

	/*	Sauvegarder le port d'E/S */
	
	if(GetPort() == PSERIAL)
		SetProfileString(G_APP_INF, PORTCOM, "Port", P_SERIAL);
	else if(GetPort() == PBOARD)
		SetProfileString(G_APP_INF, PORTCOM, "Port", P_BOARD);
	else /*if( GetPort() == PCENTRONICS )*/
		SetProfileString(G_APP_INF, PORTCOM, "Port", P_CENTR);
		
	/*	Sauvegarder les otpions souris sur fenetres */

	if(AutoTop.AutoEnable)
		SetProfileString(G_APP_INF, APPLICATION, "WindowAutoActivate", "yes");
	else
		SetProfileString(G_APP_INF, APPLICATION, "WindowAutoActivate", "no");	

	if(AutoTop.NoMouseMovement)
		SetProfileString(G_APP_INF, APPLICATION, "NoMouseMovment", "yes");
	else
		SetProfileString(G_APP_INF, APPLICATION, "NoMouseMovment", "no");	

	if(AutoTop.OnlyWorkArea)
		SetProfileString(G_APP_INF, APPLICATION, "WindowWorkArea", "yes");
	else
		SetProfileString(G_APP_INF, APPLICATION, "WindowWorkArea", "no");	

	if(Effets)
		SetProfileString(G_APP_INF, APPLICATION, "VisualEffects", "yes");
	else
		SetProfileString(G_APP_INF, APPLICATION, "VisualEffects", "no");	
	

	/*	Sauvegarder la resolution courante de l'ecran si elle a changee */

	if(G_ResolutionChanged)
	{
		SetLongProfile(G_APP_INF, ECRAN, "ResolutionX", VdiInfo.LargeurStation);
		SetLongProfile(G_APP_INF, ECRAN, "ResolutionY", VdiInfo.HauteurStation);
	}
	
	if( G_BulleAide )
		SetProfileString(G_APP_INF, APPLICATION, BULLE, "yes");
	else
		SetProfileString(G_APP_INF, APPLICATION, BULLE, "no");

	if( G_BulleJagBar )
		SetProfileString(G_APP_INF, APPLICATION, BULLEJAGBAR, "yes");
	else
		SetProfileString(G_APP_INF, APPLICATION, BULLEJAGBAR, "no");

	/*	Fichier d'aides ST-Guide */
	SetProfileString(G_APP_INF, APPLICATION, HELPFILE, G_HelpFile);
	SetProfileString(G_APP_INF, APPLICATION, HELPOS98, G_HelpOS98);
	SetProfileString(G_APP_INF, APPLICATION, HELPTB98, G_HelpToolBox98);

	SaveProfile();
	graf_mouse(ARROW, NULL);
}

/* ---------------------------------------------------------------------------------------------- */
void DisplayIcones( void )
{
	OBJECT			*Arbre;
	char			L_FileName[FILENAME_LEN];

	rsrc_gaddr(R_TREE, FBUREAU, &Arbre);

	/*	Si pas de fichier programme executable JAG */
	if(strcmp( Projet.ExeFileName, "" ) == 0)
		Arbre[IPRG].ob_flags |= HIDETREE;
	else
	{
		/*	V‚rifi la taille de la chaine pour l'icone */
		if( strlen(Projet.ExeFileName) > FILE_ICON_LEN)
		{
			memcpy( L_FileName, Projet.ExeFileName, FILE_ICON_LEN - 4);
			L_FileName[FILE_ICON_LEN - 3]	=	0;
			strcat( L_FileName, "...");
		}	
		else
			strcpy( L_FileName, Projet.ExeFileName );
			
		strcpy(Arbre[IPRG].ob_spec.iconblk->ib_ptext, L_FileName);	
		M_Enable(MAINMENU, F_CLOSE);
		M_Enable(MAINMENU, F_CLOSA);
		M_Enable(MAINMENU, F_INFO);
		M_Enable(MAINMENU, L_LOAD);
		M_Enable(MAINMENU, L_LDRUN);		
	}

	/* ---------------------------------------------------------------------------------------------- */	
	/*	Si pas de fichier source ASM */
	if(strcmp( Projet.SrcFileName, "" ) == 0)
		Arbre[ISRC].ob_flags |= HIDETREE;
	else
	{
		/*	V‚rifi la taille de la chaine pour l'icone */
		if( strlen(Projet.SrcFileName) > FILE_ICON_LEN)
		{
			memcpy( L_FileName, Projet.SrcFileName, FILE_ICON_LEN - 4);
			L_FileName[FILE_ICON_LEN - 3]	=	0;
			strcat( L_FileName, "...");
		}	
		else
			strcpy( L_FileName, Projet.SrcFileName );
			
		strcpy(Arbre[ISRC].ob_spec.iconblk->ib_ptext, L_FileName);	
		M_Enable(MAINMENU, F_CLOSE);
		M_Enable(MAINMENU, F_CLOSA);
	}

	/* ---------------------------------------------------------------------------------------------- */	
	/*	Si pas de programme editeur */
	if(strcmp( External.EditFileName, "" ) == 0)
		Arbre[IEDIT].ob_flags |= HIDETREE;
	else
	{
		/*	V‚rifi la taille de la chaine pour l'icone */
		if( strlen(External.EditFileName) > FILE_ICON_LEN)
		{
			memcpy( L_FileName, External.EditFileName, FILE_ICON_LEN - 4);
			L_FileName[FILE_ICON_LEN - 3]	=	0;
			strcat( L_FileName, "...");
		}	
		else
			strcpy( L_FileName, External.EditFileName );
			
		strcpy(Arbre[IEDIT].ob_spec.iconblk->ib_ptext, L_FileName);	
	}

	/* ---------------------------------------------------------------------------------------------- */	
	/*	Si pas de programme editeur */
	if(strcmp( External.AsmFileName, "" ) == 0)
		Arbre[IASM].ob_flags |= HIDETREE;
	else
	{
		/*	V‚rifi la taille de la chaine pour l'icone */
		if( strlen(External.AsmFileName) > FILE_ICON_LEN)
		{
			memcpy( L_FileName, External.AsmFileName, FILE_ICON_LEN- 4);
			L_FileName[FILE_ICON_LEN - 3]	=	0;
			strcat( L_FileName, "...");
		}	
		else
			strcpy( L_FileName, External.AsmFileName );
			
		strcpy(Arbre[IASM].ob_spec.iconblk->ib_ptext, L_FileName);	
	}

	form_dial(FMD_FINISH,0,0,0,0,	DesktopAddress[0].ob_x, DesktopAddress[0].ob_y,
									DesktopAddress[0].ob_width, DesktopAddress[0].ob_height);
}