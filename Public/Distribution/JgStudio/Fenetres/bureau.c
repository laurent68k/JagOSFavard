/*
 * ----------------------------------------------------------------------------------------------
 *					Gestion du bureau de l'application, de ses icones et menu popup.
 *
 * 	Author  : 	FAVARD Laurent
 *	File    : 	Bureau.C
 *	Date    : 	18 December 1996
 *	Release : 	30 December 1998
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
#include	"..\modules\mainmenu.h"
#include	"..\modules\appopen.h"
#include	"..\modules\Jaguar.h"
#include	"..\modules\Lunch.h"

#include	"..\fenetres\sender.h"
#include	"..\fenetres\options.h"
#include	"..\fenetres\fuji.h"
#include	"..\fenetres\Toolsbar.h"
#include	"..\fenetres\console.h"

/*
 * ----------------------------------------------------------------------------------------------
 *				Double clic, clic droit et mouvement sur les icones du bureau
 * ----------------------------------------------------------------------------------------------
 */

void Popup1(void)
/*
	Traitements sur clic des items du menu popup #1.
*/
{
	if(F_PopupName == POPUP1)
	{
		switch(F_PopupEntry)
		{
			/*	NEW item */
			case	ITEM15:	M_Entry		=	F_NEW;
							Fichier();
							break;
							
			/*	OPEN item */
			case	ITEM16:	M_Entry		=	F_OPEN;
							Fichier();
							break;
							
			/*	OPEN executable item */
			case	ITEM10:	M_Entry		=	F_OPENE;
							Fichier();
							break;

			/*	CLOSE item */
			case	ITEM11: M_Entry		=	F_CLOSE;
							Fichier();
							break;

			/*	INFORMATIONS item */
			case	ITEM12:	M_Entry		=	F_INFO;
							Fichier();
							break;

			/*	LOAD item */
			case	ITEM13:	M_Entry		=	L_LOAD;
							Connect();
							break;
			
			/*	LOAD&RUN item */
			case	ITEM14:	M_Entry		=	L_LDRUN;
							Connect();
							break;
		}
	}
}

void Popup2(void)
/*
	Traitements sur clic des items du menu popup #2.
*/
{
	if(F_PopupName == POPUP2)
	{
		switch(F_PopupEntry)
		{
			/*	RUN item */
			case	ITEM20:	M_Entry		=	L_RUN;
							Connect();
							break;

			/*	PING item */
			case	ITEM21: M_Entry		=	L_PING;
							Connect();
							break;

			/*	EPROM item */
			case	ITEM22:	M_Entry		=	T_EPROM;
							Outils();
							break;

			/*	GET item */
			case	ITEM23:	M_Entry		=	L_GET;
							Connect();
							break;
			
		}
	}
}

void Popup3(void)
/*
	Traitements sur clic des items du menu popup #2.
*/
{
	if(F_PopupName == POPUP3)
	{
		switch(F_PopupEntry)
		{
			/*	FUJI FRIEND item */
			case	ITEM30:	OpenSmiley();
							break;

			/*	SAVE DESKTOP item */
			case	ITEM31: SaveOptions();
							break;

			/*	PROPERTIES item */
			case	ITEM32:	Options();
							break;			

			/*	PROPERTIES item */
			case	ITEM33:	ToolBar();
							break;			

			/*	PROPERTIES item */
			case	ITEM34:	OpenConsole();
							break;			
		}
	}
}

void ClicRight(void)
/*
	Process when a right mouse clic is occured. open a popup menu.
*/
{  	
	OBJECT	*Arbre;

	switch(I_Name)
	{
		/*	Fond du bureau */
		case ROOT:
						rsrc_gaddr(R_TREE, POPUP3, &Arbre);
						F_Popup(FBUREAU, I_Name, A_MouseX, A_MouseY);
			  			break;

		/*	Icone disquette */
    	case IDISK:		rsrc_gaddr(R_TREE, POPUP1, &Arbre);
		    			Arbre[ITEM10].ob_state	&=	~DISABLED;
						Arbre[ITEM11].ob_state	|=	DISABLED;
						Arbre[ITEM12].ob_state	|=	DISABLED;
						Arbre[ITEM13].ob_state	|=	DISABLED;
						Arbre[ITEM14].ob_state	|=	DISABLED;
						F_Popup(FBUREAU, I_Name, A_MouseX, A_MouseY);
	    				break;
    				
		/*	icone fichier programme executable JAG */
		case IPRG:		rsrc_gaddr(R_TREE, POPUP1, &Arbre);
		    			Arbre[ITEM10].ob_state	&=	~DISABLED;
						Arbre[ITEM11].ob_state	&=	~DISABLED;
						Arbre[ITEM12].ob_state	&=	~DISABLED;
						Arbre[ITEM13].ob_state	&=	~DISABLED;
						Arbre[ITEM14].ob_state	&=	~DISABLED;
						Arbre[ITEM15].ob_state	&=	~DISABLED;
						Arbre[ITEM16].ob_state	&=	~DISABLED;
						F_Popup(FBUREAU, I_Name, A_MouseX, A_MouseY);
			  			break;

		/*	icone fichier programme source  */
		case ISRC:		rsrc_gaddr(R_TREE, POPUP1, &Arbre);
		    			Arbre[ITEM10].ob_state	&=	~DISABLED;
						Arbre[ITEM11].ob_state	&=	~DISABLED;
						Arbre[ITEM12].ob_state	|=	DISABLED;
						Arbre[ITEM13].ob_state	|=	DISABLED;
						Arbre[ITEM14].ob_state	|=	DISABLED;
						Arbre[ITEM15].ob_state	&=	~DISABLED;
						Arbre[ITEM16].ob_state	&=	~DISABLED;
						F_Popup(FBUREAU, I_Name, A_MouseX, A_MouseY);
			  			break;

		/*	icone Jaguar */
		case IJAG:
						rsrc_gaddr(R_TREE, POPUP2, &Arbre);
						F_Popup(FBUREAU, I_Name, A_MouseX, A_MouseY);
			  			break;

		default:		break;    
	}
}

void ClicLeft(void)
/*
	When a simple mouse clic left is occured. Nothing to do !
*/
{
}

void DClicLeft(void)
/*
	When a icon is double-cliced
*/
{  	
	int				Message[8];
	
	
	switch(I_Name)
	{
		/*	Icone disquette */
    	case IDISK:	M_Entry	=	F_OPENE;
					Fichier();
    				break;
    				
		/*	icone fichier executable JAG */
		case IPRG:	M_Entry		=	F_INFO;
					Fichier();
					break;
    				
		/*	Icone fichier source JAG: Protocole AV est puissant car cela lancera le PRG
		 *	associ‚ au type de fichier source (DEVPAC pour *.s par exemple !) 
		 */
		case ISRC:	Message[0]	=	AV_STARTPROG;
					Message[1]	=	AesHandle;
					Message[2]	=	0;
					Message[3]	=	(int)(((unsigned long)Projet.SrcPathName) >> 16);
					Message[4]	=	(int)(((unsigned long)Projet.SrcPathName) & 0xFFFF);
					Message[5]	=	0;
					Message[6]	=	0;
					Message[7]	=	0;
			
					/*	Sauver le repertoire courant et se positionner sur celui du source */

					Dsetdrv( Projet.SrcPath[0] - 'A' );
					Dsetpath( Projet.SrcPath + 2  );
					
					appl_write( AVSERVER , 16, Message);

					break;

		/*	icone editeur */
		case IEDIT:	Message[0]	=	AV_STARTPROG;
					Message[1]	=	AesHandle;
					Message[2]	=	0;
					Message[3]	=	(int)(((unsigned long)External.EditPathName) >> 16);
					Message[4]	=	(int)(((unsigned long)External.EditPathName) & 0xFFFF);
					Message[5]	=	0;
					Message[6]	=	0;
					Message[7]	=	0;
			
					appl_write( AVSERVER , 16, Message);
					break;

		/*	icone assembleur */
		case IASM:	Message[0]	=	AV_STARTPROG;
					Message[1]	=	AesHandle;
					Message[2]	=	0;
					Message[3]	=	(int)(((unsigned long)External.AsmPathName) >> 16);
					Message[4]	=	(int)(((unsigned long)External.AsmPathName) & 0xFFFF);
					Message[5]	=	0;
					Message[6]	=	0;
					Message[7]	=	0;
			
					appl_write( AVSERVER , 16, Message);
					break;

		default:	break;    
	}
}

void Move(int TypeMovment,int IconTarget)
/*
	When a icon movment is occured.
*/
{
	switch(TypeMovment)
	{
		/*	icone vers icone */
    	case ICON2ICON:	
					/*	Fichier JAG vers poubelle */
    				if( (I_Name == IPRG && IconTarget == ITRASH) ||
    					(I_Name == ISRC && IconTarget == ITRASH) )
    				{
    					M_Entry	=	F_CLOSE;
						Fichier();
    			  	}
    			  	
					/*	Fichier JAG vers JAGUAR */
    				if(I_Name == IPRG && IconTarget == IJAG)
    				{
    					M_Entry	=	L_LOAD;
						Connect();
    			  	}

					/*	Fichier Source vers editeur */
    				if(I_Name == ISRC && IconTarget == IEDIT)
    				{
						EditSource();
    			  	}

					/*	Fichier Source vers assembleur */
    				if(I_Name == ISRC && IconTarget == IASM)
    				{
    					AsmSource();
    			  	}
    				break;
		default:		
    				break;	
	}
}
