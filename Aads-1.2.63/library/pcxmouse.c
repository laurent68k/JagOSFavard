/*
 *			GEM Mouse Manager Sub-Library:
 *
 *
 *	Author  : FunShip
 *	File	: PCXwind.c
 *	Date    : 13 December 1995
 *	Release : 14 December 1995
 *	Version : 1.20
 *	Language: Pure C, Version du 3 Fevrier 1992 
 *	Country : FRANCE
 *	
 *
 *		  - Copyright Atari FunShip (c) 19995
 *		      - ATARI ST/STE/TT & Falcon -
 * -----------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <aes.h>

/*
 * -----------------------------------------------------------------------------------------
 *				Originial include
 */

#include "..\Include\PcXMouse.h"

/*
 * -----------------------------------------------------------------------------------------
 *				Private Global Variables
 */

typedef	struct	{
		  int	NewMouse;
		  int	*NewMouseForm;
		} Type_ShapeList;
		
/*
 * -----------------------------------------------------------------------------------------
 *				Private News Mouse Shape Defined
 */

#define	ATARI_MOUSE	int Atari[37]
#include ".\Mouse\Atari.h"

#define	APPLE_MOUSE	int Apple[37]
#include ".\Mouse\Apple.h"

#define	AMIGA_MOUSE	int Amiga[37]
#include ".\Mouse\Amiga.h"

#define	MAC_MOUSE	int Mac[37]
#include ".\Mouse\Mac.h"

#define	GLASS_MOUSE	int HourGlass[37]
#include ".\Mouse\Glass.h"

#define	BOMB_MOUSE	int Bomb[37]
#include ".\Mouse\Bomb.h"

#define	DISK_MOUSE	int Disk[37]
#include ".\Mouse\Floppy.h"

#define	SLEEP_MOUSE	int Sleep[37]
#include ".\Mouse\SLEEP.h"

#define	PTFLTHND_MOUSE	int PointFlatHand[37]
#include ".\Mouse\PtFltHnd.h"

#define	HELP_MOUSE	int Help[37]
#include ".\Mouse\Help.h"

#define	MOUSE_MOUSE	int AMouse[37]
#include ".\Mouse\Mouse.h"


/*
 * -----------------------------------------------------------------------------------------
 *				Private Global Variables
 */

static	int	M_Mouse		= -1;						/* Mouse shape type */
static	MFORM	*M_MouseForm	= NULL;						/* User shape defined */

static Type_ShapeList	
ShapeList[] 	= {	{MS_ATARI		,Atari},
			{MS_APPLE		,Apple},
			{MS_AMIGA		,Amiga},
			{MS_MAC			,Mac},
			{MS_HOURGLASS		,HourGlass},
			{MS_BOMB		,Bomb},
			{MS_DISK		,Disk},
			{MS_SLEEP		,Sleep},
			{MS_POINTFLATHAND	,PointFlatHand},
			{MS_HELP		,Help},
			{MS_MOUSE		,AMouse}
		};

/*
 * -----------------------------------------------------------------------------------------
 *				Publics Procedures
 */

void MS_Get(int *Mouse, MFORM **MouseForm)
/*
	Get the current Mouse type and user mouse form defined.
*/
{
  *Mouse	= M_Mouse;
  *MouseForm	= M_MouseForm;
}
 
int MS_Mouse(int Mouse,MFORM *MouseForm)
/*
	This call replace the old AES graf_mouse call. It has the same prototype but is
	more efficient.
*/
{
  switch(Mouse)
  {
    /*
     *	AES Constant: Altear the mouse cursor shape with standard shapes.
     */
    case ARROW:
    case TEXT_CRSR:
    case BUSYBEE:
    case POINT_HAND:
    case FLAT_HAND:
    case THIN_CROSS:
    case THICK_CROSS:
    case OUTLN_CROSS:
			if(M_Mouse != Mouse)
			{
			  graf_mouse(Mouse,MouseForm);
			  M_Mouse	= Mouse;
			  M_MouseForm	= MouseForm;
			}
			return(Mouse);
    /*
     *	AES Constant: Altear the mouse cursor shape with an user shape.
     */
    case USER_DEF:	if(M_MouseForm != MouseForm)
    			{
			  graf_mouse(Mouse,MouseForm);
			  M_Mouse	= Mouse;
			  M_MouseForm	= MouseForm;
    			}
			return(Mouse);
    /*
     *	AES Constant: Turn On/Off the mouse cursor only.
     */
    case M_OFF:
    case M_ON:
    			graf_mouse(Mouse,MouseForm);
			return(Mouse);
    /*
     *	New Cursor built-in
     */
    
    default:		if(M_Mouse != Mouse)
			{
			  M_Mouse	= Mouse;
			  M_MouseForm	= (MFORM *)ShapeList[Mouse-MS_ATARI].NewMouseForm;
			  graf_mouse(USER_DEF,M_MouseForm);
			}
    			return(Mouse);
  }
}
