/*
 * ----------------------------------------------------------------------------------------------
 *					APPLICATION Jaguar Studio
 *
 * 	Author  : 	FAVARD Laurent
 *	File    : 	Jaquar.C
 *	Date    : 	10 December 1996
 *	Release : 	17 May 2000
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
#include	<JToolbox.h>												/* ToolBox JAGUAR */

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
 *			Fichiers sp‚cifiques … inclure
 */

#include	"..\modules\messages.h"										/*	Messages textes */
#include	"..\modules\Tools.h"										/*	Tools !! */
#include	"..\modules\mainmenu.h"
#include	"..\modules\Object.h"										/* Associations codes <-> liste AADS */
#include	"..\modules\AppOpen.h"
#include	"..\modules\Threads.h"
#include	"..\modules\sysmsg.h"

/*
 * ----------------------------------------------------------------------------------------------
 *				Includes des fenetres
 * ----------------------------------------------------------------------------------------------
 */

#include	"..\fenetres\console.h"										/*	Gestion de la fenetre terminal Jaguar */
#include	"..\fenetres\About.h"
#include	"..\fenetres\Options.h"
#include	"..\fenetres\Toolsbar.h"
#include	"..\fenetres\bureau.h"
#include	"..\fenetres\sender.h"
#include	"..\fenetres\Transfer.h"
#include	"..\fenetres\Cpu680x0.h"
#include	"..\fenetres\Fuji.h"
#include	"..\fenetres\Status.h"

/*
 * ----------------------------------------------------------------------------------------------
 *				Variables globales standards de l'application
 * ----------------------------------------------------------------------------------------------
 */

/*	m‚morisation pour le fichier ouvert */
TProjet				Projet;
TExternal			External;
					
int					G_ResolutionChanged = FALSE;						/* Resolution changee depuis derniere fois */
int					G_Sortir = FALSE;
int					G_BulleAide = FALSE;								/* Activation ou pas des bulles d'aides */

int					G_SizeFont	=	8;
int					G_ColorFont	=	4;

int					G_AVServerID = 0;

static int			PreviousState;
/*
 * ----------------------------------------------------------------------------------------------
 *		Timer GEM:	handle jaguar command received
 * ----------------------------------------------------------------------------------------------
 */

void HandlerMessage(int FrameCode, long SourcePID, long TargetPID, int LenghtData, TRecipient *Recipient)
{
	char					chTexte[100];
	char					*chString;
	TJagMemDesc				*JagMem;
	TException				*Exception;
	TYPE_TRANSFERT_PARAM 	ParamTransfert;
	int				Fichier;
	char			Path[PATH_LEN]					= "\0"; 						/* Chemin seulement */
	char			PathName[PATH_LEN+FILENAME_LEN]	= "\0"; 						/* Chemin + Nom de fichier */
	char			FileName[FILENAME_LEN]			= "user1.bin\0"; 				/* Nom de fichier seulement */
	char			Extension[EXT_LEN]				= "*.bin\0";					/* Extension de fichiers */
	
	switch( FrameCode )
    {
    	/*	User 1 Frame code */
    	case	FRAME_USER1:		sprintf(chTexte, "USER1- SPID 0x%lX to TPID 0x%lX:", SourcePID, TargetPID);
									AddString(chTexte);
									sprintf(chTexte, "%d bytes as datas", LenghtData);		
									AddString(chTexte);
									if( form_alert(1, MSG_SAVE_DATAS ) == 1 )
									{
										if(A_FileSelector(PathName, FileName, Extension, Path, G_SAVE_BIN)) 
										{						
											Fichier	=	creat( FileName );
											if( Fichier > 0 )
											{
												graf_mouse(BUSYBEE, NULL);
												write( Fichier, (void *)Recipient, LenghtData );
												close( Fichier );
												graf_mouse(ARROW, NULL);
											}
											else
												form_alert(1, ERROR_FILECREATE );
										}
									}
    								break;
    	
    	/*	Exception 680x0 JagOS */
    	case	FRAME_EXCEPTION:	Exception	=	(TException *)Recipient;
									OpenCPU( Exception );
    								break;
    	/*	Debug message */
		case	FRAME_DBMSG:		chString =	(char *)Recipient;
									/*sprintf(chTexte, "MSG- SPID 0x%lX to TPID 0x%lX:", SourcePID, TargetPID);
									AddString(chTexte);*/
									sprintf(chTexte, "%s", chString);
									AddString(chTexte);
									break;

		case	FRAME_MEMSEND: 		JagMem	=	(TJagMemDesc *)Recipient;
									ParamTransfert.Mode			=	FRAME_MEMSEND;
									ParamTransfert.ExpectedLen	=	JagMem->Lenght;
									ParamTransfert.StartAdr		=	JagMem->StartAddress;
									ParamTransfert.SourcePID	=	SourcePID;
									ParamTransfert.TargetPID	=	TargetPID;
									OpenTransfert( &ParamTransfert );								
	              					break;

		case	FRAME_MEMGET:		JagMem	=	(TJagMemDesc *)Recipient;
									ParamTransfert.Mode			=	FRAME_MEMGET;
									ParamTransfert.ExpectedLen	=	JagMem->Lenght;
									ParamTransfert.StartAdr		=	JagMem->StartAddress;
									ParamTransfert.SourcePID	=	SourcePID;
									ParamTransfert.TargetPID	=	TargetPID;
									OpenTransfert( &ParamTransfert );
									break;

	}
}

/* ---------------------------------------------------------------------------------------------- */
static void X_Message( void )
/*
	Impl‚mentation du message VA_START.
*/
{
	int		Reponse[8];
	char	**CmdLine;
		
	switch( A_Message[0] )
	{
		case	0:		break;
		
		case	VA_START:
						G_AVServerID	=	A_Message[1];
						CmdLine	=	(void *)(&A_Message[3]);
						ReOpenFile( *CmdLine );
						
						/*	Envoyer la reponse d'acquitement au serveur AV */
						Reponse[0]	=	AV_STARTED;
						Reponse[1]	=	AesHandle;

						Reponse[3]	=	A_Message[3];
						Reponse[4]	=	A_Message[4];

						appl_write( G_AVServerID , 16, Reponse );
						A_Message[0]	=	0;
						break;

		case	VA_PROGSTART:
						if( A_Message[3] == 0 )
							form_alert(1, "[3][ Unable to start the program ][Ok]" );
						A_Message[0]	=	0;
						break;
								
		default:		A_Message[0]	=	0;
						break;			
	}				
}

/* ---------------------------------------------------------------------------------------------- */
static void AppClose(void)
/*
	Proc‚dure appel‚e lorsque la terminaison irreversible de l'application est en cours.
	dans ca cas, on appel la procedure de sauvegarde des options si sauevagrde automatique activ‚e,
	et on ferme dans ce cas le fichier INF.
*/
{
	if( !G_SaveOnExit )
		return;

	SaveOptions();
	CloseProfile();
}
/* ---------------------------------------------------------------------------------------------- */
static void CloseAll( void )
{
	F_Close(ABOUTME);											
	F_Close(MEMPARAM);
	F_Close(FOPTIONS);
	F_Close(TOOLBOX);
	F_Close(SENDER);
	F_Close(TRANSFER);
	F_Close(INQUIRE);
	F_Close(SMILEY);
	F_Close(INQUIRE);
	F_Close(INFOFILE);
	F_Close(EEPROM);
	F_Close(NEWFORM);
	F_Close(GENFORM);
	F_Close(DEBUG);
	F_Close(WERR);
	F_Close( WSTATUS );
}
/* ---------------------------------------------------------------------------------------------- */
static void JaguarBusy( int Status )
/*	Display a visual effect to show that Jaguar is trying to send something
*/
{
	OBJECT	*Arbre;
	
	if( ActiveStatus )					/*	if window is open	*/
	{
		if( PreviousState != Status )
		{
			PreviousState	=	Status;
			rsrc_gaddr(R_TREE, WSTATUS, &Arbre);
			if( Status )
				Arbre[ ISTATUS].ob_state	|=	SELECTED;
			else
				Arbre[ ISTATUS].ob_state	&=	~SELECTED;
			W_Redraw( WHStatus );
/*			F_Selected( WSTATUS, ISTATUS, Status );*/
		}
	}
}
/* ---------------------------------------------------------------------------------------------- */
int main( void )
{
	OBJECT			*Arbre;
	long			Colour, Ligne;
#ifndef	__TOS_OS__
	Type_Cookie		Cookie;
#endif
	int				Ret, Port, dummy;
	
	MS_Mouse(MS_SLEEP, NULL);
#ifndef	__TOS_OS__
	Pdomain( 1 );
#endif
	/*	V‚rifier si le c'est le bon systeme d'exploitation */
	
#ifdef	__MAGIC_OS__
	if(!J_Exist())											/* If doesn't exist a Cookie jar */
	{
		form_alert(1, GetMessage(ERROR_COOKIE) );
		return	-1;
	}
	
	if(!J_Search(ID_MAGIC, &Cookie))						/* Check cookie for Magic5 */
	{
		form_alert(1, GetMessage(ERROR_MAGIC) );
		return	-1;
	}
#endif

#ifdef	__NAES_OS__
	if(!J_Exist())											/* If doesn't exist a Cookie jar */
	{
		form_alert(1, GetMessage(ERROR_COOKIE) );
		return	-1;
	}
	
	if(!J_Search(ID_NAES, &Cookie))							/* Check for cookie for N.AES */
	{
		form_alert(1, GetMessage(ERROR_NAES) );
		return	-1;
	}
#endif

	
	if(!A_Open(G_FILE_RESSOURCE))
	{
		form_alert(1, GetMessage(ERROR_RSC));
		A_Close();
		return(G_RESSOURCE_ERROR);						
	}

	if(VdiInfo.HauteurStation < G_HEIGHT_SCREEN || VdiInfo.LargeurStation < G_WIDTH_SCREEN)		
	{
    	form_alert(1,GetMessage(ERROR_SCREEN));
	} 

	W_Init();														/* Init Window  */
	M_Init();														/* Init Menu    */
	F_Init();														/* Init Form    */
	
#ifdef	__MAGIC_OS__
	HideOthers();
#endif
	
	W_SetIconApp(IAPPLI);											/* Set Icon Form Appli. */
	M_Open(MAINMENU,BarreMenu);										/* Ouvre le menu de l'application */
	
	/*
     *	Centrer l'image de fond et restaurer la couleur a partir .INF
     */
	rsrc_gaddr(R_TREE, FBUREAU, &Arbre);
	Arbre[BANNER].ob_x		=	(VdiInfo.LargeurStation / 2) - ( Arbre[BANNER].ob_width / 2 );
	Arbre[BANNER].ob_y		=	(VdiInfo.HauteurStation / 2) - ( Arbre[BANNER].ob_height / 2 );
	
	Colour = GetLongProfile(G_APP_INF, "Desktop", "ColourTrame", Arbre[0].ob_spec.index);
	Arbre[0].ob_spec.index	=	Colour;
	
	D_Open( FBUREAU, Bureau);																	/* Ouvrir le bureau de l'application */
	
	F_PopAttach( FBUREAU, ListPopAttach );				/* Attach Pop-ups */
	F_PopEvent( ListPopEvent );					/* Install Pop-events */
	F_SetPopup( 8 );	

	Port	=	AppOpen();
	DisplayIcones();
	A_SetMouse(ARROW, NULL);										/* Application's mouse */

	/*	fontes */
	vst_point( VdiHandle, G_SizeFont, &dummy, &dummy, &dummy, &dummy );
	vst_color( VdiHandle, G_ColorFont );
	
	/*	Ouverture session JToolbox (Cr‚ation S‚maphore et mode Super) */
#ifdef	__TOS_OS__	
	Ret	=	JTB_Open( FALSE );
#else
	Ret	=	JTB_Open( TRUE );
#endif
	if( Ret <= 0 )
	{
		form_alert( 1, "[0][ Session Open JTB error ][Ok]");
	}
	SetPort( Port );												/*	active the selected port */

	/*	Now display the message to startup the Jaguar */
	form_alert( 1, WAITCONNECT );
	
	/*	Event loop */  
	G_Sortir = FALSE;
	do
	{
		A_WaitEvent();		
		W_Event();
		F_Event();
		M_Event();

		X_Message();												/*	eXtended Messages */
		
		JaguarBusy( ! JTB_DevStatus() );
			
		/*	procedure to handle jaguar messages */
		HookJaguarMessage( HandlerMessage );
	}
  	while(!G_Sortir);

	JTB_Exit();														/*	return USER mode */
	AppClose();

	W_Close(WHandleConsole);										/* Ferme la fenetre terminal */
	W_MenuClose(WHandleConsole);

	for(Ligne = 0; Ligne < G_NbreLigneConsole; Ligne++)
	{
		free(G_TexteConsole[Ligne]);
	}
	  
	CloseAll();
    
	D_Close();
	M_Close(MAINMENU);  					
	A_Close();

	return(G_NO_ERROR);												/* Rien de particulier */
}

