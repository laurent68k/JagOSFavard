/*
 *
 *				File Unit.
 *	
 *	Author	: FunShip	  
 *	File	: PCFile.c
 *	Date	: 10 February 1992
 *	Revision: 9 November 1994
 *	Version	: 1.00
 *	Language: Pure-C Version 1.1
 *	Origine : France
 *  
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "..\Include\pcfile.h"

#define	TRUE	1
#define	FALSE	0

/*
 * ------------------------- Corps des fonctions -----------------------
 */

 
Type_File *CreerFile(unsigned long Size)
/*
	Alloue une nouvelle file et l'initialise … vide.
	
	Entr‚e:	taille en octets des donn‚es … emfiler
	Sortie: retourne le pointeur sur la file ou NULL si echec
*/
{
  Type_File	*File;
  
  File = (Type_File *)malloc(sizeof(*File));
  if(File != NULL)
  {
    File->Ptr_tete=NULL;   
    File->Ptr_queue=NULL;
    File->Taille=Size;   
  }
  return(File);
}

void KillFile(Type_File *File)
/*
	LibŠre tous les postes de la file si ils existent et d‚truit
	l'objet file.
	
	Entr‚e:	pointeur sur la file
	Sortie:	rien
*/
{
  while(Defiler(File));
  free(File);
}

int EstFileVide(Type_File *File)
/*
	Indique si la file est vide ou non.
	
	Entr‚e:	pointeur sur la file
	Sortie:	retourne 1 si vide 0 sinon
*/
{
  return(File->Ptr_tete == NULL);
}

void *FileSommet(Type_File *File)
/*
	Retourne un pointeur sur la zone de donn‚e du poste en
	tete de file. Si la file est vide la valeur NULL est renvoy‚e.
	
	Entr‚e:	pointeur sur la file
	Sortie:	pointeur (void *) sur la donn‚e ou NULL si file vide
*/
{ 
  Type_FilePoste	*Ptr_poste;
  
  if(!EstFileVide(File))             /* Si y a une donnee */
  {
    Ptr_poste=File->Ptr_tete;                  /* le poste recoit l'@ */
    return(Ptr_poste->Ptr_donnee);             /* retourne le ptr donnees */
  }
  else
    return(NULL); 
}

int Emfiler(Type_File *File, void *Ptr_sdonnee)
/*
	Ajoute un nouvel ‚l‚ment dans la file si assez de m‚moire.
	
	Entr‚e:	pointeur sur la file
		pointeur sur la donn‚e
	Sortie:	retourne 1 si ajout‚ 0 sinon
*/
{
  Type_FilePoste *Ptr_poste;			/* nouveau poste a ajoute */
  Type_FilePoste *Ptr_poste_prec;		/* poste precedent eventuel */
  char           *Ptr_ddonnee;
  
  Ptr_poste=malloc(sizeof(*Ptr_poste));		/* alloue un poste */
  Ptr_ddonnee=malloc(File->Taille);		/* alloue zone de donnees */

  if(Ptr_poste==NULL)				/* si plus de memoire */
  {
    free(Ptr_ddonnee);
    return(FALSE);
  }
  else if(Ptr_ddonnee == NULL)
  {
    free(Ptr_poste);
    return(FALSE);
  }
  else
  {
    if(EstFileVide(File))			/* ajouter le 1ø poste     */
    {
      Ptr_poste->Ptr_suivant=NULL;             /* poste pointe le reste   */
      Ptr_poste->Ptr_precedent=NULL;           /* pas de poste precedent  */
      File->Ptr_tete=Ptr_poste;                /* la tete pointe ce poste */
      File->Ptr_queue=Ptr_poste;
      memcpy(Ptr_ddonnee,Ptr_sdonnee,File->Taille);  /* copie les donnees */  
      Ptr_poste->Ptr_donnee=Ptr_ddonnee;       /* ecrit le ptr de donnees */
    }
    else                                       /* ajouter en fin de liste */
    {
      Ptr_poste->Ptr_suivant=NULL;             /* pas de suivant */
      Ptr_poste->Ptr_precedent=File->Ptr_queue;  /* pointe le precedent  */
      Ptr_poste_prec=File->Ptr_queue;          /* init. @ du precedent */
      Ptr_poste_prec->Ptr_suivant=Ptr_poste;   /* le precedent pointe  */
      File->Ptr_queue=Ptr_poste;               /* la file pointe le dernier */
      
      memcpy(Ptr_ddonnee,Ptr_sdonnee,File->Taille);  /* copie les donnees */  
      Ptr_poste->Ptr_donnee=Ptr_ddonnee;       /* ecrit le ptr de donnees */
    }
    return(TRUE);				/* pas d'erreur */
  }    
}

int Defiler(Type_File *File)
/*
	Supprime le premier ‚l‚ment de la file. Si la file est vide
	aucune op‚ration n'est effectu‚e.
	
	Entr‚e:	pointeur sur la file
	Sortie:	retourne 1 si supprim‚e 0 sinon
*/
{
  char            *Ptr_donnee;
  Type_FilePoste  *Ptr_poste;
  Type_FilePoste  *Ptr_poste_suiv;

  if(EstFileVide(File)) 	              /* si est vide Erreur... */  
    return(FALSE);
  else                                         /* sinon defile */
  {
    Ptr_poste=File->Ptr_tete;                  /* @ du poste a detruire  */
    if(Ptr_poste->Ptr_suivant==NULL)           /* reste juste 1 poste */
    {
      File->Ptr_queue=NULL;                    
      File->Ptr_tete=NULL;                     /* plus de chainages   */
      Ptr_donnee=Ptr_poste->Ptr_donnee;        /* @ des donnees */
    }
    else                                       /* reste plusieur postes */
    {
      Ptr_poste_suiv=Ptr_poste->Ptr_suivant;   /* init. poste suivant   */
      File->Ptr_tete=Ptr_poste_suiv;           /* la tete pointe le suivant */
      
      Ptr_donnee=Ptr_poste->Ptr_donnee;        /* @ des donnees */
    }
    free(Ptr_donnee);                          /* libere zone data */
    free(Ptr_poste);                           /* libere le poste  */
    return(TRUE);
  }
}

int FileEchange(Type_File *File)
/*
	Echange les positions de la premiŠre et derniŠre donn‚e dans
	la file.
	Si la file est vide ou ne possŠde qu'une seule donn‚e la fonction
	n'effectue aucune op‚ration.
	
	Entr‚e:	pointeur sur la file
	Sortie:	retourne 1 si ‚change effectu‚ 0 sinon
*/
{
  Type_FilePoste *tetep;
  Type_FilePoste *queuep;

  if(!EstFileVide(File))			/* file non vide ! */
  {
    if(File->Ptr_tete != File->Ptr_queue)	/* au moins deux postes ! */
    {
      tetep = File->Ptr_tete;
      queuep = File->Ptr_queue;

      File->Ptr_tete = queuep;
      File->Ptr_queue = tetep;

      File->Ptr_tete->Ptr_suivant = tetep->Ptr_suivant;
      tetep->Ptr_suivant = NULL;
      File->Ptr_queue->Ptr_precedent = queuep->Ptr_precedent;
      queuep->Ptr_precedent = NULL;
          
      File->Ptr_tete->Ptr_suivant->Ptr_precedent = File->Ptr_tete;
      File->Ptr_queue->Ptr_precedent->Ptr_suivant = File->Ptr_queue;
    }
    return(TRUE);
  }
  else                                         /* rien faire */
    return(FALSE);
}

int FileTourne(Type_File *File)
/*
	D‚place la donn‚e en tete de file vers la queue de file.
	Si la file est vide ou ne possŠde qu'une seule donn‚e la fonction
	n'effectue aucune op‚ration.
	
	Entr‚e:	pointeur sur la file
	Sortie:	retourne 1 si d‚placement effectu‚ 0 sinon
*/
{
  Type_FilePoste *tetep;
  Type_FilePoste *queuep;

  if(!EstFileVide(File))           /* file non vide ! */
  {
    if(File->Ptr_tete != File->Ptr_queue)      /* au moins deux postes ! */
    {
      tetep = File->Ptr_tete;
      queuep = File->Ptr_queue;

      File->Ptr_tete = File->Ptr_tete->Ptr_suivant;
      File->Ptr_queue = tetep;

      queuep->Ptr_suivant = tetep;
      tetep->Ptr_precedent = queuep;

      File->Ptr_tete->Ptr_precedent = NULL;
      File->Ptr_queue->Ptr_suivant = NULL;
    }
    return(TRUE);
  }
  else                                         /* rien faire */
    return(FALSE);
}

Type_FilePoste *FileFirst(Type_File *File)
/*
	Renvoit l'adresse du poste en tee de la file. Si la file
	est vide l'adresse sera NULL.
	
	Entr‚e:	pointeur sur la file
	Sortie:	pointeur sur le poste ou NULL
*/
{
  return(File->Ptr_tete);
}

Type_FilePoste *FileNext(Type_FilePoste *Ptr_poste)
/*
	Renvoit l'adresse du poste qui suit celui dont l'adresse est
	pass‚e en paramŠtre, si il existe.
	
	Entr‚e:	pointeur sur le poste actuel
	Sortie:	pointeur sur le poste suivant ou NULL
*/
{
  return(Ptr_poste->Ptr_suivant);
}

Type_FilePoste *FilePrevious(Type_FilePoste *Ptr_poste)
/*
	Renvoit l'adresse du poste qui pr‚cŠde celui dont l'adresse est
	pass‚e en paramŠtre, si il existe.
	
	Entr‚e:	pointeur sur le poste actuel
	Sortie:	pointeur sur le poste pr‚c‚dent ou NULL
*/
{
  return(Ptr_poste->Ptr_precedent);
}

/*
  End of library
*/
