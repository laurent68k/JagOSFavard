/*
 * ----------------------------------------------------------------------------------------------
 *		JAGUAR TOOLBOX APPLICATION PROGRAM INTERFACE
 *		FOR ALL TOS ATARI AND COMPATIBLE
 *
 *	File:		JagExe.c
 *	Author:		FAVARD LAURENT
 *	Date:		25 March 1998
 *	Release:	30 September 1998
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * ----------------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tos.h>
#include <aes.h>

#include	"..\Include\JToolBox.h"

extern	TRecipient		Recipient;

#define		SIZE_MAX_HEADER			1024L

/*
 * ----------------------------------------------------------------------------------------------
 *				Procedures to handle different executable Jaguar headers.
 * ----------------------------------------------------------------------------------------------
 */

signed int IsJAGR(char *PathName, TFileJag *FileJaguar)
/*
	This procedure check for a "JAGR" roine's header.
	Return TRUE if header recognize else NO_HEADER if no found or FILE_ERREUR if a file error is occured.
	
	Format:			"JAGR"			* Roine mark
					.W	$Code		* Function number
					.L	$Target		* Destination adress in Jaguar
					.L	$Lenght		* Lenght in bytes
					.L	$Start		* Start adress in Jaguar
					.B	datastream	* Datadump

*/
{
	int				L_File;
	int				L_Retour;
	char			L_Buffer[ SIZE_MAX_HEADER ];
	char			string[4]	=	HEADER_JAGR;
	char			*pointeur;


	L_File	=	open(PathName, O_RDONLY);
	if( L_File < 0 )
	{
		return JTB_EFILE;
	}

	read(L_File, L_Buffer, SIZE_MAX_HEADER);

	pointeur	=	L_Buffer;
	while(  ( *( (long *)pointeur ) != *( (long *)string) ) && ( (unsigned long)pointeur < (unsigned long)(L_Buffer + SIZE_MAX_HEADER) ) )
		pointeur += 2;
		
	if( (long)pointeur >= (long)(L_Buffer + SIZE_MAX_HEADER) )
	{
		FileJaguar->OffsetStart	=	0L;
		return JTB_ENOHEADER;
	}

	FileJaguar->Header		=	*((long *)(pointeur));											/*	Marque JAGR	*/
	FileJaguar->CodeOp		=	*((int *) (pointeur+4));										/*	Numero de fonction */
	
	switch( FileJaguar->CodeOp )
	{
		/*	CODE for "Load and execute" */
		case 	LOAD_AND_GO:
					FileJaguar->StartAdr	=	(void *)*((long *)(pointeur+6));										
					FileJaguar->Length		=	*((long *)(pointeur+10));
					FileJaguar->StartCode	=	(void *)*((long *)(pointeur+14));
					FileJaguar->OffsetStart	=	(unsigned long)(pointeur - L_Buffer + SIZE_H_LDGOW);
					L_Retour		=	JTB_OK;

					break;

		default:	FileJaguar->StartAdr	=	NULL;										
					FileJaguar->Length		=	0L;
					FileJaguar->StartCode	=	NULL;
					FileJaguar->OffsetStart	=	0L;

					L_Retour		=	JTB_ENOHEADER;
					break;
	}
	close(L_File);
	return		L_Retour;
}

signed int IsJAGL(char *PathName, TFileJag *FileJaguar)
/*
	This procedure check for a "JAGL" roine's header. Th only difference beetween previous header is that
	'Code' is in this case a long instead of word.

	Return TRUE if header recognize else NO_HEADER if no found or FILE_ERREUR if a file error is occured.
	
	Format:			"JAGL"			* Roine mark
					.L	$Code		* Function number
					.L	$Target		* Destination adress in Jaguar
					.L	$Lenght		* Lenght in bytes
					.L	$Start		* Start adress in Jaguar
					.B	datastream	* Datadump

*/
{
	int				L_File;
	int				L_Retour;
	char			L_Buffer[SIZE_MAX_HEADER];
	char			string[4]	=	HEADER_JAGL;
	char			*pointeur;


	L_File	=	open(PathName, O_RDONLY);
	if( L_File < 0 )
	{
		return JTB_EFILE;
	}

	read(L_File, L_Buffer, SIZE_MAX_HEADER);

	pointeur	=	L_Buffer;
	while(  ( *( (long *)pointeur ) != *( (long *)string) ) && ( (unsigned long)pointeur < (unsigned long)(L_Buffer + SIZE_MAX_HEADER) ) )
		pointeur += 2;
		
	if( (long)pointeur >= (long)(L_Buffer + SIZE_MAX_HEADER) )
	{
		FileJaguar->OffsetStart	=	0L;
		return JTB_ENOHEADER;
	}

	FileJaguar->Header		=	*((long *)(pointeur));											/*	Marque JAGR	*/
	FileJaguar->CodeOp		=	*((int *) (pointeur+4));										/*	Numero de fonction */
	
	switch( FileJaguar->CodeOp )
	{
		/*	CODE for "Load and execute" */
		case 	LOAD_AND_GO:
					FileJaguar->StartAdr	=	(void *)*((long *)(pointeur+8));										
					FileJaguar->Length		=	*((long *)(pointeur+12));
					FileJaguar->StartCode	=	(void *)*((long *)(pointeur+16));
					FileJaguar->OffsetStart	=	(unsigned long)(pointeur - L_Buffer + SIZE_H_LDGOL);
					L_Retour		=	JTB_OK;

					break;

		default:	FileJaguar->StartAdr	=	NULL;
					FileJaguar->Length		=	0L;
					FileJaguar->StartCode	=	NULL;
					FileJaguar->OffsetStart	=	0;

					L_Retour		=	JTB_ENOHEADER;
					break;
	}
	close(L_File);
	return		L_Retour;
}

/*	ISXAJG IS NOT USED:	DO NOT USE BECAUSE NO XJAG HEADER IS DEFINED */

static signed int IsXJAG(char *PathName, TFileJag *FileJaguar)
/*
	This procedure check for a "XJAG" Favard Laurent's header.

	Return TRUE if header recognize else NO_HEADER if no found or FILE_ERREUR if a file error is occured.

	Format:			"XJAG"			*	FAVARD Laurent Jag PRG mark
					.W	Type		*	Executable type
					.L	Target		*	Destination upload adress in Jaguar
					.L	Start		*	Start program adress in Jaguar
					.L	*Name		*	Pointer to the name of program. NULL or != NULL
					.L	*Icon		*	Pointer to the icon bitmap of program. NULL or != NULL
					.B	datastream	* Datadump

*/
{
	int				L_File;
	int				L_Retour;
	char			L_Buffer[SIZE_MAX_HEADER];
	char			string[4]	=	HEADER_JAGR;
	char			*pointeur;


	L_File	=	open(PathName, O_RDONLY);
	if( L_File < 0 )
	{
		return JTB_EFILE;
	}

	read(L_File, L_Buffer, SIZE_MAX_HEADER);

	pointeur	=	L_Buffer;
	while(  ( *( (long *)pointeur ) != *( (long *)string) ) && ( (unsigned long)pointeur < (unsigned long)(L_Buffer + SIZE_MAX_HEADER) ) )
		pointeur += 2;
		
	if( (long)pointeur >= (long)(L_Buffer + SIZE_MAX_HEADER) )
	{
		FileJaguar->OffsetStart	=	0L;
		return JTB_ENOHEADER;
	}

	FileJaguar->Header		=	*((long *)(pointeur));											/*	Mark XJAG	*/
	FileJaguar->Type		=	*((int *) (pointeur+4));										/*	*/
	FileJaguar->StartAdr	=	(void *)*((long *)(pointeur+6));								/*	Start address to upload */
	FileJaguar->StartCode	=	(void *)*((long *)(pointeur+10));								/*	Start of program */
	FileJaguar->OffsetStart	=	(unsigned long)(pointeur - L_Buffer + SIZE_HEADER_XJAG);
	strcpy(FileJaguar->Name, (char *)(pointeur+14));											/*	Get Program name */		


	FileJaguar->CodeOp		=	0;																/*	Not exist in this format */
	FileJaguar->Length		=	0L;																/*	Not exist in this format */
	L_Retour				=	JTB_OK;

	close(L_File);
	return		L_Retour;
}
