/*
 * ----------------------------------------------------------------------------------------------
 *							D‚claration des listes d'‚v‚nements AADS.
 *
 * 	Author  : 	FAVARD Laurent, FunShip (c)
 *	File    : 	Object.H
 *	Date    : 	10 December 1996
 *	Release : 	11 April 1999
 *	Version : 	1.0
 *	Country : 	FRANCE
 *			
 *
 *								- Copyright Atari FunShip (c) 1994-96 -
 *							- Atari ST, STE, TT, Falcon, C-LAB, Medusa, Hades -
 * ----------------------------------------------------------------------------------------------
 */

#ifndef		__LIST_OBJECT__
#define		__LIST_OBJECT__		__LIST_OBJECT__


/*
 * ----------------------------------------------------------------------------------------------
 *		Description de la barre de menu principale de l'application
 * ----------------------------------------------------------------------------------------------
 */
 
void	AboutMe(void);
void	Ouvrir(void);
void	Fichier(void);  
void	Connect(void);  
void	Outils(void);
void	Aide(void);
void	Quitter(void);
void	ToolBar();
void	Options();
void	SaveQuit(void);
void	SaveOptions(void);
void 	OpenConsole(void);
void 	OpenStatus(void);
void	OpenSmiley(void);
void	Envr( void );
void	OpenDebug( void );

Type_Parameter_Menu
BarreMenu[]={	
				{APROPOS,	INITIAL,	AboutMe},						/* A propos de ... */

				/*	Menu fichier */
				{F_NEW,		INITIAL,	Fichier},						/* */
				{F_OPEN,	INITIAL,	Fichier},						/* */
				{F_OPENE,	INITIAL,	Fichier},						/* */
				{F_INFO,	INITIAL,	Fichier},						/* */
				{F_CLOSE,	INITIAL,	Fichier},						/* Fermer un fichier */
				{F_CLOSA,	INITIAL,	Fichier},						/* Fermer un fichier */
			  	{F_QUIT,	INITIAL,	Fichier},						/* Quitter l'application */

			  	/*	Menu Jaguar */
			  	{L_SERIE,	INITIAL,	Connect},						/* Use serial port */
			  	{L_CENTR,	INITIAL,	Connect},						/* use centronics port */
			  	{L_LOAD,	INITIAL,	Connect},						/* LOAD BINARIES vers JagOS */
			  	{L_LDRUN,	INITIAL,	Connect},						/* LOAD BINARIES and RUN dans JagOS */
			  	{L_RUN,		INITIAL,	Connect},						/* RUN dans JagOS */
			  	{L_PING,	INITIAL,	Connect},						/* INQUIRE vers JagOS */
			  	{L_GET,		INITIAL,	Connect},						/* INQUIRE vers JagOS */
			  	{L_SECUR,	INITIAL,	Connect},						/* INQUIRE vers JagOS */

			  	/*	Menu Outils */
			  	{T_EPROM,	INITIAL,	Outils},						/* Read EEPROM */
			  	{T_CRY,		INITIAL,	Outils},						/* Read EEPROM */

				/*	Menu Option */
				{O_OPT,		INITIAL,	Options},						/* Fenetre de param‚trage d'affichage */
				{O_ENVR,	INITIAL,	Envr},							/* Fenetre de param‚trage des externes */
				{O_BARRE,	INITIAL,	ToolBar},						/* Ouvrir fenetre barre d'outils ToolBox */
				{O_CONSOL,	INITIAL,	OpenConsole},					/* Ouvrir fenetre console */
				{O_STATUS,	INITIAL,	OpenStatus},					/* Ouvrir fenetre status */
			  	{O_SAVEQT,	INITIAL,	SaveQuit},						/* Sauver le bureau en quittant l'application */
				{O_SAVEDK,	INITIAL,	SaveOptions},					/* sauver le bureau et infos graphiques */
				{O_SMILEY,	INITIAL,	OpenSmiley},					/* sauver le bureau et infos graphiques */

				/*	Menu AIDE */
				{MENUAIDE,	INITIAL,	Aide},	
				{AIDE,		INITIAL,	Aide},	
				{AJAGOS,	INITIAL,	Aide},	
				{AJAGTB,	INITIAL,	Aide},	
				{BAIDE,		INITIAL,	Aide},	
				{BONJB,		INITIAL,	Aide},	

				/*	Menu Debug */  	
				{D_DEBUG,	INITIAL,	OpenDebug},	
			  	MENU_END
	   		};

/*
 * ----------------------------------------------------------------------------------------------
 *		Description du bureau de l'application (Icones, op‚rations possibles, ...)
 * ----------------------------------------------------------------------------------------------
 */

void	ClicLeft(void);
void	ClicRight(void);
void	DClicLeft(void);
void	Move(int typemove,int target);

Type_Bureau_List
Bureau[]=  {	
				{ROOT	,0  ,0,NULL				,NULL,		NULL,		ClicRight, ClicRight, NULL, NULL},
				{IDISK	,25 ,0,"My Atari"		,ClicLeft,	DClicLeft,	ClicRight, ClicRight, Move,	Move},
				{IPRG	,25 ,50,""				,ClicLeft,	DClicLeft,	ClicRight, ClicRight, Move,	NULL},
				{IJAG	,25	,100,"Jaguar"		,ClicLeft,	DClicLeft,	ClicRight, ClicRight, Move,	Move},
				{ITRASH	,25	,150,"Trash"		,ClicLeft,	DClicLeft,	ClicRight, ClicRight, Move,	Move},
				{ISRC	,100,0,""				,ClicLeft,	DClicLeft,	ClicRight, ClicRight, Move,	Move},
				{IEDIT	,100,50,"Editor"		,ClicLeft,	DClicLeft,	ClicRight, ClicRight, Move,	Move},
				{IASM	,100,150,"Assemble"		,ClicLeft,	DClicLeft,	ClicRight, ClicRight, Move,	Move},
				BUREAU_END
		   };

/*
 * ----------------------------------------------------------------------------------------------
 *		Description du menu popup du bureau de l'application pour tous ses objets
 * ----------------------------------------------------------------------------------------------
 */

static Type_Parameter_PopAttach											/* Popup menu */
ListPopAttach[]={
		  			{POPUP1, IPRG	,ITEM10	,SCROLL_LISTBOX, FALSE},
		  			{POPUP1, IDISK	,ITEM10	,SCROLL_LISTBOX, FALSE},
		  			{POPUP1, ISRC	,ITEM10	,SCROLL_LISTBOX, FALSE},
		  			{POPUP2, IJAG	,ITEM20	,SCROLL_LISTBOX, FALSE},
		  			{POPUP3, ROOT	,ITEM30	,SCROLL_LISTBOX, FALSE},
					POPATTACH_END
	        	};

void Popup1(void);
void Popup2(void);
void Popup3(void);

Type_Parameter_PopEvent												/* Code for each item */
ListPopEvent[]=	{	
		  			{POPUP1,ITEM10 , Popup1},						/*	Open executable item */
		  			{POPUP1,ITEM11 , Popup1},						/*	Close item */
		  			{POPUP1,ITEM12 , Popup1},						/*	info item */
		  			{POPUP1,ITEM13 , Popup1},						/*	load item */
		  			{POPUP1,ITEM14 , Popup1},						/*	load and run item */
		  			{POPUP1,ITEM15 , Popup1},						/*	new */
		  			{POPUP1,ITEM16 , Popup1},						/*	open */

		  			{POPUP2,ITEM20 , Popup2},						/*	run item */
		  			{POPUP2,ITEM21 , Popup2},						/*	ping item */
		  			{POPUP2,ITEM22 , Popup2},						/*	eeprom item */
		  			{POPUP2,ITEM23 , Popup2},						/*	get at item */

		  			{POPUP3,ITEM30 , Popup3},						/*	fuji item */
		  			{POPUP3,ITEM31 , Popup3},						/*	save desktop item */
		  			{POPUP3,ITEM32 , Popup3},						/*	properties item */
		  			{POPUP3,ITEM33 , Popup3},						/*	properties item */
		  			{POPUP3,ITEM34 , Popup3},						/*	properties item */
  		  			POPEVENT_END  			
  				};  				   


#endif
