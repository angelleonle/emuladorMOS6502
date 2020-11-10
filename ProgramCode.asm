LDA #$05
LDX #$00
_loop:
STA $0400,X
INX 
CPX #$4F
BNE _loop 
