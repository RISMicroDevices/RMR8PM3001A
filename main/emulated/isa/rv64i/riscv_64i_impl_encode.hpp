#pragma once
//
// RISC-V Instruction Set Architecture Emulator (Jasse)
//
// Base RV64I, codepoint infrastructure definitions
//

#include "riscvmisc.hpp"
#include "riscvgenutil.hpp"
#include "riscv_64i_def_encode.hpp"

#define __RV64I_DUCGEI(name) \
    &RV64ICodeGroup_Unique_##name, \
    &CodeGenRVTypeI<&AllocRVEncoderTypeI<RV_OPCODE_##name, RV64I_FUNCT3_##name>>, \
    &RV64IExecutor_##name

#define __RV64I_DUCGEJ(name) \
    &RV64ICodeGroup_Unique_##name, \
    &CodeGenRVTypeJ<&AllocRVEncoderTypeJ<RV_OPCODE_##name>>, \
    &RV64IExecutor_##name

#define __RV64I_D3CGEI_OP_IMM(name) \
    &RV64ICodePoint_Funct3_##name, \
    &CodeGenRVTypeI<&AllocRVEncoderTypeI<RV_OPCODE_OP_IMM, RV64I_FUNCT3_##name>>, \
    &RV64IExecutor_##name

#define __RV64I_D3CGEI_OP_IMM32(name) \
    &RV64ICodePoint_Funct3_##name, \
    &CodeGenRVTypeI<&AllocRVEncoderTypeI<RV_OPCODE_OP_IMM_32, RV64I_FUNCT3_##name>>, \
    &RV64IExecutor_##name

#define __RV64I_D3CGER_OP(name) \
    &RV64ICodePoint_Funct3_##name, \
    &CodeGenRVTypeR<&AllocRVEncoderTypeR<RV_OPCODE_OP, RV64I_FUNCT3_##name, RV64I_FUNCT7_##name>>, \
    &RV64IExecutor_##name

#define __RV64I_D3CGER_OP32(name) \
    &RV64ICodePoint_Funct3_##name, \
    &CodeGenRVTypeR<&AllocRVEncoderTypeR<RV_OPCODE_OP_32, RV64I_FUNCT3_##name, RV64I_FUNCT7_##name>>, \
    &RV64IExecutor_##name

#define __RV64I_D3CGEB_BRCH(name) \
    &RV64ICodePoint_Funct3_##name, \
    &CodeGenRVTypeB<&AllocRVEncoderTypeB<RV_OPCODE_BRANCH, RV64I_FUNCT3_##name>>, \
    &RV64IExecutor_##name

#define __RV64I_D3CGEI_LD(name) \
    &RV64ICodePoint_Funct3_##name, \
    &CodeGenRVTypeI<&AllocRVEncoderTypeI<RV_OPCODE_LOAD, RV64I_FUNCT3_##name>>, \
    &RV64IExecutor_##name

#define __RV64I_D3CGES_ST(name) \
    &RV64ICodePoint_Funct3_##name, \
    &CodeGenRVTypeS<&AllocRVEncoderTypeS<RV_OPCODE_STORE, RV64I_FUNCT3_##name>>, \
    &RV64IExecutor_##name

#define __RV64I_D12CGEI_SYS(name) \
    &RV64ICodePoint_Funct12_##name, \
    &CodeGenRVZeroOperand<&AllocRVEncoderTypeIZeroOperand<RV_OPCODE_SYSTEM, RV64I_FUNCT3_##name, RV64I_FUNCT12_##name>>, \
    &RV64IExecutor_##name

#define __RV64I_DUCGEU(name) \
    &RV64ICodeGroup_Unique_##name, \
    &CodeGenRVTypeU<&AllocRVEncoderTypeU<RV_OPCODE_##name>>, \
    &RV64IExecutor_##name

#define __RV64I_D3CGEI_SHx(name) \
    &RV64ICodePoint_Funct3_##name, \
    &CodeGenRVTypeI<&AllocRVEncoderSHx<RV_OPCODE_OP_IMM, RV64I_FUNCT3_##name, RV64I_FUNCT6_##name>>, \
    &RV64IExecutor_##name

#define __RV64I_D3CGEI_SHx32(name) \
    &RV64ICodePoint_Funct3_##name, \
    &CodeGenRVTypeI<&AllocRVEncoderSHx32<RV_OPCODE_OP_IMM_32, RV64I_FUNCT3_##name, RV64I_FUNCT7_##name>>, \
    &RV64IExecutor_##name

#define __RV64I_DUCGEI_FENCE \
    &RV64ICodeGroup_Unique_MISC_MEM, \
    &CodeGenRVTypeI<&AllocRVEncoderFence>, \
    &RV64IExecutor_FENCE

// Specialized encoder: RV64IEncoderSHx
namespace Jasse {

    class RV64IEncoderSHx : public RVEncoderTypeI {
    public:
        RV64IEncoderSHx(int opcode, int funct3, int funct6) noexcept;

        virtual void    SetImmediate(imm_t imm) noexcept override;
    };

    //
    template<int opcode, int funct3, int funct6> RVEncoder* AllocRVEncoderSHx() noexcept
    {
        return new RV64IEncoderSHx(opcode, funct3, funct6);
    }

    //
    RV64IEncoderSHx::RV64IEncoderSHx(int opcode, int funct3, int funct6) noexcept
        : RVEncoderTypeI(opcode, funct3)
    {
        insn |= ENCODE_STD_OPERAND(funct6, RV64I_FUNCT6);
    }

    void RV64IEncoderSHx::SetImmediate(imm_t imm) noexcept
    {
        insn |= ENCODE_STD_OPERAND(imm, RV_OPERAND_SHAMT6);
    }
}

// Specialized encoder: RV64IEncoderSHx32
namespace Jasse {

    class RV64IEncoderSHx32 : public RVEncoderTypeI {
    public:
        RV64IEncoderSHx32(int opcode, int funct3, int funct7) noexcept;

        virtual void    SetImmediate(imm_t imm) noexcept override;
    };

    // 
    template<int opcode, int funct3, int funct7> RVEncoder* AllocRVEncoderSHx32() noexcept
    {
        return new RV64IEncoderSHx32(opcode, funct3, funct7);
    }

    //
    RV64IEncoderSHx32::RV64IEncoderSHx32(int opcode, int funct3, int funct7) noexcept
        : RVEncoderTypeI(opcode, funct3)
    {
        insn |= ENCODE_STD_OPERAND(funct7, RV64I_FUNCT7);
    }

    void RV64IEncoderSHx32::SetImmediate(imm_t imm) noexcept
    {
        insn |= ENCODE_STD_OPERAND(imm, RV_OPERAND_SHAMT5);
    }
}

// Specialized encoder: RV64IEncoderFence
namespace Jasse {

    class RV64IEncoderFence : public RVEncoderTypeIZeroOperand {
    public:
        RV64IEncoderFence(int fm, int pred, int succ) noexcept;
        RV64IEncoderFence() noexcept;
    };

    //
    RVEncoder* AllocRVEncoderFence() noexcept
    {
        return new RV64IEncoderFence();
    }

    //
    RV64IEncoderFence::RV64IEncoderFence(int fm, int pred, int succ) noexcept
        : RVEncoderTypeIZeroOperand (RV_OPCODE_MISC_MEM, RV64I_FUNCT3_FENCE, 0)
    { 
        insn |= ENCODE_RV64I_FENCE(fm, pred, succ);
    }

    RV64IEncoderFence::RV64IEncoderFence() noexcept
        : RV64IEncoderFence (0, 0x0F, 0x0F)
    { }
}
