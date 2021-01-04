; INICIALIZAR
JSR plantillaTablero
JSR limpiarTablero
JSR inicializarCola
JSR inicializarSerpiente
JSR dibujarSerpiente
JSR generarNuevaManzana
JSR dibujarManzana


loopJuego:
	JSR revisarTamanoMaximo
	JSR leerMovimiento
	JSR revisarColision
	JSR moverSnake
	JSR limpiarTablero
	JSR dibujarSerpiente
	JSR dibujarManzana
	JSR delay
JMP loopJuego

; -----------------------
;  FUNCIONES INICIALIZAR
; -----------------------
plantillaTablero:
	LDA #$01
	LDX #$00
	_itlp:
		STA $0900,X
		INX
		CPX #$10
	BNE _itlp
	LDX #$00
	_itlp3:
		STA $0910,X
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
		STA $0910,X
		INX
		CPX #$e0
	BNE _itlp3
	LDX #$00
	_itlp2:
		STA $09f0,X
		INX
		CPX #$10
	BNE _itlp2
RTS

limpiarTablero:
	LDX #00
	_lt:
	LDA $0900, X
	STA $0500, X
	INX
	CPX #$ff
	BNE _lt
	LDA #1
	STA $05ff
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
		CPX #$04
	BNE _dl1
RTS

; -----------------------
;  FUNCIONES SERPIENTE
; -----------------------
inicializarSerpiente:
	LDA #17
	STA $0351
	JSR encolar
	LDA #18
	STA $0351
	JSR encolar
	LDA #19
	STA $0351
	JSR encolar
	LDA #68
	STA $0353
RTS

dibujarSerpiente:
	LDA #$02
	LDX $0352
	_ds:
		LDY $02FF,X
		STA $0500,Y
		DEX
		CPX #$00
	BNE _ds
RTS

; -----------------------
;  FUNCIONES DE MANZANA
; -----------------------
generarNuevaManzana:
	LDA $fe
	STA $0354
RTS

borrarManzana:
	LDA #0
	LDX $0354
	STA $0500, X
RTS

dibujarManzana:
	LDA #3
	LDX $0354
	STA $0500, X
RTS

; -----------------------
;     FUNCIONES COLA
; -----------------------
desencolar:
	LDA $0300
	STA $0350
	LDX #$00
	DEC $0352
	_dclr:
		INX
		LDA $0300, X
		STA $02ff, X
		CPX $0352
	BNE _dclr
RTS

encolar:
    LDA $0351
    LDX $0352
    STA $0300, X
    INC $0352
RTS

inicializarCola:
	LDA #$00
	STA $0352, X
RTS

; -----------------------
; FUNCIONES MOVIMIENTO
; -----------------------
leerMovimiento:
	LDA $ff
	; Teclas mayúsculas
	CMP #87
	BEQ  Arriba
	CMP #65
	BEQ  Izquierda
	CMP #83
	BEQ  Abajo
	CMP #68
	BEQ  Derecha
	_rlm2:
	LDA $0353
	CMP #87
	BEQ  irArriba
	CMP #65
	BEQ  irIzquierda
	CMP #83
	BEQ  irAbajo
	CMP #68
	BEQ  irDerecha
	_rlm:
RTS

Derecha:
	JSR DerechaS
	JMP _rlm2

Arriba:
	JSR ArribaS
	JMP _rlm2

Izquierda:
	JSR IzquierdaS
	JMP _rlm2

Abajo:
	JSR AbajoS
	JMP _rlm2

irDerecha:
	JSR irDerechaS
	JMP _rlm

irArriba:
	JSR irArribaS
	JMP _rlm

irIzquierda:
	JSR irIzquierdaS
	JMP _rlm

irAbajo:
	JSR irAbajoS
	JMP _rlm

ArribaS:
	LDA $0353
	CMP #83
	BEQ movimientoIlegal
	LDA #87
	STA $0353
RTS

AbajoS:
	LDA $0353
	CMP #87
	BEQ movimientoIlegal
	LDA #83
	STA $0353
RTS

IzquierdaS:
	LDA $0353
	CMP #68
	BEQ movimientoIlegal
	LDA #65
	STA $0353
RTS

DerechaS:
	LDA $0353
	CMP #65
	BEQ movimientoIlegal
	LDA #68
	STA $0353
RTS

movimientoIlegal:
RTS

moverSnake:
  	LDA $0356
  	CMP #1
  	BEQ comioManzana
	LDA $0355
	STA $0351
	JSR encolar
	JSR desencolar
RTS

comioManzana:
RTS

irArribaS:
	LDY $0352
	LDX $02ff, Y
	STX $0355
	SEC
	LDA $0355
	SBC #$10
	STA $355
RTS

irAbajoS:
	LDY $0352
	LDX $02ff, Y
	STX $0355
	CLC
	LDA #$10
	ADC $0355
	STA $0355
RTS

irIzquierdaS:
	LDY $0352
	LDX $02ff, Y
	STX $0355
	SEC
	LDA $0355
	SBC #$1
	STA $0355
RTS

irDerechaS:
	LDY $0352
	LDX $02ff, Y
	STX $0355
	CLC
	LDA #$01
	ADC $0355
	STA $0355
RTS

; -----------------------
; FUNCIONES COLISIÓN
; -----------------------
revisarColision:
  JSR revisarAppleColision
  JSR revisarSnakeColision
  RTS

revisarSnakeColision:
	LDX $0355
	LDA $0500, X
	CMP #$01
	BEQ gameOver
	LDY #0
	DEC $0352
	_rsc:
		LDA $0300, Y
	    	CMP $0355
    		BEQ gameOver
	    	INY
    		CPY $0352
	BNE _rsc
  	INC $0352
RTS

revisarAppleColision:
	LDA #00
	STA $0356
	LDA $0354
	CMP $0355
	BEQ seComio
	_rac:
RTS

seComio:
	LDA #01
	STA $0356
	LDA $0354
  	STA $0351
	JSR encolar
	JSR generarNuevaManzana
JMP _rac

revisarTamanoMaximo:
	LDA $0352
	CMP #50
	BEQ gameOver
RTS

gameOver: