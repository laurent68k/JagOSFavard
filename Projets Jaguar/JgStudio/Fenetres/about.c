/*
 * ----------------------------------------------------------------------------------------------
 *								Fenetres A propos et more.
 *
 * 	Author  : 	FAVARD Laurent
 *	File    : 	About.C
 *	Date    : 	18 December 1996
 *	Release : 	31 Juillet 1997
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
 *		Description de la fenetre A propos...
 * ----------------------------------------------------------------------------------------------
 */

void	AboutOk(void);
void	PhotoCachee(void);
void	More(void);

static Type_Form_List
FormInfo[]=	{
				{BN_3,	AboutOk,	PhotoCachee},
				{BN_4,	More,		NULL},
				FORM_END
			};

static Type_Form_List
FormMore[]=	{	{0,	NULL,	NULL},
				FORM_END
			};
	
/*
 * ----------------------------------------------------------------------------------------------
 *			Fichiers sp‚cifiques … inclure
 */

#include	"..\modules\messages.h"
#include	"..\modules\jaguar.h"
#include	"..\modules\const.h"

/*
 * ----------------------------------------------------------------------------------------------
 *			Variables
 */

int					WHInfo;
static	int			NombreClicPhoto = 1;							/* Compteur de  clics sur la photo cach‚e */

/*
 * ----------------------------------------------------------------------------------------------
 *				Gestion de fenetre a propos...
 * ----------------------------------------------------------------------------------------------
 */

void AboutMe(void)
/*
	Ouvre la fenetre A propos de ... de l'application.
*/
{
	Type_Formular_Parameter		*Parameter;
	TAadsVersion 				L_AadsVersion;
	TJTBVersion 				L_JtbVersion;
	unsigned long				L_lSTRam, L_lTTRam;
	char						L_sSTRam[20], L_sTTRam[20], L_sVideo[26], 
								L_sTOS[6], L_sAads[50];
  	char						sMail[50];;
  	
	Parameter = F_GetParameter();
	if(Parameter != NULL)
	{
    	Parameter->TreeName =	ABOUTME;														/* Indice de l'arbre de l'objet */
		Parameter->Title	=	G_TITLE_INFO;													/* Titre de la fenetre */
    	Parameter->Mode		=	F_WIND;														/* Opening mode */
    	Parameter->FirstEdit=	NOTEXT;														/* First Editable Text */
    	Parameter->List		=	FormInfo;															/* Event list */
    	Parameter->FormIcon =	IAPPLI;														/* Icone d'iconification */
		Parameter->Mouse	=	MS_ATARI;

		if(!G_ResolutionChanged)
		{
			Parameter->x	=	(int)GetLongProfile(G_APP_INF, G_WIND_ABOUT, "position_x", 0);
			Parameter->y	=	(int)GetLongProfile(G_APP_INF, G_WIND_ABOUT, "position_y", 0);
		}

    	if((WHInfo = F_XOpen(Parameter)) != FALSE)
    	{
    		F_WriteText(ABOUTME, APPNAME, G_APP_NAME);										/* Nom de cette application */
     		F_WriteText(ABOUTME, VERSION, G_APP_NUMBER);									/* Version de l'application */
			F_WriteText(ABOUTME, APPDATE, G_APP_DATE);										/* Date de cette version */
																					
			if(MchInfo.GemdosVersion >= GEMDOS_MXALLOC)										/* Si GEMDOS supporte appel de fct en TT-RAM */
			{
				L_lSTRam	=	(unsigned long)Mxalloc(-1L, MX_STRAM);						/* Taille m‚moire ST */
				L_lTTRam	=	(unsigned long)Mxalloc(-1L, MX_TTRAM);						/* Taille m‚moire TT */
			}
			/* Sinon, c'est qu'il n'existe pas de TT-Ram ! */
			else
			{
        		L_lSTRam	=	(unsigned long)Malloc(-1L);									/* Taille m‚moire ST */
        		L_lTTRam	=	0L;															/* Taille m‚moire TT */
			}
      
     		sprintf(L_sSTRam, "%ld Bytes", L_lSTRam);
      		sprintf(L_sTTRam, "%ld Bytes", L_lTTRam);      
      		sprintf(L_sVideo, "%dx%d by %d Colors", VdiInfo.LargeurStation+1, VdiInfo.HauteurStation+1, VdiInfo.NombreCouleurs);
      		sprintf(L_sTOS, "%X", MchInfo.TosVersion);
 
      		F_WriteText(ABOUTME, STRAM,   L_sSTRam);										/* M‚moire ST disponible */
      		F_WriteText(ABOUTME, TTRAM,   L_sTTRam);										/* M‚moire TT disponible */
      		F_WriteText(ABOUTME, VIDEO,   L_sVideo);										/* R‚solution courante */
			F_WriteText(ABOUTME, OS,   	L_sTOS);
			
			A_GetVersion( &L_AadsVersion );
			sprintf(L_sAads, "v%s Date %s", L_AadsVersion.Version, L_AadsVersion.Date);
      		F_WriteText(ABOUTME, AADSINFO, L_sAads);

			JTB_Version( &L_JtbVersion );
			sprintf(L_sAads, "v%s Date %s", L_JtbVersion.Version, L_JtbVersion.Date);
      		F_WriteText(ABOUTME, JTBINFO, L_sAads);
      		
      		GetProfileString(G_APP_INF, INTERNET, ADR_EMAIL, sMail, G_EMAIL_LAURENT);
      		F_WriteText(ABOUTME, EMAIL, sMail);
      		GetProfileString(G_APP_INF, INTERNET, ADR_HTTP, sMail, G_HTTP);
      		F_WriteText(ABOUTME, HTTP, sMail);
    	}
    	free(Parameter);
	}
}

static void PhotoCachee(void)
/*
	Called by Double-Clic on Exit button of about me window
*/
{
  OBJECT	*Arbre;
  
  if(NombreClicPhoto == 3)												/* Rendre la photo visible */
  {
      rsrc_gaddr(R_TREE, ABOUTME, &Arbre);								/* Bit objet invisible Off */
      Arbre[FUNSHIP].ob_flags &= ~HIDETREE;	
      F_RedrawObject(Arbre, FUNSHIP);									/* Redessiner la photo */
      NombreClicPhoto = 1;
  } 
  else																	/* Compter le nombre de Double clics */
    NombreClicPhoto++;
}

static void AboutOk(void)
{
	OBJECT	*Arbre;
  	int							L_inX, L_inY, dummy;

  	wind_get(WHInfo, WF_CURRXYWH, &L_inX, &L_inY, &dummy, &dummy);
  	
	SetLongProfile(G_APP_INF, G_WIND_ABOUT, "position_x", (long)L_inX);
	SetLongProfile(G_APP_INF, G_WIND_ABOUT, "position_y", (long)L_inY);

	if(F_NameObject == BN_3)
	{
		rsrc_gaddr(R_TREE,ABOUTME, &Arbre);												/* Tree address */
		Arbre[FUNSHIP].ob_flags |= HIDETREE;												/* Hide photo */
		WHInfo	=	0;
		F_Close( ABOUTME );
	}
}

static void More(void)
/*
	Ouvre la fenetre "more informations...."
*/
{
	F_Open(MORE, G_TITLE_MORE, F_WIND, NOTEXT, FormMore);
}
