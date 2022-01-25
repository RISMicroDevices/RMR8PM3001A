#pragma once

#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <vector>

#include "vmc.hpp"

#include "core_issue.hpp"


namespace VMC::RAT {

    static constexpr int INSN_CODE_NOP      = 0;

    static constexpr int INSN_CODE_AND      = 1;

    static constexpr int INSN_CODE_OR       = 2;

    static constexpr int INSN_CODE_XOR      = 3;

    static constexpr int INSN_CODE_ADD      = 4;

    static constexpr int INSN_CODE_SUB      = 5;

    static constexpr int INSN_COUNT         = 6;

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
        bool            IsEmpty() const;
        void            Clear();

        void            PushInsn(SimInstruction insn);
        void            PopInsn();

        void            ResetInput();

        void            Eval();
    };

    typedef struct {

        SimReOrderBuffer    ROB                         = SimReOrderBuffer();

        int                 PRF[EMULATED_PRF_SIZE]      = { 0 };

        int                 RefARF[EMULATED_ARF_SIZE]   = { 0 };

        bool                FlagStepInfo                = false;

        bool                FlagARF0Conv                = true;

    } SimContext, *SimHandle;
}


// Global components
namespace VMC::RAT {

    static SimHandle CURRENT_HANDLE = nullptr;

    void SetupCommands(VMCHandle handle);

    inline SimHandle NewHandle()
    {
        return new SimContext;
    }

    inline void DeleteHandle(SimHandle handle)
    {
        delete handle;
    }

    void Setup(VMCHandle handle)
    {
        CURRENT_HANDLE = NewHandle();
        
        SetupCommands(handle);
    }
    
    inline void SetCurrentHandle(SimHandle handle)
    {
        CURRENT_HANDLE = handle;
    }

    inline SimHandle GetCurrentHandle()
    {
        return CURRENT_HANDLE;
    }

    //
}


// VMC RAT commands
namespace VMC::RAT {

#define ECHO_COUT_VMC_RAT_HELP \
    std::cout << "RAT0 simulation command usages:" << std::endl; \
    std::cout << "- rat0.info.bystep [true|false]     Toggle by-step info of RAT0 simulation (\'false\' by default)" << std::endl; \
    std::cout << "- rat0.arf0conv [true|false]        Toggle ARF0-zero conversion (\'true\' by default)" << std::endl; \
    std::cout << "- rat0.rat.ls [-V|+V|-NRA|+NRA|-FV|+FV] " << std::endl; \
    std::cout << "                                    List all RAT entries (with optional filter)" << std::endl; \
    std::cout << "- rat0.arf.ls.ref [-Z]              List all reference ARF register values (with optional filter)" << std::endl; \
    std::cout << "- rat0.arf.ls [-Z|-U]               List all values of ARF register mapped by RAT (with optional filter)" << std::endl; \
    std::cout << "- rat0.arf.set <index> <value>      Set specified ARF register value" << std::endl; \
    std::cout << "- rat0.arf.set.random [index]       Set specified/random ARF register with random value" << std::endl; \
    std::cout << "- rat0.arf.get <index>              Get specified ARF register value" << std::endl;

    // rat0.info.bystep [true|false]
    bool _RAT0_INFO_BYSTEP(void* handle, const std::string& cmd,
                                         const std::string& paramline,
                                         const std::vector<std::string>& params)
    {
        if (params.size() > 1)
        {
            std::cout << "Too much or too less parameter(s) for \'rat0.info.bystep\'." << std::endl;
            return false;
        }

        if (!params.empty())
        {
            std::istringstream(params[0]) >> std::boolalpha >> GetCurrentHandle()->FlagStepInfo;
            std::cout << "Set: ";
        }

        std::cout << "RAT0.flag.stepinfo = ";
        std::cout << GetCurrentHandle()->FlagStepInfo << std::endl;

        return true;
    }

    // rat0.arf0conv [true|false]
    bool _RAT0_ARF0CONV(void* handle, const std::string& cmd,
                                      const std::string& paramline,
                                      const std::vector<std::string>& params)
    {
        if (params.size() > 1)
        {
            std::cout << "Too much or too less parameter(s) for \'rat0.arf0conv\'." << std::endl;
            return false;
        }

        if (!params.empty())
        {
            std::istringstream(params[0]) >> std::boolalpha >> GetCurrentHandle()->FlagARF0Conv;
            std::cout << "Set: ";
        }

        std::cout << "RAT0.flag.arf0conv = ";
        std::cout << GetCurrentHandle()->FlagARF0Conv << std::endl;

        return true;
    }

    // rat0.arf.ls.ref [-Z]
    bool _RAT0_ARF_LS_REF(void* handle, const std::string& cmd,
                                        const std::string& paramline,
                                        const std::vector<std::string>& params)
    {
        //
        bool nonzero = false;

        if (params.size() == 1)
        {
            if (params[0].compare("-Z") == 0)
            {
                nonzero = true;
            }
            else
            {
                std::cout << "Invalid parameter: \'" << params[0] << "\'" << std::endl;
                return false;
            }
        }
        else if (!params.empty())
        {
            std::cout << "Too much or too less parameter(s) for \'rat0.arf.ls.ref\'." << std::endl;
            return false;
        }

        //
        std::cout << "Architectural registers:" << std::endl;

        if (nonzero)
            std::cout << "(\'-Z\': Listing all non-zero registers)" << std::endl;

        std::cout << "Type         Index         Value" << std::endl;
        std::cout << "-----        ------        -----------" << std::endl;

        for (int i = 0; i < EMULATED_ARF_SIZE; i++)
        {
            if (nonzero)
                if (!GetCurrentHandle()->RefARF[i])
                    continue;

            std::cout << "rARF         ";
            printf("%3d", i);
            std::cout << "           ";
            printf("0x%08x\n", GetCurrentHandle()->RefARF[i]);
        }

        return true;
    }

    
    // rat0.arf.ls [-Z|-U]
    bool _RAT0_ARF_LS(void* handle, const std::string& cmd,
                                    const std::string& paramline,
                                    const std::vector<std::string>& params)
    {
        // TODO

        return true;
    }


    // rat0.arf.set <index> <value>

    
    // rat0.arf.set.random <index>

    
    // rat0.arf.get <index>


    // rat0.rat.ls [-V|+V|-NRA|+NRA|-FV|+FV]
    bool _RAT0_RAT_LS(void* handle, const std::string& cmd,
                                    const std::string& paramline,
                                    const std::vector<std::string>& params)
    {
        // TODO

        return true;
    }

    // rat0.diffsim.insn <insncode> [delay] [dstARF] [srcARF1] [srcARF2]


    // rat0.diffsim.insn.random <count>


    void SetupCommands(VMCHandle handle)
    {
        handle->handlers.push_back(CommandHandler{ std::string("rat0.info.bystep"), &_RAT0_INFO_BYSTEP });
        handle->handlers.push_back(CommandHandler{ std::string("rat0.arf0conv")   , &_RAT0_ARF0CONV });
        handle->handlers.push_back(CommandHandler{ std::string("rat0.rat.ls")     , &_RAT0_RAT_LS });
        handle->handlers.push_back(CommandHandler{ std::string("rat0.arf.ls.ref") , &_RAT0_ARF_LS_REF });
    }
}


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
        , insncode  (INSN_CODE_NOP)
    { }

    SimInstruction::SimInstruction(int clkDelay, int dstARF)
        : clkDelay  (clkDelay)
        , dstARF    (dstARF)
        , srcARF1   (0)
        , srcARF2   (0)
        , insncode  (INSN_CODE_NOP)
    { }

    SimInstruction::SimInstruction(int clkDelay, int dstARF, int srcARF1, int srcARF2)
        : clkDelay  (clkDelay)
        , dstARF    (dstARF)
        , srcARF1   (srcARF1)
        , srcARF2   (srcARF2)
        , insncode  (INSN_CODE_NOP)
    { }

    SimInstruction::SimInstruction(int clkDelay, int dstARF, int srcARF1, int srcARF2, int insncode)
        : clkDelay  (clkDelay)
        , dstARF    (dstARF)
        , srcARF1   (srcARF1)
        , srcARF2   (srcARF2)
        , insncode  (INSN_CODE_NOP)
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

    inline bool SimReOrderBuffer::IsEmpty() const
    {
        return !GetInsnCount();
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
        // eval all
        list<SimInstruction>::iterator iter = insnBuffer.begin();
        while (iter != insnBuffer.end())
            (*iter++).Eval();

        // push insn
        if (insnPushFlag)
        {
            insnBuffer.push_back(insnPush);
            insnPushFlag = false;
        }

        // pop insn
        if (insnPopFlag && !insnBuffer.empty())
        {
            insnBuffer.pop_front();
            insnPopFlag = false;
        }

        // check next ready insn
        insnReadyFlag = false;

        iter = insnBuffer.begin();
        while (iter != insnBuffer.end())
        {
            if ((*iter).Immediate())
            {
                insnReady     = *iter;
                insnReadyFlag = true;

                break;
            }
            else
                iter++;
        }
    }
}
