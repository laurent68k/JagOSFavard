;
;	Macros definitions
;	
;

RSCOUNT         SET 0

RSB             MACRO 							;label,count
\1              EQU RSCOUNT
RSCOUNT         SET RSCOUNT+\2
                ENDM

RSW             MACRO 							;label,count
\1              EQU RSCOUNT
RSCOUNT         SET RSCOUNT+\2*2
                ENDM

RSL             MACRO 							;label,count
\1              EQU RSCOUNT
RSCOUNT         SET RSCOUNT+\2*4
                ENDM

RSP             MACRO 							;label,count
\1              EQU RSCOUNT
RSCOUNT         SET RSCOUNT+\2*8
                ENDM

RSINIT          MACRO							;start address
RSCOUNT         SET \1
                ENDM
