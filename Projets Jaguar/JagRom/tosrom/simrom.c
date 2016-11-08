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

/*
 * ----------------------------------------------------------------------------------------------
 */


#define	TRUE					1
#define	FALSE					0

#define	REGISTRE_DONNEE			0xFB0000L			/*	/ROM3 */
#define	ZONE_ADRESSE			0xFA0000L			/*	/ROM4 */

#define	HEADER_BOOT				"BOOT"

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

static int SendFile(char *PathName)
/*
	Send entire file procedure to the jaguar console.
*/
{
	unsigned char		Dummy;
	unsigned long		L_lSizeFile, L_PtrCourant, L_Nombre;
	unsigned char		*L_Buffer, *L_AdresseCible;
	int					Fichier, character;
	DTA					*dta;
	
	/*	*/
	printf("\033E");
	printf("\033pFile transfer of '%s' into the RAM\033q\n", PathName);

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
		printf(" Error - unable to open %s\n", PathName);
		return FALSE;
	}

	L_Buffer	=	(unsigned char *)malloc(L_lSizeFile);
	if(L_Buffer == NULL)
	{
		printf("Error - Unable to allocate enough memory for this file\n");
		return FALSE;		
	}
		
	/*	Lire le fichier et se positionner au debut du tampon */

	L_Nombre	=	read(Fichier, L_Buffer, L_lSizeFile);

	/*	Recherche automatique d'un mot BOOT pour se caler dessus */

	printf("Automatic search for BOOT...\n");
	L_PtrCourant	=	SearchBOOT(L_Buffer, L_Nombre);
	if(L_PtrCourant == 0L)
	{
		printf("BOOT not found\n");
		L_PtrCourant =	0L;
	}
	else
	{
		printf("BOOT found - pass %ld bytes, GEMDOS header (28) + BOOT word (4)\n", L_PtrCourant);
	}

	L_AdresseCible	=	0L;
	printf("\033pPress any key to start (And ESC at any moment to exit)\033q\n\n");
	printf("Upload in process...\n\n");


	/*Transferer octet/octet */

	while( L_PtrCourant < L_Nombre)
	{	

		/*	Ecrire la donnee dans le registre */
		Dummy	=	*(unsigned char *)(REGISTRE_DONNEE | (unsigned char)L_Buffer[L_PtrCourant]);
		Dummy	=	*(unsigned char *)(REGISTRE_DONNEE | (unsigned char)L_Buffer[L_PtrCourant]);
		Dummy	=	*(unsigned char *)(REGISTRE_DONNEE | (unsigned char)L_Buffer[L_PtrCourant]);
		Dummy	=	*(unsigned char *)(REGISTRE_DONNEE | (unsigned char)L_Buffer[L_PtrCourant]);
			
		/*	Puis valider cette donnee a la bonne adresse */
		Dummy	=	*(unsigned char *)(ZONE_ADRESSE | (unsigned long)L_AdresseCible);
		Dummy	=	*(unsigned char *)(ZONE_ADRESSE | (unsigned long)L_AdresseCible);
		Dummy	=	*(unsigned char *)(ZONE_ADRESSE | (unsigned long)L_AdresseCible);
		Dummy	=	*(unsigned char *)(ZONE_ADRESSE | (unsigned long)L_AdresseCible);

		L_AdresseCible++;
		L_PtrCourant++;

		/*	Si touche appuyee, arretee */
		character = (int)Crawio(0xFF);
		if(character == 0x1B)
			break; 
	}
	close(Fichier); 		

	printf("\Press any key to go back to main menu\n");
	Crawcin();
	return TRUE;
}

/*
 * ----------------------------------------------------------------------------------------------
 */

static int MakeBIN(char *PathName)
/*
	Send entire file procedure to the jaguar console.
*/
{
	unsigned long		L_lSizeFile, L_PtrCourant, L_Nombre;
	unsigned char		*L_Buffer;
	int					Fichier, FichierCible, character;
	DTA					*dta;
	char				PathNameCible[1024], *Pointeur;

	/*	*/
	printf("\033E");
	printf("\033pConverting %s to IMG file\033q\n", PathName);

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
		printf(" Error - unable to open %s\n", PathName);
		return FALSE;
	}

	L_Buffer	=	(unsigned char *)malloc(L_lSizeFile);
	if(L_Buffer == NULL)
	{
		printf("Error - Unable to allocate enough memory for this file\n");
		return FALSE;		
	}
		
	/*	Lire le fichier et se positionner au debut du tampon */

	L_Nombre	=	read(Fichier, L_Buffer, L_lSizeFile);
	close(Fichier); 		

	/*	Recherche automatique d'un mot BOOT pour se caler dessus */

	printf("Automatic search for BOOT...\n");
	L_PtrCourant	=	SearchBOOT(L_Buffer, L_Nombre);
	if(L_PtrCourant == 0L)
	{
		printf("BOOT not found, make IMG aborted\n");
		Crawcin();
		return FALSE;
	}
	else
	{
		printf("BOOT found - pass %ld bytes, GEMDOS header (28) + BOOT word (4)\n", L_PtrCourant);
	}

	printf("\033pPress any key to start (And ESC at any moment to exit)\033q\n\n");

	character = (int)Crawcin();
	if(character == 0x1B)
		return FALSE;

	strcpy(PathNameCible, PathName);
	Pointeur	=	strrchr(PathNameCible, '.');
	*Pointeur = 0;
	strcat(PathNameCible, ".img");

	printf("Write %s binary file\n", PathNameCible);
	FichierCible	=	creat(PathNameCible);
	write(FichierCible, L_Buffer + L_PtrCourant, L_Nombre - L_PtrCourant );
	close(FichierCible);

	printf("\Press any key to go back to main menu\n");
	Crawcin();
	return TRUE;
}

/*
 * ----------------------------------------------------------------------------------------------
 */

static int Test(void)
/*
*/
{
	unsigned char		Dummy;
	int					character = 'A', L_Value, L_Adresse = 0;
	
	/*	*/
	printf("\033E");
	printf("\033pData register test mode\033q\n");
	printf("Enter a value to write\n");
	printf("\033pPress any key to start (And ESC at any moment to exit)\033q\n\n");


	printf("ROM Address where write :");
	scanf("%d", &L_Adresse);

	printf("Value to write :");
	scanf("%d", &L_Value);
	printf("Writing 0x%X value into data register\n", (unsigned char)L_Value);
	printf("with a read at 0x%lX\n", (REGISTRE_DONNEE | (unsigned char)L_Value));

	/*	Ecrire la donnee dans le registre */
		
	Dummy	=	*(unsigned char *)(REGISTRE_DONNEE | (unsigned char)L_Value);

	while( character != 0x1B)
	{	
		/*	Puis valider cette donnee a la bonne adresse */
		Dummy	=	*((unsigned char *)(ZONE_ADRESSE | (unsigned char)L_Adresse));

		/*	Si touche appuyee, arretee */
		character = (int)Crawio(0xFF);
		if(character == 0x1B)
			break; 
	}

	printf("\Press any key to go back to main menu\n");
	Crawcin();
	return TRUE;
}

/*
 * ----------------------------------------------------------------------------------------------
 */

int main(int argc, char *argv[])
{
	int			choix;
	char		filename[1024] ="\0";

	printf("\033E");
	printf("\033pATARI ROM Jaguar Simulator "__DATE__", Copyright 1997-1999\033q\n");

	if(argc == 2)
	{
		SendFile(argv[1]);
	}

	choix	=	'A';
	while(choix != 'Q' && choix != 'q' && choix != 0x1B)
	{

		printf("\033E\n");
		printf("\033pATARI ROM Jaguar Simulator V1.2 "__DATE__"- copyright 1997-2000\033q\n");
		printf("\nCurrent file selected is \033p%s\033q\n\n", filename);

		printf("(C)hoice a file to send\n");
		printf("(S)end current selected file into the RAM\n");
		printf("(B)inary file from PRG file\n");
		printf("(Q)uit program or ESC\n\n");

		printf("\033p                                                            \033q\n");

		printf("Enter your choice :");
		choix = (int)Crawcin();
	
		switch(choix)
		{
			case	'C': case 'c':
							printf("\nSelect the file to upload: ");
							scanf("%s",filename);
							break;

			case	'S': case 's':
							SendFile(filename);
							break;

			case	'I': case 'i':
							printf("\nConvert current .PRG file to .IMG file");
							MakeBIN(filename);
							break;

			default:		break;
		}
	}

	return TRUE;
}
