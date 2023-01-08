#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/* Sign extends the given field to a 32-bit integer where field is
 * interpreted an n-bit integer. */
int sign_extend_number(unsigned int field, unsigned int n) {
  /* YOUR CODE HERE */
  //basically interpret the hex as a binary and sign extend it based on whatever value is last 
  //example: 0xFF = 11111111
  //if we read this as an 9-bit integer like in the second test, it will be 011111111
  //therefore it should equal 0xFF again

  //0x1234 interpreted as 13 bits is  1001000110100
  //sign extend by adding a ton of 1's until 32 bit 
  //result is equal to 0xFFFFF234

  int mask = 1U << (n-1);
  int extended = (field^mask)-mask;
  return extended;
}

/* Unpacks the 32-bit machine code instruction given into the correct
 * type within the instruction struct */
Instruction parse_instruction(uint32_t instruction_bits) {
  /* YOUR CODE HERE */
  Instruction instruction;
  // add x8, x0, x0     hex : 00000433  binary = 0000 0000 0000 0000 0000 01000
  // Opcode: 0110011 (0x33) Get the Opcode by &ing 0x1111111, bottom 7 bits
  instruction.opcode = instruction_bits & ((1U << 7) - 1);

  // Shift right to move to pointer to interpret next fields in instruction.
  instruction_bits >>= 7;

  switch (instruction.opcode) {
  // R-Type
  case 0x33:
    // instruction: 0000 0000 0000 0000 0000 destination : 01000
    instruction.rtype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // instruction: 0000 0000 0000 0000 0 func3 : 000
    instruction.rtype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    // instruction: 0000 0000 0000  src1: 00000
    instruction.rtype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // instruction: 0000 000        src2: 00000
    instruction.rtype.rs2 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // funct7: 0000 000
    instruction.rtype.funct7 = instruction_bits & ((1U << 7) - 1);
    break;

  //custom instructions
  case 0x2b:
    instruction.rtype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.rtype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    instruction.rtype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.rtype.rs2 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.rtype.funct7 = instruction_bits & ((1U << 7) - 1);
    break;

  // case for I-type 
  case 0x13:
    instruction.itype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.itype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    instruction.itype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;   

    instruction.itype.imm = instruction_bits & ((1U << 12) - 1);
    break;

  // case for ecall and ebreak instruction
  case 0x73:
    instruction.itype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.itype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    instruction.itype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;   

    instruction.itype.imm = instruction_bits & ((1U << 12) - 1);
    break; 

  // case for load-type
  case 0x03:
    instruction.itype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.itype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    instruction.itype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;   

    instruction.itype.imm = instruction_bits & ((1U << 12) - 1);
    break;

  // case for jalr
  
  case 0x67:
    instruction.itype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.itype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    instruction.itype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;   

    instruction.itype.imm = instruction_bits & ((1U << 12) - 1);
    break;

  // case for S-type
  case 0x23:
    instruction.stype.imm5 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.stype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    instruction.stype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;   

    instruction.stype.rs2 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;  

    instruction.stype.imm7 = instruction_bits & ((1U << 7) - 1);
    break;

  //case for SB-type
  case 0x63:
    instruction.sbtype.imm5 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.sbtype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    instruction.sbtype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;   

    instruction.sbtype.rs2 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;  

    instruction.sbtype.imm7 = instruction_bits & ((1U << 7) - 1);
    break;

  //case for U-type
  case 0x37:
    instruction.utype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.utype.imm = instruction_bits & ((1U << 20) - 1);
    break;

  //case for U-type
  case 0x17:
    instruction.utype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.utype.imm = instruction_bits & ((1U << 20) - 1);
    break;

  //case for UJ-type
  case 0x6F:
    instruction.ujtype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.ujtype.imm = instruction_bits & ((1U << 20) - 1);
    break;

  default:
    exit(EXIT_FAILURE);
  }
  return instruction;
}

/* Return the number of bytes (from the current PC) to the branch label using
 * the given branch instruction */
int get_branch_offset(Instruction instruction) {
  unsigned int imm_11 = instruction.sbtype.imm5 << 31;
  imm_11 = imm_11 >> 31; 

  int imm_12 = instruction.sbtype.imm7 >> 6;

  int imm_4bits = instruction.sbtype.imm5 >> 1;

  unsigned int imm_6bits = instruction.sbtype.imm7 << 26;
  imm_6bits = imm_6bits >> 26;

  int imm_last2bits = (imm_12 << 12) | (imm_11 << 11);

  int imm_full = (imm_last2bits) | ((imm_6bits << 5) | (imm_4bits << 1));

  return sign_extend_number(imm_full,13);
}

/* Returns the number of bytes (from the current PC) to the jump label using the
 * given jump instruction */
int get_jump_offset(Instruction instruction) {
  unsigned int imm_10bits = instruction.ujtype.imm << 13;
  imm_10bits = imm_10bits >> 22; 

  int imm_20 = instruction.ujtype.imm >> 19;

  unsigned int imm_11 = instruction.ujtype.imm << 23;
  imm_11 = imm_11 >> 31;

  unsigned int imm_8bits = instruction.ujtype.imm << 24;
  imm_8bits = imm_8bits >> 24;

  int imm_fronthalf = (imm_20 << 20) | (imm_8bits << 12);
  int imm_secondhalf = (imm_11 << 11) | (imm_10bits << 1);
  int imm_full = (imm_fronthalf) | (imm_secondhalf);
  
  return sign_extend_number(imm_full,21);
}

int get_store_offset(Instruction instruction) {
  unsigned int imm_first = instruction.stype.imm7;
  unsigned int imm_second = instruction.stype.imm5;
  unsigned int imm_full = (imm_first << 5) | imm_second;
  return sign_extend_number(imm_full,12);
}

void handle_invalid_instruction(Instruction instruction) {
  printf("Invalid Instruction: 0x%08x\n", instruction.bits);
}

void handle_invalid_read(Address address) {
  printf("Bad Read. Address: 0x%08x\n", address);
  exit(-1);
}

void handle_invalid_write(Address address) {
  printf("Bad Write. Address: 0x%08x\n", address);
  exit(-1);
}
