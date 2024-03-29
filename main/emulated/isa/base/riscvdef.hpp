#pragma once
//
// RISC-V Instruction Set Architecture Definitions
//
//
//

#include <cstdint>


#define decdef          const RVDecoder* const
#define decdecl         extern const RVDecoder* const

#define codedef         const RVCodepoint
#define codedecl        extern const RVCodepoint

#define codesetdef      const RVCodepointCollection
#define codesetdecl     extern const RVCodepointCollection


// Global type definitions
namespace Jasse {

    // Type definition of Architectural Register Value of XLEN=32
    typedef     uint32_t        arch32_t;

    // Type definition of Architectural Register Value of XLEN=64
    typedef     uint64_t        arch64_t;

    // Type definition of Immediate Value of XLEN=32/64
    typedef     uint32_t        imm_t;

    // Type definition of PC Register Value of XLEN=32/64
    typedef union {
        arch64_t    pc64 = 0;
        arch32_t    pc32;
    } pc_t;

    // Type definition of Raw Instruction
    typedef     uint32_t        insnraw_t;


    // X-Len Enumeration
    typedef enum {
        XLEN32 = 0,
        XLEN64
    } XLen;


    // RISC-V Codepoint (declaration)
    class RVCodepoint;

    // RISC-V Codepoint Type
    typedef enum {
        RVTYPE_R = 0,
        RVTYPE_I,
        RVTYPE_S,
        RVTYPE_B,
        RVTYPE_U,
        RVTYPE_J
    } RVCodepointType;


    // RISC-V Instruction Instance
    class RVInstruction;


    // RISC-V Instruction Execution Status
    // *NOTICE: Special actions (e.g. WFI-Wait For Interrupt) should be handled by EEI or 
    //          external EEI-defined components.
    //          They aren't and shouldn't be embedded in the instruction execution procedures.
    //          The processing procedure of external interrupts MUST run ahead of any instruction
    //          execution procedure.
    typedef enum {
        // Naturally increase the PC. Execute instruction stream in sequence normally.
        EXEC_SEQUENTIAL = 0,

        // PC Hold. PC shouldn't be changed in instruction execution procedure.
        // And PC value MUST NOT be manipulated in EEI instance or out of iustruction
        // execution procedure.
        EXEC_PC_HOLD,

        // PC Jump. PC is changed in instruction execution procedure.
        // And PC value MUST NOT be manipulated in EEI instance or out of iustruction
        // execution procedure.
        EXEC_PC_JUMP,

        // Trap Enter. Results of instruction execution procedure shouldn't be commited
        // to the architectural states.
        // Trap-related CSRs or EEI-defined related registers were manipulated in
        // instruction execution procedure.
        // @see Jasse::TrapEnter (in header "riscvexcept.hpp")
        EXEC_TRAP_ENTER,

        // Trap Return. 
        // Trap-related CSRs or EEI-defined related registers were manipulated in
        // instruction execution procedure.
        // @see Jasse::TrapReturn (in header "riscvexcept.hpp")
        EXEC_TRAP_RETURN,

        // Fetch Access Fault. *EEI-defined*
        // MOP_ACCESS_FAULT when fetching instruction by PC from Memory Interface.
        // This status SHOULDN'T be produced in any execution procedure.
        // The way of handling this status (Raising an exception, or .etc) is EEI-defined.
        EXEC_FETCH_ACCESS_FAULT,

        // Fetch Address Misaligned. *EEI-defined*
        // MOP_ADDRESS_MISALIGNED when fetching instruction by PC from Memory Interface.
        // This status SHOULDN'T be produced in any execution procedure.
        // The way of handling this status (Raising an exception, or .etc) is EEI-defined.
        EXEC_FETCH_ADDRESS_MISALIGNED,

        // WFI. Wait-For-Interrupt. *EEI-defined*
        // WFI function is implemented in EEI. Interrupt-related procedures were not
        // necessary in instruction execution procedure. 
        // For the simplest implementation, PC is held still till a interrupt occurs.
        EXEC_WAIT_FOR_INTERRUPT,

        // Not Implemented. *EEI-defined*
        // Instruction codepoint exists, but the execution procedure not implemented.
        // The way of handling this status (Raising an exception, or .etc) is EEI-defined.
        EXEC_NOT_IMPLEMENTED,

        // Not Decoded. (Not implemented in codepoint & decoders). *EEI-defined*
        // Instruction codepoint doesn't exist. This usually indicates that an instruction
        // was properly fetched but it was illegal.
        // This status SHOULDN'T be produced in any execution procedure.
        // The way of handling this status (Raising an exception, or .etc) is EEI-defined.
        EXEC_NOT_DECODED
    } RVExecStatus;
}


// Integral extending
#define SEXT_W(expr)    ((uint64_t) ((int32_t) (expr)))
#define SEXT_H(expr)    ((uint64_t) ((int16_t) (expr)))
#define SEXT_B(expr)    ((uint64_t) ((int8_t)  (expr)))

#define ZEXT_W(expr)    ((uint64_t) ((uint32_t) (expr)))
#define ZEXT_H(expr)    ((uint64_t) ((uint16_t) (expr)))
#define ZEXT_B(expr)    ((uint64_t) ((uint8_t)  (expr)))


// Standard operand format
#define GET_OPERAND(insn, mask, offset) \
    ((insn & mask) >> offset)

#define GET_STD_OPERAND(insn, name) \
    ((insn & name##_MASK) >> name##_OFFSET)


// Basis
#define RV_ARCH_REG_COUNT                   32

#define RV_FUNCT3_MASK                      0x00007000U
#define RV_FUNCT3_OFFSET                    12

#define RV_FUNCT7_MASK                      0xFE000000U
#define RV_FUNCT7_OFFSET                    25

#define RV_FUNCT12_MASK                     0xFFF00000U
#define RV_FUNCT12_OFFSET                   20


// Operand
#define RV_OPERAND_RD_MASK                  0x00000F80U
#define RV_OPERAND_RD_OFFSET                7

#define RV_OPERAND_RS1_MASK                 0x000F8000U
#define RV_OPERAND_RS1_OFFSET               15

#define RV_OPERAND_RS2_MASK                 0x01F00000U
#define RV_OPERAND_RS2_OFFSET               20

#define RV_OPERAND_SHAMT5_MASK              0x01F00000U
#define RV_OPERAND_SHAMT5_OFFSET            20

#define RV_OPERAND_SHAMT6_MASK              0x03F00000U
#define RV_OPERAND_SHAMT6_OFFSET            20

#define RV_OPERAND_CSR_MASK                 0xFFF00000U
#define RV_OPERAND_CSR_OFFSET               20

#define RV_OPERAND_CSR_UIMM_MASK            0x000F8000U
#define RV_OPERAND_CSR_UIMM_OFFSET          15



// Opcode
#define RV_OPCODE_MASK                      0x0000007FU
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


// General Registers
#define RV_GR_X0                            0
#define RV_GR_X1                            1
#define RV_GR_X2                            2
#define RV_GR_X3                            3
#define RV_GR_X4                            4
#define RV_GR_X5                            5
#define RV_GR_X6                            6
#define RV_GR_X7                            7
#define RV_GR_X8                            8
#define RV_GR_X9                            9
#define RV_GR_X10                           10
#define RV_GR_X11                           11
#define RV_GR_X12                           12
#define RV_GR_X13                           13
#define RV_GR_X14                           14
#define RV_GR_X15                           15
#define RV_GR_X16                           16
#define RV_GR_X17                           17
#define RV_GR_X18                           18
#define RV_GR_X19                           19
#define RV_GR_X20                           20
#define RV_GR_X21                           21
#define RV_GR_X22                           22
#define RV_GR_X23                           23
#define RV_GR_X24                           24
#define RV_GR_X25                           25
#define RV_GR_X26                           26
#define RV_GR_X27                           27
#define RV_GR_X28                           28
#define RV_GR_X29                           29
#define RV_GR_X30                           30
#define RV_GR_X31                           31
