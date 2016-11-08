/*
 * ----------------------------------------------------------------------------------------------
 *
 *
 *	Author  : 	FAVARD Laurent
 *	File    : 	EEprom.C
 *	Date    : 	12 June 1998
 *	Release : 	27 November 1998
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
#include	"..\modules\const.h"
#include	"..\modules\jaguar.h"
#include	"..\modules\mainmenu.h"
#include	"..\modules\tools.h"

#include	"..\fenetres\console.h"

/*
 * ----------------------------------------------------------------------------------------------
 *		Description de la fenetre "Quitter l'application"
 * ----------------------------------------------------------------------------------------------
 */

void	Action(void);

static Type_Form_List
FormAction[]=	{	
					{BN_37,	Action,	NULL},				/* Bouton CLOSE */
					{BN_33,	Action,	NULL},				/* Bouton RETRIEVE */
					{BN_34,	Action,	NULL},				/* Bouton WRITE */
					{BN_35,	Action,	NULL},				/* Bouton READ */
					{BN_36,	Action,	NULL},				/* Bouton WRITE */
					{HELP5,	Action,	NULL},							/* Bouton HELP */
					FORM_END
				};

/*
 * ----------------------------------------------------------------------------------------------
 *			Variables
 */

static	int		WHEprom;

/*
 * ----------------------------------------------------------------------------------------------
 *			Fenetre 
 * ----------------------------------------------------------------------------------------------
 */

void OpenEEPROM(void)
/*
	Ouvre la fenetre d'acces a l'EEPROM
	
*/
{
	Type_Formular_Parameter	*Parameter;
  
	Parameter = F_GetParameter();
	if(Parameter != NULL)
	{
		Parameter->Title	=	G_TITLE_EEPROM;											/* Titre de la fenetre */
		Parameter->TreeName =	EEPROM;															/* Indice de l'arbre de l'objet */
		Parameter->Mode		=	F_WIND;																/* Opening mode */
		Parameter->FirstEdit=	EL1;																/* First Editable Text */
		Parameter->List		=	FormAction;															/* Event list */
		Parameter->FormIcon =	IAPPLI;																		/* Icone d'iconification */
		Parameter->Type		=	MODELESS;	
		Parameter->Mouse 	=	ARROW;			
		if(!G_ResolutionChanged)
		{
			Parameter->x	=	(int)GetLongProfile(G_APP_INF, G_WIND_EEPROM, "position_x", 0);
			Parameter->y	=	(int)GetLongProfile(G_APP_INF, G_WIND_EEPROM, "position_y", 0);
		}

		if((WHEprom = F_XOpen(Parameter)) != FALSE)
		{
		}
		free(Parameter);
	}
}

static void GetHexa( char *Buffer )
{
	int				ObjectLine[16] = {EL1, EL2, EL3, EL4, EL5, EL6, EL7, EL8,
									 EL9, EL10, EL11, EL12, EL13, EL14, EL15, EL16};
	int				Indice, Ligne;
	unsigned char	*Pointeur;
	char			chTexte[30];
	char			chValue[3];
	int				inValue;
	
	Pointeur	=	(unsigned char *)Buffer;
	for(Ligne = 0; Ligne < 16; Ligne++)
	{
		F_ReadText( EEPROM, ObjectLine[ Ligne ], chTexte);
		for(Indice = 0; Indice <= 14; Indice+=2)
		{
			memcpy( chValue, chTexte + Indice, 2);
			sscanf( chValue, "%X", &inValue);
			*Pointeur	=	(unsigned char)inValue;
			Pointeur++;
		}
	}
}

static void Display( char *Buffer )
{
	int				ObjectLine[16] = {EL1, EL2, EL3, EL4, EL5, EL6, EL7, EL8,
									 EL9, EL10, EL11, EL12, EL13, EL14, EL15, EL16};
	int				ObjectLine2[16] = {AL1, AL2, AL3, AL4, AL5, AL6, AL7, AL8,
									  AL9, AL10, AL11, AL12, AL13, AL14, AL15, AL16};
	int				Indice, Ligne;
	unsigned char	*Pointeur;
	char			chTexte[30], chValue[5];
	char			chAscii[9];
	
	Pointeur	=	(unsigned char *)Buffer;
	for(Ligne = 0; Ligne < 16; Ligne++)
	{
		chTexte[0]	=	0;
		chAscii[0]	=	0;
		for(Indice = 0; Indice < 8; Indice++)
		{
			if( *Pointeur > 9 )
				sprintf( chValue, "%X", *Pointeur);
			else
				sprintf( chValue, "0%u", *Pointeur);
				
			strcat( chTexte, chValue);
			
			/*	Affichage ASCII */
			sprintf( chValue, "%c", *Pointeur );
			strcat( chAscii, chValue );

			Pointeur++;
		}
		F_WriteText( EEPROM, ObjectLine[ Ligne ], chTexte);
		F_WriteText( EEPROM, ObjectLine2[ Ligne ], chAscii);
	}
}

static void Fill0( char *Buffer)
{
	unsigned char	*Pointeur;
	int				Indice;
	
	Pointeur	=	(unsigned char *)Buffer;
	for(Indice = 0; Indice < SIZE_SERIAL_EEPROM; Indice++)
	{
		*Pointeur	=	0;
		Pointeur++;
	}
}

static void Action(void)
/*
	Suivant le bouton cliqu‚ dans la fenetre de fin d'application, assigne la variable 'Sortir'.
*/
{
	int				L_inX, L_inY, dummy;
	char			L_chEEPROM[SIZE_SERIAL_EEPROM+1];
	int				inFichier, Ret;
	char			L_Path[PATH_LEN]		= "\0"; 						/* Chemin seulement */
	char			L_PathName[PATH_LEN+FILENAME_LEN]	= "\0"; 						/* Chemin + Nom de fichier */
	char			L_FileName[FILENAME_LEN]		= "eeprom.bin\0"; 						/* Nom de fichier seulement */
	char			L_Extension[EXT_LEN]		= "*.bin\0";					/* Extension de fichiers */
	
	L_chEEPROM[SIZE_SERIAL_EEPROM]	=	0;
	
	switch( F_NameObject )
	{
		/*	Bouton RETRIEVE */
		case	BN_33:	graf_mouse(BUSYBEE, NULL);
		
                        Ret = GetEEPROM( L_chEEPROM );
                        DispErr( Ret );
						Display( L_chEEPROM );
						
						graf_mouse(ARROW, NULL);
						break;

		/*	Bouton WRITE */
		case	BN_34:	graf_mouse(BUSYBEE, NULL);
		
						GetHexa( L_chEEPROM );
						Ret = SetEEPROM( L_chEEPROM );
						DispErr( Ret );
						Display( L_chEEPROM );
						
						graf_mouse(ARROW, NULL);
						break;
						
		/*	Bouton READ */
		case	BN_35:	if(A_FileSelector(L_PathName, L_FileName, L_Extension, L_Path, G_LOAD_BIN)) 
						{
							inFichier	=	open( L_FileName, O_RDONLY );
							if( inFichier > 0 )
							{
								graf_mouse(BUSYBEE, NULL);
								Fill0( L_chEEPROM);
								read( inFichier, L_chEEPROM, SIZE_SERIAL_EEPROM );
								Display( L_chEEPROM );
								close( inFichier );								
							}
							else
								form_alert(1, ERROR_FILEOPEN );
						}
						graf_mouse(ARROW, NULL);
						break;
						
		/*	Bouton SAVE */
		case	BN_36:	if(A_FileSelector(L_PathName, L_FileName, L_Extension, L_Path, G_SAVE_BIN)) 
						{						
							inFichier	=	creat( L_FileName );
							if( inFichier > 0 )
							{
								graf_mouse(BUSYBEE, NULL);
								GetHexa( L_chEEPROM );
								write( inFichier, L_chEEPROM, SIZE_SERIAL_EEPROM );
								close( inFichier );
								graf_mouse(ARROW, NULL);
							}
							else
								form_alert(1, ERROR_FILECREATE );
						}
						graf_mouse(ARROW, NULL);
						break;
						
		/*	Button CLOSE	*/
		case	BN_37: 	wind_get(WHEprom, WF_CURRXYWH, &L_inX, &L_inY, &dummy, &dummy);
  	
						SetLongProfile(G_APP_INF, G_WIND_EEPROM, "position_x", (long)L_inX);
						SetLongProfile(G_APP_INF, G_WIND_EEPROM, "position_y", (long)L_inY);
			
						graf_mouse(ARROW, NULL);
						F_Close(EEPROM);
						break;
		
		/*	Button HELP	*/
		case	HELP5:	STG_Open(G_HelpFile, SECTION_HELP_EEPROM);
						break;
		
						
		default:		break;
	}						
}

