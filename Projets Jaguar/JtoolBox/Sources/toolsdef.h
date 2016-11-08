/*
 * ----------------------------------------------------------------------------------------------
 *		JAGUAR TOOLBOX APPLICATION PROGRAM INTERFACE:	INCLUDE FILE
 *		FOR ALL TOS ATARI AND COMPATIBLE
 *
 *	File:		ToolsDef.h
 *	Author:		FAVARD LAURENT
 *	Date:		25 March 1998
 *	Release:	11 May 2000
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
 * ----------------------------------------------------------------------------------------------
 */

#ifndef	__ToolsdefH
#define	__ToolsdefH

#include	<time.h>
#include 	<tos.h>

/* --------------------------------------------------------------------------- */

extern	void Trace( char *Message );
extern	int	Debug;
extern	void SemLock( void );
extern	void SemUnLock( void );


#define	G_JTOOLBOX_VERSION		"1.0.0"
#define	G_AUTHOR				"Favard Laurent"

/* --------------------------------------------------------------------------- */

#define	C_MAX_TIMEOUT		(double)5

#define	DECL_TIMEOUT		double			diff = 0; \
							time_t			timestart, timecurrent;

#define	INIT_TIMEOUT	  	timestart	=	time( NULL ); \
							diff		=	0;

#define	INC_TIMEOUT			timecurrent	=	time( NULL ); \
							diff = difftime( timecurrent, timestart );

/* --------------------------------------------------------------------------- */

/*	Stockage area for files	*/
/*#define		SIZE_BUFFER				64L	*	1024L*/
#define		MINIMAL_BUFFER			65536L


/*	for packcci	*/
#define		FIRST_DATA		1
#define		LAST_DATA		10

#define		C_IDSEM			0x41424344L
#define		SEM_CREATE		0
#define		SEM_DESTROY		1
#define		SEM_LOCK		2
#define		SEM_UNLOCK		3


/* --------------------------------------------------------------------------- */
/*	Types	*/

typedef	struct	{
					void (*Init)( void );
                    signed int (*GetChar)( void );
                    signed int (*PutChar)( int data );
                    int (*Status)( void );
				} TDriver;


#endif

