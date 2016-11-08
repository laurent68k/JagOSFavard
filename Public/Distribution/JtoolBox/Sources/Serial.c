/*	------------------------------------------------------------------------------
 *	Project JAGUAR:	JAGUAR PROTOCOLE COMMUNICATIONS.
 *	LOW LEVEL CODE FOR TOS MACHINE FOR SERIAL PORT.
 *
 *	File:		Serial.s
 *	Author:		FAVARD Laurent
 *	Date:		26 March 1999
 *	Release:	26 March 1999
 *	Language:	HISOFT DEVPAC 3-680x0
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
 *	------------------------------------------------------------------------------
 */

#include	<stdio.h>
#include	<ext.h>

#include	"..\Include\JToolbox.h"
#include	".\Toolsdef.h"

#define	DEV_AUX		1

/*	---------------------------------------------------------------------------*/
void InitRS( void )
{
	Trace( "InitRS: Initialisation serial port\n");
}
/*	---------------------------------------------------------------------------*/
void StatRS( void )
{
	Trace( "StatRS: called\n");
}
/*	---------------------------------------------------------------------------*/
signed int GetRS( void )
{
	if( Bconstat( DEV_AUX ) == -1 )
    {
    	return	(signed int)Bconin( DEV_AUX ) & 0xFF;
    }

    return	JTB_EGETCHAR;
}
/*	---------------------------------------------------------------------------*/
signed int PutRS( int byte )
{
   	if( Debug )	fprintf( stderr, "SendRS(): Character <0x%X><%c>\n", byte & 0xFF, byte & 0xFF );
   	
/*   	if( Bcostat( DEV_AUX ) == -1 )
   	{*/
	    Bconout( DEV_AUX, byte & 0xFF );
	 	sleep( 3 );
	   	return	JTB_OK;
/*	}
	return	JTB_EPUTCHAR;	*/
}

