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
#define __RV64I_UNIQUE_CODEPOINT(name) \
    RV64IUniqueCodepoint_##name

#define __RV64I_UNIQUE_CODEPOINT_DEFINE(name, function) \
    __RV64I_UNIQUE_CODEPOINT(name)::__RV64I_UNIQUE_CODEPOINT(name)() \
        : codepoint (&function) \
    { } \

#define __RV64I_UNIQUE_CODEPOINT_DECLARE(name) \
    class __RV64I_UNIQUE_CODEPOINT(name) : public RV64IUniqueCodepoint { \
    public: \
        __RV64I_UNIQUE_CODEPOINT(name)(); \
    }; \


#define __RV64I_FUNCT3_CODEPOINT(name) \
    RV64IFunct3Codepoint_##name

#define __RV64I_FUNCT3_CODEPOINT_CONSTRUCTOR(name) \
    __RV64I_FUNCT3_CODEPOINT(name)::__RV64I_FUNCT3_CODEPOINT(name)()

#define __RV64I_FUNCT3_CODEPOINT_DECONSTRUCTOR(name) \
    __RV64I_FUNCT3_CODEPOINT(name)::~__RV64I_FUNCT3_CODEPOINT(name)()


#define __RV64I_FUNCT3_CODEPOINT_DECLARE(name) \
    class __RV64I_FUNCT3_CODEPOINT(name) : public RV64IFunct3Codepoint { \
    public: \
        __RV64I_FUNCT3_CODEPOINT(name)(); \
        ~__RV64I_FUNCT3_CODEPOINT(name)(); \
    }; \

//
namespace Jasse {

    // RV64I low-level codepoint
    typedef bool (*RV64ILLCodepoint)(insnraw_t insnraw, RVInstruction& insn); // TODO !! wrong declaration

    // RV64I middle-level codepoint
    class RV64IMLCodepoint {
    public:
        virtual bool    Decode(insnraw_t insnraw, RVInstruction& insn) const = 0;
    };


    // RV64I middle-level unique codepoint
    class RV64IUniqueCodepoint : public RV64IMLCodepoint {
    private:
        RV64ILLCodepoint    codepoint;
    
    public:
        RV64IUniqueCodepoint(RV64ILLCodepoint codepoint);
        ~RV64IUniqueCodepoint();

        virtual bool    Decode(insnraw_t insnraw, RVInstruction& insn) const override;
    };

    // RV64I middle-level FUNCT3 codepoint
    class RV64IFunct3Codepoint : public RV64IMLCodepoint {
    private:
        RV64ILLCodepoint*   codepoints; // 8 (2 ^ 3)

    public:
        RV64IFunct3Codepoint();
        ~RV64IFunct3Codepoint();

        void            Define(int funct3, RV64ILLCodepoint codepoint);
        virtual bool    Decode(insnraw_t insnraw, RVInstruction& insn) const override;
    };


    // RV64I Decoder
    class RV64IDecoder : public RVDecoder {
    private:
        RV64IFunct3Codepoint**  codepoints; // 32 (2 ^ (7 - 2))

    public:
        RV64IDecoder();
        ~RV64IDecoder();

        virtual bool    Decode(insnraw_t insnraw, RVInstruction& insn) const override;
    };


    // Codepoint declarations
    __RV64I_FUNCT3_CODEPOINT_DECLARE(OP)
    __RV64I_FUNCT3_CODEPOINT_DECLARE(OP_32)
    __RV64I_FUNCT3_CODEPOINT_DECLARE(OP_IMM)
    __RV64I_FUNCT3_CODEPOINT_DECLARE(OP_IMM_32)

    __RV64I_FUNCT3_CODEPOINT_DECLARE(BRANCH)
    __RV64I_FUNCT3_CODEPOINT_DECLARE(LOAD)
    __RV64I_FUNCT3_CODEPOINT_DECLARE(STORE)
    __RV64I_FUNCT3_CODEPOINT_DECLARE(MISC_MEM)
    __RV64I_FUNCT3_CODEPOINT_DECLARE(SYSTEM)

    __RV64I_UNIQUE_CODEPOINT_DECLARE(LUI)
    __RV64I_UNIQUE_CODEPOINT_DECLARE(AUIPC)
    __RV64I_UNIQUE_CODEPOINT_DECLARE(JAL)
    __RV64I_UNIQUE_CODEPOINT_DECLARE(JALR)
};


// Implementation of: class RV64IUniqueCodepoint
namespace Jasse {
    RV64IUniqueCodepoint::RV64IUniqueCodepoint(RV64ILLCodepoint codepoint)
        : codepoint (codepoint)
    { }

    RV64IUniqueCodepoint::~RV64IUniqueCodepoint()
    { }

    bool RV64IUniqueCodepoint::Decode(insnraw_t insnraw, RVInstruction& insn) const
    {
        return (*codepoint)(insnraw, insn);
    }
}


// Implementation of: class RV64IFunct3Codepoint
namespace Jasse {
    RV64IFunct3Codepoint::RV64IFunct3Codepoint()
        : codepoints(new RV64ILLCodepoint[8]())
    { }

    RV64IFunct3Codepoint::~RV64IFunct3Codepoint()
    { 
        delete[] codepoints;
    }

    bool RV64IFunct3Codepoint::Decode(insnraw_t insnraw, RVInstruction& insn) const
    {
        int funct3 = GET_OPERAND(insnraw, RV64I_FUNCT3_MASK, RV64I_FUNCT3_OFFSET);

        if (codepoints[funct3])
            return (*(codepoints[funct3]))(insnraw, insn);
        else
            return false;
    }

    inline void RV64IFunct3Codepoint::Define(int funct3, RV64ILLCodepoint codepoint)
    {
        codepoints[funct3] = codepoint;
    }
}



// codepoints
namespace Jasse {

}


