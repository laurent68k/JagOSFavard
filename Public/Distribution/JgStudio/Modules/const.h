/*
 * ----------------------------------------------------------------------------------------------
 *					APPLICATION Jaguar Studio Developpement Kit, sous A.A.D.S pour Atari.
 *
 * 	Author  : 	FAVARD Laurent, FunShip (c)
 *	File    : 	JaquarHC
 *	Date    : 	10 December 1996
 *	Release : 	18 August 1998
 *	Version : 	1.0
 *	Country : 	FRANCE
 *			
 *
 *								- Copyright Atari FunShip (c) 1994-97 -
 *							- Atari ST, STE, TT, Falcon, C-LAB, Medusa, Hades -
 * ----------------------------------------------------------------------------------------------
 */

#ifndef		__Const
#define		__Const		

#include		<JToolBox.h>
#include		"..\modules\platform.h"

/*
 * ----------------------------------------------------------------------------------------------
 *			D‚finitions standards du template d'application.
 */

/*	Carract‚ristiques globales de cette application */

#ifdef	__MAGIC_OS__
#define		G_APP_NAME				"JagStudio for MagiC"
#endif

#ifdef	__NAES_OS__
#define		G_APP_NAME				"JagStudio for N.AES"
#endif

#ifdef	__TOS_OS__
#define		G_APP_NAME				"JagStudio for TOS/MiNT"
#endif

#define		G_APP_NUMBER			"Version : 2.0.0"
#define		G_APP_DATE				"Dec 10 1996/"__DATE__	
#define		G_APP_INF				"Jaguar.Inf"

#define		G_EMAIL_LAURENT			"laurent.favard@libertysurf.fr"
#define		G_HTTP					"http://perso.libertysurf.fr/lfavard"

#define		G_FILE_RESSOURCE		"Jaguar_?.Rsc"
#define		G_DEFAULT_HELPFILE		"*:\\Jaguar.Hyp"
#define		G_DEFAULT_HELPOS98		"*:\\JagOS.Hyp"
#define		G_DEFAULT_TOOLBOX98		"*:\\JToolbox.Hyp"

#define		EXT_TEMPLATE			"*.Pro"
#define		EXT_CRY					"*.Cry"
#define		EXT_JAG					"*.Jag"
#define		EXT_JAG2				".Jag"
#define		EXT_SRC					"*.s"
#define		EXT_SRC2				".s"
#define		EXT_PRG					"*.Prg"

#define		G_NO_ERROR				0
#define		G_RESSOURCE_ERROR		-1
#define		G_SCREEN_SIZE_ERROR		-2
#define		G_COLOR_ERROR			-3

/*	Type d'‚cran necessaire */

#define		G_WIDTH_SCREEN			639
#define		G_HEIGHT_SCREEN			399

#define		MX_STRAM				0
#define		MX_TTRAM				1

#define		NB_ITEMS				10
#define		AES_ICON_COULEUR		0x340							/*	AES minimum supportant les icones couleurs */
#define		GEMDOS_MXALLOC			0x1900							/*	Gemdos supportant la fonction Mxalloc() */

#define		MGC_FILEICONLEN			20								/*	Length texte icon file */
#define		TOS_FILEICONLEN			8								/*	Length texte icon file */

#ifdef	__MAGIC_OS__	
#define		FILE_ICON_LEN		MGC_FILEICONLEN
#else
#define		FILE_ICON_LEN		TOS_FILEICONLEN
#endif

/*
 * ----------------------------------------------------------------------------------------------
 */

/*	Constantes pour l'‚mission */

#define		BASE10					10
#define		BASE16					16

/*
 * ----------------------------------------------------------------------------------------------
 */
 
/*	D‚finitions pour les chapitres du .INF */

#define		APPLICATION				"Application"
#define		BUREAU					"Desktop"
#define		PORTCOM					"PortCom"
#define		ECRAN					"Screen"
#define		FONTES					"Fontes"
#define		PROJET					"Projet"
#define		EXTERNAL				"Externals"
#define		INTERNET				"Internet"

#define		BULLE					"Bubbles"
#define		BULLEJAGBAR				"BubblesJagBar"
#define		HELPFILE				"HelpFile"
#define		HELPOS98				"HelpJagOS98"
#define		HELPTB98				"HelpJToolBox98"

#define		EXE_NAME				"JagName"
#define		EXE_PATHNAME			"JagPathName"
#define		EXE_PATH				"JagPath"
#define		EXE_EXTENSION			"JagExtension"

#define		SRC_NAME				"SrcName"
#define		SRC_PATHNAME			"SrcPathName"
#define		SRC_PATH				"SrcPath"
#define		SRC_EXTENSION			"SrcExtension"

#define		ADR_EMAIL				"Email"
#define		ADR_HTTP				"Http"

/*
 * ----------------------------------------------------------------------------------------------
 */

#define		G_WIND_MEMPARAM			"Window_MemParam"
#define		G_WIND_SENDER			"Window_Sender"
#define		G_WIND_CONSOLE			"Window_Terminal"
#define		G_WIND_TOOLBAR			"Window_ToolBar"
#define		G_WIND_OPTIONS			"Window_Options"
#define		G_WIND_ABOUT			"Window_About"
#define		G_WIND_TRANSFER			"Window_Transfert"
#define		G_WIND_EEPROM			"Window_Eeprom"
#define		G_WIND_CPU				"Window_Cpu"
#define		G_WIND_INQUIRE			"Window_Inquire"
#define		G_WIND_SMILEY			"Window_Smiley"
#define		G_WIND_INFOFILE			"Window_Informations"
#define		G_WIND_NEWFORM			"Window_NewForm"
#define		G_WIND_GENFORM			"Window_GenForm"
#define		G_WIND_ENVR				"Window_Envr"
#define		G_WIND_GENFRMP			"Window_GenFrmP"
#define		G_WIND_WERR				"Window_Error"
#define		G_WIND_WSTATUS			"Window_Status"

/*	Constantes pour la fenetre console */

#define		MAX_LINE_CONSOLE		1000
#define		SIZE_LINE_CONSOLE		128

/*	Section ST-GUIDE */

#define		SECTION_HELP_MENUBAR	"Chapter 2"					/*	Help menu */
#define		SECTION_HELP_SENDER		"Section 2.2.2"				/*	Load file (binaries) */
#define		SECTION_HELP_MEMPARAM1	"Section 2.2.3"				/*	RUN prg */
#define		SECTION_HELP_MEMPARAM2	"Section 2.2.5"				/*	Download binairies */
#define		SECTION_HELP_EEPROM		"Section 2.3.1"				/*	Serial EEPROM */
#define		SECTION_HELP_OPTIONS	"Section 2.4.1"				/*	Parameters window */
#define		SECTION_HELP_TRANSFER1	"Section 3.2"				/*	Jaguar Transfer to Atari */
#define		SECTION_HELP_TRANSFER2	"Section 3.2"				/*	Jaguar Transfer from Atari */
#define		SECTION_HELP_TERMINAL	"Chapter 4"					/*	Console window */
#define		SECTION_HELP_NEWFORM	"Section 5.1"				/*	New file */
#define		SECTION_HELP_GENFORM	"Section 5.2"				/*	Source code generator */
#define		SECTION_HELP_ENVR		""

/*	Window sender parameter */

typedef	struct	{	
					int					CodeOp;
					long		 		Header;
					void				*StartAdr;
					long				Length;
					void				*StartCode;
					unsigned long		OffsetStart;
					char				Name[PROGRAM_NAME_LEN];
					int					Mode;
				} TYPE_SENDER_PARAM;

/*	Window transfert parameter */

typedef	struct	{	
					void				*StartAdr;
					unsigned long		ExpectedLen;
					int					Mode;
					long					SourcePID;
					long					TargetPID;
				} TYPE_TRANSFERT_PARAM;

#define		DEFAULT_TARGET_ADR			(void *)0x5000L
#define		DEFAULT_OFFSET				0x1CL						/*	Default GEMDOS header to pass	*/
#define		DEFAULT_START_ADR			(void *)0x5000L
#define		DEFAULT_LEN					0x80L

/*
 * ----------------------------------------------------------------------------------------------
 *	Constants for filenames
 */

#define		PATH_LEN					2048
#define		FILENAME_LEN				256
#define		EXT_LEN						6

/*
 * ----------------------------------------------------------------------------------------------
 */
 
typedef	struct	{
					/*	Nom du fichier source principal */
					char	SrcExtension[EXT_LEN];
					char	SrcPath[PATH_LEN];						/* Chemin seulement */
					char	SrcPathName[PATH_LEN+FILENAME_LEN];		/* Chemin + Nom de fichier */
					char	SrcFileName[FILENAME_LEN]; 				/* Nom de fichier seulement */

					/*	Nom du fichier executable */
					char	ExeExtension[EXT_LEN];
					char	ExePath[PATH_LEN];						/* Chemin seulement */
					char	ExePathName[PATH_LEN+FILENAME_LEN];		/* Chemin + Nom de fichier */
					char	ExeFileName[FILENAME_LEN]; 				/* Nom de fichier seulement */

				} TProjet;

typedef	struct	{
					/*	Editeur de texte */
					char	EditPath[PATH_LEN];						/* Chemin seulement */
					char	EditPathName[PATH_LEN+FILENAME_LEN];		/* Chemin + Nom de fichier */
					char	EditFileName[FILENAME_LEN]; 				/* Nom de fichier seulement */

					/*	Assembleur 680x0 */
					char	AsmPath[PATH_LEN];						/* Chemin seulement */
					char	AsmPathName[PATH_LEN+FILENAME_LEN];		/* Chemin + Nom de fichier */
					char	AsmFileName[FILENAME_LEN]; 				/* Nom de fichier seulement */

				} TExternal;

/*
 * ----------------------------------------------------------------------------------------------
 */

#define		AVSERVER			0

#define		VA_START 		0x4711
#define		AV_EXIT 		0x4736
#define		AV_STARTED 		0x4738
#define		AV_STARTPROG	0x4722
#define		VA_PROGSTART	0x4723

/*
 * ----------------------------------------------------------------------------------------------
 *	Port list definitions
 */

#define		P_SERIAL		"serial"
#define		P_BOARD			"cartouche"
#define		P_CENTR			"centronics"

#endif
