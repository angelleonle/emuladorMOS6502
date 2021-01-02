#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>



void delay(int ms) 
{
    int us = ms * 1000;
    clock_t start_time = clock();
    while (clock() < start_time + us);
}

void printGameSpace(char *gamespace)
{
    werase(stdscr);
    for(int i = 0; i < MAX_ROWS; i++)
    {
        for (int j = 0; j < MAX_COLUMNS; j++)
        {
            if (gamespace[i * MAX_COLUMNS + j] == 1)
            {
                attron(COLOR_PAIR(2));
                printw("%c  ", gamespace[i * MAX_COLUMNS + j]);
                attron(COLOR_PAIR(1));
            }
            else if (gamespace[i * MAX_COLUMNS + j] == 2)
            {
                printw("%c  ", gamespace[i * MAX_COLUMNS + j]);
            }
            else if (gamespace[i * MAX_COLUMNS + j] == 3)
            {
                printw("%c  ", gamespace[i * MAX_COLUMNS + j]);
            }
            else
            {
                printw("%c  ", gamespace[i * MAX_COLUMNS + j]);
            }
        }
        printw("\n");
    }
    refresh();
}

int main(int argc, char *argv[])
{
    initscr();
    keypad(stdscr,1);
    cbreak();
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_GREEN);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_BLACK, COLOR_BLACK);

    attron(COLOR_PAIR(1));
    printGameSpace(gamespace);

    timeout(500);
    while(1)
    {
        int ch = getch();
        if (ch == 'e')
        {
            endwin();
            break;
        }
        else if (ch == 'w')
        {
            SNAKE_ORIENTATION = TOP;
        }
        else if (ch == 'd')
        {
            SNAKE_ORIENTATION = RIGHT;
        }
        else if (ch == 's')
        {
            SNAKE_ORIENTATION = BOTTOM;
        }
        else if (ch == 'a')
        {
            SNAKE_ORIENTATION = LEFT;
        }
        nextStep(gamespace);
        printSnakeGame(gamespace);
    }
}