#include "cpu_6502.h"
#include <iostream>
#include <cstring>
using namespace std;

//CONSTRUCTOR//
cpu_6502::cpu_6502(){
    memset(bus.RAM,0,sizeof(bus.RAM));
    INIT_TABLE();
}

//METHOD//
void cpu_6502::DEBUGSCREN(){
    cout << "<REGISTER> PC = $" << hex << +PC;
    cout << " SP = $" << hex << +SP;
    cout << " A = $" << hex << +A;
    cout << " X = $" << hex << +X;
    cout << " Y = $" << hex << +Y;
    cout << "\n<FLAGS> N = " << ST.N;
    cout << " V = " << ST.V;
    cout << " D = " << ST.D;
    cout << " I = " << ST.I;
    cout << " Z = " << ST.Z;
    cout << " C = " << ST.C;
    cout << "\n<STACK> (FROM 0x100 to 0x200)\n";
    for(uint16_t i = 0x100; i<=0x200; i++){
        cout << hex << +bus.read(i) << " ";
        if(i%25==0){
            cout << endl;
        }
    }
    cout << endl;
}

uint8_t cpu_6502::FetchByte(){
    uint8_t byte = bus.read(PC);
    PC++;
    return byte;
}

void cpu_6502::STEP(){
    uint8_t opcode = FetchByte();
    cout << "EXECUTING OPCODE: " << hex << +opcode << endl;
    (this->*table[opcode])();
}

void cpu_6502::RESET(uint16_t START, bool autoset){
    if(autoset){
        PC = (bus.read(0xFFFD) << 8) | bus.read(0xFFFC);
    }
    else{
        PC = START;
    }
    SP = 0xFD;
    ST.N = 0;
    ST.V = 0;
    ST.D = 0;
    ST.I = 1;
    ST.Z = 0;
    ST.C = 0;
    A = 0;
    X = 0;
    Y = 0;
}

void cpu_6502::INIT_TABLE(){
    for(int i = 0; i < 256; i++){
        table[i] = &cpu_6502::NOP; //FILLING NOPE FIRST
    }

    table[0x00] = &cpu_6502::BRK;
    table[0x01] = &cpu_6502::ORA_INDZPX;
    table[0x05] = &cpu_6502::ORA_ZP;
    table[0x06] = &cpu_6502::ASL_ZP;
    table[0x08] = &cpu_6502::PHP_IMPL;
    table[0x09] = &cpu_6502::ORA_IMM;
    table[0x0A] = &cpu_6502::ASL_AC;
    table[0x0D] = &cpu_6502::ORA_ABS;
    table[0x0E] = &cpu_6502::ASL_ABS;
    table[0x10] = &cpu_6502::BPL_REL;
    table[0x11] = &cpu_6502::ORA_INDZPY;
    table[0x15] = &cpu_6502::ORA_ZPX;
    table[0x16] = &cpu_6502::ASL_ZPX;
    table[0x18] = &cpu_6502::CLC_IMPL;
    table[0x19] = &cpu_6502::ORA_ABSY;
    table[0x1D] = &cpu_6502::ORA_ABSX;
    table[0x1E] = &cpu_6502::ASL_ABSX;
    table[0x20] = &cpu_6502::JSR_ABS;
    table[0x21] = &cpu_6502::AND_INDZPX;
    table[0x24] = &cpu_6502::BIT_ZP;
    table[0x25] = &cpu_6502::AND_ZP;
    table[0x26] = &cpu_6502::ROL_ZP;
    table[0x28] = &cpu_6502::PLP_IMPL;
    table[0x29] = &cpu_6502::AND_IMM;
    table[0x2A] = &cpu_6502::ROL_AC;
    table[0x2C] = &cpu_6502::BIT_ABS;
    table[0x2D] = &cpu_6502::AND_ABS;
    table[0x2E] = &cpu_6502::ROL_ABS;
    table[0x30] = &cpu_6502::BMI_REL;
    table[0x31] = &cpu_6502::AND_INDZPY;
    table[0x35] = &cpu_6502::AND_ZPX;
    table[0x36] = &cpu_6502::ROL_ZPX;
    table[0x38] = &cpu_6502::SEC_IMPL;
    table[0x39] = &cpu_6502::AND_ABSY;
    table[0x3D] = &cpu_6502::AND_ABSX;
    table[0x3E] = &cpu_6502::ROL_ABSX;
    table[0x40] = &cpu_6502::RTI_IMPL;
    table[0x41] = &cpu_6502::EOR_INDZPX;
    table[0x45] = &cpu_6502::EOR_ZP;
    table[0x46] = &cpu_6502::LSR_ZP;
    table[0x48] = &cpu_6502::PHA_IMPL;
    table[0x49] = &cpu_6502::EOR_IMM;
    table[0x4A] = &cpu_6502::LSR_AC;
    table[0x4C] = &cpu_6502::JMP_ABS;
    table[0x4D] = &cpu_6502::EOR_ABS;
    table[0x4E] = &cpu_6502::LSR_ABS;
    table[0x50] = &cpu_6502::BVC_REL;
    table[0x51] = &cpu_6502::EOR_INDZPY;
    table[0x55] = &cpu_6502::EOR_ZPX;
    table[0x56] = &cpu_6502::LSR_ZPX;
    table[0x58] = &cpu_6502::CLI_IMPL;
    table[0x59] = &cpu_6502::EOR_ABSY;
    table[0x5D] = &cpu_6502::EOR_ABSX;
    table[0x5E] = &cpu_6502::LSR_ABSX;
    table[0x60] = &cpu_6502::RTS_IMPL;
    table[0x61] = &cpu_6502::ADC_INDZPX;
    table[0x65] = &cpu_6502::ADC_ZP;
    table[0x66] = &cpu_6502::ROR_ZP;
    table[0x68] = &cpu_6502::PLA_IMPL;
    table[0x69] = &cpu_6502::ADC_IMM;
    table[0x6A] = &cpu_6502::ROR_AC;
    table[0x6C] = &cpu_6502::JMP_IND;
    table[0x6D] = &cpu_6502::ADC_ABS;
    table[0x6E] = &cpu_6502::ROR_ABS;
    table[0x70] = &cpu_6502::BVS_REL;
    table[0x71] = &cpu_6502::ADC_INDZPY;
    table[0x75] = &cpu_6502::ADC_ZPX;
    table[0x76] = &cpu_6502::ROR_ZPX;
    table[0x78] = &cpu_6502::SEI_IMPL;
    table[0x79] = &cpu_6502::ADC_ABSY;
    table[0x7D] = &cpu_6502::ADC_ABSX;
    table[0x7E] = &cpu_6502::ROR_ABSX;
    table[0x81] = &cpu_6502::STA_INDZPX;
    table[0x84] = &cpu_6502::STY_ZP;
    table[0x85] = &cpu_6502::STA_ZP;
    table[0x86] = &cpu_6502::STX_ZP;
    table[0x88] = &cpu_6502::DEY_IMPL;
    table[0x8A] = &cpu_6502::TXA_IMPL;
    table[0x8C] = &cpu_6502::STY_ABS;
    table[0x8D] = &cpu_6502::STA_ABS;
    table[0x8E] = &cpu_6502::STX_ABS;
    table[0x90] = &cpu_6502::BCC_REL;
    table[0x91] = &cpu_6502::STA_INDZPY;
    table[0x94] = &cpu_6502::STY_ZPX;
    table[0x95] = &cpu_6502::STA_ZPX;
    table[0x96] = &cpu_6502::STX_ZPY;
    table[0x98] = &cpu_6502::TYA_IMPL;
    table[0x99] = &cpu_6502::STA_ABSY;
    table[0x9A] = &cpu_6502::TXS_IMPL;
    table[0x9D] = &cpu_6502::STA_ABSX;
    table[0xA0] = &cpu_6502::LDY_IMM;
    table[0xA1] = &cpu_6502::LDA_INDZPX;
    table[0xA2] = &cpu_6502::LDX_IMM;
    table[0xA4] = &cpu_6502::LDY_ZP;
    table[0xA5] = &cpu_6502::LDA_ZP;
    table[0xA6] = &cpu_6502::LDX_ZP;
    table[0xA8] = &cpu_6502::TAY_IMPL;
    table[0xA9] = &cpu_6502::LDA_IMM;
    table[0xAA] = &cpu_6502::TAX_IMPL;
    table[0xAC] = &cpu_6502::LDY_ABS;
    table[0xAD] = &cpu_6502::LDA_ABS;
    table[0xAE] = &cpu_6502::LDX_ABS;
    table[0xB0] = &cpu_6502::BCS_REL;
    table[0xB1] = &cpu_6502::LDA_INDZPY;
    table[0xB4] = &cpu_6502::LDY_ZPX;
    table[0xB5] = &cpu_6502::LDA_ZPX;
    table[0xB6] = &cpu_6502::LDX_ZPY;
    table[0xB8] = &cpu_6502::CLV_IMPL;
    table[0xB9] = &cpu_6502::LDA_ABSY;
    table[0xBA] = &cpu_6502::TSX_IMPL;
    table[0xBC] = &cpu_6502::LDY_ABSX;
    table[0xBD] = &cpu_6502::LDA_ABSX;
    table[0xBE] = &cpu_6502::LDX_ABSY;
    table[0xC0] = &cpu_6502::CPY_IMM;
    table[0xC1] = &cpu_6502::CMP_INDZPX;
    table[0xC4] = &cpu_6502::CPY_ZP;
    table[0xC5] = &cpu_6502::CMP_ZP;
    table[0xC6] = &cpu_6502::DEC_ZP;
    table[0xC8] = &cpu_6502::INY_IMPL;
    table[0xC9] = &cpu_6502::CMP_IMM;
    table[0xCA] = &cpu_6502::DEX_IMPL;
    table[0xCC] = &cpu_6502::CPY_ABS;
    table[0xCD] = &cpu_6502::CMP_ABS;
    table[0xCE] = &cpu_6502::DEC_ABS;
    table[0xD0] = &cpu_6502::BNE_REL;
    table[0xD1] = &cpu_6502::CMP_INDZPY;
    table[0xD5] = &cpu_6502::CMP_ZPX;
    table[0xD6] = &cpu_6502::DEC_ZPX;
    table[0xD8] = &cpu_6502::CLD_IMPL;
    table[0xD9] = &cpu_6502::CMP_ABSY;
    table[0xDD] = &cpu_6502::CMP_ABSX;
    table[0xDE] = &cpu_6502::DEC_ABSX;
    table[0xE0] = &cpu_6502::CPX_IMM;
    table[0xE1] = &cpu_6502::SBC_INDZPX;
    table[0xE4] = &cpu_6502::CPX_ZP;
    table[0xE5] = &cpu_6502::SBC_ZP;
    table[0xE6] = &cpu_6502::INC_ZP;
    table[0xE8] = &cpu_6502::INX_IMPL;
    table[0xE9] = &cpu_6502::SBC_IMM;
    table[0xEA] = &cpu_6502::NOP;
    table[0xEC] = &cpu_6502::CPX_ABS;
    table[0xED] = &cpu_6502::SBC_ABS;
    table[0xEE] = &cpu_6502::INC_ABS;
    table[0xF0] = &cpu_6502::BEQ_REL;
    table[0xF1] = &cpu_6502::SBC_INDZPY;
    table[0xF5] = &cpu_6502::SBC_ZPX;
    table[0xF6] = &cpu_6502::INC_ZPX;
    table[0xF8] = &cpu_6502::SED_IMPL;
    table[0xF9] = &cpu_6502::SBC_ABSY;
    table[0xFD] = &cpu_6502::SBC_ABSX;
    table[0xFE] = &cpu_6502::INC_ABSX;
}

//ADDRESSING MODE//

uint16_t cpu_6502::Immediate(){
    return PC;
}

uint16_t cpu_6502::Absolute(){
    uint8_t LL = FetchByte();
    uint8_t HH = FetchByte();
    return (HH << 8) | LL;
}

uint16_t cpu_6502::ZeroPage(){
    uint8_t LL = FetchByte();
    uint8_t HH = 0x00;
    return (HH << 8) | LL;
}

uint16_t cpu_6502::AbsoluteX(){
    uint8_t LL = FetchByte();
    uint8_t HH = FetchByte();
    return ((HH << 8) | LL) + X;
}

uint16_t cpu_6502::AbsoluteY(){
    uint8_t LL = FetchByte();
    uint8_t HH = FetchByte();
    return ((HH << 8) | LL) + Y;
}

uint16_t cpu_6502::ZeropageX(){
    uint8_t LL = FetchByte();
    uint8_t HH = 0x00;
    return (((HH << 8) | LL) + X) & 0xFF;
}

uint16_t cpu_6502::ZeropageY(){
    uint8_t LL = FetchByte();
    uint8_t HH = 0x00;
    return (((HH << 8) | LL) + Y) & 0xFF;
}

uint16_t cpu_6502::Indirect(){ // WARNING ORIGINALY HAS ABUG THAT MUST BE IMPLEMENTED FOR FEDELITY IN THE FUTURE
    uint8_t LL = FetchByte();
    uint8_t HH = FetchByte();
    uint16_t addr = (HH << 8) | LL;
    LL = bus.read(addr);
    HH = bus.read(addr+1);
    return (HH << 8) | LL;
}

uint16_t cpu_6502::IndirectZPX(){
    uint8_t LL = FetchByte();
    uint8_t HH = 0x00;
    uint16_t addr = (((HH << 8) | LL) + X) & 0xFF;
    LL = bus.read(addr);
    HH = bus.read((addr+1) & 0xFF);
    return (HH << 8) | LL;
}

uint16_t cpu_6502::IndirectZPY(){
    uint8_t LL = FetchByte();
    uint8_t HH = 0x00;
    uint16_t addr = (HH << 8) | LL;
    LL = bus.read(addr);
    HH = bus.read((addr+1) & 0xFF);
    return ((HH << 8) | LL) + Y;
}

uint16_t cpu_6502::Relative(){
    int8_t offset = FetchByte();
    return PC + offset;
}

//OPCODE//

void cpu_6502::ADC_IMM(){
    uint16_t memory = Immediate();
    uint8_t val = FetchByte();
    uint16_t result = A + val + ST.C;
    ST.C = result > 0xFF;
    ST.Z = (result & 0xFF) == 0;
    ST.V = (result ^ A) & (result ^ val) & 0x80;
    ST.N = result & 0b10000000;
    A = result & 0xFF;
}

void cpu_6502::ADC_ZP(){
    uint16_t memory = ZeroPage();
    uint8_t val = bus.read(memory);
    uint16_t result = A + val + ST.C;
    ST.C = result > 0xFF;
    ST.Z = (result & 0xFF) == 0;
    ST.V = (result ^ A) & (result ^ val) & 0x80;
    ST.N = result & 0b10000000;
    A = result & 0xFF;
}

void cpu_6502::ADC_ZPX(){
    uint16_t memory = ZeropageX();
    uint8_t val = bus.read(memory);
    uint16_t result = A + val + ST.C;
    ST.C = result > 0xFF;
    ST.Z = (result & 0xFF) == 0;
    ST.V = (result ^ A) & (result ^ val) & 0x80;
    ST.N = result & 0b10000000;
    A = result & 0xFF;
}

void cpu_6502::ADC_ABS(){
    uint16_t memory = Absolute();
    uint8_t val = bus.read(memory);
    uint16_t result = A + val + ST.C;
    ST.C = result > 0xFF;
    ST.Z = (result & 0xFF) == 0;
    ST.V = (result ^ A) & (result ^ val) & 0x80;
    ST.N = result & 0b10000000;
    A = result & 0xFF;
}

void cpu_6502::ADC_ABSX(){
    uint16_t memory = AbsoluteX();
    uint8_t val = bus.read(memory);
    uint16_t result = A + val + ST.C;
    ST.C = result > 0xFF;
    ST.Z = (result & 0xFF) == 0;
    ST.V = (result ^ A) & (result ^ val) & 0x80;
    ST.N = result & 0b10000000;
    A = result & 0xFF;
}

void cpu_6502::ADC_ABSY(){
    uint16_t memory = AbsoluteY();
    uint8_t val = bus.read(memory);
    uint16_t result = A + val + ST.C;
    ST.C = result > 0xFF;
    ST.Z = (result & 0xFF) == 0;
    ST.V = (result ^ A) & (result ^ val) & 0x80;
    ST.N = result & 0b10000000;
    A = result & 0xFF;
}

void cpu_6502::ADC_INDZPX(){
    uint16_t memory = IndirectZPX();
    uint8_t val = bus.read(memory);
    uint16_t result = A + val + ST.C;
    ST.C = result > 0xFF;
    ST.Z = (result & 0xFF) == 0;
    ST.V = (result ^ A) & (result ^ val) & 0x80;
    ST.N = result & 0b10000000;
    A = result & 0xFF;
}

void cpu_6502::ADC_INDZPY(){
    uint16_t memory = IndirectZPY();
    uint8_t val = bus.read(memory);
    uint16_t result = A + val + ST.C;
    ST.C = result > 0xFF;
    ST.Z = (result & 0xFF) == 0;
    ST.V = (result ^ A) & (result ^ val) & 0x80;
    ST.N = result & 0b10000000;
    A = result & 0xFF;
}

void cpu_6502::AND_IMM(){
    uint16_t memory = Immediate();
    uint8_t val = FetchByte();
    uint8_t result = A & val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::AND_ZP(){
    uint16_t memory = ZeroPage();
    uint8_t val = bus.read(memory);
    uint8_t result = A & val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::AND_ZPX(){
    uint16_t memory = ZeropageX();
    uint8_t val = bus.read(memory);
    uint8_t result = A & val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::AND_ABS(){
    uint16_t memory = Absolute();
    uint8_t val = bus.read(memory);
    uint8_t result = A & val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::AND_ABSX(){
    uint16_t memory = AbsoluteX();
    uint8_t val = bus.read(memory);
    uint8_t result = A & val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::AND_ABSY(){
    uint16_t memory = AbsoluteY();
    uint8_t val = bus.read(memory);
    uint8_t result = A & val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::AND_INDZPX(){
    uint16_t memory = IndirectZPX();
    uint8_t val = bus.read(memory);
    uint8_t result = A & val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::AND_INDZPY(){
    uint16_t memory = IndirectZPY();
    uint8_t val = bus.read(memory);
    uint8_t result = A & val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::ASL_AC(){
    uint8_t result = A << 1;
    ST.C = A & 0b10000000;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::ASL_ZP(){
    uint16_t memory = ZeroPage();
    uint8_t val = bus.read(memory);
    uint8_t result = val << 1;
    ST.C = val & 0b10000000;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    bus.write(memory,result);
}

void cpu_6502::ASL_ZPX(){
    uint16_t memory = ZeropageX();
    uint8_t val = bus.read(memory);
    uint8_t result = val << 1;
    ST.C = val & 0b10000000;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    bus.write(memory,result);
}

void cpu_6502::ASL_ABS(){
    uint16_t memory = Absolute();
    uint8_t val = bus.read(memory);
    uint8_t result = val << 1;
    ST.C = val & 0b10000000;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    bus.write(memory,result);
}

void cpu_6502::ASL_ABSX(){
    uint16_t memory = AbsoluteX();
    uint8_t val = bus.read(memory);
    uint8_t result = val << 1;
    ST.C = val & 0b10000000;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    bus.write(memory,result);
}

void cpu_6502::BCC_REL(){
    uint16_t Targhet = Relative();
    if(ST.C == 0){
        PC = Targhet;
    }
}

void cpu_6502::BCS_REL(){
    uint16_t Targhet = Relative();
    if(ST.C == 1){
        PC = Targhet;
    }
}

void cpu_6502::BEQ_REL(){
    uint16_t Targhet = Relative();
    if(ST.Z == 1){
        PC = Targhet;
    }
}

void cpu_6502::BIT_ZP(){
    uint16_t memory = ZeroPage();
    uint8_t val = bus.read(memory);
    uint8_t result = A & val;
    ST.Z = result == 0;
    ST.V = val & 0b01000000;
    ST.N = val & 0b10000000;
}

void cpu_6502::BIT_ABS(){
    uint16_t memory = Absolute();
    uint8_t val = bus.read(memory);
    uint8_t result = A & val;
    ST.Z = result == 0;
    ST.V = val & 0b01000000;
    ST.N = val & 0b10000000;
}

void cpu_6502::BMI_REL(){
    uint16_t Targhet = Relative();
    if(ST.N == 1){
        PC = Targhet;
    }
}

void cpu_6502::BNE_REL(){
    uint16_t Targhet = Relative();
    if(ST.Z == 0){
        PC = Targhet;
    }
}

void cpu_6502::BPL_REL(){
    uint16_t Targhet = Relative();
    if(ST.N == 0){
        PC = Targhet;
    }
}

void cpu_6502::BRK(){
    uint16_t ret = PC + 1;
    bus.write(0x0100 | SP, (ret >> 8) & 0xFF);
    SP--;
    bus.write(0x0100 | SP, ret & 0xFF);
    SP--;
    uint8_t status = (ST.N << 7) | (ST.V << 6) | (1 << 5) | (1 << 4) | (ST.D << 3) | (ST.I << 2) | (ST.Z << 1) | ST.C;
    bus.write(0x0100 | SP, status);
    SP--;
    ST.I = 1;
    uint8_t lo = bus.read(0xFFFE);
    uint8_t hi = bus.read(0xFFFF);
    PC = (hi << 8) | lo;
}

void cpu_6502::BVC_REL(){
    uint16_t Targhet = Relative();
    if(ST.V == 0){
        PC = Targhet;
    }
}

void cpu_6502::BVS_REL(){
    uint16_t Targhet = Relative();
    if(ST.V == 1){
        PC = Targhet;
    }
}

void cpu_6502::CLC_IMPL(){
    ST.C = 0;
}

void cpu_6502::CLD_IMPL(){
    ST.D = 0;
}

void cpu_6502::CLI_IMPL(){
    ST.I = 0;
}

void cpu_6502::CLV_IMPL(){
    ST.V = 0;
}

void cpu_6502::CMP_IMM(){
    uint16_t memory = Immediate();
    uint8_t val = FetchByte();
    uint8_t result = A - val;
    ST.C = A >= val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
}

void cpu_6502::CMP_ZP(){
    uint16_t memory = ZeroPage();
    uint8_t val = bus.read(memory);
    uint8_t result = A - val;
    ST.C = A >= val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
}

void cpu_6502::CMP_ZPX(){
    uint16_t memory = ZeropageX();
    uint8_t val = bus.read(memory);
    uint8_t result = A - val;
    ST.C = A >= val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
}

void cpu_6502::CMP_ABS(){
    uint16_t memory = Absolute();
    uint8_t val = bus.read(memory);
    uint8_t result = A - val;
    ST.C = A >= val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
}

void cpu_6502::CMP_ABSX(){
    uint16_t memory = AbsoluteX();
    uint8_t val = bus.read(memory);
    uint8_t result = A - val;
    ST.C = A >= val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
}

void cpu_6502::CMP_ABSY(){
    uint16_t memory = AbsoluteY();
    uint8_t val = bus.read(memory);
    uint8_t result = A - val;
    ST.C = A >= val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
}

void cpu_6502::CMP_INDZPX(){
    uint16_t memory = IndirectZPX();
    uint8_t val = bus.read(memory);
    uint8_t result = A - val;
    ST.C = A >= val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
}

void cpu_6502::CMP_INDZPY(){
    uint16_t memory = IndirectZPY();
    uint8_t val = bus.read(memory);
    uint8_t result = A - val;
    ST.C = A >= val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
}

void cpu_6502::CPX_IMM(){
    uint16_t memory = Immediate();
    uint8_t val = FetchByte();
    uint8_t result = X - val;
    ST.C = X >= val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
}

void cpu_6502::CPX_ZP(){
    uint16_t memory = ZeroPage();
    uint8_t val = bus.read(memory);
    uint8_t result = X - val;
    ST.C = X >= val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
}

void cpu_6502::CPX_ABS(){
    uint16_t memory = Absolute();
    uint8_t val = bus.read(memory);
    uint8_t result = X - val;
    ST.C = X >= val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
}

void cpu_6502::CPY_IMM(){
    uint16_t memory = Immediate();
    uint8_t val = FetchByte();
    uint8_t result = Y - val;
    ST.C = Y >= val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
}

void cpu_6502::CPY_ZP(){
    uint16_t memory = ZeroPage();
    uint8_t val = bus.read(memory);
    uint8_t result = Y - val;
    ST.C = Y >= val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
}

void cpu_6502::CPY_ABS(){
    uint16_t memory = Absolute();
    uint8_t val = bus.read(memory);
    uint8_t result = Y - val;
    ST.C = Y >= val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
}

void cpu_6502::DEC_ZP(){
    uint16_t memory = ZeroPage();
    uint8_t val = bus.read(memory);
    uint8_t result = val - 1;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    bus.write(memory,result);
}

void cpu_6502::DEC_ZPX(){
    uint16_t memory = ZeropageX();
    uint8_t val = bus.read(memory);
    uint8_t result = val - 1;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    bus.write(memory,result);
}

void cpu_6502::DEC_ABS(){
    uint16_t memory = Absolute();
    uint8_t val = bus.read(memory);
    uint8_t result = val - 1;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    bus.write(memory,result);
}

void cpu_6502::DEC_ABSX(){
    uint16_t memory = AbsoluteX();
    uint8_t val = bus.read(memory);
    uint8_t result = val - 1;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    bus.write(memory,result);
}

void cpu_6502::DEX_IMPL(){
    uint8_t result = X - 1;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    X = result;
}

void cpu_6502::DEY_IMPL(){
    uint8_t result = Y - 1;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    Y = result;
}

void cpu_6502::EOR_IMM(){
    uint16_t memory = Immediate();
    uint8_t val = FetchByte();
    uint8_t result = A ^ val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::EOR_ZP(){
    uint16_t memory = ZeroPage();
    uint8_t val = bus.read(memory);
    uint8_t result = A ^ val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::EOR_ZPX(){
    uint16_t memory = ZeropageX();
    uint8_t val = bus.read(memory);
    uint8_t result = A ^ val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::EOR_ABS(){
    uint16_t memory = Absolute();
    uint8_t val = bus.read(memory);
    uint8_t result = A ^ val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::EOR_ABSX(){
    uint16_t memory = AbsoluteX();
    uint8_t val = bus.read(memory);
    uint8_t result = A ^ val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::EOR_ABSY(){
    uint16_t memory = AbsoluteY();
    uint8_t val = bus.read(memory);
    uint8_t result = A ^ val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::EOR_INDZPX(){
    uint16_t memory = IndirectZPX();
    uint8_t val = bus.read(memory);
    uint8_t result = A ^ val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::EOR_INDZPY(){
    uint16_t memory = IndirectZPY();
    uint8_t val = bus.read(memory);
    uint8_t result = A ^ val;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::INC_ZP(){
    uint16_t memory = ZeroPage();
    uint8_t val = bus.read(memory);
    uint8_t result = val + 1;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    bus.write(memory,result);
}

void cpu_6502::INC_ZPX(){
    uint16_t memory = ZeropageX();
    uint8_t val = bus.read(memory);
    uint8_t result = val + 1;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    bus.write(memory,result);
}

void cpu_6502::INC_ABS(){
    uint16_t memory = Absolute();
    uint8_t val = bus.read(memory);
    uint8_t result = val + 1;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    bus.write(memory,result);
}

void cpu_6502::INC_ABSX(){
    uint16_t memory = AbsoluteX();
    uint8_t val = bus.read(memory);
    uint8_t result = val + 1;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    bus.write(memory,result);
}

void cpu_6502::INX_IMPL(){
    uint8_t result = X + 1;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    X = result;
}

void cpu_6502::INY_IMPL(){
    uint8_t result = Y + 1;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    Y = result;
}

void cpu_6502::JMP_ABS(){
    uint16_t memory = Absolute();
    PC = memory;
}

void cpu_6502::JMP_IND(){
    uint16_t memory = Indirect();
    PC = memory;
}

void cpu_6502::JSR_ABS(){
    uint16_t memory = Absolute();
    uint16_t ret = PC - 1;
    bus.write(0x0100 | SP, (ret >> 8) & 0xFF);
    SP--;
    bus.write(0x0100 | SP, ret & 0xFF);
    SP--;
    PC=memory;
}

void cpu_6502::LDA_IMM(){
    uint16_t memory = Immediate();
    uint8_t val = FetchByte();
    ST.Z = val == 0;
    ST.N = val & 0b10000000;
    A = val;
}

void cpu_6502::LDA_ZP(){
    uint16_t memory = ZeroPage();
    uint8_t val = bus.read(memory);
    ST.Z = val == 0;
    ST.N = val & 0b10000000;
    A = val;
}

void cpu_6502::LDA_ZPX(){
    uint16_t memory = ZeropageX();
    uint8_t val = bus.read(memory);
    ST.Z = val == 0;
    ST.N = val & 0b10000000;
    A = val;
}

void cpu_6502::LDA_ABS(){
    uint16_t memory = Absolute();
    uint8_t val = bus.read(memory);
    ST.Z = val == 0;
    ST.N = val & 0b10000000;
    A = val;
}

void cpu_6502::LDA_ABSX(){
    uint16_t memory = AbsoluteX();
    uint8_t val = bus.read(memory);
    ST.Z = val == 0;
    ST.N = val & 0b10000000;
    A = val;
}

void cpu_6502::LDA_ABSY(){
    uint16_t memory = AbsoluteY();
    uint8_t val = bus.read(memory);
    ST.Z = val == 0;
    ST.N = val & 0b10000000;
    A = val;
}

void cpu_6502::LDA_INDZPX(){
    uint16_t memory = IndirectZPX();
    uint8_t val = bus.read(memory);
    ST.Z = val == 0;
    ST.N = val & 0b10000000;
    A = val;
}

void cpu_6502::LDA_INDZPY(){
    uint16_t memory = IndirectZPY();
    uint8_t val = bus.read(memory);
    ST.Z = val == 0;
    ST.N = val & 0b10000000;
    A = val;
}

void cpu_6502::LDX_IMM(){
    uint16_t memory = Immediate();
    uint8_t val = FetchByte();
    ST.Z = val == 0;
    ST.N = val & 0b10000000;
    X = val;
}

void cpu_6502::LDX_ZP(){
    uint16_t memory = ZeroPage();
    uint8_t val = bus.read(memory);
    ST.Z = val == 0;
    ST.N = val & 0b10000000;
    X = val;
}

void cpu_6502::LDX_ZPY(){
    uint16_t memory = ZeropageY();
    uint8_t val = bus.read(memory);
    ST.Z = val == 0;
    ST.N = val & 0b10000000;
    X = val;
}

void cpu_6502::LDX_ABS(){
    uint16_t memory = Absolute();
    uint8_t val = bus.read(memory);
    ST.Z = val == 0;
    ST.N = val & 0b10000000;
    X = val;
}

void cpu_6502::LDX_ABSY(){
    uint16_t memory = AbsoluteY();
    uint8_t val = bus.read(memory);
    ST.Z = val == 0;
    ST.N = val & 0b10000000;
    X = val;
}

void cpu_6502::LDY_IMM(){
    uint16_t memory = Immediate();
    uint8_t val = FetchByte();
    ST.Z = val == 0;
    ST.N = val & 0b10000000;
    Y = val;
}

void cpu_6502::LDY_ZP(){
    uint16_t memory = ZeroPage();
    uint8_t val = bus.read(memory);
    ST.Z = val == 0;
    ST.N = val & 0b10000000;
    Y = val;
}

void cpu_6502::LDY_ZPX(){
    uint16_t memory = ZeropageX();
    uint8_t val = bus.read(memory);
    ST.Z = val == 0;
    ST.N = val & 0b10000000;
    Y = val;
}

void cpu_6502::LDY_ABS(){
    uint16_t memory = Absolute();
    uint8_t val = bus.read(memory);
    ST.Z = val == 0;
    ST.N = val & 0b10000000;
    Y = val;
}

void cpu_6502::LDY_ABSX(){
    uint16_t memory = AbsoluteX();
    uint8_t val = bus.read(memory);
    ST.Z = val == 0;
    ST.N = val & 0b10000000;
    Y = val;
}

void cpu_6502::LSR_AC(){
    uint8_t result = A >> 1;
    ST.C = A & 1;
    ST.Z = result == 0;
    ST.N = 0;
    A = result;
}

void cpu_6502::LSR_ZP(){
    uint16_t memory = ZeroPage();
    uint8_t val = bus.read(memory);
    uint8_t result = val >> 1;
    ST.C = val & 1;
    ST.Z = result == 0;
    ST.N = 0;
    bus.write(memory, result);
}

void cpu_6502::LSR_ZPX(){
    uint16_t memory = ZeropageX();
    uint8_t val = bus.read(memory);
    uint8_t result = val >> 1;
    ST.C = val & 1;
    ST.Z = result == 0;
    ST.N = 0;
    bus.write(memory, result);
}

void cpu_6502::LSR_ABS(){
    uint16_t memory = Absolute();
    uint8_t val = bus.read(memory);
    uint8_t result = val >> 1;
    ST.C = val & 1;
    ST.Z = result == 0;
    ST.N = 0;
    bus.write(memory, result);
}

void cpu_6502::LSR_ABSX(){
    uint16_t memory = AbsoluteX();
    uint8_t val = bus.read(memory);
    uint8_t result = val >> 1;
    ST.C = val & 1;
    ST.Z = result == 0;
    ST.N = 0;
    bus.write(memory, result);
}

void cpu_6502::NOP(){
    //NOTTHING
}

void cpu_6502::ORA_IMM(){
    uint16_t memory = Immediate();
    uint8_t val = FetchByte();
    uint8_t result = A | val;
    ST.Z = result==0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::ORA_ZP(){
    uint16_t memory = ZeroPage();
    uint8_t val = bus.read(memory);
    uint8_t result = A | val;
    ST.Z = result==0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::ORA_ZPX(){
    uint16_t memory = ZeropageX();
    uint8_t val = bus.read(memory);
    uint8_t result = A | val;
    ST.Z = result==0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::ORA_ABS(){
    uint16_t memory = Absolute();
    uint8_t val = bus.read(memory);
    uint8_t result = A | val;
    ST.Z = result==0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::ORA_ABSX(){
    uint16_t memory = AbsoluteX();
    uint8_t val = bus.read(memory);
    uint8_t result = A | val;
    ST.Z = result==0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::ORA_ABSY(){
    uint16_t memory = AbsoluteY();
    uint8_t val = bus.read(memory);
    uint8_t result = A | val;
    ST.Z = result==0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::ORA_INDZPX(){
    uint16_t memory = IndirectZPX();
    uint8_t val = bus.read(memory);
    uint8_t result = A | val;
    ST.Z = result==0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::ORA_INDZPY(){
    uint16_t memory = IndirectZPY();
    uint8_t val = bus.read(memory);
    uint8_t result = A | val;
    ST.Z = result==0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::PHA_IMPL(){
    bus.write((SP | 0x100), A);
    SP--;
}

void cpu_6502::PHP_IMPL(){
    uint8_t status = (ST.N << 7) | (ST.V << 6) | (1 << 5) | (1 << 4) | (ST.D << 3) | (ST.I << 2) | (ST.Z << 1) | ST.C;
    bus.write((SP | 0x100), status);
    SP--;
}

void cpu_6502::PLA_IMPL(){
    SP++;
    uint8_t result = bus.read((0x100 | SP));
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::PLP_IMPL(){
    SP++;
    uint8_t val = bus.read((0x100 | SP));
    ST.N = val & 0b10000000;
    ST.V = val & 0b01000000;
    ST.D = val & 0b00001000;
    ST.I = val & 0b00000100;
    ST.Z = val & 0b00000010;
    ST.C = val & 1;
}

void cpu_6502::ROL_AC(){
    uint8_t oldC = ST.C;
    ST.C = A & 0b10000000;
    uint8_t result = (A << 1) | oldC;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::ROL_ZP(){
    uint16_t memory = ZeroPage();
    uint8_t val = bus.read(memory);
    uint8_t oldC = ST.C;
    ST.C = val & 0b10000000;
    uint8_t result = (val << 1) | oldC;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    bus.write(memory, result);
}

void cpu_6502::ROL_ZPX(){
    uint16_t memory = ZeropageX();
    uint8_t val = bus.read(memory);
    uint8_t oldC = ST.C;
    ST.C = val & 0b10000000;
    uint8_t result = (val << 1) | oldC;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    bus.write(memory, result);
}

void cpu_6502::ROL_ABS(){
    uint16_t memory = Absolute();
    uint8_t val = bus.read(memory);
    uint8_t oldC = ST.C;
    ST.C = val & 0b10000000;
    uint8_t result = (val << 1) | oldC;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    bus.write(memory, result);
}

void cpu_6502::ROL_ABSX(){
    uint16_t memory = AbsoluteX();
    uint8_t val = bus.read(memory);
    uint8_t oldC = ST.C;
    ST.C = val & 0b10000000;
    uint8_t result = (val << 1) | oldC;
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    bus.write(memory, result);
}

void cpu_6502::ROR_AC(){
    uint8_t oldC = ST.C;
    ST.C = A & 0b00000001;
    uint8_t result = (A >> 1) | (oldC << 7);
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    A = result;
}

void cpu_6502::ROR_ZP(){
    uint16_t memory = ZeroPage();
    uint8_t val = bus.read(memory);
    uint8_t oldC = ST.C;
    ST.C = val & 0b00000001;
    uint8_t result = (val >> 1) | (oldC << 7);
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    bus.write(memory, result);
}

void cpu_6502::ROR_ZPX(){
    uint16_t memory = ZeropageX();
    uint8_t val = bus.read(memory);
    uint8_t oldC = ST.C;
    ST.C = val & 0b00000001;
    uint8_t result = (val >> 1) | (oldC << 7);
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    bus.write(memory, result);
}

void cpu_6502::ROR_ABS(){
    uint16_t memory = Absolute();
    uint8_t val = bus.read(memory);
    uint8_t oldC = ST.C;
    ST.C = val & 0b00000001;
    uint8_t result = (val >> 1) | (oldC << 7);
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    bus.write(memory, result);
}

void cpu_6502::ROR_ABSX(){
    uint16_t memory = AbsoluteX();
    uint8_t val = bus.read(memory);
    uint8_t oldC = ST.C;
    ST.C = val & 0b00000001;
    uint8_t result = (val >> 1) | (oldC << 7);
    ST.Z = result == 0;
    ST.N = result & 0b10000000;
    bus.write(memory, result);
}

void cpu_6502::RTI_IMPL(){
    SP++;
    uint8_t val = bus.read(0x100 | SP);
    ST.N = val & 0b10000000;
    ST.V = val & 0b01000000;
    ST.D = val & 0b00001000;
    ST.I = val & 0b00000100;
    ST.Z = val & 0b00000010;
    ST.C = val & 1;
    SP++;
    uint8_t LL = bus.read(0x100 | SP);
    SP++;
    uint8_t HH = bus.read(0x100 | SP);
    PC = (HH << 8) | LL;
}

void cpu_6502::RTS_IMPL(){
    SP++;
    uint8_t LL = bus.read(0x100 | SP);
    SP++;
    uint8_t HH = bus.read(0x100 | SP);
    PC = ((HH << 8) | LL) + 1;
}

void cpu_6502::SBC_IMM(){
    uint16_t memory = Immediate();
    uint8_t val = FetchByte() ^ 0xFF;
    uint16_t result = A + val + ST.C;
    ST.C = result > 0xFF;
    ST.Z = (result & 0xFF) == 0;
    ST.V = (result ^ A) & (result ^ val) & 0x80;
    ST.N = result & 0x80;
    A = result & 0xFF;
}

void cpu_6502::SBC_ZP(){
    uint16_t memory = ZeroPage();
    uint8_t val = bus.read(memory) ^ 0xFF;
    uint16_t result = A + val + ST.C;
    ST.C = result & 0x100;
    ST.Z = (result & 0xFF) == 0;
    ST.V = (result ^ A) & (result ^ val) & 0x80;
    ST.N = result & 0x80;
    A = result & 0xFF;
}

void cpu_6502::SBC_ZPX(){
    uint16_t memory = ZeropageX();
    uint8_t val = bus.read(memory) ^ 0xFF;
    uint16_t result = A + val + ST.C;
    ST.C = result > 0xFF;
    ST.Z = (result & 0xFF) == 0;
    ST.V = (result ^ A) & (result ^ val) & 0x80;
    ST.N = result & 0x80;
    A = result & 0xFF;
}

void cpu_6502::SBC_ABS(){
    uint16_t memory = Absolute();
    uint8_t val = bus.read(memory) ^ 0xFF;
    uint16_t result = A + val + ST.C;
    ST.C = result > 0xFF;
    ST.Z = (result & 0xFF) == 0;
    ST.V = (result ^ A) & (result ^ val) & 0x80;
    ST.N = result & 0x80;
    A = result & 0xFF;
}

void cpu_6502::SBC_ABSX(){
    uint16_t memory = AbsoluteX();
    uint8_t val = bus.read(memory) ^ 0xFF;
    uint16_t result = A + val + ST.C;
    ST.C = result > 0xFF;
    ST.Z = (result & 0xFF) == 0;
    ST.V = (result ^ A) & (result ^ val) & 0x80;
    ST.N = result & 0x80;
    A = result & 0xFF;
}

void cpu_6502::SBC_ABSY(){
    uint16_t memory = AbsoluteY();
    uint8_t val = bus.read(memory) ^ 0xFF;
    uint16_t result = A + val + ST.C;
    ST.C = result > 0xFF;
    ST.Z = (result & 0xFF) == 0;
    ST.V = (result ^ A) & (result ^ val) & 0x80;
    ST.N = result & 0x80;
    A = result & 0xFF;
}

void cpu_6502::SBC_INDZPX(){
    uint16_t memory = IndirectZPX();
    uint8_t val = bus.read(memory) ^ 0xFF;
    uint16_t result = A + val + ST.C;
    ST.C = result > 0xFF;
    ST.Z = (result & 0xFF) == 0;
    ST.V = (result ^ A) & (result ^ val) & 0x80;
    ST.N = result & 0x80;
    A = result & 0xFF;
}

void cpu_6502::SBC_INDZPY(){
    uint16_t memory = IndirectZPY();
    uint8_t val = bus.read(memory) ^ 0xFF;
    uint16_t result = A + val + ST.C;
    ST.C = result > 0xFF;
    ST.Z = (result & 0xFF) == 0;
    ST.V = (result ^ A) & (result ^ val) & 0x80;
    ST.N = result & 0x80;
    A = result & 0xFF;
}

void cpu_6502::SEC_IMPL(){
    ST.C = 1;
}

void cpu_6502::SED_IMPL(){
    ST.D = 1;
}

void cpu_6502::SEI_IMPL(){
    ST.I = 1;
}

void cpu_6502::STA_ZP(){
    uint16_t memory = ZeroPage();
    bus.write(memory,A);
}

void cpu_6502::STA_ZPX(){
    uint16_t memory = ZeropageX();
    bus.write(memory,A);
}

void cpu_6502::STA_ABS(){
    uint16_t memory = Absolute();
    bus.write(memory,A);
}

void cpu_6502::STA_ABSX(){
    uint16_t memory = AbsoluteX();
    bus.write(memory,A);
}

void cpu_6502::STA_ABSY(){
    uint16_t memory = AbsoluteY();
    bus.write(memory,A);
}

void cpu_6502::STA_INDZPX(){
    uint16_t memory = IndirectZPX();
    bus.write(memory,A);
}

void cpu_6502::STA_INDZPY(){
    uint16_t memory = IndirectZPY();
    bus.write(memory,A);
}

void cpu_6502::STX_ZP(){
    uint16_t memory = ZeroPage();
    bus.write(memory,X);
}

void cpu_6502::STX_ZPY(){
    uint16_t memory = ZeropageY();
    bus.write(memory,X);
}

void cpu_6502::STX_ABS(){
    uint16_t memory = Absolute();
    bus.write(memory,X);
}

void cpu_6502::STY_ZP(){
    uint16_t memory = ZeroPage();
    bus.write(memory,Y);
}

void cpu_6502::STY_ZPX(){
    uint16_t memory = ZeropageX();
    bus.write(memory,Y);
}

void cpu_6502::STY_ABS(){
    uint16_t memory = Absolute();
    bus.write(memory,Y);
}

void cpu_6502::TAX_IMPL(){
    X = A;
    ST.Z = X == 0;
    ST.N = X & 0b10000000;
}

void cpu_6502::TAY_IMPL(){
    Y = A;
    ST.Z = Y == 0;
    ST.N = Y & 0b10000000;
}

void cpu_6502::TSX_IMPL(){
    X = SP;
    ST.Z = X == 0;
    ST.N = X & 0b10000000;
}

void cpu_6502::TXA_IMPL(){
    A = X;
    ST.Z = A == 0;
    ST.N = A & 0b10000000;
}

void cpu_6502::TXS_IMPL(){
    SP = X;
}

void cpu_6502::TYA_IMPL(){
    A = Y;
    ST.Z = A == 0;
    ST.N = A & 0b10000000;
}