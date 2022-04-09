#pragma once

#include "./util.hpp"
#include "./mainBus.hpp"

enum AddressingModes {
  IMPLICIT,
  ACCUMULATOR,
  IMMEDIATE, 
  ZERO_PAGE,
  ZERO_PAGE_X,
  ZERO_PAGE_Y,
  RELATIVE,
  ABSOLUTE,
  ABSOLUTE_x,
  ABSOLUTE_Y,
  INDIRECT,
  INDIRECT_X,
  INDIRECT_Y,
};

class CPU {
public: 
  CPU(MainBus& bus);
  
  u8 step();
  u8 exec();

  u8 read(u16 addr);
  u8* getPointerTo(u16 addr);
  void write(u16 addr, u8 val);

 private:
  MainBus bus;
  // FIXME: Casting might introduce bugs, esp. for arithmetic stuff
  u16 operand = 0; // u16 to hold address for indirect stuff, otherwise safe to cast to u8
  u8* effectiveAddress = 0;

  u8 A, X, Y, SP;
  struct Flags_t {
    unsigned int carry : 1;
    unsigned int zero : 1;
    unsigned int interrupt : 1;
    unsigned int decimal : 1;
    unsigned int _break : 1;
    unsigned int _none : 1;
    unsigned int overflow : 1;
    unsigned int negative : 1;
  } F;
  u8 flagsAsInt();
  void intAsFlags(u8 flags);
  u16 PC;

  u8 extraCycles = 0; // used for boundary costs mostly

  typedef void (CPU::*OpcodeFunc)();
  typedef void (CPU::*AddressingFunc)();
  struct Instruction {
    AddressingFunc addressing;
    OpcodeFunc opcode;
    u8 cycles;
  };
  struct Instruction instructionTable[0x100];

  void powerOn();
  void reset();

  void pushToStack(u8 value);
  u8 popFromStack();

  void implicit();
  void accumulator();
  void immediate();
  void zeroPage();
  void zeroPageX();
  void zeroPageY();
  void relative();
  void absolute();
  void absoluteX();
  void absoluteY();
  void indirect();
  void indirectX();
  void indirectY();

  void ADC();
  void AND();
  void ASL();
  void BCC();
  void BCS();
  void BEQ();
  void BIT();
  void BMI();
  void BNE();
  void BPL();
  void BRK();
  void BVC();
  void BVS();
  void CLC();
  void CLD();
  void CLI();
  void CLV();
  void CMP();
  void CPX();
  void CPY();
  void DEC();
  void DEX();
  void DEY();
  void EOR();
  void INC();
  void INX();
  void INY();
  void JMP();
  void JSR();
  void LDA();
  void LDX();
  void LDY();
  void LSR();
  void NOP();
  void ORA();
  void PHA();
  void PHP();
  void PLA();
  void PLP();
  void ROL();
  void ROR();
  void RTI();
  void RTS();
  void SBC();
  void SEC();
  void SED();
  void SEI();
  void STA();
  void STX();
  void STY();
  void TAX();
  void TAY();
  void TSX();
  void TXA();
  void TXS();
  void TYA();
  void ILL();

  bool logMode = false;
};
