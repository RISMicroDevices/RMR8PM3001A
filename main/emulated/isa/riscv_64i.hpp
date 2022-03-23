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
#include "riscvexcept.hpp"
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

#define RV64I_FUNCT3_LD                         0b011
#define RV64I_FUNCT3_LWU                        0b110
#define RV64I_FUNCT3_SD                         0b011


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
            = arch.GR64()[insn.GetRS1()] << insn.GetOperand(RVOPERAND_SHAMT6);
    }

    // SRLI
    void RV64I_SRLI(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = (uint64_t)arch.GR64()[insn.GetRS1()] >> insn.GetOperand(RVOPERAND_SHAMT6);
    }

    // SRAI
    void RV64I_SRAI(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = (int64_t)arch.GR64()[insn.GetRS1()] >> insn.GetOperand(RVOPERAND_SHAMT6);
    }


    // ADDIW
    void RV64I_ADDIW(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = SEXT_W((uint32_t)arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm32);
    }

    // SLLIW
    void RV64I_SLLIW(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = SEXT_W((uint32_t)arch.GR64()[insn.GetRS1()] << insn.GetOperand(RVOPERAND_SHAMT5));
    }

    // SRLIW
    void RV64I_SRLIW(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = SEXT_W((uint32_t)arch.GR64()[insn.GetRS1()] >> insn.GetOperand(RVOPERAND_SHAMT5));
    }

    // SRAIW
    void RV64I_SRAIW(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = SEXT_W((int32_t)arch.GR64()[insn.GetRS1()] >> insn.GetOperand(RVOPERAND_SHAMT5));
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


    // ADDW
    void RV64I_ADDW(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = SEXT_W((uint32_t)arch.GR64()[insn.GetRS1()] + (uint32_t)arch.GR64()[insn.GetRS2()]);
    }

    // SUBW
    void RV64I_SUBW(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = SEXT_W((uint32_t)arch.GR64()[insn.GetRS1()] - (uint32_t)arch.GR64()[insn.GetRS2()]);
    }

    // SLLW
    void RV64I_SLLW(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = SEXT_W((uint32_t)arch.GR64()[insn.GetRS1()] << (arch.GR64()[insn.GetRS2()] & 0x001F));
    }

    // SRLW
    void RV64I_SRLW(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = SEXT_W((uint32_t)arch.GR64()[insn.GetRS1()] >> (arch.GR64()[insn.GetRS2()] & 0x001F));
    }

    // SRAW
    void RV64I_SRAW(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = SEXT_W((int32_t)arch.GR64()[insn.GetRS1()] >> (arch.GR64()[insn.GetRS2()] & 0x001F));
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



    inline void __RV64I_MEMORYIO_EXCEPTION(const RVInstruction& insn, RVArchitectural& arch, RVTrapCause cause, addr_t address)
    {
        // Writing into 'mtval'
        // *NOTICE: Action of writing address into 'mtval' is just an implementation-decided
        //          behaviour, not necessary according to privileged specification.
        //          If this action is not included in your processor or implementation, you
        //          could disable this action by "commenting" this part of code.
        RVCSR* mtval = arch.CSR()->GetCSR(CSR_mtval);

        if (mtval)
            mtval->SetValue(address);
        //

        Trap(arch, TRAP_EXCEPTION, cause);
    }

    inline void __RV64I_LOAD_EXCEPTION(const RVInstruction& insn, RVArchitectural& arch, RVMOPStatus status, addr_t address)
    {
        RVTrapCause cause;

        switch (status)
        {
            case MOP_ADDRESS_MISALIGNED:
                cause = EXCEPTION_LOAD_ADDRESS_MISALIGNED;
                break;

            case MOP_ACCESS_FAULT:
                cause = EXCEPTION_LOAD_ACCESS_FAULT;
                break;

            default:
                // SHOULD_NOT_REACH_HERE()
        }

        __RV64I_MEMORYIO_EXCEPTION(insn, arch, cause, address);
    }

    // LD
    void RV64I_LD(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr = arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data;

        RVMOPStatus status 
            = arch.MI()->Read(addr, MOPW_DOUBLE_WORD, &data);

        if (status == MOP_SUCCESS)
            arch.GR64()[insn.GetRD()] = data.data64;
        else
            __RV64I_LOAD_EXCEPTION(insn, arch, status, addr);
    }

    // LW
    void RV64I_LW(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr = arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data;

        RVMOPStatus status 
            = arch.MI()->Read(addr, MOPW_WORD, &data);

        if (status == MOP_SUCCESS)
            arch.GR64()[insn.GetRD()] = SEXT_W(data.data32);
        else
            __RV64I_LOAD_EXCEPTION(insn, arch, status, addr);
    }

    // LH
    void RV64I_LH(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr = arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data;

        RVMOPStatus status 
            = arch.MI()->Read(addr, MOPW_HALF_WORD, &data);

        if (status == MOP_SUCCESS)
            arch.GR64()[insn.GetRD()] = SEXT_H(data.data16);
        else
            __RV64I_LOAD_EXCEPTION(insn, arch, status, addr);
    }

    // LB
    void RV64I_LB(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr = arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data;

        RVMOPStatus status
            = arch.MI()->Read(addr, MOPW_BYTE, &data);

        if (status == MOP_SUCCESS)
            arch.GR64()[insn.GetRD()] = SEXT_B(data.data8);
        else
            __RV64I_LOAD_EXCEPTION(insn, arch, status, addr);
    }

    // LWU
    void RV64I_LWU(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr = arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data;

        RVMOPStatus status 
            = arch.MI()->Read(addr, MOPW_WORD, &data);

        if (status == MOP_SUCCESS)
            arch.GR64()[insn.GetRD()] = ZEXT_W(data.data32);
        else
            __RV64I_LOAD_EXCEPTION(insn, arch, status, addr);
    }

    // LHU
    void RV64I_LHU(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr = arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data;

        RVMOPStatus status 
            = arch.MI()->Read(addr, MOPW_HALF_WORD, &data);

        if (status == MOP_SUCCESS)
            arch.GR64()[insn.GetRD()] = ZEXT_H(data.data16);
        else
            __RV64I_LOAD_EXCEPTION(insn, arch, status, addr);
    }

    // LBU
    void RV64I_LBU(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr = arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data;

        RVMOPStatus status
            = arch.MI()->Read(addr, MOPW_BYTE, &data);

        if (status == MOP_SUCCESS)
            arch.GR64()[insn.GetRD()] = ZEXT_B(data.data8);
        else
            __RV64I_LOAD_EXCEPTION(insn, arch, status, addr);
    }


    inline void __RV64I_STORE_EXCEPTION(const RVInstruction& insn, RVArchitectural& arch, RVMOPStatus status, addr_t address)
    {
        RVTrapCause cause;

        switch (status)
        {
            case MOP_ADDRESS_MISALIGNED:
                cause = EXCEPTION_STORE_ADDRESS_MISALIGNED;
                break;

            case MOP_ACCESS_FAULT:
                cause = EXCEPTION_STORE_ACCESS_FAULT;
                break;

            default:
                // SHOULD_NOT_REACH_HERE()
        }

        __RV64I_MEMORYIO_EXCEPTION(insn, arch, cause, address);
    }
    
    // SD
    void RV64I_SD(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr =   arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data = { arch.GR64()[insn.GetRS2()] };

        RVMOPStatus status 
            = arch.MI()->Write(addr, MOPW_DOUBLE_WORD, data);

        if (status != MOP_SUCCESS)
            __RV64I_STORE_EXCEPTION(insn, arch, status, addr);
    }

    // SW
    void RV64I_SW(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr =   arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data = { arch.GR64()[insn.GetRS2()] };

        RVMOPStatus status 
            = arch.MI()->Write(addr, MOPW_WORD, data);

        if (status != MOP_SUCCESS)
            __RV64I_STORE_EXCEPTION(insn, arch, status, addr);
    }

    // SH
    void RV64I_SH(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr =   arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data = { arch.GR64()[insn.GetRS2()] };

        RVMOPStatus status
            = arch.MI()->Write(addr, MOPW_HALF_WORD, data);

        if (status != MOP_SUCCESS)
            __RV64I_STORE_EXCEPTION(insn, arch, status, addr);
    }

    // SB
    void RV64I_SB(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr =   arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data = { arch.GR64()[insn.GetRS2()] };

        RVMOPStatus status
            = arch.MI()->Write(addr, MOPW_BYTE, data);

        if (status != MOP_SUCCESS)
            __RV64I_STORE_EXCEPTION(insn, arch, status, addr);
    }
}


#define RV64I_INSNDEF(name, executor) \
    insn.SetName(name); \
    insn.SetExecutor(&executor);

#define RV64I_TYPE(T) \
    DecodeNormalRV64Type##T(insnraw, insn);

// codepoints
namespace Jasse {


    //
    bool RV64ICodePoint_Funct3_ADDI(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(I) RV64I_INSNDEF("addi", RV64I_ADDI);

        return true;
    }

    bool RV64ICodePoint_Funct3_SLTI(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(I) RV64I_INSNDEF("slti", RV64I_ADDI);

        return true;
    }

    bool RV64ICodePoint_Funct3_SLTIU(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(I) RV64I_INSNDEF("sltiu", RV64I_SLTIU);

        return true;
    }

    bool RV64ICodePoint_Funct3_ANDI(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(I) RV64I_INSNDEF("andi", RV64I_ANDI);

        return true;
    }

    bool RV64ICodePoint_Funct3_ORI(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(I) RV64I_INSNDEF("ori", RV64I_ORI);

        return true;
    }

    bool RV64ICodePoint_Funct3_XORI(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(I) RV64I_INSNDEF("xori", RV64I_XORI);

        return true;
    }

    bool RV64ICodePoint_Funct3_SLLI(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct6 = GET_STD_OPERAND(insnraw, RV64I_FUNCT6);

        if (funct6 == RV64I_FUNCT6_SLLI)
        {
            RV64I_TYPE(I) RV64I_INSNDEF("slli", RV64I_SLLI);

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
            RV64I_TYPE(I) RV64I_INSNDEF("srli", RV64I_SRLI);

            return true;
        }
        else if (funct6 == RV64I_FUNCT6_SRAI)
        {
            RV64I_TYPE(I) RV64I_INSNDEF("srai", RV64I_SRAI);

            return true;
        }
        else
            return false;
    }

    //
    bool RV64ICodePoint_Funct3_ADDIW(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(I) RV64I_INSNDEF("addiw", RV64I_ADDIW);

        return true;
    }

    bool RV64ICodePoint_Funct3_SLLIW(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_SLLIW)
        {
            RV64I_TYPE(I) RV64I_INSNDEF("slliw", RV64I_SLLIW);

            return true;
        }
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_SRLIW__SRAIW(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_SRLIW)
        {
            RV64I_TYPE(I) RV64I_INSNDEF("srliw", RV64I_SRLIW);

            return true;
        }
        else if (funct7 == RV64I_FUNCT7_SRAIW)
        {
            RV64I_TYPE(I) RV64I_INSNDEF("sraiw", RV64I_SRAIW);

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
            RV64I_TYPE(R) RV64I_INSNDEF("add", RV64I_ADD);

            return true;
        }
        else if (funct7 == RV32I_FUNCT7_SUB)
        {
            RV64I_TYPE(R) RV64I_INSNDEF("sub", RV64I_SUB);

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
            RV64I_TYPE(R) RV64I_INSNDEF("slt", RV64I_SLT);

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
            RV64I_TYPE(R) RV64I_INSNDEF("sltu", RV64I_SLTU);

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
            RV64I_TYPE(R) RV64I_INSNDEF("and", RV64I_AND);

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
            RV64I_TYPE(R) RV64I_INSNDEF("or", RV64I_OR);

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
            RV64I_TYPE(R) RV64I_INSNDEF("xor", RV64I_XOR);

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
            RV64I_TYPE(R) RV64I_INSNDEF("sll", RV64I_SLL);

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
            RV64I_TYPE(R) RV64I_INSNDEF("srl", RV64I_SRL);

            return true;
        }
        else if (funct7 == RV32I_FUNCT7_SRA)
        {
            RV64I_TYPE(R) RV64I_INSNDEF("sra", RV64I_SRA);

            return true;
        }
        else
            return false;
    }

    //
    bool RV64ICodePoint_Funct3_ADDW__SUBW(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_ADDW)
        {
            RV64I_TYPE(R) RV64I_INSNDEF("addw", RV64I_ADDW);

            return true;
        }
        else if (funct7 == RV64I_FUNCT7_SUBW)
        {
            RV64I_TYPE(R) RV64I_INSNDEF("subw", RV64I_SUBW);

            return true;
        }
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_SLLW(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_SLLW)
        {
            RV64I_TYPE(R) RV64I_INSNDEF("sllw", RV64I_SLLW);

            return true;
        }
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_SRLW__SRAW(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_SRLW)
        {
            RV64I_TYPE(R) RV64I_INSNDEF("srlw", RV64I_SRLW);

            return true;
        }
        else if (funct7 == RV64I_FUNCT7_SRAW)
        {
            RV64I_TYPE(R) RV64I_INSNDEF("sraw", RV64I_SRAW);

            return true;
        }
        else
            return false;
    }


    //
    bool RV64ICodePoint_Funct3_BEQ(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(B) RV64I_INSNDEF("beq", RV64I_BEQ);

        return true;
    }

    bool RV64ICodePoint_Funct3_BNE(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(B) RV64I_INSNDEF("bne", RV64I_BNE);

        return true;
    }

    bool RV64ICodePoint_Funct3_BLT(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(B) RV64I_INSNDEF("blt", RV64I_BLT);

        return true;
    }

    bool RV64ICodePoint_Funct3_BLTU(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(B) RV64I_INSNDEF("bltu", RV64I_BLTU);

        return true;
    }

    bool RV64ICodePoint_Funct3_BGE(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(B) RV64I_INSNDEF("bge", RV64I_BGE);

        return true;
    }

    bool RV64ICodePoint_Funct3_BGEU(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(B) RV64I_INSNDEF("bgeu", RV64I_BGEU);

        return true;
    }

    //
    bool RV64ICodePoint_Funct3_LD(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(I) RV64I_INSNDEF("ld", RV64I_LD);

        return true;
    }

    bool RV64ICodePoint_Funct3_LW(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(I) RV64I_INSNDEF("lw", RV64I_LW);

        return true;
    }

    bool RV64ICodePoint_Funct3_LH(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(I) RV64I_INSNDEF("lh", RV64I_LH);

        return true;
    }

    bool RV64ICodePoint_Funct3_LB(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(I) RV64I_INSNDEF("lb", RV64I_LB);

        return true;
    }

    bool RV64ICodePoint_Funct3_LWU(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(I) RV64I_INSNDEF("lwu", RV64I_LWU);

        return true;
    }

    bool RV64ICodePoint_Funct3_LHU(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(I) RV64I_INSNDEF("lhu", RV64I_LHU);

        return true;
    }

    bool RV64ICodePoint_Funct3_LBU(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(I) RV64I_INSNDEF("lbu", RV64I_LBU);

        return true;
    }

    //
    bool RV64ICodePoint_Funct3_SD(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(S) RV64I_INSNDEF("sd", RV64I_SD);

        return true;
    }

    bool RV64ICodePoint_Funct3_SW(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(S) RV64I_INSNDEF("sw", RV64I_SW);

        return true;
    }

    bool RV64ICodePoint_Funct3_SH(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(S) RV64I_INSNDEF("sh", RV64I_SH);

        return true;
    }

    bool RV64ICodePoint_Funct3_SB(insnraw_t insnraw, RVInstruction& insn)
    {
        RV64I_TYPE(S) RV64I_INSNDEF("sb", RV64I_SB);

        return true;
    }

    // TODO MISC-MEM SYSTEM
}

// codegroups
namespace Jasse {
    //
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(OP_IMM)
    {
        Define(RV32I_FUNCT3_ADDI,           &RV64ICodePoint_Funct3_ADDI);
        Define(RV32I_FUNCT3_SLTI,           &RV64ICodePoint_Funct3_SLTI);
        Define(RV32I_FUNCT3_SLTIU,          &RV64ICodePoint_Funct3_SLTIU);
        Define(RV32I_FUNCT3_ANDI,           &RV64ICodePoint_Funct3_ANDI);
        Define(RV32I_FUNCT3_ORI,            &RV64ICodePoint_Funct3_ORI);
        Define(RV32I_FUNCT3_XORI,           &RV64ICodePoint_Funct3_XORI);
        Define(RV32I_FUNCT3_SLLI,           &RV64ICodePoint_Funct3_SLLI);
        Define(RV32I_FUNCT3_SRLI__SRAI,     &RV64ICodePoint_Funct3_SRLI__SRAI);
    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(OP_IMM)
    { }

    //
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(OP_IMM_32)
    {
        Define(RV64I_FUNCT3_ADDIW,          &RV64ICodePoint_Funct3_ADDIW);
        Define(RV64I_FUNCT3_SLLIW,          &RV64ICodePoint_Funct3_SLLIW);
        Define(RV64I_FUNCT3_SRLIW__SRAIW,   &RV64ICodePoint_Funct3_SRLIW__SRAIW);
    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(OP_IMM_32)
    { }

    //
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(OP)
    {
        Define(RV32I_FUNCT3_ADD__SUB,       &RV64ICodePoint_Funct3_ADD__SUB);
        Define(RV32I_FUNCT3_SLT,            &RV64ICodePoint_Funct3_SLT);
        Define(RV32I_FUNCT3_SLTU,           &RV64ICodePoint_Funct3_SLTU);
        Define(RV32I_FUNCT3_AND,            &RV64ICodePoint_Funct3_AND);
        Define(RV32I_FUNCT3_OR,             &RV64ICodePoint_Funct3_OR);
        Define(RV32I_FUNCT3_XOR,            &RV64ICodePoint_Funct3_XOR);
        Define(RV32I_FUNCT3_SLL,            &RV64ICodePoint_Funct3_SLL);
        Define(RV32I_FUNCT3_SRL__SRA,       &RV64ICodePoint_Funct3_SRL__SRA);
    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(OP)
    { }

    //
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(OP_32)
    {
        Define(RV64I_FUNCT3_ADDW__SUBW,     &RV64ICodePoint_Funct3_ADDW__SUBW);
        Define(RV64I_FUNCT3_SLLW,           &RV64ICodePoint_Funct3_SLLW);
        Define(RV64I_FUNCT3_SRLW__SRAW,     &RV64ICodePoint_Funct3_SRLW__SRAW);
    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(OP_32)
    { }

    // 
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(BRANCH)
    {
        Define(RV32I_FUNCT3_BEQ,            &RV64ICodePoint_Funct3_BEQ);
        Define(RV32I_FUNCT3_BNE,            &RV64ICodePoint_Funct3_BNE);
        Define(RV32I_FUNCT3_BLT,            &RV64ICodePoint_Funct3_BLT);
        Define(RV32I_FUNCT3_BLTU,           &RV64ICodePoint_Funct3_BLTU);
        Define(RV32I_FUNCT3_BGE,            &RV64ICodePoint_Funct3_BGE);
        Define(RV32I_FUNCT3_BGEU,           &RV64ICodePoint_Funct3_BGEU);
    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(BRANCH)
    { }

    //
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(LOAD)
    {
        Define(RV64I_FUNCT3_LD,             &RV64ICodePoint_Funct3_LD);
        Define(RV32I_FUNCT3_LW,             &RV64ICodePoint_Funct3_LW);
        Define(RV32I_FUNCT3_LH,             &RV64ICodePoint_Funct3_LH);
        Define(RV32I_FUNCT3_LB,             &RV64ICodePoint_Funct3_LB);
        Define(RV64I_FUNCT3_LWU,            &RV64ICodePoint_Funct3_LWU);
        Define(RV32I_FUNCT3_LHU,            &RV64ICodePoint_Funct3_LHU);
        Define(RV32I_FUNCT3_LBU,            &RV64ICodePoint_Funct3_LBU);
    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(LOAD)
    { }

    // 
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(STORE)
    {
        Define(RV64I_FUNCT3_SD,             &RV64ICodePoint_Funct3_SD);
        Define(RV32I_FUNCT3_SW,             &RV64ICodePoint_Funct3_SW);
        Define(RV32I_FUNCT3_SH,             &RV64ICodePoint_Funct3_SH);
        Define(RV32I_FUNCT3_SB,             &RV64ICodePoint_Funct3_SB);
    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(STORE)
    { }

    //
    __RV64I_UNIQUE_CODEGROUP_DEFINE_FUNC(LUI, RV64ICodeGroup_Unique_LUI)
    {
        RV64I_TYPE(U) RV64I_INSNDEF("lui", RV64I_LUI);

        return true;
    }

    //
    __RV64I_UNIQUE_CODEGROUP_DEFINE_FUNC(AUIPC, RV64ICodeGroup_Unique_AUIPC)
    {
        RV64I_TYPE(U) RV64I_INSNDEF("auipc", RV64I_AUIPC);

        return true;
    }

    //
    __RV64I_UNIQUE_CODEGROUP_DEFINE_FUNC(JAL, RV64ICodeGroup_Unique_JAL)
    {
        RV64I_TYPE(J) RV64I_INSNDEF("jal", RV64I_JAL);

        return true;
    }

    //
    __RV64I_UNIQUE_CODEGROUP_DEFINE_FUNC(JALR, RV64ICodeGroup_Unique_JALR)
    {
        if (GET_STD_OPERAND(insnraw, RV32I_FUNCT3) == RV32I_FUNCT3_JALR)
        {
            RV64I_TYPE(I) RV64I_INSNDEF("jalr", RV64I_JALR);

            return true;
        }
        else
            return false;
    }
}




