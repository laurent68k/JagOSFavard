/*	------------------------------------------------------------------------------
 *	Project JAGUAR:	Frames Protocoles Communications.
 *
 *	IMPLEMENTATION C OF SENDBINARIES() AND GETBINARIES().
 *
 *	File:		Binaries.c
 *	Author:		FAVARD Laurent
 *	Date:		14 April 2000
 *	Release:	25 April 2000
 *	Language:	C
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
 *	------------------------------------------------------------------------------
 */

#include	<stdio.h>

#include	".\ToolsDef.h"
#include	"..\include\JToolbox.h"

/* --------------------------------------------------------------------------- */

#define	C_MAX_TRY			3

/* --------------------------------------------------------------------------- */
signed int GetBinaries(long aSPID, long aTPID, unsigned long aLength, void *aMemoryBlock)
/*
	Get a memories block from the Jaguar to the host computer (Atari) memories.
    This process need to be negociated before beetween Jaguar and Atari.
*/
{
	unsigned char	*Address;
    unsigned long	Count	=	0;
    signed int		Retour;
	THeader			Header;
    DECL_TIMEOUT;

	if( Debug )
	    fprintf( stdout, "GetBinaries(): Called to get %ld bytes\n", aLength );

    Address	=	aMemoryBlock;
  	INIT_TIMEOUT;
    while( Count < aLength && diff <= C_MAX_TIMEOUT )
    {
    	Retour	=	GetFrame( &Header.SPID, &Header.TPID, &Header.Opcode, &Header.Length, Address);
        if( Retour == JTB_ETIMEOUT )
        {
        	Trace( "GetBinaries(): Timout, no frame received from the Jaguar");
        	return	Retour;
        }
        else if( Retour == JTB_ECHKSUM )
        {
        	Trace( "GetBinaries(): Checksum error occured, send NACK frame");
        	SendFrame( aSPID, aTPID, FRAME_NACK, 0, NULL );
           	timestart	=	time( NULL );
			diff		=	0;
        }
		else if( Retour == JTB_ENOFRAME )
        {
        	Trace( "GetBinaries(): No frame available");
        }
		else
        {
        	Trace( "GetBinaries: Frame received send ACK frame");
        	SendFrame( aSPID, aTPID, FRAME_ACK, 0, NULL );
           	timestart	=	time( NULL );
			diff		=	0;
        }

        Address		+=	Header.Length;
        Count		+=  Header.Length;
		
		if( Debug )
	        fprintf( stderr, "GetBinaries(): %ld bytes received on %ld expected\n", Count, aLength );

        INC_TIMEOUT;
    }
    return	JTB_OK;
}

/*; ---------------------------------------------------------------------------
;		long cdecl SendBinaries(long SourcePID, long TargetPID, unsigned long Lenght, void *MemoryBlock)
;
;		Send a binarie memory block FROM the jaguar TO the host computer.
; ---------------------------------------------------------------------------*/

signed int SendBinaries(long aSPID, long aTPID, unsigned long aLenght, void *aMemoryBlock)
/*
	Send a memories block from the HOST computer (Atari) to the Jaguar memories.
    This process need to be negociated before beetween Jaguar and Atari.
*/
{
	unsigned char	*Address;
    unsigned long 	Count;
    unsigned int	Length, Retour;
    short			CountFailed	=	0;
	THeader			Header;
    DECL_TIMEOUT;

    Address		=	(unsigned char *)aMemoryBlock;
    Count		=   aLenght;
    CountFailed	=	0;
    while( Count > 0 && CountFailed < C_MAX_TRY )
    {
    	/*	Calculer si la longueur remplie une ou plusieur trames ou une seule. */
    	Length	=	( Count >= MAX_DATA_LEN ) ? MAX_DATA_LEN : (unsigned int)Count;

        /*	Envoyer la trame contenant les donnees */
		Retour	=	SendFrame( aSPID, aTPID, FRAME_DATA, Length, Address );
        if( Retour <= 0 )
        {
        	Trace( "SendBinaries(): SendFrame() error occured");
            return	Retour;
        }

        /*	Attendre la trame de retour ACK ou NACK	*/
    	INIT_TIMEOUT;
        Retour 		=	JTB_ENOFRAME;
        while( Retour != JTB_OK && diff <= C_MAX_TIMEOUT )
        {
        	Retour	=	GetFrame( &Header.SPID, &Header.TPID, &Header.Opcode, &Header.Length, NULL );
            INC_TIMEOUT;
        }

        /*	Verifier si un timeout n'est pas survenu	*/
        if( diff > C_MAX_TIMEOUT)
        {
        	Trace(  "Sendbinaries(): timeout, no response from Jaguar\n");
            return	JTB_ETIMEOUT;
        }

        /*	Si la trame recue indique une reponse positive, on passe au reste du tampon */
        if( Header.Opcode == FRAME_ACK )
        {
	        Address		+=	Length;
    	    Count		-=	Length;
        }
        else
        	CountFailed++;
    }
    return	JTB_OK;
}

