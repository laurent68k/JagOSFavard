/*
 *			GEM Window Manager Sub-Library: Global Include PRIVATE.
 *
 *			This file contains somes globals definitions for:
 *				- PcXWind.c
 *				- PcXWMenu.c
 *			AND ONLY FOR THESE...(Not for the User-Programmer)
 *
 *
 *	Author  : FunShip
 *	File	: PCXwind.c
 *	Date    : 09 December 1995
 *	Release : 12 December 1995
 *	Version : 1.20
 *	Language: Pure C, Version du 3 Fevrier 1992 
 *	Country : FRANCE
 *	
 *
 *		  - Copyright Atari FunShip (c) 1991,92,93,94,95
 *			   - ATARI ST/STE/TT & Falcon -
 * -----------------------------------------------------------------------------------------
 */

#ifndef __GLOBALWINDOW__
#define	__GLOBALWINDOW__	__GLOBALWINDOW__

#include <aes.h>

#include "..\Include\PcFile.h"

/*
 * -----------------------------------------------------------------------------------------
 *
 */

#define DESKTOP		0					/* Desktop Window handle */
#define	DEPTH		999					/* For obj_draw */

#define	TRUE		1
#define	FALSE		0

/*
 * -----------------------------------------------------------------------------------------
 *				Privates types definitions
 */
 
typedef struct {
		 /* Window's datas */
		 int 	handle;					/* Window's handle GEM */
		 int 	attributs;				/* Window attributes */
		 int	type;					/* Modal/Modeless */
		 
		 int 	prev_x;					/* Previous (x,y,w,h) */
		 int 	prev_y;
		 int 	prev_w;
		 int 	prev_h;

/*		 int 	icon_x;*/					/* Icon position used currently */
/*		 int 	icon_y;*/

		 int	ligne;					/* Current graph line */
		 int	colonne;				/* Current graph colum */

		 char	titre[W_TITLE_MAX];
		 char	info[W_INFO_MAX];
		
		 /* Inconification's Datas */
		 int		treename;			/* Formular's name with Icon */
		 int		isiconify;			/* Show if window is iconify */
		 int		globalicon;			/* Is became global icon or not */

		 /* Window-Menu's Datas */
		 int		WithMenu;			/* Window Have(n't) got a window */
		 int		MenuModelName;			/* Name of menu MODEL */
		 OBJECT		*MenuAddress;			/* Menu tree instance's address */
		 Type_File	*MenuListEvent;			/* Event's list for each menu's item */
		 int		NextGTitle;			/* Next first G_Title for begin the menu display */

		 /* Window's mouse definition */
		 int		Mouse;				/* Mouse's Shape */
		 MFORM		*MouseForm;			/* User Mouse's Shape */
		 		 
		 /* Window's Methods */	 		 
		 void	(*op_close)(int);			/* Call when Close */
		 void	(*op_redraw)(int);			/* Call when Redraw */
		 void 	(*op_full)(int);			/* Call when Full  */
		 void 	(*op_sized)(int);			/* Call when Sized  */
		 void 	(*op_uppage)(int);			/* Call when Page Up */
		 void 	(*op_downpage)(int);			/* Call when Page Down */
		 void 	(*op_upline)(int);			/* Call when Line Up */
		 void 	(*op_downline)(int);			/* Call when Line Down */
		 void 	(*op_rightpage)(int);			/* Call when Page Right */
		 void 	(*op_leftpage)(int);			/* Call when Page Left */
		 void 	(*op_rightcolum)(int);			/* Call when Colum Right */
		 void 	(*op_leftcolum)(int);			/* Call when Colum Left */
		 void 	(*op_vslider)(int,int);			/* Call when V-Slide */
		 void 	(*op_hslider)(int,int);			/* Call when H-Slide */
		 void 	(*op_iconify)(int);			/* Call when Iconify */
		 void 	(*op_uniconify)(int);			/* Call when Uniconify */
 		 void 	(*op_topped)(int);			/* Call when topped */
 		 void 	(*op_moved)(int);			/* Call when moved */

 		 void 	(*ClicLeft)(int);			/* Call when left clic */
 		 void 	(*ClicRight)(int);			/* Call when right clic */
	       } Type_Window;

/*
 * -----------------------------------------------------------------------------------------
 *				Several Prototypes for PcXWind and PcXWMenu
 *				These procedures are limited private.
 */

/* 
 *	From PcXWind.c 
 */

Type_Window *WindSearch(int handle);

/* 
 *	From PcXWMenu.c 
 */

void 		WMnEvent(void);						/* Management */
void	 	RefreshMenu(Type_Window *Window);			/* Redraw Menu */
void 		AdjustPosition(Type_Window *Window);			/* When window moved */
int 		CheckSize(int WindowHandle);

#endif
