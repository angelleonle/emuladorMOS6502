# Emulador MOS6502

## Requerimientos
**ncurses**: interfaces gráficas en la línea de comandos.
>sudo apt-get install libncurses5-dev libncursesw5-dev

## Compilación del emulador
>gcc -g MOS6502Emulator.c structures.c extra_functions.c instructions.c pipeline.c -o MOS6502

## Ejecución del emulador
>./MOS6502

## Compilación y ejecución de juegos
**Snake**:
>bash SnakeGame.sh

**Pong**:
>bash PongGame.sh

### Controles de los juegos
**Snake**:
W: arriba
A: izquierda
S: abajo
D: derecha
E: salir del juego

**Pong**:
A: izquierda
D: derecha
E: salir del juego

El compilador mostrará fragmentos de la memoria y los registros de la CPU antes y después de la ejecución del programa en ensamblador. Adicionalmente, generarán dos archivos que contienen todos los datos de la memoria antes y después de la ejecución del programa en ensamblador llamados **MemoryBefore.txt** y **MemoryAfter.txt**
