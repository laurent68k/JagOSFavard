
#define		PSG_PORTB_WR		0x8F
#define		PSG_PORTB_RD		0x0F

#define		STROBE_HIGH			0x20
#define		STROBE_LOW			0xDF

/*
 * ----------------------------------------------------------------------------------------------
 */

extern	void cdecl _initC(void);
extern	unsigned int cdecl _getbusyC(void);
