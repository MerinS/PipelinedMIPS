/*
 * instr_cat.h
 *
 *  Created on: Nov 9, 2017
 *      Author: merinsan
 */

#ifndef INSTR_CAT_H_
#define INSTR_CAT_H_

enum CATEGORY_INSTR{
  CATEGORY1        = 0b01,
  CATEGORY2        = 0b11,
  SIGN_INT1        = 0b00,
  SIGN_INT2        = 0b10,
};

enum OPCODE_CAT1 {
  OP_J     = 0b0000,
  OP_JR    = 0b0001,
  OP_BEQ   = 0b0010,
  OP_BLTZ  = 0b0011,
  OP_BGTZ  = 0b0100,
  OP_BREAK = 0b0101,
  OP_SW    = 0b0110,
  OP_LW    = 0b0111,
  OP_SLL   = 0b1000,
  OP_SRL   = 0b1001,
  OP_SRA   = 0b1010,
  OP_NOP   = 0b1011,
};

enum OPCODE_CAT2 {
  OP_ADD   = 0b0000,
  OP_SUB   = 0b0001,
  OP_MUL   = 0b0010,
  OP_AND   = 0b0011,
  OP_OR    = 0b0100,
  OP_XOR   = 0b0101,
  OP_NOR   = 0b0110,
  OP_SLT   = 0b0111,
  OP_ADDI  = 0b1000,
  OP_ANDI  = 0b1001,
  OP_ORI   = 0b1010,
  OP_XORI  = 0b1011,
};

#endif /* INSTR_CAT_H_ */
