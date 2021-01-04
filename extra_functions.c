#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include "extra_functions.h"
#include "flag_modes.h"
#include "structures.c"
void set_flag(uint8_t flag, uint8_t status)
{
    if (!status){
        uint8_t flagc = ~flag;
        cpu -> sr = cpu -> sr & flagc;
    }else
        cpu -> sr = cpu -> sr | flag;
}

void flagZ(uint8_t result)
{
    if (result == 0)
        set_flag(FLAG_Z, 1);
    else
        set_flag(FLAG_Z, 0);

}

void flagN(uint8_t result)
{
    if ((result & 0b10000000) == 0b10000000)
        set_flag(FLAG_N, 1);
    else
        set_flag(FLAG_N, 0);

}

void flagC(uint16_t result)
{
    if ( result >> 8 ) set_flag(FLAG_C, 1);
    else set_flag(FLAG_C, 0);
}

void flagV(uint8_t operand1, uint8_t operand2, uint16_t result)
{
    if( (result ^ operand1) & (result ^ operand2) & 0x80 ) set_flag(FLAG_V, 1);
    else set_flag(FLAG_V, 0);
}

void print_caracter(char c, int n)
{
    for (int i = 0; i < n; i++){
        printf("%c", c);
    }
    printf("\n");
}

void printf_binary(int n, int i)
{
    i--;
    while (i >= 0){
        if (n & (((long int) 1) << i)){
            printf("1");
        }else{
            printf("0");
        }
        if(i % 4 == 0){
            printf(" ");
        }
        i--;
    }
    printf("\n");
}
void printf_registers()
{
    print_caracter('-', 33);
    printf("       REGISTROS DE LA CPU       \n");
    print_caracter('-', 33);
    printf("REG   HEX     BIN\n");
    print_caracter('-', 33);
    printf("A:    %04x    ", (int) cpu -> a);
    printf_binary(cpu -> a, 8);
    printf("X:    %04x    ", (int) cpu -> x);
    printf_binary(cpu -> x, 8);
    printf("Y:    %04x    ", (int )cpu -> y);
    printf_binary(cpu -> y, 8);
    printf("SP:   %04x    ", (int)cpu -> sp);
    printf_binary(cpu -> sp, 8);
    printf("SR:   %04x    ", (int) cpu -> sr);
    printf_binary(cpu -> sr, 8);
    printf("PC:   %04x    ", (int) cpu -> pc);
    printf_binary(cpu -> pc, 16);
    print_caracter('-', 33);
    printf("IR:   %04x    ", (int) cpu -> ins);
    printf_binary(cpu -> ins, 8);
    printf("AR:   %04x    ", (int) cpu -> addr);
    printf_binary(cpu -> addr, 16);
    print_caracter('-', 33);
}

int loadToMemory(uint16_t init_pc, const char *myFile)
{
    FILE *hexdump = fopen(myFile,"r");
    if(hexdump == NULL)
    {
        printf("Something went wrong\n");
        exit(1);
    }
    
    int i = 0;
    unsigned int aux;
    while(1)
    {
        fscanf(hexdump, "%x", &aux);
        if (feof(hexdump))
        {
            break;
        }
        mem -> ram[init_pc + i] = (uint8_t) aux;
        i++;
    }
    fclose(hexdump);
    return i;
}

void init()
{
    cpu = (mos6502 *) malloc(sizeof(mos6502));
    mem = (memory *) malloc(sizeof(memory));
    mem -> ram = (uint8_t *) malloc(sizeof(uint8_t) * 256 * 256); // 256 paginas x 256 palabras
}

// Ni bien se enciende el emulador, hace un reset
void reset_cpu()
{
    uint8_t pch = mem -> ram[RESET_MSB - 1];
    uint8_t pcl = mem -> ram[RESET_LSB - 1];
    // PCH -> 0000 0001
    // PCL -> 0000 0001
    // PC -> 0000 0001 0000 0001
    cpu -> a = 0;
    cpu -> x = 0;
    cpu -> y = 0;
    cpu -> pc = (pch * 256) + pcl;
    cpu -> sp = (uint8_t) 0xFF;
    cpu -> sr = 0;
    cpu -> ins = 0;
    cpu -> addr = 0;
}


void print_memory_report(char *name){
    FILE *myfile = fopen(name, "w");
    if(myfile == NULL)
    {
        printf("Something went wrong\n");
        exit(1);
    }
    for(int i = 0x0000; i <= 0xFFFF; i++)
    {
        if(i % 16 == 0)
        {
            fprintf(myfile, "0x%04x: ", (int) i);
        }
        fprintf(myfile, "%02x ", (int) mem -> ram[i]);
        if((i + 1) % 16 == 0 && i != 0X0000)
        {
            fprintf(myfile, "\n");
        }
    }
}


void print_memory(uint16_t inicio, int n){
    for(int i = inicio; i < inicio + n * 16; i++)
    {
        if(i % 16 == 0)
        {
            printf("0x%04x: ", (int) i);
        }
        printf("%02x ", (int) mem -> ram[i]);
        if((i + 1) % 16 == 0 && i != inicio)
        {
            printf("\n");
        }
    }
}
void printf_memory()
{
    print_caracter('-', 55);
    printf("                       MEMORIA\n");
    print_caracter('-', 55);
    printf("Página Cero (Registers)\n");
    print_memory(0x0000, 4);
/*    printf("Página Uno (Instructions)\n");
     print_memory(0x0100, 16);*/
    printf("Página Tres (Storage)\n");
    print_memory(0x0300, 6);
    print_caracter('-', 55);
    printf("Página Cinco (Board)\n");
    print_memory(0x0500, 16);
    print_caracter('-', 55);
    printf("Página Seis (Instructions)\n");
    print_memory(0x0600, 6);
    print_caracter('-', 55);
}

void generateRandom()
{
	int number;
	number = 17 + rand() % 222;
	if(number % 16 == 0)
	{
		number++;
	}
	if((number + 1) % 16 == 0)
	{
		number--;
	}
	mem -> ram[0xfe] = number;
}

int leerTecla()
{
    int ch = getch();
    if (ch == 'e' || ch == 'E')
    {
        return 1;
    }
    else if (ch == 'w' || ch == 'w')
    {
        mem -> ram[0xff] = (uint8_t) 'W';
    }
    else if (ch == 'd' || ch == 'D')
    {
        mem -> ram[0xff] = (uint8_t) 'D';
    }
    else if (ch == 's' || ch == 'S')
    {
        mem -> ram[0xff] = (uint8_t) 'S';
    }
    else if (ch == 'a' || ch == 'A')
    {
        mem -> ram[0xff] = (uint8_t) 'A';
    }
    return 0;
}