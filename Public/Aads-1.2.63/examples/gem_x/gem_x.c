/*
 * ----------------------------------------------------------------------------------------------
 *				TEMPLATE d'application standard sous A.A.D.S pour Atari FALCON.
 *				Interface GEM graphique nromalis‚e, sous AES 3.4 minimum.
 *
 * 	Author  : 	FAVARD Laurent, FunShip (c)
 *	File    : 	GEM_X.C
 *	Date    : 	6  Avril 1994
 *	Release : 	10 december 1996
 *	Version : 	1.0
 *	Country : 	FRANCE
 *			
 *
 *				- Copyright Atari FunShip (c) 1994-96 -
 *	 		     - ATARI ST,STE,TT, Falcon, Medusa et Hades-
 * ----------------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <tos.h>

#include <PcAads.h>

/*
 * ----------------------------------------------------------------------------------------------
 *			Fichiers sp‚cifiques … inclure
 */

#include	"Tools.h"
#include	"GEM_X__F.H"			

/*
 * ----------------------------------------------------------------------------------------------
 *			D‚finitions standards du template d'application.
 */

/*	Carract‚ristiques globales de cette application */

#define		G_APP_NUMBER		"Version : 0.00"
#define		G_APP_DATE		"25 Novembre 1996"	
#define		G_APP_NAME		"Application template GEM_X"
#define		G_APP_INF		"GEM_X.INF"
 
#define		G_FILE_RESSOURCE	"GEM_X__?.Rsc"

#define		G_NO_ERROR		0
#define		G_RESSOURCE_ERROR	-1
#define		G_SCREEN_SIZE_ERROR	-2
#define		G_COLOR_ERROR		-3

/*	Type d'‚cran necessaire */

#define		G_WIDTH_SCREEN		639
#define		G_HEIGHT_SCREEN		399
#define		G_COLOR			16

#define		MX_STRAM		0
#define		MX_TTRAM		1

#define		NB_ITEMS		10

#define		G_MSG_ERROR_WIND	"[2][ Not Enough Resource Available | To open this window][Ok]"


/*
 * ----------------------------------------------------------------------------------------------
 *				Variables globales standards de l'application
 * ----------------------------------------------------------------------------------------------
 */

int			SaveOnExit;								/* Sauvegarder en quittant le bureau ? */
int			ToolBarActive;								/* ToolBar de l'application active */
static Type_AutoTop	AutoTop,								/* Auto top fenetre d‚finitions actif*/
			AutoTop_Copy;								/* Auto top sauveagrde */
int			GrowShrink,								/* Effetx visuels sur les fenetres */
			GrowShrink_Copy;
long			Trame;
OBJECT			*MessageTexte;								/* Arbre d'objets des Message textes  */

/*
 * ----------------------------------------------------------------------------------------------
 *				Variables globales priv‚es
 * ----------------------------------------------------------------------------------------------
 */

 
static	int		Sortir = FALSE;								/* Variable pour indiquer la fin de l'application */
static	int		WHInfo,WHExit;								/* Handle AES des fenetres standard */
static	int		NombreClicPhoto = 1;							/* Compteur de  clics sur la photo cach‚e */

/*
 * ----------------------------------------------------------------------------------------------
 *		Description de la barre de menu principale de l'application
 * ----------------------------------------------------------------------------------------------
 */
 
void	AboutMe(void);
void	Ouvrir(void);
void	Fichier(void);
void	Quitter(void);
void	ToolBar();
void	Options();
void	SaveQuit(void);

Type_Parameter_Menu
BarreMenu[]={	
		{APROPOS,	INITIAL,	AboutMe},					/* A propos de ... */
		/*	Menu fichier */
		{F_NEW,		INITIAL,	Fichier},
		{F_OPEN,	INITIAL,	Fichier},						/* Ouvrir un fichier */
		{F_CLOSE,	INITIAL,	Fichier},					/* Fermer un fichier */
		{F_SAVE,	INITIAL,	Fichier},
		{F_SAVEAS,	INITIAL,	Fichier},
	  	{F_QUIT,	INITIAL,	Quitter},					/* Quitter l'application */
		/*	Menu Edition */
		{E_UNDO,	INITIAL,	NULL},
		{E_COUPER,	INITIAL,	NULL},
		{E_COPIER,	INITIAL,	NULL},
		{E_COLLER,	INITIAL,	NULL},
		/*	Menu Option */
		{O_OPT,		INITIAL,	Options},					/* Fentere de param‚trage de l'application */
		{O_BARRE,	INITIAL,	ToolBar},					/* Barre d'outils ToolBox */
	  	{O_SAVEQT,	INITIAL,	SaveQuit},					/* Sauver le bureau en quittant l'application */
	  	{O_SAVEDK,	INITIAL,	NULL},						/* Sauver le bureau maintenant  */
		{O_SAVEOP,	INITIAL,	NULL},
		/*	Menu AIDE */
		{AIDE,	INITIAL,	NULL},	  	
	  	MENU_END
	   };

/*
 * ----------------------------------------------------------------------------------------------
 *		Description du bureau de l'application (Icones, op‚rations possibles, ...)
 * ----------------------------------------------------------------------------------------------
 */

void	DbClicIcon(void);
void	Move(int typemove,int target);

Type_Bureau_List
Bureau[]=  {	
		{IDISK	,25  ,0,"DISQUES"	,NULL,DbClicIcon, NULL, NULL, Move, Move},
		{IFILE	,25 ,50,"FICHIER ASM"	,NULL,DbClicIcon, NULL, NULL,Move,NULL},
		{ITRASH	,25,200,"CORBEILLE"	,NULL,DbClicIcon, NULL, NULL,Move,Move},
		BUREAU_END
	   };

/*
 * ----------------------------------------------------------------------------------------------
 *		Description des fenetres "A propos de..." et "Quitter l'application"
 * ----------------------------------------------------------------------------------------------
 */

void	QuitAction(void);

static Type_Form_List
FormExit[]=	{	{BN_2,	QuitAction,	NULL},							/* Bouton Annuler */
			{BN_1,	QuitAction,	NULL},							/* Bouton OK */
			FORM_END
		};

void	AboutOk(void);
void	PhotoCachee(void);

static Type_Form_List
FormInfo[]=	{	{BN_3,	AboutOk,	PhotoCachee},
			FORM_END
		};

/*
 * ----------------------------------------------------------------------------------------------
 *				Description de la fenetre "Param‚trage..."
 * ----------------------------------------------------------------------------------------------
 */

void	OptionsAction(void);

static	Type_Form_List
FormOptions[]=	{
			{BN_5,	OptionsAction,	NULL},							/* Bouton Annuler */
			{BN_4,	OptionsAction,	NULL},							/* Bouton OK */
			{BN_6,	OptionsAction,	NULL},							/* Auto TOP fenetre */
			{BN_7,	OptionsAction,	NULL},							/* Sur souris fixe */
			{BN_8,	OptionsAction,	NULL},							/* Sur toute la fenetre */
			{BN_9,	OptionsAction,	NULL},							/* Effets visuels fenetres */

			{TRAME_1,	OptionsAction,	NULL},						/*  */
			{TRAME_2,	OptionsAction,	NULL},						/*  */
			{TRAME_3,	OptionsAction,	NULL},						/*  */
			{TRAME_4,	OptionsAction,	NULL},						/*  */
			{TRAME_5,	OptionsAction,	NULL},						/*  */
			{TRAME_6,	OptionsAction,	NULL},						/*  */
			{TRAME_7,	OptionsAction,	NULL},						/*  */
			{TRAME_8,	OptionsAction,	NULL},						/*  */

			FORM_END
		};
/*
 * ----------------------------------------------------------------------------------------------
 *				Description de la Barre d'outils de l'application
 * ----------------------------------------------------------------------------------------------
 */

void ToolBarAction(void);

static	Type_Form_List
FormToolBox[]=	{
			{0,	ToolBarAction,		NULL},
			{BN_100,Quitter,		NULL},							/* Bouton quitter application */
			{BN_101,Options,		NULL},
			{BN_102,AboutMe,		NULL},
			FORM_END
		};

/*
 * ----------------------------------------------------------------------------------------------
 *				Description de tous les menus popups
 * ----------------------------------------------------------------------------------------------
 */

static Type_Parameter_PopAttach										/* Description de tous les menus poups */
ListPopAttach[]={
		  {PCOULEUR,	BN_10	,COLOR_1	,SCROLL_LISTBOX		,TRUE},
		  POPATTACH_END
	        };

void PopupAction(void);

Type_Parameter_PopEvent										/* Attachement des actions … chaque items de tous les menus popups */
ListPopEvent[]=	{	
		  {PCOULEUR,	COLOR_1		,PopupAction},
		  {PCOULEUR,	COLOR_2		,PopupAction},
		  {PCOULEUR,	COLOR_3		,PopupAction},
		  {PCOULEUR,	COLOR_4		,PopupAction},
		  {PCOULEUR,	COLOR_5		,PopupAction},
		  {PCOULEUR,	COLOR_6		,PopupAction},
		  {PCOULEUR,	COLOR_7		,PopupAction},
		  {PCOULEUR,	COLOR_8		,PopupAction},
		  {PCOULEUR,	COLOR_9		,PopupAction},
		  {PCOULEUR,	COLOR_10	,PopupAction},
		  {PCOULEUR,	COLOR_11	,PopupAction},
		  {PCOULEUR,	COLOR_12	,PopupAction},
		  {PCOULEUR,	COLOR_13	,PopupAction},
		  {PCOULEUR,	COLOR_14	,PopupAction},
		  {PCOULEUR,	COLOR_15	,PopupAction},
		  {PCOULEUR,	COLOR_16	,PopupAction},
		  POPEVENT_END
  		};  				   
		
/*
 * ----------------------------------------------------------------------------------------------
 *				Gestion de la photo cach‚e
 * ----------------------------------------------------------------------------------------------
 */

void PhotoCachee(void)
/*
	Called by Double-Clic on Exit button of about me window
*/
{
  OBJECT	*Arbre;
  
  if(NombreClicPhoto == 3)										/* Rendre la photo visible */
  {
      rsrc_gaddr(R_TREE, FAPROPOS, &Arbre);								/* Bit objet invisible Off */
      Arbre[FUNSHIP].ob_flags &= ~HIDETREE;	
      F_RedrawObject(Arbre, FUNSHIP);									/* Redessiner la photo */
      NombreClicPhoto = 1;
  } 
  else													/* Compter le nombre de Double clics */
    NombreClicPhoto++;
}

/*
 * ----------------------------------------------------------------------------------------------
 *				Double clic et mouvement sur les icones du bureau
 * ----------------------------------------------------------------------------------------------
 */

void DbClicIcon(void)
/*
	Appel‚ quand un icone est cliqu‚ ou double-cliqu‚.
*/
{  	
  switch(I_Name)
  {
    case IDISK:  	
    			break;
    case IFILE:		
  			break;
    case ITRASH:
    			break;
    default:		break;    
  }
}

void Move(int Movment,int Target)
{
  switch(Movment)
  {
    case ICON2ICON:	
    			break;
    default:	
    			break;	
  }
}

/*
 * ----------------------------------------------------------------------------------------------
 *			Fenetre "A propos de..." de l'application
 * ----------------------------------------------------------------------------------------------
 */

void AboutMe(void)
/*
	Ouvre la fenetre A propos de ... de l'application.
*/
{
  Type_Formular_Parameter	*Parameter;
  unsigned long			L_lSTRam, L_lTTRam;
  char				L_sSTRam[20], L_sTTRam[20], L_sVideo[20];
  
  Parameter = F_GetParameter();
  if(Parameter != NULL)
  {
    Parameter->TreeName = FAPROPOS;									/* Indice de l'arbre de l'objet */
    Parameter->Title	= MessageTexte[MSG_1].ob_spec.free_string;					/* Titre de la fenetre */
    Parameter->Mode	= F_WIND;									/* Opening mode */
    Parameter->FirstEdit= NOTEXT;									/* First Editable Text */
    Parameter->List	= FormInfo;									/* Event list */
    Parameter->FormIcon = IINFO;									/* Icone d'iconification */
    if((WHInfo = F_XOpen(Parameter)) != FALSE)
    {
      F_WriteText(FAPROPOS, APPNAME, G_APP_NAME);							/* Nom de cette application */
      F_WriteText(FAPROPOS, VERSION, G_APP_NUMBER);							/* Version de l'application */
      F_WriteText(FAPROPOS, APPDATE, G_APP_DATE);							/* Date de cette version */

      L_lSTRam	=	(unsigned long)Mxalloc(-1L, MX_STRAM);						/* Taille m‚moire ST */
      L_lTTRam	=	(unsigned long)Mxalloc(-1L, MX_TTRAM);						/* Taille m‚moire TT */
      
      sprintf(L_sSTRam, "%ld Octets", L_lSTRam);
      sprintf(L_sTTRam, "%ld Octets", L_lTTRam);      
      sprintf(L_sVideo, "%d.%d.%d Couleurs", VdiInfo.LargeurStation, VdiInfo.HauteurStation, VdiInfo.NombreCouleurs);
      
      F_WriteText(FAPROPOS, STRAM,   L_sSTRam);								/* M‚moire ST disponible */
      F_WriteText(FAPROPOS, TTRAM,   L_sTTRam);								/* M‚moire TT disponible */
      F_WriteText(FAPROPOS, VIDEO,   L_sVideo);								/* R‚solution courante */
    }
    free(Parameter);
  }
}

void AboutOk(void)
{
  OBJECT	*Arbre;
  
  rsrc_gaddr(R_TREE,FAPROPOS, &Arbre);									/* Tree address */
  Arbre[FUNSHIP].ob_flags |= HIDETREE;									/* Hide photo */
  F_Close( FAPROPOS );
}

/*
 * ----------------------------------------------------------------------------------------------
 *			Fenetre "Fin de l'application..." de l'application
 * ----------------------------------------------------------------------------------------------
 */

void Quitter(void)
/*
	Ouvre la fenetre de demande de fin de l'application.
	
*/
{
  WHExit = F_Open(FEXIT, MessageTexte[MSG_2].ob_spec.free_string, F_WIND,NOTEXT, FormExit);
}

void QuitAction(void)
/*
	Suivant le bouton cliqu‚ dans la fenetre de fin d'application, assigne la variable 'Sortir'.
*/
{
  Sortir = (F_NameObject == BN_1) ? TRUE : FALSE;							/* Sortir si BN_1 … ‚t‚ cliqu‚ */
  F_Close(FEXIT);											/* Ferme la fenetre de fin d'application */
}

/*
 * ----------------------------------------------------------------------------------------------
 *			Fenetre "Param‚trages" du menu option l'application
 * ----------------------------------------------------------------------------------------------
 */

void Options(void)
/*
	Ouvre la fenetre "Param‚trage..." de l'application.
*/
{
  Type_Formular_Parameter	*Parameter;
  
  Parameter = F_GetParameter();
  if(Parameter != NULL)
  {
    Parameter->TreeName = FOPTIONS;									/* Indice de l'arbre de l'objet */
    Parameter->Title	= "ParamŠtres...";								/* Titre de la fenetre */
    Parameter->Mode	= F_WIND;									/* Opening mode */
    Parameter->FirstEdit= NOTEXT;									/* First Editable Text */
    Parameter->List	= FormOptions;									/* Event list */
    Parameter->FormIcon = IINFO;									/* Icone d'iconification */
    if((WHInfo = F_XOpen(Parameter)) != FALSE)
    {
      AutoTop_Copy	= AutoTop;
      Trame		= DesktopAddress[0].ob_spec.index;
      GrowShrink_Copy	= GrowShrink;
    }
    free(Parameter);
  }
}

void OptionsAction(void)
/*
	R‚ponse … l'action sur les boutons de la fenetre "Param‚trage..."
*/
{
  switch(F_NameObject)
  {
    /*	Bouton Annuler:	=>	Repla‡er les anciennes valeurs */
    case	BN_5:	AutoTop	   = AutoTop_Copy;
    			GrowShrink = GrowShrink_Copy;
    			if(AutoTop.AutoEnable)		F_Select(FOPTIONS, BN_6);
    			else				F_UnSelect(FOPTIONS, BN_6);
    			if(AutoTop.NoMouseMovement)	F_Select(FOPTIONS, BN_7);
    			else				F_UnSelect(FOPTIONS, BN_7);
   			if(AutoTop.OnlyWorkArea)	F_Select(FOPTIONS, BN_8);
    			else				F_UnSelect(FOPTIONS, BN_8);
    			if(GrowShrink)			F_Select(FOPTIONS, BN_9);
    			else				F_UnSelect(FOPTIONS, BN_9);
    			F_Close(FOPTIONS);
    			break;
    			
    /* Bouton Valider: =>	Appliquer les choix */
    case	BN_4:	W_AutoTop(&AutoTop);							/* Set auto top mode */
			W_VisualClue(GrowShrink);						/* Set effets visuels des fenetres */
			DesktopAddress[0].ob_spec.index = Trame;
    			F_Close(FOPTIONS);
  			form_dial(FMD_FINISH,0,0,0,0,DesktopAddress[0].ob_x,DesktopAddress[0].ob_y,DesktopAddress[0].ob_width,DesktopAddress[0].ob_height);
    			break;
    			
    case	BN_6:	AutoTop.AutoEnable = 0x0001 & ~(AutoTop.AutoEnable);
    			if(AutoTop.AutoEnable)		F_Select(FOPTIONS, BN_6);
    			else				F_UnSelect(FOPTIONS, BN_6);
    			break;

    case	BN_7:	AutoTop.NoMouseMovement = 0x0001 & ~(AutoTop.NoMouseMovement);
    			if(AutoTop.NoMouseMovement)	F_Select(FOPTIONS, BN_7);
    			else				F_UnSelect(FOPTIONS, BN_7);
    			break;

    case	BN_8:	AutoTop.OnlyWorkArea = 0x0001 & ~(AutoTop.OnlyWorkArea);
    			if(AutoTop.OnlyWorkArea)	F_Select(FOPTIONS, BN_8);
    			else				F_UnSelect(FOPTIONS, BN_8);
    			break;
    case	BN_9:	GrowShrink = 0x0001 & ~(GrowShrink);
    			if(GrowShrink)			F_Select(FOPTIONS, BN_9);
    			else				F_UnSelect(FOPTIONS, BN_9);
    			break;
  
    case	TRAME_1:Trame &= 0xFFFFFF8FL;
    			break;
    case	TRAME_2:Trame = (0xFFFFFF8FL & Trame) | 0x00000010L;
    			break;
    case	TRAME_3:Trame = (0xFFFFFF8FL & Trame) | 0x00000020L;
    			break;
    case	TRAME_4:Trame = (0xFFFFFF8FL & Trame) | 0x00000030L;
    			break;
    case	TRAME_5:Trame = (0xFFFFFF8FL & Trame) | 0x00000040L;
    			break;
    case	TRAME_6:Trame = (0xFFFFFF8FL & Trame) | 0x00000050L;
    			break;
    case	TRAME_7:Trame = (0xFFFFFF8FL & Trame) | 0x00000060L;
    			break;
    case	TRAME_8:Trame = (0xFFFFFF8FL & Trame) | 0x00000070L;
    			break;
    			
    default:		break;
  }
}

/*
 * ----------------------------------------------------------------------------------------------
 *			Prise en compte des actions de la ToolBar
 * ----------------------------------------------------------------------------------------------
 */

void ToolBar(void)
/*
	Ouvre la fenetre Barre d'outils de l'application.
*/
{
  Type_Formular_Parameter	*Parameter;

  /*
   *	Si la toolbar est d‚j… active => La d‚sactiver 
   */
  if(ToolBarActive)											/* EnlŠve ou place un checkmark */
  {
    ToolBarActive = FALSE;										/* Compl‚mente SaveOnExit */
    M_UnChecked(MAINMENU, O_BARRE);
    F_Close(FTOOLBOX);
    return;
  }

  /*
   *	toolbar non active => L'activer 
   */
  Parameter = F_GetParameter();
  if(Parameter != NULL)
  {
    Parameter->TreeName = FTOOLBOX;									/* Indice de l'arbre de l'objet */
    Parameter->Title	= "ToolBox";									/* Titre de la fenetre */
    Parameter->Mode	= F_WIND;									/* Opening mode */
    Parameter->FirstEdit= NOTEXT;									/* First Editable Text */
    Parameter->List	= FormToolBox;									/* Event list */
    Parameter->FormIcon = IINFO;									/* Icone d'iconification */
    if((WHInfo = F_XOpen(Parameter)) != FALSE)
    {
      M_Checked(MAINMENU, O_BARRE);
      ToolBarActive = TRUE;										/* Compl‚mente SaveOnExit */
    }
    free(Parameter);
  }
}

void ToolBarAction(void)
{
  ToolBarActive = FALSE;										/* Compl‚mente SaveOnExit */
  M_UnChecked(MAINMENU, O_BARRE);
  F_Close(FTOOLBOX);
}

/*
 * ----------------------------------------------------------------------------------------------
 *			Prise en comptes des actions des Menus Popups.
 * ----------------------------------------------------------------------------------------------
 */

void PopupAction(void)
{
  if(F_PopupName == PCOULEUR)
  {
    Trame = (0xFFFFFFF0L & Trame) | (0x0000000FL & F_PopupEntry - 1);    
  }  

}

/*
 * ----------------------------------------------------------------------------------------------
 *			
 * ----------------------------------------------------------------------------------------------
 */

void SaveQuit(void)
/*
	S‚lectionne ou d‚selectionne la sauvegarde automatique du bureau … chaque fin du programme.
	Un checkmark est pla‡‚ dans le menu pour rappeler l'option coch‚e.
*/
{
  if(SaveOnExit)											/* EnlŠve ou place un checkmark */
    M_UnChecked(MAINMENU, O_SAVEQT);
  else
    M_Checked(MAINMENU, O_SAVEQT);
  SaveOnExit = 0x0001 & (~SaveOnExit);									/* Compl‚mente SaveOnExit */
}

void Fichier(void)
/*
	Appel‚ par l'item Fermer du menu Fichier. Celui provoque la disparition entre autre, de l'icone rep‚rsentant le fichier ouvert
	comme document.
*/
{
  OBJECT	*Arbre;
  char		Texte[256], Path[256]="\0", PathName[256]="\0", FileName[13]="\0", Extension[5]="*.*\0";
  
  switch(M_Entry)
  {
    case	F_NEW:		if(A_FileSelector(PathName, FileName, Extension, Path, "Nouveau fichier"))
    				{
    				  sprintf(Texte, "[0][ %s | %s | %s ][Ok]",Path, PathName, FileName);
    				  form_alert(1,Texte);
    				}
    				break;
    case	F_OPEN:		if(A_FileSelector(PathName, FileName, Extension, Path, "Ouvrir un fichier"))
    				{
    				  sprintf(Texte, "[0][ %s | %s | %s ][Ok]",Path, PathName, FileName);
    				  form_alert(1,Texte);
    				}
    				break;
    case	F_CLOSE:	rsrc_gaddr(R_TREE,BUREAU,&Arbre);					/* Cache l'icone repr‚sentant le fichier */
				Arbre[IFILE].ob_flags |= HIDETREE;
				form_dial(FMD_FINISH,0,0,0,0,Arbre[IFILE].ob_x,Arbre[IFILE].ob_y,Arbre[IFILE].ob_width,Arbre[IFILE].ob_height*2);
				break;
    case	F_SAVE:		break;
    case	F_SAVEAS:	if(A_FileSelector(PathName, FileName, Extension, Path, "Enregistrer sous..."))
    				{
    				  sprintf(Texte, "[0][ %s | %s | %s ][Ok]",Path, PathName, FileName);
    				  form_alert(1,Texte);
    				}
    				break;    
    default:			break;
  }
}


/*
 * ----------------------------------------------------------------------------------------------
 *		Corps du programme principal: Ensemble d'initialisations
 *		et v‚rifications sur la confirguration courante de la machine.
 * ----------------------------------------------------------------------------------------------
 */

void AppOpen(void)
{
  GetProfileInf(G_APP_INF);
}

void AppClose(void)
{
  SetProfileInf(G_APP_INF);
}

int main(void)
{
  if(!A_Open(G_FILE_RESSOURCE))
  {
    form_alert(1,"[1][ | Ressource not found ][ Ok ]");
    A_Close();
    return(G_RESSOURCE_ERROR);						
  }
  
  if(VdiInfo.HauteurStation < G_HEIGHT_SCREEN || VdiInfo.LargeurStation < G_WIDTH_SCREEN)		
  {
    form_alert(1,"[1][ Bad Screen Resolution | 640 x 400 needed ][ Ok ]");
    A_Close();
    return(G_SCREEN_SIZE_ERROR);
  } 

  if(VdiInfo.NombreCouleurs < G_COLOR )
  {
    form_alert(1,"[1][ Bad Colour Number | 16 colours needed ][ Ok ]");
    A_Close();
    return(G_COLOR_ERROR);  
  }

  W_Init();											/* Init Window  */
  M_Init();											/* Init Menu    */
  F_Init();											/* Init Form    */

  A_SetMouse(ARROW, NULL);									/* Application's mouse */
  W_SetIconApp(IAPPLI);										/* Set Icon Form Appli. */

  F_PopAttach(FOPTIONS,	ListPopAttach);								/* Attach Pop-ups */
  F_PopEvent(ListPopEvent);									/* Install Pop-events */
  F_SetPopup(NB_ITEMS);										/* number items displaying */
  
  /*
   *	Calcul l'adresse de l'arbre d'objets des MessageTextes en langue actulle.
   */
  rsrc_gaddr(R_TREE, MESSAGES, &MessageTexte);

  /*
   *		Ouverture des ‚l‚ments Menu, Bureau et fenetre
   */
  M_Open(MAINMENU,BarreMenu);									/* Create root menu */
  D_Open(BUREAU,Bureau);									/* It's my own Desktop */

  AppOpen();
  
  /*
   *		Event loop
   */
  Sortir = FALSE;
  do
  {
    A_WaitEvent();										/* Waiting an event	*/
    F_Event();											/* Form Event 		*/
    M_Event();											/* Menu Event		*/
    W_Event();											/* Window Event		*/
  }
  while(!Sortir);

  AppClose();
  
  D_Close();											/* Restore GEM Desktop	*/
  M_Close(MAINMENU);  										/* Close Root Menu	*/
  A_Close();											/* Close Application	*/

  return(G_NO_ERROR);										/* Rien de particulier */
}

