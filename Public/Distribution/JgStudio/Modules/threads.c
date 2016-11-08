/*
 * ----------------------------------------------------------------------------------------------
 *					APPLICATION Jaguar Studio: Gestion des Threads MagiC5
 *
 * 	Author  : 	FAVARD Laurent
 *	File    : 	Threads.c
 *	Date    : 	21 October 1998
 *	Release : 	21 October 1998
 *	Version : 	1.0
 *	Country : 	FRANCE
 *			
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * ----------------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <tos.h>

#include	"MT_AES.h"

#include	<PcAads.h>
#include	<JToolbox.h>												/* ToolBox JAGUAR */

#include	"..\modules\const.h"

/* ---------------------------------------------------------------------------------------------- */

int	PIDReceiver	=	-1;
extern	void HandlerMessage(int FrameCode, long SourcePID, long TargetPID, int LenghtData, TRecipient *Recipient);

WORD	p_apid;
WORD	p_global[15];

/*
 * ----------------------------------------------------------------------------------------------
 */

#define SHW_THR_CREATE	20
#define SHW_THR_EXIT	21
#define SHW_THR_KILL	22

/* ---------------------------------------------------------------------------------------------- */
static long cdecl Receiver( void *Parameters )
{
		
	p_apid = MT_appl_init(p_global);
	while(TRUE)
	{
		HookJaguarMessage( HandlerMessage );
	}
}

/* ---------------------------------------------------------------------------------------------- */
int CreatReceiver( void )
{
#ifdef	__MAGIC_OS__
	THREADINFO	thi;
	int			pid;

	if( PIDReceiver < 0 )
	{
		thi.proc = (void *) Receiver;
		thi.user_stack = NULL;
		thi.stacksize = 4096L;
		thi.mode = 0;
		thi.res1 = 0L;
		pid	=	shel_write( SHW_THR_CREATE, 1, 0, (char *) &thi, NULL );
		return	pid;
	}
#endif
	return -1;
}

/* ---------------------------------------------------------------------------------------------- */
int KillReceiver( void )
{
#ifdef	__MAGIC_OS__
	int	Retour;

	/*	Kill thread Receiver	*/
	Retour	=	shel_write(SHW_THR_KILL, 0, PIDReceiver, NULL, NULL);
	return	Retour;
#else
	return	-1;
#endif
}
