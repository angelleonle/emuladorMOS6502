# Emulador MOS6502 (Equipo 03 - OAC)

**Este repositorio contiene el emulador MOS6502 realizado por estudiantes de la Pontificia Universidad Católica del Perú como parte del curso Organización y Arquitectura de Computadoras**

## Consideraciones del desarrollo de los juegos
Para el desarrollo de la interfaz de los juegos **Snake** y **Pong** se empleó **NCURSES**, biblioteca que permite construir interfaces gráficas en la línea de comandos, por tanto, para compilar el programa es necesario instalar dicha biblioteca utilizando el siguiente comando:

>sudo apt-get install libncurses5-dev libncursesw5-dev

- También se incluyen en el repositorio ejecutables ya compilados de los programas (**SnakeGame** y **PongGame**), los cuales pueden utilizarse para probar los juegos.

- El código ASM de los juegos se encuentra en los archivos **SnakeGame.asm** y **PongGame.asm**

- El volcado hexadecimal de las instrucciones se encuentra en los archivos **HEXDumpSnake.txt** y **HEXDumpPong.txt**

- El archivo **SnakeEmulator.c** y **PongEmulator.c** son versiones modificados del módulo principal del emulador (**MOS6502Emulator.c**), presentado en la primera parte, con la diferencia de que aquí se incluye la interfaz gràfica.
- Para ejecutar y compilas los juegos use los archivos bash siguiente:
>bash SnakeGame.sh
>bash PongGame.sh
## Controles de los juegos
**Los controles para Snake son:

W: arriba
A: izquierda
S: abajo
D: derecha
E: salir del juego

**Los controles para Pong son:

A: izquierda
D: derecha
E: salir del juego

## Emulador
Para ejecutar solo el emulador con algún programa, sin la interfaz gráfica de videojuego, siga este procedimiento
Con el archivo HEXDump.txt, compile el emulador mediante el siguiente comando:

>gcc -g MOS6502Emulator.c structures.c extra_functions.c instructions.c pipeline.c -o MOS6502

Y ejecute mediante el siguiente comando:

>./MOS6502

El compilador mostrará fragmentos de la memoria y los registros de la CPU antes y después de la ejecución del programa en ensamblador. Adicionalmente, generará dos archivos que contienen todos los datos de la memoria antes y después de la ejecución del programa en ensamblador llamados **MemoryBefore.txt** y **MemoryAfter.txt**
