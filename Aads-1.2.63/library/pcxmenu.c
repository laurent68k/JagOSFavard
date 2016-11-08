/*
 *		Module de gestion des Menus et Sous-Menus d‚roulant.
 *
 *
 *	Author  : 	FunShip
 *	File	: 	PCXmenu.c
 *	Date    : 	01 October 1991
 *	Revision: 	13 january 1997
 *	Version : 	1.20
 *	Release : 	2
 *	Language: 	Pure C, Version du 3 Fevrier 1992 
 *	Origine : 	FRANCE
 *	
 *
 */

#include <tos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "..\include\PCXmenu.h"
#include "..\include\PCFile.h"
#include "..\include\PCXWind.h"

#include "Window.h"
#include "Tools.h"

/*
 * --------------------- publics datas and structures --------------------
 */

/* 
   When an entry's object is selected, you could get its Object's tree
   address parent and its number Entry
 */
   
OBJECT		*M_Address;																					/* Given Current Tree Menu Address */
int			M_Name;																						/* Given int Name Menu */
int			M_Entry;																					/* Given int Current Entry */

/*
 * -----------------------------------------------------------------------------------------
 *				Privates global defines
 */

#define ERROR_CODE	"[0][      PCXMenu Warning: | No Code to call ][ok]"

#define	TRUE		1
#define	FALSE		0

#define	AES_SUBMENU	0x340
#define	MU_NULL		0

/*
 * -----------------------------------------------------------------------------------------
 *				Privates Datas Types
 */
 
typedef struct	{
		  OBJECT	*RootTree;		/* Root Menu Address  */
		  int		RootName;		/* Root Menu name     */
		  OBJECT 	*Tree;			/* Tree Menu Adress   */
		  int		Name;			/* Name of Menu       */
		  int		Entry;			/* Indicie Entry      */
		  int		Key;				/* For the Key Event  */

		  void (*Procedure)(void);			/* Process associated */
		} Type_Entry;

typedef struct	{
		  OBJECT	*Tree;				/* Sub Menu */
		} Type_Sub_Checked;

/*
 * -----------------------------------------------------------------------------------------
 *				Privates Datas Structures
 */
		
static	Type_File	*File_Event;				/* List for Event */
static	Type_File	*File_SubMenu_Checked;			/* List for Check */

/*
 * -----------------------------------------------------------------------------------------
 *				Miscallenous publics procedures
 */

int M_Checked(int Tree,int Entry)
/*
	Place un CheckMark sur l'entr‚e Entry du menu Tree.
	
	Entr‚es:	indice du menu
			indice de l'entr‚e
	Sorties:	True si r‚ussi
*/
{
  OBJECT	*Arbre;
  
  if(!rsrc_gaddr(R_TREE,Tree,&Arbre))
    return(FALSE);
  menu_icheck(Arbre,Entry,TRUE);  
  return(TRUE);
}


int M_UnChecked(int Tree,int Entry)
/*
	EnlŠve un CheckMark sur l'entr‚e Entry du menu Tree.
	
	Entr‚es:	indice du menu
			indice de l'entr‚e
	Sorties:	True si r‚ussi
*/
{
  OBJECT	*Arbre;
  
  if(!rsrc_gaddr(R_TREE,Tree,&Arbre))
    return(FALSE);
  menu_icheck(Arbre,Entry,FALSE);  
  return(TRUE);
}

int M_Enable(int Tree,int Entry)
/*
	Active une entr‚e Entry du menu Tree.
	
	Entr‚es:	indice du menu
			indice de l'entr‚e
	Sorties:	True si r‚ussi
*/
{
  OBJECT	*Arbre;
  
  if(!rsrc_gaddr(R_TREE,Tree,&Arbre))
    return(FALSE);
  menu_ienable(Arbre,Entry,TRUE);
  return(TRUE);
}
  
int M_Disable(int Tree,int Entry)
/*
	D‚sactive une entr‚e Entry du menu Tree.
	
	Entr‚es:	indice du menu
			indice de l'entr‚e
	Sorties:	True si r‚ussi
*/
{
  OBJECT	*Arbre;
  
  if(!rsrc_gaddr(R_TREE,Tree,&Arbre))
    return(FALSE);
  menu_ienable(Arbre,Entry,FALSE);
  return(TRUE);
}

int M_Change(int Tree,int Entry,char *string)
/*
	Remplace le texte de l'entr‚e du menu par une nouvelle chaine. Cette chaine
	doit avoir une longueur maximum ‚gale … la pr‚c‚dente.
	
	Entr‚es:	Indice du menu
			Entr‚e … modifier
			Chaine texte
	Sorties:	True si r‚ussi
*/
{
  OBJECT	*Arbre;
  
  if(!rsrc_gaddr(R_TREE,Tree,&Arbre))
    return(FALSE);
  menu_text(Arbre,Entry,string);
  return(TRUE);
}

void M_SetSubMenu(int Size)
/*
	Indique le nombre d'entr‚es de Sous Menu … afficher.Par d‚faut l'AES initialise
	la valeur … 16.
	
	Entr‚es:	Size = nombre d'entr‚es
	Sorties:	rien
*/
{
  MN_SET	Settings;
  
  Settings.Display	= 50;
  Settings.Drag		= 200;
  Settings.Delay	= 10;
  Settings.Speed	= 100;
  Settings.Height	= Size;
  menu_settings(TRUE,&Settings);
}

/*
 * -----------------------------------------------------------------------------------------
 *				Miscallenous privates procedures
 */

static void MoveCheck(OBJECT *Arbre,int Entry)
/*
	D‚place le CheckMark de la position courante sur la nouvelle entr‚e de
	Menu Entry, qui devient l'entr‚e active
	
	Entr‚es:	Pointeur sur l'arbre d'objets menu
			Entier identifiant l'entr‚e
	Sortie:		rien
*/
{
  int	ExOption;
  
  ExOption = menu_istart(FALSE,Arbre,ROOT,TRUE);		/* Lire entr‚e active */
  Arbre[ExOption].ob_state &=~CHECKED;				/* Enlever le CheckMark */

  menu_istart(TRUE,Arbre,ROOT,Entry);				/* Nouvelle entr‚e active */
  Arbre[Entry].ob_state |=CHECKED;				/* Placer le CheckMark */
}

static int WorkWithCheck(OBJECT *Tree)
/*
	Indique si un Sous-Menu fonctionne avec un CheckMark automatique.
	
	Entr‚es:	Adresse de l'arbre d'objets Sous-Menu
	Sorties:	True si utilise un CheckMark Automatique
*/
{
  Type_Sub_Checked	*Poste;
  OBJECT		*Initial_adresse;
  
  if(!EstFileVide(File_SubMenu_Checked))		/* Si la liste n'est pas vide */
  {
    Poste = FileSommet(File_SubMenu_Checked);		/* Lire le premier Sous-Menu */
    if(Poste->Tree == Tree)				/* Si c'est celui cherch‚ */
      return(TRUE);					/* Retourner Vrai */
    else						/* Sinon le chercher */
    {
      Initial_adresse = Poste->Tree;			/* Noter le premier Sous-Menu */
      FileTourne(File_SubMenu_Checked);			/* Prochain Sous-Menu */
      Poste = FileSommet(File_SubMenu_Checked);		/* Le lire */
      while(!(Poste->Tree == Tree || Initial_adresse == Poste->Tree))
      {
        FileTourne(File_SubMenu_Checked);		/* Prochain Sous-Menu */
        Poste = FileSommet(File_SubMenu_Checked);	/* Le lire */	
      }
      /*
       *	Address of Tree found or not !
       */
      return(Poste->Tree == Tree);			/* Si c'est celui cherch‚ => Vrai */
    }
  }
  else							/* La liste est vide */
    return(FALSE);					/* donc Faux */
}

/*
 * -----------------------------------------------------------------------------------------
 *				Publics Aads users procedures:
 *				For Main Menu
 */

int M_Init(void)
/*
	Initialise le gestionnaire de Menus et Sous-Menus.
	Entr‚es:	rien
	Sorties:	rien
*/
{
  /*
   *	Create mains structures: Liste des ‚vŠnements et des menus fonctionnant
   *	avec un CheckMark automatique.
   */
  File_Event		= CreerFile(sizeof(Type_Entry));
  File_SubMenu_Checked	= CreerFile(sizeof(Type_Sub_Checked));
  
  return(File_Event != NULL && File_SubMenu_Checked != NULL);
}

int M_Exit(void)
/*
	Termine une session avec la sous-librairie
	
	Entr‚es:	rien
	Sorties:	True si bien ex‚cut‚ sinon False
*/
{
  while(Defiler(File_Event));
  while(Defiler(File_SubMenu_Checked));

  KillFile(File_Event);
  KillFile(File_SubMenu_Checked);
  return(TRUE);
}

int M_Open(int Tree,Type_Parameter_Menu *Liste)
/*
	Ouvre un Menu principal (Racine).Celui qui est log‚ en haut de l'‚cran
	dans la barre de menu GEM.
	
	Entr‚es:	Entier indentifiant le Menu
			Liste de cr‚ation d'un Menu
	Sorties:	True si r‚ussi
*/
{
  OBJECT	*Arbre;
  Type_Entry	Poste;
  unsigned long	Indice;
    
  if(!rsrc_gaddr(R_TREE,Tree,&Arbre))
    return(FALSE);
  /*
   *	Insert into File_Event the object that call process when
   *	an event occurs.
   */
  Poste.Tree  = Arbre;						/* Copy its Tree Address */
  Poste.Name  = Tree;						/* Copy its Name */
  Poste.RootTree = Arbre;					/* Copy it to Root himself */
  Poste.RootName = Tree;					/* And its name */
  Indice = 0L;							/* First list's atom */
  while(!(Liste[Indice].Entry     == 0    &&
  	  Liste[Indice].Procedure == NULL &&
  	  Liste[Indice].State     == 0))
  {
    Poste.Entry = Liste[Indice].Entry;				/* Copy indicie entry */
    Poste.Procedure = Liste[Indice].Procedure;			/* Address of procedure */
    								/* Quick Access key */
    Poste.Key = CalculKey(Arbre[Poste.Entry].ob_spec.free_string);	
    if(!Emfiler(File_Event,&Poste))			     	/* Insert new data */
      return(FALSE);
      								/* Entry initial State */
    Arbre[Poste.Entry].ob_state |= Liste[Indice].State; 
    Indice++;
  }
  menu_bar(Arbre,TRUE);						/* Show Menu Tree */
  return(TRUE);
}

int M_Close(int Tree)
/*
	Ferme le Menu principal et d‚truit les structures de donn‚es.
	
	Entr‚es:
	Sorties:
*/
{
  OBJECT	*Arbre;
    
  if(!rsrc_gaddr(R_TREE,Tree,&Arbre))				/* Adresse du Menu Principal */
    return(FALSE);
  menu_bar(Arbre,FALSE);					/* Hide Menu Tree */
  return(TRUE);
}

/*
 * -----------------------------------------------------------------------------------------
 *				Publics Aads users procedures:
 *				For Sub-Menus
 */

int M_SubAttach(int Tree,Type_Parameter_SubAttach *Liste)
/*
	R‚alise l'attachement de Sous-Menus … des entr‚es du Menu Principal.
		
	Entr‚es:
	Sorties:
*/

{
  OBJECT		*Arbre,  
  			*SousArbre;
  MENU			SubMenu;
  unsigned long		Indice;
  Type_Sub_Checked	Poste;
  
  if(!rsrc_gaddr(R_TREE,Tree,&Arbre))		/* Adresse du Menu racine */
    return(FALSE);

  if(AesInfo->ap_version <AES_SUBMENU)		/* Aes doesn't support its !! */
    return(FALSE);

  Indice = 0L; 
  while(!(Liste[Indice].SubMenu     == 0 &&
  	  Liste[Indice].ParentEntry == 0 &&
  	  Liste[Indice].StartEntry  == 0 &&
  	  Liste[Indice].ScrollEntry == 0 &&
  	  Liste[Indice].UseChecked  == 0))
  {
    /*
     *	Making attach beetwen a parent and sub-menu 
     */
    if(!rsrc_gaddr(R_TREE,Liste[Indice].SubMenu,&SousArbre))
      return(FALSE);  
    SubMenu.mn_tree = SousArbre;			/* Adresse du Sous-Menu */
    SubMenu.mn_menu = ROOT;				/* A partir de la racine */
    SubMenu.mn_item = Liste[Indice].StartEntry;		/* Entr‚e initiale active */
    SubMenu.mn_scroll = Liste[Indice].ScrollEntry;  	/* Entr‚e de scroll */
  
  							/* Attachement au Menu racine */
    if(!menu_attach(TRUE,Arbre,Liste[Indice].ParentEntry,&SubMenu))
      return(FALSE);
      							/* Indiquer l'entr‚e active */
    if(!menu_istart(TRUE,SousArbre,ROOT,Liste[Indice].StartEntry))
      return(FALSE);
      
    /*
     *	Store in File_SubMenu_Checked the sub-menu working with
     *	the check mark automatically
     */
    if(Liste[Indice].UseChecked)			/* Si utilise un auto CheckMark */
    {
      Poste.Tree = SousArbre;				/* Adresse du SousMenu */
      SousArbre[Liste[Indice].StartEntry].ob_state |=CHECKED;	/* Placer le CheckMark */
      if(!Emfiler(File_SubMenu_Checked,&Poste))		/* Ins‚rer dans la liste */
        return(FALSE);
    }
    Indice++;
  }/*EndWhile*/  
  return(TRUE);
}

int M_SubEvent(int RootTree,Type_Parameter_SubEvent *Liste)
/*
	Cr‚er la liste des ‚vŠnements. On associe un traitement … chaque entr‚e de tous
	les SousMenus.
	
	Entr‚es:	Entier indentifiant le Menu racine
			Liste des ‚vŠnements
	Sorties:	True si r‚ussi
*/
{
  OBJECT	*Arbre,
  		*SousArbre;
  Type_Entry	Poste;
  unsigned long	Indice;

  if(!rsrc_gaddr(R_TREE,RootTree,&Arbre))	/* Root Menu Address */
    return(FALSE);

  if(AesInfo->ap_version <AES_SUBMENU)		/* Aes doesn't support its */
    return(FALSE);

  Poste.RootTree = Arbre;			/* Store root address */
  Poste.RootName = RootTree;			/* and its name */
  /*
   *	Insert into File_Event the triple object to call process 
   */
  Indice = 0L;
  while(!(Liste[Indice].SubMenu   == 0 &&
  	  Liste[Indice].Entry     == 0 &&
  	  Liste[Indice].Procedure == NULL))
  {
    if(!rsrc_gaddr(R_TREE,Liste[Indice].SubMenu,&SousArbre))
      return(FALSE);
    Poste.Tree  = SousArbre;			/* Copy Tree Address */  
    Poste.Name  = Liste[Indice].SubMenu;	/* Copy Its name */
    Poste.Entry = Liste[Indice].Entry;		/* Copy indicie entry */
    Poste.Procedure = Liste[Indice].Procedure;	/* Procedure associated */
    Poste.Key = CalculKey(SousArbre[Poste.Entry].ob_spec.free_string);	/* Quick Access key */
    if(!Emfiler(File_Event,&Poste))
      return(FALSE);

    Indice ++;
  }/*EndWhile*/
  return(TRUE);
}

/*
 * -----------------------------------------------------------------------------------------
 *				Publics Aads users procedure:
 *				Event Manager
 */
 
void M_Event(void)
/*
	Gestion des messages s‚lection de menu et ‚vŠnements clavier.
	
	Entr‚es:	
	Sorties:
*/
{
	Type_Entry		*Poste, *Initial_Poste;
	OBJECT			**Pointeur;
	int				Touche;
    Type_Window		*window;	
    int				Handle, Dummy;


	wind_get(DESKTOP, WF_TOP, &Handle, &Dummy, &Dummy, &Dummy);														/* Fenetre actuellement TOP */
	window = WindSearch(Handle);
	if(window != NULL)
	{
		if(window->type & MODAL)																/* Si la fenetre courante est MODALE */
		{	
			if( ((A_Event & MU_MESAG) && (A_Message[0] == MN_SELECTED)) || A_Event & MU_KEYBD)
			{
				if((A_Event & MU_MESAG) && (A_Message[0] == MN_SELECTED))						/* Disable Menu selected */
				{
					Poste = FileSommet(File_Event);												/* Lire premier poste */
					if(MchInfo.AesVersion >= AES_SUBMENU)										/* Si Popup g‚r‚s */
      					Pointeur = (OBJECT **)&A_Message[5];									/* Ptr sur Menu en cours */
    				else																		/* Forc‚ment le Menu principal */
      					Pointeur = &Poste->RootTree;											/* Adr du Main Menu */  

					menu_tnormal(*Pointeur, A_Message[3], TRUE);									/* Disable menu selected */
				}
				A_Event		&=	(~MU_MESAG | ~MU_KEYBD);				
				return;																			/* Et quitter */
			}
		}
	}	
    
  if((A_Event & MU_MESAG) && (A_Message[0]==MN_SELECTED))	/* Message Event */
  {

    Poste = FileSommet(File_Event);				/* Lire premier poste */
    
    /*
     *	Sur les AES >= 3.4 Le GEM fournit un Ptr sur l'Arbre Menu s‚lectionn‚ (Main ou 
     *	sous-menu), mais pour les AES pr‚c‚dents, on utilisera l'adresse du menu principal
     *	puisqu'il n'y a forc‚ment que lui !
     */
    if(MchInfo.AesVersion >= AES_SUBMENU)			/* Si Popup g‚r‚s */
      Pointeur = (OBJECT **)&A_Message[5];			/* Ptr sur Menu en cours */
    else											/* Forc‚ment le Menu principal */
      Pointeur = &Poste->RootTree;					/* Adr du Main Menu */  

    if(Poste == NULL)
      form_alert(1,ERROR_CODE);
    if(Poste->Tree != *Pointeur || Poste->Entry != A_Message[4])
    {
      Initial_Poste = Poste;
      FileTourne(File_Event);
      Poste = FileSommet(File_Event);
      while(!((Poste->Tree == *Pointeur && Poste->Entry == A_Message[4]) || (Initial_Poste == Poste)))
      {
        FileTourne(File_Event);
        Poste = FileSommet(File_Event);
      }
    }
    /*
     *	Menu et entr‚e trouv‚s
     */
    if(Poste->Tree == *Pointeur && Poste->Entry == A_Message[4])
    {
      M_Address	= Poste->Tree;
      M_Entry 	= Poste->Entry;
      M_Name	= Poste->Name;
      if(Poste->Procedure != NULL)
        (*Poste->Procedure)();
      if(WorkWithCheck(Poste->Tree))				/* Only for a sub-menu */
        MoveCheck(Poste->Tree,Poste->Entry);
      else
      {
        if((MchInfo.AesVersion >= AES_SUBMENU))			/* Si Popup g‚r‚s */
          menu_istart(TRUE,Poste->Tree,ROOT,Poste->Entry);
      }
    }   
    else
     form_alert(1,ERROR_CODE);
    menu_tnormal(Poste->RootTree,A_Message[3],TRUE);
  }
  else if(A_Event & MU_KEYBD)				/* keyboard event */
  {
    if((A_Key & 0x00FF) == 0)				/* ALT_xx */
      Touche = A_Key & 0xFF00;
    else						/* CTRL_xx */
      Touche = A_Key & 0x00FF;
    
    if(!EstFileVide(File_Event))
      Poste = FileSommet(File_Event);
    else
      return;
      
    if(Poste->Key != Touche)				/* Not this Key */
    {
      Initial_Poste = Poste;				/* First poste visited */
      FileTourne(File_Event);
      Poste = FileSommet(File_Event);			/* Read next */
      while(!(Poste->Key == Touche || Poste == Initial_Poste))
      {						
        FileTourne(File_Event);
        Poste = FileSommet(File_Event);
      }
    }
    /*
     *	Good key found and not disabled entry
     */
    if(Poste->Key == Touche && !(Poste->Tree[Poste->Entry].ob_state & DISABLED))
    {
      M_Address 	= Poste->Tree;
      M_Entry		= Poste->Entry;
      M_Name		= Poste->Name;
      if(Poste->Procedure != NULL)
        (*Poste->Procedure)();
      if(WorkWithCheck(Poste->Tree))				/* Only for a sub-menu */
        MoveCheck(Poste->Tree,Poste->Entry);
      else
      {
        if((MchInfo.AesVersion >= AES_SUBMENU))			/* Si Popup g‚r‚s */
          menu_istart(TRUE,Poste->Tree,ROOT,Poste->Entry);
      }
      A_Event = MU_NULL;
    }
  }
  else								/* Another event, not for us ! */
    ;
}
