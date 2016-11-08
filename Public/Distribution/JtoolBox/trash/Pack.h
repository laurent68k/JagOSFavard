/*	------------------------------------------------------------------------------
;	Project JAGUAR:	JAGUAR PROTOCOLE COMMUNICATIONS.
;	LOW LEVEL CODE FOR TOS MACHINE. IMPLEMENTATION C OG GetPack and SendPack
;
;	File:		Pack.h
;	Author:		FAVARD Laurent
;	Date:		12 April 2000
;	Release:	12 April 2000
;	Language:	C
;
;	Atari Jaguar France / Copyright 1997-2000
;	------------------------------------------------------------------------------*/

#ifndef	__PackH__
#define	__PackH__

extern	unsigned long cdecl GetPack(void *Buffer, int Expected);
extern	long cdecl SendPack(void *Buffer, int Lenght);
extern	unsigned int cdecl GetPort( void );
extern	void cdecl SetPort( unsigned int port );
extern	void cdecl ClearBuffer( void );
extern	void cdecl InstallDrivers( void );

#endif

