/*
 * ----------------------------------------------------------------------------------------------
 *
 *
 * Author  : 	FAVARD Laurent
 *	File    : 	Cpu680x0.C
 *	Date    : 	12 June 1998
 *	Release : 	12 June 1998
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
#include	"..\modules\const.h"
#include	"..\modules\jaguar.h"

/*
 * ----------------------------------------------------------------------------------------------
 *		Description de la fenetre "Quitter l'application"
 * ----------------------------------------------------------------------------------------------
 */

void	Action(void);
void	MouseEnter(void);

static Type_Form_List
FormAction[]=	{	
					{BN_29,	Action,	NULL},
					{BN_32,	Action,	NULL, NULL, NULL, MouseEnter, NULL},
					FORM_END
				};

static	char	Erreur[60][50] = {	"No error\0",
									"No error\0",
									"Bus error\0",
									"Address error\0",
									"Illegal instruction\0",
									"Divide by 0\0",
									"CHK exception\0",
									"TRAPV in,struction\0",
									"Privilege violation\0",
									"Trace exception\0",
									"Line-A emulation 1010\0",
									"Line-F emulation 1111\0",
									"Reserved by Motorola\0",
									"Reserved by Motorola\0",
									"Reserved by Motorola\0",
									"Unitialized interrupt\0",
									"Reserved by Motorola\0",
									"Reserved by Motorola\0",
									"Reserved by Motorola\0",
									"Reserved by Motorola\0",
									"Reserved by Motorola\0",
									"Reserved by Motorola\0",
									"Reserved by Motorola\0",
									"Reserved by Motorola\0",
									"Spurious Interrupt\0",
									"Auto vector interrupt 1\0",
									"Auto vector interrupt 2\0",
									"Auto vector interrupt 3\0",
									"Auto vector interrupt 4\0",
									"Auto vector interrupt 5\0",
									"Auto vector interrupt 6\0",
									"Auto vector interrupt 7\0",
									"Trap #1 called without any code\0",
									"Trap #2 called without any code\0",
									"Trap #3 called without any code\0",
									"Trap #4 called without any code\0",
									"Trap #5 called without any code\0",
									"Trap #6 called without any code\0",
									"Trap #7 called without any code\0",
									"Trap #8 called without any code\0",
									"Trap #9 called without any code\0",
									"Trap #10 called without any code\0",
									"Trap #11 called without any code\0",
									"Trap #12 called without any code\0",
									"Trap #13 called without any code\0",
									"Trap #14 called without any code\0",
									"Trap #15 called without any code\0",
									"Trap #16 called without any code\0",
									"User interrupt vectors ($100 to $3FF)\0",
								};
							
/*
 * ----------------------------------------------------------------------------------------------
 *			Variables
 */

static	int			WHCpu;
static	TException	G_Exception;

/*
 * ----------------------------------------------------------------------------------------------
 *			Fenetre 
 * ----------------------------------------------------------------------------------------------
 */

static void Display( TException *Exception )
{
	char	L_texte[100];
	OBJECT	*Arbre;
	
	/*	Afficher les infos suppl‚mentaires pour Bus et Address, sinon cacher */
	rsrc_gaddr(R_TREE, ABOUTME, &Arbre);
	if( Exception->ExceptionNumber == 2 || Exception->ExceptionNumber == 3 )
		Arbre[ERRBUS].ob_flags &= ~HIDETREE;
	else
		Arbre[ERRBUS].ob_flags |= HIDETREE;

	sprintf(L_texte, "Exception 680x0 number $%X", Exception->ExceptionNumber );
	F_WriteText(CPU, EX_NUM, L_texte);		
	
	sprintf(L_texte, "%s", Erreur[Exception->ExceptionNumber]);	
	F_WriteText(CPU, EX_LIB, L_texte);

	sprintf(L_texte, "%lX", Exception->usp );
	F_WriteText(CPU, USP, L_texte);
	sprintf(L_texte, "%lX", Exception->ssp );
	F_WriteText(CPU, SSP, L_texte);
	
	sprintf(L_texte, "%lX", Exception->d0 );
	F_WriteText(CPU, D0, L_texte);
	sprintf(L_texte, "%lX", Exception->d1 );
	F_WriteText(CPU, D1, L_texte);
	sprintf(L_texte, "%lX", Exception->d2 );
	F_WriteText(CPU, D2, L_texte);
	sprintf(L_texte, "%lX", Exception->d3 );
	F_WriteText(CPU, D3, L_texte);
	sprintf(L_texte, "%lX", Exception->d4 );
	F_WriteText(CPU, D4, L_texte);
	sprintf(L_texte, "%lX", Exception->d5 );
	F_WriteText(CPU, D5, L_texte);
	sprintf(L_texte, "%lX", Exception->d6 );
	F_WriteText(CPU, D6, L_texte);
	sprintf(L_texte, "%lX", Exception->d7 );
	F_WriteText(CPU, D7, L_texte);
	
	sprintf(L_texte, "%lX", Exception->a0 );
	F_WriteText(CPU, A0, L_texte);
	sprintf(L_texte, "%lX", Exception->a1 );
	F_WriteText(CPU, A1, L_texte);
	sprintf(L_texte, "%lX", Exception->a2 );
	F_WriteText(CPU, A2, L_texte);
	sprintf(L_texte, "%lX", Exception->a3 );
	F_WriteText(CPU, A3, L_texte);
	sprintf(L_texte, "%lX", Exception->a4 );
	F_WriteText(CPU, A4, L_texte);
	sprintf(L_texte, "%lX", Exception->a5 );
	F_WriteText(CPU, A5, L_texte);
	sprintf(L_texte, "%lX", Exception->a6 );
	F_WriteText(CPU, A6, L_texte);

	sprintf(L_texte, "%lX", Exception->stack_pc );				/*	PC pushed into the stack */
	F_WriteText(CPU, PC, L_texte);
	sprintf(L_texte, "%X", Exception->stack_sr );				/*	PC pushed into the stack */
	F_WriteText(CPU, SR, L_texte);

	sprintf(L_texte, "%X", Exception->ri );						/*	Instr. Register (Bus error, address error) */
	F_WriteText(CPU, RI, L_texte);
	sprintf(L_texte, "%lX", Exception->fault_pc );				/*	fault address responsable */
	F_WriteText(CPU, FAULTPC, L_texte);
	sprintf(L_texte, "%X", Exception->control );
	F_WriteText(CPU, CTRL, L_texte);
}

/*
 * ----------------------------------------------------------------------------------------------
 */

static void Save( TException *Exception )
{
	FILE			*L_Fichier;
	char			L_Path[PATH_LEN]		= "\0"; 						/* Chemin seulement */
	char			L_PathName[PATH_LEN+FILENAME_LEN]	= "\0"; 						/* Chemin + Nom de fichier */
	char			L_FileName[FILENAME_LEN]		= "\0"; 						/* Nom de fichier seulement */
	char			L_Extension[EXT_LEN]		= "*.txt\0";					/* Extension de fichiers */
	
	
	if(A_FileSelector(L_PathName, L_FileName, L_Extension, L_Path, G_SAVE_BIN)) 
	{						
		graf_mouse(BUSYBEE, NULL);
		L_Fichier	=	fopen( L_FileName, "w" );
		if( L_Fichier!= NULL )
		{
		
			fprintf(L_Fichier, ">>>>>> 680x0 Exception repport error <<<<<\n");
			fprintf(L_Fichier, ">  Generate by JagStudio98               <\n");
			fprintf(L_Fichier, ">  FAVARD laurent, Copyright France 1998 <\n\n");

			fprintf(L_Fichier, "JagOS returned these informations:\n\n");
			
			fprintf(L_Fichier, "Exception 680x0 number $%X\n", Exception->ExceptionNumber );	
			fprintf(L_Fichier, "Exception is: %s\n\n", Erreur[Exception->ExceptionNumber]);	

			fprintf(L_Fichier, "USP: $%lX\n", Exception->usp );
			fprintf(L_Fichier, "SSP: $%lX\n\n", Exception->ssp );
	
			fprintf(L_Fichier, "Data registers:\n");
			fprintf(L_Fichier, "D1:  $%lX\n", Exception->d1 );
			fprintf(L_Fichier, "D2:  $%lX\n", Exception->d2 );
			fprintf(L_Fichier, "D3:  $%lX\n", Exception->d3 );
			fprintf(L_Fichier, "D4:  $%lX\n", Exception->d4 );
			fprintf(L_Fichier, "D5:  $%lX\n", Exception->d5 );
			fprintf(L_Fichier, "D6:  $%lX\n", Exception->d6 );
			fprintf(L_Fichier, "D7:  $%lX\n\n", Exception->d7 );
	
			fprintf(L_Fichier, "Address registers:\n");
			fprintf(L_Fichier, "A0:  $%lX\n", Exception->a0 );
			fprintf(L_Fichier, "A1:  $%lX\n", Exception->a1 );
			fprintf(L_Fichier, "A2:  $%lX\n", Exception->a2 );
			fprintf(L_Fichier, "A3:  $%lX\n", Exception->a3 );
			fprintf(L_Fichier, "A4:  $%lX\n", Exception->a4 );
			fprintf(L_Fichier, "A5:  $%lX\n", Exception->a5 );
			fprintf(L_Fichier, "A6:  $%lX\n\n", Exception->a6 );
			
			fprintf(L_Fichier, "From the supervisor stack:\n" );				/*	PC pushed into the stack */
			fprintf(L_Fichier, "PC:  $%lX\n", Exception->stack_pc );				/*	PC pushed into the stack */
			fprintf(L_Fichier, "SR:  $%X\n\n", Exception->stack_sr );				/*	PC pushed into the stack */

			if( Exception->ExceptionNumber == 2 || Exception->ExceptionNumber == 3 )
			{
				fprintf(L_Fichier, "Supplemntary informations for Bus and Address Errors\n\n");
				fprintf(L_Fichier, "Instruction register: $%X\n", Exception->ri );						/*	Instr. Register (Bus error, address error) */
				fprintf(L_Fichier, "Fault PC: $%lX\n", Exception->fault_pc );				/*	fault address responsable */
				fprintf(L_Fichier, "Control:  $%X\n", Exception->control );
			}
			fclose( L_Fichier );
		}
	}
	graf_mouse(ARROW, NULL);
}

/*
 * ----------------------------------------------------------------------------------------------
 */

void OpenCPU( TException *Exception)
/*
	Open the CPU 680x0 window
	
*/
{
	Type_Formular_Parameter	*Parameter;
  
  	G_Exception	=	*Exception;
  	
	Parameter = F_GetParameter();
	if(Parameter != NULL)
	{
		Parameter->Title	=	G_TITLE_CPU;														/* Titre de la fenetre */
		Parameter->TreeName =	CPU;															/* Indice de l'arbre de l'objet */
		Parameter->Mode		=	F_WIND;																/* Opening mode */
		Parameter->FirstEdit=	NOTEXT;																/* First Editable Text */
		Parameter->List		=	FormAction;															/* Event list */
		Parameter->FormIcon =	IAPPLI;																		/* Icone d'iconification */
		Parameter->Mouse 	=	ARROW;			
		Parameter->Type 	=	MODAL;
		if(!G_ResolutionChanged)
		{
			Parameter->x	=	(int)GetLongProfile(G_APP_INF, G_WIND_CPU, "position_x", 0);
			Parameter->y	=	(int)GetLongProfile(G_APP_INF, G_WIND_CPU, "position_y", 0);
		}

		if((WHCpu = F_XOpen(Parameter)) != FALSE)
		{
			Display( &G_Exception );
		}

		free(Parameter);
	}		
}

static void MouseEnter(void)
/*

*/
{
	if( !G_BulleAide )
		return;

	switch( F_NameObject )
	{
		case	BN_32:		STG_Bubble( BUBBLE39, -1, -1);
							break;

		default:			break;
	}
}

static void Action(void)
/*
	Suivant le bouton cliqu‚ dans la fenetre de fin d'application, assigne la variable 'Sortir'.
*/
{
  	int							L_inX, L_inY, dummy;

	switch( F_NameObject)
	{
		case	BN_29:
					  	wind_get(WHCpu, WF_CURRXYWH, &L_inX, &L_inY, &dummy, &dummy);
  	
						SetLongProfile(G_APP_INF, G_WIND_CPU, "position_x", (long)L_inX);
						SetLongProfile(G_APP_INF, G_WIND_CPU, "position_y", (long)L_inY);

						F_Close(CPU);
						break;
						
		case	BN_32:	Save( &G_Exception );
						break;
						
	}
}

