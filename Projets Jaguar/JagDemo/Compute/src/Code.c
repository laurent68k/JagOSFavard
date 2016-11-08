 /*
		Programme de Demonstration de la Jaguar en calculateur.
		
		23-25 September 1998 - Version 1.0
		Copyright Atari Funship (c) 1998
*/

#include <stdio.h>
#include <stdlib.h>
#include <tos.h>
#include <string.h>

#include <Pcaads.h>
#include <JToolBox.h>

/*	---------------------------------------------------------------------------------------	*/

#include "ComputeF.h"

/*	---------------------------------------------------------------------------------------	*/

extern	void C_Start( void );
extern	unsigned long	C_Len;

/*	---------------------------------------------------------------------------------------	*/

#define		CMD_END		0x0000
#define		CMD_ADD		0x0001
#define		CMD_SUB		0x0002

int			G_WaitParam	=	FALSE;					/*	Jaguar is awaiting datas */
TJagMemDesc *G_JagMemParam;

/*	---------------------------------------------------------------------------------------	*/
void LoadCode( void )
/*
	Upload a jaguar program.
*/
{
	TJagProgram 	JagProgram;

	strcpy(JagProgram.Name, "Compute");
	JagProgram.TargetAddress	=	(void *)(0x5000L);
	JagProgram.StartAddress		=	(void *)(0x5000L);
	JagProgram.Lenght			=	C_Len;


	graf_mouse(BUSYBEE, NULL);
	LoadAndGo(C_Start, &JagProgram);
	graf_mouse(ARROW, NULL);
}

/*	---------------------------------------------------------------------------------------	*/
void StopCode( void )
/*
	Send a "special" datas blocks where the JAGEND code is stored.
	This will stop the Jaguar program running
*/
{
    int				STBuffer[3] = { CMD_END, 0, 0 };

	graf_mouse(BUSYBEE, NULL);

    if( G_WaitParam )
    {
/*		SendFrame( TOOLBOX_PID, KERNEL_PID, FRAME_ACK, 0, NULL);
		SendBinaries(TOOLBOX_PID, KERNEL_PID, G_JagMemParam->Lenght, (char *)STBuffer);*/
        MemSend( TRUE, STBuffer, G_JagMemParam );
	}
	else
    	form_alert(1, "[1][ Jaguar isn't awaiting parameters ][ok]");

	graf_mouse(ARROW, NULL);
}

/*	---------------------------------------------------------------------------------------	*/
void ParamAdd( int ParamX, int ParamY )
/*
	Send a datas blocks where are stored the Command and the parameters
	to process.
*/
{
    int            STBuffer[3] = { CMD_ADD, 0, 0 };

    if( G_WaitParam )
    {
 /*		graf_mouse(BUSYBEE, NULL);*/

		STBuffer[1]	=	ParamX;
		STBuffer[2]	=	ParamY;

/*		SendFrame( TOOLBOX_PID, KERNEL_PID, FRAME_ACK, 0, NULL);
		SendBinaries(TOOLBOX_PID, KERNEL_PID, G_JagMemParam->Lenght, (char *)STBuffer);

		graf_mouse(ARROW, NULL);*/

        MemSend( TRUE, STBuffer, G_JagMemParam );
        G_WaitParam	=	FALSE;
    }
    else
    	form_alert(1, "[1][ Jaguar isn't awaiting parameters ][ok]");
}

/*	---------------------------------------------------------------------------------------	*/
void ParamSub( int ParamX, int ParamY )
/*
	Send a datas blocks where are stored the Command and the parameters
	to process.
*/
{
    int            STBuffer[3] = { CMD_SUB, 0, 0 };

    if( G_WaitParam )
    {
/*		graf_mouse(BUSYBEE, NULL);*/

		STBuffer[1]	=	ParamX;
		STBuffer[2]	=	ParamY;

/*		SendFrame( TOOLBOX_PID, KERNEL_PID, FRAME_ACK, 0, NULL);
		SendBinaries(TOOLBOX_PID, KERNEL_PID, G_JagMemParam->Lenght, (char *)STBuffer);

		graf_mouse(ARROW, NULL);                 */

        MemSend( TRUE, STBuffer, G_JagMemParam );
        G_WaitParam	=	FALSE;
    }
    else
    	form_alert(1, "[1][ Jaguar isn't awaiting parameters ][ok]");
}

/*	---------------------------------------------------------------------------------------	*/
void Display( TJagMemDesc *JagMemDescriptor )
{
    int          	STBuffer[3];
	char			chTexte[100];
	
	graf_mouse(BUSYBEE, NULL);

/*	SendFrame( TOOLBOX_PID, KERNEL_PID, FRAME_ACK, 0, NULL);
	GetBinaries(TOOLBOX_PID, KERNEL_PID, JagMemDescriptor->Lenght, (char *)STBuffer);*/

    MemGet( TRUE, STBuffer, JagMemDescriptor);

	sprintf(chTexte, "%d", STBuffer[1]);
	F_WriteText( FMAIN, RESULT, chTexte);
	
/*	graf_mouse(ARROW, NULL); */
}


