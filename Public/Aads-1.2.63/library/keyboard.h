/*
 *	Standart ASCII code for any keyboard
 *
 *	- The CTRL key are only defined with the ASCII code
 *	- The ALT key are defined only with th scan code, cause their
 *	  ASCII code are always equal 0.
 */

#ifndef	__KEYBOARD__
#define __KEYBOARD__	__KEYBOARD__

#define	NO_KEY	0xFFFF

typedef struct	{
		  char	Lettre;
		  int	Touche;
		} Type_Raccourcie;

static
Type_Raccourcie	 CTRL_[]={
	{'A',0x01},
	{'Z',0x1A},
	{'E',0x05},
	{'R',0x12},
	{'T',0x14},
	{'Y',0x19},
	{'U',0x15},
	{'I',0x09},
	{'O',0x0F},
	{'P',0x10},
	{'Q',0x11},
	{'S',0x13},
	{'D',0x04},
	{'F',0x06},
	{'G',0x07},
	{'H',0x08},
	{'J',0x0A},
	{'K',0x0B},
	{'L',0x0C},
	{'M',0x0D},
	{'W',0x17},
	{'X',0x18},
	{'C',0x03},
	{'V',0x16},
	{'B',0x02},
	{'N',0x0E},
	{'0',0x10},
	{'1',0x11},
	{'2',0x00},
	{'3',0x13},
	{'4',0x14},
	{'5',0x15},
	{'6',0x1E},
	{'7',0x17},
	{'8',0x18},
	{'9',0x19}
			};

/*
 * ALT_xx defined 
 */
 
static 
Type_Raccourcie	 ALT_[]= {
	{'A',0x1000},
	{'Z',0x1100},
	{'E',0x1200},
	{'R',0x1300},
	{'T',0x1400},
	{'Y',0x1500},
	{'U',0x1600},
	{'I',0x1700},
	{'O',0x1800},
	{'P',0x1900},
	{'Q',0x1E00},
	{'S',0x1F00},
	{'D',0x2000},
	{'F',0x2100},
	{'G',0x2200},
	{'H',0x2300},
	{'J',0x2400},
	{'K',0x2500},
	{'L',0x2600},
	{'M',0x2700},
	{'W',0x2C00},
	{'X',0x2D00},
	{'C',0x2E00},
	{'V',0x2F00},
	{'B',0x3000},
	{'N',0x3100},
	{'0',0xFFFF}
			};

#endif
