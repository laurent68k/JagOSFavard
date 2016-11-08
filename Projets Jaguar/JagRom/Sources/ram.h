

#ifndef		__RAM

#define		__RAM__	__RAM__

/*	---------------------------------------------------------------------------------------	*/
void MenuAction(int WindowHandle);

static Type_Window_Menu_List
WMenu1[]=		{
					{ITEM11	, INITIAL, MenuAction},		/*	About */

					{ITEM21 , INITIAL, MenuAction},		/*	Open */
					{ITEM22 , INITIAL, MenuAction},		/*	Quit */

					{ITEM31 , INITIAL, MenuAction},		/*	Upload to RAM */
					{ITEM32 , INITIAL, MenuAction},		/*	Make .IMG */

					{ITEM41 , INITIAL, MenuAction},		/*	Help	*/
					WMENU_END
				};

void WindAction(void);

static Type_Form_List
MainListe[]=	{
					{EXIT	, WindAction, NULL},
					{SELECT	, WindAction, NULL},
					{UPLOAD	, WindAction, NULL},
					{MAKE	, WindAction, NULL},
					{JGON	, WindAction, NULL},
					{QUIT	, WindAction, NULL},
					FORM_END
				};

static Type_Form_List
AboutListe[]=	{
					{0	, NULL, NULL},
					FORM_END
				};

/*	---------------------------------------------------------------------------------------	*/

#define					G_FILE_OPEN			"Open a binary file..."

#define					PATH_LEN			10240
#define					FILENAME_LEN		1024
#define					EXT_LEN				6

/*	---------------------------------------------------------------------------------------	*/

extern	int	G_JaugeON;

extern	int SendFile(	char *PathName, 
				void (*UserIncrement)(unsigned long CountToDo, unsigned long CountPerfomed),
				unsigned int *StartTime, unsigned int *EndTime );
extern	int MakeBIN(char *PathName);


#endif
