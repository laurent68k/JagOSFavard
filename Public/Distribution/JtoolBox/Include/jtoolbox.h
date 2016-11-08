/*
 * ----------------------------------------------------------------------------------------------
 *		JAGUAR TOOLBOX APPLICATION PROGRAM INTERFACE
 *
 *
 *	File:		JToolBox.h
 *	Author:		FAVARD LAURENT
 *	Date:		25 March 1998
 *	Release:	11 May 2000
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
 *
 * ----------------------------------------------------------------------------------------------
 */

#ifndef		__JtoolboxH
#define		__JtoolboxH

/*	---------------------------------------------------------------------------*/

#define		true				1
#define		false				0

/*	---------------------------------------------------------------------------*/

#define		JTB_OK				 1
#define		JTB_ERROR			 0
#define		JTB_EFRAMEMISMATCH	-1
#define		JTB_ESTARTADR		-2
#define		JTB_ETARGETADR		-3
#define		JTB_ELENGHT			-4
#define		JTB_ETIMEOUT		-5
#define		JTB_EBADVALUE		-6
#define		JTB_ENOHEADER		-7
#define		JTB_EFILE			-8
#define		JTB_ENOFRAME		-9
#define		JTB_ECHKSUM			-10
#define		JTB_EPUTCHAR		-11
#define		JTB_EGETCHAR		-12
#define		JTB_ENOCRYFILE	 	-13
#define		JTB_ENOCCIFILE	 	-14
#define		JTB_EMEM			-15
#define		JTB_ETRANSFER		-16
#define		JTB_EBUSYLOW		-17
#define		JTB_EBUSYHIGH		-18

#define		JTB_DEVFREE			1
#define		JTB_DEVBUSY			0

/*	---------------------------------------------------------------------------*/

#define		FRAME_NACK			'N'
#define		FRAME_ACK			'A'
#define		FRAME_DATA			'D'

#define		FRAME_INQUIRE		0x0001								/*	ask the jaguar's identification */
#define		FRAME_PRGSEND		0x0002								/*	send a program to the jaguar */
#define		FRAME_MEMSEND		0x0003								/*	send a memory block to the jaguar */
#define		FRAME_MEMGET		0x0004								/*	get a memory block from the jaguar */
#define		FRAME_DBMSG			0x0005								/*	jaguar send a Ascii string to the host */
#define		FRAME_PRGRUN		0x0006								/*	ask to execute a program */
#define		FRAME_EEPROMGET		0x0007								/*	Ask to the jaguar to send its serial eeprom */
#define		FRAME_EEPROMSET		0x0008								/*	Ask to the jaguar the write its serial eeprom */
#define		FRAME_EXCEPTION		0x0009								/*	When an exception 680x0 occur */
#define		FRAME_USER1			0x000A								/*	User1 frame code */


/*	---------------------------------------------------------------------------*/

#define		KERNEL_PID				(long)0x4A4A4A4AL
#define		TOOLBOX_PID				(long)0x48484848L
#define		PROGRAM_NAME_LEN		20
#define		MAX_DATA_LEN			32767							/*	MAXIMAL data block received for GetFrame() */
#define		SIZE_SERIAL_EEPROM		128
#define		LOAD_AND_GO				3
#define		SIZE_H_LDGOW			18
#define		SIZE_H_LDGOL			20
#define		SIZE_HEADER_XJAG		22
#define		HEADER_JAGR				"JAGR"							/*	Roine header	*/
#define		HEADER_JAGL				"JAGL"							/*	Roine header	*/
#define		HEADER_XJAG				"XJAG"							/*	Favard header	*/

#define		PBOARD					0
#define		PSERIAL					1
#define		PCENTRONICS				2

#define		JTB_COUNTMSG			20
#define		JTB_LENMSG				50

/*	---------------------------------------------------------------------------*/

typedef	struct	{
					char	Version[ 50 ];
					char	Date[ 50 ];
                    char	Author[ 50 ];
				}TJTBVersion;

/*	Frame header */
typedef	struct	{
					long			SPID;
					long			TPID;
					int				Opcode;
					unsigned int	Length;
					int				CheckSum;
				} THeader;

/*	Generic type for receive a frame */
typedef	struct	{
					char	Table[ MAX_DATA_LEN ];
				} TRecipient;

/*	Type for receive a jaguar identification */
typedef	struct	{
					long	Mark;
                    int     _SystemVersion;
                    int		_HardwareVersion;
                    long    _SystemDate;
                    long    _HardwareDate;
				} TJagId;

/*	Type for jaguar memory descriptor */
typedef	struct	{
					void 			*StartAddress;
                    unsigned long	Lenght;
				} TJagMemDesc;

/*	Type for jaguar program descriptor */
typedef	struct	{
					void 			*TargetAddress;
					void 			*StartAddress;
					unsigned long	Lenght;
					char			Name[PROGRAM_NAME_LEN];
				} TJagProgram;

/*	Type for jaguar file program */
typedef	struct	{
					int					CodeOp;
					long		 		Header;
					void				*StartAdr;
					long				Length;
					void				*StartCode;
					unsigned long		OffsetStart;
					char				Name[PROGRAM_NAME_LEN];
					int					Type;
				} TFileJag;

/*	Type for an exception 680x0 */
typedef	struct	{	
					unsigned long		usp;
					unsigned long		ssp;
					unsigned long		d0;
					unsigned long		d1;
					unsigned long		d2;
					unsigned long	 	d3;
					unsigned long		d4;
					unsigned long		d5;
					unsigned long		d6;
					unsigned long		d7;
					unsigned long		a0;
					unsigned long		a1;
					unsigned long		a2;
					unsigned long		a3;
					unsigned long		a4;
					unsigned long		a5;
					unsigned long		a6;
					unsigned int		ExceptionNumber;
					unsigned long		stack_pc;				/*	PC pushed into the stack */
					unsigned int		stack_sr;				/*	PC pushed into the stack */
					unsigned int		ri;						/*	Instr. Register (Bus error, address error) */
					unsigned long		fault_pc;				/*	fault address responsable */
					unsigned int		control;				/*	R/W, FC0-FC1, I/N state */
				} TException;

/*	---------------------------------------------------------------------------*/

extern	signed int SendBinaries(long SourcePID, long TargetPID, unsigned long Lenght, void *Buffer);
extern	signed int GetBinaries(long SourcePID, long TargetPID, unsigned long Lenght, void *Buffer);
extern	signed int SendFrame(long SourcePID, long TargetPID, int Code, unsigned int Lenght, void *Buffer);
extern	signed int GetFrame(long *SourcePID, long *TargetPID, int *Code, unsigned int *Lenght, void *Buffer);
extern	signed int SendPack(void *Buffer, unsigned int Lenght);
extern	unsigned int GetPack(void *Buffer, unsigned int Lenght);

/*	---------------------------------------------------------------------------*/

extern	void JTB_Version( TJTBVersion *Version );
extern	int JTB_Open( int UseSemaphore );
extern	int JTB_Exit( void );
extern	void JTB_ErrorText( int ErrorCode, char *ErrorText );
extern	void JTB_Debug( int OnOff );
extern	int JTB_DevStatus( void );

extern	int GetPort(void);
extern	void SetPort(int Port);

extern	void HookJaguarMessage( void (*UserFonction)(	int FrameCode, long SourcePID,
														long TargetPID, int LenghtDatas,
                                                        TRecipient *Recipient)
								);

extern	int Inquire( TJagId *JagId );

extern	int MemoryGet(void *STAddress, TJagMemDesc *JagMemDescriptor);
extern	int MemorySend(void *STAddress, TJagMemDesc *JagMemDescriptor);

extern	int MemGet(int Accept, void *STAddress, TJagMemDesc *JagMemDescriptor);
extern	int MemSend(int Accept, void *STAddress, TJagMemDesc *JagMemDescriptor);

extern	int LoadAndGo(void *STAddress, TJagProgram *JagProgram);
extern	int RunPRG(TJagProgram *JagProgram);

extern	int JagToDisk(char *chFileName, TJagMemDesc *JagMem);
extern	int DiskToJag(	char *PathName,
						unsigned long OffsetFromStart,
						TJagMemDesc		*JagMemoire,
						void (*UserFonctionJauge)(unsigned long CountToDo, unsigned long CountPerfomed));

extern	signed int IsJAGR(char *PathName, TFileJag *FileJaguar);
extern	signed int IsJAGL(char *PathName, TFileJag *FileJaguar);

extern	int GetEEPROM( char *chEEPROM );
extern	int SetEEPROM( char *chEEPROM );

/*
 * ----------------------------------------------------------------------------------------------
 *					Misc. procedures
 */

extern	int PackCCI(char *FileName, char *Destination);

#endif

