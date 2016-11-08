/*
 * ----------------------------------------------------------------------------------------------
 *			Module de gestion du fichier INF de configuration de l'application.
 *			La gestion repose sur la notion de Chapitre et d'EntrÇe.
 *
 * 	Author  : 	FAVARD Laurent, FunShip (c)
 *	File    : 	TOOLS.C
 *	Date    : 	15 Mai 1997
 *	Release : 	08 Juillet 1997
 *	Version : 	1.0
 *	Country : 	FRANCE
 *			
 *
 *							- Copyright Atari FunShip (c) 1997 -
 * ----------------------------------------------------------------------------------------------
 */

#include	<tos.h>
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<aes.h>

/*
 *	TRACE = form_alert pour informer
 */

/*#define			__TRACE__*/

#define			ERREUR_OPEN_INF				"[3][ Open error INF file ][OK]"
#define			ERROR_NO_ENTRYCHAPITRE		"[3][ No entry %s found | for the chapter %s][OK]"
#define			ERROR_NO_ENTRY				"[3][ No entry %s found ][OK]"
#define			ERROR_NO_CHAPITRE			"[3][ No chapter %s found ][OK]"		
#define			ERROR_NO_INF				"[3][ No INF file specified ][OK]"
#define			ERREUR_SIZE_INF				"[3][ Unable to get the inf's size ][Ok]"

#define			ERROR_MEMORY				3

#define			STR_MARK_DEBUT				"["
#define			STR_MARK_FIN				"]"
#define			CAR_MARK_DEBUT				'['

#define			ENTRY_FOUND					1
#define			NO_CHAPITRE					0
#define			NO_ENTRY					-1

#define			SIZE_DEFAULT				1024

/*	Tampon cache de maintien de l'image du fichier INF */

#define	SIZE_NAME		256
#define	SIZE_PATHNAME	8192

static	char	*G_TAMPON_INF 		 		=	NULL;
static	char	INIFILENAME[SIZE_NAME]		=	"\0";
static	char	*PATHINIFILENAME			=	"\0";
static	size_t	SIZETAMPON 					=	0;

static	char	FileEnFaute[1024]			=	"\0";

/*
 * ----------------------------------------------------------------------------------------------
 *									Primitives privÇes
 * ----------------------------------------------------------------------------------------------
 */

static	void AgrandirTampon(size_t Supplement)
/*
		Agrandie le tampon de 'Supplement' caractäres de plus, et majore celui-cid de 25%.	
*/
{
	char	*Pointeur;
	
	
 	SIZETAMPON	=	strlen(G_TAMPON_INF) + Supplement;									/*	Calculer nouvelle taille */
	SIZETAMPON	=	SIZETAMPON + (size_t)(SIZETAMPON / 4);
			
	Pointeur	=	malloc(SIZETAMPON);													/*	Allouer un plus grand */
	if(Pointeur != NULL)
	{
		strcpy(Pointeur, G_TAMPON_INF);													/*	Recopier */

		free(G_TAMPON_INF);																/*	LibÇrer l'ancien */
		G_TAMPON_INF	=	Pointeur;
	}
	else
		form_error(ERROR_MEMORY);	
}

static	int	Search(char *Chapitre, char *Entree, char **Position)
{
	char		L_Chapitre[256];
	char		L_Entree[256];
	char		*DebutChapitre, *NextChapitre;
	int			Code;
#ifdef	__TRACE__	
	char		message[128];
#endif

	/*	Formatter le chapitre en [xx] */
	strcpy(L_Chapitre, STR_MARK_DEBUT);
	strcat(L_Chapitre, Chapitre);
	strcat(L_Chapitre, STR_MARK_FIN);
		
	/*	Formatter l'entrÇe en \nxx= */
	strcpy(L_Entree, "\n");
	strcat(L_Entree, Entree);
	strcat(L_Entree, "=");
	
	if((DebutChapitre = strstr(G_TAMPON_INF, L_Chapitre)) != NULL)								/*	Chapitre trouvÇ */
	{
		/*	Rechercher le prochain caractäre [ dÇlimitant le prochain Chapitre */
		NextChapitre	=	strchr(DebutChapitre + 1L, CAR_MARK_DEBUT);
		*Position		=	strstr(DebutChapitre, L_Entree);	/* !! */

		if(*Position != NULL)
		{
			if(NextChapitre != NULL && *Position > NextChapitre)							/*	Si pointeur sur entrÇe, supÇrieur prochain Chapitre */
			{
#ifdef	__TRACE__
				sprintf(message,ERROR_NO_ENTRYCHAPITRE , Entree, Chapitre);	
				form_alert(1,message);				
#endif
				*Position = DebutChapitre;													/* pas d'entrÇe, retourner la Chapitre trouvÇ */
				Code = NO_ENTRY;
			}
			else																			/*	EntrÇe trouvÇe, on retourne l'adresse */
			{
				/*	Position = adresse char * trouvÇe */
				*Position += 1L;							/* !! */
				Code = ENTRY_FOUND;
			}
		}
		else
		{
#ifdef	__TRACE__
			sprintf(message, ERROR_NO_ENTRY, Entree);
			form_alert(1,message);
#endif			
			*Position = DebutChapitre;														/* pas d'entrÇe, retourner la Chapitre trouvÇ */
			Code = NO_ENTRY;
		}
	}
	else																					/*	pas de Chapitre */
	{
#ifdef	__TRACE__
		sprintf(message,ERROR_NO_CHAPITRE , Chapitre);
		form_alert(1,message);
#endif
		Code = NO_CHAPITRE;
	}
	return Code;
}

static void ReadPath( char *FileNameINF)
{
	PATHINIFILENAME		=	malloc( SIZE_PATHNAME );
	if( PATHINIFILENAME == NULL )
	{
		form_error( ERROR_MEMORY );
		return;
	}
		
	PATHINIFILENAME[0]	=	Dgetdrv() + 'A';
	PATHINIFILENAME[1]	=	':';
	Dgetpath( &(PATHINIFILENAME[2]), 0 ); 
	strcat( PATHINIFILENAME, "\\" );
	strcat( PATHINIFILENAME, FileNameINF );
}

/*
 * ----------------------------------------------------------------------------------------------
 *									Sauvegarde le tampon dans le fichier INF
 * ----------------------------------------------------------------------------------------------
 */

void SaveProfile(void)
/*
	Sauvegarde le tampon du .INF dans le fichier.
*/

{
	FILE		*FichierINF;

	graf_mouse(BUSYBEE,NULL);
  
	/*	Sauvegarder le fichier */
	if((FichierINF = fopen(PATHINIFILENAME, "w")) != NULL)
	{
		/*fprintf(FichierINF, G_TAMPON_INF);*/
		fwrite(G_TAMPON_INF, 1, strlen(G_TAMPON_INF), FichierINF);
		fclose(FichierINF);
	}
  
	graf_mouse(ARROW,NULL);
}

void CloseProfile(void)
/*
	Sauvegarde le tampon du .INF dans le fichier et le detruit.
	Plus aucun acces n'est possible.
*/
{
	SaveProfile();
  
	free( G_TAMPON_INF );
	free( PATHINIFILENAME );
	INIFILENAME[0]	=	0;
	SIZETAMPON		=	0L;
	G_TAMPON_INF	=	NULL;
  
	graf_mouse(ARROW,NULL);
}

/*
 * ----------------------------------------------------------------------------------------------
 *									Lecture d'une entrÇe INF.
 * ----------------------------------------------------------------------------------------------
 */

void GetProfileString(char *FileNameINF, char *Chapitre, char *Entree, char *ValeurChamp, char *Defaut)
{
	DTA			*PtrDta;
	FILE		*FichierINF = NULL;
	char		Format[257];
	char		*Position;
	long		size;

	/*	Initialiser la valeur de defaut au cas oó on ne trouve pas les informations */	
	strcpy(ValeurChamp, Defaut);

	/*	Si le tampon contient quelque choses et que ce n'est pas ce fichier, alors sauvegarder le tampon avant. */
	if(G_TAMPON_INF != NULL && strcmp(INIFILENAME, FileNameINF) != 0 )
	{
		SaveProfile();
		
		free( G_TAMPON_INF );
		free( PATHINIFILENAME );
		G_TAMPON_INF = NULL;
		INIFILENAME[0] = 0;
	}
	
	/*	Si le tampon ne contient pas d'image de fichier INF et qu'aucun INF n'a ÇtÇ chargÇ, alors le charger */
	if(G_TAMPON_INF == NULL && strlen(INIFILENAME) == 0 )
	{
		graf_mouse(BUSYBEE, NULL);

		ReadPath( FileNameINF);
		
		FichierINF	=	fopen(FileNameINF, "r");
		if(FichierINF == NULL)
		{
			if(strcmp(FileEnFaute, FileNameINF) != 0)												/*	affichier UNE fois le fichier non trouvÇ */
			{
				form_alert(1,ERREUR_OPEN_INF);
				strcpy(FileEnFaute, FileNameINF);
			}
			graf_mouse(ARROW, NULL);
			return;
		}		
		
		if(Fsfirst(FileNameINF, 0) != 0)
		{
			if(strcmp(FileEnFaute, FileNameINF) != 0)												/*	affichier UNE fois le fichier non trouvÇ */
			{
				form_alert(1,ERREUR_SIZE_INF);
				strcpy(FileEnFaute, FileNameINF);
			}
			graf_mouse(ARROW, NULL);
			return;
		}		
		
		PtrDta		=	Fgetdta();																/*	Alloue un tampon de 125% de la taille */
		SIZETAMPON	=	PtrDta->d_length;
		SIZETAMPON	=	SIZETAMPON + (size_t)(SIZETAMPON / 4);
		
		if((G_TAMPON_INF = malloc( SIZETAMPON)) == NULL )
		{
			form_error(ERROR_MEMORY);
			return;
		}

		size = fread(G_TAMPON_INF, 1, PtrDta->d_length , FichierINF);	

		G_TAMPON_INF[size]	=	0;
		strcpy(INIFILENAME, FileNameINF);
		FileEnFaute[0]		=	0;
		
		fclose(FichierINF);
		graf_mouse(ARROW, NULL);
	}  
	
	/*	Rechercher la Chapitre 'Chapitre' */

	if(Search(Chapitre, Entree, &Position) == ENTRY_FOUND)
	{

		if(*(Position + strlen(Entree) + 1) == 10 || *(Position + strlen(Entree) + 1) == 13)	/* Si dÇbute par \n alors pas de valeurs ! */
		{
			return;
		}
		else
		{			
			strcpy(Format, Entree);
			strcat(Format, "=%s");
			sscanf(Position, Format, ValeurChamp);													/*	lire la valeur */
		}
	}
}

long GetLongProfile(char *FileNameINF, char *Chapitre, char *Entree, long Default)
{
	char	ValString[128];
	char	ValDefault[128];

	ltoa( Default, ValDefault, 10 );

	GetProfileString( FileNameINF, Chapitre, Entree, ValString, ValDefault );
	
	return atol( ValString );
}

/*
 * ----------------------------------------------------------------------------------------------
 *							Ecriture d'une entrÇe INF.
 * ----------------------------------------------------------------------------------------------
 */

void SetProfileString(char *FileNameINF, char *Chapitre, char *Entree, char *ValeurChamp)
{
	char			*Position, Format[257], AncienneValeur[256];
	long			Difference, Retour;
#ifdef	__TRACE__
	char			message[50];
#endif
	
	/*	Si on tente d'ecrire dans le tampon pour un fichier INF inexistant */
	if(G_TAMPON_INF == NULL)
	{
#ifdef	__TRACE__
		form_alert(1,ERROR_NO_INF);
#endif
		G_TAMPON_INF	=	(char *)malloc(SIZE_DEFAULT);
		G_TAMPON_INF[0] = 0;
		strcpy(INIFILENAME, FileNameINF);
		ReadPath( FileNameINF);
	}
	
	Retour = Search(Chapitre, Entree, &Position);
	
	/*	Si l'entrÇe existe pour cette Chapitre */
	if(Retour == ENTRY_FOUND)
	{
		/*	Lire l'ancienne valeur */

		/* Si dÇbute par \n alors pas de valeurs ! (chaine vide) */
		if(*(Position + strlen(Entree) + 1) == 10 || *(Position + strlen(Entree) + 1) == 13)	
		{
			Difference	=	0 - strlen(ValeurChamp);
		}
		/*	Sinon, il y a bien une valeur texte */
		else
		{
			strcpy(Format, Entree);
			strcat(Format, "=%s");
			sscanf(Position, Format, AncienneValeur);		

			Difference	=	strlen(AncienneValeur) - strlen(ValeurChamp);
		}

#ifdef	__TRACE__
			sprintf(message, "[0][ ENTREE = %s => Difference = %ld][Ok]", Entree, Difference);
			form_alert(1,message);
#endif

		if(Difference == 0)																	/*	Nouvelle valeur prends le meme nombre de caractäres */
		{
			strcpy(Format, Entree);															/*	Constiue la nouvelle ligne du fichier */
			strcat(Format, "=");
			strcat(Format, ValeurChamp);
			
			memcpy(Position, Format, strlen(Format));
		}
		else if(Difference > 0)																/*	Nouvelle valeur prends moins de caractäres */
		{
			strcpy(Format, Entree);															/*	Constiue la nouvelle ligne du fichier */
			strcat(Format, "=");
			strcat(Format, ValeurChamp);

			memcpy(Position, Format, strlen(Format));										/*	Ecrire la nouvelle valeur */
			
			/*	DÇcaler le reste du tampon */
			memmove(Position + strlen(Format), Position + strlen(Format) + Difference, strlen(Position) + strlen(Format) + Difference + 1);
		}
		else																				/*	Nouvelle valeur prends plus de caractäres */
		{
			strcpy(Format, Entree);															/*	Constiue la nouvelle ligne du fichier */
			strcat(Format, "=");
			strcat(Format, ValeurChamp);

			if( strlen(G_TAMPON_INF) + strlen(Format) < SIZETAMPON )
			{
				memmove( Position + strlen(Format), Position + strlen(Format) + Difference, 
													strlen(Position) + strlen(Format) + (size_t)abs((int)Difference) + 1 );
				memcpy( Position, Format, strlen(Format));
			}
			else
			{
				AgrandirTampon(strlen(Format));	
				Retour = Search(Chapitre, Entree, &Position);
				if(Retour == ENTRY_FOUND)
				{
					memmove( Position + strlen(Format), Position + strlen(Format) + Difference, 
														strlen(Position) + strlen(Format) + (size_t)abs((int)Difference) + 1 );
					memcpy( Position, Format, strlen(Format));
				}							
			}
		}
	}
	/*	L'entrÇe n'existe pas, il faut la crÇer */
	else if(Retour == NO_ENTRY)
	{
		strcpy(Format, Entree);																/*	Constiue la nouvelle ligne du fichier */
		strcat(Format, "=");
		strcat(Format, ValeurChamp);
		strcat(Format, "\n");
		
		if( strlen(G_TAMPON_INF) + strlen(Format) < SIZETAMPON )
		{
			Position +=	strlen(Chapitre) + 3;
			memmove( Position + strlen(Format), Position, strlen(Position) + 1);
			memcpy( Position, Format, strlen(Format));
		}
		else
		{
			AgrandirTampon(strlen(Format));			
			Retour = Search(Chapitre, Entree, &Position);
			if(Retour == NO_ENTRY)
			{
				Position +=	strlen(Chapitre) + 3;
				memmove( Position + strlen(Format), Position, strlen(Position)  + 1);	
				memcpy( Position, Format, strlen(Format));
			}							
		}
	}
	/*	Pas de Chapitre trouvÇe */
	else
	{
		sprintf(Format, "\n[%s]\n%s=%s\n", Chapitre, Entree, ValeurChamp);					/*	construit le texte de le nouveau Chapitre */
		
		if( (strlen(G_TAMPON_INF) + strlen(Format)) < SIZETAMPON )							/*	Si de place, ajoute dans le tampon */
		{
			strcat(G_TAMPON_INF, Format);			
		}
		else																				/*	Tamon trop petit */
		{
			AgrandirTampon(strlen(Format));			
			strcat(G_TAMPON_INF, Format);													/*	Ajoute enfin le nouveau Chapitre */
		}
	}
}

void SetLongProfile(char *FileNameINF, char *Chapitre, char *Entree, long ValeurChamp)
{
	char	ValueString[128];
	
	ltoa(ValeurChamp, ValueString, 10);
	SetProfileString(FileNameINF, Chapitre, Entree, ValueString);
}

void display(void)
{
	int	i;

	for(i = 0; i < strlen(G_TAMPON_INF); i++)
	{
		printf("%c",G_TAMPON_INF[i]);
	}
}