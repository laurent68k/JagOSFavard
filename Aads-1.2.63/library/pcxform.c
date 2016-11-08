/*
 *		Module de gestion des formulaires en fenetres ou normal.
 *
 *
 *	Author  : 	FunShip
 *	File	: 	PCXForm.c
 *	Date    : 	01 October 1991
 *	Revision: 	21 juillet 1997
 *	Version : 	1.20
 *	Release : 	2	
 *	Language: 	Pure C, Version du 3 Fevrier 1992 
 *	Origine : 	FRANCE
 *	
 *		  - Copyright Atari FunShip (c) 1991 - 1997
 *			   - ATARI ST/STE/TT & Falcon -
 * -----------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "..\Include\PCXForm.h"
#include "..\Include\PCXApp.h"
#include "..\Include\PCXWind.h"
#include "..\Include\PCFile.h"
#include "..\Include\PCPile.h"

#include "window.h"
#include "Tools.h"

#define	TRUE		1
#define	FALSE		0

#define	DROIT		2							/* Right Mouse button */
#define	GAUCHE		1							/* mouse left button */ 
#define	MU_NULL		0							/* No message */


#define	UP			0x4800		
#define	DOWN		0x5000
#define	LEFT		0x4B00
#define	RIGHT		0x4D00
#define	SUP			0x4838						/* Shift_xx */
#define	SDOWN		0x5032
#define	SLEFT		0x4B34
#define	SRIGHT		0x4D36

#define	ONELEVEL	0						/* Obj_draw One level drawing   */
#define	BORDER		10					/* Border for "Classic" formular shadowed */

#define	X_START		0					/* Origine of growing box */
#define	Y_START		0
#define	W_START		50
#define	H_START		50
#define	X_END		0					/* Destination of shrinking box */
#define	Y_END		0
#define	W_END		50
#define	H_END		50

#define	ATTRIBUTS	NAME|MOVER				/* window's form attributes */

#define	ERROR_FORM	"[0][ PCXForm warning: | Formular not found ][ok]"
#define	ERROR_EVENT	"[0][ PCXForm warning: | No Event defined ][ok]"

/*
 * ------------------------------------------------------------------------
 *				From module PCXPopup.c
 *	This procedures aren't really public. Just for Module PCXForm.c
 *	and will never given to the Aads user.
 */
 
extern	int  _InitPopup(void);					/* Initialise gestion Popups */
extern	void _ExitPopup(void);					/* Terminaison gestion Popups */
extern	void F_Popup(int IdFormulaire,int IdBouton, int Mouse_X, int Mouse_Y);	/* Gestion du Popup menu */

/*
 * ------------------------------------------------------------------------
 *				From module PCXDesk.c
 *	This procedures aren't really public. Just for Module PCXForm.c
 *	and will never given to the Aads user.
 */

extern	int  _InitDesk(void);					/* Initialise gestion Desktop */
extern	void _ExitDesk(void);					/* Terminaison gestion Desktop */
extern	void _DeskEvent(void);					/* Gestion du bureau */

/*
 * --------------------------- Publics datas ------------------------------
 */

int	F_NameObject;						/* Object selected */
int	F_Name;							/* Formular Name */
OBJECT	*F_Address;						/* Formular Address  */

OBJECT	*DesktopAddress;					/* Desktop Address */
int	I_Name;							/* Icone Name */

int	F_PopupName;						/* Name of Popup Menu Enable */
int	F_PopupEntry;						/* Name of Popup Entry selected */

/*
 * -----------------------------------------------------------------------------------------
 *				Privates Datas Types
 */

typedef	struct	{
					int		Object;									/* order number object */
					void (*ProcClic)(void);							/* when clic event */
					void (*ProcDbClic)(void);						/* when double clics event */
					void (*ProcClicRight)(void);					/* Procedure when right clic */
					void (*ProcDbClicRight)(void);					/* Procedure when right double clic */
					void (*ProcMouseEnter)(void);					/* Procedure when mouse enter to the object */
					void (*ProcMouseExit)(void);					/* Procedure when mouse exit to the object  */				
				} Type_Event;

typedef struct	{
					int		Exit;									/* For Exit formular in "Classic" loop */
					int		TreeName;								/* Formular's Name */
					OBJECT	*TreeAddress;							/* Formuler's Address */
					int		Suspend;								/* TRUE when iconify */
					int		Handle;									/* Window's handle */
					int		FirstEdit;								/* Current field edit name */
					int		CursorOn;								/* Cursor On/Off */
					int		Position;								/* Cursor position */
					int		CloseButton;							/* Botuon of Close code clic */
					int		ObjectHaveMouse;						/* Current object under mouse pointer */
					void (*CloseClic)(void);						/* Close Code clic */
					Type_File	*FileEvent;							/* Its Event list */
					Type_UserEvent UserEvent;						/* Suppementary procedures events */
				} Type_Form;	

/*
 * -----------------------------------------------------------------------------------------
 *				Privates Datas Structures
 */
		
static	Type_File	*File_Formular;				/* Formular list */

/*
 * -----------------------------------------------------------------------------------------
 *				Miscallenous privates procedures
 */

static int _Prochain(Type_Form *Form,int Object)
/*
	Parcour un niveau d'objet pour trouver le prochain objet Editable dans le
	formulaire Form. Si aucun trouv‚ on retourne l'objet en paramŠtre.
	
	Object est l'objet auquel il faut trouv‚ le prochain.
*/
{
  int	Pere,Prochain;
  
  Prochain	= Form->TreeAddress[Object].ob_next;
  Pere		= GetPere(Form->TreeAddress,Object);
  while(!((Form->TreeAddress[Prochain].ob_flags & EDITABLE) || (Prochain == Object)))
  {
    if(Prochain != Pere)
      Prochain = Form->TreeAddress[Prochain].ob_next;
    else
      Prochain = Form->TreeAddress[Prochain].ob_head;
  }
  return(Prochain);						/* Retourner */
}

static int NextEdit(Type_Form *Form,int Object)
/*
	Calcul le prochain champ Editable de l'objet en paramŠtre.

	Il DOIT EXISTER au moins un objet Editable dans le niveau d'arborescence
	de l'objet en paramŠtre ou alors qu'il soit lui meme Editable.
*/
{	
  int	Prochain,Nouveau;
  
  Prochain = Form->TreeAddress[Object].ob_next;			/* Lire Prochain frŠre */
  if(Form->TreeAddress[Prochain].ob_tail == Object)		/* Prochain est son PŠre */
    Prochain = Form->TreeAddress[Prochain].ob_head;		/* Lire premier enfant */
    
  if(Form->TreeAddress[Prochain].ob_flags & EDITABLE)		/* Si enfant Editable */
    return(Prochain);						/* Le retourner */
  else								/* Chercher le prochain Editable */
  {
    Nouveau = _Prochain(Form,Prochain);				/* Calculer prochain Editable */
    if(Nouveau != Prochain)					/* Si un nouveau trouv‚ */
      return(Nouveau);						/* Le retourner */
    else
      return(Object);						/* Sinon il n'existe que lui ! */
  }
}

static int PrevEdit(Type_Form *Form,int Object)
/*
	Calcul quel est le Champ Editable pr‚c‚dent celui pass‚ en paramŠtre.

	Il DOIT EXISTER au moins un objet Editable dans le niveau d'arborescence
	de l'objet en paramŠtre ou alors qu'il soit lui meme Editable.
*/
{
  int		Prochain;
  Type_Pile	*Pile;
  
  Pile = CreerPile(sizeof(int));				/* Cr‚er une pile */

  if(!(Form->TreeAddress[Object].ob_flags & EDITABLE))		/* Si il est non EDITABLE */
    Object = _Prochain(Form,Object);				/* Take the next one */  
  Empiler(Pile,&Object);					/* Empiler notre objet ou le suivant */

  Prochain = _Prochain(Form,Object);				/* Take next object */
  while(Prochain != Object)					/* Until our object is found */
  {
    Empiler(Pile,&Prochain);					/* Store the new editable founded */
    Prochain = _Prochain(Form,Prochain);  			/* Search the next editable */
  }
  Prochain = *((int *)PileSommet(Pile));			/* Lire le dernier */

  KillPile(Pile);  						/* d‚truire la pile */
  return(Prochain);						/* Retourner le pr‚c‚dent */
}

#define F_EstDoubleClic(bouton)		(bouton & 0x8000)
/*
  Indique si l'effet double clic a ete effectue sur un objet.
  (L'effet est reconnaissable par la mise a 1 du bit 15 de son entier)
*/

/*
 * -----------------------------------------------------------------------------------------
 *				Miscallenous privates procedures:
 *				Search Procedures
 */

static Type_Event *R_Event(Type_File *FileEvent,int Object)
{
  Type_Event	*Event,*Initial_Event;
    
  Event = FileSommet(FileEvent);
  if(Event == NULL)
    return(NULL);
  else if(Event->Object == Object)
    return(Event);
  else
  {
    Initial_Event = Event;
    FileTourne(FileEvent);
    Event = FileSommet(FileEvent);
    while(!(Event->Object == Object || Event==Initial_Event))
    {
      FileTourne(FileEvent);
      Event = FileSommet(FileEvent);
    }  
    if(Event->Object == Object)
      return(Event);
    else
      return(NULL);  
  } 
}

static Type_Form *R_TreeName(int TreeName)
{
  Type_Form	*Form,*Initial_Poste;
  
  Form = FileSommet(File_Formular);
  if(Form == NULL)
    return(NULL);
  else if(Form->TreeName == TreeName)
    return(Form);
  else
  {
    Initial_Poste = Form;
    FileTourne(File_Formular);
    Form = FileSommet(File_Formular);
    while(!(Form->TreeName == TreeName || Form == Initial_Poste))
    {
      FileTourne(File_Formular);
      Form = FileSommet(File_Formular);
    }
    if(Form->TreeName == TreeName)
      return(Form);
    else
      return(NULL);
  }
}

static Type_Form *R_Handle(int handle)
{
  Type_Form	*Form,*Initial_Poste;
  
  Form = FileSommet(File_Formular);
  if( Form == NULL)
    return(NULL);
  else if(Form->Handle == handle)
    return(Form);
  else
  {
    Initial_Poste = Form;
    FileTourne(File_Formular);
    Form = FileSommet(File_Formular);
    while(!(Form->Handle == handle || Form == Initial_Poste))
    {
      FileTourne(File_Formular);
      Form = FileSommet(File_Formular);
    }
    if(Form->Handle == handle)
      return(Form);
    else
      return(NULL);
  }
}


/*
 * -----------------------------------------------------------------------------------------
 *				Miscallenous privates procedures:
 *				Procedure that call Users code for an action 
 */


static void CallProcess(Type_Form *Form, int Object, int NumberClic, int MouseButton)
/*
	Proc‚dure charg‚e d'appeler le traitement utilisteur d‚finie pour Clic
	ou double clic sur un bouton. Si le bouton est un accŠs … un Popup la
	proc‚dure se charge d'appeler le gestionnaire de Popup du Module PCXPOUP.C

	Entr‚es:	Form		=	Pointeur sur un objet formulaire
				Object		=	Indice de l'objet activ‚
				NumberClic	=	Nombre de clics souris sur l'objet
	Sorties:	rien	
*/
{
	int		EstDbClic;
	Type_Event	*Event;
  
	EstDbClic	= (NumberClic == 2);				/* True si double cliqu‚ */
	Event		= R_Event(Form->FileEvent,Object);	/* Rechercher l'event */
	if(Event != NULL)								/* Event found for this object */
	{ 
		F_Address		= Form->TreeAddress;		/* Address of formular */
		F_NameObject	= Object;					/* Order number object */
		F_Name			= Form->TreeName;			/* Order number of formular */

		/*
		 *	Call User procedure if ONE or TWO clic 
		 */
		if( !EstDbClic && Event->ProcClic != NULL && MouseButton == GAUCHE)
			(*Event->ProcClic)();
		else if(EstDbClic && Event->ProcDbClic != NULL && MouseButton == GAUCHE)
			(*Event->ProcDbClic)();
		else if( !EstDbClic && Event->ProcClicRight != NULL && MouseButton == DROIT)
			(*Event->ProcClicRight)();
		else if(EstDbClic && Event->ProcDbClicRight != NULL && MouseButton == DROIT)
			(*Event->ProcDbClicRight)();
		else										/* More two clics */
			;
	}
	else											/* Sinon c'est un accŠs Popup */
	{
		F_Popup(Form->TreeName,Object, 0, 0);		/* Gestion du Popup menu */
	}
}


/*
 * -----------------------------------------------------------------------------------------
 *				Miscallenous privates procedures:
 *				Procedures used for windows formulars
 */
 
static void WForm_Moved(int handle)
/*
	Proc‚dure appel‚e losrque la fenetre porteuse est d‚pla‡‚e. Dans ce cas
	il faut ajuster la position du formulaire au niveau de sa structure pour que
	tout les objets adopte ce changement.
	
	Entr‚es:	Handle = handle de la fenetre porteuse
	Sorties:	rien
*/
{
  Type_Form	*Form;
  int		Dummy;
  
  Form = R_Handle(handle);
  if(Form != NULL)
  {
    wind_get(handle, WF_WORKXYWH, &Form->TreeAddress->ob_x, &Form->TreeAddress->ob_y, &Dummy, &Dummy);
    if(Form->UserEvent.moved != NULL)
      (*Form->UserEvent.moved)(handle);			/* User redraw proc‚dure */
  }    			
}

static void WForm_Redraw(int handle)
/*
	Called when a redraw event occurs, we must redraw formular.
	Formular will be redrawing with Red_xx variables applied by
	PCXWind.
*/
{
  Type_Form	*Form;
  
  Form = R_Handle(handle);
  if(Form != NULL)
  {							/* Tout redessiner dans le clipping */
    objc_draw(Form->TreeAddress,ROOT,DEPTH,Red_x,Red_y,Red_w,Red_h);
    Form->CursorOn = FALSE;				/* Le curseur est for‡‚ment perdu */
    if(Form->UserEvent.redraw != NULL)
      (*Form->UserEvent.redraw)(handle);		/* User redraw proc‚dure */
  }
}

static void WForm_Close(int handle)
/*
	Called when a close event occurs, we must close the formular like if user
	have cliced the formular's cancel button.
	Searching formular in list to destroy it and close window
*/
{
	Type_Form	*Form;
	
	Form = R_Handle(handle);							/* Search formular */
	if(Form != NULL)									/* If found */
	{
		KillFile(Form->FileEvent);						/* Kill its structures */
		Defiler(File_Formular);							/* D‚truire le formulaire */
	}

	F_Address		=	Form->TreeAddress;				/* Initialize global variables */
	F_NameObject	=	Form->CloseButton;
	F_Name			=	Form->TreeName;
	if(Form->CloseClic != NULL)							/* Call close's code */
		(*Form->CloseClic)();							/* If exist ! */

	W_Close(handle);									/* Close window */

}

static void WForm_UnIconify(int handle)
{
  Type_Form	*Form;
  int		Dummy;
  
  Form = R_Handle(handle);
  if(Form != NULL)
  {
    Form->Suspend = FALSE;				/* Annuler la suspension */
    							/* r‚ajuste le formulaire en position */
    wind_get(handle,WF_WORKXYWH,&Form->TreeAddress->ob_x,&Form->TreeAddress->ob_y,
    				&Dummy,&Dummy);
    if(Form->UserEvent.uniconify != NULL)
      (*Form->UserEvent.uniconify)(handle);		/* User redraw proc‚dure */
    			
  }
}

static void WForm_Iconify(int handle)
{
  Type_Form	*Form;
  
  Form = R_Handle(handle);
  if(Form != NULL)
  {
    Form->Suspend = TRUE;
    if(Form->UserEvent.iconify != NULL)
      (*Form->UserEvent.iconify)(handle);		/* User redraw proc‚dure */
  }
}

/*
 * ------------------- Classic Formular Management with form_do() -------------------
 */
 
static void Form_Process(int TreeName)
/*
	Process a classic formular. It's placed into the File_Formular,
	like a window's formular but it will operate with the standart
	form_do().
*/
{
  int		Object,
  		NbClic;
  Type_Form	*Form;

  Form = R_TreeName(TreeName);					/* Search formular */
  if(Form != NULL)						/* if found */
  {
    do
    {								/* Wait with form_do() */
      Object = form_do(Form->TreeAddress,Form->FirstEdit);
								/* Calculate number of clics */
      NbClic = F_EstDoubleClic(Object) ? 2: 1;
      Object &= 0x7FFF;						/* clear double-clic */
      
      CallProcess(Form, Object, NbClic, GAUCHE);   		 	/* Call Users procedures */
      Form->TreeAddress[Object].ob_state &=~SELECTED;
      F_RedrawObject(Form->TreeAddress,Object);
    }
    while(!Form->Exit);						/* Wait the Exit flag is set */

    KillFile(Form->FileEvent);					/* Kill its structure */
    Defiler(File_Formular);					/* Kill formular */
    form_dial(FMD_SHRINK,X_END,Y_END,W_END,H_END		/* Shrinking box */
    			,Form->TreeAddress->ob_x,Form->TreeAddress->ob_y
    			,Form->TreeAddress->ob_width,Form->TreeAddress->ob_height);

    form_dial(FMD_FINISH,0,0,0,0				/* Redessiner la r‚gion ‚cran */
    			,Form->TreeAddress->ob_x-BORDER,Form->TreeAddress->ob_y-BORDER
    			,Form->TreeAddress->ob_width+2*BORDER,Form->TreeAddress->ob_height+2*BORDER);
  }
  else								/* Pas de formulaire d‚finie */
    form_alert(1,ERROR_FORM);
}

/*
 * ---------------------------- Publics Tools procedure ----------------------------
 */

void F_Selected(int TreeName,int Object, int State)
/* 
	Place dans un ‚tat Selectionne/d‚selectionn‚ un objet.
	
	Entree:		Indice de l'arbre d'objet                         
				Indice de l'objet             
				True pour selected, false pour unselected
	Sortie:		rien    
*/
{
  OBJECT	*Arbre;
  
  if(rsrc_gaddr(R_TREE,TreeName,&Arbre))
  {
  	if( State )
	    Arbre[Object].ob_state |=SELECTED;
	else
	    Arbre[Object].ob_state &=~SELECTED;
    F_RedrawObject(Arbre,Object);
  }
}

void F_Disabled(int TreeName,int Object, int State)
/*
	 Place dans une etat enabled/disabled un objet
	
	 Entree: 	Indice de l'arbre d'objet      
        		Indice de l'objet            
				True pour disabled, false pour enabled
	 Sortie: 	rien  
*/
{
  OBJECT	*Arbre;
  
  if(rsrc_gaddr(R_TREE,TreeName,&Arbre))
  {
  	if( State )
  		Arbre[Object].ob_state |=DISABLED;
  	else
	    Arbre[Object].ob_state &=~DISABLED;
    F_RedrawObject(Arbre,Object);
  }
}

void F_WriteText(int TreeName,int Object,char *string)
/*
	Ecrit une chaine texte dans un champ EDIT du GEM ou un champ TEXT.

	Entree: 	Adresse de l'arbre d'objets
        		Indice de l'edit field
		        Chaine texte … ‚crire

	Sortie:		rien
*/
{
  TEDINFO	*Ted;
  OBJECT	*Arbre;
  
  if(rsrc_gaddr(R_TREE,TreeName,&Arbre))
  {
    Ted=(TEDINFO *) Arbre[Object].ob_spec.tedinfo;
    strcpy (Ted->te_ptext,string);
    F_RedrawObject(Arbre,Object);
  }
}

void F_ReadText(int TreeName,int Object,char *string)
/*
	   Copie une chaine texte ecrite dans un champ EDIT du GEM dans 'string'

	   Entree:	Adresse de l'arbre d'objets
			Indice de l'edit field
			Une chaine o— sera recopi‚ le contenu
	   Sortie:	rien
*/
{
  TEDINFO	*Ted;
  OBJECT	*Arbre;
  
  if(rsrc_gaddr(R_TREE,TreeName,&Arbre))
  {
    Ted=(TEDINFO *) Arbre[Object].ob_spec.tedinfo;
    strcpy (string,Ted->te_ptext);
  }
}

int F_LObject(int TreeName,int Object)
/*
	Retourne la largeur en pixels d'un objet inclu dans un arbre d'objet
	
	Entr‚es:	Indice de l'arbre d'objets
			Indice de l'objet
	Sortie:		Largeur ou 0 si erreur
*/
{
  OBJECT	*Arbre;
  
  if(rsrc_gaddr(R_TREE,TreeName,&Arbre))
    return(Arbre[Object].ob_width);
  else
    return(0);
}

int F_HObject(int TreeName,int Object)
/*
	Retourne la hauteur en pixels d'un objet inclu dans un arbre d'objet
	
	Entr‚es:	Indice de l'arbre d'objets
			Indice de l'objet
	Sortie:		Largeur ou 0 si erreur
*/
{
  OBJECT	*Arbre;
  
  if(rsrc_gaddr(R_TREE,TreeName,&Arbre))
    return(Arbre[Object].ob_height);
  else
    return(0);
}

int F_EstIconifie(int Handle)
/*
	Indique si une fenetre formulaire est iconifiee ou non
	
	Entr‚es:	Handle de la fenetre porteuse du formulaire
	Sorties		True si iconifi‚e
*/
{
  Type_Form	*Form;
  
  Form = R_Handle(Handle);
  if(Form != NULL)
    return(Form->Suspend);
  else
    return(FALSE);
}

void F_RedrawObject(OBJECT *Arbre,int Index)
/* 
	  Redessine localement un objet unique compris dans un arbre d'objet.
	  Le Redraw … lieu sur un niveau d'arborescence.

	  Entree:	Arbre = Pointeur sur l'arbre d'objet
	  		Index = Indice de l'objet
	  Sortie:	rien
*/
{
  objc_draw(Arbre, Index, ONELEVEL, Arbre->ob_x, Arbre->ob_y, Arbre->ob_width, Arbre->ob_height);
}

void F_FreeDraw(int handle,void (*ProcedureDraw)(int x,int y,int w,int h))
/*
	Permet de faire ex‚cuter une proc‚dure de dessin dans les zones visibles
	d'une fenetre formulaire sous le controle de la liste des rectangles.
	pour des sorties AES.(Pour la VDI utiliser dans la proc‚dure User
	W_OnClip et W_OffClip si n‚cessaire)
	
	Entr‚es:	Handle de la fenetre portant le formulaire
			Proc‚dure User de dessin 
	Sorties:	rien
*/
{
  int x_rec,y_rec,w_rec,h_rec;			/* One rectangle from list */
  
  wind_update(BEG_UPDATE); 			/* prevenir l'AES ! */
  wind_get(handle,WF_FIRSTXYWH,&x_rec,&y_rec,&w_rec,&h_rec);
  while(h_rec != 0)					/* While exist rectangles */
  {
    (*ProcedureDraw)(x_rec,y_rec,w_rec,h_rec);	/* Call S/P User */
    wind_get(handle,WF_NEXTXYWH,&x_rec,&y_rec,&w_rec,&h_rec);
  }
  wind_update(END_UPDATE); 			/* all done! */
}

/*
 * -----------------------------------------------------------------------------------------
 *				Publics Aads users procedures
 */
 
int F_Init(void)
/*
	Initialise les structures de donn‚es
	
	Entr‚es:	rien
	Sorties:	True si correct
*/
{
  int	Retour;
  
  File_Formular = CreerFile(sizeof(Type_Form));		/* Liste des formulaires actifs */
  Retour  = _InitDesk();				/* Initialise les structures Desktop */
  Retour &= _InitPopup();				/* Initialise les structures Popups */
  DesktopAddress = NULL;				/* Pas de bureau personnel */
  
  return(File_Formular != NULL && Retour);
}

int F_Exit(void)
/*
	Initialise les structures de donn‚es
	
	Entr‚es:	rien
	Sorties:	True si correct
*/
{
  KillFile(File_Formular);

  _ExitPopup();
  _ExitDesk();
  
  return(TRUE);
}

int F_Open(int TreeName,char *Title,int Mode,int FirstEdit,Type_Form_List *List)
/*
	Ouvre un formulaire en fenetre ou classic. Retourne False si echec, 
	NOWINDOW si ouvert en classic ou le handle de la fenetre si ouvert en fenetre.

	Cette fonction est l'ancien appel d'ouverture. Elle ouvre juste un formulaire
	sans caract‚ristiques sp‚ciales: Cf: F_XOpen(...)
			
	Entr‚es:
	
	Sorties:
*/
{
	Type_Formular_Parameter		*Parameter;
	int							Retour;
    
	Parameter	=	F_GetParameter();
	if(Parameter != NULL)
	{
    	Parameter->TreeName =	TreeName;
	    Parameter->Type		=	MODAL;
	    Parameter->Title	=	Title;
	    Parameter->Mode		=	Mode;
	    Parameter->FirstEdit= FirstEdit;
	    Parameter->List		= List;
	    Parameter->Closer	= TRUE;																		/* allows closer */					
	    Parameter->Smaller 	= FALSE;																	/* not allows smaller */
	    Retour	=	F_XOpen(Parameter);																	/* Call the real function */
	    free(Parameter);
	}
	else
    	Retour = FALSE;

	return(Retour);
}

int F_Close(int TreeName)
/*
	Ferme un formulaire Classic ou Fenetre.
	
	Entr‚es:	
	Sorties:
*/
{
	Type_Form		*Form;
  
	/*
	 *	Searching formular to set its Exit's varaible
	 */
	Form = R_TreeName(TreeName);
	if(Form != NULL)							/* If found */
	{
		Form->Exit = TRUE;						/* Set exit for classic form_do() loop */
		if(Form->Handle != NOWINDOW)			/* If a window formular */
		{
			KillFile(Form->FileEvent);			/* Kill its events structures */
			Defiler(File_Formular);				/* Kill this formular */
			W_Close(Form->Handle);				/* Close window */
		}
		return(TRUE);							/* Correct */
	}
	return(FALSE);								/* Pas de formulaire … fermer */
}

int F_UserEvent(int TreeName, Type_UserEvent *UserEvent)
/*
	Cette fonction installe des routines suppl‚mentaires appel‚es aprŠs
	les traitements des ‚v‚nements re‡us par un formulaire

	Entr‚es:	Entier identifiant le formulaire
			Structure UserEvent
	Sorties:	True si l'installation a r‚ussie False sinon
*/
{
    Type_Form	*Form;
    
    Form = R_TreeName(TreeName);
    if(Form!=NULL)
    {
      Form->UserEvent.moved 	= UserEvent->moved;		/* Supplementary users proc. */
      Form->UserEvent.redraw 	= UserEvent->redraw;
      Form->UserEvent.closed 	= UserEvent->closed;
      Form->UserEvent.iconify	= UserEvent->iconify;
      Form->UserEvent.uniconify = UserEvent->uniconify;
      return(TRUE);
    }
    else
      return(FALSE);     
}

/* --------- News calls --------- */

Type_Formular_Parameter *F_GetParameter(void)
/*
	Alloue un block de paramŠtres pour ouvrir un formulaire et l'initialise
	… des valeurs de d‚fauts.
*/
{
  Type_Formular_Parameter *Parameter;
  
  Parameter = (Type_Formular_Parameter *) malloc( sizeof(Type_Formular_Parameter) );
  if(Parameter != NULL)
  {
    Parameter->TreeName		=	FALSE;
    Parameter->Title		=	NULL;															/* Formular's title */
    Parameter->Mode			=	F_WIND;															/* Opening mode */
    Parameter->FirstEdit	=	NOTEXT;															/* First Editable Text */
    Parameter->List			=	NULL;															/* Event list */
    Parameter->FormIcon		=	NOICON;															/* TreeName icon */
    Parameter->Closer		=	TRUE;															/* allows closer */					
    Parameter->Smaller 		=	TRUE;															/* not allows smaller */
	Parameter->Type			=	MODELESS;														/* Par defaut non modale */
	Parameter->x			=	0;																/* Par defaut non modale */
	Parameter->y			=	0;																/* Par defaut non modale */
	
    Parameter->Mouse		=	ARROW;															/* Mouse's Shape */
    Parameter->MouseForm	=	(MFORM *)NULL;													/* User Mouse's Shape */
  }
  else
    form_alert(1,"[0][ Not Enough Memory | To alloc a block | For Formular opening ][ Ok ]");
  return(Parameter);    
}

int F_XOpen(Type_Formular_Parameter *Parameter)
/*
	Appel ‚tendue d'ouverture d'un formulaire. Cette fonction permet d'ouvrir
	un formulaire tout comme F_Open() mais avec des caract‚ristiques suppl‚mentaires.

	Ouvre un formulaire en fenetre ou classic. Retourne False si echec, 
	NOWINDOW si ouvert en classic ou le handle de la fenetre si ouvert en fenetre.
*/
{
	Type_Form					Form, *Formulaire;
	Type_Event					Event;
	Type_Parameter_Window		*parameter;
	OBJECT						*Arbre;
	unsigned long				Indice;
	int							x, y, w, h, dummy;
    
	/*
	 *	First, check if formular isn't already open !
	 */
	Formulaire = R_TreeName(Parameter->TreeName);													/* Searching this formular */
	if(Formulaire != NULL)																		/* if does exist */
	{
		wind_set(Formulaire->Handle,WF_TOP,0,0,0,0);												/* ONTOP window */
		return(FALSE);																				/* No creating */
	}
	
	/*
	 *	Second, create Formular object and its own event list
	 */

	rsrc_gaddr(R_TREE,Parameter->TreeName,&Arbre);												/* Making New Formular object */
	Form.Exit			=	FALSE;																	/* Exit flag to False */
	Form.TreeName		=	Parameter->TreeName;														/* Name of formular's tree */
	Form.TreeAddress	=	Arbre;																	/* Address of formular's tree */
	Form.Suspend		=	FALSE;																	/* Not icnify */
	Form.FirstEdit		=	Parameter->FirstEdit;														/* 1st editable field */
	Form.CursorOn 		=	FALSE;																	/* Not set on cursor  */
	Form.ObjectHaveMouse=	-1;
	if(Form.FirstEdit != NOTEXT)																	/* If exit field edit */
    	Form.Position	= (int)strlen(Form.TreeAddress[Form.FirstEdit].ob_spec.tedinfo->te_ptext);
	Form.FileEvent	= CreerFile(sizeof(Type_Event));
	if(Form.FileEvent == NULL)
    	return(FALSE);
    	
	/*
	 *	Note, the very first procedure will be called when the window's
	 *	closer button cliced
	 */
	if(Parameter->List == NULL)
	{
    	form_alert(1,"   PCXForm warning | Form List empty ][ok]");
    	return(FALSE);
	}
	
	/*
   	 *	For window close button: It emulate this object button and call this 
     *	procedure.
     *	For example: It could be the Cancel or Ok button.
     */
	Form.CloseClic		=	Parameter->List[0].ProcClic;
	Form.CloseButton	=	Parameter->List[0].Object;
  
	/*
     *	Making the Event's list
     */
  	Indice = 0L;
  	while	(!(Parameter->List[Indice].Object == 0 &&	Parameter->List[Indice].ProcClic == NULL && Parameter->List[Indice].ProcDbClic == NULL && 
														Parameter->List[Indice].ProcClicRight == NULL && Parameter->List[Indice].ProcDbClicRight == NULL &&
														Parameter->List[Indice].ProcMouseEnter == NULL && Parameter->List[Indice].ProcMouseExit == NULL )
			) 
  	{
    	Event.Object			=	Parameter->List[Indice].Object;							/* Indice object */
    	Event.ProcClic			=	Parameter->List[Indice].ProcClic; 						/* User proc. */
   	 	Event.ProcDbClic		=	Parameter->List[Indice].ProcDbClic; 					/* User proc. */
		Event.ProcClicRight		=	Parameter->List[Indice].ProcClicRight;					/* Procedure when right clic */
		Event.ProcDbClicRight	=	Parameter->List[Indice].ProcDbClicRight;				/* Procedure when right double clic */
		Event.ProcMouseEnter	=	Parameter->List[Indice].ProcMouseEnter;					/* Procedure when mouse enter to the object */
		Event.ProcMouseExit		=	Parameter->List[Indice].ProcMouseExit;					/* Procedure when mouse exit to the object  */					

    	if(!Emfiler(Form.FileEvent,&Event))				/* Insert this event */
      		return(FALSE);
    	Indice++;
	}
  
	/*
	 *	Third, try to open formular into a window if asked.
	 */
  
	if(Parameter->Mode == F_WIND)
	{
   		parameter = W_GetParameter();																		/* Get a WINDOW blok to create */
		if(parameter == NULL)																				/* Erreur d'allocation */
      		return(FALSE);																					/* Abandonner */
      		
    	if(Parameter->Title != NULL)
      		strcpy(parameter->titre,Parameter->Title);	/* Copier la chaine titre */
    	else
      		strcpy(parameter->titre,"Without title");														/* Default title */

		parameter->type = Parameter->Type;																	/* Type de fenetre, Modale/Modeless */
    	
		parameter->attributs = ATTRIBUTS;																	/* Mover and Name */
    	if(Parameter->Closer)
      		parameter->attributs |= CLOSER;																	/* Allowed closer */
    	if(Parameter->Smaller)
      		parameter->attributs |= SMALLER;																/* Allowed smaller */
    
    	parameter->op_redraw	=	WForm_Redraw;															/* AADS Formular procedures */
    	parameter->op_close		=	WForm_Close;
    	parameter->op_moved		=	WForm_Moved;
    	parameter->op_iconify	=	WForm_Iconify;
    	parameter->op_uniconify	=	WForm_UnIconify;
    
    	parameter->treename		=	Parameter->FormIcon;													/* Copy Object Tree Icon */
    	parameter->isiconify	=	FALSE;

    	parameter->Mouse		= Parameter->Mouse;
    	parameter->MouseForm	= Parameter->MouseForm;

		/*	So on a passe des coordonn‚es pour ouvrir la fenetre formulaire */
		if ( Parameter->x || Parameter->y )
		{
			/*	d‚terminer la taille de fenetre n‚cessaire pour le formulaire */
			parameter->x	=	Parameter->x;
			parameter->y	=	Parameter->y;
			wind_calc(FALSE, parameter->attributs, 0, 0, Arbre[0].ob_width, Arbre[0].ob_height, &dummy, &dummy, &parameter->w, &parameter->h);
			
			/*	modifier l'origine (x,y) du formulaire pour suivre l'emplacement de la fenetre */
			wind_calc(TRUE,  parameter->attributs, parameter->x, parameter->y, parameter->w, parameter->h, &x, &y, &dummy, &dummy);
			Arbre[0].ob_x	=	x;
			Arbre[0].ob_y	=	y;			
		}
		/*	Sinon, on centre la fenetre par rapport au formulaire */
		else																			
		{        
			form_center(Arbre, &x, &y, &w, &h);																	/* Coord. centr‚es du formulaire */
			wind_calc(FALSE, parameter->attributs, x, y, w, h, &parameter->x, &parameter->y, &parameter->w, &parameter->h);	    /* Calculate correct (x,y,w,h) */
		}
		
    	Form.Handle = W_Open(parameter);																	/* And... Opening ! */
    	free(parameter);

    	Form.UserEvent.moved		=	NULL;																/* Supplementary users proc. */
    	Form.UserEvent.redraw		=	NULL;
    	Form.UserEvent.iconify		=	NULL;
    	Form.UserEvent.uniconify	=	NULL;      

		if(!Emfiler(File_Formular,&Form))																	/* Insert this formular object */
			return(FALSE);
	}

	/*
	 *		Classic formular open if asked OR if window open failled !!
	 */  
	if(Parameter->Mode == F_CLASSIC || Form.Handle < DESKTOP)		
	{
		if(Parameter->Mode == F_CLASSIC)																		/* cas du mode Classic demand‚ */ 
		{
      		Form.Handle = NOWINDOW;																				/* Pas de handle fenetre */
			if(!Emfiler(File_Formular,&Form))																	/* Emfiler ‚chec */
				return(FALSE);																					/* Erreur */
		}
    	form_center(Arbre, &x, &y, &w, &h);																		/* coordonn‚es centr‚es */
    	form_dial(FMD_GROW ,X_START,Y_START,W_START,H_START, x, y, w, h);								    	/* Boite grandissante */
    	form_dial(FMD_START,0,0,0,0, x-BORDER, y-BORDER, w+2*BORDER, h+2*BORDER);								/* sauvegarde r‚gion ‚cran */
		objc_draw(Arbre,ROOT, DEPTH, x, y, w, h);																/* dessiner le formulaire */

		Form_Process(Parameter->TreeName);																		/* Gestion bas‚ sur form_do() */
	}
	return(Form.Handle);																						/* Retourner handle fenetre porteuse */
}

/*
 * -----------------------------------------------------------------------------------------
 *				Private Aads procedure:	Event Manager Procedure
 */

static void KeybdEvent(void)
/*
	Proc‚dure priv‚e de gestion des ‚v‚nements clavier pour un formulaire.
	
	Entr‚es:	A_Key
	Sorties:
*/
{
	int		handle,dummy,Object;
	Type_Form	*Form;
	
	wind_get(DESKTOP, WF_TOP, &handle, &dummy, &dummy, &dummy);
	if(handle <= DESKTOP)
		return;
	
	Form = R_Handle(handle);
	Form = ( (Form != NULL) && (Form->Suspend) ) ? NULL : Form;
  
	if(Form != NULL)
	{ 
		if((A_Key & 0x00FF) == 0x0D)				/* touche Return */
		{
			Object = 0;						/* Tout premier enfant */
			while(!( (Form->TreeAddress[Object].ob_flags & DEFAULT) ||	(Form->TreeAddress[Object].ob_flags & LASTOB) ))
			Object ++;						/* Objet suivant */
			if((Form->TreeAddress[Object].ob_flags & DEFAULT) && !(Form->TreeAddress[Object].ob_state & DISABLED))
			{
				Form->TreeAddress[Object].ob_state |= SELECTED;		/* S‚lectionner objet */
				F_RedrawObject(Form->TreeAddress,Object);		/* Redessiner */
				CallProcess(Form, Object, 1, GAUCHE);				/* User Code */
				Form->TreeAddress[Object].ob_state &=~SELECTED;		/* D‚selectionner */
			}
			A_Event = MU_NULL;					/* Kill event pour M_Event */
		}	
		else if(Form->FirstEdit != FALSE)				/* Il existe une edit field */
		{	
			if(!Form->CursorOn)					/* r‚activer le curseur */
			{
				objc_edit(Form->TreeAddress, Form->FirstEdit, 0,	&Form->Position, ED_INIT);
				Form->CursorOn = TRUE;
			}
			if(A_Key == SUP)						/* TOUT PREMIER Editable */
			{
				objc_edit(Form->TreeAddress, Form->FirstEdit, 0,  &Form->Position,ED_END);
				Form->FirstEdit = GetPere(Form->TreeAddress,Form->FirstEdit);	/* pŠre de tous */
				Form->FirstEdit = Form->TreeAddress[Form->FirstEdit].ob_tail;	/* Prendre dernier */
				Form->FirstEdit = _Prochain(Form,Form->FirstEdit);	/* Calculer prochain */
				Form->Position  = (int)strlen(Form->TreeAddress[Form->FirstEdit].ob_spec.tedinfo->te_ptext);
				objc_edit(Form->TreeAddress,Form->FirstEdit, 0, &Form->Position,ED_INIT);
			}
			else if(A_Key == SDOWN)					/* TOUT DERNIER Editable */
			{
				objc_edit(Form->TreeAddress,Form->FirstEdit,0, &Form->Position,ED_END);
				Form->FirstEdit = GetPere(Form->TreeAddress,Form->FirstEdit);
				Form->FirstEdit = Form->TreeAddress[Form->FirstEdit].ob_head;
				Form->FirstEdit = PrevEdit(Form,Form->FirstEdit);	/* Calculer pr‚c‚dent */        
				Form->Position  = (int)strlen(Form->TreeAddress[Form->FirstEdit].ob_spec.tedinfo->te_ptext);
				objc_edit(Form->TreeAddress,Form->FirstEdit,0,
				&Form->Position,ED_INIT);
			}
			else if(A_Key == SLEFT)
			{
				while(Form->Position != 0)
				objc_edit(Form->TreeAddress,Form->FirstEdit,LEFT, &Form->Position,ED_CHAR);
			}
			else if(A_Key == SRIGHT)
			{
				objc_edit(Form->TreeAddress,Form->FirstEdit,A_Key, &Form->Position,ED_END);
				Form->Position  = (int)strlen(Form->TreeAddress[Form->FirstEdit].ob_spec.tedinfo->te_ptext);
				objc_edit(Form->TreeAddress,Form->FirstEdit,A_Key,
				&Form->Position,ED_INIT);
			}
			else if((A_Key & 0xFF00) == UP)
			{
				objc_edit(Form->TreeAddress,Form->FirstEdit,0, &Form->Position,ED_END);
				Form->FirstEdit = PrevEdit(Form,Form->FirstEdit);
				Form->Position  = (int)strlen(Form->TreeAddress[Form->FirstEdit].ob_spec.tedinfo->te_ptext);
				objc_edit(Form->TreeAddress,Form->FirstEdit,0,
				&Form->Position,ED_INIT);
			}
			else if((A_Key & 0xFF00) == DOWN)
			{
				objc_edit(Form->TreeAddress,Form->FirstEdit,A_Key, &Form->Position,ED_END);
				Form->FirstEdit = NextEdit(Form,Form->FirstEdit);
				Form->Position  = (int)strlen(Form->TreeAddress[Form->FirstEdit].ob_spec.tedinfo->te_ptext);
				objc_edit(Form->TreeAddress,Form->FirstEdit,A_Key,
				&Form->Position,ED_INIT);
			}
			else
			{              
				objc_edit(Form->TreeAddress,Form->FirstEdit,A_Key,
				&Form->Position,ED_CHAR);
			}
			A_Event = MU_NULL;						/* Kill event pour M_Event */
		}
	}/*EndIf Form!=NULL*/
}

static void MouseFly( Type_Form *Form, int Object )
{
	Type_Event	*Event;
  	
	if( Form->ObjectHaveMouse != Object )
	{
		Event	= R_Event(Form->FileEvent, Form->ObjectHaveMouse);	
		if( Event != NULL && Event->ProcMouseExit != NULL)
		{
			F_NameObject			=	Form->ObjectHaveMouse;
			(*Event->ProcMouseExit)();
		}

		Event	= R_Event(Form->FileEvent, Object);
		if( Event != NULL && Event->ProcMouseEnter != NULL )
		{
			Form->ObjectHaveMouse 	=	Object;
			F_NameObject			=	Object;
			(*Event->ProcMouseEnter)();
		}
		else
			Form->ObjectHaveMouse	=	-1;
	}
}

/*
 * -----------------------------------------------------------------------------------------
 *				Publics Aads users procedure:
 *				Event Manager Procedure
 */

void F_Event(void)
/*
	Controle la gestion des evenements Souris et Clavier, pour les formulaires en
	mode fenetres.

	Entrees: VAR Globales A_Event, A_Clic
	Sorties: rien
*/
{
	int				Bouton, Dummy, MouseX, MouseY, Handle, Handle_Top, Object, NewObject;
	int				Fils, Pere;				
	Type_Form		*Form;					/* Un objet formulaire */
	Type_Window		*window;	


	/*
	 *	EvŠnement clic souris
	 */  

	Handle = wind_find(A_MouseX,A_MouseY);																/* Quelle fenetre en x,y ? */
	wind_get(DESKTOP, WF_TOP, &Handle_Top, &Dummy, &Dummy, &Dummy);										/*	Quelle fenetre TOP ? */

	if( A_Event & MU_BUTTON )																			/* Event Clic Souris */
	{	
		/*	Continuer en analysant fenetre bureau ou fenetre GEM si bien TOP */
		if(Handle <= DESKTOP)																			/* Si est celle du Bureau */
		{
			window = WindSearch(Handle_Top);															/* Rechercher la fenetre TOP */
			if(window != NULL)
			{
				if(window->type & MODAL)																/* Si la fenetre courante est MODALE */
				{	
					if( (A_Event & MU_BUTTON) || (A_Event & MU_KEYBD) )									/* On n'accepte pas les clics sur bureau */
					{
						A_Event	&=	~MU_BUTTON;															/* D‚truire l'event completement */
						A_Event	&=	~MU_KEYBD;															/* D‚truire l'event completement */
						return;																			/* Et quitter */
					}
				}
      		}
	   		Form = NULL;
    	}
		else																							/* Une autre...*/
    	{																								/* rechercher celle en Top */
	    	if(Handle == Handle_Top)																		/* Celle cliquee = Celle Top */
			{
				Form   = R_Handle(Handle);																	/* Rechercher le formulaire */
				Form = ( (Form != NULL) && (Form->Suspend) ) ? NULL : Form;
			}
			else																							/* Elle n'est pas TOP */
				Form = NULL;
		}

    	/*
		 *	Si un Formulaire est trouv‚ pour l'objet incrimin‚
	     */  
		if( (Form != NULL) && (A_MouseButton & GAUCHE) )					/* Si Form correspondant trouve */
		{
			/*
			 *	Activer le curseur dans le premier champ edit si il existe
        	 */
			if(!Form->CursorOn)
			{
				objc_edit(Form->TreeAddress, Form->FirstEdit, 0, &Form->Position, ED_INIT);
				Form->CursorOn = TRUE;
			}
			
			/*
			 *	Rechercher l'objet en MouseX,MouseY de la souris
			 */

			Object = objc_find( Form->TreeAddress, ROOT, DEPTH, A_MouseX, A_MouseY ); 
			if(Object > ROOT)												/* L'objet n'est pas la racine */
			{
				if(!(Form->TreeAddress[Object].ob_state & DISABLED))
				{
					if(!(Form->TreeAddress[Object].ob_flags & TOUCHEXIT))	/* Objet Non TouchExit */
					{
						if(Form->TreeAddress[Object].ob_flags & SELECTABLE)	/* Est s‚lectable */
						{
							do												/* Attendre relacher souris */
							{
								graf_mkstate(&MouseX, &MouseY, &Bouton, &Dummy);
								NewObject = objc_find(Form->TreeAddress, ROOT, DEPTH, MouseX, MouseY);      
								if(NewObject == Object)
								{
							 	if( !(Form->TreeAddress[Object].ob_state & SELECTED) )
									{
										Form->TreeAddress[Object].ob_state |= SELECTED;	/* Le d‚s‚lectionner */
										F_RedrawObject(Form->TreeAddress, Object);
									}
								}
								else
								{
									if( Form->TreeAddress[Object].ob_state & SELECTED )
									{
										Form->TreeAddress[Object].ob_state &= ~SELECTED;
										F_RedrawObject(Form->TreeAddress, Object);
									}
								}	          
							}
							while(Bouton & GAUCHE);

							/*	Check if mouse always under the selected object */
							NewObject = objc_find(Form->TreeAddress, ROOT, DEPTH, MouseX, MouseY);      
							if(NewObject != Object)
								return;
						}
					}
					else							/* Objet TouchExit */
					{
						CallProcess(Form,Object,A_Clic, GAUCHE);
					}

					/*
					 *	Objet classique: Le d‚selectionner et appeler le code User
					 */          
		 
					if((Form->TreeAddress[Object].ob_flags & SELECTABLE) && !(Form->TreeAddress[Object].ob_flags & RBUTTON))
					{
						Form->TreeAddress[Object].ob_state &=~SELECTED;
						F_RedrawObject(Form->TreeAddress,Object);
						CallProcess(Form,Object,A_Clic, GAUCHE);
					}
    
					/*
					 *	Objet RadioBouton:   Le laisser s‚lectionner et inhiber ses frŠres
					 *			     puis appeler le code User
		   			 */
	
					if((Form->TreeAddress[Object].ob_flags & SELECTABLE) && (Form->TreeAddress[Object].ob_flags & RBUTTON))
					{
						Pere = GetPere(Form->TreeAddress,Object);		/* PŠre de Object */
						Fils = Form->TreeAddress[Pere].ob_head;		/* Premier enfant */
						while(Fils != Pere)					/* Pour tout les enfants */
						{
							if(Fils != Object)				/* Si n'est pas Object */
							{
								Form->TreeAddress[Fils].ob_state &= ~SELECTED;	/* Le d‚s‚lecter */
								F_RedrawObject(Form->TreeAddress,Fils);
							}
							Fils = Form->TreeAddress[Fils].ob_next;		/* Prochain enfant */
						}
						CallProcess(Form,Object,A_Clic, GAUCHE);			/* User Code */
					}
			
					/*
					 *	Champ ‚ditable: Permettre son ‚dition au clavier
					 */
			 
					if(Form->TreeAddress[Object].ob_flags & EDITABLE)
					{
					
						objc_edit(	Form->TreeAddress, Form->FirstEdit, 0, &Form->Position, ED_END);
						Form->FirstEdit = Object;
						Form->Position =(int)strlen(Form->TreeAddress[Object].ob_spec.tedinfo->te_ptext);
						objc_edit(	Form->TreeAddress, Object, 0, &Form->Position, ED_INIT);
						
					}	/*Endif Object edit field*/
				}	/*Endif Object Not Disable*/
			}	/*Endif Object > ROOT*/
		}	/*Endif Form != NULL)*/
		else if(Handle == DESKTOP)																				/* Clic sur le bureau  */
		{
			_DeskEvent();																							/* Gestion des icones  */
		}
		else 																									/* Sinon, par pour nous */
			;
	}  
	/*
	 *	EvŠnement clavier peut etre traitable
	 */
	else if(A_Event & MU_KEYBD)																				/* Si est Event keyboard */
	{
		KeybdEvent();
	}
	/*	Evenement timer GEM:	Si la fenetre survollee par la souris est celle active et non bureau	*/
	else if( (A_Event & MU_TIMER) && Handle > DESKTOP && Handle == Handle_Top )
	{
		/*	Si cette fenetre contient un formulaire => activer gestion survol des objets */
		Form   = R_Handle(Handle);																	/* Rechercher le formulaire */
		Form = ( (Form != NULL) && (Form->Suspend) ) ? NULL : Form;
		if( Form != NULL )
		{
			Object = objc_find( Form->TreeAddress, ROOT, DEPTH, A_MouseX, A_MouseY ); 
			MouseFly( Form, Object );
		}
	}
	else																										/* Sinon autre event */
		;
}
