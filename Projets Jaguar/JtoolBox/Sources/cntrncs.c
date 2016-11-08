/*
 * ----------------------------------------------------------------------------------------------
 *		Centronics Jaguar driver.
 *
 *	File:		Cntrncs.S
 *	Author:		FAVARD Laurent
 *	Date:		05 March 1999
 *	Release:	04 May 2000
 *
 *
 *	Behaviour:	STROBE ouput line is active at LOW level.
 *				BUSY input line is active at LOW level.
 *
 *
 *	I think that active level should be always LOW, 'cause it is default mode of centronics
 *	port. HIGH level are dangerous when software isn't started to initialize this port.
 *
 *	http://wwwlinux.utc.fr/~atari.html
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
#include <time.h>
#include <tos.h>

/* ----------------------------------------------------------------------------------------------*/

#include	"..\include\JToolbox.h"
#include	".\Parallel.h"
#include	".\Toolsdef.h"

/* ----------------------------------------------------------------------------------------------*/
void InitC( void )
{
	_initC();
	Ongibit( STROBE_HIGH );							/*	quiet = STROBE HIGH (as BUSY for Jaguar) */
}
/* ----------------------------------------------------------------------------------------------*/
int StatC( void )
/*
	This function return JTB_DEVBUSY if the Jaguar is being to initiate
	an exchange otherwise JTB_DEVFREE
*/
{
	int	Ret;
	
	Trace( "function StatC() called");
	
	if( (_getbusyC() & 0x01) == 1 )					/*	if BUSY is HIGH, no data */
    {
    	Trace( "StatC(): Centronics BUSY is HIGH, so device free" );
		Ret	=	JTB_DEVFREE;
    }
    else
    {
    	Trace( "StatC(): Centronics BUSY is LOW, so device busy by Jaguar" );
    	Ret	=	JTB_DEVBUSY;
    }
    
    return	Ret;
}
/* ----------------------------------------------------------------------------------------------*/
signed int GetC( void )
/*
	Behaviour:	when BUSY line is LOW, the jaguar is sending a byte in three step as
				follows:
				First 3 low bits and so on...d0-d2, d3-d5 and d6-d8 (d8 don't care !).
				ST must acknowledge each 3 blocks in settings STROBE line LOW and HIGH at each
				3 bits group.

*/
{
	unsigned int	data;
	unsigned int	shift, count, byte;
    DECL_TIMEOUT;

	if( (_getbusyC() & 0x01) == 1 )					/*	if BUSY is HIGH, return */
    {
    	Trace( "GetC(): Centronics error. BUSY is HIGH, so no data from Jaguar" );
		return	JTB_EGETCHAR;
    }
	byte	=	0;
	shift	=	0;
	data	=	0;
	for( count = 1; count <= 3; count++)
	{
        /*	wait until BUSY is LOW */
    	INIT_TIMEOUT;
		while( (_getbusyC() & 0x01) == 1 && diff < C_MAX_TIMEOUT)
		{
	    	INC_TIMEOUT;
        }
		if( diff > C_MAX_TIMEOUT )
        {
        	Trace( "GetC(): Centronics time out. BUSY is always HIGH" );
            return  JTB_ETIMEOUT;
        }

		data	=	Giaccess(0, PSG_PORTB_RD);
		byte	|=	( (data & 0x07) << shift );
		shift	+=	3;

		Offgibit( STROBE_LOW );						/*	set STROBE LOW */

		/*	wait until BUSY is HIGH */
        INIT_TIMEOUT;
		while( (_getbusyC() & 0x01) == 0 && diff < C_MAX_TIMEOUT )
        {
        	INC_TIMEOUT;
        }
		Ongibit( STROBE_HIGH );						/*	set STROBE HIGH */

        if( diff > C_MAX_TIMEOUT )
        {
        	Trace( "GetC(): Centronics time out. BUSY is always LOW" );
            return  JTB_ETIMEOUT;
        }

	}
	return	byte;
}
/* ----------------------------------------------------------------------------------------------*/
signed int PutC(int data)
{
	unsigned int	shift, count, byte;
    DECL_TIMEOUT;

	if( (_getbusyC() & 0x01) == 0 )					/*	check if BUSY is HIGH	*/
    {
    	Trace( "PutC(): Centronics error. BUSY is already LOW, so Jaguar is sending perhaps" );
		return	JTB_EBUSYLOW;						/*	if not then return (Jaguar may be busy)	*/
    }

	shift	=	0;
	for( count = 1; count <= 2; count++)
	{
		byte	=	( (data >> shift) & 0x0F ) << 4;
		shift	+=	4;

		Giaccess(byte, PSG_PORTB_WR);				/*	set data on port	*/
		Offgibit( STROBE_LOW );						/*	STROBE line LOW, 'cause data ready */

        /*	wait until BUSY LOW, this means data sampled */
        INIT_TIMEOUT;
		while( (_getbusyC() & 0x01) == 1  && diff < C_MAX_TIMEOUT )
        {
        	INC_TIMEOUT;
        }
		Ongibit( STROBE_HIGH );						/*	STROBE line HIGH no more data */

        if( diff > C_MAX_TIMEOUT )
        {
        	Trace( "PutC(): Centronics time out. BUSY is always HIGH" );
            return  JTB_EBUSYHIGH;
        }

        /*	wait until BUSY HIGH, transfer terminated */
        INIT_TIMEOUT;
		while( (_getbusyC() & 0x01) == 0  && diff < C_MAX_TIMEOUT )
        {
        	INC_TIMEOUT;
        }

        if( diff > C_MAX_TIMEOUT )
        {
        	Trace( "PutC(): Centronics time out. BUSY is always LOW" );
            return  JTB_ETIMEOUT;
        }
	}
	return	JTB_OK;
}
