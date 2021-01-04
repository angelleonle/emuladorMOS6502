#include <inttypes.h>
#include <stdio.h>

#ifndef EMULADOR_6502_FLAGS_FUNCTIONS_H
#define EMULADOR_6502_FLAGS_FUNCTIONS_H

/**** PROTOTIPOS DE FUNCIONES PARA LAS BANDERAS ****/
void set_flag(uint8_t, uint8_t);
void flagZ(uint8_t);
void flagI(uint8_t);
void flagN(uint8_t);
void flagB(uint8_t);
void flagC(uint16_t);
void flagD(uint8_t);
void flagV(uint8_t,uint8_t,uint16_t);

/**** PROTOTIPOS DE FUNCIONES AUXILIARES ****/
void printf_binary(int, int);
void print_Registros();
void print_caracter(char, int);
void printf_registers();
int loadToMemory(uint16_t, const char *);
void init();
void reset_cpu();
void print_memory_report(char *);
void print_memory(uint16_t, int);
void printf_memory();

#endif //EMULADOR_6502_FLAGS_FUNCTIONS_H
