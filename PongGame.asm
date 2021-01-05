LDA #$f8
STA $0300
LDA #$f9
STA $0301
LDA #$fa
STA $0302

JSR inicializarTablero
JSR inicializarPelota
JSR inicializarVelocidad

JSR imprimirPaddle

loopJuego:
	JSR leerMovimiento
	JSR ProyectarPosicion
	JSR VerificarColision
	JSR ActualizarPelota
	JSR imprimirPuntaje
	JSR delay
JMP loopJuego

imprimirPuntaje:
    LDA $200
    STA $508
RTS
inicializarTablero:
	LDA #$05
	LDX #$00
	_itlp:
		STA $0510,X
		INX
		CPX #$10
	BNE _itlp
	LDA #$01
	LDX #$00
	_itlp3:
		STA $0520,X
		INX
		INX
		INX
		INX
		INX
		INX
		INX
		INX
		INX
		INX
		INX
		INX
		INX
		INX
		INX
		STA $0510,X
		INX
		CPX #$d0
	BNE _itlp3
	STA $05ef
	LDA #$02
	LDX #$00
	_itlp2:
		STA $05f0,X
		INX
		CPX #$10
	BNE _itlp2
RTS

inicializarPelota:
LDA #00
STA $200
LDA #137
STA $0400 ; POSICION INICIAL

LDA #00
STA $0402 ; VERTICAL ARRIBA

LDA #00
STA $0401 ; HORIZONTAL IZQUIERDA

LDA #04 
LDX #137 ;  PINTAMOS 
STA $0500, X
RTS

ProyectarPosicion:
    LDA $0401
    CMP #00
    BEQ CalcularIzquierda
    JMP CalcularDerecha
    next:
    LDA $0402
    CMP #00
    BEQ CalcularArriba
    JMP CalcularAbajo
    next2:
    RTS

CalcularIzquierda:
    LDA $0400
    STA $0403
    DEC $0403
    JMP next
  
CalcularDerecha:
    LDA $0400
    STA $0403
    INC $0403
    JMP next

CalcularArriba:
    LDA $0403
    SEC
    SBC #16
    STA $0403
    JMP next2
  
CalcularAbajo:
	LDA #16
    CLC
    ADC $0403
    STA $0403
    JMP next2

VerificarColision:
    LDX $0403
    LDA $0500, X
    CMP #01
    BEQ ColisionaHorizontal
    LDX $0403
    LDA $0500, X
    CMP #02
    BEQ GameOver
    LDX $0403
    LDA $0500, X
    CMP #03
    BEQ ColisionPaddle
    LDX $0403
    LDA $0500, X
    CMP #05
    BEQ ColisionVertical
    RTS
    huboColision:
    JSR ProyectarPosicion
    JSR VerificarColision
RTS
GameOver:
	JSR fin

ColisionaHorizontal:
    LDA $0401
    EOR #01
    STA $0401
    JMP huboColision

ColisionVertical:
    LDA $0402
    EOR #01
    STA $0402
    JMP huboColision

ColisionPaddle:
    INC $0200
    JMP aumentarVelocidad
    _cpc:
    JMP ColisionVertical

aumentarVelocidad:
    DEC $0409
JMP _cpc

ActualizarPelota:
    ;Borrar posicion actual
    LDX $0400
    LDA #00
    STA $0500, X
    ;Pintar nueva posicion
    LDX $0403
    LDA #04
    STA $0500, X
    ;Actualizar posicion
    LDA $0403                    
    STA $0400
RTS

delay:
	LDX #$00
	_dl1:
		LDY #$00
		_dl2:
			INY
			CPY #$ff
		BNE _dl2
		INX
		CPX $0409
	BNE _dl1
RTS
inicializarVelocidad:
	LDA #6
	STA $0409
RTS

imprimirPaddle:
	LDA #3
	LDX $0300
	STA $500, X
	LDX $0301
	STA $500, X
	LDX $0302
	STA $500, X
RTS

leerMovimiento:
	LDA $ff
	; Teclas mayúsculas
	CMP #65
	BEQ Izquierda
	CMP #68
	BEQ Derecha
	_rlm2:
RTS

Derecha:
	JSR DerechaS
	JMP _rlm2

Izquierda:
	JSR IzquierdaS
	JMP _rlm2

IzquierdaS:
	LDA $0300
	CMP #$f0
	BEQ movimientoIlegal
	LDX $0302
	LDA #2
	STA $0500, X
	DEC $0300
	DEC $0301
	DEC $0302
	JSR imprimirPaddle
	LDA #0
	STA $ff
RTS

DerechaS:
	LDA $0302
	CMP #$ff
	BEQ movimientoIlegal
	LDX $0300
	LDA #2
	STA $0500, X
	INC $0300
	INC $0301
	INC $0302
	JSR imprimirPaddle
	LDA #0
	STA $ff
RTS

movimientoIlegal:
RTS

fin: