/*
 * ----------------------------------------------------------------------------------------------
 *					Messages de l'application
 *
 * 	Author  : 	FAVARD Laurent, FunShip (c)
 *	File    : 	Messages.h
 *	Date    : 	10 December 1996
 *	Release : 	04 May 2000
 *	Version : 	1.0
 *	Country : 	FRANCE
 *			
 *
 *								- Copyright Atari FunShip (c) 1994-97 -
 *							- Atari ST, STE, TT, Falcon, C-LAB, Medusa, Hades -
 * ----------------------------------------------------------------------------------------------
 */


#define		GetMessage(a)		a		

/*
 *		Message d'erreurs
 *		0 = Rien, 1 = Exclamation, 2 = Interrogation, 3 = Stop, 4 = Informations, 5 = Disquette
 */
 
#define		ERROR_COLOR			"[3][ Bad Colours Number | 16 colours needed ][ Ok ]"
#define		ERROR_SCREEN		"[3][ Bad Screen Resolution | 640 x 400 needed ][ Ok ]"
#define		ERROR_RSC			"[3][ Jaguar_?.Rsc | Ressource not found ][ Ok ]"
#define		ERROR_WINDOW		"[1][ Unable to open the console window ][Cancel]"
#define		ERROR_JAGHEADER		"[3][ This file have not any Jag header | Use default settings ][Ok]"
#define		ERROR_NOFILE		"[3][ File not found or it doesn't exists ][Ok]"
#define		ERROR_HELPAPP		"[3][ No Help application | selected ][Ok]"

#define		ERROR_STGUIDE		"[3][ ST-Guide not found as | an accessory installed | or process running ][ok]"
#define		ERROR_EXCEDED_SIZE	"[3][ Not enough space to add a new line ][Ok]"
#define		ERROR_TRANSFER		"[3][ Send error during transfer ][Ok]"
#define		ERROR_PROCESS		"[1][ Process no lunched ][Ok]"
#define		ERROR_MEMTRANSF		"[2][ Not enought memory to accept this transfer ][Ok]"

#define		ERROR_FILECREATE	"[2][ Unable to create the file ][Ok]"
#define		ERROR_FILEOPEN		"[2][ Unable to open the file ][Ok]"

#define		ERROR_NOJAGUAR		"[2][ Jaguar not found on cartridge. | Cannot use this port ][Ok]"
#define		ERROR_INQUIRE		"[2][ Ping error: | Frame returned doesn't match | to an INQUIRE Command ][Ok]"

#define		ERROR_COOKIE		"[3][ No Cookie Jar found | Unable to check your OS ][Quit]"
#define		ERROR_MAGIC			"[3][ This JagStudio version run |   only with MagiC5 ][Quit]"
#define		ERROR_NAES			"[3][ This JagStudio version run |   only with N.AES ][Quit]"
#define		ERROR_FALCON30		"[3][ This JagStudio version run | with Falcon and TOS/MiNT ][Quit]"
#define		ERROR_RECEIVER		"[3][ Unable to create thread receiver ][Ok]"

#define		ERROR_INPUT			"[3][ Name, upload and start address | must be entered ][Ok]"

#define		RUNOK				"[1][ Run command completed | attention before to hit OK | No JagOS soft can disturbe Joypad#2 ][Ok]"
#define		GETOK				"[1][ dump complete, datas saved as dump.bin ][Ok]"

#define		WAITCONNECT			"[3][ Now you can turn on your Jaguar | press OK when jaguar is ready ][OK]"

/*
 *	Cookie de Magic et N.AES
 */

#define		ID_MAGIC			"MagX"
#define		ID_NAES				""

/*
 *		Titres des fenetres
 */
 
#define		G_TITLE_TOOLBAR		"JagBar"
#define		G_TITLE_INFO		"Jaguar Studio Server"
#define		G_TITLE_TERM		"JagConsole"
#define		G_TITLE_PARM		"Parameters"
#define		G_TITLE_QUIT		"Jaguar exit"
#define		G_TITLE_MORE		"Team developpers"
#define		G_TITLE_SENDER1		"Load binaries only"
#define		G_TITLE_SENDER2		"Load and Run program"
#define		G_TITLE_MEMPARAM1	"Run program at"
#define		G_TITLE_MEMPARAM2	"Get at"
#define		G_TITLE_TRANSFERT1	"Jaguar to Atari"
#define		G_TITLE_TRANSFERT2	"Atari to Jaguar"
#define		G_TITLE_EEPROM		"Read/Write EEPROM"
#define		G_TITLE_CPU			"CPU 68000"
#define		G_TITLE_INQUIRE		"Ping succeed"
#define		G_TITLE_SMILEY		""
#define		G_TITLE_INFOFILE	"File informations"
#define		G_TITLE_NEWFORM		"New object"
#define		G_TITLE_GENFORM1	"Create a cooperant program"
#define		G_TITLE_GENFORM2	"Create a assembler source"
#define		G_TITLE_GENFORM3	"Create a program under JagOS"
#define		G_TITLE_GENFORM4	"Create a program whitout JagOS"
#define		G_TITLE_ENVR		"Externals configuration"
#define		G_TITLE_GENFRMP		"Advanced"
#define		G_TITLE_DEBUG		"Debug window"
#define		G_TITLE_WERR		"Jaguar comm. error"
#define		G_TITLE_WSTATUS		"Status"

/*
 *		Messages quelconques
 */
 
#define		G_FILE_NEW			"New source file..."
#define		G_FILE_OPEN			"Open executable jaguar..."
#define		G_SRC_OPEN			"Open source jaguar..."
#define		G_SAVE_GEN			"Save your new source as..."
#define		G_SAVE_BIN			"Save datas as..."
#define		G_LOAD_BIN			"Load datas from..."
#define		G_OPEN_HIST			"Load a text file..."
#define		G_SAVE_HIST			"Save text as..."
#define		G_CRY_OPEN			"Select the CRY file to convert..."
#define		G_SELECT_EDITER		"Select your text editor..."
#define		G_SELECT_ASM		"Select your 680x0 Assembler..."

#define		G_JAG_OK			"Ready"
#define		G_JAG_ERR			"No connected"

#define		G_JAG2ST			"Transfer: Jaguar to ATARI"
#define		G_ST2JAG			"Transfer: ATARI to Jaguar"

#define		G_BROWSE_TGA		"Show your .CRY file"
/*
 *		Messages de confirmations
 */

#define		WRNG_SCREEN			"[2][ Resolution has been changed | Is it your new resolution ? ][Yes|Ignore]"
#define		ATT_CONSMODIF		"[2][ Contents of console | has changed | save it before ? ][Yes|No]"
#define		ATT_QUITTER			"[2][ Do you want to exit ? ][Ok|Cancel]"
#define		MSG_ASKINQUIRE		"[2][ Do you want an Inquire ? ][Ok|Cancel]"
#define		MSG_SAVE_DATAS		"[2][ Do you want to save datas ? ][Ok|Cancel]"

