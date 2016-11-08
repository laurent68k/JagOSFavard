/*
		demo of how to use the Jaguar ToolBox.
		in this example we just perform a simple addition and substraction
		
		
		23-28 September 1998 - Version 1.0
		Copyright Atari Funship (c) 1998
*/

#include <stdio.h>
#include <stdlib.h>
#include <tos.h>
#include <string.h>


#include <PcAads.h>
#include <JToolBox.h>

/*	---------------------------------------------------------------------------------------	*/

#include "ComputeF.h"

/*	---------------------------------------------------------------------------------------	*/
void MenuAction(int WindowHandle);

static Type_Window_Menu_List
WMenu1[]=		{
					{ABOUT	 , INITIAL, MenuAction},
					{QUITTER , INITIAL, MenuAction},
					WMENU_END
				};

void WindAction(void);

static Type_Form_List
MainListe[]=	{
					{0		, WindAction, NULL},
					{LD		, WindAction, NULL},
					{ED		, WindAction, NULL},
					{ADD	, WindAction, NULL},
					{SUB	, WindAction, NULL},
					FORM_END
				};

static Type_Form_List
AboutListe[]=	{
					{0	, NULL, NULL},
					FORM_END
				};

/*	---------------------------------------------------------------------------------------	*/

static	int				Sortir;

OBJECT					*TreeObject,
						*mnInstance;
int						WHandle1;

#define					G_LAPS_TIME		1000L
#define					BASE10			10

/*
 * ---------------------------------------------------------------------------------------
 *							From Code.c
 */

extern	void LoadCode( void );
extern	void StopCode( void );
extern	void ParamAdd( int ParamX, int ParamY );
extern	void ParamSub( int ParamX, int ParamY );
extern	void Display( TJagMemDesc *JagMemDescriptor );

extern	int			G_WaitParam;
extern	TJagMemDesc *G_JagMemParam;

/*
 * ---------------------------------------------------------------------------------------
 *							Buttons's actions
 */

void WindAction(void)
{
	char		chTexte[3];
	char		*chStopper;
	int			ParamX, ParamY;

	switch( F_NameObject)
	{
		/*	Upload a jag program */
		case	LD:		LoadCode( );

						TreeObject[LD].ob_state	|= DISABLED;	
						TreeObject[ED].ob_state	&= ~DISABLED;	
						TreeObject[ADD].ob_state	&= ~DISABLED;	
						TreeObject[SUB].ob_state	&= ~DISABLED;	
						F_RedrawObject( TreeObject, LD );
						F_RedrawObject( TreeObject, ED );
						F_RedrawObject( TreeObject, ADD );
						F_RedrawObject( TreeObject, SUB );
						break;

		/*	Terminate a jag program */
		case	ED:		StopCode();

						TreeObject[LD].ob_state	&= ~DISABLED;	
						TreeObject[ED].ob_state	|= DISABLED;	
						TreeObject[ADD].ob_state	|= DISABLED;	
						TreeObject[SUB].ob_state	|= DISABLED;	
						F_RedrawObject( TreeObject, LD );
						F_RedrawObject( TreeObject, ED );
						F_RedrawObject( TreeObject, ADD );
						F_RedrawObject( TreeObject, SUB );
						break;

		/*	Send datas to add */
		case	ADD:	
						F_ReadText(FMAIN, PARAMX, chTexte);
						ParamX	=	(int)strtol(chTexte, &chStopper, BASE10); 
						F_ReadText(FMAIN, PARAMY, chTexte);
						ParamY	=	(int)strtol(chTexte, &chStopper, BASE10); 

						ParamAdd( ParamX, ParamY );
						break;

		/*	Send datas to sub */
		case	SUB:	
						F_ReadText(FMAIN, PARAMX, chTexte);
						ParamX	=	(int)strtol(chTexte, &chStopper, BASE10); 
						F_ReadText(FMAIN, PARAMY, chTexte);
						ParamY	=	(int)strtol(chTexte, &chStopper, BASE10); 

						ParamSub( ParamX, ParamY );
						break;

		/*	Exit button */
		case	0:		Sortir = TRUE;
						break;
		default:
		    		   	form_alert(1,"[0][ unknow object ][ok]");
    					break;
	}
}

/*
 * ---------------------------------------------------------------------------------------
 *								Process menu command
 */

void MenuAction(int WindowHandle)
{
   	switch(W_MenuItem)
    {
		    case	QUITTER:
			    			Sortir = TRUE;
	    					break;

		    case	ABOUT:
					    	F_Open( FABOUT, "A propos...", F_WIND, NOTEXT, AboutListe );
    						break;

		    default:
			    		   	form_alert(1,"[0][ Item de menu inconnu ][ok]");
    						break;
		}
}

/*
 * ---------------------------------------------------------------------------------------
 *				
 */

void HandlerMessage(int FrameCode, long SourcePID, long TargetPID, int LenghtData, TRecipient *Recipient)
/*
	This is the "Message Jaguar" handler. When JagOS send a request, this procedure will handle it and
	process what is need.
*/
{
	char					*chString;
	char					chTexte[100];
	TJagMemDesc				*JagMem;


	switch( FrameCode )
    {
    	/*	User 1 Frame code */
    	case	FRAME_USER1:		form_alert( 1, "[1][ User frame 1 received | I will ignore it ][ Ok ]");
    								break;
    	
    	/*	Exception 680x0 JagOS */
    	case	FRAME_EXCEPTION:	form_alert( 1, "[1][ JAGUAR EXCEPTION 680x0 | You must reboot your Jaguar ][ Ok ]");
    								break;

    	/*	Debug message */
		case	FRAME_DBMSG:		chString =	(char *)Recipient;
									sprintf(chTexte, "[0][%s][Ok]", chString);
									form_alert( 1, chTexte);
									break;

		/*	Jaguar ask memory tranfser as: JAGUAR to ATARI */
		case	FRAME_MEMSEND:		JagMem	=	(TJagMemDesc *)Recipient;
									Display( JagMem );
									break;

		/*	Jaguar ask memory tranfser as: ATARI to JAGUAR */
		case	FRAME_MEMGET:		G_WaitParam	=	TRUE;
        							G_JagMemParam	=	(TJagMemDesc *)Recipient;
									form_alert( 1, "[0][ Jaguar ask datas | ATARI --> JAGUAR ][ Ok ]");
                                    break;

		default:					form_alert( 1, "[0][ Unexpected frame received ][ Ok ]");
									break;
									
	}
}

/*	---------------------------------------------------------------------------------------	*/

int main(void)
{
	Type_Formular_Parameter	*Formulaire_Class;
  
	if(!A_Open("ComputeF.Rsc"))
	{
    	form_alert(1,"[1][ComputeF.rsc error: | Unable to found it ][ Cancel ]");
    	A_Close();
    	return(-1);												/* Quit immediatly */
	}

	W_Init();													/* Init Window  */
	F_Init();													/* Init Form    */
	M_Init();
  
	/*
	 *	Ouverture des formulaires et fenetres
 	 */
	Formulaire_Class 			= F_GetParameter();

	Formulaire_Class->TreeName	= FMAIN;
	Formulaire_Class->Title		= "Jaguar Calculator";		/* Formular's title */
	Formulaire_Class->Mode		= F_WIND;					/* Opening mode */
	Formulaire_Class->FirstEdit	= PARAMX;					/* First Editable Text */
	Formulaire_Class->List		= MainListe;				/* Event list */
	Formulaire_Class->FormIcon	= W1ICON;					/* window's Icon  */
	Formulaire_Class->Mouse		= ARROW;
	WHandle1 = F_XOpen(Formulaire_Class);
  
	rsrc_gaddr( R_TREE, FMAIN, &TreeObject);
	
	mnInstance	=	W_MenuOpen(WHandle1, MAINMENU , WMenu1);
	W_SetIconApp(APPICON);

	JTB_Open();
	SetPort( PCENTRONICS );
  
	Sortir = FALSE;
	do
	{
	    A_WaitEvent();
	    W_Event();
	    M_Event();
    	F_Event();				
    	
    	HookJaguarMessage( HandlerMessage );
	}
	while(!Sortir);

	JTB_Exit();
	
	free(Formulaire_Class);
	W_MenuClose(WHandle1);
	F_Close(FMAIN);
  
	W_Exit();
	F_Exit();
	M_Exit();
  
	A_Close();							/* Close Application	*/
	return(0);							/* for the shell 	*/
}

