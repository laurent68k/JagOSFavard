/*
 * ----------------------------------------------------------------------------------------------
 *		XApplication C Module for Aads.
 *		Souces are freeware but all copyright by Funship 
 *
 *	Author  : FunShip
 *	File	: PCXApp.c
 *	Date    : 01 October 1991  (very first version, very old...)
 *	Revision: 7 December 1998
 *	Version : 0.1
 *	Release : 1.00
 *	Language: Pure C, Version du 3 Fevrier 1992 
 *	Origine : FRANCE
 *	
 * ----------------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <tos.h>
#include <string.h>

#include "..\Include\PCXApp.h"
#include "..\Include\PCCookie.h"				/* To calculate somes informations */
#include "..\Include\PCXMouse.h"				/* To set the application's mouse */

static	char	G_VERSION[10] = "1.2.63";
static	char	G_DATE[12]="22/12/1998";

#define	AES_FAILLED	-1			/* Critical error !! */
#define	VDI_FAILLED	0

#define	ERROR_AES	"[0][ | PCXAppl Critical Error Aes ][ok]"
#define	ERROR_VDI	"[0][ | PCXAppl Critical Error Vdi ][ok]"
#define ERROR_OTHER	"[0][ | PCXAppl Critical Error App ][ok]"

#define TOS_EXINPUT	0x102			/* don't Support fsel_exinput() */
#define TOS14				0x0104

#define	PATH_LEN			4096
#define	FILENAME_LEN		256

#define	TIMEHI				0			/* default time for evnt_timer() */
#define	TIMELO				150			/* 150 ms */

#define		BUBBLEGEM_ACK		0xBABC

/*
 * ----------------------------- Publics datas --------------------------
 */

Type_Aes_Info		*AesInfo;		/* AES's informations */
Type_Vdi_Info		VdiInfo;		/* VDI's informations */
Type_Mch_Info		MchInfo;		/* Machine's informations */
int					VdiHandle;		/* handle de la VWKS  */
int					AesHandle;		/* handle de l'AES    */

int 				A_Event,		/* Event's informations */
					A_Message[8],
					A_Key,
					A_Clic,
					A_MouseX,
					A_MouseY,
					A_MouseButton;

int	  				A_Mouse;		/* Mouse shape */
MFORM				*A_MouseForm;		/* User defined shape */

/*
 * ----------------------------- Privates datas -------------------------
 */

#define	TRUE		1
#define	FALSE		0

#define	HAUT		0x00							/* Mouse button Up */
#define	GAUCHE		1							/* Left Mouse button */
#define	DROIT		2							/* Right Mouse button */
#define	MAXCLICS	2							/* Double clics maximum surveill‚ */

						/* Pour initialiser la VDI */
static int 			WorkIn[]={1,1,1,1,1,1,1,1,1,1,2};

static int			A_TimerLo, A_TimerHi;			/* Time for timer event  */
static int			A_TimerOn;						/* Active ou no le Timer */
static void 		(*ProcedureUserTimer)(void);	/* User proc‚dure Timer  */

static long			RefDecompte;					/* Decompte p‚riode user plus grande */
static long			Decompte;

/*
 *
 */

void A_GetVersion( TAadsVersion *AadsVersion)
{
	AadsVersion->Version	=	G_VERSION;
	AadsVersion->Date		=	G_DATE;
}

/*
 *
 */
  
static void CopyCookies(Type_Mch_Info *MchInfo)
/*
	Cette proc‚dure copie les informations contenues dans le Cookie Jar, si
	il existe ou considŠre alors qu'il s'agit d'un simple ST dans ce cas.

	Entr‚es:	MchInfo pointeur sur une structure Type_Mch_Info
	Sorties:	Structure partiellement intialis‚e
*/
{
  Type_Cookie	cookie;

  /*
   *	Default value is for an English Atari ST
   */  
  MchInfo->Processor	= M68000;
  MchInfo->Machine	= ST;
  MchInfo->Switch	= FALSE;		/* !! */
  MchInfo->Fdc		= FALSE;		/* !! */
  MchInfo->Keyboard	= KEYB_GB;
  MchInfo->Language	= LG_GB;
  MchInfo->Shifter	= SHIFTER_ST;
  MchInfo->Sound	= PSG;
  MchInfo->Fpu		= NO_FPU;
  MchInfo->Separator	= '/';
  MchInfo->Time 	= TIME_12;
  MchInfo->Date		= DATE_MJA;
  /*
   *	Try to get the true value from the Cookie Jar System
   */
  if( J_Exist() )				/* TOS >= 1.6 */
  {
    if(J_Search("_CPU",&cookie))		/* Copy all cookies */
      MchInfo->Processor= (int)(cookie.information.mot_long & 0xFFFF);
    if(J_Search("_MCH",&cookie))
      MchInfo->Machine	= cookie.information.mot_long;
    if(J_Search("_SWI",&cookie))
      MchInfo->Switch	= (int)(cookie.information.mot_long & 0x00FF);
    if(J_Search("_FDC",&cookie))
      MchInfo->Fdc	= cookie.information.mot_long;
    if(J_Search("_AKP",&cookie))
    {
      MchInfo->Keyboard = (int)(cookie.information.mot_long & 0x00FF);
      MchInfo->Language = (int)((cookie.information.mot_long & 0xFF00) >> 8);
    }
    if(J_Search("_VDO",&cookie))
      MchInfo->Shifter	= cookie.information.mot_long;
    if(J_Search("_SND",&cookie))
      MchInfo->Sound	= (int)(cookie.information.mot_long & 0xFFFF);
    if(J_Search("_FPU",&cookie))
      MchInfo->Fpu 	= (int)(cookie.information.mot_long & 0xFFFF);
    if(J_Search("_IDT",&cookie))
    {
      MchInfo->Separator= (int) (cookie.information.mot_long & 0x00FF);
      MchInfo->Date 	= (int)((cookie.information.mot_long & 0x0F00) >> 8);
      MchInfo->Time 	= (int)((cookie.information.mot_long & 0xF000) >> 12);
    }
  }
}

static void GetAllVersion(Type_Mch_Info *MchInfo)
/*
	D‚termine la version de composantes logicielle du systŠme d'exploitation.
	
	Entr‚es:	MchInfo pointeur sur une structure Type_Mch_Info
	Sorties:	Structure partiellement intialis‚e
*/
{
  Type_SysEntete	Tos;
  Type_Cookie		cookie;
  
  A_HeaderSystem(&Tos);
  MchInfo->TosVersion	= Tos.version;
  MchInfo->GemdosVersion = Sversion();
  MchInfo->AesVersion	= AesInfo->ap_version; 
  if(J_Search("MiNT",&cookie))				/* Mot faible = version */
    MchInfo->MultiTosVersion = (int)(cookie.information.mot_long & 0xFFFF);
  else							/* MultiTos no found */
    MchInfo->MultiTosVersion = FALSE;
}

static void CriticalError(void)
/*
	Cette proc‚dure provoque une sortie violente du programme en cours.
	Si echec … l'initialisation de l'AES, la VDI ou autre, on retourne
	un code d'erreur au Shell et on retourne le control au systŠme.
	Entr‚es:	Rien
	Sorties:	Code de retour vers le Shell
*/
{
  int	error_code;
  
  if(AesHandle == AES_FAILLED)			/* Echec sur l'AES */
  {
    form_alert(1,ERROR_AES);
    error_code = ERROR_CODE_AES;
  }
  else if(VdiHandle == VDI_FAILLED)		/* Echec sur la VDI */
  {
    form_alert(1,ERROR_VDI);
    error_code = ERROR_CODE_VDI;
  }
  else						/* Autre ‚chec */
  {
    form_alert(1,ERROR_OTHER);
    error_code = ERROR_CODE_APP;
  }
  Pterm(error_code);				/* Quitter avec code de retour */
}

/*
 * ----------------------------- Publics procedures ---------------------
 */
 
int A_Open(char *ressource_file)
/*
	Primitive d'ouverture d'un programme en tant qu'application GEM.
	Initialise l'AES, la VDI, le timer GEM event, la structure MchInfo, 
	VdiInfo, AesInfo, et tente de charger un fichier ressource si demand‚ 
	et suivant la langue.
	
	Entr‚es:	pointeur sur le Chemin+Nom du fichier ressource
	Sorties:	True si r‚ussie sinon False.
*/
{
  AesHandle = appl_init();				/* Initialize AES */
  if(AesHandle == AES_FAILLED)				/* if failled */
    CriticalError();				
  v_opnvwk(WorkIn,&VdiHandle,(int *)&VdiInfo);		/* Initialize VWKS*/
  if(VdiHandle == VDI_FAILLED)				/* if failled */
    CriticalError();
  /*
   *	Initialize AES's information structure
   */
  AesInfo = (Type_Aes_Info *)&(_GemParBlk.global);

  /*
   *	Intialize Timer Countdown to 1 second and disabled it
   */  
  A_TimerOn	= FALSE;
  A_TimerLo	= TIMELO;
  A_TimerHi	= TIMEHI;
  RefDecompte	= 0;
  Decompte	= 0;

  /*
   *	Intialize the application's mouse default
   */
  A_Mouse	= ARROW;
  A_MouseForm	= NULL;
  
  /*
   *	Copy all standards cookies, Get all versions
   */
  CopyCookies(&MchInfo);
  GetAllVersion(&MchInfo);				/* Tos, Aes, Gemdos, ... */
  /*
   *	Try to load the ressource file with the active language
   */	
  if(ressource_file != NULL)				/* If ressource defined  */
  {
    switch(MchInfo.Language)				/* select for this language */
    {
      case LG_USA:	ressource_file[7] = 'G';
      			break;
      case LG_D:	ressource_file[7] = 'D';
      			break;
      case LG_F:	ressource_file[7] = 'F';
      			break;
      case LG_GB:	ressource_file[7] = 'G';
      			break;
      case LG_SP:	ressource_file[7] = 'S';
      			break;
      case LG_I:	ressource_file[7] = 'I';
      			break;
      case LG_CHD:	ressource_file[7] = 'D';
      			break;
      case LG_CHF:	ressource_file[7] = 'F';
      			break;
      			
    }
    if(!rsrc_load(ressource_file))				/* Load error 	    */
    {
      ressource_file[7] = 'G';					/* Try with English */
      if(rsrc_load(ressource_file))
        return(TRUE);
      ressource_file[7] = 'F';					/* Try with French  */
      if(rsrc_load(ressource_file))
        return(TRUE);
      ressource_file[7] = 'D';					/* Try with German  */
      if(rsrc_load(ressource_file))
        return(TRUE);
      return(FALSE);						/* Failled loading  */
    }
    else
      return(TRUE);
  }
  return(TRUE);
}

void A_Close(void)
/*
	Termine un programme en tant qu'application GEM.LibŠre l'AES et la VDI.
	
	Entr‚es:	Rien
	Sorties:	Rien
*/
{
  rsrc_free();							/* Free Rsc memory */
  v_clsvwk(VdiHandle);						/* Close VWKS */
  if(!appl_exit())						/* Close AES */
    CriticalError();
}

void A_HeaderSystem(Type_SysEntete *type)
/* 
	Return the informations from Tos's system header.
	
	Input:	pointer on a specific structure
	Output:	the structure contains these system's informations
*/  

{
  long  *sysbase;				/* Header's pointer */
  long  pile;					/* For store SP */
  Type_SysEntete *info;
  
  pile=Super(0L);				/* Turn into Super  */

  sysbase=(long *)0x4F2L;			/* Address of _sysbase   */
  info=(Type_SysEntete *)*sysbase;		/* Read pointer 	 */
  
  type->version		= info->version;	/* Copy all informations */
  type->reseth		= info->reseth;
  type->os_beg		= info->os_beg;
  type->os_end		= info->os_end;
  type->os_rsvl		= info->os_rsvl;
  type->os_magic	= info->os_magic;
  type->os_date		= info->os_date;
  type->os_conf		= info->os_conf;
  type->os_dosdate	= info->os_dosdate;
  type->p_root		= info->p_root;
  type->pkbshift	= info->pkbshift;
  type->p_run		= info->p_run;
  type->p_rsv2		= info->p_rsv2;
  Super((void *)pile);				/* Turn into User mode */
}

/*
 * ----------------------------------------------------------------------------------------------
 *				S‚lecteur de fichier pour parcourir
 * ----------------------------------------------------------------------------------------------
 */

int A_FselInput(char *Prefix, char *Dummy, int *Bouton, char *Title)
/*
	Appel le s‚lecteur de fichiers ‚tendue (avec un titre) si le TOS actuel le permet.
	(TOS >= 1.4)
*/
{
	int	retour;

	if(MchInfo.TosVersion < TOS14)
		retour	=	fsel_input(Prefix,Dummy,Bouton);
	else
		retour	=	fsel_exinput(Prefix,Dummy,Bouton, Title); 
	return retour;
}

void A_GetPath(char *Chemin,char *Label)
/*	
	Appel le s‚lecteur de fichiers pour designer un chemin complet r‚pertoire.
	Si un nouveau est valid‚, Chemin prend cette valeur, sinon Chemin conserve
	son ancienne valeur.
*/
{
	char	Prefix[PATH_LEN], Dummy[14], *Prochain;
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
	
	A_FselInput(Prefix,Dummy,&Bouton,Label);
	if(Bouton)
	{
		Prochain = strrchr(Prefix,'\\');
		*(Prochain) = '\0';										/* Supprimer le \*.* … la fin */
		
		strcpy(Chemin,Prefix);									/* Recopier le nouveau chemin */
	}
}

int A_FileSelector(char *pathname,char *nom_fichier,char *suffix,
		  char *prefix,char *label)
/*
	Standart File Selector GEM management procedure.
	Use the classic Selector or Extented selector if it possible.

	Input:	'pathname'	= Path, Name and extension of file.
		'nom_fichier'	= Name and extension of file.
		'suffix'	= Extension of files to select(ex: "*.doc")->
		'prefix'	= Initial access path (could be changed) or
	  		  	empty string
		'label'		= Title of formular, only with TOS > 1.2
	Output:	the four parameter could be changed.
*/
{
  char	separator[2]="\\";
  char	name[FILENAME_LEN]="\0";
  char	*cpt;
  int	erreur,bouton;
  
  if(strlen(prefix)==0)			/* Si pas de chemin donne */
  {
    cpt = prefix;			/* Calculer le disque courant */
    *cpt++ = Dgetdrv()+'A';		/* et le chemin par defaut */
    *cpt++ = ':';
    Dgetpath(cpt,0); 
  }

  strcpy(pathname,prefix);		/* ajoute le mask de recherche */
  strcat(pathname,separator);
  strcat(pathname,suffix);

  erreur	=	A_FselInput(pathname, name, &bouton, label);
  
  if(erreur!=0 && name[0]!=0 && pathname[0]!=0 && bouton!=0)
  {

    if( strchr(name, '*') != NULL)	/* Si le nom contient l'etoile, pas de fichier */
		return FALSE;

    strcpy(nom_fichier,name);		/* recopier le Nom choisi */
    cpt = strrchr(pathname,'\\'); 
    *cpt=0;
    strcpy(prefix,pathname);		/* nouveau chemin d'acces complet */
    *cpt++='\\';
    strcpy(suffix,cpt);			/* nouveau suffix eventuel */
    strcpy(cpt,name);
    name[0]=0;

    cpt = strrchr(pathname,'\\');             
    return(TRUE);
  }
  else
   return(FALSE);
}

/*
 * ----------------------------------------------------------------------------------------------
 */


int A_GCellHeigh(void)
/*
	Retourne la hauteur de la cellule des caracteres
  
	Entr‚es:	Rien
	Sorties:	Rien
*/
{
  int	attributs[10];
  
  vqt_attributes(VdiHandle,attributs);
  return(attributs[9]);
}


int A_GCellWidth(void)
/*
	Retourne la largeur de la cellule des caractŠre
	
	Entr‚es:	Rien
	Sorties:	Rien
*/
{
  int	attributs[10];
  
  vqt_attributes(VdiHandle,attributs);
  return(attributs[8]);
}

 
int A_GFontHeigh(void)
/*
	Retourne la hauteur de la fonte de caractŠre actuelle

	Entr‚es:	Rien
	Sorties:	Rien
*/
{
  int	attributs[10];
  
  vqt_attributes(VdiHandle,attributs);
  return(attributs[7]);
}


int A_GFontWidth(void)
/*
	Retourne la largeur de la fonte de caractŠre actuelle.
	
	Entr‚es:	Rien
	Sorties:	Rien
*/
{
  int	attributs[10];
  
  vqt_attributes(VdiHandle,attributs);
  return(attributs[6]);
}

void A_WaitEvent(void)
/*
	Process Gem's Messages, Mouse's button and Keyboard event.
	
	Entree:	A_xx Event globals variables
	Sortie:	A_xx Event globals variables changed
*/
{
	int Dummy,Bouton,State,NbClics;
	char	*Pointer;
	  
	NbClics = 0x100 | MAXCLICS;						/*	Atendre les deux boutons gauche et droit */
	Bouton  = GAUCHE | DROIT;
	State   = HAUT;

	A_Event=evnt_multi(	MU_MESAG | MU_KEYBD | MU_BUTTON | MU_TIMER,
						NbClics,Bouton,State,0,0,0,0,0,0,0,0,0,0,
                      	A_Message,
                      	A_TimerLo,A_TimerHi,
                      	&A_MouseX,&A_MouseY,&A_MouseButton,&Dummy,
                      	&A_Key,&A_Clic);

	if((A_Event & MU_TIMER) && A_TimerOn)		/* If an user timer code is defined */
	{
		Decompte--;
		if(Decompte <= 0)
    	{
			(*ProcedureUserTimer)();					/* Call it if it's allowed */
			Decompte = RefDecompte;
		}
	}
  
	/*	Check if it is a BubbleGEM message reply	*/
	if( (A_Event & MU_MESAG) && (A_Message[0] == (int)BUBBLEGEM_ACK) )
	{
		Pointer	=	*(char **)&A_Message[5];						
		free(Pointer);
	}
}

/*
 *	Primitives de gestion de l'ŠvŠnement Timer GEM.
 */

void A_CycleTimer(int TimeHi,int TimeLo)
/*
	Initialise le temps de d‚comptage de l'‚vŠnement Timer;
	Un temps nul est interdit
	
	Entr‚es:	Valeur MSB et LSB du temps
	Sorties:	Rien
*/
{
  if(!(TimeLo == 0 && TimeHi == 0))			/* Temps Nul interdit */
  {
    if(((TimeHi << 16) | TimeLo) < ((A_TimerHi << 16) | A_TimerLo))	/* Demande une p‚riode plus petite */
    {
      A_TimerLo = TimeLo;
      A_TimerHi = TimeHi;
      RefDecompte = 0;;
      Decompte = RefDecompte;
    }
    else						/* Demande une p‚riode plus grande */
    {
      /*
       *	calcul le nombre de p‚riode de notre horloge avant d'appeler le code user
       *	d‚fini sur une p‚riode 'Decompte' fois plus grande.
       */
      RefDecompte = ((TimeHi << 16) | TimeLo) / ((A_TimerHi << 16) | A_TimerLo);
      Decompte = RefDecompte;
    }
  }
  else							/* Valeur de temps d‚faut */
  {
    A_TimerLo = TIMELO;
    A_TimerHi = TIMEHI;
    RefDecompte = 0;
    Decompte = RefDecompte;
  }
}

void A_OnTimer(void)
/*
	Active le Timer. La proc‚dure utilisateur sera d‚sormais appel‚e.

	Entr‚es:	Rien
	Sorties:	Rien
*/
{
  A_TimerOn = TRUE;
}

void A_OffTimer(void)
/*
	Inhibe le Timer. La proc‚dure utilisateur ne sera plus appel‚e.

	Entr‚es:	Rien
	Sorties:	Rien
*/
{
  A_TimerOn = FALSE;
}

void A_SetCodeTimer(void (*UserTimer)(void))
/*
	Cette primitive installe la proc‚dure appel‚ … chaque ‚vŠnement Timer.
	
	Entr‚es:	Adresse de la proc‚dure
	Sorties:	Rien
*/
{
  ProcedureUserTimer	= UserTimer;
}

int A_SetMouse(int Mouse,MFORM *MouseForm)
/*
	This function allows you set the application's mouse with a standard shape or an
	user shape. 
	It work the same way as the graf_mouse call system and return the same codes when
	an error occurs or not.
	(Date: 12/12/95)
*/
{
  int	MouseX,MouseY,Dummy;
  
  
  A_Mouse 	= Mouse;						/* Saved into Global variables */
  A_MouseForm	= MouseForm;						/* Saved into Global variables */
  graf_mkstate(&MouseX,&MouseY,&Dummy,&Dummy);
  if(wind_find(MouseX,MouseY) == 0)
    return(MS_Mouse(A_Mouse,A_MouseForm));				/* AES call to set it */
  else
    return(0);
}