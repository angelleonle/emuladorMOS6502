#include "flags_modes.c"
#include "estructuras.c"
#include "pipeline_6502.h"
#include "instructions.h"
#include "extra_functions.h"

int main()
{
    // Inicializar memoria y CPU
    init();
    mem -> ram[RESET_MSB - 1] = 0x03;
    mem -> ram[RESET_LSB - 1] = 0x00;
    printf("Antes de la ejecución\n");
    reset_cpu(); // Reseteamos la cpu

    printf_registers();
    int n = loadToMemory((uint16_t) 0x0300);
    while(((cpu -> pc) - 0x0300) != n)
    {
        fetch();
        decode();
        execute();
    }
    set_flag(FLAG_B, 1);
    set_flag(0b00100000, 1);
    printf("Después de la ejecución\n");
    printf_registers();
    print_memory(0x0300, 3);
    print_memory(0x0400, 1);
    return 0;
}
