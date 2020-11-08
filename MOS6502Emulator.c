#include "flag_modes.h"
#include "structures.c"
#include "pipeline.h"
#include "instructions.h"
#include "extra_functions.h"

int main()
{
    // Inicializar memoria y CPU
    init();
    mem -> ram[RESET_MSB - 1] = 0x03;
    mem -> ram[RESET_LSB - 1] = 0x00;
    reset_cpu(); // Reseteamos la cpu
    int n = loadToMemory((uint16_t) 0x0300);

    printf("Antes de la ejecución\n");
    printf_registers();
    printf_memory();
    print_memory_report("MemoryBefore.txt");

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
    printf_memory();
    print_memory_report("MemoryAfter.txt");

    return 0;
}
