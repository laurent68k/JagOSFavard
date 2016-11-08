/*
 * ----------------------------------------------------------------------------------------------
 *					Fenetre quitter application
 *
 * 	Author  : 	FAVARD Laurent
 *	File    : 	MainMenu.C
 *	Date    : 	10 December 1996
 *	Release : 	10 April 1999
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
#include	<JToolBox.h>

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
 *			Fichiers sp‚cifiques … inclure
 */

#include	"..\modules\messages.h"
#include	"..\modules\bulles.h"
#include	"..\modules\jaguar.h"
#include	"..\modules\Tools.h"

#include	"..\fenetres\About.h"
#include	"..\fenetres\Memparam.h"
#include	"..\fenetres\Toolsbar.h"
#include	"..\fenetres\bureau.h"
#include	"..\fenetres\sender.h"
#include	"..\fenetres\console.h"
#include	"..\fenetres\eeprom.h"
#include	"..\fenetres\options.h"
#include	"..\fenetres\Cpu680x0.h"
#include	"..\fenetres\Inquire.h"
#include	"..\fenetres\Fuji.h"
#include	"..\fenetres\InfoFile.h"
#include	"..\fenetres\NewForm.h"

/*
 * ----------------------------------------------------------------------------------------------
 *			Variables
 */

char			G_HelpFile[1024]	=	"";						/* St-Guide help file */
char			G_HelpOS98[1024]	=	"";						/* St-Guide JagOS98 help file */
char			G_HelpToolBox98[1024] =	"";						/* St-Guide JToolbox98 */

int				G_SaveOnExit;									/* Sauvegarder en quittant le bureau ? */ 

/*
 * ----------------------------------------------------------------------------------------------
 *						Gestion de la barre de menu principale.
 * ----------------------------------------------------------------------------------------------
 */

static void CloseItem( void )
{
	OBJECT				*Arbre;
	int					inObject;
	
	rsrc_gaddr(R_TREE, FBUREAU, &Arbre);
	if( Arbre[IPRG].ob_state & SELECTED )
    	inObject	=	IPRG;
    else if( Arbre[ISRC].ob_state & SELECTED )
    	inObject	=	ISRC;
    else
    	return;			

	Arbre[inObject].ob_flags |= HIDETREE;
	form_dial(FMD_FINISH,0,0,0,0,Arbre[inObject].ob_x,Arbre[inObject].ob_y,Arbre[inObject].ob_width,Arbre[inObject].ob_height*2);
						
	switch( inObject )
	{
		case	IPRG:	strcpy(Projet.ExePathName, "");
						strcpy(Projet.ExeFileName, "");
						M_Disable(MAINMENU, F_INFO);
						M_Disable(MAINMENU, L_LOAD);
						M_Disable(MAINMENU, L_LDRUN);
						break;
						
		case	ISRC:	strcpy(Projet.SrcPathName, "");
						strcpy(Projet.SrcFileName, "");
						break;
	}
							
 	if( (Arbre[IPRG].ob_flags & HIDETREE) && (Arbre[ISRC].ob_flags & HIDETREE) )
 	{
		M_Disable(MAINMENU, F_CLOSE);
		M_Disable(MAINMENU, F_CLOSA);
	}
}

static void CloseAll( void )
{
	OBJECT				*Arbre;
	
	rsrc_gaddr(R_TREE, FBUREAU, &Arbre);
	
	Arbre[IPRG].ob_flags |= HIDETREE;
	form_dial(FMD_FINISH,0,0,0,0,Arbre[IPRG].ob_x,Arbre[IPRG].ob_y,Arbre[IPRG].ob_width,Arbre[IPRG].ob_height*2);

	Arbre[ISRC].ob_flags |= HIDETREE;
	form_dial(FMD_FINISH,0,0,0,0,Arbre[ISRC].ob_x,Arbre[ISRC].ob_y,Arbre[ISRC].ob_width,Arbre[ISRC].ob_height*2);
	
	strcpy(Projet.ExePathName, "");
	strcpy(Projet.ExeFileName, "");

	strcpy(Projet.SrcPathName, "");
	strcpy(Projet.SrcFileName, "");
							
	M_Disable(MAINMENU, F_CLOSE);
	M_Disable(MAINMENU, F_CLOSA);
	M_Disable(MAINMENU, F_INFO);
	M_Disable(MAINMENU, L_LOAD);
	M_Disable(MAINMENU, L_LDRUN);
}

void Fichier(void)
/*
	Appel‚ par l'item Fermer du menu Fichier. Celui provoque la disparition entre autre, de l'icone rep‚rsentant le fichier ouvert
	comme document.
*/
{
	OBJECT				*Arbre;
	char				L_chFileName[FILENAME_LEN];
	TProjet				lProjet;
	int					IconTextLength;
	
#ifdef	__MAGIC_OS__
	IconTextLength	=	MGC_FILEICONLEN;
#else
	IconTextLength	=	TOS_FILEICONLEN;
#endif
	
	switch(M_Entry)
	{
		/*	NEW source file */
		case	F_NEW:		NewForm();
							break;

		/*	OPEN a source file */
	    case	F_OPEN:		lProjet	=	Projet;
	    					if(A_FileSelector(	lProjet.SrcPathName, lProjet.SrcFileName, lProjet.SrcExtension, 
	    										lProjet.SrcPath, G_SRC_OPEN)) 
    						{
    							Projet	=	lProjet;
								rsrc_gaddr(R_TREE, FBUREAU, &Arbre);							/* Afficher l'icone repr‚sentant le fichier */
								Arbre[ISRC].ob_flags &= ~HIDETREE;
							
								/*	V‚rifi la taille de la chaine pour l'icone */
								if( strlen(Projet.SrcFileName) > IconTextLength)
								{
									memcpy( L_chFileName, Projet.SrcFileName, IconTextLength - 4);
									L_chFileName[IconTextLength - 3]	=	0;
									strcat( L_chFileName, "...");
								}	
								else
									strcpy( L_chFileName, Projet.SrcFileName );
								
								strcpy(Arbre[ISRC].ob_spec.iconblk->ib_ptext, L_chFileName);
								form_dial(FMD_FINISH,0,0,0,0,Arbre[ISRC].ob_x,Arbre[ISRC].ob_y,Arbre[ISRC].ob_width,Arbre[ISRC].ob_height*2);
								M_Enable(MAINMENU, F_CLOSE);
								M_Enable(MAINMENU, F_CLOSA);
	    					}
		    				break;

		/*	OPEN executable	file */
	    case	F_OPENE:	lProjet	=	Projet;
	    					if(A_FileSelector(	lProjet.ExePathName, lProjet.ExeFileName, lProjet.ExeExtension, 
				    							lProjet.ExePath, G_FILE_OPEN)) 
    						{
    							Projet	=	lProjet;
								rsrc_gaddr(R_TREE,FBUREAU,&Arbre);							/* Afficher l'icone repr‚sentant le fichier */
								Arbre[IPRG].ob_flags &= ~HIDETREE;
							
								/*	V‚rifi la taille de la chaine pour l'icone */
								if( strlen(Projet.ExeFileName) > IconTextLength)
								{
									memcpy( L_chFileName, Projet.ExeFileName, 16);
									L_chFileName[17]	=	0;
									strcat( L_chFileName, "...");
								}	
								else
									strcpy( L_chFileName, Projet.ExeFileName );
								
								strcpy(Arbre[IPRG].ob_spec.iconblk->ib_ptext, L_chFileName);
								form_dial(FMD_FINISH,0,0,0,0,Arbre[IPRG].ob_x,Arbre[IPRG].ob_y,Arbre[IPRG].ob_width,Arbre[IPRG].ob_height*2);
							
								M_Enable(MAINMENU, F_CLOSE);
								M_Enable(MAINMENU, F_CLOSA);
								M_Enable(MAINMENU, F_INFO);
								M_Enable(MAINMENU, L_LOAD);
								M_Enable(MAINMENU, L_LDRUN);
	    					}
		    				break;
		    				
		/*	Informations sur l'executable */		
		case	F_INFO:		if( strcmp(Projet.ExePathName, "") != 0 && strcmp(Projet.ExeFileName, "") != 0)
								OpenInfoFile();
							break;

    	case	F_CLOSE:	CloseItem();
    						break;

    	case	F_CLOSA:	CloseAll();
    						break;

		case	F_QUIT:		if( G_ConsoleModifie )
							{
								if( form_alert(1, ATT_CONSMODIF) == 1)
								{
									W_MenuItem	=	M_2;
									MenuWind( WHandleConsole );
								}
							}
							G_Sortir = TRUE;
							break;							

    	default:			break;
	}
}

void Connect(void)
/*
	Appel‚ par les items du menu jaguar
*/
{
	int					inErreur, Ret;
	TJagId				JagIdentification;
	TYPE_SENDER_PARAM	SenderParameters;
	DTA					*L_Dta;

	switch( M_Entry )
	{
    					
    	/*	Select serial port */	
		case	L_SERIE:	SetPort( PSERIAL );
							M_Checked(MAINMENU, L_SERIE);
							M_UnChecked(MAINMENU, L_CENTR);
							break;

    	/*	Select centronics port */	
		case	L_CENTR:	SetPort( PCENTRONICS );
							M_Checked(MAINMENU, L_CENTR);
							M_UnChecked(MAINMENU, L_SERIE);
							break;

		case	L_LOAD:
		case	L_LDRUN:	if(strcmp(Projet.ExeFileName, "") != 0)
							{
								/*	Check if current file have any jaguar header with Address and lenght informations */

								inErreur	=	GetHeader(Projet.ExePathName, &SenderParameters);
								if( inErreur == JTB_ENOHEADER )
								{
									form_alert(1, GetMessage(ERROR_JAGHEADER));

									SenderParameters.StartAdr		= 	DEFAULT_TARGET_ADR;	
									SenderParameters.OffsetStart	=	DEFAULT_OFFSET;	
									SenderParameters.StartCode		=	DEFAULT_START_ADR;

									Fsfirst(Projet.ExePathName,0);
									L_Dta		=	Fgetdta();

									SenderParameters.Length		=	 L_Dta->d_length;
								}

								if( inErreur != JTB_EFILE )
								{
									SenderParameters.Mode	=	M_Entry;
									OpenSender(&SenderParameters);
								}
								else
									form_alert(1, GetMessage(ERROR_FILEOPEN));
							}
							break;

		case	L_RUN:		OpenMemParam(L_RUN);
							break;

	    case	L_PING:		if( form_alert(1, MSG_ASKINQUIRE) == 2)
	    						return;
							
							graf_mouse( BUSYBEE, NULL );
	    					Ret	=	Inquire( &JagIdentification );
							graf_mouse( ARROW, NULL );
		   					if( Ret == JTB_OK )
								OpenInquire( &JagIdentification );
							else
						       	DispErr( Ret );
 							break;

		case	L_GET:		OpenMemParam(L_GET);
							break;
		
		case	L_SECUR:	form_alert( 1, WAITCONNECT );
							break;
																
    	default:			break;
	}
}

void Outils(void)
{
	char			L_Path[PATH_LEN]						= "\0"; 						/* Chemin seulement */
	char			L_PathName[PATH_LEN+FILENAME_LEN]		= "\0"; 						/* Chemin + Nom de fichier */
	char			L_Destination[PATH_LEN+FILENAME_LEN]	= "\0";
	char			L_FileName[FILENAME_LEN]				= "\0"; 				/* Nom de fichier seulement */
	char			L_Extension[EXT_LEN]					= "*.cry\0";					/* Extension de fichiers */
	char			*Pointer;
	char			L_sTexte[PATH_LEN+FILENAME_LEN];

	switch(M_Entry)
	{
		case	T_EPROM:	OpenEEPROM();		
							break;
															
		case	T_CRY:	
							if(A_FileSelector(L_PathName, L_FileName, L_Extension, L_Path, G_CRY_OPEN)) 
    						{
								graf_mouse(BUSYBEE, NULL);
								strcpy(L_Destination, L_PathName);
								Pointer	=	strrchr(L_Destination, '.');
								if( Pointer != NULL)
								{
									*Pointer	=	0;
								}
								strcat(L_Destination, ".Cci");
								sprintf(L_sTexte, "Convert %s to CCI format\n", L_FileName);
								
								PackCCI(L_PathName, L_Destination);
								graf_mouse(ARROW, NULL);	
								SpeechSmiley( BUBBLE30 );
							}
							else
								SpeechSmiley( BUBBLE31 );
							
							break;

    	default:			break;
	}
}

void Aide(void)
/*
	Appel‚ par l'item Fermer du menu Fichier. Celui provoque la disparition entre autre, de l'icone rep‚rsentant le fichier ouvert
	comme document.
*/
{
	switch(M_Entry)
	{
		/*	Aide ST-Guide JagStudio98 */
    	case	AIDE:		STG_Open(G_HelpFile, NULL);
    						break;

		/*	Aide ST-Guide JagOS98 */
    	case	AJAGOS:		STG_Open(G_HelpOS98, NULL);
    						break;

		/*	Aide ST-Guide JagOS98 */
    	case	AJAGTB:		STG_Open(G_HelpToolBox98, NULL);
    						break;

		/*	Aide ST-Guide jaguar kit */
    	case	MENUAIDE:	STG_Open(G_HelpFile, SECTION_HELP_MENUBAR);
    						break;

		/*	Bulle d'aide pour l'aplication: */
    	case	BAIDE:		if( !G_BulleAide )
							{
								G_BulleAide	=	TRUE;
	    						M_Checked(MAINMENU, BAIDE);
							}
							else
							{
								G_BulleAide	=	FALSE;
								M_UnChecked(MAINMENU, BAIDE);
							}
							break;

		/*	Bulle d'aide pour la fenetre JagBar: */
    	case	BONJB:		if( !G_BulleJagBar )
							{
								G_BulleJagBar	=	TRUE;
	    						M_Checked(MAINMENU, BONJB);
							}
							else
							{
								G_BulleJagBar	=	FALSE;
								M_UnChecked(MAINMENU, BONJB);
							}
							break;
							
    	default:			break;
	}
}

void SaveQuit(void)
/*
	S‚lectionne ou d‚selectionne la sauvegarde automatique du bureau … chaque fin du programme.
	Un checkmark est pla‡‚ dans le menu pour rappeler l'option coch‚e.
*/
{
	if(G_SaveOnExit)															/* EnlŠve ou place un checkmark */
	{
    	M_UnChecked(MAINMENU, O_SAVEQT);
    	SetProfileString(G_APP_INF, "Application", "SaveOnExit", "no");
    }
	else
	{
		M_Checked(MAINMENU, O_SAVEQT);
    	SetProfileString(G_APP_INF, "Application", "SaveOnExit", "yes");
	}
	G_SaveOnExit = 0x0001 & (~G_SaveOnExit);									/* Compl‚mente SaveOnExit */
}
