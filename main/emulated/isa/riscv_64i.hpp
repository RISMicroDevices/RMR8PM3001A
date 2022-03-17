#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Base RV64I
//

#ifndef __INCLUDE_RV64I
#define __INCLUDE_RV64I
#endif

#include "riscv.hpp"
#include "riscvmisc.hpp"
#include "riscv_32i.hpp"


// Function-3
#define RV64I_FUNCT3_MASK                       0x00007000
#define RV64I_FUNCT3_OFFSET                     12

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


// Function-6
#define RV64I_FUNCT6_MASK                       0xFC000000
#define RV64I_FUNCT6_OFFSET                     26

#define RV64I_FUNCT6_SLLI                       0b000000
#define RV64I_FUNCT6_SRLI                       0b000000
#define RV64I_FUNCT6_SRAI                       0b010000


// Function-7
#define RV64I_FUNCT7_MASK                       0xFE000000
#define RV64I_FUNCT7_OFFSET                     25

#define RV64I_FUNCT7_SLLIW                      0b0000000
#define RV64I_FUNCT7_SRLIW                      0b0000000
#define RV64I_FUNCT7_SRAIW                      0b0100000

#define RV64I_FUNCT7_ADDW                       0b0000000
#define RV64I_FUNCT7_SUBW                       0b0100000

#define RV64I_FUNCT7_SLLW                       0b0000000
#define RV64I_FUNCT7_SRLW                       0b0000000
#define RV64I_FUNCT7_SRAW                       0b0100000


//
#define __RV64I_UNIQUE_CODEGROUP(name) \
    RV64IUniqueCodegroup_##name

#define __RV64I_UNIQUE_CODEGROUP_DEFINE(name, function) \
    __RV64I_UNIQUE_CODEGROUP(name)::__RV64I_UNIQUE_CODEGROUP(name)() \
        : RV64IUniqueCodegroup (&function) \
    { } \

#define __RV64I_UNIQUE_CODEGROUP_DEFINE_FUNC(name, function) \
    bool function##(insnraw_t insnraw, RVInstruction& insn); \
    __RV64I_UNIQUE_CODEGROUP(name)::__RV64I_UNIQUE_CODEGROUP(name)() \
        : RV64IUniqueCodegroup (&function) \
    { } \
    bool function##(insnraw_t insnraw, RVInstruction& insn) \

#define __RV64I_UNIQUE_CODEGROUP_DECLARE(name) \
    class __RV64I_UNIQUE_CODEGROUP(name) : public RV64IUniqueCodegroup { \
    public: \
        __RV64I_UNIQUE_CODEGROUP(name)(); \
    }; \


#define __RV64I_FUNCT3_CODEGROUP(name) \
    RV64IFunct3Codegroup_##name

#define __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(name) \
    __RV64I_FUNCT3_CODEGROUP(name)::__RV64I_FUNCT3_CODEGROUP(name)()

#define __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(name) \
    __RV64I_FUNCT3_CODEGROUP(name)::~__RV64I_FUNCT3_CODEGROUP(name)()


#define __RV64I_FUNCT3_CODEGROUP_DECLARE(name) \
    class __RV64I_FUNCT3_CODEGROUP(name) : public RV64IFunct3Codegroup { \
    public: \
        __RV64I_FUNCT3_CODEGROUP(name)(); \
        ~__RV64I_FUNCT3_CODEGROUP(name)(); \
    }; \

//
namespace Jasse {

    // RV64I codepoint
    typedef bool (*RV64Codepoint)(insnraw_t insnraw, RVInstruction& insn);

    // RV64I codegroup (OPCODE base)
    class RV64Codegroup {
    public:
        virtual bool    Decode(insnraw_t insnraw, RVInstruction& insn) const = 0;
    };


    // RV64I unique codegroup
    class RV64IUniqueCodegroup : public RV64Codegroup {
    private:
        RV64Codepoint   codepoint;
    
    public:
        RV64IUniqueCodegroup(RV64Codepoint codepoint);
        ~RV64IUniqueCodegroup();

        virtual bool    Decode(insnraw_t insnraw, RVInstruction& insn) const override;
    };

    // RV64I FUNCT3 codegroup
    class RV64IFunct3Codegroup : public RV64Codegroup {
    private:
        RV64Codepoint*  codepoints; // 8 (2 ^ 3)

    public:
        RV64IFunct3Codegroup();
        ~RV64IFunct3Codegroup();

        void            Define(int funct3, RV64Codepoint codepoint);
        virtual bool    Decode(insnraw_t insnraw, RVInstruction& insn) const override;
    };


    // RV64I Decoder
    class RV64IDecoder : public RVDecoder {
    private:
        RV64IFunct3Codegroup**  codegroups; // 32 (2 ^ (7 - 2))

    public:
        RV64IDecoder();
        ~RV64IDecoder();

        virtual bool    Decode(insnraw_t insnraw, RVInstruction& insn) const override;
    };


    // Codepoint declarations
    __RV64I_FUNCT3_CODEGROUP_DECLARE(OP)
    __RV64I_FUNCT3_CODEGROUP_DECLARE(OP_32)
    __RV64I_FUNCT3_CODEGROUP_DECLARE(OP_IMM)
    __RV64I_FUNCT3_CODEGROUP_DECLARE(OP_IMM_32)

    __RV64I_FUNCT3_CODEGROUP_DECLARE(BRANCH)
    __RV64I_FUNCT3_CODEGROUP_DECLARE(LOAD)
    __RV64I_FUNCT3_CODEGROUP_DECLARE(STORE)
    __RV64I_FUNCT3_CODEGROUP_DECLARE(MISC_MEM)
    __RV64I_FUNCT3_CODEGROUP_DECLARE(SYSTEM)

    __RV64I_UNIQUE_CODEGROUP_DECLARE(LUI)
    __RV64I_UNIQUE_CODEGROUP_DECLARE(AUIPC)
    __RV64I_UNIQUE_CODEGROUP_DECLARE(JAL)
    __RV64I_UNIQUE_CODEGROUP_DECLARE(JALR)
};


// Implementation of: class RV64IUniqueCodegroup
namespace Jasse {
    RV64IUniqueCodegroup::RV64IUniqueCodegroup(RV64Codepoint codepoint)
        : codepoint (codepoint)
    { }

    RV64IUniqueCodegroup::~RV64IUniqueCodegroup()
    { }

    bool RV64IUniqueCodegroup::Decode(insnraw_t insnraw, RVInstruction& insn) const
    {
        return (*codepoint)(insnraw, insn);
    }
}


// Implementation of: class RV64IFunct3Codegroup
namespace Jasse {
    RV64IFunct3Codegroup::RV64IFunct3Codegroup()
        : codepoints(new RV64Codepoint[8]())
    { }

    RV64IFunct3Codegroup::~RV64IFunct3Codegroup()
    { 
        delete[] codepoints;
    }

    bool RV64IFunct3Codegroup::Decode(insnraw_t insnraw, RVInstruction& insn) const
    {
        int funct3 = GET_OPERAND(insnraw, RV64I_FUNCT3_MASK, RV64I_FUNCT3_OFFSET);

        if (codepoints[funct3])
            return (*(codepoints[funct3]))(insnraw, insn);
        else
            return false;
    }

    inline void RV64IFunct3Codegroup::Define(int funct3, RV64Codepoint codepoint)
    {
        codepoints[funct3] = codepoint;
    }
}


// executors
namespace Jasse {






    // ADDI
    void RV64I_ADDI(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()] 
            = arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
    }

    // SLTI
    void RV64I_SLTI(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = (int64_t)arch.GR64()[insn.GetRS1()] < (int64_t)insn.GetImmediate().imm64;
    }

    // SLTIU
    void RV64I_SLTIU(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = (uint64_t)arch.GR64()[insn.GetRS1()] < (uint64_t)insn.GetImmediate().imm64;
    }

    // ANDI
    void RV64I_ANDI(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.GR64()[insn.GetRS1()] & insn.GetImmediate().imm64;
    }

    // ORI
    void RV64I_ORI(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.GR64()[insn.GetRS1()] | insn.GetImmediate().imm64;
    }

    // XORI
    void RV64I_XORI(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.GR64()[insn.GetRS1()] ^ insn.GetImmediate().imm64;
    }

    
    // SLLI
    void RV64I_SLLI(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.GR64()[insn.GetRS1()] << insn.GetOperand(SHAMT6);
    }

    // SRLI
    void RV64I_SRLI(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = (uint64_t)arch.GR64()[insn.GetRS1()] >> insn.GetOperand(SHAMT6);
    }

    // SRAI
    void RV64I_SRAI(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = (int64_t)arch.GR64()[insn.GetRS1()] >> insn.GetOperand(SHAMT6);
    }


    // ADD
    void RV64I_ADD(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.GR64()[insn.GetRS1()] + arch.GR64()[insn.GetRS2()];
    }

    // SUB
    void RV64I_SUB(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.GR64()[insn.GetRS1()] - arch.GR64()[insn.GetRS2()];
    }

    // SLT
    void RV64I_SLT(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = (int64_t)arch.GR64()[insn.GetRS1()] < (int64_t)arch.GR64()[insn.GetRS2()];
    }

    // SLTU
    void RV64I_SLTU(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = (uint64_t)arch.GR64()[insn.GetRS1()] < (uint64_t)arch.GR64()[insn.GetRS2()];
    }

    // AND
    void RV64I_AND(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.GR64()[insn.GetRS1()] & arch.GR64()[insn.GetRS2()];
    }

    // OR
    void RV64I_OR(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.GR64()[insn.GetRS1()] | arch.GR64()[insn.GetRS2()];
    }

    // XOR
    void RV64I_XOR(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.GR64()[insn.GetRS1()] ^ arch.GR64()[insn.GetRS2()];
    }

    // SLL
    void RV64I_SLL(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.GR64()[insn.GetRS1()] << (arch.GR64()[insn.GetRS2()] & 0x003F);
    }

    // SRL
    void RV64I_SRL(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = (uint64_t)arch.GR64()[insn.GetRS1()] >> (arch.GR64()[insn.GetRS2()] & 0x003F);
    }

    // SRA
    void RV64I_SRA(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = (int64_t)arch.GR64()[insn.GetRS1()] >> (arch.GR64()[insn.GetRS2()] & 0x003F);
    }


    // LUI
    void RV64I_LUI(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()] 
            = insn.GetImmediate().imm64;
    }

    // AUIPC
    void RV64I_AUIPC(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()] 
            = insn.GetImmediate().imm64 + arch.PC().pc64;
    }


    // JAL
    void RV64I_JAL(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.PC().pc64 + 4;

        arch.SetPC64(
            arch.PC().pc64 + insn.GetImmediate().imm64);
    }

    // JALR
    void RV64I_JALR(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.PC().pc64 + 4;

        arch.SetPC64(
            (arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64) & 0xFFFFFFFFFFFFFFFELU);
    }


    // BEQ
    void RV64I_BEQ(const RVInstruction& insn, RVArchitectural& arch)
    {
        if (arch.GR64()[insn.GetRS1()] == arch.GR64()[insn.GetRS2()])
            arch.SetPC64(arch.PC().pc64 + insn.GetImmediate().imm64);
    }

    // BNE
    void RV64I_BNE(const RVInstruction& insn, RVArchitectural& arch)
    {
        if (arch.GR64()[insn.GetRS1()] != arch.GR64()[insn.GetRS2()])
            arch.SetPC64(arch.PC().pc64 + insn.GetImmediate().imm64);
    }

    // BLT
    void RV64I_BLT(const RVInstruction& insn, RVArchitectural& arch)
    {
        if ((int64_t)arch.GR64()[insn.GetRS1()] < (int64_t)arch.GR64()[insn.GetRS2()])
            arch.SetPC64(arch.PC().pc64 + insn.GetImmediate().imm64);
    }

    // BLTU
    void RV64I_BLTU(const RVInstruction& insn, RVArchitectural& arch)
    {
        if ((uint64_t)arch.GR64()[insn.GetRS1()] < (uint64_t)arch.GR64()[insn.GetRS2()])
            arch.SetPC64(arch.PC().pc64 + insn.GetImmediate().imm64);
    }

    // BGE
    void RV64I_BGE(const RVInstruction& insn, RVArchitectural& arch)
    {
        if ((int64_t)arch.GR64()[insn.GetRS1()] >= (int64_t)arch.GR64()[insn.GetRS2()])
            arch.SetPC64(arch.PC().pc64 + insn.GetImmediate().imm64);
    }

    // BGEU
    void RV64I_BGEU(const RVInstruction& insn, RVArchitectural& arch)
    {
        if ((uint64_t)arch.GR64()[insn.GetRS1()] >= (uint64_t)arch.GR64()[insn.GetRS2()])
            arch.SetPC64(arch.PC().pc64 + insn.GetImmediate().imm64);
    }
    

}


#define RV64I_INSNDEF(name, executor) \
    insn.SetName(name); \
    insn.SetExecutor(&executor);

// codepoints
namespace Jasse {


    //
    bool RV64ICodePoint_Funct3_ADDI(insnraw_t insnraw, RVInstruction& insn)
    {
        DecodeNormalRV64TypeI(insnraw, insn);
        RV64I_INSNDEF("addi", RV64I_ADDI)

        return true;
    }

    bool RV64ICodePoint_Funct3_SLTI(insnraw_t insnraw, RVInstruction& insn)
    {
        DecodeNormalRV64TypeI(insnraw, insn);
        RV64I_INSNDEF("slti", RV64I_ADDI)

        return true;
    }

    bool RV64ICodePoint_Funct3_SLTIU(insnraw_t insnraw, RVInstruction& insn)
    {
        DecodeNormalRV64TypeI(insnraw, insn);
        RV64I_INSNDEF("sltiu", RV64I_SLTIU)

        return true;
    }

    bool RV64ICodePoint_Funct3_ANDI(insnraw_t insnraw, RVInstruction& insn)
    {
        DecodeNormalRV64TypeI(insnraw, insn);
        RV64I_INSNDEF("andi", RV64I_ANDI)

        return true;
    }

    bool RV64ICodePoint_Funct3_ORI(insnraw_t insnraw, RVInstruction& insn)
    {
        DecodeNormalRV64TypeI(insnraw, insn);
        RV64I_INSNDEF("ori", RV64I_ORI)

        return true;
    }

    bool RV64ICodePoint_Funct3_XORI(insnraw_t insnraw, RVInstruction& insn)
    {
        DecodeNormalRV64TypeI(insnraw, insn);
        RV64I_INSNDEF("xori", RV64I_XORI)

        return true;
    }

    //
    bool RV64ICodePoint_Funct3_SLLI(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct6 = GET_STD_OPERAND(insnraw, RV64I_FUNCT6);

        if (funct6 == RV64I_FUNCT6_SLLI)
        {
            DecodeNormalRV64TypeI(insnraw, insn);
            RV64I_INSNDEF("slli", RV64I_SLLI)

            return true;
        }
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_SRLI__SRAI(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct6 = GET_STD_OPERAND(insnraw, RV64I_FUNCT6);

        if (funct6 == RV64I_FUNCT6_SRLI)
        {
            DecodeNormalRV64TypeI(insnraw, insn);
            RV64I_INSNDEF("srli", RV64I_SRLI)

            return true;
        }
        else if (funct6 == RV64I_FUNCT6_SRAI)
        {
            DecodeNormalRV64TypeI(insnraw, insn);
            RV64I_INSNDEF("srai", RV64I_SRAI)

            return true;
        }
        else
            return false;
    }

    //
    bool RV64ICodePoint_Funct3_ADD__SUB(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV32I_FUNCT7);

        if (funct7 == RV32I_FUNCT7_ADD)
        {
            DecodeNormalRV64TypeR(insnraw, insn);
            RV64I_INSNDEF("add", RV64I_ADD)

            return true;
        }
        else if (funct7 == RV32I_FUNCT7_SUB)
        {
            DecodeNormalRV64TypeR(insnraw, insn);
            RV64I_INSNDEF("sub", RV64I_SUB)

            return true;
        }
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_SLT(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV32I_FUNCT7);

        if (funct7 == RV32I_FUNCT7_SLT)
        {
            DecodeNormalRV64TypeR(insnraw, insn);
            RV64I_INSNDEF("slt", RV64I_SLT)

            return true;
        }
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_SLTU(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV32I_FUNCT7);

        if (funct7 == RV32I_FUNCT7_SLTU)
        {
            DecodeNormalRV64TypeR(insnraw, insn);
            RV64I_INSNDEF("sltu", RV64I_SLTU)

            return true;
        }
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_AND(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV32I_FUNCT7);

        if (funct7 == RV32I_FUNCT7_AND)
        {
            DecodeNormalRV64TypeR(insnraw, insn);
            RV64I_INSNDEF("and", RV64I_AND)

            return true;
        }
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_OR(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV32I_FUNCT7);

        if (funct7 == RV32I_FUNCT7_OR)
        {
            DecodeNormalRV64TypeR(insnraw, insn);
            RV64I_INSNDEF("or", RV64I_OR)

            return true;
        }
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_XOR(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV32I_FUNCT7);

        if (funct7 == RV32I_FUNCT7_XOR)
        {
            DecodeNormalRV64TypeR(insnraw, insn);
            RV64I_INSNDEF("xor", RV64I_XOR)

            return true;
        }
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_SLL(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV32I_FUNCT7);

        if (funct7 == RV32I_FUNCT7_SLL)
        {
            DecodeNormalRV64TypeR(insnraw, insn);
            RV64I_INSNDEF("sll", RV64I_SLL)

            return true;
        }
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_SRL__SRA(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV32I_FUNCT7);

        if (funct7 == RV32I_FUNCT7_SRL)
        {
            DecodeNormalRV64TypeR(insnraw, insn);
            RV64I_INSNDEF("srl", RV64I_SRL)

            return true;
        }
        else if (funct7 == RV32I_FUNCT7_SRA)
        {
            DecodeNormalRV64TypeR(insnraw, insn);
            RV64I_INSNDEF("sra", RV64I_SRA)

            return true;
        }
        else
            return false;
    }



    //
}

// codegroups
namespace Jasse {
    //
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(OP_IMM)
    {

    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(OP_IMM)
    { }

    //
    __RV64I_UNIQUE_CODEGROUP_DEFINE_FUNC(LUI, RV64ICodeGroup_Unique_LUI)
    {
        DecodeNormalRV64TypeU(insnraw, insn);
        RV64I_INSNDEF("lui", RV64I_LUI)

        return true;
    }

    //
    __RV64I_UNIQUE_CODEGROUP_DEFINE_FUNC(AUIPC, RV64ICodeGroup_Unique_AUIPC)
    {
        DecodeNormalRV64TypeU(insnraw, insn);
        RV64I_INSNDEF("auipc", RV64I_AUIPC)

        return true;
    }
}




