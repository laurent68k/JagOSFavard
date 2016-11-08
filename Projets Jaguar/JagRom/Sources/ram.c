/*
 * ----------------------------------------------------------------------------------------------
 *		programme TOS pour transferer dans la carte ROM simulateur le contenu d'un fichier.
 *
 *		VERSION : 1.0
 *		FAVARD Laurent 1997 - Atari FunShip pour la jaguar
 *
 * ----------------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tos.h>
#include <aes.h>

/*
 * ----------------------------------------------------------------------------------------------
 */


#define	TRUE					1
#define	FALSE					0

#define	REGISTRE_DONNEE			0xFB0000L			/*	/ROM3 */
#define	ZONE_ADRESSE			0xFA0000L			/*	/ROM4 */

#define	HEADER_BOOT				"BOOT"

int	G_JaugeON = FALSE;

/*
 * ----------------------------------------------------------------------------------------------
 */

unsigned long SearchBOOT(unsigned char *Buffer, unsigned long longueur)
/*
	Recherche le mot JAGR
*/
{
	unsigned char			string[4]	=	HEADER_BOOT;
	unsigned char			*pointeur;
	
	pointeur	=	Buffer;
	while(  ( *( (long *)pointeur ) != *( (long *)string) ) && ( (unsigned long)pointeur < (unsigned long)(Buffer + longueur) ) )
		pointeur += 2;
		
	if( (unsigned long)pointeur >= (unsigned long)(Buffer + longueur) )
	{
		return 0L;
	}
	else
		return (unsigned long)(pointeur - Buffer + 4L);
}

/*
 * ----------------------------------------------------------------------------------------------
 */

int SendFile(	char *PathName, 
				void (*UserIncrement)(unsigned long CountToDo, unsigned long CountPerfomed),
				unsigned int *StartTime, unsigned int *EndTime )
/*
	Send entire file procedure to the jaguar console.
*/
{
	unsigned char		Dummy;
	unsigned long		L_lSizeFile, L_PtrCourant, L_Nombre;
	unsigned char		*L_Buffer, *L_AdresseCible;
	int					Fichier, character;
	DTA					*dta;
	char				Texte[100];
	
	Fichier	=	open(PathName, O_RDONLY);
	Fsfirst(PathName,0);
	dta		=	Fgetdta();

	L_lSizeFile=	dta->d_length;
	if(L_lSizeFile == 0L)
	{
		printf("Error - Empty file\n");
		return FALSE;		
	}
	
	if(Fichier == -1)
	{
		sprintf(Texte, "[0][ Error - unable to open the file ][Ok]", PathName);
		form_alert(1, Texte);
		return FALSE;
	}

	L_Buffer	=	(unsigned char *)malloc(L_lSizeFile);
	if(L_Buffer == NULL)
	{
		form_alert(1, "[0][ Unable to allocate enough memory ][Ok]");
		return FALSE;		
	}
		
	/*	Lire le fichier et se positionner au debut du tampon */

	L_Nombre	=	read(Fichier, L_Buffer, L_lSizeFile);

	/*	Recherche automatique d'un mot BOOT pour se caler dessus */
	L_PtrCourant	=	SearchBOOT(L_Buffer, L_Nombre);
	if(L_PtrCourant == 0L)
	{
		form_alert(1, "[0][ BOOT keyword not found | Pass GEMDOS header 28 bytes ][Ok]");
		L_PtrCourant =	0L;
	}
	else
	{
		/*printf("BOOT found - pass %ld bytes, GEMDOS header (28) + BOOT word (4)\n", L_PtrCourant);*/
	}

	L_AdresseCible	=	0L;

	*StartTime	=	Tgettime();
	while( L_PtrCourant < L_Nombre)
	{	

		/*	Ecrire la donnee dans le registre */
		Dummy	=	*(unsigned char *)(REGISTRE_DONNEE | (unsigned char)L_Buffer[L_PtrCourant]);

		/*	Puis valider cette donnee a la bonne adresse */
		Dummy	=	*(unsigned char *)(ZONE_ADRESSE | (unsigned long)L_AdresseCible);

		/*	Incrementer la barre de progression */
		if( (L_PtrCourant & 0x1FFF) == 0 && G_JaugeON )
			(*UserIncrement)(L_Nombre, L_PtrCourant);
		
		L_AdresseCible++;
		L_PtrCourant++;
	}
	*EndTime	=	Tgettime();	
	(*UserIncrement)(L_Nombre, L_Nombre);
	
	close(Fichier); 		
	return TRUE;
}

/*
 * ----------------------------------------------------------------------------------------------
 */

int MakeBIN(char *PathName)
/*
	Send entire file procedure to the jaguar console.
*/
{
	unsigned long		L_lSizeFile, L_PtrCourant, L_Nombre;
	unsigned char		*L_Buffer;
	int					Fichier, FichierCible;
	DTA					*dta;
	char				PathNameCible[1024], *Pointeur;

	Fichier	=	open(PathName, O_RDONLY);
	Fsfirst(PathName,0);
	dta		=	Fgetdta();

	L_lSizeFile=	dta->d_length;
	if(L_lSizeFile == 0L)
	{
		printf("Error - Empty file\n");
		return FALSE;		
	}
	
	if(Fichier == -1)
	{
		form_alert(1, "[0][ Unable to open the file ][Ok]");
		return FALSE;
	}

	L_Buffer	=	(unsigned char *)malloc(L_lSizeFile);
	if(L_Buffer == NULL)
	{
		form_alert(1, "[0][ Unable to allocate enough memory ][Ok]");
		return FALSE;		
	}
		
	/*	Lire le fichier et se positionner au debut du tampon */

	L_Nombre	=	read(Fichier, L_Buffer, L_lSizeFile);
	close(Fichier); 		

	/*	Recherche automatique d'un mot BOOT pour se caler dessus */
	L_PtrCourant	=	SearchBOOT(L_Buffer, L_Nombre);
	if(L_PtrCourant == 0L)
	{
		form_alert(1, "[0][ BOOT not found, .IMG aborted ][Ok]");
		return FALSE;
	}
	else
	{
		/*printf("BOOT found - pass %ld bytes, GEMDOS header (28) + BOOT word (4)\n", L_PtrCourant);*/
	}

	strcpy(PathNameCible, PathName);
	Pointeur	=	strrchr(PathNameCible, '.');
	*Pointeur = 0;
	strcat(PathNameCible, ".img");

	FichierCible	=	creat(PathNameCible);
	write(FichierCible, L_Buffer + L_PtrCourant, L_Nombre - L_PtrCourant );
	close(FichierCible);

	return TRUE;
}
