/* CONSTANTES SIMBOLICAS DE LAS BANDERAS */
// Carry flag
#define FLAG_N 0b10000000
// Overflow flag
#define FLAG_V 0b01000000
// Ignored (bit 5)
// Break Command
#define FLAG_B 0b00010000
// Decimal Mode flag
#define FLAG_D 0b00001000
// Interrupt flag
#define FLAG_I 0b00000100
// Zero flag
#define FLAG_Z 0b00000010
// Carry flag
#define FLAG_C 0b00000001

/* MODOS DE DIRECCIONAMIENTO */
// Modo acumulador
#define M_ACUMULADOR 0
// Modo relativo
#define M_RELATIVO 1
// Modo inmediato
#define M_INMEDIATO 2
// Modo absoluto
#define M_ABSOLUTO 3
// Modo implícito
#define M_IMPLICITO 4
// Modo página cero
#define M_PCERO 5
// Modo absoluto indirecto
#define M_INDIRECTO 6
// Modo absoluto con índice X
#define M_INDICEX 7
// Modo absoluto con índice Y
#define M_INDICEY 8
// Modo página cero con índice X
#define M_PCEROX 9
// Modo página cero con índice Y
#define M_PCEROY 10
// Modo indirecto indexado
#define M_INDINDEX 11
// Modo indexado indirecto
#define M_INDEXINDI 12

// Byte mas significativo
#define RESET_MSB 0xfffd
// Byte menos significativo
#define RESET_LSB 0xfffc