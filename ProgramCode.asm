LDX #$00
INX 
INY 
CMP #$02
BNE _loop 
LDA #$05
_loop:
