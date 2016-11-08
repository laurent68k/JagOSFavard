/*
 *	Author:		FAVARD LAURENT
 *	Date:		14 April 2000
 *	Release:	17 April 2000
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
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<tos.h>

#include	<Jtoolbox.h>

void DisplayCommands( void )
{
	fprintf( stdout, "\033E\n");
	fprintf( stdout, "Your choice: \n");
    fprintf( stdout, "(O)Debug ON\n");
    fprintf( stdout, "(F)Debug OFF\n");
    fprintf( stdout, "(h)Help\n");
    fprintf( stdout, "(R)Serial\n");
    fprintf( stdout, "(C)Centronics\n");
    fprintf( stdout, "(I)Inquire\n");
    fprintf( stdout, "(S)Send Frame\n");
    fprintf( stdout, "(ESC) or (Q)uit\n");
}
/* ----------------------------------------------------------------------------------------------*/
void Shell( void )
{
	int			Code;
	TJagId 		JagIdentification;

    DisplayCommands();
    Code	=	0;
    while( !(Code == 0x1B || Code == 'Q' || Code == 'q') )
    {
    	switch( Code )
        {
        	case	'O':
        	case	'o':printf( "Debug ON\n");
        				JTB_Debug( 1 );
        				break;

        	case	'F':
        	case	'f':printf( "Debug OFF\n");
        				JTB_Debug( 0 );
        				break;
        				
        	case	'I':
            case	'i':fprintf( stderr, "Shell: Starting INQUIRE command\n");
            			if( Inquire( &JagIdentification ) != JTB_OK )
            			{
                           	fprintf( stderr, "Shell: Command inquire error\n");
                        }
                        else
                        {
	                        fprintf( stderr, "Shell: Command inquire SUCCESS\n");
                            fprintf( stderr, "OS Version 0x%X\n", JagIdentification._SystemVersion);
                            fprintf( stderr, "HD Version 0x%X\n", JagIdentification._HardwareVersion);
                            fprintf( stderr, "OS Date    0x%lX\n", JagIdentification._SystemDate);
                            fprintf( stderr, "HD Date    0x%lX\n", JagIdentification._HardwareDate);
                        }
            			break;

           	case	'S':
            case	's':fprintf( stdout, "Envoit trame TEST INQUIRE\n");
            			SendFrame( TOOLBOX_PID, KERNEL_PID, FRAME_INQUIRE, 0, NULL);
            			break;

            case	'H':
            case	'h':DisplayCommands();
            			break;

            case	'r':
            case	'R':printf( "Port serie\n" );
            			SetPort( PSERIAL );
            			break;

            case	'c':
            case	'C':printf( "Port Centronics\n" );
            			SetPort( PCENTRONICS );
						break;

            default:    printf( "What ??\n" );
            			break;
        }
        fprintf( stdout, "Press any to continue\n");
    	Code	=	(int)Crawcin() & 0xFF;
		printf( "\033E" );        
    }
}
/* ----------------------------------------------------------------------------------------------*/
int main( void )
{
    TJTBVersion Version;

	fprintf( stdout, "\033E");
	fprintf( stdout, "TOStudio Copyright Favard Laurent " __DATE__  "\n" );

    if( ! JTB_Open( 0 ) )
    {
    	fprintf( stderr, "Jtoolbox open error\n");
        return	-1;
    }

    JTB_Version( &Version );

    fprintf( stdout, "Jtoolbox version and date release:\n");
    fprintf( stdout, "Version: %s\n", Version.Version );
    fprintf( stdout, "Date   : %s\n", Version.Date );
    fprintf( stdout, "Press any key...\n" );
    Crawcin();

	Shell();

    JTB_Exit();
	return	0;
}
