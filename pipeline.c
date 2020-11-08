#include "pipeline.h"
#include "flag_modes.h"
#include "structures.h"
#include "instructions.h"

// Fetch : quiero leer la instruccion que quiero modificar
void fetch()
{
    (cpu -> ins) = (mem -> ram[cpu -> pc]);
}

// Decode
void decode()
{
    (cpu -> addr) = (cpu -> pc) + 1;
}

// Execute
void execute()
{
    uint8_t operator1, operator2;
    operator1 = mem -> ram[cpu -> addr];
    operator2 = mem -> ram[cpu -> addr + 1];
    uint16_t operator = (operator2 << 8) + operator1;

    switch (cpu -> ins)
    {
        // ADC
        case 0x69: ADC(M_INMEDIATO, 2, 2);break;
        case 0x65: ADC(M_PCERO, operator1, 2);break;
        case 0x75: ADC(M_PCEROX, operator1, 2);break;
        case 0x6D: ADC(M_ABSOLUTO, operator1, operator2);break;
        case 0x7D: ADC(M_INDICEX, operator1, operator2);break;
        case 0x79: ADC(M_INDICEY, operator1, operator2);break;
        case 0x61: ADC(M_INDEXINDI, operator1, operator2);break;
        case 0x71: ADC(M_INDINDEX, operator1, operator2);break;

        // AND
        case 0x29: AND(M_INMEDIATO, operator1, 2); break;
        case 0x25: AND(M_PCERO, operator1, 2); break;
        case 0x35: AND(M_PCEROX, operator1, 2); break;
        case 0x2D: AND(M_ABSOLUTO, operator1, operator2); break;
        case 0x3D: AND(M_INDICEX, operator1, operator2); break;
        case 0x39: AND(M_INDICEY, operator1, operator2); break;
        case 0x31: AND(M_INDEXINDI, operator1, 2); break;
        case 0x21: AND(M_INDINDEX,operator1, 2); break;

        // ASL
        case 0x0A: ASL(M_ACUMULADOR, 2, 2); break;
        case 0x06: ASL(M_PCERO, operator1, 2); break;
        case 0x16: ASL(M_PCEROX, operator1, 2); break;
        case 0x0E: ASL(M_ABSOLUTO, operator1, operator2); break;
        case 0x1E: ASL(M_INDICEX, operator1, operator2); break;

        // BCC
        case 0x90: BCC(operator1); break;

        // BCS
        case 0xB0: BCS(operator1); break;

        // BEQ
        case 0xF0: BEQ(operator1); break;

        // BIT
        case 0x24: BIT(M_PCERO, operator1, 2); break;
        case 0x2C: BIT(M_ABSOLUTO, operator1, operator2); break;

        // BMI
        case 0x30: BMI(operator1); break;

        // BNE
        case 0xD0: BNE(operator1); break;

        // BPL
        case 0x10: BPL(operator1); break;

        // BRK
        case 0x00: BRK(); break;

        // BVC
        case 0x50: BVC(operator1); break;

        // BVS
        case 0x70: BVS(operator1); break;

        // CLC
        case 0x18: CLC(); break;

        // CLD
        case 0xD8: CLD(); break;

        // CLI
        case 0x58: CLI(); break;

        // CLV
        case 0xB8: CLV(); break;

        // CMP
        case 0xC9: CMP(M_INMEDIATO, operator1, 2); break;
        case 0xC5: CMP(M_PCERO, operator1, 2); break;
        case 0xD5: CMP(M_PCEROX, operator1, 2); break;
        case 0xCD: CMP(M_ABSOLUTO, operator1, operator2); break;
        case 0xDD: CMP(M_INDICEX, operator1, operator2); break;
        case 0xD9: CMP(M_INDICEY, operator1, operator2); break;
        case 0xC1: CMP(M_INDEXINDI, operator1, operator2); break;
        case 0xD1: CMP(M_INDINDEX, operator1, operator2); break;

        // CPX
        case 0xE0: CPX(M_INMEDIATO, operator1, 2); break;
        case 0xE4: CPX(M_PCERO, operator1, 2); break;
        case 0xEC: CPX(M_ABSOLUTO, operator1, operator2); break;

        // CPY
        case 0xC0: CPY(M_INMEDIATO, operator1, 2); break;
        case 0xC4: CPY(M_PCERO, operator1, 2); break;
        case 0xCC: CPY(M_ABSOLUTO, operator1, operator2); break;

        // DEC
        case 0xC6: DEC(M_PCERO, operator1, 2); break;
        case 0xD6: DEC(M_PCEROX, operator1 , 2); break;
        case 0xCE: DEC(M_ABSOLUTO, operator1,  operator2); break;
        case 0xDE: DEC(M_INDICEX, operator1, operator2); break;

        // DEX
        case 0xCA : DEX(); break;

        // DEY
        case 0x88 : DEY(); break;

        // EOR
        case 0x49: EOR(M_INMEDIATO, operator1, 2); break;
        case 0x45: EOR(M_PCERO, operator1, 2); break;
        case 0x55: EOR(M_PCEROX, operator1, 2); break;
        case 0x4D: EOR(M_ABSOLUTO, operator1, operator2); break;
        case 0x5D: EOR(M_INDICEX, operator1, operator2); break;
        case 0x59: EOR(M_INDICEY, operator1, operator2); break;
        case 0x41: EOR(M_INDEXINDI, operator1, operator2); break;
        case 0x51: EOR(M_INDINDEX, operator1, operator2); break;

        // INC
        case 0xE6: INC(M_PCERO, operator1, 2); break;
        case 0xF6: INC(M_PCEROX, operator1, 2); break;
        case 0xEE: INC(M_ABSOLUTO, operator1, operator2); break;
        case 0xFE: INC(M_INDICEX, operator1, operator2); break;

        // INX
        case 0xE8: INX();break;

        // INY
        case 0xC8: INY();break;

        // JMP
        case 0x4C: JMP(M_ABSOLUTO, operator); break;
        case 0x6C: JMP(M_INDIRECTO, operator); break;

        // JSR
        case 0x20: JSR(operator); break;

        // LDA
        case 0xA9: LDA(M_INMEDIATO, operator1, 2); break;
        case 0xA5: LDA(M_PCERO, operator1, 2); break;
        case 0xB5: LDA(M_PCEROX, operator1, 2); break;
        case 0xAD: LDA(M_ABSOLUTO, operator1, operator2); break;
        case 0xBD: LDA(M_INDICEX, operator1, operator2); break;
        case 0xB9: LDA(M_INDICEY, operator1, operator2); break;
        case 0xA1: LDA(M_INDEXINDI, operator1, operator2); break;
        case 0xB1: LDA(M_INDINDEX, operator1, operator2); break;

        // LDX
        case 0xA2: LDX(M_INMEDIATO, operator1, 2); break;
        case 0xA6: LDX(M_PCERO, operator1, 2); break;
        case 0xB6: LDX(M_PCEROY, operator1, 2); break;
        case 0xAE: LDX(M_ABSOLUTO, operator1, operator2); break;
        case 0xBE: LDX(M_INDICEY, operator1, operator2); break;

        // LDY
        case 0xA0: LDY(M_INMEDIATO, operator1, 2); break;
        case 0xA4: LDY(M_PCERO, operator1, 2); break;
        case 0xB4: LDY(M_PCEROX, operator1, 2); break;
        case 0xAC: LDY(M_ABSOLUTO, operator1, operator2); break;
        case 0xBC: LDY(M_INDICEX, operator1, operator2); break;

        // LSR
        case 0x4A: LSR(M_ACUMULADOR, 2, 2); break;
        case 0x46: LSR(M_PCERO, operator1, 2); break;
        case 0x56: LSR(M_PCEROX, operator1, 2); break;
        case 0x4E: LSR(M_ABSOLUTO, operator1, operator2); break;
        case 0x5E: LSR(M_INDICEX, operator1, operator2); break;

        // NOP
        case 0xEA: NOP();break;

        // ORA
        case 0x09: ORA(M_INMEDIATO, operator1, 2); break;
        case 0x05: ORA(M_PCERO, operator1, 2); break;
        case 0x15: ORA(M_PCEROX, operator1, 2); break;
        case 0x0D: ORA(M_ABSOLUTO, operator1, operator2); break;
        case 0x1D: ORA(M_INDICEX, operator1, operator2);break;
        case 0x19: ORA(M_INDICEY, operator1, operator2); break;
        case 0x01: ORA(M_INDEXINDI, operator1, operator2); break;
        case 0x11: ORA(M_INDINDEX, operator1, operator2); break;

        // PHA
        case 0x48: PHA();break;

        // PHP
        case 0x08: PHP();break;

        // PLA
        case 0x68: PLA();break;

        // PLP
        case 0x28: PLP();break;

        // ROL
        case 0x2A: ROL(M_ACUMULADOR, operator1, 2); break;
        case 0x26: ROL(M_PCERO, operator1, 2); break;
        case 0x36: ROL(M_PCEROX, operator1, 2); break;
        case 0x2E: ROL(M_ABSOLUTO, operator1, operator2); break;
        case 0x3E: ROL(M_INDICEX, operator1, operator2); break;

        // ROR
        case 0x6A: ROR(M_ACUMULADOR, operator1, 2); break;
        case 0x66: ROR(M_PCERO, operator1, 2); break;
        case 0x76: ROR(M_PCEROX, operator1, 2); break;
        case 0x6E: ROR(M_ABSOLUTO, operator1, operator2); break;
        case 0x7E: ROR(M_INDICEX, operator1, operator2); break;

        //  RTI
        case 0x40: RTI(); break;

        // RTS
        case 0x60: RTS();break;

        // SBC
        case 0xE9: SBC(M_INMEDIATO, operator1, 2); break;
        case 0xE5: SBC(M_PCERO, operator1, 2); break;
        case 0xF5: SBC(M_PCEROX, operator1, 2); break;
        case 0xED: SBC(M_ABSOLUTO, operator1, operator2); break;
        case 0xFD: SBC(M_INDICEX, operator1, operator2); break;
        case 0xF9: SBC(M_INDICEY, operator1, operator2); break;
        case 0xE1: SBC(M_INDEXINDI, operator1, operator2); break;
        case 0xF1: SBC(M_INDINDEX, operator1, operator2); break;

        // SEC
        case 0x38: SEC(); break;

        // SED
        case 0xF8: SED(); break;

        // SEI
        case 0x78: SEI(); break;

        // STA
        case 0x85: STA(M_PCERO, operator1, 2); break;
        case 0x95: STA(M_PCEROX, operator1, 2); break;
        case 0x8D: STA(M_ABSOLUTO, operator1, operator2); break;
        case 0x9D: STA(M_INDICEX, operator1, operator2); break;
        case 0x99: STA(M_INDICEY, operator1, operator2); break;
        case 0x81: STA(M_INDEXINDI, operator1, operator2); break;
        case 0x91: STA(M_INDINDEX, operator1, operator2); break;

        // STX
        case 0x86: STX(M_PCERO, operator1, 2); break;
        case 0x96: STX(M_PCEROY, operator1, 2); break;
        case 0x8E: STX(M_ABSOLUTO, operator1, operator2); break;

        // STY
        case 0x84: STY(M_PCERO, operator1, 2); break;
        case 0x94: STY(M_PCEROX, operator1, 2); break;
        case 0x8C: STY(M_ABSOLUTO, operator1, operator2); break;

        // TAX
        case 0xAA: TAX(); break;

        // TAY
        case 0xA8: TAY(); break;

        // TSX
        case 0xBA: TSX(); break;

        // TXA
        case 0x8A: TXA(); break;

        // TXS
        case 0x9A: TXS(); break;

        // TYA
        case 0x98: TYA(); break;

        default: break;
    }
}
