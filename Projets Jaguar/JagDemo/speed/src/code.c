 /*
		Demo program for speed rate.

		28/29 September 1998 - Version 1.0
		Copyright Atari Funship (c) 1998
*/

#include <stdio.h>
#include <stdlib.h>
#include <tos.h>
#include <string.h>
#include <time.h>

#include <Pcaads.h>
#include <JToolBox.h>

/*	---------------------------------------------------------------------------------------	*/

#include "Speed__F.h"

extern	unsigned long		SizeSelected;

/*	---------------------------------------------------------------------------------------	*/
int doJAG2ST( time_t *StartTime, time_t *EndTime )
{
	TJagMemDesc 	JagMemDescriptor;
	void 			*STAddress;
	int				Ret = 0;
	
    JagMemDescriptor.StartAddress	=	(void *)0x5000L;
    JagMemDescriptor.Lenght			=	SizeSelected;
	STAddress						=	(void *)malloc( SizeSelected );

	if( STAddress )
    {
    	*StartTime	=	time( NULL );
    	graf_mouse( BUSYBEE, NULL );
		Ret = MemoryGet( STAddress, &JagMemDescriptor );
		if( Ret != JTB_OK )
			form_alert( 1, "[0][ Error occured ][Ok]" );
    	graf_mouse( ARROW, NULL );
        *EndTime   	=	time( NULL );
	    free(STAddress);
    }
    else
    	form_alert(1, "[1][ This machine doesn't have | enough of free memory ][Ok]");
    
    return	Ret;
}

/*	---------------------------------------------------------------------------------------	*/
int doST2JAG( time_t *StartTime, time_t *EndTime )
/*
	Upload a datas blocks to the jaguar.
*/
{
	TJagMemDesc 	JagMemDescriptor;
	void 			*STAddress;
	int				Ret = 0;

    JagMemDescriptor.StartAddress	=	(void *)0x5000L;
    JagMemDescriptor.Lenght			=	SizeSelected;
	STAddress						=	(void *)malloc( SizeSelected );

	if( STAddress )
    {
    	*StartTime	=	time( NULL );
    	graf_mouse( BUSYBEE, NULL );
		Ret = MemorySend( STAddress, &JagMemDescriptor );
		if( Ret != JTB_OK )
			form_alert( 1, "[0][ Error occured ][Ok]" );
    	graf_mouse( ARROW, NULL );
        *EndTime   	=	time( NULL );
	    free(STAddress);
    }
    else
    	form_alert(1, "[1][ This machine doesn't have | enough of free memory ][Ok]");
    	
    return	Ret;
}


