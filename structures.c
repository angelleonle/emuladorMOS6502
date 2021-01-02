#include <inttypes.h>

// Funciones que van a operar los elementos del procesador
// Registros: enteros sin signo
typedef struct mos6502
{
    uint8_t a;// 8bits-Acumulador
    uint8_t x;// Index x
    uint8_t y;// Index y
    uint16_t pc;// Program Counter
    uint8_t sp;// Stack Pointer
    uint8_t sr;// Status Register - Flag Register
    uint8_t ins;// Instruction register
    uint16_t addr;// Address register
} mos6502;

typedef struct memory
{
    uint8_t *ram;
} memory;

/**** VARIABLES GLOBALES DEL 6502 ****/
mos6502 *cpu;
memory *mem;

