/*
 * ----------------------------------------------------------------------------------------------
 *			Module de gestion des s‚lecteurs de fichiers et d'appel un PRG externe
 *
 * 	Author  : 	FAVARD Laurent
 *	File    : 	TOOLS.C
 *	Date    : 	15 Decembre 1996
 *	Release : 	30 September 1998
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

#include <tos.h>
#include <stdio.h>
#include <string.h>

#include 	<PcAads.h>
#include 	<JToolBox.h>

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
 */

#include 	"..\modules\Tools.h"
#include 	"..\modules\Messages.h"
#include 	"..\modules\Const.h"
#include 	"..\modules\Jaguar.h"

#include 	"..\fenetres\About.h"
#include 	"..\fenetres\Console.h"
#include 	"..\fenetres\Options.h"
#include 	"..\fenetres\Toolsbar.h"
#include 	"..\fenetres\Fuji.h"
#include	"..\fenetres\bureau.h"
#include	"..\fenetres\Werr.h"

/* ---------------------------------------------------------------------------------------------- */
void DispErr( int Code )
{
	char	Message[ JTB_LENMSG ];
	
	if( Code <= 0 )
	{
		JTB_ErrorText( Code, Message );
		WError( Code, Message );
	}
}					
/* ---------------------------------------------------------------------------------------------- */
static void RefreshWindow(void)
{
	if(ActiveSmiley)
		W_Redraw(WHSmiley);

	if(ActiveToolBar)
		W_Redraw(WHToolBar);

	if(ActiveConsole)
		W_Redraw(WHandleConsole);

	if(WHOptions)
		W_Redraw(WHOptions);

	if(WHInfo)
		W_Redraw(WHInfo);
}
/* ---------------------------------------------------------------------------------------------- */
signed int GetHeader(char *PathName, TYPE_SENDER_PARAM	*SenderParameters)
/*
	This procedure will try to search somes header into the file, such JAGR, JAGL, XJAG.
	If a header is found, 	SenderParameters will contains the essentials informations for the upload
	else return NO HEADER found.
*/
{
	signed int		inRetour;
	TFileJag 		FileJaguarInfo;
	
	inRetour	=	IsJAGR( PathName, &FileJaguarInfo );	
	if( inRetour > 0 ) 
	{
		SenderParameters->Header		=	FileJaguarInfo.Header;
		SenderParameters->CodeOp		=	FileJaguarInfo.CodeOp;
		SenderParameters->StartAdr		=	FileJaguarInfo.StartAdr;
		SenderParameters->Length		=	FileJaguarInfo.Length;
		SenderParameters->StartCode		=	FileJaguarInfo.StartCode;
		SenderParameters->OffsetStart	=	FileJaguarInfo.OffsetStart;
		return	TRUE;
	}
	
	inRetour	=	IsJAGL( PathName, &FileJaguarInfo );
	if( inRetour > 0 )
	{
		SenderParameters->Header		=	FileJaguarInfo.Header;
		SenderParameters->CodeOp		=	FileJaguarInfo.CodeOp;
		SenderParameters->StartAdr		=	FileJaguarInfo.StartAdr;
		SenderParameters->Length		=	FileJaguarInfo.Length;
		SenderParameters->StartCode		=	FileJaguarInfo.StartCode;
		SenderParameters->OffsetStart	=	FileJaguarInfo.OffsetStart;
		return	TRUE;
	}

	return	inRetour;	
		
}
/* ---------------------------------------------------------------------------------------------- */
#define		VA_START 		0x4711
#define		AV_EXIT 		0x4736
#define		AV_STARTED 		0x4738

void ReOpenFile( char *CmdLine )
/*
	Impl‚mentation du message VA_START.
*/
{
	char	L_FileName[FILENAME_LEN];
	char	*chIndice;
	OBJECT	*Arbre;
	
	graf_mouse(BUSYBEE, NULL);
	rsrc_gaddr(R_TREE, FBUREAU, &Arbre);

	/*	Get command line */
	if( CmdLine != NULL )
	{
		chIndice	=	strrchr( CmdLine, '\\');
		if( chIndice != NULL )
		{
			*chIndice	=	0;
			strcpy(Projet.ExePath, CmdLine);
			*chIndice	=	'\\';
		}
			
		strcpy(Projet.ExePathName, CmdLine);

		chIndice	=	strrchr( CmdLine, '\\');
		if( chIndice != NULL )
			strcpy(Projet.ExeFileName, chIndice+1);
		else
			strcpy(Projet.ExeFileName, "????");

		chIndice	=	strrchr( CmdLine, '.');
		if( chIndice != NULL )	
		{
			strcpy(Projet.ExeExtension, "*");			
			strcat(Projet.ExeExtension, chIndice);
		}
		else
			strcpy(Projet.ExeExtension, "*.JAG");
	
		if( strlen(Projet.ExeFileName) > FILE_ICON_LEN)
		{
			memcpy( L_FileName, Projet.ExeFileName, 16);
			L_FileName[17]	=	0;
			strcat( L_FileName, "...");
		}	
		else
			strcpy( L_FileName, Projet.ExeFileName );

		strcpy(Arbre[IPRG].ob_spec.iconblk->ib_ptext, L_FileName);	
		M_Enable(MAINMENU, F_CLOSE);
		M_Enable(MAINMENU, F_INFO);
		M_Enable(MAINMENU, L_LOAD);
		M_Enable(MAINMENU, L_LDRUN);

		Arbre[IPRG].ob_flags &= ~HIDETREE;
		form_dial(FMD_FINISH,0,0,0,0, DesktopAddress[0].ob_x, DesktopAddress[0].ob_y, DesktopAddress[0].ob_width, DesktopAddress[0].ob_height);
	}
}

#define	SCRENMGR			1
#define	SM_M_SPECIAL		101

#define SMC_TIDY_UP		0							/* MagiC 2	*/
#define SMC_TERMINATE	1							/* MagiC 2	*/
#define SMC_SWITCH		2							/* MagiC 2	*/
#define SMC_FREEZE		3							/* MagiC 2	*/
#define SMC_UNFREEZE	4							/* MagiC 2	*/
#define SMC_RES5		5							/* MagiC 2	*/
#define SMC_UNHIDEALL	6							/* MagiC 3.1	*/
#define SMC_HIDEOTHERS	7							/* MagiC 3.1	*/
#define SMC_HIDEACT		8							/* MagiC 3.1	*/

void HideOthers( void )
{
	int		Message[8];
	
	Message[0]	=	SM_M_SPECIAL;
	Message[1]	=	AesHandle;
	Message[2]	=	0;		/* keine šberl„nge */
	Message[3]	=	0;
	Message[4]	=	'MA';
	Message[5]	=	'GX';
	Message[6]	=	SMC_HIDEACT;
	Message[7]	=	0;
			
	appl_write( SCRENMGR , 16, Message);
	
	
}
