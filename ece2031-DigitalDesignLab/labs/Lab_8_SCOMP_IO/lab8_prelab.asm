        ORG     0
Start:  CALL    CALC
        JUMP    Start    ;Return to the beginning
        ORG     &H010
CALC:   LOAD    A
        AND     B
        XOR     C
        RETURN
        ORG     &H030
A:      DW      &H00FF
B:      DW      &HA5A5
C:      DW      &H3300
D:      DW      &H0ACE

