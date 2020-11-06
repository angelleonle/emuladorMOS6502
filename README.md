# emuladorMOS6502 Equipo 10 (OAC)
Este repositorio contiene el emulador MOS6502 realizado por estudiantes de la Pontificia Universidad Católica del Perú como parte del curso Organización y Arquitectura de Computadoras

Para utilizar el emulador, es necesario compilar el programa HEXDump, el cual leerá su archivo .ASM para MOS6502 y generará un archivo de volcado hexadecimal llamado HEXDump.txt que será leído por el programa emulador, el cual cargará estos datos a la memoria. Vale resaltar que este programa requiere que se maneje adecuadamente la sintaxis y se coloque un salto de línea al final del archivo, puesto que no contempla errores de sintaxis. Así mismo, el archivo .ASM de entrada debe de usar como End Sequence el LF si se ejecuta en Linux o CRLF si se ejecuta en Windows (El archivo .ASM de prueba proporcionado funciona para Linux).
La compilación del programa que generará el volcado hexadecimal se realiza mediante el siguiente comando:

gcc HEXDumpMOS6502.cpp -lstdc++ -o HEXDump

Su ejecución se realiza mediante el siguiente comando, donde ProgramCode.asm es el archivo .ASM que desea leer:
./HEXDump ProgramCode.asm

Una vez generado el archivo HEXDump.txt, ejecute el emulador usando el siguiente comando:
