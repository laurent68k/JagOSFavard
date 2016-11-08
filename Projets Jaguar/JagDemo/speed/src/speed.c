/*
		demo of how to use the Jaguar ToolBox.
		in this example we just perform a multiple memory transfer to show
		the speed rate.

		the transfer are perform between this program and JagOS.

		28/29 September 1998 - Version 1.0
		Copyright Atari Funship (c) 1998
*/

#include <stdio.h>
#include <stdlib.h>
#include <tos.h>
#include <string.h>
#include <float.h>
#include <time.h>

#include <PcAads.h>
#include <JToolBox.h>

/*	---------------------------------------------------------------------------------------	*/

#include "Speed__F.h"

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
					{B1KB	, WindAction, NULL},
					{B10KB	, WindAction, NULL},
					{B50KB	, WindAction, NULL},
					{ST2JAG	, WindAction, NULL},
					{JAG2ST	, WindAction, NULL},

					FORM_END
				};

static Type_Form_List
AboutListe[]=	{
					{0	, NULL, NULL},
					FORM_END
				};

/*	---------------------------------------------------------------------------------------	*/

static	int				Sortir;
int						WHandle1;
OBJECT					*TreeObject;

#define					G_LAPS_TIME		1000L
#define					BASE10			10

#define					SPEED_1KB		1 * 1024L
#define					SPEED_10KB		10 * 1024L
#define					SPEED_50KB      50 * 1024L

/*
 * ---------------------------------------------------------------------------------------
 *							From Code.c
 */

extern	int doJAG2ST( time_t *StartTime, time_t *EndTime );
extern	int doST2JAG( time_t *StartTime, time_t *EndTime );

unsigned long		SizeSelected;

/*
 * ---------------------------------------------------------------------------------------
 *							Buttons's actions
 */

void Display( unsigned long SizeSelected, time_t StartTime, time_t EndTime )
{
	char			chTexte1[10] = "0000";
	char			chTexte2[10] = "0000";
    double			Number;
    unsigned int	Rate;

    Number			=	difftime( EndTime, StartTime );
	Rate			=	(unsigned int)( (double)SizeSelected / Number);

    sprintf( chTexte1, "%d", (unsigned int)Number );
    sprintf( chTexte2, "%d", Rate );
	
	if( SizeSelected == SPEED_1KB )
    {
    	F_WriteText( FMAIN, SEC1, chTexte1);
    	F_WriteText( FMAIN, RATE1, chTexte2);
    }
    else if( SizeSelected == SPEED_10KB )
    {
    	F_WriteText( FMAIN, SEC2, chTexte1);
    	F_WriteText( FMAIN, RATE2, chTexte2);
    }
    else
    {
    	F_WriteText( FMAIN, SEC3, chTexte1);
    	F_WriteText( FMAIN, RATE3, chTexte2);
    }
}

void WindAction(void)
{
	time_t	StartTime, EndTime;

	switch( F_NameObject)
	{
		/*	Speed at 64 Kb of datas */
		case	B1KB:  	SizeSelected	=	SPEED_1KB;
						break;

		/*	Speed at 500 Kb of datas */
		case	B10KB:	SizeSelected	=	SPEED_10KB;
						break;

		/*	Speed at 1Mb of datas */
		case	B50KB:	SizeSelected	=	SPEED_50KB;
						break;

		/*	Transfer ATARI to Jag */
		case	ST2JAG: Display( SizeSelected, 0, 0 );
						if( doST2JAG( &StartTime, &EndTime ) )
	        				Display( SizeSelected, StartTime, EndTime );
        				break;

		/*	Transfer Jag to Atari */
		case	JAG2ST: Display( SizeSelected, 0, 0 );
						if( doJAG2ST( &StartTime, &EndTime ) )
	        				Display( SizeSelected, StartTime, EndTime );
        				break;
		/*	to exit */
		case	0:		Sortir = TRUE;
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
					    	F_Open( FABOUT, "About...", F_WIND, NOTEXT, AboutListe );
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
		case	FRAME_MEMSEND:		form_alert( 1, "[0][ Jaguar ask datas: | JAGUAR --> ATARI ][ Ok ]");
									break;

		/*	Jaguar ask memory tranfser as: ATARI to JAGUAR */
		case	FRAME_MEMGET:		form_alert( 1, "[0][ Jaguar ask datas: | ATARI --> JAGUAR ][ Ok ]");
                                    break;
		
		default:					form_alert( 1, "[0][ Unexpected frame received ][ Ok ]");
									break;
	}
}

/*	---------------------------------------------------------------------------------------	*/

int main(void)
{
	Type_Formular_Parameter	*Formulaire_Class;

	if(!A_Open("Speed__F.Rsc"))
	{
    	form_alert(1,"[1][Speed__F.rsc error: | Unable to found it ][ Cancel ]");
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
	Formulaire_Class->Title		= "Jaguar Speed Rate benchmark";		/* Formular's title */
	Formulaire_Class->Mode		= F_WIND;					/* Opening mode */
	Formulaire_Class->FirstEdit	= NOTEXT;					/* First Editable Text */
	Formulaire_Class->List		= MainListe;				/* Event list */
	Formulaire_Class->FormIcon	= APPICON;					/* window's Icon  */
	Formulaire_Class->Mouse		= ARROW;
	WHandle1 = F_XOpen(Formulaire_Class);

	rsrc_gaddr( R_TREE, FMAIN, &TreeObject);
	
	W_MenuOpen(WHandle1, MAINMENU , WMenu1);
	W_SetIconApp(APPICON);

	JTB_Open();
	SetPort( PCENTRONICS );
  
	F_Selected( FMAIN, B1KB, TRUE );
	SizeSelected	=	SPEED_1KB;
	
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

