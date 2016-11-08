/*
 * ----------------------------------------------------------------------------------------------
 *		Jaguar TOOLBOX an Application Program Interface.
 *
 *	File:		JToolBox.c
 *	Author:		FAVARD LAURENT
 *	Date:		25 March 1998
 *	Release:	17 May 2000
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
 *
 * ----------------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include	".\Toolsdef.h"
#include	"..\Include\JToolBox.h"


/* ---------------------------------------------------------------------------------------------- */

extern	void DrvInst( void );
extern	unsigned int	CurrDriver;
extern	TDriver			TabDriver[];

/* ---------------------------------------------------------------------------------------------- */

int					Debug;
TRecipient			Recipient;

/* ---------------------------------------------------------------------------------------------- */

static long			IDSempahore = C_IDSEM;
static	int			SessionOpen =	false;
static	int			UseSem		=	false;
static	void 		*OldStack;

/* ---------------------------------------------------------------------------------------------- */

static char	EText[ JTB_COUNTMSG ][ JTB_LENMSG ]=
								{   "General purpose error",
                                    "Frame received but no expected for this command",
                                    "Wrong start address",
                                    "Wrong target address",
                                    "Wrong length",
                                    "Time out error",
                                    "Bad value occured",
                                    "No header",
                                    "Error file access",
                                    "No frame available",
                                    "Checsum error",
                                    "Error while sending character",
                                    "No character available",
                                    "No CRY file",
                                    "No CCI file",
                                    "Not enought memories",
                                    "Transfer error",
                                    "BUSY line is always LOW",
                                    "BUSY line is always HIGH"
								};

/* ---------------------------------------------------------------------------------------------- */
void JTB_Version( TJTBVersion *Version )
{
	strcpy( Version->Version, G_JTOOLBOX_VERSION );
	strcpy( Version->Date, __DATE__ );
    strcpy( Version->Author, G_AUTHOR );
}
/* ---------------------------------------------------------------------------------------------- */
int JTB_Open( int aUseSem )
{
	OldStack	=	(void *)Super( 0L );

	DrvInst();						/*	must be supervisor mode */
    Debug		=	false;
	SessionOpen	=	true;
	UseSem		=	aUseSem;

	Super( OldStack );

	if( aUseSem )
	{
		if( (int)Psemaphore( SEM_CREATE, IDSempahore, 0L) != 0)
		{
			return	true;
		}
		/*	Il faut lib‚rer le s‚maphore, automatiquement appropri‚ ! */
		Psemaphore( SEM_UNLOCK, IDSempahore, -1L);
	}

	return	true;
}
/* ---------------------------------------------------------------------------------------------- */
int JTB_Exit( void )
{   
    if( UseSem )
    {
		if( Psemaphore( SEM_DESTROY, IDSempahore, 0L) != 0)
			Trace( "Unable to remove the sempahore" );
	}
	SessionOpen	=	false;
	return	true;
}
/* ---------------------------------------------------------------------------------------------- */
void SemLock( void )
/*	Set SUPER mode and if required, lock sempahore */
{
	OldStack	=	(void *)Super( 0L );

	if( UseSem )
	{
		Psemaphore( SEM_LOCK, IDSempahore, -1L);
	}
}
/* ---------------------------------------------------------------------------------------------- */
void SemUnLock( void )
/*	Set USER mode and if required, unlock sempahore */
{
	Super( OldStack );

	if( UseSem )
	{
		Psemaphore( SEM_UNLOCK, IDSempahore, -1L);
	}
}
/* ---------------------------------------------------------------------------------------------- */
void JTB_Debug( int OnOff )
{
	Debug	=	OnOff;
}
/* ---------------------------------------------------------------------------------------------- */
void JTB_ErrorText( int ErrorCode, char *ErrorText )
{
	if( ErrorCode <= 0 )
		strcpy( ErrorText, EText[ -ErrorCode ] );
}
/* ---------------------------------------------------------------------------------------------- */
int JTB_DevStatus( void )
{
	int		Ret;
	
	OldStack	=	(void *)Super( 0L );

	Ret	=	TabDriver[ CurrDriver ].Status();

	Super( OldStack );
	
	return	Ret;
}
/* ---------------------------------------------------------------------------------------------- */
void Trace( char *Message )
{
	if( Debug )
	{
		fprintf( stderr, "%s\n", Message );
	}
}
/* ----------------------------------------------------------------------------------------------*/
int Inquire( TJagId *JagIdentification )
{
	long			SPID, TPID;
    int				Opcode, Retour = JTB_OK;
    unsigned int	Lenght;
	TJagId		   	*L_JagIdentification;
    DECL_TIMEOUT;

    Trace( "JToolbox - Inquire(): command begin");

	SemLock();

    Retour	=	SendFrame( TOOLBOX_PID, KERNEL_PID, FRAME_INQUIRE, 0, NULL);
    if(  Retour <= 0 )
    {
    	Trace( "Inquire: Error while SendFrame. Command aborted" );
        return	Retour;
    }

    Trace( "JToolbox - Inquire(): trame Inquire envoyee");

    INIT_TIMEOUT;
    while( GetFrame( &SPID, &TPID, &Opcode, &Lenght, (void *)&Recipient) == JTB_ENOFRAME && diff <= C_MAX_TIMEOUT  )
    {
    	INC_TIMEOUT;
    }
    if( diff >= C_MAX_TIMEOUT )
    {
    	Trace( "JToolbox - Inquire(): timout error, no response from Jaguar");
        Retour	=	 JTB_ETIMEOUT;
    }
	else
    {
        if( Opcode == FRAME_INQUIRE )
        {
            L_JagIdentification	=	(TJagId *)&Recipient;
            *JagIdentification	=	*L_JagIdentification;

            Retour	=	JTB_OK;
        }
        else
            Retour	=	JTB_EFRAMEMISMATCH;
    }
    SemUnLock();

	return	Retour;
}
/* ----------------------------------------------------------------------------------------------*/
int MemoryGet(void *STAddress, TJagMemDesc *JagMemDescriptor)
/*
	This function permit you to do a memory transfer request as Jaguar to ATARI Host under JagOS
    control. For example, download a part of memory to cjeck it...

*/
{
	long			SPID, TPID;
    int				Opcode, Retour = JTB_OK;
    unsigned int	Lenght;
    TJagMemDesc 	*L_JagMemDescriptor;
    DECL_TIMEOUT;

	SemLock();

	if( Debug )
		fprintf( stdout, "MemoryGet(): Called to dump %ld bytes\n", JagMemDescriptor->Lenght );
		
	Retour	=	SendFrame( TOOLBOX_PID, KERNEL_PID, FRAME_MEMGET, (unsigned int)sizeof(TJagMemDesc), (void *)JagMemDescriptor);
    if( Retour <= 0 )
    {
    	Trace( "MemoryGet: Error while SendFrame. Command aborted" );
        return	Retour;
    }

    INIT_TIMEOUT;
    while( GetFrame( &SPID, &TPID, &Opcode, &Lenght, (void *)&Recipient) == JTB_ENOFRAME && diff <= C_MAX_TIMEOUT )
    {
    	INC_TIMEOUT;
    }

    if( diff >= C_MAX_TIMEOUT )
    {
    	Trace( "JToolbox - MemoryGet(): timout error, no response from Jaguar");
        Retour	=	JTB_ETIMEOUT;
    }

    if( diff >= C_MAX_TIMEOUT )
    {
    	Trace( "JToolbox - MemoryGet(): timout error, no response from Jaguar");
        Retour	=	JTB_ETIMEOUT;
    }

	if( Retour != JTB_ETIMEOUT )
    {
        if( Opcode == FRAME_MEMGET )
        {
            L_JagMemDescriptor	=	(TJagMemDesc *)&Recipient;
            if( L_JagMemDescriptor->StartAddress == JagMemDescriptor->StartAddress )
            {
                if( L_JagMemDescriptor->Lenght == JagMemDescriptor->Lenght )
                {
                    Retour	=	GetBinaries(TOOLBOX_PID, KERNEL_PID, L_JagMemDescriptor->Lenght, STAddress);
                }
                else
                    Retour	=	JTB_ELENGHT;
            }
            else
                Retour	=	JTB_ESTARTADR;
        }
        else
            Retour	=	JTB_EFRAMEMISMATCH;
	}
	SemUnLock();

	return	Retour;
}
/* ----------------------------------------------------------------------------------------------*/
int MemorySend(void *STAddress, TJagMemDesc *JagMemDescriptor)
/*
	This function permit you to do a memory transfer request as ATARI Host to Jaguar under JagOS
    control. For example, fill a part of memory, upload a program, etc...

*/
{
	long			SPID, TPID;
    int				Opcode, Retour;
    unsigned int	Lenght;
    TJagMemDesc 	*L_JagMemDescriptor;
	DECL_TIMEOUT;

	SemLock();

    Retour	=	SendFrame( TOOLBOX_PID, KERNEL_PID, FRAME_MEMSEND, (unsigned int)sizeof(TJagMemDesc), (void *)JagMemDescriptor);
    if( Retour <= 0 )
    {
    	Trace( "MemorySend: Error while SendFrame. Command aborted" );
        return	Retour;
    }

    INIT_TIMEOUT;
    while( GetFrame( &SPID, &TPID, &Opcode, &Lenght, (void *)&Recipient ) == JTB_ENOFRAME && diff <= C_MAX_TIMEOUT )
	{
    	INC_TIMEOUT;
    }

    if( diff >= C_MAX_TIMEOUT )
    {
    	Trace( "JToolbox - MemorySend(): timout error, no response from Jaguar");
        Retour	=	JTB_ETIMEOUT;
    }
	else
    {
        if( Opcode == FRAME_MEMSEND )
        {
            L_JagMemDescriptor	=	(TJagMemDesc *)&Recipient;
            if( L_JagMemDescriptor->StartAddress == JagMemDescriptor->StartAddress )
            {
                if( L_JagMemDescriptor->Lenght == JagMemDescriptor->Lenght )
                {
                    Retour	=	SendBinaries(TOOLBOX_PID, KERNEL_PID, L_JagMemDescriptor->Lenght, STAddress);
                }
                else
                    Retour	=	JTB_ELENGHT;
            }
            else
                Retour	=	JTB_ESTARTADR;
        }
        else
            Retour	=	JTB_EFRAMEMISMATCH;
    }
	SemUnLock();

	return	Retour;
}
/* ----------------------------------------------------------------------------------------------*/
int LoadAndGo(void *STAddress, TJagProgram *JagProgram)
{
	long			SPID, TPID;
    int				Opcode, Retour;
    unsigned int	Lenght;
    TJagProgram 	*L_JagProgram;
    DECL_TIMEOUT;


	SemLock();
	
    Retour	=	SendFrame( TOOLBOX_PID, KERNEL_PID, FRAME_PRGSEND, (unsigned int)sizeof(TJagProgram), (void *)JagProgram);
    if( Retour <= 0 )
    {
    	Trace( "LoadAndGo: Error while SendFrame. Command aborted" );
        return	Retour;
    }

    INIT_TIMEOUT;
	while( GetFrame( &SPID, &TPID, &Opcode, &Lenght, (void *)&Recipient) == JTB_ENOFRAME && diff <= C_MAX_TIMEOUT )
	{
    	INC_TIMEOUT;
    }
    if( diff >= C_MAX_TIMEOUT )
    {
    	Trace( "JToolbox - LoadAndGo(): timout error, no response from Jaguar");
        Retour	=	JTB_ETIMEOUT;
    }
	else
    {
        if( Opcode == FRAME_PRGSEND )
        {
            L_JagProgram	=	(TJagProgram *)&Recipient;
            if( L_JagProgram->TargetAddress == JagProgram->TargetAddress )
            {
                if( L_JagProgram->Lenght == JagProgram->Lenght )
                {
                    if( L_JagProgram->StartAddress == JagProgram->StartAddress )
                    {
                        SendBinaries(TOOLBOX_PID, KERNEL_PID, JagProgram->Lenght, STAddress);
                        Retour	=	JTB_OK;
                     }
                     else
                        Retour	=	JTB_ESTARTADR;
                }
                else
                    Retour	=	JTB_ELENGHT;
            }
            else
                Retour	=	JTB_ETARGETADR;
        }
        else
            Retour	=	JTB_EFRAMEMISMATCH;
    }
	SemUnLock();

	return	Retour;
}
/* ----------------------------------------------------------------------------------------------*/
int RunPRG(TJagProgram *JagProgram)
{
	long			SPID, TPID;
    int				Opcode, Retour;
    unsigned int	Lenght;
    DECL_TIMEOUT;

	SemLock();

    Retour	=	SendFrame( TOOLBOX_PID, KERNEL_PID, FRAME_PRGRUN, (unsigned int)sizeof(TJagProgram), (void *)JagProgram);
    if( Retour <= 0 )
    {
    	Trace( "RunPRG: Error while SendFrame. Command aborted" );
        return	Retour;
    }

    INIT_TIMEOUT;
    while( GetFrame( &SPID, &TPID, &Opcode, &Lenght, (void *)&Recipient) == JTB_ENOFRAME && diff <= C_MAX_TIMEOUT )
	{
    	INC_TIMEOUT;;
    }

    if( diff >= C_MAX_TIMEOUT )
    {
    	Trace( "JToolbox - RunPRG(): timout error, no response from Jaguar");
        Retour	=	JTB_ETIMEOUT;
    }
	else
	    Retour	=	( Opcode == FRAME_PRGRUN ) ? JTB_OK : JTB_EFRAMEMISMATCH;

	SemUnLock();

	return	Retour;
}
/* ----------------------------------------------------------------------------------------------*/
int MemSend(int Accept, void *STBuffer, TJagMemDesc *JagMemDescriptor)
/*
	This function is used when you want to send a response to the Jaguar which did
    a transfer request for ATARI to Jaguar.

	Transfer asked by the Jaguar:	ATARI Host to Jaguar
    We can accept or refuse this one.	Accept = True  => Transfer accepted
										Accept = False => Transfer refused
*/
{
	int		Retour;

	SemLock();

	if( Accept )
    {
    	Retour	=	SendFrame( TOOLBOX_PID, KERNEL_PID, FRAME_ACK, 0, NULL);
		if( Retour <= 0 )
        {
            Trace( "MemSend: Error while SendFrame for FRAME_ACK. Command aborted" );
            return	Retour;
        }
		Retour	=	SendBinaries( TOOLBOX_PID, KERNEL_PID, JagMemDescriptor->Lenght, STBuffer );
        if( Retour <= 0 )
        {
            Trace( "MemSend: Error while SendBinaries. Command aborted" );
            return	Retour;
        }
    }
    else
    {
    	Retour	=	SendFrame( TOOLBOX_PID, KERNEL_PID, FRAME_NACK, 0, NULL);
		if( Retour <= 0 )
        {
            Trace( "MemSend: Error while SendFrame for FRAME_NACK. Command aborted" );
            return	Retour;
        }
    }
	SemUnLock();

	return	Retour;
}
/* ----------------------------------------------------------------------------------------------*/
int MemGet(int Accept, void *STBuffer, TJagMemDesc *JagMemDescriptor)
/*
	This function is used when you want to send a response to the Jaguar which did
    a transfer request for Jaguar to ATARI.

	Transfer asked by the Jaguar:	Jaguar to ATARI Host
    We can accept or refuse this one.	Accept = True  => Transfer accepted
										Accept = False => Transfer refused
*/
{
	int		Retour;

	SemLock();

	if( Accept )
    {
    	Retour	=	SendFrame( TOOLBOX_PID, KERNEL_PID, FRAME_ACK, 0, NULL);
		if(  Retour <= 0 )
        {
            Trace( "MemGet: Error while execute SendFrame for FRAME_ACK. Command aborted" );
            return	Retour;
        }
		Retour	=	GetBinaries(TOOLBOX_PID, KERNEL_PID, JagMemDescriptor->Lenght, STBuffer);
        if(  Retour <= 0 )
        {
            Trace( "MemGet: Error while execute GetBinaries. Command aborted" );
            return	Retour;
        }
    }
    else
    {
    	Retour	=	SendFrame( TOOLBOX_PID, KERNEL_PID, FRAME_NACK, 0, NULL);
        if(  Retour <= 0 )
        {
            Trace( "MemGet: Error while execute SendFrame fro FRAME_NACK. Command aborted" );
            return	Retour;
        }
    }

	SemUnLock();

	return	JTB_OK;
}

