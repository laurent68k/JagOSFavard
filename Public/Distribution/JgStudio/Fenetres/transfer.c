/*
 * ----------------------------------------------------------------------------------------------
 *
 *
 * Author  : 	FAVARD Laurent, FunShip (c)
 *	File    : 	Transfer.C
 *	Date    : 	12 June 1998
 *	Release : 	30 September 1998
 *	Version : 	1.0
 *	Country : 	FRANCE
 *			
 *
 *								- Copyright Atari FunShip (c) 1994-97 -
 *						- Atari ST, STE, TT, Falcon, C-LAB, Medusa, Hades, Milan -
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

#include	"..\modules\tools.h"
#include	"..\modules\messages.h"
#include	"..\modules\Bulles.h"
#include	"..\modules\const.h"
#include	"..\modules\jaguar.h"
#include	"..\modules\mainmenu.h"

#include	"..\fenetres\console.h"

/* ----------------------------------------------------------------------------------------------*/

void	Action(void);
void	MouseEnter(void);

static Type_Form_List
FormAction[]=	{

					{BN_23,	Action,	NULL, NULL, NULL, MouseEnter, NULL},							/* Bouton ACCEPT */
					{BN_24,	Action,	NULL, NULL, NULL, MouseEnter, NULL},							/* Bouton DENY */
					{HELP4,	Action,	NULL},																	/* Bouton HELP */
					FORM_END
				};

/* ----------------------------------------------------------------------------------------------*/

static	int						WHTransfert;
static	TYPE_TRANSFERT_PARAM 	G_ParamTransfert;

/* ----------------------------------------------------------------------------------------------*/
void OpenTransfert(TYPE_TRANSFERT_PARAM *ParamTransfert)
/*
	Ouvre la fenetre de configuration des paramŠtres m‚moire jaguar
	
*/
{
	Type_Formular_Parameter	*Parameter;
	char					L_texte[20];
  
	G_ParamTransfert	=	*ParamTransfert;

	Parameter = F_GetParameter();
	if(Parameter != NULL)
	{
		if( G_ParamTransfert.Mode == FRAME_MEMSEND)
			Parameter->Title		=	G_TITLE_TRANSFERT1;											/* Titre de la fenetre */
		else
			Parameter->Title		=	G_TITLE_TRANSFERT2;											/* Titre de la fenetre */

		Parameter->TreeName =	TRANSFER;															/* Indice de l'arbre de l'objet */
		Parameter->Mode		=	F_WIND;																/* Opening mode */
		Parameter->FirstEdit=	E_ADR2;																/* First Editable Text */
		Parameter->List		=	FormAction;															/* Event list */
		Parameter->FormIcon =	IAPPLI;																		/* Icone d'iconification */
		Parameter->Type		=	MODAL;
		Parameter->Mouse 	=	ARROW;			
		if(!G_ResolutionChanged)
		{
			Parameter->x	=	(int)GetLongProfile(G_APP_INF, G_WIND_TRANSFER, "position_x", 0);
			Parameter->y	=	(int)GetLongProfile(G_APP_INF, G_WIND_TRANSFER, "position_y", 0);
		}

		if((WHTransfert = F_XOpen(Parameter)) != FALSE)
		{
			if( G_ParamTransfert.Mode == FRAME_MEMSEND)
				F_WriteText(TRANSFER, COMMENT, G_JAG2ST);
			else
				F_WriteText(TRANSFER, COMMENT, G_ST2JAG);

			sprintf(L_texte, "%lX", (unsigned long)G_ParamTransfert.SourcePID);
			F_WriteText(TRANSFER, SPID, L_texte);

			sprintf(L_texte, "%lX", (unsigned long)G_ParamTransfert.TargetPID);
			F_WriteText(TRANSFER, TPID, L_texte);

			sprintf(L_texte, "%ld", (unsigned long)G_ParamTransfert.ExpectedLen);
			F_WriteText(TRANSFER, E_LEN2, L_texte);
					
			sprintf(L_texte, "%lX", G_ParamTransfert.StartAdr);	
			F_WriteText(TRANSFER, E_ADR2, L_texte);
		}
		free(Parameter);
	}
}
/* ----------------------------------------------------------------------------------------------*/
static void MouseEnter(void)
/*

*/
{
	if( !G_BulleAide )
		return;

	switch( F_NameObject )
	{
		/*	Button ACCEPT */
		case	BN_23:	if(G_ParamTransfert.Mode == FRAME_MEMSEND )
							STG_Bubble( BUBBLE13, -1, -1);
						else
							STG_Bubble( BUBBLE14, -1, -1);
						break;

		/*	Button Deny */
		case	BN_24: 	STG_Bubble( BUBBLE15, -1, -1);
							break;
	}
}
/* ----------------------------------------------------------------------------------------------*/
static void Action(void)
/*
	Suivant le bouton cliqu‚ dans la fenetre de fin d'application, assigne la variable 'Sortir'.
*/
{
	int				X, Y, dummy, Ret;
	void			*ReceiveBuffer;
	char			Path[ PATH_LEN ]					= "\0"; 						/* Chemin seulement */
	char			PathName[ PATH_LEN+FILENAME_LEN ]	= "\0"; 						/* Chemin + Nom de fichier */
	char			FileName[ FILENAME_LEN ]			= "datas.bin\0"; 						/* Nom de fichier seulement */
	char			Extension[ EXT_LEN ]				= "*.bin\0";					/* Extension de fichiers */
	int				inFichier;
	TJagMemDesc 	JagMemDescriptor;

	switch( F_NameObject )
	{
		/*	Button ACCEPT */
		case	BN_23:	ReceiveBuffer	=	malloc( G_ParamTransfert.ExpectedLen );
		
						/*	if malloc failed, we cancel this operation */
						if( ReceiveBuffer == NULL )
						{	
							form_alert(1, ERROR_MEMTRANSF);
							graf_mouse(BUSYBEE, NULL);
							
							/*	To cancel i can call MemGet or MemSend...*/
							Ret	=	MemGet( FALSE, NULL, NULL);
							if( Ret != JTB_OK )
		    				{
    							DispErr( Ret );
								return;
    						}
    							
							graf_mouse(ARROW, NULL);
						}
						else
						{
							/*	Transfert JAGUAR vers ATARI	*/
							if( G_ParamTransfert.Mode == FRAME_MEMSEND )
							{
								graf_mouse(BUSYBEE, NULL);
								

								JagMemDescriptor.Lenght	=	G_ParamTransfert.ExpectedLen;
								Ret	=	MemGet( TRUE, ReceiveBuffer, &JagMemDescriptor);
								graf_mouse(ARROW, NULL);
								if( Ret != JTB_OK )
		    					{
    								DispErr( Ret );
									return;
    							}
								
								/*	Sauvegarde des donnees */
								if(A_FileSelector( PathName, FileName, Extension, Path, G_SAVE_BIN)) 
								{
									inFichier	=	creat( FileName );
									if( inFichier > 0 )
									{
										graf_mouse(BUSYBEE, NULL);
										write( inFichier, ReceiveBuffer, G_ParamTransfert.ExpectedLen );
										close( inFichier );
										graf_mouse(ARROW, NULL);
									}
									else
									{
										form_alert(1, ERROR_FILECREATE );
										return;
									}
								}	
								else
								{
									return;
								}
							}
							/*	Transfert ATARI vers JAGUAR	*/
							else
							{
								/*	Lecture des donnees */
								if( A_FileSelector( PathName, FileName, Extension, Path, G_SAVE_BIN) ) 
								{									
									inFichier	=	open( FileName, O_RDONLY );
									if( inFichier > 0 )
									{
										graf_mouse(BUSYBEE, NULL);
										read( inFichier, ReceiveBuffer, G_ParamTransfert.ExpectedLen );
										close( inFichier );
										graf_mouse(ARROW, NULL);
									}
									else
									{
										form_alert(1, ERROR_FILEOPEN );
										MemSend( FALSE, NULL, NULL);
										return;
									}
								}	
								else
								{
									MemSend( FALSE, NULL, NULL);
									return;
								}

								graf_mouse(BUSYBEE, NULL);
								JagMemDescriptor.Lenght	=	G_ParamTransfert.ExpectedLen;	
								Ret	=	MemSend( TRUE, ReceiveBuffer, &JagMemDescriptor);
								if( Ret != JTB_OK )
		    					{
    								DispErr( Ret );
									return;
    							}
								graf_mouse(ARROW, NULL);
							}
							free( ReceiveBuffer );
						}
					
						wind_get(WHTransfert, WF_CURRXYWH, &X, &Y, &dummy, &dummy);
  	
						SetLongProfile(G_APP_INF, G_WIND_TRANSFER, "position_x", (long)X);
						SetLongProfile(G_APP_INF, G_WIND_TRANSFER, "position_y", (long)Y);

						F_Close( TRANSFER );
						break;

		/*	Button Deny */
		case	BN_24: 	graf_mouse(BUSYBEE, NULL);
						SendFrame( TOOLBOX_PID, KERNEL_PID, FRAME_NACK, 0, NULL);								
						
						wind_get(WHTransfert, WF_CURRXYWH, &X, &Y, &dummy, &dummy);
  	
						SetLongProfile(G_APP_INF, G_WIND_TRANSFER, "position_x", (long)X);
						SetLongProfile(G_APP_INF, G_WIND_TRANSFER, "position_y", (long)Y);

						F_Close( TRANSFER );
						graf_mouse(ARROW, NULL);
						break;
						
		/*	Button HELP */
		case	HELP4:
						switch( G_ParamTransfert.Mode )
						{
							/*	Transfert JAGUAR vers ATARI	*/
							case	FRAME_MEMSEND:	STG_Open(G_HelpFile, SECTION_HELP_TRANSFER1);	
													break;

							/*	Transfert ATARI vers JAGUAR	*/
							default:				STG_Open(G_HelpFile, SECTION_HELP_TRANSFER2);	
						}
						break;

		default:		break;
	}						
}
