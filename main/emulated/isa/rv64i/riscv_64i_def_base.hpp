#pragma once
//
// RISC-V Instruction Set Architecture Emulator (Jasse)
//
// Base RV64I, basic definitions
//


#include "riscv_32i.hpp"


// Function-3
#define RV64I_FUNCT3_MASK                       RV_FUNCT3_MASK
#define RV64I_FUNCT3_OFFSET                     RV_FUNCT3_OFFSET

#define RV64I_FUNCT3_ADDI                       RV32I_FUNCT3_ADDI 
#define RV64I_FUNCT3_SLTI                       RV32I_FUNCT3_SLTI 
#define RV64I_FUNCT3_SLTIU                      RV32I_FUNCT3_SLTIU
#define RV64I_FUNCT3_XORI                       RV32I_FUNCT3_XORI 
#define RV64I_FUNCT3_ORI                        RV32I_FUNCT3_ORI  
#define RV64I_FUNCT3_ANDI                       RV32I_FUNCT3_ANDI 

#define RV64I_FUNCT3_SLLI                       RV32I_FUNCT3_SLLI      
#define RV64I_FUNCT3_SRLI                       RV32I_FUNCT3_SRLI      
#define RV64I_FUNCT3_SRAI                       RV32I_FUNCT3_SRAI      
#define RV64I_FUNCT3_SRLI__SRAI                 RV32I_FUNCT3_SRLI__SRAI

#define RV64I_FUNCT3_ADD                        RV32I_FUNCT3_ADD      
#define RV64I_FUNCT3_SUB                        RV32I_FUNCT3_SUB      
#define RV64I_FUNCT3_ADD__SUB                   RV32I_FUNCT3_ADD__SUB 
#define RV64I_FUNCT3_SLL                        RV32I_FUNCT3_SLL      
#define RV64I_FUNCT3_SLT                        RV32I_FUNCT3_SLT      
#define RV64I_FUNCT3_SLTU                       RV32I_FUNCT3_SLTU     
#define RV64I_FUNCT3_XOR                        RV32I_FUNCT3_XOR      
#define RV64I_FUNCT3_SRL                        RV32I_FUNCT3_SRL      
#define RV64I_FUNCT3_SRA                        RV32I_FUNCT3_SRA      
#define RV64I_FUNCT3_SRL__SRA                   RV32I_FUNCT3_SRL__SRA 
#define RV64I_FUNCT3_OR                         RV32I_FUNCT3_OR       
#define RV64I_FUNCT3_AND                        RV32I_FUNCT3_AND      

#define RV64I_FUNCT3_JALR                       RV32I_FUNCT3_JALR
#define RV64I_FUNCT3_BEQ                        RV32I_FUNCT3_BEQ 
#define RV64I_FUNCT3_BNE                        RV32I_FUNCT3_BNE 
#define RV64I_FUNCT3_BLT                        RV32I_FUNCT3_BLT 
#define RV64I_FUNCT3_BGE                        RV32I_FUNCT3_BGE 
#define RV64I_FUNCT3_BLTU                       RV32I_FUNCT3_BLTU
#define RV64I_FUNCT3_BGEU                       RV32I_FUNCT3_BGEU

#define RV64I_FUNCT3_LB                         RV32I_FUNCT3_LB 
#define RV64I_FUNCT3_LH                         RV32I_FUNCT3_LH 
#define RV64I_FUNCT3_LW                         RV32I_FUNCT3_LW 
#define RV64I_FUNCT3_LBU                        RV32I_FUNCT3_LBU
#define RV64I_FUNCT3_LHU                        RV32I_FUNCT3_LHU

#define RV64I_FUNCT3_SB                         RV32I_FUNCT3_SB
#define RV64I_FUNCT3_SH                         RV32I_FUNCT3_SH
#define RV64I_FUNCT3_SW                         RV32I_FUNCT3_SW

#define RV64I_FUNCT3_FENCE                      RV32I_FUNCT3_FENCE

#define RV64I_FUNCT3_ADDIW                      0b000
#define RV64I_FUNCT3_SLLIW                      0b001
#define RV64I_FUNCT3_SRLIW                      0b101
#define RV64I_FUNCT3_SRAIW                      0b101
#define RV64I_FUNCT3_SRLIW__SRAIW               0b101

#define RV64I_FUNCT3_ADDW                       0b000
#define RV64I_FUNCT3_SUBW                       0b000
#define RV64I_FUNCT3_ADDW__SUBW                 0b000
#define RV64I_FUNCT3_SLLW                       0b001
#define RV64I_FUNCT3_SRLW                       0b101
#define RV64I_FUNCT3_SRAW                       0b101
#define RV64I_FUNCT3_SRLW__SRAW                 0b101

#define RV64I_FUNCT3_LD                         0b011
#define RV64I_FUNCT3_LWU                        0b110
#define RV64I_FUNCT3_SD                         0b011

#define RV64I_FUNCT3_ECALL                      RV32I_FUNCT3_ECALL 
#define RV64I_FUNCT3_EBREAK                     RV32I_FUNCT3_EBREAK

#define RV64I_FUNCT3_SRET                       RV32I_FUNCT3_SRET  
#define RV64I_FUNCT3_MRET                       RV32I_FUNCT3_MRET  

#define RV64I_FUNCT3_WFI                        RV32I_FUNCT3_WFI   


// Function-6
#define RV64I_FUNCT6_MASK                       0xFC000000U
#define RV64I_FUNCT6_OFFSET                     26

#define RV64I_FUNCT6_SLLI                       0b000000
#define RV64I_FUNCT6_SRLI                       0b000000
#define RV64I_FUNCT6_SRAI                       0b010000


// Function-7
#define RV64I_FUNCT7_MASK                       RV_FUNCT7_MASK
#define RV64I_FUNCT7_OFFSET                     RV_FUNCT7_OFFSET

#define RV64I_FUNCT7_ADD                        RV32I_FUNCT7_ADD 
#define RV64I_FUNCT7_SUB                        RV32I_FUNCT7_SUB 

#define RV64I_FUNCT7_SLL                        RV32I_FUNCT7_SLL 
#define RV64I_FUNCT7_SLT                        RV32I_FUNCT7_SLT 
#define RV64I_FUNCT7_SLTU                       RV32I_FUNCT7_SLTU
#define RV64I_FUNCT7_XOR                        RV32I_FUNCT7_XOR 
#define RV64I_FUNCT7_OR                         RV32I_FUNCT7_OR  
#define RV64I_FUNCT7_AND                        RV32I_FUNCT7_AND 

#define RV64I_FUNCT7_SRL                        RV32I_FUNCT7_SRL 
#define RV64I_FUNCT7_SRA                        RV32I_FUNCT7_SRA 

#define RV64I_FUNCT7_SLLIW                      0b0000000
#define RV64I_FUNCT7_SRLIW                      0b0000000
#define RV64I_FUNCT7_SRAIW                      0b0100000

#define RV64I_FUNCT7_ADDW                       0b0000000
#define RV64I_FUNCT7_SUBW                       0b0100000

#define RV64I_FUNCT7_SLLW                       0b0000000
#define RV64I_FUNCT7_SRLW                       0b0000000
#define RV64I_FUNCT7_SRAW                       0b0100000

// Function-12
#define RV64I_FUNCT12_MASK                      RV32I_FUNCT12_MASK  
#define RV64I_FUNCT12_OFFSET                    RV32I_FUNCT12_OFFSET

#define RV64I_FUNCT12_ECALL                     RV32I_FUNCT12_ECALL 
#define RV64I_FUNCT12_EBREAK                    RV32I_FUNCT12_EBREAK

#define RV64I_FUNCT12_SRET                      RV32I_FUNCT12_SRET  
#define RV64I_FUNCT12_MRET                      RV32I_FUNCT12_MRET  

#define RV64I_FUNCT12_WFI                       RV32I_FUNCT12_WFI   


// Operands
#define RV64I_OPERAND_FENCE_FM_MASK             RV32I_OPERAND_FENCE_FM_MASK  
#define RV64I_OPERAND_FENCE_FM_OFFSET           RV32I_OPERAND_FENCE_FM_OFFSET

#define RV64I_OPERAND_FENCE_PI_MASK             RV32I_OPERAND_FENCE_PI_MASK  
#define RV64I_OPERAND_FENCE_PI_OFFSET           RV32I_OPERAND_FENCE_PI_OFFSET

#define RV64I_OPERAND_FENCE_PO_MASK             RV32I_OPERAND_FENCE_PO_MASK  
#define RV64I_OPERAND_FENCE_PO_OFFSET           RV32I_OPERAND_FENCE_PO_OFFSET

#define RV64I_OPERAND_FENCE_PR_MASK             RV32I_OPERAND_FENCE_PR_MASK  
#define RV64I_OPERAND_FENCE_PR_OFFSET           RV32I_OPERAND_FENCE_PR_OFFSET

#define RV64I_OPERAND_FENCE_PW_MASK             RV32I_OPERAND_FENCE_PW_MASK  
#define RV64I_OPERAND_FENCE_PW_OFFSET           RV32I_OPERAND_FENCE_PW_OFFSET

#define RV64I_OPERAND_FENCE_SI_MASK             RV32I_OPERAND_FENCE_SI_MASK  
#define RV64I_OPERAND_FENCE_SI_OFFSET           RV32I_OPERAND_FENCE_SI_OFFSET

#define RV64I_OPERAND_FENCE_SO_MASK             RV32I_OPERAND_FENCE_SO_MASK  
#define RV64I_OPERAND_FENCE_SO_OFFSET           RV32I_OPERAND_FENCE_SO_OFFSET

#define RV64I_OPERAND_FENCE_SR_MASK             RV32I_OPERAND_FENCE_SR_MASK  
#define RV64I_OPERAND_FENCE_SR_OFFSET           RV32I_OPERAND_FENCE_SR_OFFSET

#define RV64I_OPERAND_FENCE_SW_MASK             RV32I_OPERAND_FENCE_SW_MASK  
#define RV64I_OPERAND_FENCE_SW_OFFSET           RV32I_OPERAND_FENCE_SW_OFFSET

