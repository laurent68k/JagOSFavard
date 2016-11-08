/*
 *
 *				Stack Unit.
 *	
 *	Author	: FunShip	  
 *	File	: PCPile.c
 *	Date	: 8 February 1992
 *	Revision: 28 juillet 1997
 *	Version	: 2.00
 *	Language: Pure-C Version 1.1
 *	Origine : France
 *  
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "..\Include\pcpile.h"

#define	TRUE	1
#define	FALSE	0

/*
 * ------------------------- Corps des fonctions -----------------------
 */

Type_Pile *CreerPile(unsigned long size)
/*
	Alloue un objet de type pile et l'initialise … vide. Si plus
	de m‚moire disponible la fonction retourne la valeur NULL.
	
	Entr‚e:	size est la taille des donn‚es en octets 
		qui seront empil‚es.
	Sortie:	retourne un pointeur sur l'objet pile
*/
{
  Type_Pile	*pile;
  
  pile  = (Type_Pile *)malloc((long)sizeof(Type_Pile));	/* Cr‚er tete */
  if(pile != NULL)					/* Assez de m‚moire */
  {
    pile->Entree=NULL;					/* Pas de poste */
    pile->Taille=size;					/* Taille des donn‚es */
  }
  return(pile);   
}

void KillPile(Type_Pile *pile)
/*
	Vide la pile si elle est non vide et d‚truit l'objet pile.
	
	Entr‚e:	pointeur sur l'objet pile
	Sortie: rien
*/
{
  while(Depiler(pile));				/* Tant que postes */
  						/* D‚piler */
  free(pile);					/* d‚truire la pile */
}

int EstPileVide(Type_Pile *pile)
/*
	Indique si la pile est vide ou non.
	
	Entr‚e:	pointeur sur la pile
	Sortie: retourne 1 si vide 0 sinon
*/
{
  return(pile->Entree == NULL);  
}

void *PileSommet(Type_Pile *pile)
/*
	Retourne un pointeur sur la zone de donn‚e actuellement
	en sommet de pile.
	
	Entr‚e:	pointeur sur la pile
	Sortie: retourne un pointeur (void *) sur la zone
*/
{ 
	if(pile->Entree != NULL) 	            /* Si y a une donnee */
		return(pile->Entree->PtrDonnee);
	else
		return(NULL); 
}

int Empiler(Type_Pile *pile,void *ptr_sdonnee)
/*
	Empile une nouvelle donn‚e, si de la m‚moire est encore disponible.
	Si plus de m‚moire l'empilage n'est pas effectu‚.
	
	Entr‚e: pointeur sur la pile
		pointeur sur la donn‚e … empiler
	Sortie:	retourne 1 si empil‚ 0 sinon
*/
{
	Type_PilePoste	*ptr_poste;
	char			*ptr_ddonnee;
  
	ptr_poste=(Type_PilePoste *)malloc((long)sizeof(Type_PilePoste));	/* alloue un poste */
	ptr_ddonnee=malloc(pile->Taille);		/* alloue zone de donnees */

	if(ptr_poste==NULL)
	{
		free(ptr_ddonnee);
		return(FALSE);
	}
	else if(ptr_ddonnee == NULL)
	{
		free(ptr_poste);
		return(FALSE);
	}
	else
	{
		ptr_poste->PtrPrecedent		=	NULL;				/*	nouveau pas de precedent */
		ptr_poste->PtrSuivant		=	pile->Entree;		/*	poste pointe le reste */
	
		if(	pile->Entree != NULL)							/*	If exist previous poste */
			pile->Entree->PtrPrecedent	=	ptr_poste;
			
		pile->Entree				=	ptr_poste;			/*	la tete pointe ce poste */
	
		memcpy(ptr_ddonnee, ptr_sdonnee, pile->Taille);		/*	copie les donnees */  
		ptr_poste->PtrDonnee		=	ptr_ddonnee;		/*	ecrit le ptr de donnees */
		
		return(TRUE);				/* pas d'erreur */
	}    
}

int Depiler(Type_Pile *pile)
/*
	D‚pile la donn‚e au sommet de la pile. Si la pile est vide aucun 
	d‚pilage n'est effectu‚.
	
	Entr‚e:	pointeur sur la pile
	Sortie:	retourne 1 si d‚pil‚e 0 sinon
	
*/
{
  Type_PilePoste  *ptr_poste;

  if(pile->Entree==NULL)										/* Si est vide rien faire */  
    return(FALSE);
  else															/* Sinon on d‚pile */
  {
    ptr_poste		=	pile->Entree;							/* @ du poste   */
    pile->Entree	=	ptr_poste->PtrSuivant;					/*	Extrait le poste suivant */
    if(pile->Entree != NULL)									/*	Si poste suivant existait */
		pile->Entree->PtrPrecedent	=	NULL;

    free(ptr_poste->PtrDonnee);									/* Libere zone data */
    free(ptr_poste);											/* Libere le poste  */
    return(TRUE);
  }
}

Type_PilePoste *PileFirst(Type_Pile *Pile)
/*
	Renvoit l'adresse du poste en tete de la pile. Si la pile
	est vide l'adresse sera NULL.
	
	Entr‚e:	pointeur sur la file
	Sortie:	pointeur sur le poste ou NULL
*/
{
	return(Pile->Entree);
}

Type_PilePoste *PileNext(Type_PilePoste *Ptr_poste)
/*
	Renvoit l'adresse du poste qui suit celui dont l'adresse est
	pass‚e en paramŠtre, si il existe.
	
	Entr‚e:	pointeur sur le poste actuel
	Sortie:	pointeur sur le poste suivant ou NULL
*/
{
	if(Ptr_poste != NULL)
		return(Ptr_poste->PtrSuivant);
	return NULL;
}

Type_PilePoste *PilePrevious(Type_PilePoste *Ptr_poste)
/*
	Renvoit l'adresse du poste qui pr‚cŠde celui dont l'adresse est
	pass‚e en paramŠtre, si il existe.
	
	Entr‚e:	pointeur sur le poste actuel
	Sortie:	pointeur sur le poste pr‚c‚dent ou NULL
*/
{
	if(Ptr_poste != NULL)
 		return(Ptr_poste->PtrPrecedent);
	return NULL;
}

int PileDelete(Type_Pile *pile, Type_PilePoste *Ptr_poste)
/*
	D‚pile le poste present dans la pile. Si la pile est vide aucune
	suppression n'est effectuee;
	
	Entr‚e:	pointeur sur la pile
	Sortie:	retourne 1 si d‚pil‚e 0 sinon
	
*/
{
  Type_PilePoste  *Poste;

  if(pile->Entree == NULL)									/* Si est vide rien faire */  
    return(FALSE);
  else						
  {
    if(Ptr_poste->PtrPrecedent != NULL)								/*	Le poste n'est pas le premier */
	{
		Poste				=	Ptr_poste->PtrPrecedent;
		Poste->PtrSuivant	=	Ptr_poste->PtrSuivant;
		
		if(Ptr_poste->PtrSuivant != NULL)							/*	Si prochain poste existe */
		{
			Poste				=	Ptr_poste->PtrSuivant;
			Poste->PtrPrecedent	=	Ptr_poste->PtrPrecedent;
		}
	}
	else															/*	Le poste est le premier */
	{
		pile->Entree				=	Ptr_poste->PtrSuivant;		/* Extrait le poste */
		if(Ptr_poste->PtrSuivant != NULL)							/*	Si prochain poste existe */
			pile->Entree->PtrPrecedent	=	NULL;
	}
	free(Ptr_poste->PtrDonnee);										/* Libere zone data */
	free(Ptr_poste);												/* Libere le poste  */		
    return(TRUE);
  }
}
