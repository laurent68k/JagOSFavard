/*
 * ----------------------------------------------------------------------------------------------
 *		JAGUAR TOOLBOX APPLICATION PROGRAM INTERFACE
 *		FOR ALL TOS ATARI AND COMPATIBLE
 *
 *	File:		DiskJag.c
 *	Author:		FAVARD LAURENT, FunShip(c)
 *	Date:		25 March 1998
 *	Release:	25 April 2000
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
#include <string.h>
#include <stdlib.h>
#include <tos.h>
#include <aes.h>

#include	"..\include\JToolBox.h"
#include	"toolsdef.h"

/* ----------------------------------------------------------------------------------------------*/
signed int JagToDisk(char *aFileName, TJagMemDesc *JagMem)
/*
	Perform a download from the jaguar memory directly to a file.
*/
{
	char			*Buffer;
	unsigned long	Count, SizeAllocate;
	int				Fichier;
	signed int		Retour;
	
	/*	First - try to allocate a buffer to get all datas to download	*/
	SizeAllocate	=	JagMem->Lenght;
	Buffer			=	malloc( SizeAllocate );

	if( Buffer == NULL )
	{
		SizeAllocate	=	JagMem->Lenght / 2;
		while( (Buffer == NULL) && (SizeAllocate > MINIMAL_BUFFER))
		{
			SizeAllocate	=	SizeAllocate / 2;
			Buffer			=	malloc( SizeAllocate );
		}

		if( Buffer == NULL )
		{
			Trace( "Not enought memories" );
			return	JTB_EMEM;
		}
	}

	/*	Second - Download datas */
	Fichier	=	creat( aFileName );

	if( Fichier < 0 )
	{
		Trace( "JagToDisk(): Unable to create the dump memories file");
		return	JTB_EFILE;
	}

	Retour	=	JTB_OK;
	Count	=	JagMem->Lenght;	
	while( Count > 0 && Retour == JTB_OK )
	{
		JagMem->Lenght			=	( Count >= SizeAllocate ) ? SizeAllocate :  Count;

		Retour	=	MemoryGet( Buffer, JagMem );
		if( Retour <= 0 )
 			Trace( "Error while execute MemoryGet()");

		write( Fichier, (void *)Buffer, (size_t)JagMem->Lenght);
 			
		/*	Decrement count to receive and increment next jaguar location address */
		Count					-=	(unsigned long)JagMem->Lenght;
		JagMem->StartAddress	=	(void *)((unsigned long)JagMem->StartAddress + (unsigned long)JagMem->Lenght);
	}	

	free( Buffer );
	close( Fichier );
	return	Retour;
}

/* ----------------------------------------------------------------------------------------------*/
signed int DiskToJag( char *PathName, unsigned long OffsetFromStart,
						TJagMemDesc		*JagMemoire,
						void (*UserFonctionJauge)(unsigned long CountToDo, unsigned long CountPerfomed))
/*
	Send any binaries file to the jaguar.
*/
{
	int					File;
	signed int			Retour;
	char				*Buffer, *PtrCourant;
	size_t				CountRead;
	DTA					*Dta;
	unsigned long		CountToDo, CountPerfomed, SizeAllocate;
	TJagMemDesc			JagMem;
	
	File	=	open(PathName, O_RDONLY);

	Fsfirst( PathName, 0 );																	/* Pour fixer une zone DTA du fichier */
	Dta		=	Fgetdta();									/* Lecture de la DTA */

	CountToDo		=	Dta->d_length;
	CountPerfomed	=	0L;

	if( Dta->d_length == 0L)																	/* Taille fichier = 0 octets !! */
	{
		Trace( "File is empty" );
		return JTB_EFILE;
	}
	
	if( File < 0)
	{
		Trace( "Unable to open the file" );
 		return JTB_EFILE;
	}
	
	/*	First - try to allocate a buffer to get all datas to download	*/
	SizeAllocate	=	Dta->d_length;	/*SIZE_BUFFER; FIXME */
	Buffer			=	malloc( SizeAllocate );
	
	if( Buffer == NULL )
	{
		SizeAllocate	=	SizeAllocate / 2;
		while( (Buffer == NULL) && (SizeAllocate > MINIMAL_BUFFER))
		{
			SizeAllocate	=	SizeAllocate / 2;	
			Buffer			=	malloc( SizeAllocate );
		}

		if( Buffer == NULL )
		{
			Trace( "Not enough memories available to allocate a buffer" );
			return	JTB_EMEM;
		}
	}

	CountRead	=	read( File, Buffer, SizeAllocate);
	PtrCourant	= (char *)((unsigned long)Buffer + OffsetFromStart);

	Retour	=	JTB_OK;
	JagMem.StartAddress	=	(void *)JagMemoire->StartAddress;
	JagMem.Lenght		=	CountRead - OffsetFromStart;
	while( CountRead > (size_t)0 && Retour == JTB_OK )
	{
		CountPerfomed	+=	( CountRead / 2 );
		(*UserFonctionJauge)(CountToDo, CountPerfomed);

		JagMem.Lenght	=	CountRead - OffsetFromStart;
		Retour			=	MemorySend( PtrCourant, &JagMem );
		if( Retour <= 0 )
		{
			Trace( "error while transfer" );
		}

		CountPerfomed	+=	( CountRead / 2 );
		(*UserFonctionJauge)(CountToDo, CountPerfomed);

		CountRead		=	read( File, Buffer, SizeAllocate);
		PtrCourant		=	Buffer;
		JagMem.StartAddress	=	(void *)((unsigned long)JagMem.StartAddress + JagMem.Lenght);
		JagMem.Lenght		=	CountRead;
	}
	close( File );
	free( Buffer );

	return Retour;
}
