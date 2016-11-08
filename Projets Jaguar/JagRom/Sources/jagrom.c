/*
 * ----------------------------------------------------------------------------------------------
 *		programme GEM pour transferer dans la carte ROM simulateur 
 *		le contenu d'un fichier.
 *
 *		VERSION : 2.0
 *		HISTORIQUE:	Version initiale TOS v1.0
 *					Port‚ sous GEM v2.0
 *
 *		FAVARD Laurent 1997 - Atari FunShip pour la jaguar
 *
 * ----------------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <tos.h>
#include <string.h>
#include <float.h>

#include <PcAads.h>

/*	---------------------------------------------------------------------------------------	*/

#include "Jagrom_F.h"
#include "RAM.h"

/*	---------------------------------------------------------------------------------------	*/

static	int				Sortir;

OBJECT					*TreeObject,
						*mnInstance;
int						WHandle1;

char					G_Path[PATH_LEN]					= "\0", 		/* Chemin seulement */
						G_PathName[PATH_LEN+FILENAME_LEN]	= "\0", 		/* Chemin + Nom de fichier */
						G_FileName[FILENAME_LEN]			= "\0", 		/* Nom de fichier seulement */
					
						G_Extension[EXT_LEN]				= "*.PRG\0";	/* Extension de fichiers */

unsigned long			G_BytesTotal, G_BytesCount;

/*	---------------------------------------------------------------------------------------	*/
static void ClearJauge(int x,int y, int w,int h)
/*
	Remet la jauge a zero.
	
	Les valeurs (x,y,w,h) sont le clipping AES fournie par AADS pour effectuer un Redraw sous la liste des rectangles.
*/
{
	TreeObject[JAUGE].ob_width = 0;
	objc_draw(TreeObject,BJAUGE,2,x,y,w,h);
}

/*	---------------------------------------------------------------------------------------	*/
static void Jauge(int x,int y, int w,int h)
/*
	Incr‚mente la jauge de progession en fonction du nombre de la valeur G_BytesCount par rapport a G_BytesTotal.
	
	Les valeurs (x,y,w,h) sont le clipping AES fournie par AADS pour effectuer un Redraw sous la liste des rectangles.
*/
{
	TreeObject[JAUGE].ob_width = (unsigned int)( (unsigned long)(TreeObject[BJAUGE].ob_width) * G_BytesCount / G_BytesTotal);
	objc_draw(TreeObject,JAUGE,2,x,y,w,h);
}

/*	---------------------------------------------------------------------------------------	*/
static	void ToIncrementeJauge(unsigned long CountToDo, unsigned long CountPerfomed)
{
	G_BytesTotal	=	CountToDo;
	G_BytesCount	=	CountPerfomed;
	F_FreeDraw( WHandle1, Jauge );
}

/*	---------------------------------------------------------------------------------------	*/
void Display( unsigned int StartTime, unsigned int EndTime )
{
	char				chTexte1[10] = "0000";
	char				chTexte2[10] = "0000";
    unsigned int		MinutesStart, MinutesEnd;
    unsigned int		SecondsStart, SecondsEnd;
    unsigned int		Number, Rate;

	if( StartTime != 0 && EndTime != 0)
	{
	    MinutesStart	=	(StartTime & 0x7E0) >> 5;
    	MinutesEnd		=	(EndTime & 0x7E0) >> 5;

	    Number			=	MinutesEnd - MinutesStart;
    	Number			=	Number	*	60;           				/*	Convert to seconds */

	    SecondsStart	=	StartTime & 0x1F;
    	SecondsEnd		=	EndTime & 0x1F;

	    Number			=	Number + ( SecondsEnd - SecondsStart );

		Rate			=	(unsigned int)( (float)G_BytesTotal / (float)Number);

	    sprintf( chTexte1, "%d", Number );
	    sprintf( chTexte2, "%d", Rate );
	}
	else
	{
	    sprintf( chTexte1, "??????" );
	    sprintf( chTexte2, "????????" );
	}
	
   	F_WriteText( FMAIN, TIME, chTexte1);
   	F_WriteText( FMAIN, RATE, chTexte2);
}

/*	---------------------------------------------------------------------------------------	*/
void WindAction(void)
{
	switch( F_NameObject)
	{
		/*	Select file */
		case	SELECT:	W_MenuItem	=	ITEM21;
						MenuAction( WHandle1);
						break;

		/*	Load */
		case	UPLOAD:	W_MenuItem	=	ITEM31;
						MenuAction( WHandle1);
						break;

		/*	Make IMG */
		case	MAKE:	W_MenuItem	=	ITEM32;
						MenuAction( WHandle1);
						break;

		case	JGON:	G_JaugeON	=	( G_JaugeON ) ? FALSE: TRUE;
						F_Selected( FMAIN, JGON, G_JaugeON);
						break;
						
		/*	Exit button */
		case	QUIT:	Sortir = TRUE;
						break;
		default:
		    		   	form_alert(1,"[0][ unknow object ][ok]");
    					break;
	}
}

/*
 * ---------------------------------------------------------------------------------------
 *								Process menu command
 */

void MenuAction(int WindowHandle)
{
	char			L_chFileName[30];
	unsigned int	StartTime, EndTime ;
	
   	switch(W_MenuItem)
    {
    		/*	About */
		    case	ITEM11:
					    	F_Open( FABOUT, "About me...", F_WIND, NOTEXT, AboutListe );
    						break;

			/*	Open */
		    case	ITEM21:	if(A_FileSelector(G_PathName, G_FileName, G_Extension, G_Path, G_FILE_OPEN)) 
    						{								
								/*	V‚rifi la taille de la chaine pour l'icone */
								if( strlen(G_FileName) > 27)
								{
									memcpy( L_chFileName, G_FileName, 26);
									L_chFileName[27]	=	0;
									strcat( L_chFileName, "...");
								}	
								else
									strcpy( L_chFileName, G_FileName );
									
								F_WriteText( FMAIN, FILEINFO, L_chFileName);
								W_MnEnable(WindowHandle, ITEM31);
								W_MnEnable(WindowHandle, ITEM32);
								
								TreeObject[UPLOAD].ob_state &= ~DISABLED;
								TreeObject[MAKE].ob_state &= ~DISABLED;
						 		F_RedrawObject( TreeObject, UPLOAD);
						 		F_RedrawObject( TreeObject, MAKE);
						 		
						 		F_FreeDraw( WHandle1, ClearJauge );
    						}
	    					break;

			/*	Quit */
		    case	ITEM22:	Sortir = TRUE;
	    					break;

			/*	Load */
		    case	ITEM31:	graf_mouse( BUSYBEE, NULL );
						    F_FreeDraw( WHandle1, ClearJauge );
							Display( 0, 0 );
		    				SendFile( G_PathName, ToIncrementeJauge, &StartTime, &EndTime );
							Display( StartTime, EndTime );
		    				graf_mouse( ARROW, NULL );
	    					break;

			/*	Make IMG */
		    case	ITEM32:	graf_mouse( BUSYBEE, NULL );
		    				MakeBIN( G_PathName );
		    				graf_mouse( ARROW, NULL );
	    					break;

			/*	help */
		    case	ITEM41:
	    					break;

		    default:
			    		   	form_alert(1,"[0][ Unknow menu item ][ok]");
    						break;
		}
}

/*	---------------------------------------------------------------------------------------	*/

int main(void)
{
	Type_Formular_Parameter	*Formulaire_Class;

	Pdomain( 1 );
  
	if(!A_Open("JagRom_F.Rsc"))
	{
    	form_alert(1,"[1][JagRom_F.rsc error: | Unable to found it ][ Cancel ]");
    	A_Close();
    	return(-1);												/* Quit immediatly */
	}

	W_Init();													/* Init Window  */
	F_Init();													/* Init Form    */
	M_Init();
  
	/*
	 *	Ouverture des formulaires et fenetres
 	 */
	Formulaire_Class 			= F_GetParameter();

	Formulaire_Class->TreeName	= FMAIN;
	Formulaire_Class->Title		= "Jaguar ROM Simulator";	/* Formular's title */
	Formulaire_Class->Mode		= F_WIND;					/* Opening mode */
	Formulaire_Class->FirstEdit	= NOTEXT;					/* First Editable Text */
	Formulaire_Class->List		= MainListe;				/* Event list */
	Formulaire_Class->FormIcon	= APPICON;					/* window's Icon  */
	Formulaire_Class->Mouse		= ARROW;
	WHandle1 = F_XOpen(Formulaire_Class);
  
	rsrc_gaddr( R_TREE, FMAIN, &TreeObject);
	
	mnInstance	=	W_MenuOpen(WHandle1, MAINMENU , WMenu1);
	W_SetIconApp(APPICON);
	F_FreeDraw( WHandle1, ClearJauge );

	Sortir = FALSE;
	do
	{
	    A_WaitEvent();
	    W_Event();
	    M_Event();
    	F_Event();				
	}
	while(!Sortir);

	free(Formulaire_Class);
	W_MenuClose(WHandle1);
	F_Close(FMAIN);
  
	W_Exit();
	F_Exit();
	M_Exit();
  
	A_Close();							/* Close Application	*/
	return(0);							/* for the shell 	*/
}

