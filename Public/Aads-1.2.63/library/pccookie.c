/*
 *			Fichier C des primitives sur les cookies
 *
 *	Auteur	: FunShip
 *	Fichier	: PCcookie.c
 *	Date	: 8 D‚cembre 1993
 *	Revision: 15 Mai 1994
 *	Version	: 1.00
 *	Release	: 1.00
 *	Langage	: Pure C
 */

#include <stdio.h> 
#include <tos.h>

#include "..\Include\PCcookie.h"

/* ----------------------------------------------------------------
 *
 *   Fonction d‚finie dans le fichier "CookRst.s"
 *
 * # Routine d'installation de la routine CookieReset 
 *   Elle installe la routine CookieReset en tant que S/P Reset TOS
 *
 * # Routine Reset de la variable 0x5A0 pour les TOS < 1.6
 *   Cette routine doit etre appel‚e lors d'un Reset par le TOS, 
 *   elle efface le contenu de la variable _cookie qui n'existait
 *   pas … "l'‚poque" et supprime sa propre installation ensuite.
 *
 * -----------------------------------------------------------------
 */

void Install_Reset(void);

/*
 *--------------------------- Primitives priv‚es -------------------------
 */			   

static int CmpIdt(char chaine1[4],char chaine2[4])
/*
  Compare chaine1 et chaine2 sur 4 octets uniquement. Retourne 1 si ‚galit‚
  sinon 0.
*/
{
  int indice,
      egal;
  
  egal=1;
  for(indice=0;indice<4;indice++)
  {
    if(chaine1[indice] != chaine2[indice])
      egal = 0;
  }
  return(egal);
}

/*
 *--------------------------- Primitives publiques -----------------------
 */			   

Type_Cookie *J_GetAdr(void)
/*
	Retourne l'adresse de d‚part du cookie jar systŠme.
	Si inexistant retourne NULL.
  
  	Entr‚e:	rien
  	Sortie:	pointeur sur le premier cookie du cookie jar
*/
{
  Type_Cookie *adr_cookiejar;
  long        pile;
  
  pile = Super(0L);
  adr_cookiejar = *(Type_Cookie **)0x5A0;
  Super((void *)pile);
  if(adr_cookiejar == 0L)
    return( (Type_Cookie *)NULL );  			   
  else
    return( adr_cookiejar );
}

int J_Exist(void)
/*
	Indique si il existe un cookie jar dans le TOS de la machine.
	Tous les TOS post‚rieurs au TOS 1.6 doivent normallement 
	avoir un cookie jar.
	
	Entr‚e:	rien
	Sortie:	retourne 1 si oui 0 sinon
*/
{
  return( J_GetAdr() != NULL ); 
}

int J_Read(Type_Cookie *liste)
/*
	Recopie dans la liste de cookies vide pass‚e en paramŠtre, 
	tout le cookie jar du systŠme y compris le cookie nul.
	Si le cookie jar n'existe pas aucune op‚ration n'est effectu‚e.
	
	Entr‚e:	liste de cookies vide utilisateurs
	Sortie: retourne 1 si effectu‚ 0 sinon
*/
{
  Type_Cookie *my_cookie;
  
  my_cookie = J_GetAdr();
  if(my_cookie != NULL)			/* Existe Cookie Jar */
  {
    while( CmpIdt(my_cookie->identification,"\0\0\0\0") != 1 )
    {
      *(long *)liste->identification = *(long *)my_cookie->identification;
      liste->information.mot_long = my_cookie->information.mot_long;
      my_cookie++;
      liste++;
    }
    *(long *)liste->identification = *(long *)my_cookie->identification;
    liste->information.mot_long = my_cookie->information.mot_long;
    return(1);
  }
  else										/* Pas de Cookie Jar */
    return(0);
}

int J_Install(int nombre_slots)
/*
	Installe un nouveau cookie jar de 'nombre_slots' slots. 
	Si il existait d‚j… un cookie jar, recopie automatiquement 
	tous les cookies.
	Si plus assez de m‚moire la fonction est abandonn‚e.
	Pour les TOS ant‚rieur au 1.6, installe ‚galement une 
	routine reset de la variable systŠme long 0x5A0.
	
	Entr‚e:	taille du cookie jar en nombre de slots
	Sortie:	retourne 1 si effectu‚ 0 sinon.
*/
{
  Type_Cookie *new_cookiejar;
  Type_Cookie *cur_cookie;
  long        pile;
  int         ok;
  
  new_cookiejar = (Type_Cookie *) Malloc((long)(sizeof(Type_Cookie)
  						* nombre_slots)
  					);
  cur_cookie    = new_cookiejar;
  if(new_cookiejar!=NULL)						/* Malloc reussi */
  {
    /* Recopie l'ancien Cookie Jar si il existait */
    if( J_Read(new_cookiejar) )	    
    {	    												 
      while( CmpIdt(cur_cookie->identification,"\0\0\0\0") != 1 )
      {
        cur_cookie++;
      }
      /* Sortie: - Cookie NUL trouv‚ */
      cur_cookie->information.mot_long = (long)(nombre_slots);
    }
    else				/* Pas de cookie jar auparavant */
    {
      *(long *)cur_cookie->identification = (long)0L;
      cur_cookie->information.mot_long = (long)nombre_slots;
      					/* Installer la routine Reset pour les ST */
      pile = Super(0L);
      Install_Reset(); 
      Super((void *)pile);
    }
    /* Installer en 0x5A0, le nouveau */
    pile = Super(0L);
    *(Type_Cookie **)0x5A0 = new_cookiejar;
    Super((void *)pile);
    ok = 1;
  }
  else
    ok = 0;
  return(ok);
}

int J_Number(void)
/*
	Retourne le nombre de cookies effectivement pr‚sent dans 
	le cookie jar du systŠme sans compter le cookie nul.
	Si pas de cookie jar la fonction est abandonn‚e.
	
	Entr‚e:	rien
	Sortie: nombre de cookies dans le systŠme
*/
{
  long *my_cookie;
  int  nb_slots;
  
  my_cookie = (long *)J_GetAdr();
  nb_slots  = 0;
  if(my_cookie != NULL)			/* Existe Cookie Jar */
  {
    while( *my_cookie != 0L )
	{
	  my_cookie++;
	  my_cookie++;
	  nb_slots++;
	}
    /* Sortie: cookie nul trouv‚ */
  }
  return(nb_slots);    
}

long J_Size(void)
/*
	Retourne la taille du cookie jar systŠme en nombre de 
	slots incluant le slot du cookie Nul.
	Si pas de cookie jar la fonction est abandonn‚e.
	
	Entr‚e:	rien
	Sortie: nombre de slots
*/
{
  long *my_cookie;
  long nb_slots;
  
  nb_slots  = 0L;
  my_cookie = (long *)J_GetAdr();
  if(my_cookie != NULL)			/* Existe Cookie Jar */
  {
    while( *my_cookie != 0L )
    {
      my_cookie++;
      my_cookie++;
    }
    /* Sortie: cookie nul trouv‚ */
    my_cookie++;
    nb_slots = *my_cookie;
  }
  return(nb_slots);    
}

int J_Search(char identification[4],Type_Cookie *cookie)
/*
	Recherche dans le cookie jar du systŠme, le cookie dont 
	l'identification correspond … l'identification pass‚e en 
	paramŠtre. 
	Si succŠs, retourne dans le cookie pass‚ en paramŠtre la
	copie du cookie trouv‚ en m‚moire.
	Si pas de cookie jar la fonction est abandonn‚e.
	
	Entr‚e:	
	Sortie:	
*/
{
  Type_Cookie *my_cookie;
  int         ok;
  
  my_cookie = J_GetAdr();
  if(my_cookie != NULL)			/* Existe Cookie Jar */
  {
    while( CmpIdt(my_cookie->identification,identification) != 1 &&
    	   *(long *)my_cookie != 0L )
    {
      my_cookie++;
    }
    /* Sortie: cookie trouv‚ ou cookie nul */
    if(CmpIdt(my_cookie->identification,identification))	/* Trouv‚ */
    {
      *(long *)cookie->identification = *(long *)my_cookie->identification;
      cookie->information.mot_long = my_cookie->information.mot_long;
      ok = 1;
    }
    else									/* Non trouv‚ */
      ok = 0;
  }											/* Pas de Cookie Jar */
  else
    ok = 0;
  return(ok);
}

int J_Insert(Type_Cookie *new_cookie)
/*
	InsŠre dans le cookie jar systŠme, le nouveau cookie pass‚
	en parametre.
	Le cookie est recopi‚ dans le cookie jar en m‚moire.
	Si le cookie existe d‚j… ou si le cookie Jar est plein ou bien
	si il n'existe pas de cookie jar, la fonction est abandonn‚e.
	
	Entr‚e:	pointeur sur le nouveau cookie
	Sortie:	retourne 1 si ins‚r‚ 0 sinon
*/
{
  Type_Cookie *my_cookie,
              *next_cookie;
  int         ok;
  long        nb_cookie;
  
  nb_cookie  = 0L;
  my_cookie = J_GetAdr();
  if(my_cookie != NULL)			/* Existe Cookie Jar */
  {
    while( CmpIdt(my_cookie->identification,
    		  new_cookie->identification) != 1 &&
    	   *(long *)my_cookie != 0L )
    {
      my_cookie++;
      nb_cookie++;
    }
    /* 
     * Sortie: 1- Cookie … ins‚rer existe d‚j… !!
     *         2- Cookie Nul trouv‚ et nb_cookie = nombre de cookie compt‚
     */
    if( CmpIdt(my_cookie->identification,new_cookie->identification) )	
    {
      ok = 0;
    }
    else									/* Cookie non d‚j… pr‚sent */
    {
      if( nb_cookie+1 < my_cookie->information.mot_long )
      {
      	next_cookie = my_cookie;	/* D‚place le cookie nul */
      	next_cookie++;
        *(long *)next_cookie->identification = 
    				*(long *)my_cookie->identification;
        next_cookie->information.mot_long = my_cookie->information.mot_long;
        
					/* Ins‚rer le nouveau */        
        *(long *)my_cookie->identification = 
        			*(long *)new_cookie->identification;
        my_cookie->information.mot_long = new_cookie->information.mot_long;
        ok = 1;
      }
      else									/* Plus de place */
        ok = 0;
    }
  }											/* Pas de Cookie Jar */
  else
    ok = 0;
  return(ok);
}

int J_Delete(char identification[4])
/*
	Supprime un cookie pr‚sent dans le cookie jar systŠme,
	si le cookie jar existe et que le cookie est effectivement
	trouv‚.
	
	Entr‚e:	identification du cookie … d‚truire
	Sortie:	retourne 1 si supprim‚ 0 sinon
*/
{
  Type_Cookie *cur_cookie,
              *next_cookie;
  int         ok;
                
  cur_cookie = J_GetAdr();
  if(cur_cookie != NULL)						/* Existe Cookie Jar */
  {
    while( CmpIdt(cur_cookie->identification,identification) != 1 &&
    	   *(long *)cur_cookie != 0L )
    {
      cur_cookie++;
    }
    /* 
     * Sortie: 1- Cookie … supprimer trouv‚
     *         2- Cookie Nul trouv‚ 
     */
    										/* Identification correct */
    if( CmpIdt(cur_cookie->identification,identification) )
    {
      next_cookie = cur_cookie;
      next_cookie++;
      while( CmpIdt(cur_cookie->identification,"\0\0\0\0") !=1 )
      {
	*(long *)(cur_cookie->identification) = *(long *)next_cookie->identification;
	cur_cookie->information.mot_long = next_cookie->information.mot_long;
	next_cookie++;
	cur_cookie++;
      }
      ok = 1;
    }
    else									/* Cookie non trouv‚ */
      ok = 0;
  }
  else										/* Pas de Cookie Jar ! */
    ok = 0;
  return(ok);     
}

/* End cookie.c */
