#pragma once
//
// RISC-V Instruction Set Architecture Definitions
//
//
//

// Basis
#define RV_ARCH_REG_COUNT                   32

// Opcode
#define RV_OPCODE_MASK                      0x0000007F
#define RV_OPCODE_OFFSET                    0

#define RV_OPCODE_OP_IMM                    0b0010011
#define RV_OPCODE_OP_IMM_32                 0b0011011
#define RV_OPCODE_OP                        0b0110011
#define RV_OPCODE_OP_32                     0b0111011
#define RV_OPCODE_LUI                       0b0110111
#define RV_OPCODE_AUIPC                     0b0010111
#define RV_OPCODE_JAL                       0b1101111
#define RV_OPCODE_JALR                      0b1100111
#define RV_OPCODE_BRANCH                    0b1100011
#define RV_OPCODE_LOAD                      0b0000011
#define RV_OPCODE_STORE                     0b0100011
#define RV_OPCODE_MISC_MEM                  0b0001111
#define RV_OPCODE_SYSTEM                    0b1110011
