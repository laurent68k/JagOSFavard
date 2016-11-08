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

#include	<jtoolbox.h>

/* ----------------------------------------------------------------------------------------------*/

int main(void)
{
	int				choix;
	long			old_super_stack;
	unsigned long	byte;
	
	old_super_stack = Super( 0L );
	printf("\033E\n");
	printf("\033pATARI <-> Jaguar communication via Centronics\n");
	printf("This version use STROBE and BUSY with LOW level enable\n");
	printf("Each character is sent to Jaguar which return it\n");
	printf("USE JTOOLBOX\n");
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
		if( (byte = GetC()) != -1L )
		{		
			printf("(%c)", (unsigned char)byte);
		}

		choix = ((int)Crawio(0xFF) & 0xFF);
		if( choix > 0x1B && choix != 'q' )
		{
			printf("%c", (unsigned char)choix);
			PutC( choix );
			printf("<%c>", (unsigned char)choix);
		}
	}

	Super( (void *)old_super_stack );
	return 0;
}
