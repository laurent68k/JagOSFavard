/*
 * ----------------------------------------------------------------------------------------------
 *					Module Debug interne pour developpement
 *
 * 	Author  : 	FAVARD Laurent
 *	File    : 	Debug.C
 *	Date    : 	21 January 1999
 *	Release : 	21 Janaury 1999
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
#include	"..\Tos.rsc\jaguar_F.H"
#endif

/*
 * ----------------------------------------------------------------------------------------------
 *			Fichiers sp‚cifiques … inclure
 */

#include	"..\modules\jaguar.h"
#include	"..\modules\messages.h"

#include	"..\fenetres\console.h"										/*	Gestion de la fenetre terminal Jaguar */

/*
 * ----------------------------------------------------------------------------------------------
 */

static void	Close(void);
static void	DBProjet(void);
static void	DBExternal(void);

static Type_Form_List
FormAction[]=	{
					{BN_50,	Close,		NULL},
					{BN_51,	DBProjet,	NULL},
					{BN_52,	DBExternal,	NULL},
					FORM_END
				};

static	int		WHDebug;

/*
 * ----------------------------------------------------------------------------------------------
 */

void OpenDebug( void )
/*
	Ouvre la fenetre Debug.
*/
{
	Type_Formular_Parameter		*Parameter;
  
	Parameter = F_GetParameter();
	if(Parameter != NULL)
	{
    	Parameter->TreeName =	DEBUG;														/* Indice de l'arbre de l'objet */
		Parameter->Title	=	G_TITLE_DEBUG;													/* Titre de la fenetre */
    	Parameter->Mode		=	F_WIND;														/* Opening mode */
    	Parameter->FirstEdit=	NOTEXT;														/* First Editable Text */
    	Parameter->List		=	FormAction;															/* Event list */
    	Parameter->FormIcon =	IAPPLI;														/* Icone d'iconification */
		Parameter->Mouse	=	ARROW;

    	if((WHDebug = F_XOpen(Parameter)) != FALSE)
    	{
    	}
    	free(Parameter);
	}
}

/*
 * ----------------------------------------------------------------------------------------------
 */
 
void DBProjet( void )
{
	char	chTexte[PATH_LEN+FILENAME_LEN];
	
	/*	Nom du fichier source principal */	
	AddString("Main source file");	
	sprintf(chTexte, "SrcExtension=%s", Projet.SrcExtension);		
	AddString(chTexte);	
	sprintf(chTexte, "SrcPath=%s", Projet.SrcPath);
	AddString(chTexte);	
	sprintf(chTexte, "SrcPathName=%s", Projet.SrcPathName);
	AddString(chTexte);	
	sprintf(chTexte, "SrcFileName=%s", Projet.SrcFileName);
	AddString(chTexte);	

	/*	Nom du fichier executable */
	AddString("Executable Jaguar file");	
	sprintf(chTexte, "ExeExtension=%s", Projet.ExeExtension);
	AddString(chTexte);	
	sprintf(chTexte, "ExePath=%s", Projet.ExePath);
	AddString(chTexte);	
	sprintf(chTexte, "ExePathName=%s", Projet.ExePathName);
	AddString(chTexte);	
	sprintf(chTexte, "ExeFileName=%s", Projet.ExeFileName);
	AddString(chTexte);	
}

void DBExternal( void )
{
	char	chTexte[PATH_LEN+FILENAME_LEN];
	
	/*	Editeur de texte */
	AddString("Text editer");	
	sprintf(chTexte, "EditPath=%s", External.EditPath);
	AddString(chTexte);	
	sprintf(chTexte, "EditPathName=%s", External.EditPathName);
	AddString(chTexte);	
	sprintf(chTexte, "EditFileName=%s", External.EditFileName);
	AddString(chTexte);	

	/*	Assembleur 680x0 */
	AddString("Assembler");	
	sprintf(chTexte, "AsmPath=%s", External.AsmPath);
	AddString(chTexte);	
	sprintf(chTexte, "AsmPathName=%s", External.AsmPathName);
	AddString(chTexte);	
	sprintf(chTexte, "AsmFileName=%s", External.AsmFileName);
	AddString(chTexte);	
}

static void Close(void)
{
	F_Close( DEBUG );
}