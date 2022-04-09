#include "./cpu.hpp"
#include <iostream>
#include <random>

CPU::CPU(MainBus& bus) : bus(bus) { 
    powerOn();

    struct Instruction illegalInstruction = {&CPU::implicit, &CPU::ILL, 0};
    for (int i = 0; i < 0x100; i++) {
        this->instructionTable[i] = illegalInstruction;
    }

    this->instructionTable[0x69] = {&CPU::immediate,   &CPU::ADC, 2};
    this->instructionTable[0x65] = {&CPU::zeroPage,    &CPU::ADC, 3};
    this->instructionTable[0x75] = {&CPU::zeroPageX,   &CPU::ADC, 4};
    this->instructionTable[0x6D] = {&CPU::absolute,    &CPU::ADC, 4};
    this->instructionTable[0x7D] = {&CPU::absoluteX,   &CPU::ADC, 4};
    this->instructionTable[0x79] = {&CPU::absoluteY,   &CPU::ADC, 4};
    this->instructionTable[0x61] = {&CPU::indirectX,   &CPU::ADC, 6};
    this->instructionTable[0x71] = {&CPU::indirectY,   &CPU::ADC, 5};
  
    this->instructionTable[0x29] = {&CPU::immediate,   &CPU::AND, 2};
    this->instructionTable[0x25] = {&CPU::zeroPage,    &CPU::AND, 3};
    this->instructionTable[0x35] = {&CPU::zeroPageX,   &CPU::AND, 4};
    this->instructionTable[0x2D] = {&CPU::absolute,    &CPU::AND, 4};
    this->instructionTable[0x3D] = {&CPU::absoluteX,   &CPU::AND, 4};
    this->instructionTable[0x39] = {&CPU::absoluteY,   &CPU::AND, 4};
    this->instructionTable[0x21] = {&CPU::indirectX,   &CPU::AND, 6};
    this->instructionTable[0x31] = {&CPU::indirectY,   &CPU::AND, 5};

    this->instructionTable[0x0A] = {&CPU::accumulator, &CPU::ASL, 2};
    this->instructionTable[0x06] = {&CPU::zeroPage,    &CPU::ASL, 5};
    this->instructionTable[0x16] = {&CPU::zeroPageX,   &CPU::ASL, 6};
    this->instructionTable[0x0E] = {&CPU::absolute,    &CPU::ASL, 6};
    this->instructionTable[0x1E] = {&CPU::absoluteX,   &CPU::ASL, 7};
   
    this->instructionTable[0x90] = {&CPU::relative,    &CPU::BCC, 2};
    this->instructionTable[0xB0] = {&CPU::relative,    &CPU::BCS, 2};
    this->instructionTable[0xF0] = {&CPU::relative,    &CPU::BEQ, 2};

    this->instructionTable[0x24] = {&CPU::zeroPage,    &CPU::BIT, 3};
    this->instructionTable[0x2C] = {&CPU::absolute,    &CPU::BIT, 4};

    this->instructionTable[0x30] = {&CPU::relative,    &CPU::BMI, 2};
    this->instructionTable[0xD0] = {&CPU::relative,    &CPU::BNE, 2};
    this->instructionTable[0x10] = {&CPU::relative,    &CPU::BPL, 2};
    this->instructionTable[0x00] = {&CPU::implicit,    &CPU::BRK, 7};
    this->instructionTable[0x50] = {&CPU::relative,    &CPU::BVC, 2};
    this->instructionTable[0x70] = {&CPU::relative,    &CPU::BVS, 2};

    this->instructionTable[0x18] = {&CPU::implicit,    &CPU::CLC, 2};
    this->instructionTable[0xD8] = {&CPU::implicit,    &CPU::CLD, 2};
    this->instructionTable[0x58] = {&CPU::implicit,    &CPU::CLI, 2};
    this->instructionTable[0xB8] = {&CPU::implicit,    &CPU::CLV, 2};

    this->instructionTable[0xC9] = {&CPU::immediate,   &CPU::CMP, 2};
    this->instructionTable[0xC5] = {&CPU::zeroPage,    &CPU::CMP, 3};
    this->instructionTable[0xD5] = {&CPU::zeroPageX,   &CPU::CMP, 4};
    this->instructionTable[0xCD] = {&CPU::absolute,    &CPU::CMP, 4};
    this->instructionTable[0xDD] = {&CPU::absoluteX,   &CPU::CMP, 4};
    this->instructionTable[0xD9] = {&CPU::absoluteY,   &CPU::CMP, 4};
    this->instructionTable[0xC1] = {&CPU::indirectX,   &CPU::CMP, 6};
    this->instructionTable[0xD1] = {&CPU::indirectY,   &CPU::CMP, 5};

    this->instructionTable[0xE0] = {&CPU::immediate,   &CPU::CPX, 2};
    this->instructionTable[0xE4] = {&CPU::zeroPage,    &CPU::CPX, 3};
    this->instructionTable[0xEC] = {&CPU::absolute,    &CPU::CPX, 4};
    
    this->instructionTable[0xC0] = {&CPU::immediate,   &CPU::CPY, 2};
    this->instructionTable[0xC4] = {&CPU::zeroPage,    &CPU::CPY, 3};
    this->instructionTable[0xCC] = {&CPU::absolute,    &CPU::CPY, 4};

    this->instructionTable[0xC6] = {&CPU::zeroPage,    &CPU::DEC, 5};
    this->instructionTable[0xD6] = {&CPU::zeroPageX,   &CPU::DEC, 6};
    this->instructionTable[0xCE] = {&CPU::absolute,    &CPU::DEC, 6};
    this->instructionTable[0xDE] = {&CPU::absoluteX,   &CPU::DEC, 7};
    this->instructionTable[0xCA] = {&CPU::implicit,    &CPU::DEX, 2};
    this->instructionTable[0x88] = {&CPU::implicit,    &CPU::DEY, 2};

    this->instructionTable[0x49] = {&CPU::immediate,   &CPU::EOR, 2};
    this->instructionTable[0x45] = {&CPU::zeroPage,    &CPU::EOR, 3};
    this->instructionTable[0x55] = {&CPU::zeroPageX,   &CPU::EOR, 4};
    this->instructionTable[0x4D] = {&CPU::absolute,    &CPU::EOR, 4};
    this->instructionTable[0x5D] = {&CPU::absoluteX,   &CPU::EOR, 4};
    this->instructionTable[0x59] = {&CPU::absoluteY,   &CPU::EOR, 4};
    this->instructionTable[0x41] = {&CPU::indirectX,   &CPU::EOR, 6};
    this->instructionTable[0x51] = {&CPU::indirectY,   &CPU::EOR, 5};

    this->instructionTable[0xE6] = {&CPU::zeroPage,    &CPU::INC, 5};
    this->instructionTable[0xF6] = {&CPU::zeroPageX,   &CPU::INC, 6};
    this->instructionTable[0xEE] = {&CPU::absolute,    &CPU::INC, 6};
    this->instructionTable[0xFE] = {&CPU::absoluteX,   &CPU::INC, 7};
    this->instructionTable[0xE8] = {&CPU::implicit,    &CPU::INX, 2};
    this->instructionTable[0xC8] = {&CPU::implicit,    &CPU::INY, 2};

    this->instructionTable[0x4C] = {&CPU::absolute,    &CPU::JMP, 3};
    this->instructionTable[0x6C] = {&CPU::absoluteX,   &CPU::JMP, 5};
    this->instructionTable[0x20] = {&CPU::absolute,    &CPU::JSR, 6};

    this->instructionTable[0xA9] = {&CPU::immediate,   &CPU::LDA, 2};
    this->instructionTable[0xA5] = {&CPU::zeroPage,    &CPU::LDA, 3};
    this->instructionTable[0xB5] = {&CPU::zeroPageX,   &CPU::LDA, 4};
    this->instructionTable[0xAD] = {&CPU::absolute,    &CPU::LDA, 4};
    this->instructionTable[0xBD] = {&CPU::absoluteX,   &CPU::LDA, 4};
    this->instructionTable[0xB9] = {&CPU::absoluteY,   &CPU::LDA, 4};
    this->instructionTable[0xA1] = {&CPU::indirectX,   &CPU::LDA, 6};
    this->instructionTable[0xB1] = {&CPU::indirectY,   &CPU::LDA, 5};

    this->instructionTable[0xA2] = {&CPU::immediate,   &CPU::LDX, 2};
    this->instructionTable[0xA6] = {&CPU::zeroPage,    &CPU::LDX, 3};
    this->instructionTable[0xB6] = {&CPU::zeroPageY,   &CPU::LDX, 4};
    this->instructionTable[0xAE] = {&CPU::absolute,    &CPU::LDX, 4};
    this->instructionTable[0xBE] = {&CPU::absoluteY,   &CPU::LDX, 4};

    this->instructionTable[0xA0] = {&CPU::immediate,   &CPU::LDY, 2};
    this->instructionTable[0xA4] = {&CPU::zeroPage,    &CPU::LDY, 3};
    this->instructionTable[0xB4] = {&CPU::zeroPageX,   &CPU::LDY, 4};
    this->instructionTable[0xAC] = {&CPU::absolute,    &CPU::LDY, 4};
    this->instructionTable[0xBC] = {&CPU::absoluteX,   &CPU::LDY, 4};

    this->instructionTable[0x4A] = {&CPU::accumulator, &CPU::LSR, 2};
    this->instructionTable[0x46] = {&CPU::zeroPage,    &CPU::LSR, 5};
    this->instructionTable[0x56] = {&CPU::zeroPageX,   &CPU::LSR, 6};
    this->instructionTable[0x4E] = {&CPU::absolute,    &CPU::LSR, 6};
    this->instructionTable[0x5E] = {&CPU::absoluteX,   &CPU::LSR, 7};
    
    this->instructionTable[0xEA] = {&CPU::implicit,    &CPU::NOP, 2};

    this->instructionTable[0x09] = {&CPU::immediate,   &CPU::ORA, 2};
    this->instructionTable[0x05] = {&CPU::zeroPage,    &CPU::ORA, 3};
    this->instructionTable[0x15] = {&CPU::zeroPageX,   &CPU::ORA, 4};
    this->instructionTable[0x0D] = {&CPU::absolute,    &CPU::ORA, 4};
    this->instructionTable[0x1D] = {&CPU::absoluteX,   &CPU::ORA, 4};
    this->instructionTable[0x19] = {&CPU::absoluteY,   &CPU::ORA, 4};
    this->instructionTable[0x01] = {&CPU::indirectX,   &CPU::ORA, 6};
    this->instructionTable[0x11] = {&CPU::indirectY,   &CPU::ORA, 5};

    this->instructionTable[0x48] = {&CPU::implicit,    &CPU::PHA, 3};
    this->instructionTable[0x08] = {&CPU::implicit,    &CPU::PHP, 3};
    this->instructionTable[0x68] = {&CPU::implicit,    &CPU::PLA, 4};
    this->instructionTable[0x28] = {&CPU::implicit,    &CPU::PLP, 4};

    this->instructionTable[0x2A] = {&CPU::accumulator, &CPU::ROL, 2};
    this->instructionTable[0x26] = {&CPU::zeroPage,    &CPU::ROL, 5};
    this->instructionTable[0x36] = {&CPU::zeroPageX,   &CPU::ROL, 6};
    this->instructionTable[0x2E] = {&CPU::absolute,    &CPU::ROL, 6};
    this->instructionTable[0x3E] = {&CPU::absoluteX,   &CPU::ROL, 7};

    this->instructionTable[0x6A] = {&CPU::accumulator, &CPU::ROR, 2};
    this->instructionTable[0x66] = {&CPU::zeroPage,    &CPU::ROR, 5};
    this->instructionTable[0x76] = {&CPU::zeroPageX,   &CPU::ROR, 6};
    this->instructionTable[0x6E] = {&CPU::absolute,    &CPU::ROR, 6};
    this->instructionTable[0x7E] = {&CPU::absoluteX,   &CPU::ROR, 7};

    this->instructionTable[0x40] = {&CPU::implicit,    &CPU::RTI, 6};
    this->instructionTable[0x60] = {&CPU::implicit,    &CPU::RTS, 6};

    this->instructionTable[0xE9] = {&CPU::immediate,   &CPU::SBC, 2};
    this->instructionTable[0xE5] = {&CPU::zeroPage,    &CPU::SBC, 3};
    this->instructionTable[0xF5] = {&CPU::zeroPageX,   &CPU::SBC, 4};
    this->instructionTable[0xED] = {&CPU::absolute,    &CPU::SBC, 4};
    this->instructionTable[0xFD] = {&CPU::absoluteX,   &CPU::SBC, 4};
    this->instructionTable[0xF9] = {&CPU::absoluteY,   &CPU::SBC, 4};
    this->instructionTable[0xE1] = {&CPU::indirectX,   &CPU::SBC, 6};
    this->instructionTable[0xF1] = {&CPU::indirectY,   &CPU::SBC, 5};

    this->instructionTable[0x38] = {&CPU::implicit,    &CPU::SEC, 2};
    this->instructionTable[0xF8] = {&CPU::implicit,    &CPU::SED, 2};    
    this->instructionTable[0x78] = {&CPU::implicit,    &CPU::SEI, 2};
    
    this->instructionTable[0x85] = {&CPU::zeroPage,    &CPU::STA, 3};
    this->instructionTable[0x95] = {&CPU::zeroPageX,   &CPU::STA, 4};
    this->instructionTable[0x8D] = {&CPU::absolute,    &CPU::STA, 4};
    this->instructionTable[0x9D] = {&CPU::absoluteX,   &CPU::STA, 5};
    this->instructionTable[0x99] = {&CPU::absoluteY,   &CPU::STA, 5};
    this->instructionTable[0x81] = {&CPU::indirectX,   &CPU::STA, 6};
    this->instructionTable[0x91] = {&CPU::indirectY,   &CPU::STA, 6};

    this->instructionTable[0x86] = {&CPU::zeroPage,    &CPU::STX, 3};
    this->instructionTable[0x96] = {&CPU::zeroPageY,   &CPU::STX, 4};
    this->instructionTable[0x8E] = {&CPU::absolute,    &CPU::STX, 4};

    this->instructionTable[0x84] = {&CPU::zeroPage,    &CPU::STY, 3};
    this->instructionTable[0x94] = {&CPU::zeroPageX,   &CPU::STY, 4};
    this->instructionTable[0x8C] = {&CPU::absolute,    &CPU::STY, 4};

    this->instructionTable[0xAA] = {&CPU::implicit,    &CPU::TAX, 2};
    this->instructionTable[0xA8] = {&CPU::implicit,    &CPU::TAY, 2};
    this->instructionTable[0xBA] = {&CPU::implicit,    &CPU::TSX, 2};
    this->instructionTable[0x8A] = {&CPU::implicit,    &CPU::TXA, 2};
    this->instructionTable[0x9A] = {&CPU::implicit,    &CPU::TXS, 2};
    this->instructionTable[0x98] = {&CPU::implicit,    &CPU::TYA, 2};
}
void CPU::powerOn() {
    intAsFlags(0x34);
    A = X = Y = 0;
    SP = 0xFD;
    PC = 0;
}
void CPU::reset() {
    SP -= 3;
    F.interrupt = true;
}

u8 CPU::step() {
    // TODO: Check interrupts?
    u8 cycles = exec();
    return cycles;
}
u8 CPU::exec() {
    u8 opcode = read(PC++);

    Instruction currentInstruction = instructionTable[opcode];
    (this->*currentInstruction.addressing)();
    (this->*currentInstruction.opcode)();

    u8 cycleCount = currentInstruction.cycles + this->extraCycles;
    this->extraCycles = 0;
    return cycleCount;
}


u8 CPU::read(u16 addr) {
    return bus.read(addr);
}
void CPU::write(u16 addr, u8 val){
    bus.write(addr, val);
}
u8* CPU::getPointerTo(u16 addr) {
    return bus.getPointerTo(addr);
}


void CPU::pushToStack(u8 value) {
    write(0x100 + SP--, value);
}
u8 CPU::popFromStack() {
    return read(0x100 + ++SP);
}


u8 CPU::flagsAsInt() { // Slow as heck. The fast, but nonportable, way is to cast the bitfield struct as an u8
    u8 statusFlags =    F.carry 
                    |   F.zero      << 1
                    |   F.interrupt << 2
                    |   F.decimal   << 3
                    |   1           << 4 // break
                    |   1           << 5 // none
                    |   F.overflow  << 6
                    |   F.negative  << 7;
    return statusFlags;
}
void CPU::intAsFlags(u8 flags) {
    // Slow as heck. The fast, but nonportable, way is to cast the int as the struct (via union?)
    F.carry = readBit(flags, 0);
    F.zero = readBit(flags, 1);
    F.interrupt = readBit(flags, 2);
    F.decimal = readBit(flags, 3);
    F._break = 0; // FIXME: Is this behavior correct?
    F._none = 0;
    F.overflow = readBit(flags, 6);
    F.negative = readBit(flags, 7);
}


void CPU::ADC() {
    u16 result = operand + A + F.carry;

    F.zero = !getLowByte(result);
    F.negative = readBit(result, 7);
    u8 signSwapA_Op = A ^ operand;
    u8 signSwapA_Res = A ^ result;
    F.overflow = !(readBit(signSwapA_Op, 7) && readBit(signSwapA_Res, 7));
    F.carry = readBit(result, 8);

    A = getLowByte(result);
}
void CPU::AND() {
    u8 result = operand & A;

    F.negative = readBit(result, 7);
    F.zero = !result;

    A = result;
}
void CPU::ASL() {
    u8 result = operand << 1;

    F.carry = readBit(operand, 7);
    F.negative = readBit(result, 7);
    F.zero = !result;

    *effectiveAddress = result;
}
void CPU::BCC() {
    if (!F.carry) {
        PC = operand;
    }
}
void CPU::BCS() {
    if (F.carry) {
        PC = operand;
    }
}
void CPU::BEQ() {
    if (F.carry) {
        PC = operand;
    }
}
void CPU::BIT() {
    F.zero = !(A & operand);
    F.negative = readBit(operand, 7);
    F.overflow = readBit(operand, 6);
}
void CPU::BMI() {
    if (F.negative) {
        PC = operand;
    }
}
void CPU::BNE() {
    if (!F.zero) {
        PC = operand;
    }
}
void CPU::BPL(){
    if (!F.negative) {
        PC = operand;
    }
}
void CPU::BRK(){
    // TODO: After interrupt and stack work, return here
}
void CPU::BVC(){
    if (!F.overflow) {
        PC = operand;
    }
}
void CPU::BVS(){
    if (F.overflow) {
        PC = operand;
    }
}
void CPU::CLC() {
    F.carry = 0;
}
void CPU::CLD() {
    F.decimal = 0;
}
void CPU::CLI() {
    F.interrupt = 0;
}
void CPU::CLV() {
    F.overflow = 0;
}
void CPU::CMP() {
    s8 result = A - operand;
    F.carry = result >= 0;
    F.zero = result == 0;
    F.negative = result < 0;
}
void CPU::CPX() {
    s8 result = X - operand;
    F.carry = result >= 0;
    F.zero = result == 0;
    F.negative = result < 0;
}
void CPU::CPY() {
    s8 result = Y - operand;
    F.carry = result >= 0;
    F.zero = result == 0;
    F.negative = result < 0;
}
void CPU::DEC() {
    u8 result = operand - 1;

    F.zero = !result;
    F.negative = readBit(result, 7);

    *effectiveAddress = result;
}
void CPU::DEX() {
    u8 result = X - 1;

    F.zero = !result;
    F.negative = readBit(result, 7);
    
    X = result;
}
void CPU::DEY() {
    u8 result = Y - 1;

    F.zero = !result;
    F.negative = readBit(result, 7);
    
    Y = result;
}
void CPU::EOR() {
    u8 result = A ^ operand;

    F.zero = !result;
    F.negative = readBit(result, 7);

    A = result;
}
void CPU::INC() {
    u8 result = operand + 1;

    F.zero = !result;
    F.negative = readBit(result, 7);

    *effectiveAddress = result;
}
void CPU::INX() {
    u8 result = X + 1;

    F.zero = !result;
    F.negative = readBit(result, 7);

    X = result;
}
void CPU::INY() {
    u8 result = Y + 1;

    F.zero = !result;
    F.negative = readBit(result, 7);

    Y = result;
}
void CPU::JMP() {
    PC = operand;
}
void CPU::JSR() {
    u16 savedPC = --PC;
    pushToStack(getHighByte(savedPC));
    pushToStack(getLowByte(savedPC));
    PC = operand;
}
void CPU::LDA() {
    u8 result = operand;

    F.zero = !result;
    F.negative = readBit(result, 7);

    A = result;
}
void CPU::LDX() {
    u8 result = operand;

    F.zero = !result;
    F.negative = readBit(result, 7);

    X = result;
}
void CPU::LDY() {
    u8 result = operand;

    F.zero = !result;
    F.negative = readBit(result, 7);

    Y = result;
}
void CPU::LSR() {
    u8 result = operand >> 1;

    F.carry = readBit(operand, 0);
    F.zero = !result;
    F.negative = readBit(result, 7);

    *effectiveAddress = result;
}
void CPU::NOP() {
    // intentionally blank
}
void CPU::ORA() {
    u8 result = A | operand;

    F.zero = !result;
    F.negative = readBit(result, 7);

    A = result;
}
void CPU::PHA() {
    pushToStack(A);
}
void CPU::PHP() {
    u8 flags = flagsAsInt();
    pushToStack(flags);
}
void CPU::PLA() {
    A = popFromStack();
    F.zero = !A;
    F.negative = readBit(A, 7);
}
void CPU::PLP() {
    intAsFlags(popFromStack());
}
void CPU::ROL() {
    u8 result = operand << 1;
    result = changeIthBitToX(result, 0, F.carry);

    F.carry = readBit(operand, 7);
    F.negative = readBit(result, 7);
}
void CPU::ROR() {
    u8 result = operand >> 1;
    result = changeIthBitToX(result, 7, F.carry);

    F.carry = readBit(operand, 0);
    F.negative = readBit(result, 7);
}
void CPU::RTI() {
    intAsFlags(popFromStack());
    PC = popFromStack() | popFromStack() << 8;
}
void CPU::RTS() {
    PC = popFromStack() | popFromStack() << 8;
    PC++;
}
void CPU::SBC() {
    u16 result = A - operand - !F.carry;

    F.negative = readBit(result, 7);
    F.zero = !getLowByte(result);
    u8 signSwapA_Op = A ^ operand;
    u8 signSwapA_Res = A ^ result;
    F.overflow = !(readBit(signSwapA_Op, 7) && readBit(signSwapA_Res, 7));
    F.carry = readBit(result, 8);

    A = getLowByte(result);
}
void CPU::SEC() {
    F.carry = true;
}
void CPU::SED() {
    F.decimal = true;
}
void CPU::SEI() {
    F.interrupt = true;
}
void CPU::STA() {
    *effectiveAddress = A;
}
void CPU::STX() {
    *effectiveAddress = X;
}
void CPU::STY() {
    *effectiveAddress = Y;
}
void CPU::TAX() {
    F.negative = readBit(A, 7);
    F.zero = !A;
    X = A;
}
void CPU::TAY() {
    F.negative = readBit(A, 7);
    F.zero = !A;
    Y = A;
}
void CPU::TSX() {
    F.negative = readBit(SP, 7);
    F.zero = !SP;
    X = SP;
}
void CPU::TXA() {
    F.negative = readBit(X, 7);
    F.zero = !X;
    A = X;
}
void CPU::TXS() {
    F.negative = readBit(X, 7);
    F.zero = !X;
    SP = X;
}
void CPU::TYA() {
    F.negative = readBit(Y, 7);
    F.zero = !Y;
    A = Y;
}
void CPU::ILL() {
    std::cout << "Encountered illegal instruction!!" << std::endl;
}



// TODO: Cycle penalties for crossing pages and similar
void CPU::implicit() {
    // do nothing
}
void CPU::accumulator() {
    this->operand = A;
    this->effectiveAddress = &A;
}
void CPU::immediate() {
    this->operand = read(PC++);
}
void CPU::zeroPage() {
    u16 addr = read(PC++);
    this->operand = read(addr);
    this->effectiveAddress = getPointerTo(addr);
}
void CPU::zeroPageX() {
    u8 imm = read(PC++);
    u16 addr = (X + imm) % 0x100;
    this->operand = read(addr);
    this->effectiveAddress = getPointerTo(addr);
}
void CPU::zeroPageY() {
    u8 imm = read(PC++);
    u16 addr = (Y + imm) % 0x100;
    this->operand = read(addr);
    this->effectiveAddress = getPointerTo(addr);
}
void CPU::relative() {
    s8 offset = read(PC++);
    u16 addr = offset + PC;
    this->operand = addr; // I think this is correct?
}
void CPU::absolute() {
    u8 low = read(PC++);
    u8 high = read(PC++);
    u16 addr = high << 8 | low;
    this->operand = addr;
    this->effectiveAddress = getPointerTo(addr);
}
void CPU::absoluteX() {
    u8 low = read(PC++);
    u8 high = read(PC++);
    u16 addr = high << 8 | low;
    addr += X;
    this->operand = read(addr);
    this->effectiveAddress = getPointerTo(addr);
}
void CPU::absoluteY() {
    u8 low = read(PC++);
    u8 high = read(PC++);
    u16 addr = high << 8 | low;
    addr += Y;
    this->operand = read(addr);
    this->effectiveAddress = getPointerTo(addr);
}
void CPU::indirect() {
    u8 low = read(PC++);
    u8 high = read(PC++);
    u16 addr = high << 8 | low;
    this->operand = read(addr) | read(addr + 1) << 8;
    this->effectiveAddress = getPointerTo(addr); // LSB
}
void CPU::indirectX() {
    u8 imm = read(PC++);
    u16 addr = read((imm + X) % 0x100) | read((imm + 1+ X) % 0x100) << 8;
    this->operand = read(addr) | read(addr + 1) << 8;
    this->effectiveAddress = getPointerTo(addr); // LSB
}
void CPU::indirectY() {
    u8 imm = read(PC++);
    u16 addr = read(imm) | read((imm + 1) % 256) << 8;
    addr += Y;
    this->operand = read(addr) | read(addr + 1) << 8;
    this->effectiveAddress = getPointerTo(addr); // LSB
}