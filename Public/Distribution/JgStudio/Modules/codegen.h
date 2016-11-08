/*
 * ----------------------------------------------------------------------------------------------
 *			Module de gestion des s‚lecteurs de fichiers et d'appel un PRG externe
 *
 * 	Author  : 	FAVARD Laurent, FunShip (c)
 *	File    : 	CODEGEN.H
 *	Date    : 	19 December 1998
 *	Release : 	14 janauray 1999
 *	Version : 	1.0
 *	Country : 	FRANCE
 *			
 *
 *				- Copyright Atari FunShip (c) 1994-96 -
 *	 		     - ATARI ST,STE,TT, Falcon, Medusa et Hades-
 * ----------------------------------------------------------------------------------------------
 */

#ifndef		__CODEGEN__
#define		__CODEGEN__	__CODEGEN__

#include	"..\modules\const.h"

#define		COOPERANT		1
#define		SOURCEASM		2
#define		FREEWITHOS		3
#define		FREEPROGRAM		4

#define		LEN_FILE		31
#define		LEN_TITLE		61
#define		LEN_DATE		5
#define		LEN_BUSINESS	61
#define		LEN_EXECNAME	31
#define		LEN_MSG			61
#define		LEN_AUTHOR		31
#define		LEN_ADDRESS		7

typedef	struct	{
					int		Type;
                    char	IconName[LEN_FILE];
                    char	IconPathName[PATH_LEN+FILENAME_LEN];
                    char	CursorName[LEN_FILE];
                    char	CursorPathName[PATH_LEN+FILENAME_LEN];
                    int		BeginMessage;
                    int		EndMessage;
                    int		Icon;
                    int		Cursor;

                    /*	To define the source program header */
                    char	Title[LEN_TITLE];
                    char	Date[LEN_DATE];
                    char	Business[LEN_BUSINESS];
                    char	Author[LEN_AUTHOR];
                    char	ExecName[LEN_EXECNAME];
                    char	UploadAddress[LEN_ADDRESS];
                    char	StartAddress[LEN_ADDRESS];

                    char	TextMsgStart[LEN_MSG];
                    char	TextMsgEnd[LEN_MSG];

                    /* To define param buffers for Jag Routine source only */
                    unsigned int	SizeParamIn;
                    unsigned int	SizeParamOut;

				} TSource;
				
extern	int Generation( TProjet *Projet, TSource *Definition );
				
#endif