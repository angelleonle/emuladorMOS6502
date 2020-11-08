#include <inttypes.h>

#ifndef EMULADOR_6502_INSTRUCTIONS_H
#define EMULADOR_6502_INSTRUCTIONS_H

/**** PROTOTIPOS DE INSTRUCCIONES ****/
/**** INSTRUCCIONES DE ANGEL ****/

// BRK  Force Break
void BRK();
// CLC  Clear Carry Flag
void CLC();
// CLD  Clear Decimal Mode
void CLD();
// CLI  Clear Interrupt Disable Bit
void CLI();
// CLV  Clear Overflow Flag
void CLV();
// DEX  Decrement Index X by One
void DEX();
// DEY  Decrement Index Y by One
void DEY();
// INX  Increment Index X by One
void INX();
// INY  Increment Index Y by One
void INY();
// NOP  No Operation
void NOP();
// PHA  Push Accumulator on Stack
void PHA();
// PHP  Push Processor Status on Stack
void PHP();
// PLA  Pull Accumulator from Stack
void PLA();

/**** INSTRUCCIONES DE KIMY ****/
//BCC  Branch on Carry Clear
void BCC(uint8_t);
//BCS  Branch on Carry Set
void BCS(uint8_t);
//BEQ  Branch on equal (zero set)
void BEQ(uint8_t);
//BMI  Branch on minus (negative set)
void BMI(uint8_t);
//BNE  Branch on not equal (zero clear)
void BNE(uint8_t);
//BPL  Branch on plus (negative clear)
void BPL(uint8_t);
//BVC  Branch on overflow clear
void BVC(uint8_t);
//BVS  Branch on overflow set
void BVS(uint8_t);
//JMP Jump
void JMP(int, uint16_t);
//JSR Jump subroutine
void JSR(uint16_t);

/**** INSTRUCCIONES DE CARLOS ****/
//AND  AND Memory with Accumulator
void AND(int,uint8_t,uint8_t);
//LDX  Load Index X with Memory
void LDX(int, uint8_t,uint8_t);
//INC  Increment Memory by One
void INC(int , uint8_t, uint8_t);

void CPY(int , uint8_t, uint8_t);

void LSR(int , uint8_t , uint8_t);

void EOR(int , uint8_t , uint8_t);

//PLP Pull Processor Status from Stack
void PLP();
//RTI  Return from Interrupt
void RTI();
//RTS  Return from Subroutine
void RTS();
//SEC  Set Carry Flag
void SEC();
//SED  Set Decimal Flag
void SED();
//SEI  Set Interrupt Disable Status
void SEI();
//TAX  Transfer Accumulator to Index X
void TAX();
//TAY  Transfer Accumulator to Index Y
void TAY();
//TSX  Transfer Stack Pointer to Index X
void TSX();
//TXA  Transfer Index X to Accumulator
void TXA();
//TXS  Transfer Index X to Stack Register
void TXS();
//TYA  Transfer Index Y to Accumulator
void TYA();

void BVC(uint8_t);

void ASL(int, uint8_t, uint8_t);
void DEC(int, uint8_t, uint8_t);
void LDA(int, uint8_t, uint8_t);
void ORA(int, uint8_t, uint8_t);
void STY(int, uint8_t, uint8_t);
void LDY(int, uint8_t, uint8_t);
void CMP(int, uint8_t, uint8_t);
void CPX(int, uint8_t, uint8_t);
void SBC(int, uint8_t, uint8_t);
void ROR(int, uint8_t, uint8_t);
void ROL(int, uint8_t, uint8_t);
void STX(int, uint8_t, uint8_t);
void ADC(int, uint8_t, uint8_t);
void STA(int, uint8_t, uint8_t);
void BIT(int, uint8_t, uint8_t);
void SBC(int, uint8_t, uint8_t);

#endif //EMULADOR_6502_INSTRUCTIONS_H
