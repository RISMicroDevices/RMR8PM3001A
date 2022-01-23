#pragma once

#include "core_global.hpp"


namespace VMC::RAT {

    class SimInstruction
    {
    private:
        int     dstARF;
        int     clkDelay;

    public:
        SimInstruction(int dstARF, int clkDelay);
        ~SimInstruction();

        int     GetRemainingClkDelay() const;
        int     GetDstARF() const;

        bool    Immediate() const;

        void    Eval();
    };

    class SimReOrderBuffer
    {

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

    SimInstruction::SimInstruction(int dstARF, int clkDelay)
        : dstARF    (dstARF)
        , clkDelay  (clkDelay)
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

