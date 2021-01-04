#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include "flag_modes.h"
#include "structures.c"
#include "pipeline.h"
#include "instructions.h"
#include "extra_functions.h"
#define MAX_ROWS 16
#define MAX_COLUMNS 16

int leerTecla();
void generateRandom();
void printGameSpace();
void delay(int);
int main()
{
    // Inicializar semilla
	srand(time(NULL));
    // Inicializar memoria y CPU
    init();
    mem -> ram[RESET_MSB - 1] = 0x06;
    mem -> ram[RESET_LSB - 1] = 0x00;
    reset_cpu();
    // Reiniciar CPU
    int n = loadToMemory((uint16_t) 0x0600);
    // Inicializar interface
    initscr();
    keypad(stdscr,1);
    cbreak();
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_CYAN);
    init_pair(2, COLOR_GREEN, COLOR_GREEN);
    init_pair(3, COLOR_RED, COLOR_RED);
    init_pair(4, COLOR_BLUE, COLOR_BLUE);
    attron(COLOR_PAIR(1));

    timeout(0.1);
    while(((cpu -> pc) - 0x0600) != n)
    {
        // Leer tecla
        if(leerTecla())
        {
            endwin();
            break;
        }
        // Generar número aleatorio
        generateRandom();
        // Pipeline
        fetch();
        decode();
        execute();
        // Salida a pantalla
        printGameSpace();
    }
    delay(100);
    endwin();
    printf("Game Over\n");
    set_flag(FLAG_B, 1);
    set_flag(0b00100000, 1);
    return 0;
}

void delay(int ms) 
{
    int us = ms * 1000;
    clock_t start_time = clock();
    while (clock() < start_time + us);
}

void printGameSpace()
{
    werase(stdscr);
    for(int i = 0; i < MAX_ROWS; i++)
    {
        for (int j = 0; j < MAX_COLUMNS; j++)
        {
            if (((mem -> ram)[0x500 + i * MAX_COLUMNS + j]) == 1)
            {
                attron(COLOR_PAIR(2));
                printw("11 ");
                attron(COLOR_PAIR(1));
            }
            else if (((mem -> ram)[0x500 + i * MAX_COLUMNS + j]) == 2)
            {
                attron(COLOR_PAIR(3));
                printw("22 ");
                attron(COLOR_PAIR(1));
            }
            else if (((mem -> ram)[0x500 + i * MAX_COLUMNS + j]) == 3)
            {
                attron(COLOR_PAIR(4));
                printw("33 ");
                attron(COLOR_PAIR(1));
            }
            else
            {
                attron(COLOR_PAIR(1));
                printw("00 ");
            }
        }
        printw("\n");
    }
    refresh();
}