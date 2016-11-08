/*
 * ----------------------------------------------------------------------------------------------
 *					Interfacage avec ST-GUIDE
 *
 * 	Author  : 	FAVARD Laurent, FunShip (c)
 *	File    : 	PCStGuid.C
 *	Date    : 	05 September 1997
 *	Release : 	15 July 1998
 *	Version : 	1.0
 *	Country : 	FRANCE
 *			
 *
 *								- Copyright Atari FunShip (c) 1994-97 -
 *							- Atari ST, STE, TT, Falcon, C-LAB, Medusa, Hades -
 * ----------------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <aes.h>
#include <string.h>

#include "..\include\Pcxapp.h"
#include "..\include\Pcstguid.h"

#define		ERROR_STGUIDE		"[3][ ST-Guide not found as | an accessory installed | or process running ][ok]"

/*
 * ----------------------------------------------------------------------------------------------
 *				Gestion des bulles d'aides
 */

#define		BUBBLE_APPLICATION	"BUBBLE  "
#define		BUBBLEGEM_SHOW		0xBABB
#define		BUBBLEGEM_ACK		0xBABC

/*
 * ----------------------------------------------------------------------------------------------
 */

static TYPE_MESSAGE_START	G_MessageStart;

/*
 * ----------------------------------------------------------------------------------------------
 */

int STG_Open(char *HelpFile, char *Parameter)
{
	int		L_HandleSTGuide;
	char		L_HelpFile[1024];


	L_HandleSTGuide	=	appl_find(G_STGUIDE);							
	if(L_HandleSTGuide != -1)
	{
		strcpy(L_HelpFile, HelpFile);

		if( Parameter != NULL && strlen(Parameter) != 0)
		{
			strcat(L_HelpFile, " ");
			strcat(L_HelpFile, Parameter);
		}
			
		G_MessageStart.MessageNumber	= AV_START;
		G_MessageStart.HandleSource	= AesHandle;
		G_MessageStart.String			= L_HelpFile;
								
		G_MessageStart.NU0 = 0;
		G_MessageStart.NU1 = 0;
		G_MessageStart.NU2 = 0;
		G_MessageStart.NU3 = 0;	

		appl_write(L_HandleSTGuide, 16, (int *)&G_MessageStart);
	}
	else
		form_alert(1, ERROR_STGUIDE);

	return L_HandleSTGuide != -1;
}

/*
 * ----------------------------------------------------------------------------------------------
 */

int STG_Bubble(char *String, int MouseX, int MouseY)
{
	char	*bbBuffer;
	int		BubbleID;
	int		Message[8];

	bbBuffer	=	(char *)malloc( strlen(String) + 1 );
	strcpy(bbBuffer, String);
	BubbleID	=	appl_find(BUBBLE_APPLICATION);
	if( BubbleID >= 0)
	{
		Message[0]	=	BUBBLEGEM_SHOW;
		Message[1]	=	BubbleID;
		Message[2]	=	0;
		if( MouseX == -1 && MouseY == -1 )
		{
			Message[3]	=	A_MouseX;
			Message[4]	=	A_MouseY;
		}
		else
		{
			Message[3]	=	MouseX;
			Message[4]	=	MouseY;
		}
		Message[5]	=	(int)(((long)bbBuffer >> 16) & 0x0000FFFF);
		Message[6]	=	(int)((long)bbBuffer & 0x0000FFFF);
		Message[7]	=	0x0;
		appl_write(BubbleID, 16, Message);			
	}
	else
		return	0;
		
	return	1;
}