/*
 * ----------------------------------------------------------------------------------------------
 *			Module de gestion des s‚lecteurs de fichiers et d'appel un PRG externe
 *
 * 	Author  : 	FAVARD Laurent, FunShip (c)
 *	File    : 	TOOLS.C
 *	Date    : 	6  Avril 1994
 *	Release : 	10 December 1996
 *	Version : 	1.0
 *	Country : 	FRANCE
 *			
 *
 *				- Copyright Atari FunShip (c) 1994-96 -
 *	 		     - ATARI ST,STE,TT, Falcon, Medusa et Hades-
 * ----------------------------------------------------------------------------------------------
 */

#include <tos.h>
#include <stdio.h>
#include <string.h>

#include <PcAads.h>
#include "Tools.h"

/*
 * ----------------------------------------------------------------------------------------------
 */

#define		LOADGO			0
#define		TOS14			0x0104

/*
 * ----------------------------------------------------------------------------------------------
 *				S‚lecteur de fichier pour parcourir
 * ----------------------------------------------------------------------------------------------
 */

static void FselInput(char *Prefix, char *Dummy, int *Bouton, char *Title)
/*
	Appel le s‚lecteur de fichiers ‚tendue (avec un titre) si le TOS actuel le permet.
	(TOS >= 1.4)
*/
{
  if(MchInfo.TosVersion < TOS14)
    fsel_input(Prefix,Dummy,Bouton);
  else
    fsel_exinput(Prefix,Dummy,Bouton, Title); 
}

void GetPath(char *Chemin,char *Label)
/*	
	Appel le s‚lecteur de fichiers pour designer un chemin complet r‚pertoire.
	Si un nouveau est valid‚, Chemin prend cette valeur, sinon Chemin conserve
	son ancienne valeur.
*/
{
  char	Prefix[PATHLONG],Dummy[14],*Prochain;
  int	Bouton;
    
  strcpy(Prefix,Chemin);									/* Lit le chemin par pass‚ */
  if(strlen(Prefix) == 0)									/* Si vide */
  {
    Prefix[0] = Dgetdrv()+'A';									/* Disque courant */
    Prefix[1] = ':';
    Dgetpath(Prefix+2,0);									/* Chemin courant */
  }
  strcat(Prefix,"\\*.*");
  strcpy(Dummy,"\0");

  FselInput(Prefix,Dummy,&Bouton,Label);
  if(Bouton)
  {
    Prochain = strrchr(Prefix,'\\');
    *(Prochain) = '\0';										/* Supprimer le \*.* … la fin */
    
    strcpy(Chemin,Prefix);									/* Recopier le nouveau chemin */
  }
}

/*
 * ----------------------------------------------------------------------------------------------
 *				Appel d'un porgramme externe … l'application
 * ----------------------------------------------------------------------------------------------
 */

void CallApplication(char *AppPathName, char *Parametre)
/*
	Appel de l'‚diteur de texte s‚par‚ avec ou sans un fichier … ‚diter.
*/
{
  char	OldPath[PATHLONG],NewName[PATHLONG];
  int	OldLecteur;
  char	*Pointeur,envp[1]="\0";

  OldLecteur = Dgetdrv();										/* Lecteur Actuel */
  OldPath[0] = Dgetdrv()+'A';										/* Lettre du lecteur actuel */
  OldPath[1] = ':';
  Dgetpath(OldPath+2,0);										/* Chemin courant */
  
  if(strlen(Parametre) != 0)										/* Si un fichier est donn‚ */
  {
    Pointeur = strrchr(Parametre,'\\'); 								/* Set new path */
    if(Pointeur != NULL)
    {
      Dsetdrv(Parametre[0]-'A');									/* Fixe le lecteur du source */
      *Pointeur = '\0';
      Dsetpath(Parametre);										/* Path is the file's path */
      *Pointeur = '\\';
    }
    else
      Dsetpath("\\");											/* else main root path */

    strcpy(NewName," ");										/* Insert a space before !! */
    Pointeur = strrchr(Parametre,'\\');									/* searching last \ */
    strcat(NewName,Pointeur+1);										/* Extraire que le nom fichier */
    Pexec(LOADGO,AppPathName,NewName,envp);								/* Call editer with file name */
    Dsetdrv(OldLecteur);										/* Fixe l'ancien lecteur */
  }
  else
    Pexec(LOADGO, Parametre, envp, envp);								/* Call editer without file name */
    
  Dsetpath(OldPath);											/* Restore previous current path */
}

/*
 * ----------------------------------------------------------------------------------------------
 *				Gestion fichiers INF de l'application
 * ----------------------------------------------------------------------------------------------
 */

int GetProfileInf(char *FileNameINF)
{
  FILE		*Fichier;
  char		Texte[30], Message[100];
  int		Nombre;
  
  if((Fichier = fopen(FileNameINF, "r")) != NULL)
  {
    fscanf(Fichier,"ToolBarActive=%s\n", Texte);
    fscanf(Fichier,"FileBuffer=%d\n",&Nombre);

    sprintf(Message, "[0][ %s | %d ][Ok]", Texte, Nombre);
    form_alert(1, Message);
    
    fclose(Fichier);
    return(TRUE);
  }
  else
    return(FALSE);

}

void SetProfileInf(char *FileNameINF)
{
  FILE		*Fichier;

  graf_mouse(BUSYBEE,NULL);
  
  if((Fichier = fopen(FileNameINF, "w")) != NULL)
  {

    fprintf(Fichier,"ToolBarActive=Yes\n");
    fprintf(Fichier,"FileBuffer=256\n");			

    fclose(Fichier);
  }
  graf_mouse(ARROW,NULL);
}
