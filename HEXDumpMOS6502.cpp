/*
File: HEXDumpMOS6502
By: Angel Oropeza León
*/

/*
 * El siguiente programa interpreta un archivo .asm para MOS6502
 * y lo convierte en el volcado hexadecimal que debe colocarse
 * en la memoria. Este programa requiere que la sintaxis sea correcta
 * y que las instrucciones sigan el siguiente formato:
 * LDA #$01
 * STA $0200
 * Siempre se deben de respetar los espacios, los cuales no pueden
 * ser ni más ni menos, de lo contrario la lectura fallará y los resultados será incorrectos
 * Para compilarlo usar el siguiente comando:
 * gcc HEXDumpMOS6502.cpp -lstdc++ -o HEXDump
 */

#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <inttypes.h>
using namespace std;

#define MAX_REGISTERS 10000
#define MAX_BUFFER 100
#define MAX_MODES 26
#define MAX_REL 20

/* MODOS DE DIRECCIONAMIENTO */
// Modo acumulador
#define M_ACUMULADOR 0
// Modo relativo
#define M_RELATIVE 1
// Modo inmediato
#define M_INMEDIATE 2
// Modo absoluto
#define M_ABSOLUTE 3
// Modo implícito
#define M_IMPLICIT 4
// Modo página cero
#define M_PZERO 5
// Modo absoluto indirecto
#define M_INDIRECT 6
// Modo absoluto con índice X
#define M_INDEXX 7
// Modo absoluto con índice Y
#define M_INDEXY 8
// Modo página cero con índice X
#define M_PZEROX 9
// Modo página cero con índice Y
#define M_PZEROY 10
// Modo indirecto indexado
#define M_INDINDEX 11
// Modo indexado indirecto
#define M_INDEXINDI 12

typedef struct RelativeMatrix
{
    int n;
    char *tag[MAX_REL];
    int direction[MAX_REL];
}
RelativeMatrix;

RelativeMatrix *relmatrix;

ifstream readFile(char *);
int readString(ifstream &, char *&, char);

//Leer un archivo de texto
ifstream readFile(char *filename)
{
    ifstream myfile;
    myfile.open(filename, ios :: in);
    if(!myfile)
    {
        cout << "An error occurred while opening file " << filename << "." << endl;
        exit(1);
    }
    cout << "File " << filename << " was opened successfully." << endl;
    return myfile;
}

//Escribir un archivo de texto desde el principio
ofstream writeFile(char *filename)
{
    ofstream myfile;
    myfile.open(filename, ios :: out);

    if(!myfile)
    {
        cout << "An error occurred while opening file " << filename << "." << endl;
        exit(1);
    }
    cout << "File " << filename << " was opened successfully." << endl;
    return myfile;
}

int readString(ifstream &myfile, char *&str, char separator)
{
    //Definimos una arreglo estático que se usará como buffer
    char buffer[MAX_BUFFER];
    //Leemos la cadena de la entrada estádar
    myfile.getline(buffer, MAX_BUFFER, separator);
    if (myfile.eof())
    {
        myfile.clear();
        return 1;
    }
    //Encontramos el tamaño de la cadena
    int len = strlen(buffer);
    /*Asignamos memoria dinámicamente para la nueva cadena
    incluido el caracter nulo*/
    str = new char[len + 1];
    //Copiamos la cadena al destino
    strcpy(str, buffer);
    return 0;
}

int getDirModeID(char *dirMode)
{
    // Acumulador
    if(!strcmp(dirMode, "accumulator"))
    {
        return M_ACUMULADOR;
    }
    // Relativo
    if(!strcmp(dirMode, "relative"))
    {
        return M_RELATIVE;
    }
    // Inmediato
    if(!strcmp(dirMode, "immidiate"))
    {
        return M_INMEDIATE;
    }
    // Absoluto
    if(!strcmp(dirMode, "absolute"))
    {
        return M_ABSOLUTE;
    }
    // Implícito
    if(!strcmp(dirMode, "implied"))
    {
        return M_IMPLICIT;
    }
    // Página cero
    if(!strcmp(dirMode, "zeropage"))
    {
        return M_PZERO;
    }
    // Indirecto
    if(!strcmp(dirMode, "indirect"))
    {
        return M_INDIRECT;
    }
    // Absoluto con índice X
    if(!strcmp(dirMode, "absolute,X"))
    {
        return M_INDEXX;
    }
    // Absoluto con índice Y
    if(!strcmp(dirMode, "absolute,Y"))
    {
        return M_INDEXY;
    }
    // Página cero con índice X
    if(!strcmp(dirMode, "zeropage,X"))
    {
        return M_PZEROX;
    }
    // Página cero con índice Y
    if(!strcmp(dirMode, "zeropage,Y"))
    {
        return M_PZEROY;
    }
    if(!strcmp(dirMode, "(indirect),Y"))
    {
        return M_INDINDEX;
    }
    if(!strcmp(dirMode, "(indirect,X)"))
    {
        return M_INDEXINDI;
    }
    return -1;
}

int readInstruction(ifstream &myfile, int n, void *auxRegisters[])
{
    void **reg;
    char *upname, *dirMode;
    uint8_t *data = new uint8_t[MAX_MODES * 2];
    for(int i = 0; i < MAX_MODES * 2; i++)
    {
        data[i] = 0;
    }
    int upcode, bytes;
    if(readString(myfile, upname, '\n'))
    {
        return 1;
    }
    while(1)
    {
        readString(myfile, dirMode, ' ');
        int dmid = getDirModeID(dirMode);
        myfile >> hex >> upcode;
        myfile.get();
        myfile >> bytes;
        data[dmid * 2] = upcode;
        data[dmid * 2 + 1] = bytes;
        delete dirMode;
        char c = myfile.get();
        if(c == '\n')
        {
            break;
        }
    }
    reg = new void * [2];
    reg[0] = upname;
    reg[1] = data;
    auxRegisters[n] = reg;
    return 0;
}

void storageInstructions(void *&instructions)
{
    void **registers, *auxRegisters[MAX_REGISTERS];
    int n = 0;
    ifstream myfile;
    myfile = readFile("Instructions.txt");
    while(1)
    {
        if (readInstruction(myfile, n, auxRegisters))
        {
            break;
        }
        n++;
    }
    registers = new void * [n + 1];
    for (int i = 0; i < n; i++)
    {
        registers[i] = auxRegisters[i];
    }
    registers[n] = nullptr;
    instructions = registers;
}

void printInstructionReport(void *instructions)
{
    char *str;
    uint8_t *arr;
    void **registers = (void **) instructions;
    ofstream myfile = writeFile("InstructionReport.txt");
    int i = 0;
    myfile << endl << "-----------------------------------------------------------------------------" << endl;
    myfile << "                     MICROPROCESSOR INSTRUCTION REPORT" << endl;
    myfile << "-----------------------------------------------------------------------------" << endl;
    while(registers[i] != nullptr)
    {
        void **aregister = (void **) registers[i];
        str = (char *) (aregister[0]);
        arr = (uint8_t *) (aregister[1]);
        myfile << left << str << endl;
        myfile << "-----------------------------------------------------------------------------" << endl;
        myfile << "ACU   REL   INM   ABS   IMP   PZE   ABI   AIX   AIY   PZX   PZY   IIX   IXI" << endl;
        myfile << "-----------------------------------------------------------------------------" << endl;
        for(int i = 0; i < 13; i++)
        {
            myfile.fill('0');
            if(arr[2 * i] != 0)
            {
                myfile << right << setw(2) << hex << (int) arr[2 * i] << " "<< setw(2) << (int) arr[2 * i + 1] << " ";
            }
            else
            {
                myfile << "      ";
            }
        }
        myfile << endl;
        i++;
    }
}

void freeInstructions(void *instructions)
{
    char *str;
    uint8_t *ptInt;
    void **registers = (void **) instructions;
    int i = 0;
    while(registers[i] != nullptr)
    {
        void **aregister = (void **) registers[i];
        str = (char *) (aregister[0]);
        ptInt = (uint8_t *) (aregister[1]);
        delete str;
        delete ptInt;
        i++;
    }
    delete registers;
}

//HEXDUMP

int findRelativeTag(char *tagname)
{
    int matlen = relmatrix -> n;
    for(int i = 0; i < matlen; i++)
    {
        char *aux = (relmatrix -> tag)[i];
        if(!strcmp(tagname, aux))
        {
            return (relmatrix -> direction)[i];
        }
    }
}

uint8_t getUpcode(void *instructions, char *upname, int modeCode, int &nbytes)
{
    char *str;
    uint8_t *arr;
    void **registers = (void **) instructions;
    int i = 0;
    while(registers[i] != nullptr)
    {
        void **aregister = (void **) registers[i];
        str = (char *) (aregister[0]);
        if(!strcmp(upname,str))
        {
            arr = (uint8_t *) (aregister[1]);
            nbytes = arr[modeCode * 2 + 1];
            return arr[modeCode * 2];
        }
        i++;
    }
    return 0;
}

void insertInstruction(int upcode, int number, int nbytes, uint8_t auxRegisters[], int &n)
{
    nbytes--;
    uint8_t n1 = (uint8_t) (number >> 8);
    uint8_t n2 = (uint8_t) (number & 0b0000000011111111);
    auxRegisters[n] = (uint8_t) upcode;
    n++;
    if(nbytes == 1)
    {
        auxRegisters[n] = n2;
        n++;
    }
    else if (nbytes == 2)
    {
        auxRegisters[n] = n2;
        n++;
        auxRegisters[n] = n1;
        n++;
    }
}

int getInstruction(ifstream &myfile, void *instructions, int &n, uint8_t auxRegisters[])
{
    char *upname;
    int modeCode, number;


    char c = myfile.get();
    if(myfile.eof())
    {
        return 1;
    }
    myfile.unget();
    if(c == '_')
    {
        char buffer[MAX_BUFFER];
        myfile.getline(buffer, MAX_BUFFER, '\n');
        return 0;
    }
    if(readString(myfile, upname, ' '))
    {
        return 1;
    }
    c = myfile.get();
    if(c == '\n')
    {
        myfile.unget();
        modeCode = M_IMPLICIT;
    }
    else if (c == '#')
    {
        modeCode = M_INMEDIATE;
        if(myfile.peek() == '$')
        {
            myfile.get();
            myfile >> hex >> number;
        }
        else
        {
            myfile >> dec >> number;
        }
    }
    else if (c == 'A')
    {
        modeCode = M_ACUMULADOR;
    }
    else if (c == '(')
    {
        myfile.get();
        myfile >> hex >> number;
        if(number > 0xFF)
        {
            modeCode = M_INDIRECT;
        }
        else
        {
            c = myfile.get();
            if(c == ',')
            {
                modeCode = M_INDEXINDI;
            }
            else
            {
                modeCode = M_INDINDEX;
            }
        }
    }
    else if (c == '_')
    {
        char *tagname;
        myfile.unget();
        readString(myfile, tagname, ' ');
        if(!strcmp(upname,"JMP") || !strcmp(upname,"JSR"))
        {
            modeCode = M_ABSOLUTE;
        }
        else
        {
            modeCode = M_RELATIVE;
        }
        int aux  = findRelativeTag(tagname);

        if(!strcmp(upname,"JMP") || !strcmp(upname,"JSR"))
        {
            number = aux;
        }
        else
        {
            number = aux - n - 2;
        }
    }
    else
    {
        myfile >> hex >> number;
        if(number > 0xFF)
        {
            c = myfile.get();
            if(c == '\n')
            {
                myfile.unget();
                modeCode = M_ABSOLUTE;
            }
            else
            {
                c = myfile.get();
                if(c == 'X')
                {
                    modeCode = M_INDEXX;
                }
                else
                {
                    modeCode = M_INDEXY;
                }
            }
        }
        else
        {
            c = myfile.get();
            if(c == ',')
            {
                c = myfile.get();
                if(c == 'X')
                {
                    modeCode = M_PZEROX;
                }
                else
                {
                    modeCode = M_PZEROY;
                }
            }
            else
            {
                modeCode = M_PZERO;
            }
        }
    }
    char buffer[MAX_BUFFER];
    myfile.getline(buffer, MAX_BUFFER, '\n');
    int nbytes;
    uint8_t upcode= getUpcode(instructions, upname, modeCode, nbytes);
    insertInstruction(upcode, number, nbytes, auxRegisters, n);
    return 0;
}

void incN(int upcode, int nbytes, int &n)
{
    nbytes--;
    n++;
    if(nbytes == 1)
    {
        n++;
    }
    else if (nbytes == 2)
    {
        n++;
        n++;
    }
}

int getTag(ifstream &myfile, void *instructions, int &n)
{
    char *upname;
    int modeCode, number;

    char c = myfile.get();
    if(myfile.eof())
    {
        return 1;
    }
    myfile.unget();
    if(c == '_')
    {
        char *tagname;
        readString(myfile, tagname, ':');
        int aux = (relmatrix -> n);
        (relmatrix -> tag)[aux] = tagname;
        (relmatrix -> direction)[aux] = n;
        (relmatrix -> n) += 1;
        char buffer[MAX_BUFFER];
        myfile.getline(buffer, MAX_BUFFER, '\n');
        return 0;
    }
    readString(myfile, upname, ' ');
    c = myfile.get();
    if(c == '\n')
    {
        myfile.unget();
        modeCode = M_IMPLICIT;
    }
    else if (c == '#')
    {
        modeCode = M_INMEDIATE;
        if(myfile.peek() == '$')
        {
            myfile.get();
            myfile >> hex >> number;
        }
        else
        {
            myfile >> dec >> number;
        }
    }
    else if (c == 'A')
    {
        modeCode = M_ACUMULADOR;
    }
    else if (c == '(')
    {
        myfile.get();
        myfile >> hex >> number;
        if(number > 0xFF)
        {
            modeCode = M_INDIRECT;
        }
        else
        {
            c = myfile.get();
            if(c == ',')
            {
                modeCode = M_INDEXINDI;
            }
            else
            {
                modeCode = M_INDINDEX;
            }
        }
    }
    else if (c == '_')
    {
        if(!strcmp(upname,"JMP") || !strcmp(upname,"JSR"))
        {
        modeCode = M_ABSOLUTE;
        }
        else
        {
        modeCode = M_RELATIVE;
        }
    }
    else
    {
        myfile >> hex >> number;
        if(number > 0xFF)
        {
            c = myfile.get();
            if(c == '\n')
            {
                myfile.unget();
                modeCode = M_ABSOLUTE;
            }
            else
            {
                c = myfile.get();
                if(c == 'X')
                {
                    modeCode = M_INDEXX;
                }
                else
                {
                    modeCode = M_INDEXY;
                }
            }
        }
        else
        {
            c = myfile.get();
            if(c == ',')
            {
                c = myfile.get();
                if(c == 'X')
                {
                    modeCode = M_PZEROX;
                }
                else
                {
                    modeCode = M_PZEROY;
                }
            }
            else
            {
                modeCode = M_PZERO;
            }
        }
    }
    char buffer[MAX_BUFFER];
    myfile.getline(buffer, MAX_BUFFER, '\n');
    int nbytes;
    uint8_t upcode= getUpcode(instructions, upname, modeCode, nbytes);
    incN(upcode, nbytes, n);
    return 0;
}

int generateHEXDump(void *instructions, uint8_t *&programCode, char *filename)
{
    uint8_t *registers, auxRegisters[MAX_REGISTERS];
    int n = 0;
    relmatrix -> n = 0;
    ifstream myfile;
    myfile = readFile(filename);


    while(1)
    {
        if (getTag(myfile, instructions, n))
        {
            break;
        }
    }

    myfile.clear();
    myfile.seekg(0, myfile.beg);
    n = 0;

    while(1)
    {
        if (getInstruction(myfile, instructions, n, auxRegisters))
        {
            break;
        }
    }
    registers = new uint8_t[n + 1];
    for (int i = 0; i < n; i++)
    {
        registers[i] = auxRegisters[i];
    }
    registers[n] = 0;
    programCode = registers;
    return n;
}

void printHEXDumpReport(uint8_t *programCode, int n)
{
    ofstream myfile = writeFile("HEXDump.txt");
    for(int i = 0; i < n; i++)
    {
        myfile.fill('0');
        myfile << hex << setw(2) << (int) programCode[i] << endl;
    }
}

int main(int argc, char *argv[])
{
    void *instructions;
    uint8_t *programCode;
    relmatrix = (RelativeMatrix *) new RelativeMatrix;
    storageInstructions(instructions);
    printInstructionReport(instructions);
    int n = generateHEXDump(instructions, programCode, "ProgramCode.asm");
    printHEXDumpReport(programCode, n);
    return 0;
}
