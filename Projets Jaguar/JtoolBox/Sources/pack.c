/*	------------------------------------------------------------------------------
 *	Project JAGUAR:	JAGUAR PROTOCOLE COMMUNICATIONS.
 *	LOW LEVEL CODE FOR TOS MACHINE. IMPLEMENTATION C OG GetPack and SendPack
 *
 *	File:		Pack.c
 *	Author:		FAVARD Laurent
 *	Date:		12 April 2000
 *	Release:	04 May 2000
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
 *	------------------------------------------------------------------------------
 */

#include	<stdio.h>
#include	<ext.h>

#include	".\Toolsdef.h"
#include	"..\include\JToolbox.h"

/*	Import functions SERIAL	*/

extern	void InitRS( void );
extern	int StatRS( void );
extern	signed int GetRS( void );
extern	signed int PutRS( int data );

/*	Import functions CENRONICS	*/

extern	void InitC( void );
extern	int StatC( void );
extern	signed int GetC( void );
extern	signed int PutC( int data );

/* --------------------------------------------------------------------------- */
/*	Tables of drivers */

unsigned int	CurrDriver = PCENTRONICS;				/*	contents current port to use	*/
TDriver			TabDriver[ 3 ];							/*	tables of drivers */

/* ---------------------------------------------------------------------------	*/
/*	FRAME structure:	<SPID><TPID><OPCODE><LEN>CHECKSUM><DATA>				*/
/*						  4     4      2     2     2       n					*/


/*	---------------------------------------------------------------------------*/
static int SumValid( int CheckSum )
/*
	Check if the Checksum is valid. No very usefull currently !
*/
{
	return	( CheckSum == CheckSum );
}
/*	---------------------------------------------------------------------------*/
signed int GetFrame(long *SourcePID, long *TargetPID, int *Code, unsigned int *Length, void *Buffer)
/*
	Try to get a frame if possible. Buffer will get an unknow size of datas, so becarefull.

    Return	JTB_OK			if a frame was received
    		JTB_ETIMEOUT	if a timeout error occured
            JTB_NOFRAME		if no frame available
            JTB_ECHKSUM		if checksum error occured
*/
{
	THeader			Header;
    unsigned char	*Adresse;
    unsigned int	Count = 0, Number;
    DECL_TIMEOUT;

	/*  PART 1:	Get the frame header	*/;
    Count	=	GetPack( &Header, (unsigned int)sizeof( Header ) );
    if( Count == 0 )
    {
    	return	JTB_ENOFRAME;
    }
    if( Debug )
	    fprintf( stderr, "GetFrame(): %d bytes received for the Header\n", Count );

    Adresse		=	(unsigned char *)(&Header + Count);
    INIT_TIMEOUT;
    while( Count < (unsigned int)sizeof( Header ) && diff <= C_MAX_TIMEOUT )
    {
     	Count		+=	GetPack( Adresse, (unsigned int)sizeof( THeader ) - Count );
        Adresse		=	(unsigned char *)(&Header + Count);
        INC_TIMEOUT;
    }

	if( diff >= C_MAX_TIMEOUT )
    {
    	Trace( "GetFrame(): Timeout while receiving header");
        return	JTB_ETIMEOUT;
    }

    *SourcePID	=   Header.SPID;
    *TargetPID	=   Header.TPID;
    *Code		=	Header.Opcode;
    *Length		=   Header.Length;

    /*	PART 2:	Check the checksum */
    if( ! SumValid( Header.CheckSum ) )
    {
    	Trace( "GetFrame(): Checksum error occured");
    	return	JTB_ECHKSUM;
    }

	if( Debug )
	    fprintf( stdout, "GetFrame(): Now try to get frame data block Length=%d\n", Header.Length );

	/*	PART 3:	Get the data block */
    Adresse		=	(unsigned char *)Buffer;
    Count		=	0;
    timestart	=	time( NULL );
    diff		=	0;
    Number		=	0;
    INIT_TIMEOUT;
    while( Count < *Length && diff <= C_MAX_TIMEOUT )
    {
        Number		=	GetPack( Adresse, *Length );				/*	Try to read something */
        Count		+=	Number;										/*	Update total received */
        Adresse		=	(unsigned char *)Buffer + Count;			/*	next location */
        if( Number == 0 )											/*	If nothing update Timout */
			INC_TIMEOUT;
    }

	if( diff >= C_MAX_TIMEOUT )
    {
    	Trace( "GetFrame: Timeout during reception data block");
        return	JTB_ETIMEOUT;
    }
    return	JTB_OK;
}
/*	---------------------------------------------------------------------------*/
signed int SendFrame(long SourcePID, long TargetPID, int Opcode, unsigned int Length, void *Buffer)
/*
	Try to send a frame.

    Return	JTB_OK	if frame was sent
            Error code returned by SendPack if error occured
*/
{
	THeader		Header;
	int			Retour;

    Header.SPID		=	SourcePID;
    Header.TPID		=	TargetPID;
    Header.Opcode	=	Opcode;
    Header.Length	=	Length;
	Header.CheckSum	=	('C' << 8) | 'K';

	if( Debug )
		fprintf( stdout, "SendFrame(): Opcode = %X Length = %d\n", Opcode, Length );
		
    Retour = SendPack( &Header, (unsigned int)sizeof( Header ) );
   	if( Retour <= 0 )
    {
    	Trace( "SendFrame(): Error while sending header");
    	return	Retour;
    }

    if( Buffer != NULL && Length > 0 )
    {
    	Retour	=	SendPack( Buffer, Length );
        if( Retour <= 0 )
        {
            Trace( "SendFrame(): Error while sending data block");
            return	Retour;
        }
    }

    return	Retour;
}
/*	---------------------------------------------------------------------------*/
unsigned int GetPack(void *Buffer, unsigned int Expected)
/*
    This function try to get Expected bytes if possible and store them into Buffer.

    Return	0		if no bytes
    		Count	of bytes received.

    PROBLEMS:	I don't know why, but i need to pause each characters received
    			with a delay of 5 ms. It isn't very clean, but i don't understand
                what's happen...
*/
{
	signed int		byte = 0;
    unsigned int	Indice = 0;
    unsigned char	*Adresse;

    Adresse		=	(unsigned char *)Buffer;
    while( byte >= 0 && Indice < Expected )
    {
    	byte = TabDriver[ CurrDriver ].GetChar();
        if( byte >= 0 )
        {
        	Adresse[ Indice++ ]	=	(unsigned char)byte;
    		delay( 5 );							/*	FIXME: i don't why, but IT'S REQUIRED TO WORK NICE !!*/
        }
    }

    return	Indice;
}
/*	---------------------------------------------------------------------------*/
signed int SendPack(void *Buffer, unsigned int Length)
/*
	This function try to send Length bytes from the Buffer.

    Return	JTB_OK 	if successfull
    		Error code returned by the PuChar function if error
*/
{
	unsigned int 	Indice	=	0;
    signed int		Ret;
    unsigned char	*Adresse;

	/*	si pas d'octets, on considere que c'est bien envoye ! */
	if( Length == 0 )
	{
		Trace( "SendPack(): Length = 0 so no bytes to send, it's ok");
		return	JTB_OK;
	}

    Adresse	=	(unsigned char *)Buffer;
    Ret		=	JTB_OK;
    while( Indice < Length && Ret > 0 )
    {
    	Ret	=	TabDriver[ CurrDriver ].PutChar( (int)( Adresse[ Indice ] ) );
    	Indice++;
    }

    return	Ret;
}
/*	---------------------------------------------------------------------------*/
int GetPort( void )
/*
	Return the current port selected
*/
{
	return	CurrDriver;
}
/*	---------------------------------------------------------------------------*/
void SetPort( int port )
/*
	Set the current port to use and call init() of this one.
*/
{
	void		*OldStack;
	
	OldStack	=	(void *)Super( 0L );

	if( port == PBOARD )				/*	FIXME	*/
		return;

    Trace("Settings port" );
	CurrDriver	=	port;
	TabDriver[ port ].Init();
	
	Super( OldStack );
}
/*	---------------------------------------------------------------------------*/
void DrvInst( void )
/*
	Install drivers and Initialize default
*/
{
	Trace( "InstallDrivers() Ok\n");

	CurrDriver = PCENTRONICS;

    /*	Install subroutines for the board	*/
	TabDriver[ PBOARD ].Init	=	NULL;
	TabDriver[ PBOARD ].GetChar	=	NULL;
	TabDriver[ PBOARD ].PutChar	=	NULL;
	TabDriver[ PBOARD ].Status	=	NULL;

    /*	Install subroutines for the SERIAL	*/
	TabDriver[ PSERIAL ].Init		=	InitRS;
	TabDriver[ PSERIAL ].GetChar	=	GetRS;
	TabDriver[ PSERIAL ].PutChar	=	PutRS;
	TabDriver[ PSERIAL ].Status		=	StatRS;

    /*	Install subroutines for the CENTRONICS	*/
	TabDriver[ PCENTRONICS ].Init		=	InitC;
	TabDriver[ PCENTRONICS ].GetChar	=	GetC;
	TabDriver[ PCENTRONICS ].PutChar	=	PutC;
	TabDriver[ PCENTRONICS ].Status		=	StatC;
	
	/*	Initialize the default driver */
	TabDriver[ CurrDriver ].Init();
}
