/*
 * ----------------------------------------------------------------------------------------------
 *		programme TOS de test d'‚change de donn‚es via le port JoyPad 2.
 *		FONCTIONNE PARFAITEMENT !
 *
 *	File:		Cntrncs.S
 *	Author:		FAVARD Laurent
 *	Date:		05 March 1999
 *	Release:	10 April 2000
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
 * ----------------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <tos.h>

/* ----------------------------------------------------------------------------------------------*/

#include	"parallel.h"

/* ----------------------------------------------------------------------------------------------*/

void InitC(void)
{
	printf("Centronics Initialization:STROBE set HIGH\n");
	_initC();
	Ongibit( STROBE_HIGH );							/*	quiet = STROBE HIGH (as BUSY for Jaguar) */
}

/* ----------------------------------------------------------------------------------------------*/

int GetC(void)
/*
	Behaviour:	when BUSY line is LOW, the jaguar is sending a byte in three step as
				follows:
				First 3 low bits and so on...d0-d2, d3-d5 and d5-d8 (d8 don't care !).
				ST must acknowledge each 3 blocks with set STROBE line LOW and HIGH after
				to got it.
				
*/
{
	unsigned int	data;
	unsigned int	shift, count, byte;

	if( (_getbusyC() & 0x01) == 1 )							/*	if BUSY line is HIGH, no data */
		return	-1;

	byte	=	0;
	shift	=	0;
	data	=	0;
	for( count = 1; count <= 3; count++)
	{
		while( (_getbusyC() & 0x01) == 1 );					/*	check BUSY line LOW */

		data	=	Giaccess(0, PSG_PORTB_RD);
		byte	|=	( (data & 0x07) << shift );
		shift	+=	3;

		Offgibit( STROBE_LOW );								/*	set STROBE line LOW */
		while( (_getbusyC() & 0x01) == 0);					/*	wait until BUSY line is HIGH */
		Ongibit( STROBE_HIGH );								/*	set STROBE line HIGH */
	}
	return	byte;
}

/* ----------------------------------------------------------------------------------------------*/

int PutC(int data)
{
	unsigned int	shift, count, byte;

	if( (_getbusyC() & 0x01) == 0 )							/*	check if BUSY is HIGH	*/
		return	-1;											/*	if not then return (Jaguar may be busy)	*/

	shift	=	0;
	for( count = 1; count <= 2; count++)
	{
		byte	=	( (data >> shift) & 0x0F ) << 4;
		shift	+=	4;
		
		Giaccess(byte, PSG_PORTB_WR);						/*	set data on port	*/		
		Offgibit( STROBE_LOW );								/*	STROBE line LOW, 'cause data ready */
		
		while( (_getbusyC() & 0x01) == 1 );					/*	wait until BUSY LOW, this means data sampled */
		Ongibit( STROBE_HIGH );								/*	STROBE line HIGH no more data */
		while( (_getbusyC() & 0x01) == 0 );					/*	wait until BUSY HIGH, transfer terminated */
	}
	return	byte;
}

/* ----------------------------------------------------------------------------------------------*/

int main(void)
{
	int				choix;
	long			old_super_stack;
	int	byte;
	
	old_super_stack = Super( 0L );
	printf("\033E\n");
	printf("\033pATARI <-> Jaguar communication via Centronics\n");
	printf("This version use STROBE and BUSY with LOW level enable\n");
	printf("Each character is sent to Jaguar which return it\n");
	printf("http://wwwlinux.utc.fr/~atari.html\n");
	printf(__DATE__" (c) Laurent FAVARD   \033q\n\n");

	InitC();
		
	printf(	"Press any key to start\n"
			"ESC to quit TOS program only (Let Jaguar running)\n"
			"'q' to exit TOS and Jaguar program\n\n");
			
	choix	=	(int)Crawcin() & 0xFF;

	printf("Enter in echo mode\n");
	while( (choix != 0x1B) && (choix != 'q') )
	{
		if( (byte = GetC()) != -1 )
		{		
			printf("%c", (unsigned char)byte);
		}
		choix = ((int)Crawio(0xFF) & 0xFF);
		if( choix > 32 && choix != 'q' )
		{
			PutC( choix );
			while( (byte = GetC()) == -1 )
			{
			}
			printf("%c", (unsigned char)byte);
		}
	}

	Super( (void *)old_super_stack );
	return 0;
}
