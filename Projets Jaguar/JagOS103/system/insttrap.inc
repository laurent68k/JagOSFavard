;	------------------------------------------------------------------------------
;	Project JAGUAR:	Trap system installation and global exceptions to error handler
;					and dummy RTE.
;
;	File:		InsTrap.inc
;	Author:		FAVARD Laurent
;	Date:		26 March 1998
;	Release:	03 Jully 1998
;	Language:	HISOFT DEVPAC 3-680x0
;
;	Funship - Atari Jaguar France / Copyright 1997-1998
;	------------------------------------------------------------------------------


                        XDEF		InstallTrap

; ---------------------------------------------------------------------------


						TEXT
                        EVEN

; ---------------------------------------------------------------------------
;	Vector Offset (Address)
; ---------------------------------------------------------------------------

ADR_BUS					set			$08
ADR_ADDRESS				set			$0C
ADR_ILLEGAL				set			$10
ADR_DIVIDE				set			$14
ADR_CHK					set			$18
ADR_TRAPV				set			$1C
ADR_PRIVILIEGE			set			$20
ADR_TRACE				set			$24
ADR_LINEA				set			$28
ADR_LINEF				set			$2C
ADR_RESERVED1			set			$30
ADR_COPRO				set			$34
ADR_FORMAT				set			$38
ADR_UNINITIALIZED		set			$3C

ADR_SPURIOUS			set			$60
AUTOVECTOR_1			set			$64
AUTOVECTOR_2			set			$68
AUTOVECTOR_3			set			$6C
AUTOVECTOR_4			set			$70
AUTOVECTOR_5			set			$74
AUTOVECTOR_6			set			$78
AUTOVECTOR_7			set			$7C

ADR_BASE_TRAP			set			$80

ADR_TRAP_0				set			$80
ADR_TRAP_1				set			$84
ADR_TRAP_2				set			$88
ADR_TRAP_3				set			$8C
ADR_TRAP_4				set			$90
ADR_TRAP_5				set			$94
ADR_TRAP_6				set			$98
ADR_TRAP_7				set			$9C
ADR_TRAP_8				set			$A0
ADR_TRAP_9				set			$A4
ADR_TRAP_10				set			$A8
ADR_TRAP_11				set			$AC
ADR_TRAP_12				set			$B0
ADR_TRAP_13				set			$B4
ADR_TRAP_14				set			$B8
ADR_TRAP_15				set			$BC

; ---------------------------------------------------------------------------
;	Vector number(s)
; ---------------------------------------------------------------------------


ERR_BUS					equ				2
ERR_ADDRESS				equ				3
ERR_ILLEGAL				equ				4
ERR_DIVIDE				equ				5
ERR_CHK					equ				6
ERR_TRAPV				equ				7
ERR_PRIVILEGE			equ				8
ERR_TRACE				equ				9
ERR_LINEA				equ				10
ERR_LINEF				equ				11
ERR_RESERVED1			equ				12
ERR_COPRO				equ				13
ERR_FORMAT				equ				14
ERR_UNTINIALIZED		equ				15

;	16 To 23 Are unassigned because RESERVED by MOTOROLA

ERR_SPURIOUS			equ				24
ERR_AUTO1				equ				25
ERR_AUTO2				equ				26
ERR_AUTO3				equ				27
ERR_AUTO4				equ				28
ERR_AUTO5				equ				29
ERR_AUTO6				equ				30
ERR_AUTO7				equ				31
ERR_TRAP0				equ				32
ERR_TRAP1				equ				33
ERR_TRAP2				equ				34
ERR_TRAP3				equ				35
ERR_TRAP4				equ				36
ERR_TRAP5				equ				37
ERR_TRAP6				equ				38
ERR_TRAP7				equ				39
ERR_TRAP8				equ				40
ERR_TRAP9				equ				41
ERR_TRAP10				equ				42
ERR_TRAP11				equ				43
ERR_TRAP12				equ				44
ERR_TRAP13				equ				45
ERR_TRAP14				equ				46
ERR_TRAP15				equ				47

ERR_USERIT				equ				64				; Number 64 to 255

; ---------------------------------------------------------------------------
;		void cdecl InstallTrap(void)
;
;		This function install all Trap handler system.
;
; ---------------------------------------------------------------------------

InstallTrap:			move.l		a0,-(sp)

;	JagOS Trap
						move.l		#JagOS,ADR_TRAP_0

;	Not used trap

						move.l		#Trap1,ADR_TRAP_1
						move.l		#Trap2,ADR_TRAP_2
						move.l		#Trap3,ADR_TRAP_3
						move.l		#Trap4,ADR_TRAP_4
						move.l		#Trap5,ADR_TRAP_5
						move.l		#Trap6,ADR_TRAP_6
						move.l		#Trap7,ADR_TRAP_7
						move.l		#Trap8,ADR_TRAP_8
						move.l		#Trap9,ADR_TRAP_9
						move.l		#Trap10,ADR_TRAP_10
						move.l		#Trap11,ADR_TRAP_11
						move.l		#Trap12,ADR_TRAP_12
						move.l		#Trap13,ADR_TRAP_13
						move.l		#Trap14,ADR_TRAP_14
						move.l		#Trap15,ADR_TRAP_15
						
;	Standards exceptions

						move.l		#BusError,ADR_BUS
						move.l		#AddressError,ADR_ADDRESS
						move.l		#Illegal,ADR_ILLEGAL
						move.l		#Divide,ADR_DIVIDE
						move.l		#CHKInst,ADR_CHK
						move.l		#TrapVVector,ADR_TRAPV
						move.l		#PrivilegeVector,ADR_PRIVILIEGE
						move.l		#TraceVector,ADR_TRACE
						move.l		#LineA,ADR_LINEA
						move.l		#LineF,ADR_LINEF
						move.l		#Reserved1,ADR_RESERVED1
						move.l		#Copro,ADR_COPRO
						move.l		#Format,ADR_FORMAT
						move.l		#Unitialized,ADR_UNINITIALIZED

						move.l		#Spurious,ADR_SPURIOUS

;	Auto vectors Interrupts

						move.l		#Auto_1,AUTOVECTOR_1
						move.l		#Auto_2,AUTOVECTOR_2
						move.l		#Auto_3,AUTOVECTOR_3
						move.l		#Auto_4,AUTOVECTOR_4
						move.l		#Auto_5,AUTOVECTOR_5
						move.l		#Auto_6,AUTOVECTOR_6
						move.l		#Auto_7,AUTOVECTOR_7

;	User interrupt vectors ($100-->$3FF) all show the same dummy vectors

						move.l		#$100,a0
.loop:					move.l		#UserInt,(a0)+
						cmpa.l		#$400,a0
						bne.s		.loop
						
						move.l		(sp)+,a0
						rts

; ---------------------------------------------------------------------------

						
BusError:				move.w			#ERR_BUS,-(sp)
						move.w			#84,SizeExcepData
						bra				ProcessError2
									
AddressError:			move.w			#ERR_ADDRESS,-(sp)
						move.w			#84,SizeExcepData
						bra				ProcessError2
						
Illegal:				move.w			#ERR_ILLEGAL,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2
			
Divide:					move.w			#ERR_DIVIDE,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2
						
CHKInst:				move.w			#ERR_CHK,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2		

TrapVVector:			move.w			#ERR_TRAPV,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2	
		
PrivilegeVector:		move.w			#ERR_PRIVILEGE,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2
		
TraceVector:			move.w			#ERR_TRACE,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2
			
LineA:					move.w			#ERR_LINEA,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2			

LineF:					move.w			#ERR_LINEF,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2		
	
Reserved1:				move.w			#ERR_RESERVED1,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2	
		
Copro:					move.w			#ERR_COPRO,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2
		
Format:					move.w			#ERR_FORMAT,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2	
		
Unitialized:			move.w			#ERR_UNTINIALIZED,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2
		
Spurious:				move.w			#ERR_SPURIOUS,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2	
		
Auto_1:					move.w			#ERR_AUTO1,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2	

Auto_2:					move.w			#ERR_AUTO2,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2	

Auto_3:					move.w			#ERR_AUTO3,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2	

Auto_4:					move.w			#ERR_AUTO4,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2	

Auto_5:					move.w			#ERR_AUTO5,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2	

Auto_6:					move.w			#ERR_AUTO6,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2	

Auto_7:					move.w			#ERR_AUTO7,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2	

Trap1:					move.w			#ERR_TRAP1,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2
		
Trap2:					move.w			#ERR_TRAP2,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2
			
Trap3:					move.w			#ERR_TRAP3,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2
		
Trap4:					move.w			#ERR_TRAP4,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2
			
Trap5:					move.w			#ERR_TRAP5,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2
			
Trap6:					move.w			#ERR_TRAP6,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2
			
Trap7:					move.w			#ERR_TRAP7,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2
			
Trap8:					move.w			#ERR_TRAP8,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2
			
Trap9:					move.w			#ERR_TRAP9,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2
			
Trap10:					move.w			#ERR_TRAP10,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2
			
Trap11:					move.w			#ERR_TRAP11,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2
			
Trap12:					move.w			#ERR_TRAP12,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2
			
Trap13:					move.w			#ERR_TRAP13,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2
			
Trap14:					move.w			#ERR_TRAP14,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2			

Trap15:					move.w			#ERR_TRAP15,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2

UserInt:				move.w			#ERR_USERIT,-(sp)
						move.w			#76,SizeExcepData
						bra				ProcessError2	

; ---------------------------------------------------------------------------

ProcessError:     		
						move.w			#DBMSG,-(sp)
						trap			#SYSTEM
                        addq.l			#6,sp

						pea.l			ERR_FATAL_REBOOT
						move.w			#DBMSG,-(sp)
						trap			#SYSTEM
                        addq.l			#6,sp
						
.ForEver:				bra.s			.ForEver
						RTE

; ---------------------------------------------------------------------------

ProcessError2:     		
						movem.l			d0-d7/a0-a6,-(sp)							;	Save all registers
						move.l			sp,-(sp)									;	Push the SSP
						move.l			usp,a0										
						move.l			a0,-(sp)									;	Push the USP
						
						move.l			sp,-(sp)									;	Push address of CPU informations
                        move.w			SizeExcepData,-(sp) 						;	Lenght to send 
                        move.w			#FRAME_EXCEPTION,-(sp)						;	Frame's CODE Exception
                        move.l			#TOOLBOX_PID,-(sp)							;	Target PID
                        move.l			#KERNEL_PID,-(sp)							;	Source PID is the kernel itself
		   				jsr				SendFrame
                        add.l			#16,sp

						pea.l			ERR_FATAL_REBOOT
						move.w			#DBMSG,-(sp)
						trap			#SYSTEM
                        addq.l			#6,sp
						
.ForEver:				bra.s			.ForEver
						RTE
						
; ---------------------------------------------------------------------------

						DATA
						EVEN



ERR_FATAL_REBOOT:		dc.b			"SYSTEM HALTED ! TRY TO RE-START",0
						