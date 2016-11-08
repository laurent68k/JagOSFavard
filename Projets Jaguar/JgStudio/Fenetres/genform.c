/*
 * ----------------------------------------------------------------------------------------------
 *								Fenetres Barre d'outils.
 *
 * 	Author  : 	FAVARD Laurent
 *	File    : 	GENFORM.C
 *	Date    : 	19 December 1998
 *	Release : 	23 December 1998
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
#include	"..\Tos.rsc\jaguar_F.H"
#endif
	
/*
 * ----------------------------------------------------------------------------------------------
 *			Fichiers sp‚cifiques … inclure
 */

#include	"..\modules\messages.h"
#include	"..\modules\Bulles.h"
#include	"..\modules\jaguar.h"
#include	"..\modules\const.h"
#include	"..\modules\codegen.h"
#include	"..\modules\mainmenu.h"

#include	"..\fenetres\newform.h"
#include	"..\fenetres\genfrmp.h"

/*
 * ----------------------------------------------------------------------------------------------
 *		Description de la Barre d'outils de l'application
 * ----------------------------------------------------------------------------------------------
 */

void Action(void);
void Close(void);
void MouseEnter(void);

static	Type_Form_List
ActionList[]=	{
					{ROOT,	Close,	NULL, NULL, NULL, NULL, NULL},							/*	Root object Close window */
					{BN_40,	Action,	NULL, NULL, NULL, MouseEnter, NULL},							/*	*/
					{BN_41,	Action,	NULL, NULL, NULL, NULL, NULL},							/*	*/
					{BN_42,	Action,	NULL, NULL, NULL, MouseEnter, NULL},							/*	*/
					{BN_43,	Action,	NULL, NULL, NULL, MouseEnter, NULL},							/*	*/
					{BN_44,	Action,	NULL, NULL, NULL, MouseEnter, NULL},							/*	*/
					{BN_45,	Action,	NULL, NULL, NULL, MouseEnter, NULL},							/*	*/
					{EICON,		NULL,	Action, NULL, NULL, MouseEnter, NULL},							/*	*/
					{ECURSOR,	NULL,	Action, NULL, NULL, MouseEnter, NULL},							/*	*/
					{CB1,	Action,	NULL, NULL, NULL, MouseEnter, NULL},							/*	*/
					{CB2,	Action,	NULL, NULL, NULL, MouseEnter, NULL},							/*	*/
					{CB3,	Action,	NULL, NULL, NULL, MouseEnter, NULL},							/*	*/
					{CB4,	Action,	NULL, NULL, NULL, MouseEnter, NULL},							/*	*/
					{HELP7,	Action,	NULL, NULL, NULL, NULL, NULL},							/*	*/
					{BN_47,	Action,	NULL, NULL, NULL, MouseEnter, NULL},							/*	*/

					FORM_END
				};

static TSource 	Definition;

/*
 * ----------------------------------------------------------------------------------------------
 *			Variables
 */

static int					WHGenForm;

/*
 * ----------------------------------------------------------------------------------------------
 *			Ouverture et fermeture de la toolbar.
 * ----------------------------------------------------------------------------------------------
 */

void GenWindow( int TypeGeneration)
/*
	Ouvre la fenetre Barre d'outils de l'application.
*/
{
	Type_Formular_Parameter		*Parameter;

	
	Parameter = F_GetParameter();
	if(Parameter != NULL)
	{
		Definition.Type	=	TypeGeneration;
		switch( TypeGeneration )
		{
			case	COOPERANT:	Parameter->Title	=	G_TITLE_GENFORM1;							/* Titre de la fenetre */
								break;
			case	SOURCEASM:	Parameter->Title	=	G_TITLE_GENFORM2;							/* Titre de la fenetre */
								break;
			case	FREEWITHOS:	Parameter->Title	=	G_TITLE_GENFORM3;							/* Titre de la fenetre */
								break;
			case	FREEPROGRAM:Parameter->Title	=	G_TITLE_GENFORM4;							/* Titre de la fenetre */
								break;
		}
		Parameter->TreeName =	GENFORM;								/* Indice de l'arbre de l'objet */
		Parameter->Mode		=	F_WIND;									/* Opening mode */
		Parameter->FirstEdit=	ENAME;									/* First Editable Text */
		Parameter->List		=	ActionList;								/* Event list */
		Parameter->FormIcon =	IAPPLI;									/* Icone d'iconification */
		Parameter->Mouse	=	MS_POINTFLATHAND;

		if(!G_ResolutionChanged)
		{
			Parameter->x	=	(int)GetLongProfile(G_APP_INF, G_WIND_GENFORM, "position_x", 0);
			Parameter->y	=	(int)GetLongProfile(G_APP_INF, G_WIND_GENFORM, "position_y", 0);
		}
		
		if((WHGenForm = F_XOpen(Parameter)) != FALSE)
		{
			F_WriteText( GENFORM, ETITLE, "");
			F_WriteText( GENFORM, EDATE, "");
			F_WriteText( GENFORM, EBNESS, "");
			F_WriteText( GENFORM, EAUTHOR, "");
			F_WriteText( GENFORM, ENAME, "");
			F_WriteText( GENFORM, EUPLOAD, "5000");
			F_WriteText( GENFORM, ESTART, "5000");

			F_WriteText( GENFORM, EICON, "");
			F_WriteText( GENFORM, ECURSOR, "");

			F_WriteText( GENFORM, EBEGIN, "");
			F_WriteText( GENFORM, EEND, "");
			
			Definition.BeginMessage	=	FALSE;
			Definition.EndMessage	=	FALSE;
			Definition.Icon			=	FALSE;
			Definition.Cursor		=	FALSE;

			if( TypeGeneration == SOURCEASM || TypeGeneration ==  FREEPROGRAM )
			{
				F_Disabled( GENFORM, CB1, TRUE );				
				F_Disabled( GENFORM, CB2, TRUE );				
				F_Disabled( GENFORM, CB3, TRUE );				
				F_Disabled( GENFORM, CB4, TRUE );				

				F_Disabled( GENFORM, EICON, TRUE );				
				F_Disabled( GENFORM, ECURSOR, TRUE );				
				F_Disabled( GENFORM, EBEGIN, TRUE );				
				F_Disabled( GENFORM, EEND, TRUE );				
			}
			else
			{
				F_Disabled( GENFORM, CB1, FALSE );				
				F_Disabled( GENFORM, CB2, FALSE );				
				F_Disabled( GENFORM, CB3, FALSE );				
				F_Disabled( GENFORM, CB4, FALSE );				

				F_Selected( GENFORM, CB1, FALSE );				
				F_Selected( GENFORM, CB2, FALSE );				
				F_Selected( GENFORM, CB3, FALSE );				
				F_Selected( GENFORM, CB4, FALSE );				

				F_Disabled( GENFORM, EICON, FALSE );				
				F_Disabled( GENFORM, ECURSOR, FALSE );				
				F_Disabled( GENFORM, EBEGIN, FALSE );				
				F_Disabled( GENFORM, EEND, FALSE );				
			}			
			F_Selected( GENFORM, CB1, FALSE );				
			F_Selected( GENFORM, CB2, FALSE );				
			F_Selected( GENFORM, CB3, FALSE );				
			F_Selected( GENFORM, CB4, FALSE );				
			
			F_Disabled( GENFORM, BN_47, !(TypeGeneration == COOPERANT) );

		}
		M_Disable( MAINMENU, F_NEW);
		free(Parameter);
	}
	CloseNew();
}

/*
 * ----------------------------------------------------------------------------------------------
 *			Prise en compte des actions de la ToolBar
 * ----------------------------------------------------------------------------------------------
 */

static void MouseEnter(void)
/*
	This procedure is called when the mouse enter onto an object.
*/
{
	if( G_BulleAide )
	{
		switch(F_NameObject)
		{
			case	CB1:	STG_Bubble( BUBBLE40, -1, -1);
							break;

			case	CB2:	STG_Bubble( BUBBLE41, -1, -1);
							break;

			case	CB3:	STG_Bubble( BUBBLE42, -1, -1);
							break;

			case	CB4:	STG_Bubble( BUBBLE43, -1, -1);
							break;
							
			case	BN_42:	STG_Bubble( BUBBLE44, -1, -1);
							break;

			case	BN_43:	STG_Bubble( BUBBLE45, -1, -1);
							break;

			case	BN_44:	
			case	EICON:	STG_Bubble( BUBBLE46, -1, -1);
							break;
							
			case	BN_45:	
			case	ECURSOR:STG_Bubble( BUBBLE47, -1, -1);
							break;
							
			case	BN_40:	STG_Bubble( BUBBLE48, -1, -1);
							break;

			case	BN_47:	STG_Bubble( BUBBLE51, -1, -1);
							break;
			default:		
							break;
		}
	}
}

/* ---------------------------------------------------------------------------------------------- */
static void Close(void)
{
	int		L_inX, L_inY, dummy;

  	wind_get(WHGenForm, WF_CURRXYWH, &L_inX, &L_inY, &dummy, &dummy);
  	
	SetLongProfile(G_APP_INF, G_WIND_GENFORM, "position_x", (long)L_inX);
	SetLongProfile(G_APP_INF, G_WIND_GENFORM, "position_y", (long)L_inY);

   	F_Close(GENFORM);
}

/* ---------------------------------------------------------------------------------------------- */
static void SaveTemplate( void )
{
	FILE	*Fichier;
	char	chLigne[LEN_BUSINESS+1];
	char	L_Path[PATH_LEN]					= "\0"; 						/* Chemin + Nom de fichier */
	char	L_PathName[PATH_LEN+FILENAME_LEN]	= "\0"; 						/* Chemin + Nom de fichier */
	char	L_FileName[FILENAME_LEN]			= "\0"; 						/* Nom de fichier seulement */
	char	L_Extension[EXT_LEN]				= EXT_TEMPLATE;					/* Extension de fichiers */

	if(!A_FileSelector(L_PathName, L_FileName, L_Extension, L_Path, G_SAVE_GEN)) 
		return;					

	MS_Mouse( MS_SLEEP, NULL );

	if( strrchr( L_PathName, '.' ) == NULL )
		strcat( L_PathName, ".def" );
			
    Fichier	=	fopen( L_PathName, "w" );
    if( Fichier == NULL )
    	return;

	F_ReadText( GENFORM, ETITLE, chLigne);
	strcat( chLigne, "\n");
	fprintf( Fichier, chLigne );
	
	F_ReadText( GENFORM, EDATE, chLigne);
	strcat( chLigne, "\n");
	fprintf( Fichier, chLigne );
	
	F_ReadText( GENFORM, EBNESS, chLigne);
	strcat( chLigne, "\n");
	fprintf( Fichier, chLigne );
	
	F_ReadText( GENFORM, EAUTHOR, chLigne);
	strcat( chLigne, "\n");
	fprintf( Fichier, chLigne );
	
	F_ReadText( GENFORM, ENAME, chLigne);
	strcat( chLigne, "\n");
	fprintf( Fichier, chLigne );
	
	F_ReadText( GENFORM, EUPLOAD, chLigne);
	strcat( chLigne, "\n");
	fprintf( Fichier, chLigne );
	
	F_ReadText( GENFORM, ESTART, chLigne);
	strcat( chLigne, "\n");
	fprintf( Fichier, chLigne );


	F_ReadText( GENFORM, EICON, chLigne);
	strcat( chLigne, "\n");
	fprintf( Fichier, chLigne );

	fprintf( Fichier, Definition.IconPathName );	
	fprintf( Fichier, "\n");

	F_ReadText( GENFORM, ECURSOR, chLigne);
	strcat( chLigne, "\n");
	fprintf( Fichier, chLigne );

	fprintf( Fichier, Definition.CursorPathName );	
	fprintf( Fichier, "\n");

	F_ReadText( GENFORM, EBEGIN, chLigne);
	strcat( chLigne, "\n");
	fprintf( Fichier, chLigne );
	
	F_ReadText( GENFORM, EEND, chLigne);
	strcat( chLigne, "\n");
	fprintf( Fichier, chLigne );
	
	MS_Mouse( MS_POINTFLATHAND, NULL );
	fclose( Fichier );
}

/* ---------------------------------------------------------------------------------------------- */
static void ReadTemplate( void )
{
	FILE	*Fichier;
	char	chLigne[LEN_BUSINESS];
	char	L_Path[PATH_LEN]					= "\0"; 						/* Chemin + Nom de fichier */
	char	L_PathName[PATH_LEN+FILENAME_LEN]	= "\0"; 						/* Chemin + Nom de fichier */
	char	L_FileName[FILENAME_LEN]			= "\0"; 						/* Nom de fichier seulement */
	char	L_Extension[EXT_LEN]				= EXT_TEMPLATE;					/* Extension de fichiers */

	if(!A_FileSelector(L_PathName, L_FileName, L_Extension, L_Path, G_SAVE_GEN)) 
		return;					

	MS_Mouse( MS_SLEEP, NULL );
			
    Fichier	=	fopen( L_PathName, "r" );
    if( Fichier == NULL )
    	return;

	fgets( chLigne, LEN_BUSINESS, Fichier );
	chLigne[ strlen(chLigne)-1] = '\0';
	F_WriteText( GENFORM, ETITLE, chLigne);
	
	fgets( chLigne, LEN_BUSINESS, Fichier );	
	chLigne[ strlen(chLigne)-1] = '\0';
	F_WriteText( GENFORM, EDATE, chLigne);
	
	fgets( chLigne, LEN_BUSINESS, Fichier );	
	chLigne[ strlen(chLigne)-1] = '\0';
	F_WriteText( GENFORM, EBNESS, chLigne);
	
	fgets( chLigne, LEN_BUSINESS, Fichier );	
	chLigne[ strlen(chLigne)-1] = '\0';
	F_WriteText( GENFORM, EAUTHOR, chLigne);
		
	fgets( chLigne, LEN_BUSINESS, Fichier );	
	chLigne[ strlen(chLigne)-1] = '\0';
	F_WriteText( GENFORM, ENAME, chLigne);
	
	fgets( chLigne, LEN_BUSINESS, Fichier );	
	chLigne[ strlen(chLigne)-1] = '\0';
	F_WriteText( GENFORM, EUPLOAD, chLigne);
	
	fgets( chLigne, LEN_BUSINESS, Fichier );	
	chLigne[ strlen(chLigne)-1] = '\0';
	F_WriteText( GENFORM, ESTART, chLigne);

	fgets( chLigne, LEN_BUSINESS, Fichier );	
	chLigne[ strlen(chLigne)-1] = '\0';
	F_WriteText( GENFORM, EICON, chLigne);
	F_Selected( GENFORM, CB3, ( (int)strlen( chLigne ) ) );
	if( Definition.Type != SOURCEASM && Definition.Type !=  FREEPROGRAM)
	{
		F_Disabled( GENFORM, EICON, ( strlen( chLigne ) == 0 ) );
		F_Disabled( GENFORM, BN_44, ( strlen( chLigne ) == 0 ) );
	}
	Definition.Icon = ( strlen( chLigne ) ) ? TRUE : FALSE;
	
	fgets( Definition.IconPathName, PATH_LEN+FILENAME_LEN, Fichier );	
	Definition.IconPathName[ strlen(Definition.IconPathName)-1] = '\0';
	
	fgets( chLigne, LEN_BUSINESS, Fichier );	
	chLigne[ strlen(chLigne)-1] = '\0';
	F_WriteText( GENFORM, ECURSOR, chLigne);
	F_Selected( GENFORM, CB4, ( (int)strlen( chLigne ) ) );
	if( Definition.Type != SOURCEASM && Definition.Type !=  FREEPROGRAM)
	{
		F_Disabled( GENFORM, ECURSOR, ( strlen( chLigne ) == 0) );
		F_Disabled( GENFORM, BN_45, ( strlen( chLigne ) == 0) );
	}
	Definition.Cursor = ( strlen( chLigne ) ) ? TRUE : FALSE;
	
	fgets( Definition.CursorPathName, PATH_LEN+FILENAME_LEN, Fichier );	
	Definition.CursorPathName[ strlen(Definition.CursorPathName)-1] = '\0';

	fgets( chLigne, LEN_BUSINESS, Fichier );	
	chLigne[ strlen(chLigne)-1] = '\0';
	F_WriteText( GENFORM, EBEGIN, chLigne);
	F_Selected( GENFORM, CB1, ( (int)strlen( chLigne ) ) );
	if( Definition.Type != SOURCEASM && Definition.Type !=  FREEPROGRAM)
		F_Disabled( GENFORM, EBEGIN, ( strlen( chLigne ) == 0) );
	Definition.BeginMessage = ( strlen( chLigne ) ) ? TRUE : FALSE;
	
	fgets( chLigne, LEN_BUSINESS, Fichier );	
	chLigne[ strlen(chLigne)-1] = '\0';
	F_WriteText( GENFORM, EEND, chLigne);
	F_Selected( GENFORM, CB2, ( (int)strlen( chLigne ) ) );
	if( Definition.Type != SOURCEASM && Definition.Type !=  FREEPROGRAM)
		F_Disabled( GENFORM, EEND, ( strlen( chLigne ) == 0)  );
	Definition.EndMessage = ( strlen( chLigne ) ) ? TRUE : FALSE;
	
	MS_Mouse( MS_POINTFLATHAND, NULL );
	fclose( Fichier );
}

/* ---------------------------------------------------------------------------------------------- */
static void ActiveProjet( TProjet *pProjet )
{
	OBJECT	*Arbre;
	char	L_chFileName[FILENAME_LEN];
	
	rsrc_gaddr(R_TREE, FBUREAU, &Arbre);	
	
	/* Afficher l'icone repr‚sentant le fichier source */
	Arbre[ISRC].ob_flags &= ~HIDETREE;
							
	/*	V‚rifi la taille de la chaine pour l'icone */
	if( strlen(pProjet->SrcFileName) > FILE_ICON_LEN)
	{
		memcpy( L_chFileName, pProjet->SrcFileName, FILE_ICON_LEN - 4);
		L_chFileName[FILE_ICON_LEN - 3]	=	0;
		strcat( L_chFileName, "...");
	}	
	else
		strcpy( L_chFileName, pProjet->SrcFileName );
								
	strcpy(Arbre[ISRC].ob_spec.iconblk->ib_ptext, L_chFileName);
	form_dial(FMD_FINISH,0,0,0,0,Arbre[ISRC].ob_x,Arbre[ISRC].ob_y,Arbre[ISRC].ob_width,Arbre[ISRC].ob_height*2);

	/* Afficher l'icone repr‚sentant le fichier executable */
	Arbre[IPRG].ob_flags &= ~HIDETREE;
							
	/*	V‚rifi la taille de la chaine pour l'icone */
	if( strlen(pProjet->ExeFileName) > FILE_ICON_LEN)
	{
		memcpy( L_chFileName, pProjet->ExeFileName, FILE_ICON_LEN - 4);
		L_chFileName[FILE_ICON_LEN - 3]	=	0;
		strcat( L_chFileName, "...");
	}	
	else
		strcpy( L_chFileName, pProjet->ExeFileName );
								
	strcpy(Arbre[IPRG].ob_spec.iconblk->ib_ptext, L_chFileName);
	form_dial(FMD_FINISH,0,0,0,0,Arbre[IPRG].ob_x,Arbre[IPRG].ob_y,Arbre[IPRG].ob_width,Arbre[IPRG].ob_height*2);

	M_Enable(MAINMENU, F_CLOSE);
}

/* ---------------------------------------------------------------------------------------------- */
static void Action(void)
/*
	Prise en compte des actions sur les boutons.
*/
{	
	char			L_Path[PATH_LEN]					= "\0"; 						/* Chemin + Nom de fichier */
	char			L_PathName[PATH_LEN+FILENAME_LEN]	= "\0"; 						/* Chemin + Nom de fichier */
	char			L_FileName[FILENAME_LEN]			= "\0"; 						/* Nom de fichier seulement */
	char			L_Extension[EXT_LEN]				= EXT_CRY;						/* Extension de fichiers */
	char			chTexte[LEN_FILE];
	char			*Ptr; 
	
	switch( F_NameObject )
	{
		/*	Bouton OK */
		case	BN_40:	F_ReadText( GENFORM, ETITLE, Definition.Title);
						F_ReadText( GENFORM, EDATE, Definition.Date);
						F_ReadText( GENFORM, EBNESS, Definition.Business);
						F_ReadText( GENFORM, EAUTHOR, Definition.Author);
						F_ReadText( GENFORM, ENAME, Definition.ExecName);
						F_ReadText( GENFORM, EUPLOAD, Definition.UploadAddress);
						F_ReadText( GENFORM, ESTART, Definition.StartAddress);


						F_ReadText( GENFORM, EICON, Definition.IconName);
						F_ReadText( GENFORM, ECURSOR, Definition.CursorName);

						F_ReadText( GENFORM, EBEGIN, Definition.TextMsgStart);
						F_ReadText( GENFORM, EEND, Definition.TextMsgEnd);

						if( strlen( Definition.ExecName ) == 0 || 
							strlen( Definition.UploadAddress ) == 0 || 
							strlen( Definition.StartAddress ) == 0 )
						{
							form_alert( 1, ERROR_INPUT );
						}
						else
						{
							if( Generation( &Projet, &Definition ) )
							{
								M_Enable( MAINMENU, F_NEW);
								M_Enable( MAINMENU, F_CLOSA);
								ActiveProjet( &Projet );
								Close();
							}							
						}
						break;

		/*	Bouton Cancel */
		case	BN_41:	M_Enable( MAINMENU, F_NEW);
						Close();
    					break;
						
		/*	*/								
		case	BN_42:	ReadTemplate();
						break;

		/*	*/
		case	BN_43:	SaveTemplate();
						break;

		/*	Browse Icon */
		case	BN_44:	
		case	EICON:
						if( A_FileSelector(L_PathName, L_FileName, L_Extension, L_Path, G_BROWSE_TGA) ) 
						{
							if( (Ptr = strrchr( L_FileName, '.' )) != NULL )
								*Ptr = '\0';
								
							if( strlen( L_FileName ) > LEN_FILE - 1 )
							{
								memcpy( chTexte, L_FileName, 30);
								strcat( chTexte, "...");
							}
							else
								strcpy( chTexte, L_FileName );
							F_WriteText( GENFORM, EICON, chTexte );
							strcpy( Definition.IconPathName, L_PathName );
						}
						break;

		/*	Browse Cursor */
		case	BN_45:	
		case	ECURSOR:

						if( A_FileSelector(L_PathName, L_FileName, L_Extension, L_Path, G_BROWSE_TGA) ) 
						{
							if( (Ptr = strrchr( L_FileName, '.' )) != NULL )
								*Ptr = '\0';
								
							if( strlen( L_FileName ) > LEN_FILE - 1 )
							{
								memcpy( chTexte, L_FileName, 30);
								strcat( chTexte, "...");
							}
							else
								strcpy( chTexte, L_FileName );
							F_WriteText( GENFORM, ECURSOR, chTexte );							
							strcpy( Definition.CursorPathName, L_PathName );
						}
						break;

		case	CB1:	F_Disabled( GENFORM, EBEGIN, Definition.BeginMessage );
						Definition.BeginMessage = ( Definition.BeginMessage ) ? FALSE : TRUE;
						F_Selected( GENFORM, F_NameObject, Definition.BeginMessage );
						break;

		case	CB2:	F_Disabled( GENFORM, EEND, Definition.EndMessage );
						Definition.EndMessage = ( Definition.EndMessage ) ? FALSE : TRUE;
						F_Selected( GENFORM, F_NameObject, Definition.EndMessage );
						break;

		case	CB3:	F_Disabled( GENFORM, EICON, Definition.Icon );
						F_Disabled( GENFORM, BN_44, Definition.Icon );
						Definition.Icon = ( Definition.Icon ) ? FALSE : TRUE;
						F_Selected( GENFORM, F_NameObject, Definition.Icon );
						break;

		case	CB4:	F_Disabled( GENFORM, ECURSOR, Definition.Cursor );
						F_Disabled( GENFORM, BN_45, Definition.Cursor );
						Definition.Cursor = ( Definition.Cursor ) ? FALSE : TRUE;
						F_Selected( GENFORM, F_NameObject, Definition.Cursor );
						break;

		case	BN_47:	GenFrmP( &Definition );
						break;
						
		case	HELP7:	STG_Open(G_HelpFile, SECTION_HELP_GENFORM);
						break;
						
		default:		break;
	}
}

