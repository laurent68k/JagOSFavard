/*
 * ----------------------------------------------------------------------------------------------
 *		JAGUAR TOOLBOX APPLICATION PROGRAM INTERFACE
 *		FOR ALL TOS ATARI AND COMPATIBLE
 *
 *	File:		HookJag.c
 *	Author:		FAVARD LAURENT, FunShip(c)
 *	Date:		25 March 1998
 *	Release:	27 April 2000
 *
 *	VERSION : 1.0
 *
 *	Now use its own TRecipient to check if a command is comming from Jaguar
 * ----------------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tos.h>
#include <aes.h>

#include	"..\Include\JToolBox.h"
#include	"toolsdef.h"

static	TRecipient			Recipient;
extern	long				IDSempahore;

/*
 * ----------------------------------------------------------------------------------------------
 */

void HookJaguarMessage( void (*UserFonction)(int FrameCode, long SPID, long TPID, int LenghtDatas, TRecipient *Recipient) )
/*
	This fonction check if a frame is available and in this case, call the user function given in parameters.
*/
{
	long			SPID, TPID;
    int				Code;
    unsigned int	Lenght;
	char			Message[100];
	
	SemLock();
	if( GetFrame( &SPID, &TPID, &Code, &Lenght, (void *)&Recipient) == JTB_OK )
    {
		switch( Code )
        {
			case	FRAME_USER1:
			case	FRAME_EXCEPTION:
			case	FRAME_DBMSG:
			case	FRAME_MEMSEND:
			case	FRAME_MEMGET:   (*UserFonction)( Code, SPID, TPID, Lenght, &Recipient );
                                    break;

			default:				sprintf( Message, "JToolBox Unexpected Frame received Code : 0x%X", Code);
									Trace( Message );
									break;

		}
	}
	SemUnLock();
}

