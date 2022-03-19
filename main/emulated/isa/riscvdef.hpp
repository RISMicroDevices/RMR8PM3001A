#pragma once
//
// RISC-V Instruction Set Architecture Definitions
//
//
//


// Global type definitions
namespace Jasse {

    // Type definition of Architectural Register Value of XLEN=32
    typedef     uint32_t        arch32_t;

    // Type definition of Architectural Register Value of XLEN=64
    typedef     uint64_t        arch64_t;

    // Type definition of Immediate Value of XLEN=32/64
    typedef union {
        arch64_t    imm64 = 0;
        arch32_t    imm32;
    } imm_t;

    // Type definition of PC Register Value of XLEN=32/64
    typedef union {
        arch64_t    pc64 = 0;
        arch32_t    pc32;
    } pc_t;

    // Type definition of Raw Instruction
    typedef     uint32_t        insnraw_t;
}


// Standard operand format
#define GET_OPERAND(insn, mask, offset) \
    ((insn & mask) >> offset)

#define GET_STD_OPERAND(insn, name) \
    ((insn & name##_MASK) >> name##_OFFSET)


// Basis
#define RV_ARCH_REG_COUNT                   32


// Operand
#define RV_OPERAND_RD_MASK                  0x00000F80
#define RV_OPERAND_RD_OFFSET                7

#define RV_OPERAND_RS1_MASK                 0x000F8000
#define RV_OPERAND_RS1_OFFSET               15

#define RV_OPERAND_RS2_MASK                 0x01F00000
#define RV_OPERAND_RS2_OFFSET               20

#define RV_OPERAND_SHAMT5_MASK              0x01F00000
#define RV_OPERAND_SHAMT5_OFFSET            20

#define RV_OPERAND_SHAMT6_MASK              0x03F00000
#define RV_OPERAND_SHAMT6_OFFSET            20

#define RV_OPERAND_CSR_MASK                 0xFFF00000
#define RV_OPERAND_CSR_OFFSET               20

#define RV_OPERAND_CSR_UIMM_MASK            0x000F8000
#define RV_OPERAND_CSR_UIMM_OFFSET          15



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
