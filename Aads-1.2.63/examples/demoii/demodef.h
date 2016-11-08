/*
 * ---------------------------------------------------------------------------------------
 *				Main Menu Event List
 */

void GoMainMenu(void);

Type_Parameter_Menu
MainMenu[]=	{	
			{ABOUT		,INITIAL ,GoMainMenu},		/* A propos... */
		  	{QUITTER	,INITIAL ,GoMainMenu},		
		  	{OPENWDB	,INITIAL ,GoMainMenu},			
		  	{OPENWMDB	,DISABLED,GoMainMenu},	
		  	{OPENW		,INITIAL ,GoMainMenu},			
		  	{OPENWM		,INITIAL ,GoMainMenu},			
		  	{FLIP		,DISABLED,GoMainMenu},
		  	{MSAUTO		,INITIAL ,GoMainMenu},
		  	{MSWORK		,INITIAL ,GoMainMenu},
		  	{MSNOMOVE	,INITIAL ,GoMainMenu},
			{GROWSHRINK	,INITIAL ,GoMainMenu},		  	
	  		MENU_END
	   	};

/*
 * ---------------------------------------------------------------------------------------
 *				Dialog Box Event List
 */

void GoMain(void);

static Type_Form_List							/* Main Form Setup */
FormMain[]=	{	
			{CLOSE		,GoMain,NULL},
			{R4		,GoMain,NULL},
			{R5		,GoMain,NULL},
			{R6		,GoMain,NULL},
			{R7		,GoMain,NULL},
			{R8		,GoMain,NULL},
			{BTN1		,GoMain,NULL},
			{BTN2		,GoMain,NULL},
			{BTN3		,GoMain,NULL},
			FORM_END
		};

void GoInfo(void);

static Type_Form_List							/* Info Form */
FormInfo[]=	{	
			{OK		,GoInfo,NULL},
			{MORE		,GoInfo,NULL},
			FORM_END
		};

static Type_Form_List							/* Info Form */
FormMore[]=	{	
			{0		,NULL,NULL},
			FORM_END
		};

static Type_Form_List							/* System Form */
FormSys[]=	{	
			{0		,NULL,NULL},
			FORM_END
		};

/*
 * ---------------------------------------------------------------------------------------
 *				Description Popups
 */

static Type_Parameter_PopAttach					/* Popup menu */
ListPopAttach[]={
		  {PCLAVIER,KEYBOARD  ,KBD	,KBCHF		,TRUE},
		  {PMACHINE,MACHINE   ,M8       ,SCROLL_LISTBOX	,TRUE},		  
		  POPATTACH_END
	        };

void GoPopup(void);

Type_Parameter_PopEvent						/* Popups Event list */
ListPopEvent[]=	{	
		  {PCLAVIER,KBFR  ,GoPopup},
		  {PCLAVIER,KBGB  ,GoPopup},
		  {PCLAVIER,KBD   ,GoPopup},
		  {PCLAVIER,KBSP  ,GoPopup},
		  {PCLAVIER,KBI   ,GoPopup},
		  {PCLAVIER,KBUSA ,GoPopup},
		  {PCLAVIER,KBCHF ,GoPopup},
		  {PCLAVIER,KBCHD ,GoPopup},
		  {PCLAVIER,KBRUS ,GoPopup},
		  {PCLAVIER,KBUK  ,GoPopup},
		  {PCLAVIER,KBQB  ,GoPopup},
		  {PCLAVIER,KBB   ,GoPopup},
		  {PCLAVIER,KBDK  ,GoPopup},
		  {PCLAVIER,KBOS  ,GoPopup},
		  {PCLAVIER,KBCN  ,GoPopup},
		  {PCLAVIER,KBHOL ,GoPopup},
		  		  
		  {PMACHINE,M1    ,GoPopup},
		  {PMACHINE,M2    ,GoPopup},
		  {PMACHINE,M3    ,GoPopup},
		  {PMACHINE,M4    ,GoPopup},
		  {PMACHINE,M5    ,GoPopup},
		  {PMACHINE,M6    ,GoPopup},
		  {PMACHINE,M7    ,GoPopup},
		  {PMACHINE,M8    ,GoPopup},
		  {PMACHINE,M9    ,GoPopup},
		  {PMACHINE,M10   ,GoPopup},
		  {PMACHINE,M11   ,GoPopup},
		  {PMACHINE,M12   ,GoPopup},
		  {PMACHINE,M13   ,GoPopup},
		  {PMACHINE,M14   ,GoPopup},
		  {PMACHINE,M15   ,GoPopup},
		  {PMACHINE,M16   ,GoPopup},
		  {PMACHINE,M17   ,GoPopup},
		  
  		  POPEVENT_END  			
  		};  				   

/*
 * ---------------------------------------------------------------------------------------
 *				Desktop Event list
 */

void IconClic(void);
void DbClic(void);
void IconMove(int Move,int Target);

Type_Bureau_List
Bureau[]=  {	
		{IFLOPPY	,25  ,0,"Disk"		,IconClic,DbClic,NULL,NULL,IconMove,IconMove},
		{IMUSIC		,25 ,50,"Sound"		,IconClic,DbClic,NULL,NULL,IconMove,IconMove},
		{ICLIPBRD	,25,100,"ClipBoard"	,IconClic,DbClic,NULL,NULL,IconMove,IconMove},
		{IECRAN		,25,150,"Screen"	,IconClic,DbClic,NULL,NULL,IconMove,IconMove},
		{ITELEPHONE	,25,200,"Phone"		,IconClic,DbClic,NULL,NULL,IconMove,IconMove},
		{IATARI		,25,250,"Falcon"	,IconClic,DbClic,NULL,NULL,IconMove,IconMove},
		{ITRASH		,25,300,"Trash"		,IconClic,DbClic,NULL,NULL,IconMove,IconMove},
		BUREAU_END
	   };

/*
 * ---------------------------------------------------------------------------------------
 *				Window built-in menu description
 */

void Operation(int WindowHandle);

static Type_Window_Menu_List
WMenu1[]=	{	{APROPOS,INITIAL,Operation},
			{QUITTE ,INITIAL,Operation},
			{ATARI	,INITIAL,Operation},
			{AMIGA	,INITIAL,Operation},
			{APPLE	,INITIAL,Operation},
			{NEXT	,INITIAL,Operation},
			{UNIX	,INITIAL,Operation},
			{CLOSEW1,INITIAL,Operation},
			{CLOSEM1,INITIAL,Operation},
			WMENU_END
		};

static Type_Window_Menu_List
WMenu2[]=	{	{COMPILE,INITIAL,Operation},
			{CLOSEW	,INITIAL,Operation},
			{CLOSEM	,INITIAL,Operation},
			{ENDE	,INITIAL,Operation},
			WMENU_END
		};
