# Emulador MOS6502 (Equipo 03 - OAC)

**Este repositorio contiene el emulador MOS6502 realizado por estudiantes de la Pontificia Universidad Católica del Perú como parte del curso Organización y Arquitectura de Computadoras**

## Programa de prueba
El programa de prueba **ProgramCode.asm** es un programa sencillo que copia 48 veces en la memoria el número 5, a partir de la dirección $0400.

## Generador de datos de volcado hexadecimal en memoria
Para utilizar el emulador es necesario compilar y ejecutar el programa HEXDump, el cual leerá su archivo .ASM para MOS6502 y generará un archivo de volcado hexadecimal llamado **HEXDump.txt**, que será leído por el programa emulador, el cual cargará estos datos a la memoria. Vale resaltar que **HEXDump es un programa simple que no es capaz de procesadar errores de sintaxis (fue desarrollado con el único fin de hacer pruebas)**, por lo que requiere que se maneje adecuadamente su formato y se coloque un salto de línea al final del archivo, y un espacio en blanco luego de las instrucciones con modo implícito o relativo (y absoluto con una etiqueta en el caso de JMP y JSR). Así mismo, el archivo .ASM de entrada debe de usar como End Sequence **LF si se ejecuta en Linux** o **CRLF si se ejecuta en Windows** (El archivo .ASM de prueba proporcionado funciona para Linux).
La compilación del programa que generará el volcado hexadecimal se realiza mediante el siguiente comando:

>gcc HEXDumpMOS6502.cpp -lstdc++ -o HEXDump

Su ejecución se lleva a cabo mediante el siguiente comando, donde ProgramCode.asm es el archivo .ASM que desea leer:

>./HEXDump ProgramCode.asm

Alternativamente, **puede usar el archivo HEXDump.sh para leer el archivo ProgramCode.asm de prueba que se incluye en este repositorio**

>bash HEXDump.sh


## Emulador
Una vez generado el archivo HEXDump.txt con la ejecución del programa anterior, compile el emulador mediante el siguiente comando:

>gcc -g MOS6502Emulator.c structures.c extra_functions.c instructions.c pipeline.c -o MOS6502

Y ejecute mediante el siguiente comando:

>./MOS6502

Alternativamente, **puede usar el archivo Emulator.sh para compilar y ejecutar el emulador**.

>bash Emulator.sh
