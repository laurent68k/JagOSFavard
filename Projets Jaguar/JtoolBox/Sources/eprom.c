/*
 * ----------------------------------------------------------------------------------------------
 *		JAGUAR TOOLBOX APPLICATION PROGRAM INTERFACE
 *		FOR ALL TOS ATARI AND COMPATIBLE
 *
 *	File:		Eeprom.c
 *	Author:		FAVARD LAURENT
 *	Date:		25 March 1998
 *	Release:	17 April 2000
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * ----------------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tos.h>
#include <aes.h>

#include	".\Toolsdef.h"
#include	"..\Include\JToolBox.h"

/* ---------------------------------------------------------------------------------------------- */

extern	TRecipient		Recipient;
extern	long			IDSempahore;

/* ---------------------------------------------------------------------------------------------- */
int GetEEPROM( char *chEEPROM )
/*
*/
{
	long			SPID, TPID;
    int				Opcode, Retour;
    unsigned int	Lenght;
    DECL_TIMEOUT;

	SemLock();

    SendFrame( TOOLBOX_PID, KERNEL_PID, FRAME_EEPROMGET,0 , NULL);
    INIT_TIMEOUT;
    while( GetFrame( &SPID, &TPID, &Opcode, &Lenght, (void *)&Recipient) ==JTB_ENOFRAME && diff <= C_MAX_TIMEOUT )
    {
    	INC_TIMEOUT;
    }

    /*	V‚rifier si un timeout n'est pas survenu	*/
    if( diff > C_MAX_TIMEOUT)
    {
        Trace( "JToolbox - GetEEPROM(): timeout, no response from Jaguar");
        Retour	=	JTB_ETIMEOUT;
    }
	else
    {
        if( Opcode == FRAME_EEPROMGET )
        {
            memcpy( chEEPROM, (char *)&Recipient, SIZE_SERIAL_EEPROM);
            Retour	=	JTB_OK;
        }
        else
            Retour	=	JTB_EFRAMEMISMATCH;
    }
	SemUnLock();

	return	Retour;
}
/* ----------------------------------------------------------------------------------------------*/
int SetEEPROM( char *chEEPROM )
/*
*/
{
	long			SPID, TPID;
    int				Opcode, Retour;
    unsigned int	Lenght;
	DECL_TIMEOUT;

	SemLock();

    SendFrame( TOOLBOX_PID, KERNEL_PID, FRAME_EEPROMSET, SIZE_SERIAL_EEPROM, chEEPROM );
    INIT_TIMEOUT;
    while( GetFrame( &SPID, &TPID, &Opcode, &Lenght, (void *)&Recipient) == JTB_ENOFRAME && diff <= C_MAX_TIMEOUT )
	{
		INC_TIMEOUT;
    }

    /*	Vérifier si un timeout n'est pas survenu	*/
    if( diff > C_MAX_TIMEOUT)
    {
        Trace( "JToolbox - SetEEPROM(): timeout, no response from Jaguar");
        Retour	=	JTB_ETIMEOUT;
    }
	else
    {
	    Retour	=	( Opcode == FRAME_EEPROMSET ) ? JTB_OK : JTB_EFRAMEMISMATCH;
    }
	SemUnLock();

	return	Retour;
}
/* ----------------------------------------------------------------------------------------------*/

