/*
 *		Module de gestion des Popups menus dans les formulaires.
 *		Ce module est un sous-ensemble de PCXFORM.C
 *
 *
 *	Author  : FunShip
 *	File	: PCXPopup.c
 *	Date    : 06 October 1995
 *	Revision: 7 December 1998
 *	Version : 1.20
 *	Release : 2
 *	Language: Pure C, Version du 3 Fevrier 1992 
 *	Origine : FRANCE
 *	
 *			Copyright (c) FunShip, ATARI 1991-1995
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "..\Include\PCXApp.h"
#include "..\Include\PCFile.h"
#include "..\Include\PCXForm.h"

#include "Tools.h"

/*
 * -----------------------------------------------------------------------------------------
 *				Quelques d‚finitions
 */
 
#define	TRUE		1
#define	FALSE		0

#define	AES_POPUP	0x340			/* This Aes and above support Popups */

#define	DEPTH			999
#define	TIME_ITEM	50				/* 50 Ms clignotement */
#define	NB_CLIGNOTE	7				/* 3 Enabled + 4 Disabled */

/*
 * -----------------------------------------------------------------------------------------
 *			Parametres de fonctionnement des Popups 
 */
 
#define	MN_DISPLAY	50
#define	MN_DRAG		200
#define	MN_DELAY	10
#define	MN_SPEED	100


#define	ERROR_POPUP_VIDE	"[0][ PCXPopup Warning: | No Popup found at all][ok]"
#define	ERROR_POPUP			"[0][ PCXPopup Warning: | No Popup attached ][ok]"
#define	ERROR_EVENT			"[0][ PCXPopup Warning: | No Popup Event Set ][ok]"
#define	ERROR_CODE			"[0][ PCXPopup Warning: | No Popup Code Event Set ][ok]"

/*
 * -----------------------------------------------------------------------------------------
 *				Popups menus list.
 */

typedef	struct	{
					int		Formular;		/* Name of formular */
					OBJECT	*FormularAddress;	/* Its address */
					int		Button;			/* Button where attached */
					int		Popup;			/* Name of Popup */
					MENU		PopMenu;		/* Structure of Popup */
				} Type_Popup;
		
/*
 * -----------------------------------------------------------------------------------------
 *				Popups menus events list.
 */
 
typedef	struct	{
					int		Popup;			/* A Popup menu */
					int		Entry;			/* A Popup Entry */
					void (*Procedure)(void);		/* Procedure to call */
				} Type_PopEvent;

/*
 *	Popups menus list are working with an automatic checkmark to point the entry
 *	selected.
 */
		
typedef struct	{
					OBJECT	*Tree;			/* Sub Menu */
				} Type_PopChecked;

/*
 * -----------------------------------------------------------------------------------------
 *			Privates Datas Structures 
 */
static	Type_File	*File_Popup;			/* Popups list */
static	Type_File	*File_PopEvent;			/* Popups event list */
static	Type_File	*File_PopChecked;		/* Popups working with check mark */

/*
 * -----------------------------------------------------------------------------------------
 *				Miscallenous privates procedures
 */
 
static int WorkWithCheck(OBJECT *Tree)
/*
	Return True if the Popup-Menu as Tree address work with an automatic
	checkmark.
	
	Input:	Tree is a pointer to tree object Popup-Menu
	OutPut:	Return True or False.
*/
{
  Type_PopChecked	*Poste;
  OBJECT		*Initial_adresse;
  
  if(!EstFileVide(File_PopChecked))			/* Si la file n'est pas vide */
  {
    Poste = FileSommet(File_PopChecked);		/* Lire le premier poste */
    if(Poste->Tree == Tree)				/* Si est celui cherch‚  */
      return(TRUE);					/* Ok */
    else
    {
      Initial_adresse = Poste->Tree;			/* Sinon parcourir tout la file */
      FileTourne(File_PopChecked);
      Poste = FileSommet(File_PopChecked);
      while(!(Poste->Tree == Tree || Initial_adresse == Poste->Tree))
      {
        FileTourne(File_PopChecked);
        Poste = FileSommet(File_PopChecked);
      }
      /*
       *	Tree'address found or not !
       */
      return(Poste->Tree == Tree);			/* True si est celui cherch‚ */
    }
  }
  else
    return(FALSE);
}

static Type_PopEvent *S_PopEvent(int PopName,int Entry)
/*
	Recherche un event <Popup><Entr‚e de Popup> dans le liste PopEvent.
	
	Entr‚es:	PopName = Entier indice du Popup Menu
			Entry	= Entr‚e du Popup
	Sorties:	Retourne un event PopEvent ou NULL
*/
{
  Type_PopEvent		*PopEvent,*Init_PopEvent;
  
  if(EstFileVide(File_PopEvent))
    return(NULL);
  else
  {
    PopEvent = FileSommet(File_PopEvent);
    if(PopEvent->Popup != PopName || PopEvent->Entry != Entry)
    {
      Init_PopEvent = PopEvent;
      FileTourne(File_PopEvent);
      PopEvent = FileSommet(File_PopEvent);
      while(!( (PopEvent->Popup == PopName && PopEvent->Entry == Entry) ||
               (PopEvent == Init_PopEvent) ))
      {
        FileTourne(File_PopEvent);
        PopEvent = FileSommet(File_PopEvent);  
      }
    }
    if(PopEvent->Popup == PopName && PopEvent->Entry == Entry)
      return(PopEvent);
    else
      return(NULL);
  }
}

static void PrepareXY( int *X, int *Y, int Width, int Height )
/*
	Cette proc‚dure d‚termine en fonction de l'‚cran le positionnement
	du popup pour que celui-ci soit toujour … l'int‚rieur de l'‚cran.
*/
{
	/*	Le popup est trop a gauche	*/
	if( *X == 0 )
		*X	=	*X + 2;

	/*	Le popup est trop a droite	*/
	if( *X + Width + 10 >= VdiInfo.LargeurStation )
		*X	=	*X - Width - 10;
	
	/*	Le popup est trop en bas	*/
	if( *Y + Height + 10 >= VdiInfo.HauteurStation)
		*Y	=	*Y - Height - 10;	
}

static int _PopupUser(int IDObjectPopup, int RegionX, int RegionY, MENU *Resultat)
/*
	Cette procedure gere un menu popup pour les AES ne les supportant pas.
	En fait c'est une emulation de menu_popup() de l'AES.
*/
{
	int						MouseX, MouseY, MouseButton, Dummy, Etat;
	int						RegionW, RegionH;
	int						Object, OldEntry;
	Type_Buffer_Screen		*RegionBoite;
	OBJECT					*Arbre;


	rsrc_gaddr(R_TREE, IDObjectPopup, &Arbre);	

	RegionW			=	Arbre[0].ob_width; 
	RegionH			=	Arbre[0].ob_height;

	PrepareXY( &RegionX, &RegionY, RegionW, RegionH );

	Arbre[0].ob_x	=	RegionX; 
	Arbre[0].ob_y	=	RegionY;

	
	RegionBoite = ScreenSave(VdiHandle, RegionX-1, RegionY-1, RegionW + 6, RegionH + 6);
	objc_draw(Arbre, 0, DEPTH, RegionX, RegionY, RegionW + 5, RegionH + 5);			/* Draw menu box */


	graf_mkstate(&MouseX,&MouseY,&MouseButton,&Dummy);
	OldEntry =	0; /*IDObjectPopup;*/

	while( (MouseButton & 0x03) )						/* Tant que non boutons relaches */
		graf_mkstate( &MouseX, &MouseY, &MouseButton, &Dummy );
			

	while( (MouseButton & 0x03) == 0 )										/* Tant que boutons relache */
	{	
		Object = objc_find(Arbre, 0, DEPTH, MouseX, MouseY);		
		Object = (MouseX >= RegionX && MouseX <= RegionX + RegionW - 1 && MouseY >= RegionY && MouseY <= RegionY + RegionH - 1) ? Object : -1;

		/*	Si la souris est sur une entree du popup */
		if( Object > 0 &&
			( (Arbre[Object].ob_type == G_STRING) || (Arbre[Object].ob_type == G_BOXTEXT) ) 
			&& !(Arbre[Object].ob_state & DISABLED) )
		{
			if(Object != OldEntry)
			{
				Etat = Arbre[Object].ob_state | SELECTED;					/* New Entry selected */
				objc_change(Arbre, Object, 0, RegionX, RegionY, RegionW, RegionH, Etat, TRUE);

				Etat = Arbre[OldEntry].ob_state & ~SELECTED;				/* Old Entry cleared */
				objc_change(Arbre, OldEntry, 0, RegionX, RegionY, RegionW, RegionH, Etat, TRUE);
				OldEntry = Object;
			}
		}
		else 
		{
			if(OldEntry > 0)
			{
				if((Arbre[OldEntry].ob_state & SELECTED))				/* Souris sortie du Menu d‚roulant */
				{
					Etat = Arbre[OldEntry].ob_state & ~SELECTED;				/* Old Entry cleared */
					objc_change(Arbre, OldEntry, 0,RegionX, RegionY, RegionW, RegionH, Etat, TRUE);				
					OldEntry = 0; /*IDObjectPopup;*/
				}
			}			
		}
		
		graf_mkstate(&MouseX, &MouseY, &MouseButton, &Dummy);					/* Next mouse location */
	}	
	/* EndWhile */

	/*
	 *	Fait clignoter l'item du menu s‚lectionn‚ si il existe (!), juste pour faire jolie !!
	 */
	if(OldEntry != 0) /*!= IDObjectPopup)*/
	{

		for( Dummy=1; Dummy <= NB_CLIGNOTE; Dummy++)
		{
			Etat = (Arbre[OldEntry].ob_state & SELECTED) ? Arbre[OldEntry].ob_state & ~SELECTED : Arbre[OldEntry].ob_state | SELECTED;	/* Old Entry cleared */
			objc_change(Arbre, OldEntry, 0, RegionX, RegionY, RegionW, RegionH, Etat, TRUE);
			evnt_timer(TIME_ITEM,0);
		}
	}

	ScreenRestore(RegionBoite);							/* Restore graphic area */

	/*	Preparer les donn‚es de retours */
	Resultat->mn_item	=	OldEntry;
	Resultat->mn_tree	=	Arbre;

	while( (MouseButton & 0x01) == 1 )										/* Tant que boutons relache */
	{
		graf_mkstate(&MouseX, &MouseY, &MouseButton, &Dummy);					/* Next mouse location */
	}
	
	return	(OldEntry > 0 && OldEntry != IDObjectPopup);
}

static void	CopyText( OBJECT *Button, OBJECT *Item )
/*
	Copie le texte contenu dans l'objet Item du menu popup dans le bouton.
	Tient compte du type d'objet dans le popup, soit G_String ou G_xxText.
*/
{
	TEDINFO		*TedInfo;
	
	/*	Si l(objet d‚clancheur du popup n'est pas un bouton; on ne sait pas faire */
	if( Button->ob_type != G_BUTTON )
		return;
	
	/*	Si l'objet du menu popup est un G_xTEXT, alors le texte est stock‚ dans
	une structure TEDINFO */
	if( Item->ob_type == G_TEXT || Item->ob_type == G_BOXTEXT ||
		Item->ob_type == G_FTEXT || Item->ob_type == G_FBOXTEXT )
	{
	    TedInfo=(TEDINFO *)Item->ob_spec.tedinfo;
	    strcpy(	Button->ob_spec.free_string, TedInfo->te_ptext);
	}
	/*	Si l'objet du menu popup est un G_STRING on a acces directement au texte */
	else if( Item->ob_type == G_STRING )
	{
   		strcpy(	Button->ob_spec.free_string, Item->ob_spec.free_string);
	}
  	else
  		;
}					

/*
 * -----------------------------------------------------------------------------------------
 *			Miscallenous publics procedures for PcXForm.c ONLY.
 */

int _InitPopup(void)
/*
	Initialise les structures de donn‚es pour la gestion des Popups en formulaire.
	
*/
{
	File_Popup		= CreerFile(sizeof(Type_Popup));						/* Liste des Popups */
	File_PopEvent	= CreerFile(sizeof(Type_PopEvent));					/* Liste des Events Popups */
	File_PopChecked= CreerFile(sizeof(Type_PopChecked));				/* Liste des Popups auto CheckMark */
	
	return(File_Popup != NULL && File_PopEvent != NULL && File_PopChecked != NULL);
}

void _ExitPopup(void)
/*
	D‚truit les structure de donn‚es pour la gestion des Popus menus
	attach‚s aux boutons.
*/
{
	while(Defiler(File_Popup));
	while(Defiler(File_PopEvent));
	while(Defiler(File_PopChecked));
	
	KillFile(File_Popup);
	KillFile(File_PopEvent);
	KillFile(File_PopChecked);
}

void F_Popup(int IdFormulaire,int IdObject, int Mouse_X, int Mouse_Y)
/*
	Proc‚dure de gestion du fonctionnement d'un Menu Popup.
	Si attach‚ … un object de type bouton, on place le menu align‚ a celui-ci,
	sinon on ouvre le menu en (m_x, m_y).
	
	Input:	IdFormulaire	L'entier identifiant le formulaire
			IdObject		L'entier identifiant l'object d‚clencheur du Popup-Menu
			
	OutPut:	None
*/
{
	Type_PopEvent 	*PopEvent;
	Type_Popup		*Popup,*Initial_Popup;
	MENU			Resultat;
	OBJECT			*SousArbre;
	int				new, L_TypeObject;
	Type_CoordXY	CoordXY;
  
	Popup = FileSommet(File_Popup);							/* Rechercher le premier Popup */
	if(Popup != NULL)										/* Il existe des Popups */
	{
		/*	Rechercher le Popup associ‚ au bouton */
		if(!(Popup->Formular == IdFormulaire && Popup->Button == IdObject))
		{
			Initial_Popup = Popup;
			FileTourne(File_Popup);
			Popup = FileSommet(File_Popup);
			while(!( (Popup->Formular == IdFormulaire && Popup->Button == IdObject) || Popup == Initial_Popup))
			{
				FileTourne(File_Popup);
				Popup = FileSommet(File_Popup);
			}
		}
	
		/*	Sortie de boucle: Soit on a trouv‚ le Popup ou pas	*/
		
		if(Popup->Formular == IdFormulaire && Popup->Button == IdObject)
		{
			/*	Quel est le type de l'objet d‚clencheur du popup */
			rsrc_gaddr(R_TREE, IdFormulaire, &SousArbre);
			L_TypeObject	=	SousArbre[IdObject].ob_type;
			
			/*	Si est de type bouton, on aligne le popup sur le bouton */
			if( L_TypeObject == G_BUTTON )
			{
				SumXY(&CoordXY,Popup->FormularAddress, IdObject);
			}
			else
			{
				CoordXY.X	=	Mouse_X;
				CoordXY.Y	=	Mouse_Y;
			}

			/*	On utilise la routine Atari si c'est un BOUTON uniquement */	    		
			if(AesInfo->ap_version >= AES_POPUP && L_TypeObject == G_BUTTON)			
				new	=	menu_popup(&Popup->PopMenu, CoordXY.X, CoordXY.Y, &Resultat);
			else
				new	=	_PopupUser(Popup->Popup, CoordXY.X, CoordXY.Y, &Resultat);

			/*	Resultat aprŠs fermeture par l'utilisateur du Popup	*/
			if( new )																	/*	Si une entree a ete selectionne */
			{			
				/*	Move the check-mark and set new effective entry	*/
				
				if(WorkWithCheck(Resultat.mn_tree))								/* D‚pla‡er le CheckMark */
				{
					Resultat.mn_tree[Popup->PopMenu.mn_item].ob_state &=~CHECKED;
					Resultat.mn_tree[Resultat.mn_item].ob_state |=CHECKED;
				}
				Popup->PopMenu.mn_item = Resultat.mn_item;					/* Copier New entr‚e active */

				/*	Rechercher l'‚v‚nement associ‚ pour cette entr‚e	*/
				PopEvent    = S_PopEvent(Popup->Popup, Resultat.mn_item);
				if(PopEvent != NULL)													/* Si trouv‚ */
				{
					F_PopupName = Popup->Popup;									/* Name of Popup Menu Enable */
					F_PopupEntry= Resultat.mn_item;								/* Name of Popup Entry selected */
					if(PopEvent->Procedure != NULL)								/* Code User d‚finie */
					{
						(*PopEvent->Procedure)();									/* Call user procedure */
					}												
					else																		/* Pas de code User */
						form_alert(1,ERROR_CODE);
					
					/* Si object type bouton, copier le texte dedans */
					if( L_TypeObject == G_BUTTON )
					{			
						CopyText(	&(Popup->FormularAddress[Popup->Button]),
									&(Resultat.mn_tree[Resultat.mn_item]));
					}
				}
				else																			/* Pas d'event pour cette entr‚e */
					form_alert(1,ERROR_EVENT);
			}
				        
			if( L_TypeObject == G_BUTTON )
			{
				Popup->FormularAddress[IdObject].ob_state &= ~SELECTED;			/* D‚sactiver le bouton d'accŠs */
				F_RedrawObject(Popup->FormularAddress,IdObject);					/* redessiner de suite */
			}
		}
		else																					/* Pas de Popup pour ce bouton */
			form_alert(1,ERROR_POPUP);													/* Donc pas d'‚v‚nement */
	}
	else																					/* Aucun Popup dans la liste */
		form_alert(1,ERROR_POPUP_VIDE);
}

void F_PopupXY( TPopup *Popup, int Mouse_X, int Mouse_Y)
/*
	Proc‚dure de gestion d'un menu popup libre en Position X, Y.
	On ouvre le popup, le fait fonctionner et appele la routine CALL BACK utilisateur
	pour l'entr‚e s‚lectionn‚e si une l'a ‚t‚.
	
	Input:	Position (X,Y) demand‚e
	OutPut:	None
*/
{
	MENU			Resultat;
	int				new;
	Type_CoordXY	CoordXY;
  
	CoordXY.X	=	Mouse_X;
	CoordXY.Y	=	Mouse_Y;
		
	new	=	_PopupUser(Popup->Tree, CoordXY.X, CoordXY.Y, &Resultat);

	/*	Resultat aprŠs fermeture par l'utilisateur du Popup	*/
	if( new )														/*	Si une entree a ete selectionne */
	{			
		if(Popup->routine != NULL)								/* Code User d‚finie */
			(*Popup->routine)(Resultat.mn_item);					/* Call user procedure */
		else														/* Pas de code User */
			form_alert( 1, ERROR_EVENT );
	}
}

/*
 * -----------------------------------------------------------------------------------------
 * 				Publics Procedures to Aads's users	
 */
	
void F_SetPopup(int Size)
/*
	Indique le nombre d'entr‚es de Popup-Menu … afficher. Par defaut le GEM
	l'initialise … 16.
	
	Entr‚es:	Size = nombre d'entr‚es
	Sorties:	rien
*/
{
	MN_SET	settings;

	if(AesInfo->ap_version <AES_POPUP)			
		return;
	
	settings.Display	= MN_DISPLAY;
	settings.Drag		= MN_DRAG;
	settings.Delay	= MN_DELAY;
	settings.Speed	= MN_SPEED;
	settings.Height	= Size;
	menu_settings(TRUE,&settings);
}

int F_PopAttach(int Formular,Type_Parameter_PopAttach *Liste)
/*
	R‚alise l'attachement de un ou plusieurs Menus-Popups … des 
	boutons de 1 ou plusieurs formulaires.
	
	Entr‚es:
	Sorties:
*/
{
	OBJECT					*Arbre, *SousArbre;
	unsigned long			Indice;
	Type_PopChecked			PopChecked;											/* A Popup working with Check Mark */
	Type_Popup				Popup;												/* A Popup object */
  
	if(!rsrc_gaddr(R_TREE,Formular,&Arbre))									/* Where popup attached */
    	return(FALSE);

	Indice = 0L; 
	while(!(Liste[Indice].Popup  == 0 &&										/* While not end of list */
			Liste[Indice].Button	    == 0 &&
			Liste[Indice].StartEntry  == 0 &&
			Liste[Indice].ScrollEntry == 0 &&
			Liste[Indice].UseChecked  == 0))
	{
		/*
		 *	Making attach beetwen a button formular and pop-menu 
		 */
		if(!rsrc_gaddr(R_TREE,Liste[Indice].Popup,&SousArbre))
			return(FALSE);  

    Popup.Formular	  = Formular;											/* Name of formular */
    Popup.FormularAddress = Arbre;											/* Address of formular */
    Popup.Button	  = Liste[Indice].Button;								/* Button where attached */
    Popup.Popup		  = Liste[Indice].Popup;							/* Popup Name */    
    Popup.PopMenu.mn_tree = SousArbre;										/* Popup menu address */
    Popup.PopMenu.mn_menu = ROOT;											/* Parent object */
    Popup.PopMenu.mn_item = Liste[Indice].StartEntry;					/* Entry selected */
    Popup.PopMenu.mn_scroll = Liste[Indice].ScrollEntry;				/* Entry to start scroll */
  
    if(!Emfiler(File_Popup,&Popup))
      return(FALSE);
    /*
     *	Copy current Popup Entry to formular's Button if is a type button !
     */
	if(Popup.FormularAddress[Popup.Button].ob_type == G_BUTTON)
	{
		CopyText(	&Popup.FormularAddress[Popup.Button], 
					&SousArbre[Popup.PopMenu.mn_item]);
			
   	}
    /*
     *	Store in File_PopChecked the sub-menu working with
     *	the check mark automatically
     */
    if(Liste[Indice].UseChecked)												/* Use an auto-check mark */
    {
      PopChecked.Tree = SousArbre;
      SousArbre[Liste[Indice].StartEntry].ob_state |=CHECKED;
      if(!Emfiler(File_PopChecked,&PopChecked))
        return(FALSE);
	}
    Indice++;
  }/*EndWhile*/  
  return(TRUE);
}

int F_PopEvent(Type_Parameter_PopEvent *Liste)
/*
	R‚alise la cr‚ation de la liste des ‚vŠnements des popups menus.
	
	Entr‚es:
	Sorties:
*/
{
	Type_PopEvent	PopEvent;
	unsigned long	Indice;

	 /*
	  *	Insert into File_PopEvent the object to call process 
	  */
	Indice = 0L;
	while(!(Liste[Indice].PopMenu == 0 && Liste[Indice].Entry == 0 && Liste[Indice].Procedure == NULL))
	{
		PopEvent.Popup     = Liste[Indice].PopMenu;		/* Name of popup */
		PopEvent.Entry     = Liste[Indice].Entry;		/* Entry selected */
		PopEvent.Procedure = Liste[Indice].Procedure;	/* Procedure associated */
    	if(!Emfiler(File_PopEvent,&PopEvent))
      		return(FALSE);
    	Indice ++;
	}

	return(TRUE);
}


