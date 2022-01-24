#pragma once

#include <list>

#include "core_global.hpp"


namespace VMC::RAT {

    static constexpr int INSN_NOP   = 0;

    class SimInstruction
    {
    private:
        int     clkDelay;
        int     dstARF;
        int     srcARF1;
        int     srcARF2;
        int     insncode;

    public:
        SimInstruction();
        SimInstruction(int clkDelay, int dstARF);
        SimInstruction(int clkDelay, int dstARF, int srcARF1, int srcARF2);
        SimInstruction(int clkDelay, int dstARF, int srcARF1, int srcARF2, int insncode);
        SimInstruction(const SimInstruction& obj);
        ~SimInstruction();

        int     GetRemainingClkDelay() const;
        int     GetDstARF() const;
        int     GetSrcARF1() const;
        int     GetSrcARF2() const;

        void    SetInsnCode(int insncode);
        int     GetInsnCode() const;

        bool    Immediate() const;

        void    Eval();
    };

    class SimReOrderBuffer
    {
    private:
        list<SimInstruction>    insnBuffer;
        SimInstruction          insnReady;
        bool                    insnReadyFlag;
        SimInstruction          insnPush;
        bool                    insnPushFlag;
        bool                    insnPopFlag;

    public:
        SimReOrderBuffer();
        ~SimReOrderBuffer();

        bool            IsNextReady() const;
        SimInstruction  GetNextReady() const;

        int             GetInsnCount() const;
        void            Clear();

        void            PushInsn(SimInstruction insn);
        void            PopInsn();

        void            ResetInput();

        void            Eval();
    };

    class SimHandle 
    {

    };
}


// Global components
namespace VMC::RAT {

    
};


// class VMC::RAT::SimInstruction
namespace VMC::RAT {
    /*
    int     dstARF;
    int     clkDelay;
    */

    SimInstruction::SimInstruction()
        : clkDelay  (0)
        , dstARF    (0)
        , srcARF1   (0)
        , srcARF2   (0)
        , insncode  (INSN_NOP)
    { }

    SimInstruction::SimInstruction(int clkDelay, int dstARF)
        : clkDelay  (clkDelay)
        , dstARF    (dstARF)
        , srcARF1   (0)
        , srcARF2   (0)
        , insncode  (INSN_NOP)
    { }

    SimInstruction::SimInstruction(int clkDelay, int dstARF, int srcARF1, int srcARF2)
        : clkDelay  (clkDelay)
        , dstARF    (dstARF)
        , srcARF1   (srcARF1)
        , srcARF2   (srcARF2)
        , insncode  (INSN_NOP)
    { }

    SimInstruction::SimInstruction(int clkDelay, int dstARF, int srcARF1, int srcARF2, int insncode)
        : clkDelay  (clkDelay)
        , dstARF    (dstARF)
        , srcARF1   (srcARF1)
        , srcARF2   (srcARF2)
        , insncode  (INSN_NOP)
    { }

    SimInstruction::SimInstruction(const SimInstruction& obj)
        : clkDelay  (obj.clkDelay)
        , dstARF    (obj.dstARF)
        , srcARF1   (obj.srcARF1)
        , srcARF2   (obj.srcARF2)
        , insncode  (obj.insncode)
    { }

    SimInstruction::~SimInstruction()
    { }

    inline int SimInstruction::GetRemainingClkDelay() const
    {
        return clkDelay;
    }

    inline int SimInstruction::GetDstARF() const
    {
        return dstARF;
    }

    inline int SimInstruction::GetSrcARF1() const
    {
        return srcARF1;
    }

    inline int SimInstruction::GetSrcARF2() const
    {
        return srcARF2;
    }

    inline void SimInstruction::SetInsnCode(int insncode)
    {
        this->insncode = insncode;
    }

    inline int SimInstruction::GetInsnCode() const
    {
        return insncode;
    }

    inline bool SimInstruction::Immediate() const
    {
        return clkDelay == 0;
    }

    void SimInstruction::Eval()
    {
        if (clkDelay)
            clkDelay--;
    }
}


// class VMC::RAT::SimReOrderBuffer
namespace VMC::RAT {
    /*
    list<SimInstruction>    insnBuffer;
    SimInstruction          insnReady;
    bool                    insnReadyFlag;
    SimInstruction          insnPush;
    bool                    insnPushFlag;
    bool                    insnPopFlag;
    */

    SimReOrderBuffer::SimReOrderBuffer()
        : insnBuffer    (list<SimInstruction>())
        , insnReady     (SimInstruction())
        , insnReadyFlag (false)
        , insnPush      (SimInstruction())
        , insnPushFlag  (false)
        , insnPopFlag   (false)
    { }

    SimReOrderBuffer::~SimReOrderBuffer()
    { }

    inline bool SimReOrderBuffer::IsNextReady() const
    {
        return insnReadyFlag;
    }

    inline SimInstruction SimReOrderBuffer::GetNextReady() const
    {
        return insnReady;
    }

    inline int SimReOrderBuffer::GetInsnCount() const
    {
        return insnBuffer.size();
    }

    inline void SimReOrderBuffer::Clear()
    {
        insnBuffer.clear();
    }

    void SimReOrderBuffer::PushInsn(SimInstruction insn)
    {
        insnPush     = insn;
        insnPushFlag = true;
    }

    void SimReOrderBuffer::PopInsn()
    {
        insnPopFlag = true;
    }

    void SimReOrderBuffer::ResetInput()
    {
        insnPushFlag = false;
        insnPopFlag  = false;
    }

    void SimReOrderBuffer::Eval()
    {
        // TODO
        
    }
}
