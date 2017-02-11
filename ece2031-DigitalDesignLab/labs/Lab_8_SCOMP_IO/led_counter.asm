          ORG     &H000
Start:    IN      SWITCHES
          STORE   INDATA
Display:  LOAD    INDATA
          OUT     LEDS
          OUT     SEVENSEG
          SHIFT   ONE
          STORE   INDATA
          LOAD    ZERO
          OUT     TIMER
Check:    IN      TIMER
          SUB     TWENTY
          JNEG    Check
          JUMP    Display
          ORG     &H040
INDATA:   DW      &H0000
ZERO:     DW      &H0000
TWENTY:   DW      &H0014
ONE:      EQU     &H0001
SWITCHES: EQU &H00
LEDS:     EQU &H01
TIMER:    EQU &H02
SEVENSEG: EQU &H04

