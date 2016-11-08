/*
 *			Miscallenous Procedures Modules Header
 *
 *
 *	Author  : FunShip
 *	File	: Tools.h
 *	Date    : 18 December 1995
 *	Release : 27 January  1996
 *	Version : 1.20
 *	Language: Pure C, Version du 3 Fevrier 1992 
 *	Country : FRANCE
 *	
 *
 *			  - Copyright Atari FunShip (c) 1991-1995 -
 *			      - ATARI ST/STE/Mega/TT & Falcon -
 * -----------------------------------------------------------------------------------------
 */
 

#ifndef	__AADSTOOLS__
#define	__AADSTOOLS__	__AADSTOOLS__

#include <aes.h>
#include <vdi.h>

/*
 * ---------------------------------------------------------------------------------------
 *			
 */

#define		MU_NULL		0
/*
 *	Type Absolute Coordinate for an object
 */

typedef	struct	{
		  int	X;
		  int	Y;
		} Type_CoordXY;
				

/*
 *	To save/restore an area screen
 */
 
typedef	struct	{  		  int		VdiHandle;			/* VDI Handle of buffer (= VDIhandle) */		  int		pxy[8];				/* Point XY array for memory buffer */		  MFDB		mfdb,scr;			/* Memory Form Definition Blocks of graphics */		  char		data[];				/* Any extra data */		} Type_Buffer_Screen;

/*
 * ---------------------------------------------------------------------------------------
 *			
 */

int	GetPere(OBJECT *Arbre,int fils);
void	SumXY(Type_CoordXY *CoordXY,OBJECT *Arbre,int Id_Object);
int	CalculIntersect(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2,int tablexywh[]);

void	*ScreenSave(int VdiHandle,int x,int y,int w,int h);
void	ScreenRestore(void *buffer);

int	CalculKey(char *EntryText);

OBJECT	*CpMenu(OBJECT *ArbreMenu);
int CalculBoite (OBJECT *MenuAddress,int object);

#endif