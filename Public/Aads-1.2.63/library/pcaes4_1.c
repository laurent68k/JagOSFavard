/*
 *
 *	News AES definitions for version 3.3, 4.0 and 4.1
 *		
 *
 *		FunShip, (c) FRANCE 1994 - ATARI Falcon030
 *
 *		Version 1.00 from 5 November 1994.
 *		Turbo-C and Pure-C language
 *
 */

#include <AES.h>
#include "..\Include\PCAes4_1.h"
 
typedef struct 
{
	int		*contrl;
	int		*global;
	int		*intin;
	int		*intout;
	void	**addrin;
	void	**addrout;
} Type_Params;

Type_Params	params;		

extern	void XAes(Type_Params *params);

static void InitParams(void)
/*
	Initialize params structure to make a call aes.
*/
{
	params.contrl	= _GemParBlk.contrl;
	params.global	= _GemParBlk.global;
	params.intin	= _GemParBlk.intin;
	params.intout	= _GemParBlk.intout;
	params.addrin	= _GemParBlk.addrin;
	params.addrout	= _GemParBlk.addrout;
	
	XAes(&params);
}

/*
 * ----------------------- New's Aes functions body -----------------------
 */

#ifndef	__AES40__

int menu_popup(MENU *me_menu,int me_xpos,int me_ypos,MENU *me_mdata)
{
	_GemParBlk.contrl[0]	= 36;
	_GemParBlk.contrl[1]	= 2;
	_GemParBlk.contrl[2]	= 1;
	_GemParBlk.contrl[3]	= 2;
	_GemParBlk.contrl[4]	= 0;
	_GemParBlk.intin[0]	= me_xpos;
	_GemParBlk.intin[1]	= me_ypos;
	_GemParBlk.addrin[0]	= me_menu;
	_GemParBlk.addrin[1]	= me_mdata;

	InitParams();
	return(_GemParBlk.intout[0]);
} /* menu_popup */

int menu_attach(int me_flag,OBJECT *me_tree,int me_item,MENU *me_mdata)
{
	_GemParBlk.contrl[0]	= 37;
	_GemParBlk.contrl[1]	= 2;
	_GemParBlk.contrl[2]	= 1;
	_GemParBlk.contrl[3]	= 2;
	_GemParBlk.contrl[4]	= 0;
	_GemParBlk.intin[0]	= me_flag;	
	_GemParBlk.intin[1]	= me_item;
	_GemParBlk.addrin[0]	= me_tree;
	_GemParBlk.addrin[1]	= me_mdata;

	InitParams();
	return(_GemParBlk.intout[0]);
}

int menu_istart(int me_flag,OBJECT *me_tree,int me_imenu,int me_item)
{
	_GemParBlk.contrl[0]	= 38;
	_GemParBlk.contrl[1]	= 3;
	_GemParBlk.contrl[2]	= 1;
	_GemParBlk.contrl[3]	= 1;
	_GemParBlk.contrl[4]	= 0;
	_GemParBlk.intin[0]	= me_flag;
	_GemParBlk.intin[1]	= me_imenu;
	_GemParBlk.intin[2]	= me_item;
	_GemParBlk.addrin[0]	= me_tree;

	InitParams();
	return(_GemParBlk.intout[0]);
}

int menu_settings(int me_flag,MN_SET *me_values)
{
  _GemParBlk.contrl[0]	= 39;
  _GemParBlk.contrl[1]	= 1;
  _GemParBlk.contrl[2]	= 1;
  _GemParBlk.contrl[3]	= 1;
  _GemParBlk.contrl[4]	= 0;
  _GemParBlk.intin[0]	= me_flag;
  _GemParBlk.addrin[0]	= me_values;
	
  InitParams();
  return(_GemParBlk.intout[0]);
}

int appl_getinfo(int gtype,int *gout1,int *gout2,int *gout3,int *gout4)
{
  _GemParBlk.contrl[0]	= 13;
  _GemParBlk.contrl[1]	= 0;
  _GemParBlk.contrl[2]	= 0;
  _GemParBlk.contrl[3]	= 0;
  _GemParBlk.contrl[4]	= 1;
  _GemParBlk.contrl[5]	= 4;
  _GemParBlk.intin[0]	= gtype;
  
  InitParams();
  *gout1		= _GemParBlk.intout[0];
  *gout2		= _GemParBlk.intout[1];
  *gout3		= _GemParBlk.intout[2];
  *gout4		= _GemParBlk.intout[3];

  return(_GemParBlk.intout[0]);
}

int appl_search(int mode,char *ap_sname,int *op_stype,int *op_sid)
/*
	La DDFS n'est pas claire … propos de cet appel de fonction.
	
*/
{
  _GemParBlk.contrl[0]	= 18;
  _GemParBlk.contrl[1]	= 1;
  _GemParBlk.contrl[2]	= 3;
  _GemParBlk.contrl[3]	= 1;
  _GemParBlk.contrl[4]	= 0;
  _GemParBlk.intin[0]	= mode;
  _GemParBlk.addrin[0]	= ap_sname;
  
  
  InitParams();
  *op_stype		= _GemParBlk.intout[1];			/* !! */
  *op_sid		= _GemParBlk.intout[2];			/* !! */
  return(_GemParBlk.intout[0]);					/* !! */
}

int rsrc_rcfix(OBJECT *rc_header)
{
  _GemParBlk.contrl[0]	= 115;
  _GemParBlk.contrl[1]	= 0;
  _GemParBlk.contrl[2]	= 1;
  _GemParBlk.contrl[3]	= 1;
  _GemParBlk.contrl[4]	= 0;
  _GemParBlk.addrin[0]	= rc_header;
  
  InitParams();
  return(_GemParBlk.intout[0]);
}

#endif

int objc_sysvar(int mode,int which,int in1,int in2,int *out1,int *out2)
{
  _GemParBlk.contrl[0]	= 48;
  _GemParBlk.contrl[1]	= 4;
  _GemParBlk.contrl[2]	= 3;
  _GemParBlk.contrl[3]	= 0;
  _GemParBlk.contrl[4]	= 0;
  _GemParBlk.intin[0]	= mode;
  _GemParBlk.intin[1]	= which;
  _GemParBlk.intin[2]	= in1;
  _GemParBlk.intin[3]	= in2;
  
  InitParams();
  *out1			= _GemParBlk.intout[1];
  *out2			= _GemParBlk.intout[2];
  return(_GemParBlk.intout[0]);
}
