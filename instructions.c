#include <stdio.h>
#include "extra_functions.h"
#include "flag_modes.h"
#include "structures.c"
#include "instructions.h"

/*
AND  AND Memory with Accumulator

    A AND M -> A                 N Z C I D V
                                 + + - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    immidiate     AND #oper    29    2    2
    zeropage      AND oper     25    2    3
    zeropage,X    AND oper,X   35    2    4
    absolute      AND oper     2D    3    4
    absolute,X    AND oper,X   3D    3    4*
    absolute,Y    AND oper,Y   39    3    4*
    (indirect,X)  AND (oper,X) 21    2    6
    (indirect),Y  AND (oper),Y 31    2    5*
*/
void AND(int mode ,uint8_t value1, uint8_t value2){
    int position, position1, position2;
    position = (value2 << 8) + value1;

    if (value2 == 0xff || value2 == 0x01){
        printf("AND: No se puede acceder a la página uno (0x01) o 256 (0xff) \n");
        return;
    }

    switch (mode){
        case M_INMEDIATO :
            cpu -> a &= value1;
            (cpu -> pc) +=2;
            break;

        case M_PCERO :
            cpu -> a &= mem -> ram[value1];
            (cpu -> pc) +=2;
            break;

        case M_PCEROX :
            cpu -> a &= (mem -> ram[(value1 + cpu -> x) & 0x00FF]);
            (cpu -> pc) +=2;
            break;

        case M_ABSOLUTO :
            cpu -> a &= (mem -> ram[position]);
            (cpu -> pc) += 3;
            break;

        case M_INDICEX :
            cpu -> a &= (mem -> ram[position + cpu -> x]);
            (cpu -> pc) +=3;
            break;

        case M_INDICEY :
            cpu -> a = cpu ->a & (mem -> ram[(( position+ cpu ->y) )]);
            (cpu ->pc) +=3;
            break;

        case M_INDINDEX : // Y
            
            position1 = mem -> ram[value1];
            position2 = mem -> ram[value1 + 1];
            position = (position2 << 8) + position1;
            cpu -> a &= (mem -> ram [position + cpu -> y]);
            (cpu -> pc) += 2;
            break;

        case M_INDEXINDI : // X
            position1 = mem -> ram[value1 + cpu -> x] ;
            position2 = mem -> ram[value1 + cpu -> x + 1];
            position = (position2 << 8) + position1;
            cpu -> a &= (mem -> ram[position]);
            (cpu -> pc) +=2;
            break;
    }
    flagN(cpu -> a);
    flagZ(cpu -> a);
}

/*
ADC  Add Memory to Accumulator with Carry

    A + M + C -> A, C             N Z C I D V
                                  + + + - - +
    addressing    assembler    opc  bytes  cyles
     --------------------------------------------
    immidiate     ADC #oper    69    2    2
    zeropage      ADC oper     65    2    3
    zeropage,X    ADC oper,X   75    2    4
    absolute      ADC oper     6D    3    4
    absolute,X    ADC oper,X   7D    3    4*
    absolute,Y    ADC oper,Y   79    3    4*
    (indirect,X)  ADC (oper,X) 61    2    6
    (indirect),Y  ADC (oper),Y 71    2    5*
*/
void ADC(int mode ,uint8_t value1, uint8_t value2)
{
    int position, position1, position2;
    position = (value2 << 8 ) + value1;
    if (value2 == 0xff || value2 == 0x01){
        printf("ADC: No se puede acceder a la página uno (0x01) o 256 (0xff) \n");
        return;
    }
    if ( (cpu -> sr) & FLAG_C ) cpu -> a++;
    uint8_t sumando, aux_a = cpu -> a;
    uint16_t suma;
    switch (mode){
        case M_INMEDIATO:
            sumando = value1;
            cpu -> a += sumando;
            cpu -> pc += 2;
            break;

        case M_PCERO:
            sumando = mem -> ram[value1];
            cpu -> a += sumando;
            cpu -> pc += 2;
            break;

        case M_PCEROX:
            sumando = mem -> ram[(value1 + cpu -> x) & 0x00FF];
            cpu -> a += sumando;
            (cpu -> pc) += 2;
            break;

        case M_ABSOLUTO:
            sumando = mem -> ram[position];
            cpu -> a += sumando;
            (cpu -> pc) += 3;
            break;

        case M_INDICEX:
            sumando = mem -> ram[position + cpu -> x];
            cpu -> a += sumando;
            cpu -> pc += 3;
            break;

        case M_INDICEY:
            sumando = mem -> ram[position + cpu -> y];
            cpu -> a += sumando;
            cpu -> pc += 3;
            break;

        case M_INDINDEX:
            position1 = mem -> ram[value1];
            position2 = mem -> ram[value1 + 1];
            position = (position2 << 8) + position1;
            sumando = mem -> ram[position + cpu -> y];
            cpu -> a += sumando;
            cpu -> pc += 2;
            break;

        case M_INDEXINDI:
            position1 = mem -> ram[value1 + cpu -> x];
            position2 = mem -> ram[value1 + cpu -> x + 1];
            position = (position2 << 8) + position1;
            sumando = mem -> ram[position];
            cpu -> a += sumando;
            cpu -> pc += 2;
            break;
    }
    flagZ(cpu -> a);
    flagN(cpu -> a);
    suma = sumando + aux_a;
    flagC(suma);
    flagV(sumando, aux_a, suma);
}

/*
ASL  Shift Left One Bit (Memory or Accumulator)

    C <- [76543210] <- 0           N Z C I D V
                                   + + + - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    accumulator   ASL A        0A    1    2
    zeropage      ASL oper     06    2    5
    zeropage,X    ASL oper,X   16    2    6
    absolute      ASL oper     0E    3    6
    absolute,X    ASL oper,X   1E    3    7
*/
void ASL(int mode, uint8_t value1, uint8_t value2)
{
    int position;
    position = (value2 << 8 ) + value1;
    if (value2 == 0xff || value2 == 0x01){
        printf("ASL: No se puede acceder a la página uno (0x01) o 256 (0xff) \n");
        return;
    }
    uint8_t aux;
    switch (mode){
        case M_ACUMULADOR:
            aux = (cpu -> a & 0b10000000);
            cpu -> a <<= 1;
            cpu -> pc += 1;
            break;

        case M_PCERO:
            aux = (mem -> ram[value1] & 0b10000000);
            cpu -> a = (mem -> ram[value1] << 1);
            cpu -> pc += 2;
            break;

        case M_PCEROX:
            aux = (mem -> ram [ value1 + cpu -> x] & 0b10000000);
            cpu -> a = mem -> ram [value1 + cpu -> x] << 1;
            cpu -> pc += 2;
            break;

        case M_ABSOLUTO:
            aux = (mem -> ram[position] & 0b10000000);
            cpu -> a = mem -> ram[position] << 1;
            cpu -> pc += 3;
            break;

        case M_INDICEX:
            aux = (mem -> ram[position + cpu -> x] & 0b10000000);
            cpu -> a = (mem -> ram[position + cpu -> x]) << 1;
            cpu -> pc += 3;
            break;
    }
    flagN(cpu -> a);
    flagZ(cpu -> a);
    set_flag(FLAG_C, aux);
}

/*
BCC  Branch on Carry Clear

    branch on C = 0               N Z C I D V
                                  - - - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    relative      BCC oper     90    2    2**
*/
void BCC(uint8_t displacement)
{
    if(((cpu -> sr) & FLAG_C) == 0){
        if(displacement >> 7)
        {
            uint8_t aux = ~displacement - 1;
            (cpu -> pc) -= (int) aux;
        }
        else
        {
            (cpu -> pc) += (displacement + 2);
        }
    }
    else{
        (cpu -> pc) += 2;
    }
}

/*
BCS  Branch on Carry Set

    branch on C = 1               N Z C I D V
                                  - - - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    relative      BCS oper     B0    2    2**
*/
void BCS(uint8_t displacement)
{
    if(((cpu -> sr) & FLAG_C) == 1){
        if(displacement >> 7)
        {
            uint8_t aux = ~displacement - 1;
            (cpu -> pc) -= (int) aux;
        }
        else
        {
            (cpu -> pc) += (displacement + 2);
        }
    }
    else{
        (cpu -> pc) += 2;
    }
}

/*
BEQ  Branch on Result Zero

    branch on Z = 1               N Z C I D V
                                - - - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    relative     BEQ oper      F0    2    2**
*/
void BEQ(uint8_t displacement)
{
    if(((cpu -> sr) & FLAG_Z) == FLAG_Z){
        if(displacement >> 7)
        {
            uint8_t aux = ~displacement - 1;
            (cpu -> pc) -= (int) aux;
        }
        else
        {
            (cpu -> pc) += (displacement + 2);
        }
    }
    else{
        (cpu -> pc) += 2;
    }
}

/*
BIT  Test Bits in Memory with Accumulator

    bits 7 and 6 of operand are transfered to bit 7 and 6 of SR (N,V);
    the zeroflag is set to the result of operand AND accumulator.
    A AND M, M7 -> N, M6 -> V       N Z C I D V
                                   M7 + - - - M6
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    zeropage      BIT oper     24    2    3
    absolute      BIT oper     2C    3    4
*/
void BIT(int mode ,uint8_t value1, uint8_t value2){
    int position = (value2 << 8 ) + value1;
    if (value2 == 0xff || value2 == 0x01){
        printf("BIT: No se puede acceder a la página uno (0x01) o 256 (0xff) \n");
        return;
    }
    uint8_t memory_value;
    switch (mode){
        case M_PCERO:
            memory_value = mem -> ram[value1];
            cpu -> pc += 2;
            break;
        case M_ABSOLUTO:
            memory_value = mem -> ram[position];
            (cpu -> pc) += 3;
            break;
    }
    set_flag(FLAG_V, (memory_value & 0b01000000));
    set_flag(FLAG_N, (memory_value & 0b10000000));
    flagZ( cpu -> a & memory_value );
}

/*
BMI  Branch on Result Minus

    branch on N = 1               N Z C I D V
                                  - - - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    relative      BMI oper     30    2    2**
*/
void BMI(uint8_t displacement)
{
    if(((cpu -> sr) & FLAG_N) == FLAG_N){
        if(displacement >> 7)
        {
            uint8_t aux = ~displacement - 1;
            (cpu -> pc) -= (int) aux;
        }
        else
        {
            (cpu -> pc) += (displacement + 2);
        }
    }
    else{
        (cpu -> pc) += 2;
    }
}

/*
BNE  Branch on Result not Zero

    branch on Z = 0               N Z C I D V
                                  - - - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    relative      BNE oper     D0    2    2**
*/
void BNE(uint8_t displacement)
{
    if(((cpu -> sr) & FLAG_Z) == 0){
        if(displacement >> 7)
        {
            uint8_t aux = ~displacement - 1;
            (cpu -> pc) -= (int) aux;
        }
        else
        {
            (cpu -> pc) += (displacement + 2);
        }
    }
    else{
        (cpu -> pc) += 2;
    }
}

/*
BPL  Branch on Result Plus

    branch on N = 0               N Z C I D V
                                  - - - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    relative      BPL oper     10    2    2**
*/
void BPL(uint8_t displacement)
{
    if(((cpu -> sr) & FLAG_N) == 0){
        if(displacement >> 7)
        {
            uint8_t aux = ~displacement - 1;
            (cpu -> pc) -= (int) aux;
        }
        else
        {
            (cpu -> pc) += (displacement + 2);
        }
    }
    else{
        (cpu -> pc) += 2;
    }
}

/*
BVC  Branch on Overflow Clear

    branch on V = 0               N Z C I D V
                                  - - - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    relative      BVC oper     50    2    2**
*/
void BVC(uint8_t displacement)
{
    if(((cpu -> sr) & FLAG_V) == 0){
        if(displacement >> 7)
        {
            uint8_t aux = ~displacement - 1;
            (cpu -> pc) -= (int) aux;
        }
        else
        {
            (cpu -> pc) += (displacement + 2);
        }
    }
    else{
        (cpu -> pc) += 2;
    }
}

/*
BVS  Branch on Overflow Set

    branch on V = 1               N Z C I D V
                                  - - - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    relative      BVC oper     70    2    2**

*/
void BVS(uint8_t displacement)
{
    if(((cpu -> sr) & FLAG_V) == FLAG_V){
        if(displacement >> 7)
        {
            uint8_t aux = ~displacement - 1;
            (cpu -> pc) -= (int) aux;
        }
        else
        {
            (cpu -> pc) += (displacement + 2);
        }
    }
    else{
        (cpu -> pc) += 2;
    }
}

/*
BRK  Force Break

    interrupt,                   N Z C I D V
    push PC+2, push SR           - - - 1 - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       BRK          00    1    7
*/
void BRK()
{
    (cpu -> pc) += 1;
    // Bandera interrupción
    set_flag(FLAG_I, 1);
    // Push PC + 2
    (mem -> ram[0x0100 + (cpu -> sp)]) = ((cpu -> pc) + 2);
    (cpu -> sp) -= 1;
    // Push SR
    PHP();
}

/*
CLC  Clear Carry Flag

    0 -> C                      N Z C I D V
                                - - 0 - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       CLC          18    1    2
*/
void CLC()
{
    (cpu -> pc) += 1;
    // Bandera acarreo
    set_flag(FLAG_C, 0);
}

/*
CLD  Clear Decimal mode

    0 -> D                      N Z C I D V
                                - - - - 0 -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       CLD          D8    1    2
*/
void CLD()
{
    (cpu -> pc) += 1;
    // Bandera decimal
    set_flag(FLAG_D, 0);
}

/*
CLI  Clear Interrupt Disable Bit

    0 -> I                      N Z C I D V
                                - - - 0 - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       CLI          58    1    2
*/
void CLI()
{
    (cpu -> pc) += 1;
    // Bandera interrupción
    set_flag(FLAG_I, 0);
}

/*
CLV  Clear Overflow Flag

    0 -> V                      N Z C I D V
                                - - - - - 0
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       CLV          B8    1    2
*/
void CLV()
{
    (cpu -> pc) += 1;
    // Bandera overflow
    set_flag(FLAG_V, 0);
}

/*
CMP  Compare Memory with Accumulator

    A - M                       N Z C I D V
                                + + + - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    immidiate     CMP #oper    C9    2    2
    zeropage      CMP oper     C5    2    3
    zeropage,X    CMP oper,X   D5    2    4
    absolute      CMP oper     CD    3    4
    absolute,X    CMP oper,X   DD    3    4*
    absolute,Y    CMP oper,Y   D9    3    4*
    (indirect,X)  CMP (oper,X) C1    2    6
    (indirect),Y  CMP (oper),Y D1    2    5*
*/
void CMP(int mode, uint8_t value1, uint8_t value2){
    int position, position1, position2;
    position = (value2 << 8) + value1;

    if (value2 == 0xff || value2 == 0x01){
        printf("CMP: No se puede acceder a la página uno (0x01) o 256 (0xff) \n");
        return;
    }

    uint8_t aux;

    switch (mode){
        case M_INMEDIATO:
            aux = value1;
            cpu -> pc += 2;
            break;

        case M_PCERO:
            aux = mem -> ram[value1];
            cpu -> pc += 2;
            break;

        case M_PCEROX:
            aux = mem -> ram[(value1 + cpu -> x) & 0x00FF];
            cpu -> pc += 2;
            break;

        case M_ABSOLUTO:
            aux = mem -> ram[position];
            cpu -> pc += 3;
            break;

        case M_INDICEX:
            aux = mem -> ram[position + cpu -> x];
            cpu -> pc += 3;
            break;

        case M_INDICEY:
            aux = mem -> ram[position + cpu -> y];
            cpu -> pc += 3;
            break;

        case M_INDINDEX: //CON Y
            position1 = mem -> ram[value1];
            position2 = mem -> ram[value1 + 1];
            position = (position2 << 8) + position1;
            aux = mem -> ram[position + cpu -> y];
            (cpu -> pc) += 2;
            break;

        case M_INDEXINDI: //CON X
            position1 = mem -> ram[value1 + cpu -> x ];
            position2 = mem -> ram[value1 + cpu -> x + 1];
            position = (position2 << 8) + position1;
            aux = mem -> ram[position];
            (cpu ->pc) +=2;
            break;
    }
    set_flag(FLAG_Z, (cpu ->a == aux));
    set_flag(FLAG_C, (cpu ->a >= aux));
    flagN(cpu -> a - aux);
}

/*
CPX  Compare Memory and Index X

    X - M                       N Z C I D V
                                + + + - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    immidiate     CPX #oper    E0    2    2
    zeropage      CPX oper     E4    2    3
    absolute      CPX oper     EC    3    4
*/
void CPX(int mode, uint8_t value1, uint8_t value2){
    int position;
    position = (value2 << 8) + value1;

    if (value2 == 0xff || value2 == 0x01){
        printf("CPX: No se puede acceder a la página uno (0x01) o 256 (0xff) \n");
        return;
    }

    uint8_t aux;
    switch (mode){
        case M_INMEDIATO:
            aux = value1;
            cpu -> pc += 2;
            break;

        case M_PCERO:
            aux = mem ->  ram[value1];
            cpu -> pc += 2;
            break;

        case M_ABSOLUTO:
            aux = mem -> ram[position];
            cpu -> pc += 3;
            break;

    }
    set_flag(FLAG_Z, (cpu ->x == aux));
    set_flag(FLAG_C, (cpu ->x >= aux));
    flagN(cpu -> x - aux);
}

/*
CPY  Compare Memory and Index Y

    Y - M                       N Z C I D V
                                + + + - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    immidiate     CPY #oper    C0    2    2
    zeropage      CPY oper     C4    2    3
    absolute      CPY oper     CC    3    4
*/
void CPY(int mode, uint8_t value1, uint8_t value2){
    int position;
    position = (value2 << 8) + value1;
    if (value2 == 0xff || value2 == 0x01){
        printf("CPY: No se puede acceder a la página uno (0x01) o 256 (0xff) \n");
        return;
    }
    uint8_t aux;
    switch (mode){
        case M_INMEDIATO:
            aux = value1;
            cpu -> pc += 2;
            break;

        case M_PCERO:
            aux = mem -> ram[value1];
            cpu -> pc += 2;
            break;

        case M_ABSOLUTO:
            aux = mem -> ram[position];
            cpu -> pc += 3;
            break;
    }
    set_flag(FLAG_Z, (cpu ->y == aux));
    set_flag(FLAG_C, (cpu ->y >= aux));
    flagN(cpu -> y - aux);
}

/*
DEC  Decrement Memory by One

    M - 1 -> M                   N Z C I D V
                                 + + - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    zeropage      DEC oper     C6    2    5
    zeropage,X    DEC oper,X   D6    2    6
    absolute      DEC oper     CE    3    6
    absolute,X    DEC oper,X   DE    3    7
*/
void DEC(int mode,uint8_t value1, uint8_t value2){
    int position;
    position = (value2 << 8) + value1;
    if (value2 == 0xff || value2 == 0x01){
        printf("DEC: No se puede acceder a la página uno (0x01) o 256 (0xff) \n");
        return;
    }
    uint8_t aux;
    switch (mode){
        case M_PCERO:
            mem -> ram[value1] -= 1;
            aux = mem -> ram[value1];
            cpu -> pc += 2;
            break;

        case M_PCEROX:
            mem -> ram[(value1 + cpu -> x) & 0x00FF] -= 1;
            aux = mem -> ram[(value1 + cpu -> x) & 0x00FF];
            cpu -> pc += 2;
            break;

        case M_ABSOLUTO:
            mem -> ram[position] -= 1;
            aux = mem -> ram[position];
            cpu -> pc += 3;
            break;

        case M_INDICEX:
            mem -> ram[position + cpu -> x] -= 1;
            aux = mem -> ram[position + cpu -> x];
            cpu -> pc += 3;
            break;
    }
    flagZ(aux);
    flagN(aux);
}

/*
DEX  Decrement Index X by One

    X - 1 -> X                   N Z C I D V
                                 + + - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       DEC          CA    1    2
*/
void DEX()
{
    (cpu -> x) -= 1;
    flagN(cpu -> x);
    flagZ(cpu -> x);
    (cpu -> pc) += 1;
}

/*
DEY  Decrement Index Y by One

    Y - 1 -> Y                   N Z C I D V
                                 + + - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       DEC          88    1    2
*/
void DEY()
{
    (cpu -> y) -= 1;
    flagN(cpu -> y);
    flagZ(cpu -> y);
    (cpu -> pc) += 1;
}

/*
EOR  Exclusive-OR Memory with Accumulator

    A EOR M -> A                 N Z C I D V
                                 + + - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    immidiate     EOR #oper    49    2    2
    zeropage      EOR oper     45    2    3
    zeropage,X    EOR oper,X   55    2    4
    absolute      EOR oper     4D    3    4
    absolute,X    EOR oper,X   5D    3    4*
    absolute,Y    EOR oper,Y   59    3    4*
    (indirect,X)  EOR (oper,X) 41    2    6
    (indirect),Y  EOR (oper),Y 51    2    5*
*/
void EOR(int mode, uint8_t value1, uint8_t value2){
    int position, position1, position2;
    position = (value2 << 8) + value1;

    if (value2 == 0xff || value2 == 0x01){
        printf("EOR: No se puede acceder a la página uno (0x01) o 256 (0xff) \n");
        return;
    }

    switch (mode) {
        case M_INMEDIATO:
            cpu -> a = ((cpu -> a & (~value1)) | ((~(cpu -> a)) & value1));
            (cpu -> pc) +=2;
            break;

        case M_PCERO:
            cpu -> a = ((cpu -> a & (~(mem -> ram [value1]))) | (~(cpu -> a) & mem -> ram [value1]));
            (cpu -> pc) +=2;
            break;

        case M_PCEROX:
            cpu -> a = ((cpu -> a & ~(mem -> ram [(value1 + cpu -> x) & 0x00FF])) | ((~cpu -> a) & (mem -> ram [(value1 + cpu -> x) & 0x00FF])));
            (cpu -> pc) +=2;
            break;

        case M_ABSOLUTO:
            cpu -> a = ((cpu -> a & (~mem -> ram [position])) | ((~cpu -> a) & mem -> ram [position]));
            (cpu ->pc) +=3;
            break;

        case M_INDICEX:
            cpu -> a = ((cpu -> a & (~mem -> ram [position + cpu -> x])) | ((~cpu -> a) & mem -> ram [position + cpu -> x]));
            (cpu ->pc) +=3;
            break;

        case M_INDICEY:
            cpu -> a = ((cpu -> a & (~mem -> ram [position + cpu -> y])) | ((~cpu -> a) & mem -> ram [position + cpu -> y]));
            (cpu ->pc) +=3;
            break;

        case M_INDINDEX:
            position1 = mem -> ram[value1];
            position2 = mem -> ram[value1 + 1];
            position = (position2 << 8) + position1;
            cpu -> a = ((cpu -> a & (~mem -> ram [position + cpu -> y])) | ((~cpu -> a) & mem -> ram [value1 + cpu -> y]));
            (cpu -> pc) +=2;
            break;

        case M_INDEXINDI:
            position1 = mem -> ram[ value1 + cpu -> x ];
            position2 = mem -> ram[ value1 + cpu -> x + 1];
            position = (position2 << 8) + position1;
            cpu -> a = ((cpu -> a & (~mem -> ram [position])) | ((~cpu -> a) & mem -> ram [position]));
            (cpu ->pc) +=2;
            break;
    }
    flagN(cpu ->a);
    flagZ(cpu ->a);
}



/*
INC  Increment Memory by One

    M + 1 -> M                   N Z C I D V
                                 + + - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    zeropage      INC oper     E6    2    5
    zeropage,X    INC oper,X   F6    2    6
    absolute      INC oper     EE    3    6
    absolute,X    INC oper,X   FE    3    7
*/
void INC(int mode, uint8_t value1, uint8_t value2){
    int position;
    position = (value2 << 8) + value1;

    if (value2 == 0xff || value2 == 0x01){
        printf("INC: No se puede acceder a la página uno (0x01) o 256 (0xff) \n");
        return;
    }
    uint8_t aux;
    switch (mode){
        case M_PCERO:
            mem -> ram[value1] = mem -> ram[value1] + 1;
            aux = mem -> ram[value1];
            (cpu -> pc) += 2;
            break;

        case M_PCEROX:
            mem -> ram[(value1+cpu -> x) & 0x00FF] += 1;
            aux = mem -> ram[(value1 + cpu -> x) & 0x00FF];
            (cpu -> pc) += 2;
            break;

        case M_ABSOLUTO:
            mem -> ram[position] += 1;
            aux = mem -> ram[position];
            (cpu -> pc) += 3;
            break;

        case M_INDICEX:
            mem -> ram[position+cpu -> x] += 1;
            aux = mem -> ram[position + cpu -> x];
            (cpu -> pc) += 3;
            break;
    }
    flagZ(aux);
    flagN(aux);
}

/*
INX  Increment Index X by One

    X + 1 -> X                   N Z C I D V
                                 + + - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       INX          E8    1    2
*/
void INX()
{
    (cpu -> pc) += 1;
    (cpu -> x) = (cpu -> x) + 1;
    flagN(cpu -> x);
    flagZ(cpu -> x);
}

/*
INY  Increment Index Y by One

    Y + 1 -> Y                   N Z C I D V
                                 + + - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       INY          C8    1    2
*/
void INY()
{
    (cpu -> pc) += 1;
    (cpu -> y) = (cpu -> y) + 1;
    flagN(cpu -> y);
    flagZ(cpu -> y);
}

/*
JMP  Jump to New Location

    (PC+1) -> PCL                N Z C I D V
    (PC+2) -> PCH                - - - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    absolute      JMP oper     4C    3    3
    indirect      JMP (oper)   6C    3    5
*/
void JMP(int mode, uint16_t direction){
    uint8_t lower, upper;

    switch(mode){
        case M_ABSOLUTO:
            (cpu -> pc) = direction;
            break;

        case M_INDIRECTO:
            lower = mem -> ram[direction];
            upper = mem -> ram[direction + 1];
            (cpu -> pc) = (upper << 8) + lower;
            break;
    }
}

/*
JSR  Jump to New Location Saving Return Address

    push (PC+2),                 N Z C I D V
    (PC+1) -> PCL                - - - - - -
    (PC+2) -> PCH
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    absolute      JSR oper     20    3    6
*/
void JSR(uint16_t direction){
    uint8_t upper, lower;
    upper = (((cpu -> pc) + 3) >> 8 ) & 0x00ff;
    lower = ((cpu -> pc) + 3) & 0X00FF;
    (mem -> ram[0x0100 + (cpu -> sp)]) = upper;
    (cpu -> sp) -= 1;
    (mem -> ram[0x0100 + (cpu -> sp)]) = lower;
    (cpu -> sp) -= 1;
//    printf("Dirección a la cual se va a saltar  : %04x\n",direction);
    (cpu -> pc) = direction;
}

/*
LDA  Load Accumulator with Memory

    M -> A                      N Z C I D V
                                + + - - - -

    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    immidiate     LDA #oper    A9    2    2
    zeropage      LDA oper     A5    2    3
    zeropage,X    LDA oper,X   B5    2    4
    absolute      LDA oper     AD    3    4
    absolute,X    LDA oper,X   BD    3    4*
    absolute,Y    LDA oper,Y   B9    3    4*
    (indirect,X)  LDA (oper,X) A1    2    6
    (indirect),Y  LDA (oper),Y B1    2    5*
*/
void LDA(int mode, uint8_t value1, uint8_t value2)
{
    int position, position1, position2;
    position = (value2 << 8) + value1;
    if (value2 == 0xff || value2 == 0x01){
        printf("LDA: No se puede acceder a la página uno (0x01) o 256 (0xff) \n");
        return;
    }

    switch (mode){
        case M_INMEDIATO:
            cpu -> a = value1;
            (cpu -> pc) += 2;
            break;

        case M_PCERO:
            cpu -> a = mem -> ram[value1];
            (cpu -> pc) += 2;
            break;

        case M_PCEROX:
            cpu -> a = mem -> ram[(value1+ cpu -> x) & 0x00FF];
            (cpu -> pc) += 2;
            break;

        case M_ABSOLUTO:
            cpu -> a = mem -> ram[position];
            (cpu -> pc) += 3;
            break;

        case M_INDICEX:
            cpu -> a = mem -> ram[position + cpu -> x];
            (cpu -> pc) += 3;
            break;

        case M_INDICEY:
            cpu -> a = mem -> ram[position + cpu -> y];
            (cpu -> pc) += 3;
            break;

        case M_INDINDEX:
            //usa y
            position1 = mem -> ram[value1];
            position2 = mem -> ram[value1 + 1];
            position = position2 << 8 + position1;
            cpu -> a = mem -> ram[position + cpu -> y];
            (cpu -> pc) += 2;
            break;

        case M_INDEXINDI:
            //usa x
            position1 = mem -> ram[value1 + cpu -> x];
            position2 = mem -> ram[value1 + cpu -> x + 1];
            position = position2 << 8 + position1;
            cpu -> a = mem -> ram[position];
            (cpu -> pc) += 2;
            break;
    }
    flagZ(cpu ->a);
    flagN(cpu ->a);
}


/*
LDX  Load Index X with Memory

    M -> X                      N Z C I D V
                                + + - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    immidiate     LDX #oper    A2    2    2
    zeropage      LDX oper     A6    2    3
    zeropage,Y    LDX oper,Y   B6    2    4
    absolute      LDX oper     AE    3    4
    absolute,Y    LDX oper,Y   BE    3    4*
*/
void LDX(int mode, uint8_t value1, uint8_t value2){
    int position;
    position = (value2 << 8) + value1;
    if (value2 == 0xff || value2 == 0x01){
        printf("LDX: No se puede acceder a la página uno (0x01) o 256 (0xff) \n");
        return;
    }
    switch(mode){
        case M_INMEDIATO:
            cpu -> x = value1;
            cpu -> pc += 2;
            break;

        case M_PCERO:
            cpu -> x = mem -> ram[value1];
            cpu -> pc += 2;
            break;

        case M_PCEROY:
            cpu -> x = mem -> ram[(value1 + cpu -> y) & 0x00FF];
            cpu -> pc += 2;
            break;

        case M_ABSOLUTO:
            cpu -> x = mem -> ram[position];
            cpu -> pc += 3;
            break;

        case M_INDICEY:
            cpu -> x = mem -> ram[position + cpu -> y];
            cpu -> pc += 3;
            break;
    }

    flagZ(cpu -> x);
    flagN(cpu -> x);
}

/*
LDY  Load Index Y with Memory

    M -> Y                      N Z C I D V
                                + + - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    immidiate     LDY #oper    A0    2    2
    zeropage      LDY oper     A4    2    3
    zeropage,X    LDY oper,X   B4    2    4
    absolute      LDY oper     AC    3    4
    absolute,X    LDY oper,X   BC    3    4*
*/
void LDY(int mode, uint8_t value1, uint8_t value2){
    int position;
    position = (value2 << 8) + value1;
    if (value2 == 0xff || value2 == 0x01){
        printf("LDY: No se puede acceder a la página uno (0x01) o 256 (0xff) \n");
        return;
    }
    switch(mode){
        case M_INMEDIATO:
            cpu -> y = value1;
            cpu -> pc += 2;
            break;

        case M_PCERO:
            cpu -> y = mem -> ram[value1];
            cpu ->pc += 2;
            break;

        case M_PCEROX:
            cpu -> y = mem -> ram[(value1 + cpu -> x) & 0x00FF];
            cpu -> pc += 2;
            break;

        case M_ABSOLUTO:
            cpu -> y = mem -> ram[position];
            cpu -> pc += 3;
            break;

        case M_INDICEX:
            cpu -> y = mem -> ram[position + cpu -> x];
            cpu -> pc += 3;
            break;
    }
    flagZ(cpu -> y);
    flagN(cpu -> y);
}

/*
NOP  No Operation

    ---                        N Z C I D V
                               - - - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       NOP          EA    1    2
*/
void NOP()
{
    (cpu -> pc) += 1;
}

/*
LSR  Shift One Bit Right (Memory or Accumulator)

    0 -> [76543210] -> C           N Z C I D V
                                   0 + + - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    accumulator   LSR A        4A    1    2
    zeropage      LSR oper     46    2    5
    zeropage,X    LSR oper,X   56    2    6
    absolute      LSR oper     4E    3    6
    absolute,X    LSR oper,X   5E    3    7
*/
void LSR(int mode, uint8_t value1, uint8_t value2){
    int position;
    position = (value2 << 8) + value1;
    if (value2 == 0xff || value2 == 0x01){
        printf("LSR: No se puede acceder a la página uno (0x01) o 256 (0xff) \n");
        return;
    }
    uint8_t aux;

    switch (mode){
        case M_ACUMULADOR:
            aux = (cpu -> a) & (0b00000001);
            cpu -> a >>= 1;
            cpu -> pc += 1;
            break;

        case M_PCERO:
            aux = (mem -> ram[value1] & 0b00000001);
            cpu -> a = (mem -> ram[value1] >> 1);
            cpu -> pc += 2;
            break;

        case M_PCEROX:
            aux = (mem -> ram[value1 + cpu -> x] & 0b00000001);
            cpu -> a = mem -> ram[value1 + cpu -> x] >> 1;
            cpu -> pc += 2;
            break;

        case M_ABSOLUTO:
            aux = (mem -> ram[position] & 0b00000001);
            cpu -> a = mem -> ram[position] >> 1;
            cpu -> pc += 3;
            break;
        case M_INDICEX:
            aux = (mem -> ram[position + cpu -> x] & 0b00000001);
            cpu -> a = (mem -> ram[position + cpu -> x]) >> 1;
            cpu -> pc += 3;
            break;
    }

    flagN(cpu -> a);
    flagZ(cpu -> a);
    set_flag(FLAG_C, aux);
}

/*
ORA  OR Memory with Accumulator

    A OR M -> A                  N Z C I D V
                                 + + - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    immidiate     ORA #oper    09    2    2
    zeropage      ORA oper     05    2    3
    zeropage,X    ORA oper,X   15    2    4
    absolute      ORA oper     0D    3    4
    absolute,X    ORA oper,X   1D    3    4*
    absolute,Y    ORA oper,Y   19    3    4*
    (indirect,X)  ORA (oper,X) 01    2    6
    (indirect),Y  ORA (oper),Y 11    2    5*
*/
void ORA(int mode, uint8_t value1, uint8_t value2){
    int position, position1, position2;
    position = (value2 << 8) + value1;
    if (value2 == 0xff || value2 == 0x01){
//        printf("ORA: No se puede acceder a la página uno (0x01) o 256 (0xff) \n");
        return;
    }
    switch(mode){
        case M_INMEDIATO :
            cpu -> a = cpu -> a | value1;
            (cpu -> pc) +=2;
            break;

        case M_PCERO :
            cpu -> a = cpu -> a | mem -> ram[value1];
            (cpu -> pc) +=2;
            break;

        case M_PCEROX :
            cpu -> a = cpu -> a | (mem -> ram[(value1+ cpu -> x) & 0x00FF] );
            (cpu -> pc) +=2;
            break;
        case M_ABSOLUTO :
            cpu -> a = cpu ->a | (mem -> ram[position]);
            (cpu -> pc) += 3;
            break;
        case M_INDICEX :
            cpu -> a = cpu ->a | (mem -> ram[position + cpu -> x]);
            (cpu -> pc) +=3;
            break;

        case M_INDICEY :
            cpu -> a = cpu ->a | (mem -> ram[position + cpu -> y]);
            (cpu -> pc) +=3;
            break;

        case M_INDINDEX :
            position1 = mem -> ram[ value1 ];
            position2 = mem -> ram[ value1+ 1];
            position = (position2 << 8) + position1;
            cpu -> a = cpu -> a | (mem -> ram[position + cpu -> y]);
            (cpu -> pc) +=2;
            break;

        case M_INDEXINDI :
            position1 = mem -> ram[ value1 + cpu ->x];
            position2 = mem -> ram[ value1+ cpu ->x + 1];
            position = (position2 << 8) + position1;
            cpu -> a = cpu -> a | (mem -> ram[position]);
            (cpu -> pc) +=2;
            break;
    }
    flagN(cpu ->a);
    flagZ(cpu ->a);
}

/*
PHA  Push Accumulator on Stack

    push A                      N Z C I D V
                                - - - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       PHA          48    1    3
*/
void PHA()
{
    (cpu -> pc) += 1;
    (mem -> ram[0x0100 + (cpu -> sp)]) = (cpu -> a);
    (cpu -> sp) -= 1;
}

/*
PHP  Push Processor Status on Stack

    push SR                     N Z C I D V
                                - - - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       PHP          08    1    3
*/
void PHP()
{
    (mem -> ram[0x0100 + (cpu -> sp)]) = (cpu -> sr);
    (cpu -> sp) -= 1;
    (cpu -> pc) += 1;
}

/*
PLA  Pull Accumulator from Stack

    pull A                      N Z C I D V
                                + + - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       PLA          68    1    4
*/
void PLA()
{
    (cpu -> a) = (mem -> ram[0x0100 + (cpu -> sp)]);
    (cpu -> sp) += 1;
    (cpu -> pc) += 1;
}

/*
PLP  Pull Processor Status from Stack

    pull SR                     N Z C I D V
                                from stack
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       PLP          28    1    4
*/
void PLP(){
    (cpu -> sr) = mem -> ram[0x0100+(cpu -> sp)];
    (cpu -> sp) += 1;
    (cpu -> pc) += 1;
}

/*
ROL  Rotate One Bit Left (Memory or Accumulator)

    C <- [76543210] <- C           N Z C I D V
                                   + + + - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    accumulator   ROL A        2A    1    2
    zeropage      ROL oper     26    2    5
    zeropage,X    ROL oper,X   36    2    6
    absolute      ROL oper     2E    3    6
    absolute,X    ROL oper,X   3E    3    7
*/
void ROL(int mode, uint8_t value1, uint8_t value2){
    int position;
    position = (value2 << 8) + value1;
    if (value2 == 0xff || value2 == 0x01){
        printf("ROL: No se puede acceder a la página uno (0x01) o 256 (0xff) \n");
        return;
    }
    int numUlt, carry;
    carry= (cpu ->sr & FLAG_C); // Carry
    switch (mode){
        case M_ACUMULADOR:
            numUlt = cpu -> a & 0b10000000;
            cpu -> a = (cpu -> a << 1) + (carry);
            (cpu ->pc) += 1;
            break;
        case M_PCERO :
            numUlt=(mem -> ram[ value1] & 0b10000000);
            (mem -> ram[value1]) =  ((mem -> ram[value1]) << 1) + (carry);
            (cpu ->pc) += 2;
            break;
        case M_PCEROX :
            numUlt=(mem -> ram[(value1 + cpu -> x) & 0x00FF] & 0b10000000);
            (mem -> ram[(value1 + cpu -> x) & 0x00FF])  = ((mem -> ram[(value1 + cpu -> x) & 0x00FF]) << 1) + (carry);
            (cpu -> pc) += 2;
            break;
        case M_ABSOLUTO :
            numUlt = (mem -> ram[position] & 0b10000000);
            (mem -> ram[position]) = ((mem -> ram[position]) << 1) + (carry);
            (cpu -> pc) += 3;
            break;
        case M_INDICEX :
            numUlt=(mem -> ram[(position + cpu ->x) ] & 0b10000000);
            (mem -> ram[position + cpu -> x]) = ((mem -> ram[position + cpu -> x]) << 1 ) + (carry);
            (cpu -> pc) += 3;
            break;
    }
    set_flag(FLAG_C, numUlt >> 7);
    flagN(cpu -> a);
    flagZ(cpu -> a);
}

/*
ROR  Rotate One Bit Right (Memory or Accumulator)

    C -> [76543210] -> C           N Z C I D V
                                   + + + - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    accumulator   ROR A        6A    1    2
    zeropage      ROR oper     66    2    5
    zeropage,X    ROR oper,X   76    2    6
    absolute      ROR oper     6E    3    6
    absolute,X    ROR oper,X   7E    3    7
*/
void ROR(int mode, uint8_t value1, uint8_t value2){
    int position;
    position = (value2 << 8) + value1;
    if (value2 == 0xff || value2 == 0x01){
        printf("ROR: No se puede acceder a la página uno (0x01) o 256 (0xff) \n");
        return;
    }
    int numUlt, carry;
    carry= (cpu -> sr & FLAG_C) ;
    switch (mode){
        case M_ACUMULADOR: // ROR  del acumulador
            numUlt = (cpu -> a & FLAG_C);
            cpu -> a = (carry << 7) + (cpu -> a >> 1);
            (cpu -> pc) += 1;
            break;

        case M_PCERO :
            numUlt = (mem -> ram[value1] & FLAG_C);
            (mem -> ram[value1]) = (carry << 7) + ((mem -> ram[value1]) >> 1);
            (cpu -> pc) += 2;
            break;

        case M_PCEROX :
            numUlt = (mem -> ram[( value1 + cpu -> x) & 0x00FF] & FLAG_C);
            (mem -> ram[value1 + cpu -> x]) = (carry << 7) + ((mem -> ram[value1 + cpu -> x]) >> 1);
            (cpu -> pc) += 2;
            break;

        case M_ABSOLUTO :
            numUlt = (mem -> ram[position] & FLAG_C);
            (mem -> ram[position]) = (carry << 7 ) + ((mem -> ram[position]) >> 1);
            (cpu -> pc) += 3;
            break;

        case M_INDICEX :
            numUlt=(mem -> ram[( position + cpu ->x) ] & FLAG_C);
            (mem -> ram[ position + cpu ->x]) = (carry << 7) + ((mem -> ram[ position + cpu ->x]) >> 1);
            (cpu -> pc) += 3;
            break;
    }
    flagN(cpu -> a);
    flagZ(cpu -> a);
    set_flag(FLAG_C, numUlt);
}

/*
RTI  Return from Interrupt

    pull SR, pull PC              N Z C I D V
                                  from stack
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       RTI          40    1    6
*/
void RTI(){
    PLP(); //pull SR
    (cpu -> pc) = mem -> ram[0x0100 + (cpu -> sp)];
    (cpu -> sp) += 1;
    (cpu -> pc) += 1;
}

/*
RTS  Return from Subroutine

    pull PC, PC+1 -> PC            N Z C I D V
                                   - - - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       RTS          60    1    6
*/
void RTS(){
    (cpu -> sp) += 1;
    uint8_t lower = mem -> ram[0x0100 + (cpu -> sp)];
    (cpu -> sp) += 1;
    uint8_t upper = mem -> ram[0x0100 + (cpu -> sp)];
    uint16_t direction = (upper << 8) + lower;
    (cpu -> pc) = direction;
//    printf("Direction de regreso : %04x\n",direction);
}

/*
SBC  Subtract Memory from Accumulator with Borrow

    A - M - C -> A                 N Z C I D V
                                   + + + - - +
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    immidiate     SBC #oper    E9    2     2
    zeropage      SBC oper     E5    2     3
    zeropage,X    SBC oper,X   F5    2     4
    absolute      SBC oper     ED    3     4
    absolute,X    SBC oper,X   FD    3     4*
    absolute,Y    SBC oper,Y   F9    3     4*
    (indirect,X)  SBC (oper,X) E1    2     6
    (indirect),Y  SBC (oper),Y F1    2     5*
*/
void SBC (int mode, uint8_t value1, uint8_t value2) {
    int position, position1, position2;
    position = (value2 << 8) + value1;
    if (value2 == 0xff || value2 == 0x01){
        printf("SBC: No se puede acceder a la página uno (0x01) o 256 (0xff) \n");
        return;
    }
    uint8_t carry = ((cpu -> sr) & FLAG_C);
    cpu -> a -= (1 - carry);
    uint8_t resta, aux_a = cpu -> a;
    uint16_t total;

    switch (mode){
        case M_INMEDIATO:
            resta = value1;
            cpu -> a -= resta;
            (cpu -> pc) += 2;
            break;

        case M_PCERO:
            resta = mem -> ram[value1];
            cpu -> a -= resta;
            (cpu -> pc) += 2;
            break;

        case M_PCEROX:
            resta = mem -> ram[(value1 + cpu -> x) & 0x00FF];
            cpu -> a -= resta;
            (cpu -> pc) += 2;
            break;

        case M_ABSOLUTO:
            resta = mem -> ram[position];
            cpu -> a -= resta;
            (cpu -> pc) += 3;
            break;
        case M_INDICEX:
            resta = mem -> ram[position + cpu -> x];
            cpu -> a -= resta;
            (cpu -> pc) += 3;
            break;
        case M_INDICEY:
            resta = mem -> ram[position + cpu -> y];
            cpu -> a -= resta;
            (cpu -> pc) +=3;
            break;
        case M_INDINDEX: // y
            position1 = mem -> ram[value1];
            position2 = mem -> ram[value1 + 1];
            position = (position2 << 8) + position1;
            resta = mem -> ram[position + cpu -> y];
            cpu -> a -= resta;
            (cpu -> pc) += 2;
            break;
        case M_INDEXINDI: // X
            position1 = mem -> ram[value1 + cpu -> x];
            position2 = mem -> ram[value1 + cpu -> x + 1];
            position = (position2 << 8) + position1;
            resta = mem -> ram[position];
            cpu -> a -= resta;
            (cpu->pc) +=2;
            break;
    }
    flagN(cpu->a);
    flagZ(cpu->a);
    total = aux_a - resta;
    flagC(total);
    flagV(aux_a, resta, total);
}

/*
SEC  Set Carry Flag

    1 -> C                      N Z C I D V
                                - - 1 - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       SEC          38    1    2
*/
void SEC(){
    (cpu -> pc) += 1;
    set_flag(FLAG_C, 1);
}

/*
SED  Set Decimal Flag

    1 -> D                      N Z C I D V
                                - - - - 1 -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       SED          F8    1    2
*/
void SED(){
    (cpu -> pc) += 1;
    set_flag(FLAG_D, 1);
}

/*
SEI  Set Interrupt Disable Status

    1 -> I                      N Z C I D V
                                - - - 1 - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       SEI          78    1    2
*/
void SEI(){
    (cpu -> pc) += 1;
    set_flag(FLAG_I, 1);
}

/*
STA  Store Accumulator in Memory

    A -> M                      N Z C I D V
                                - - - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    zeropage      STA oper     85    2    3
    zeropage,X    STA oper,X   95    2    4
    absolute      STA oper     8D    3    4
    absolute,X    STA oper,X   9D    3    5
    absolute,Y    STA oper,Y   99    3    5
    (indirect,X)  STA (oper,X) 81    2    6
    (indirect),Y  STA (oper),Y 91    2    6
*/
void STA(int mode ,uint8_t value1, uint8_t value2){
    int position, position1, position2;
    position = (value2 << 8) + value1;
    if (value2 == 0xff || value2 == 0x01){
        printf("STA: No se puede acceder a la página uno (0x01) o 256 (0xff) \n");
        return;
    }
    switch (mode){
        case M_PCERO:
            mem -> ram[value1] = cpu -> a;
            cpu -> pc += 2;
            break;

        case M_PCEROX:
            mem -> ram[(value1 + cpu -> x)& 0x00FF] = cpu -> a;
            (cpu -> pc) += 2;
            break;

        case M_ABSOLUTO:
            mem -> ram[position] = cpu -> a;
            (cpu -> pc) += 3;
            break;

        case M_INDICEX:
            mem -> ram[position + cpu -> x] = cpu -> a;
            cpu -> pc += 3;
            break;

        case M_INDICEY:
            mem -> ram[position + cpu -> y] = cpu -> a;
            cpu -> pc += 3;
            break;

        case M_INDINDEX:
            position1 = mem -> ram[value1];
            position2 = mem -> ram[value1 + 1];
            position = (position2 << 8) + position1;
            mem -> ram [mem -> ram[position + cpu -> y]] = cpu -> a;
            cpu -> pc += 2;
            break;

        case M_INDEXINDI:
            position1 = mem -> ram[value1 + cpu -> x];
            position2 = mem -> ram[value1 + cpu -> x + 1];
            position = (position2 << 8) + position1;
            mem -> ram[mem -> ram[position]] = cpu -> a;
            cpu -> pc += 2;
            break;
    }
}

/*
STX  Store Index X in Memory

    X -> M                      N Z C I D V
                                - - - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    zeropage      STX oper     86    2    3
    zeropage,Y    STX oper,Y   96    2    4
    absolute      STX oper     8E    3    4
*/
void STX (int mode, uint8_t value1, uint8_t value2) {
    int position;
    position = (value2 << 8) + value1;
    if (value2 == 0xff || value2 == 0x01){
        printf("STX: No se puede acceder a la página uno (0x01) o 256 (0xff) \n");
        return;
    }
    switch (mode)
    {
        case M_PCERO:
            mem -> ram[value1] = cpu -> x;
            (cpu -> pc) += 2;
            break;

        case M_PCEROY:
            mem -> ram[(value1 + cpu -> y) & 0x00FF] = cpu -> x;
            (cpu -> pc) += 2;
            break;

        case M_ABSOLUTO:
            mem -> ram[position] = cpu -> x;
            (cpu -> pc) += 3;
            break;
    }
}

/*
STY  Sore Index Y in Memory

    Y -> M                      N Z C I D V
                                - - - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    zeropage      STY oper     84    2    3
    zeropage,X    STY oper,X   94    2    4
    absolute      STY oper     8C    3    4
*/
void STY(int mode, uint8_t value1, uint8_t value2){
    int position;
    position = (value2 << 8) + value1;
    if (value2 == 0xff || value2 == 0x01){
        printf("STY: No se puede acceder a la página uno (0x01) o 256 (0xff) \n");
        return;
    }
    switch(mode){
        case M_PCERO :
            mem -> ram[value1] = cpu -> y;
            (cpu -> pc) +=2;
            break;

        case M_PCEROX :
            mem -> ram[(value1 + cpu -> x) & 0x00FF] = cpu -> y;
            (cpu -> pc) +=2;
            break;

        case M_ABSOLUTO :
            mem -> ram[position] = cpu -> y;
            (cpu -> pc) +=3;
            break;
    }
}

/*
TAX  Transfer Accumulator to Index X

    A -> X                      N Z C I D V
                                + + - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       TAX          AA    1    2
*/
void TAX(){
    (cpu -> x) = (cpu -> a);
    flagN(cpu -> x);
    flagZ(cpu -> x);
    (cpu -> pc) += 1;
}

/*
TAY  Transfer Accumulator to Index Y

    A -> Y                      N Z C I D V
                                + + - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       TAY          A8    1    2
*/
void TAY(){
    (cpu -> y) = (cpu -> a);
    flagN(cpu -> y);
    flagZ(cpu -> y);
    (cpu -> pc) += 1;
}

/*
TSX  Transfer Stack Pointer to Index X

    SP -> X                     N Z C I D V
                                + + - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       TSX          BA    1    2
*/
void TSX(){
    (cpu -> pc) += 1;
    (cpu -> x) = (cpu -> sp);
    flagN(cpu -> x);
    flagZ(cpu -> x);
}

/*
TXA  Transfer Index X to Accumulator

    X -> A                      N Z C I D V
                                + + - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       TXA          8A    1    2
*/
void TXA(){
    (cpu -> a) = (cpu -> x);
    flagN(cpu -> a);
    flagZ(cpu -> a);
    (cpu -> pc) +=1;
}

/*
TXS  Transfer Index X to Stack Register

    X -> SP                     N Z C I D V
                                - - - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       TXS          9A    1    2
*/
void TXS(){
    (cpu -> sp) = (cpu -> x);
    (cpu -> pc) += 1;
}

/*
TYA  Transfer Index Y to Accumulator

    Y -> A                      N Z C I D V
                                + + - - - -
    addressing    assembler    opc  bytes  cyles
    --------------------------------------------
    implied       TYA          98    1    2
*/
void TYA(){
    (cpu -> a) = (cpu -> y);
    flagN(cpu -> a);
    flagZ(cpu -> a);
    (cpu -> pc) += 1;
}
