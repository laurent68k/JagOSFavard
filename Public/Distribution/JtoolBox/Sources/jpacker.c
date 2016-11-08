/*
 * ----------------------------------------------------------------------------------------------
 *		JPacker: compress a CRY file to a CCI file.
 *		Copyright FAVARD Laurent 1998-2000
 *
 *	File:		JPacker.c
 *	Author:		FAVARD LAURENT
 *	Date:		07 April 1998
 *	Release:	20 April 2000
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tos.h>

/*	---------------------------------------------------------------------------*/

#include	"..\Include\JToolBox.h"
#include	"toolsdef.h"

/*	---------------------------------------------------------------------------*/
int PackCCI(char *FileName, char *Destination)
{
	FILE	*Fichier, *Sortie;
	char	Ligne[1024], ImageName[1024];
    char	chRefValue[6], chValue[6];
    char	*Indice;
    int		inNumber, inHowToWrite;

	Fichier	=	fopen( FileName, "r");
	if( Fichier == NULL )
		return	JTB_ENOCRYFILE;
    Sortie	=	fopen( Destination, "w");
	if( Sortie == NULL )
		return	JTB_ENOCCIFILE;
		
	/*
	 *	Compute the name from the source file
	 */
	Indice	=	strrchr(FileName, '\\');
	if( Indice != NULL )
		strcpy( ImageName, Indice + 1);
	else
		strcpy( ImageName, FileName);

	Indice	=	strrchr(ImageName, '.');
	if( Indice != NULL )
		*Indice	=	0;
	
	fgets(Ligne, 1023, Fichier);
	Indice	=	strchr(Ligne, '$');
	while( Indice == NULL && !feof(Fichier) )
	{
    	fprintf(Sortie, ";IGNORE>%s\n", Ligne);
		fgets(Ligne, 1023, Fichier);
		Indice	=	strchr(Ligne, '$');
	}
 
	if( Indice != NULL && !feof(Fichier) )
	{
		memcpy(chRefValue, Indice, 5);
		chRefValue[5]	=	0;
	}
    else
    {
		Trace("Error - No hexadecimal value found");
    	return	JTB_EBADVALUE;
    }

    fprintf(Sortie, ";\t---------------------------------------------------------\n");
    fprintf(Sortie, ";\tCry Compressed Image %s\n", FileName);
    fprintf(Sortie, ";\tJagStudio2000 Copyright FAVARD Laurent 2000\n");
    fprintf(Sortie, "\t\t\tDATA\n");
    fprintf(Sortie, "\t\t\tEVEN\n\n");
    fprintf(Sortie, "%sHeader:\tdc.b\t\"_CCI\"\t\t; compression type\n", ImageName);
    fprintf(Sortie, "\t\tdc.w\t$0010\t\t; version number\n");
    fprintf(Sortie, "%sBegin:\n", ImageName);

    inNumber		=	0;
	inHowToWrite	=	FIRST_DATA;
	while( !feof(Fichier) )
	{
    	Indice	=	strchr(Ligne, '$');
        while( Indice != NULL )
        {
        	memcpy(chValue, Indice, 5);
			chValue[5]	=	0;

			if( strcmp( chRefValue, chValue ) == 0 )
            {
             	inNumber++;
            }
            else
            {
				if( inHowToWrite == FIRST_DATA )
				{
	            	fprintf(Sortie, "\t\tdc.w\t%d,%s", inNumber, chRefValue);
					inHowToWrite++;
				}
				else if( inHowToWrite <= LAST_DATA )
				{
            		fprintf(Sortie, ",%d,%s", inNumber, chRefValue);
					inHowToWrite++;
				}
				else
				{
            		fprintf(Sortie, ",%d,%s\n", inNumber, chRefValue);
					inHowToWrite	=	FIRST_DATA;
				}

				strcpy( chRefValue, chValue );
				inNumber	=	1;
            }
            Indice	=	strchr(Indice+1, '$');
        }
		fgets(Ligne, 1023, Fichier);
	}
    fprintf(Sortie, "\n%sEnd:\tdc.w\t%d,%s\n", ImageName, inNumber, chRefValue);
    fprintf(Sortie, ";\t---------------------------------------------------------\n");

    fclose(Sortie);
	fclose(Fichier);
	
	return JTB_OK;
}
