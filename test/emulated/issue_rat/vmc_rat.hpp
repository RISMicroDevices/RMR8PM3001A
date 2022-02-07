#pragma once

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <vector>

#include "vmc.hpp"

#include "core_issue.hpp"


using namespace MEMU::Core::Issue;

namespace VMC::RAT {

    static constexpr int INSN_CODE_NOP      = 0;

    static constexpr int INSN_CODE_AND      = 1;

    static constexpr int INSN_CODE_OR       = 2;

    static constexpr int INSN_CODE_XOR      = 3;

    static constexpr int INSN_CODE_ADD      = 4;

    static constexpr int INSN_CODE_SUB      = 5;

    static constexpr int INSN_COUNT         = 6;

    class SimRefARF
    {
    private:
        uint64_t* const refARF;

    public:
        SimRefARF(uint64_t* refARF);
        SimRefARF(const SimRefARF& obj);

        uint64_t    operator[](const int index) const;
    };

    class SimMappedARF
    {
    private:
        RegisterAliasTable*     const RAT;
        PhysicalRegisterFile*   const PRF;

    public:
        SimMappedARF(RegisterAliasTable* RAT, PhysicalRegisterFile* PRF);
        SimMappedARF(const SimMappedARF& obj);

        uint64_t    operator[](const int index) const;
    };

    class SimInstruction
    {
    private:
        int     FID;
        int     delay;
        int     dst;
        int     src1;
        int     src2;
        int     insncode;

    public:
        SimInstruction();
        SimInstruction(int FID, int clkDelay, int dst);
        SimInstruction(int FID, int clkDelay, int dst, int src1, int src2);
        SimInstruction(int FID, int clkDelay, int dst, int src1, int src2, int insncode);
        SimInstruction(const SimInstruction& obj);
        ~SimInstruction();

        int     GetFID() const;
        int     GetDelay() const;
        int     GetDst() const;
        int     GetSrc1() const;
        int     GetSrc2() const;
        int     GetInsnCode() const;

        void    SetDst(int dst);
        void    SetSrc1(int src1);
        void    SetSrc2(int src2);
        void    SetInsnCode(int insncode);
    };

    class SimScoreboard // Only support ARF0-conv mode
    {
    private:
        const int   size;

        bool* const busy;
        int*  const FID;

    public:
        SimScoreboard(int size);
        SimScoreboard(const SimScoreboard& obj);
        ~SimScoreboard();

        void    Clear();

        int     GetFID(int index) const;
        bool    IsBusy(int index) const;
        void    SetBusy(int index, int FID);
        void    Release(int FID);
    };

    class SimReservation // Only support ARF0-conv mode
    {
    public:
        class Entry {
        private:
            SimInstruction  insn;
            bool            src1rdy;
            bool            src2rdy;

        public:
            Entry(const SimInstruction& insn);
            Entry(const SimInstruction& insn, bool src1rdy, bool src2rdy);
            Entry(const Entry& obj);
            ~Entry();

            const SimInstruction&   GetInsn() const;
            bool                    IsSrc1Ready() const;
            bool                    IsSrc2Ready() const;
            bool                    IsReady() const;
            int                     GetSrc1() const;
            int                     GetSrc2() const;

            void                    SetSrc1Ready(bool rdy = true);
            void                    SetSrc2Ready(bool rdy = true);
        };

    private:
        const SimScoreboard*    const scoreboard;
        std::list<Entry>              entries;
        std::list<Entry>::iterator    next;

    public:
        SimReservation(const SimScoreboard* scoreboard);
        SimReservation(const SimReservation& obj);
        ~SimReservation();

        const SimScoreboard*    GetScoreboardRef() const;

        void                    PushInsn(const SimInstruction& insn);

        bool                    NextInsn(SimInstruction* insn = nullptr);
        bool                    PopInsn();

        void                    Clear();

        void                    Eval();
    };

    class SimExecution
    {
    public:
        class Entry {
        private:
            SimInstruction  insn;
            uint64_t        src1val;
            uint64_t        src2val;
            int             delay;
            bool            dstrdy;
            uint64_t        dstval;

        public:
            Entry(const SimInstruction& insn);
            Entry(const SimInstruction& insn, uint64_t src1val, uint64_t src2val);
            Entry(const Entry& entry);
            ~Entry();

            const SimInstruction&   GetInsn() const;
            uint64_t                GetSrc1Value() const;
            uint64_t                GetSrc2Value() const;
            bool                    IsReady() const;
            bool                    IsDstReady() const;
            uint64_t                GetDstValue() const;

            void                    SetSrc1Value(uint64_t value);
            void                    SetSrc2Value(uint64_t value);
            void                    SetDstReady(bool ready = true);
            void                    SetDstValue(uint64_t value);

            void                    Eval();
        };

    private:
        bool                  const ref;
        SimRefARF*            const refARF;
        SimMappedARF*         const mappedARF;
        std::list<Entry>            entries;
        std::list<Entry>::iterator  next;

    public:
        SimExecution(uint64_t* ARF);
        SimExecution(RegisterAliasTable* RAT, PhysicalRegisterFile* PRF);
        SimExecution(const SimExecution& obj);
        ~SimExecution();

        void    PushInsn(const SimInstruction& insn, Entry* entry = nullptr);

        bool    NextInsn(Entry* entry = nullptr);
        bool    PopInsn();

        void    Eval();
    };

    class SimReOrderBuffer
    {
    public:
        class Entry {
        private:
            SimInstruction  insn;
            bool            rdy;
            uint64_t        dstval;

        public:
            Entry(const SimInstruction& insn);
            Entry(const Entry& obj);
            ~Entry();

            const SimInstruction&   GetInsn() const;
            int                     GetFID() const;
            bool                    IsReady() const;
            uint64_t                GetDstValue() const;

            void                    SetReady(bool ready = true);
            void                    SetDstValue(uint64_t value);
        };
    
    private:
        std::list<Entry>            entries;
        std::list<Entry>::iterator  next;

    public:
        SimReOrderBuffer();
        SimReOrderBuffer(const SimReOrderBuffer& obj);
        ~SimReOrderBuffer();

        void    TouchInsn(const SimInstruction& insn);
        bool    WritebackInsn(const SimInstruction& insn, uint64_t value);

        bool    NextInsn(Entry* entry = nullptr);
        bool    PopInsn();
    };

    //
    static constexpr uint64_t       RAND_MAX_REG_VALUE_MAX  = UINT64_MAX;

    static constexpr unsigned int   RAND_MAX_REG_INDEX_MAX  = EMULATED_ARF_SIZE;

    //
    static constexpr bool           SIM_DEFAULT_FLAG_STEPINFO       = false;

    static constexpr bool           SIM_DEFAULT_FLAG_ARF0CONV       = true;

    static constexpr uint64_t       SIM_DEFAULT_RAND_MAX_REG_VALUE  = 0;

    static constexpr int            SIM_DEFAULT_RAND_MAX_REG_INDEX  = RAND_MAX_REG_INDEX_MAX;

    typedef struct {

        RegisterAliasTable      RAT                         = RegisterAliasTable();

        PhysicalRegisterFile    PRF                         = PhysicalRegisterFile();

        SimReOrderBuffer        ROB                         = SimReOrderBuffer();

        uint64_t                RefARF[EMULATED_ARF_SIZE]   = { 0 };

        int                     GlobalFID                   = 0;

        bool                    FlagStepInfo                = SIM_DEFAULT_FLAG_STEPINFO;

        bool                    FlagARF0Conv                = SIM_DEFAULT_FLAG_ARF0CONV;

        uint64_t                RandMaxRegValue             = SIM_DEFAULT_RAND_MAX_REG_VALUE;

        unsigned int            RandMaxRegIndex             = SIM_DEFAULT_RAND_MAX_REG_INDEX;

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

    uint64_t RandRegValue(SimHandle handle)
    {
        uint64_t val = rand() 
                     ^ ((uint64_t)rand() << 16) 
                     ^ ((uint64_t)rand() << 32) 
                     ^ ((uint64_t)rand() << 48);

        uint64_t orb = rand();
        uint64_t orc =  (orb & 0x0000000F)
                     | ((orb & 0x000000F0) << 12)
                     | ((orb & 0x00000F00) << 24)
                     | ((orb & 0x00007000) << 36);
        
        if (handle->RandMaxRegValue)
            return (val ^ orc) % handle->RandMaxRegValue;
        else
            return val ^ orc;
    }

    int RandRegIndex(SimHandle handle)
    {
        return rand() % handle->RandMaxRegIndex;
    }

    inline uint64_t GetRefARF(SimHandle handle, int arf)
    {
        if (handle->FlagARF0Conv && !arf)
            return 0;
        
        return handle->RefARF[arf];
    }

    inline void SetRefARF(SimHandle handle, int arf, uint64_t val)
    {
        if (handle->FlagARF0Conv && !arf)
            return;

        handle->RefARF[arf] = val;
    }

    inline uint64_t GetMappedARF(SimHandle handle, int arf, bool* out_mapped = 0, int* out_mappedPRF = 0)
    {
        if (handle->FlagARF0Conv && !arf)
            return 0;

        int  mappedPRF = handle->RAT.GetAliasPRF(arf);
        bool mapped    = mappedPRF >= 0;

        if (out_mapped)
            *out_mapped = mapped;

        if (out_mappedPRF)
            *out_mappedPRF = mappedPRF;

        return mapped ? handle->PRF.Get(mappedPRF) : 0;
    }

    inline bool SetMappedARF(SimHandle handle, int arf, uint64_t val, int* out_prf = 0)
    {
        if (handle->FlagARF0Conv && !arf)
            return true;

        int prf;
        if (!handle->RAT.TouchAndCommit(handle->GlobalFID++, arf, &prf))
            return false;

        handle->PRF.Set(prf, val);

        if (out_prf)
            *out_prf = prf;

        return true;
    }

    inline bool SetMappedARFAndEval(SimHandle handle, int arf, uint64_t val, int* out_prf = 0)
    {
        if (!SetMappedARF(handle, arf, val, out_prf))
            return false;

        handle->RAT.Eval();
        handle->PRF.Eval();

        return true;
    }

    //
}


// VMC RAT commands
namespace VMC::RAT {

#define ECHO_COUT_VMC_RAT_HELP \
    std::cout << "RAT0 simulation command usages:" << std::endl; \
    std::cout << "- rat0.infobystep [true|false]      Toggle by-step info of RAT0 simulation (\'false\' by default)" << std::endl; \
    std::cout << "- rat0.arf0conv [true|false]        Toggle ARF0-zero conversion (\'true\' by default)" << std::endl; \
    std::cout << "- rat0.rand.reg.value [(uint)max_value|@DEFAULT]" << std::endl; \
    std::cout << "                                    Get/set the maximum value of random register value" << std::endl; \
    std::cout << "- rat0.rand.reg.index [(uint)max_value|@DEFAULT]" << std::endl; \
    std::cout << "                                    Get/set the maximum value of random register index" << std::endl; \
    std::cout << "- rat0.prf.ls [-V|+V|-NRA|+NRA|-FV|+FV|-Z|+Z] " << std::endl; \
    std::cout << "                                    List all RAT entries and related PRF (with optional filter)" << std::endl; \
    std::cout << "- rat0.arf.ls.ref [-Z]              List all reference ARF register values (with optional filter)" << std::endl; \
    std::cout << "- rat0.arf.ls [-Z|-U]               List all values of ARF register mapped by RAT (with optional filter)" << std::endl; \
    std::cout << "- rat0.arf.set <index> <value> [-S|-NEQ] " << std::endl; \
    std::cout << "                                    Set specified ARF register value" << std::endl; \
    std::cout << "- rat0.arf.set.randomval <index> [-S|-NEQ]" << std::endl; \
    std::cout << "                                    Set specified ARF register with random value" << std::endl; \
    std::cout << "- rat0.arf.set.random [-S|-NEQ]     Set random ARF register with random value" << std::endl; \
    std::cout << "- rat0.arf.setall.random [-S|-NEQ]  Set all ARF register with random value" << std::endl; \
    std::cout << "- rat0.arf.get <index> [-U|-S|-NEQ] Get specified ARF register value" << std::endl;  \
    std::cout << "- rat0.diffsim.arf.set.random <count>" << std::endl; \
    std::cout << "                                    Random difftest of immediate register writes" << std::endl; \

    // rat0.infobystep [true|false]
    bool _RAT0_INFOBYSTEP(void* handle, const std::string& cmd,
                                         const std::string& paramline,
                                         const std::vector<std::string>& params)
    {
        if (params.size() > 1)
        {
            std::cout << "Too much or too less parameter(s) for \'rat0.infobystep\'." << std::endl;
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

    // rat0.rand.reg.value [max_value|@DEFAULT]
    bool _RAT0_RAND_REG_VALUE(void* handle, const std::string& cmd,
                                            const std::string& paramline,
                                            const std::vector<std::string>& params)
    {
        if (params.size() > 1)
        {
            std::cout << "Too much or too less parameter(s) for \'rat0.rand.reg.value\'." << std::endl;
            return false;
        }

        if (!params.empty())
        {
            uint64_t val;

            if (params[0].compare("@DEFAULT") == 0)
                val = SIM_DEFAULT_RAND_MAX_REG_VALUE;
            else
                std::istringstream(params[0]) >> val;

            if (val > RAND_MAX_REG_VALUE_MAX)
            {
                std::cout << "Param 0 \'" << val << "\' is invalid";
                printf(" [Max %lu (0x%016lx)].\n", RAND_MAX_REG_VALUE_MAX, RAND_MAX_REG_VALUE_MAX);
                return false;
            }

            GetCurrentHandle()->RandMaxRegValue = val;

            std::cout << "Set: ";
        }

        std::cout << "RAT0.rand.max.regvalue = ";
        printf("%lu (0x%016lx) ", GetCurrentHandle()->RandMaxRegValue, GetCurrentHandle()->RandMaxRegValue);
        printf(" [Max %lu (0x%016lx)]\n", RAND_MAX_REG_VALUE_MAX, RAND_MAX_REG_VALUE_MAX);

        return true;
    }


    // rat0.rand.reg.index [max_value|@DEFAULT]
    bool _RAT0_RAND_REG_INDEX(void* handle, const std::string& cmd,
                                            const std::string& paramline,
                                            const std::vector<std::string>& params)
    {
        if (params.size() > 1)
        {
            std::cout << "Too much or too less parameter(s) for \'rat0.rand.reg.index\'." << std::endl;
            return false;
        }

        if (!params.empty())
        {
            int val = -1;

            if (params[0].compare("@DEFAULT") == 0)
                val = SIM_DEFAULT_RAND_MAX_REG_INDEX;
            else 
                std::istringstream(params[0]) >> val;
            
            if (val < 0)
            {
                std::cout << "Param 0 \'" << params[0] << "\' is not a unsigned integer." << std::endl;
                return false;
            }

            unsigned int uval = *(unsigned int*)(&val);

            if (uval > RAND_MAX_REG_INDEX_MAX)
            {
                std::cout << "Param 0 \'" << uval << "\' is invalid";
                printf(" [Max %u (0x%08x)].\n", RAND_MAX_REG_INDEX_MAX, RAND_MAX_REG_INDEX_MAX);
                return false;
            }

            GetCurrentHandle()->RandMaxRegIndex = uval;

            std::cout << "Set: ";
        }

        std::cout << "RAT0.rand.max.regindex = ";
        printf("%u (0x%08x) ", GetCurrentHandle()->RandMaxRegIndex, GetCurrentHandle()->RandMaxRegIndex);
        printf(" [Max %u (0x%08x)]\n", RAND_MAX_REG_INDEX_MAX, RAND_MAX_REG_INDEX_MAX);

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
            std::cout << "(\'-Z\': Not listing zero registers)" << std::endl;

        std::cout << "Type         Index         Value" << std::endl;
        std::cout << "-----        ------        ------------------" << std::endl;

        for (int i = 0; i < EMULATED_ARF_SIZE; i++)
        {
            if (nonzero)
                if (!GetRefARF(GetCurrentHandle(), i))
                    continue;

            std::cout << "rARF         ";
            printf("%3d", i);
            std::cout << "           ";
            printf("0x%016lx\n", GetRefARF(GetCurrentHandle(), i));
        }

        return true;
    }

    
    // rat0.arf.ls [-Z|-U]
    bool _RAT0_ARF_LS(void* handle, const std::string& cmd,
                                    const std::string& paramline,
                                    const std::vector<std::string>& params)
    {
        bool filterZ = false;
        bool filterU = false;

        for (int i = 0; i < params.size(); i++)
        {
            std::string param = params[i];

            if (param.compare("-Z") == 0)
                filterZ = true;
            else if (param.compare("-U") == 0)
                filterU = true;
            else
            {
                std::cout << "Param " << i << " \'" << param << "\' is invalid." << std::endl;
                return false;
            }
        }

        SimHandle csim = GetCurrentHandle();

        if (filterZ)
            std::cout << "('-Z': Listing all non-zero ARFs)" << std::endl;

        if (filterU)
            std::cout << "('-U': Listing all mapped ARFs)" << std::endl;

        std::cout << "ARF        PRF        Value(PRF)              Value(Ref)" << std::endl;
        std::cout << "-----      -----      ------------------      ------------------" << std::endl;

        for (int i = 0; i < EMULATED_ARF_SIZE; i++)
        {
            bool mapped;
            int  mappedPRF;

            uint64_t val = GetMappedARF(csim, i, &mapped, &mappedPRF);
            uint64_t ref = GetRefARF(csim, i);

            if (filterU && !mapped)
                continue;

            if (filterZ && !val)
                continue;

            //
            if (mapped && !(i == 0 && csim->FlagARF0Conv))
            {
                if (val != ref)
                    printf("\033[1;31m");
                else
                    printf("\033[1;32m");
            }

            printf("%-5d      ", i);

            if (csim->FlagARF0Conv && !i)
                printf("ZERO       ");
            else if (mapped)
                printf("%-5d      ", mappedPRF);
            else
                printf("-          ");

            printf("0x%016lx      ", val);
            printf("0x%016lx\033[0m\n", GetRefARF(csim, i));
        }

        return true;
    }


    //
    bool _common_RAT0_ARF_SET(VMCHandle vmc, int index, uint64_t value,
                        bool flagS, bool flagNEQ)
    {
        //
        SimHandle csim = GetCurrentHandle();

        int prf = -1;

        if (!SetMappedARFAndEval(csim, index, value, &prf))
        {
            if (vmc->bWarnOnFalse)
                std::cout << "Failed to allocate entry in RAT for ARF #" << index << "." << std::endl;
            
            return false;
        }


        SetRefARF(csim, index, value);

        //
        uint64_t mARF = GetMappedARF(csim, index);
        uint64_t Ref  = GetRefARF(csim, index);

        if (!flagS)
        {
            if (mARF == Ref)
                printf("\033[1;32m");
            else
                printf("\033[1;31m");

            if (prf < 0) // Not mapped into PRF, converted by architecture
            {
                printf("ARF register #%d set but not mapped. mARF:0x%016lx. Ref: 0x%016lx.\033[0m\n",
                    index, mARF, Ref);
            }
            else
            {
                printf("ARF register #%d set. PRF #%d: 0x%016lx. mARF:0x%016lx. Ref: 0x%016lx.\033[0m\n",
                    index, prf, csim->PRF.Get(prf), mARF, Ref);
            }
        }

        return flagNEQ ? mARF == Ref : true;
    }

    // 
    bool _common_RAT0_ARF_SET_EX(VMCHandle vmc, int index, uint64_t value,
                        const std::vector<std::string>& params, int param_offset)
    {
        //
        bool flagS   = false;
        bool flagNEQ = false;

        for (int i = param_offset; i < params.size(); i++)
        {
            std::string param = params[i];

            if (param.compare("-S") == 0)
                flagS = true;
            else if (param.compare("-NEQ") == 0)
                flagNEQ = true;
            else
            {
                std::cout << "Param " << i << " \'" << param << "\' is invalid." << std::endl;
                return false;
            }
        }

        //
        return _common_RAT0_ARF_SET(vmc, index, value, flagS, flagNEQ);
    }


    // rat0.arf.set <index> <value> [-S|-NEQ]
    bool _RAT0_ARF_SET(void* handle, const std::string& cmd,
                                     const std::string& paramline,
                                     const std::vector<std::string>& params)
    {
        if (params.size() < 2)
        {
            std::cout << "Too much or too less parameter(s) for \'rat0.arf.set\'" << std::endl;
            return false;
        }

        //
        int      index;
        uint64_t value;

        std::istringstream(params[0]) >> index;
        std::istringstream(params[1]) >> value;

        //
        return _common_RAT0_ARF_SET_EX((VMCHandle) handle, index, value, params, 2);
    }

    
    // rat0.arf.set.randomval <index> [-S|-NEQ]
    bool _RAT0_ARF_SET_RANDOMVAL(void* handle, const std::string& cmd,
                                               const std::string& paramline,
                                               const std::vector<std::string>& params)
    {
        if (params.size() < 1)
        {
            std::cout << "Too much or too less parameter(s) for \'rat0.arf.set.randomval\'" << std::endl;
            return false;
        }

        //
        int      index;
        uint64_t value;

        std::istringstream(params[0]) >> index;
        value = RandRegValue(GetCurrentHandle());

        //
        return _common_RAT0_ARF_SET_EX((VMCHandle) handle, index, value, params, 1);
    }


    // rat0.arf.set.random [-S|-NEQ]
    bool _RAT0_ARF_SET_RANDOM(void* handle, const std::string& cmd,
                                            const std::string& paramline,
                                            const std::vector<std::string>& params)
    {
        //
        int      index;
        uint64_t value;

        index = RandRegIndex(GetCurrentHandle());
        value = RandRegValue(GetCurrentHandle());

        //
        return _common_RAT0_ARF_SET_EX((VMCHandle) handle, index, value, params, 0);
    }


    // rat0.arf.setall.random [-S|-NEQ]
    bool _RAT0_ARF_SETALL_RANDOM(void* handle, const std::string& cmd,
                                               const std::string& paramline,
                                               const std::vector<std::string>& params)
    {
        for (int i = 0; i < EMULATED_ARF_SIZE; i++)
        {
            int      index = i;
            uint64_t value = RandRegValue(GetCurrentHandle());

            if (!_common_RAT0_ARF_SET_EX((VMCHandle) handle, index, value, params, 0))
                return false;
        }

        return true;
    }


    // rat0.arf.get <index> [-NEQ|-U|-S]
    bool _RAT0_ARF_GET(void* handle, const std::string& cmd,
                                     const std::string& paramline,
                                     const std::vector<std::string>& params)
    {
        //
        if (params.empty())
        {
            std::cout << "Too much or too less parameter(s) for \'rat0.arf.get\'" << std::endl;
            return false;
        }

        //
        bool flagEQ = false;
        bool flagU  = false;
        bool flagS  = false;

        int index;
        std::istringstream(params[0]) >> index;

        for (int i = 1; i < params.size(); i++)
        {
            std::string param = params[i];

            if (param.compare("-NEQ") == 0)
                flagEQ = true;
            else if (param.compare("-U") == 0)
                flagU = true;
            else if (param.compare("-S") == 0)
                flagS = true;
            else 
            {
                std::cout << "Param " << i << " \'" << param <<  "\' is invalid." << std::endl;
                return false;
            }
        }

        //
        SimHandle csim = GetCurrentHandle();

        uint64_t ref;
        uint64_t val;

        bool mapped;
        int  mappedPRF;

        ref = GetRefARF(csim, index);
        val = GetMappedARF(csim, index, &mapped, &mappedPRF);

        //
        if (!flagS)
        {
            std::cout << "ARF        PRF        Value(PRF)              Value(Ref)" << std::endl;
            std::cout << "-----      -----      ------------------      ------------------" << std::endl;

            printf("%-5d      ", index);

            if (csim->FlagARF0Conv && !index)
                printf("ZERO       ");
            else if (mapped)
                printf("%-5d      ", mappedPRF);
            else
                printf("-          ");

            printf("0x%016lx      ", val);
            printf("0x%016lx\n"    , ref);
        }

        //
        if (flagEQ && val != ref)
            return false;

        if (!flagU || mapped)
            SetLastReturnInt((VMCHandle) handle, val);
        
        return true;
    }

    
    // rat0.prf.ls [-V|+V|-NRA|+NRA|-FV|+FV|-Z|+Z]
    bool _RAT0_PRF_LS(void* handle, const std::string& cmd,
                                    const std::string& paramline,
                                    const std::vector<std::string>& params)
    {
        bool enFilterV   = false, filterV;
        bool enFilterNRA = false, filterNRA;
        bool enFilterFV  = false, filterFV;
        bool enFilterZ   = false, filterZ;

        for (int i = 0; i < params.size(); i++)
        {
            std::string param = params[i];

            bool filterFlag;

            if (param.length() < 2)
            {
                std::cout << "Param " << i << " \'" << param << "\' is too short." << std::endl;
                return false;
            }

            char prefix = param.at(0);
            if (prefix == '-')
                filterFlag = false;
            else if (prefix == '+')
                filterFlag = true;
            else
            {
                std::cout << "Param " << i << " \'" << params[i] << "\' is invalid." << std::endl;
                return false;
            }

            std::string suffix = param.substr(1, param.length() - 1);
            if (suffix.compare("V") == 0)
            {
                enFilterV   = true;
                filterV     = filterFlag;
            }
            else if (suffix.compare("NRA") == 0)
            {
                enFilterNRA = true;
                filterNRA   = filterFlag;
            }
            else if (suffix.compare("FV") == 0)
            {
                enFilterFV  = true;
                filterFV    = filterFlag;
            }
            else if (suffix.compare("Z") == 0)
            {
                enFilterZ   = true;
                filterZ     = filterFlag;
            }
            else
            {
                std::cout << "Param " << i << " \'" << params[i] << "\' is invalid." << std::endl;
                return false;
            }
        }

        cout << "Register Alias Table entries:" << endl;

        if (enFilterV)
        {
            if (filterV)
                std::cout << "(\'+V\':   Listing all entries with V flag of 1)" << std::endl;
            else
                std::cout << "(\'-V\':   Listing all entries with V flag of 0)" << std::endl;
        }

        if (enFilterNRA)
        {
            if (filterNRA)
                std::cout << "(\'+NRA\': Listing all entries with NRA flag of 1)" << std::endl;
            else
                std::cout << "(\'-NRA\': Listing all entries with NRA flag of 0)" << std::endl;
        }

        if (enFilterFV)
        {
            if (filterFV)
                std::cout << "(\'+FV\':  Listing all entries with FV flag of 1)" << std::endl;
            else
                std::cout << "(\'-FV\':  Listing all entries with FV flag of 0)" << std::endl;
        }

        if (enFilterZ)
        {
            if (filterZ)
                std::cout << "(\'+Z\':   Listing all entries with zero PRF)" << std::endl;
            else
                std::cout << "(\'-Z\':   Listing all entries with non-zero PRF)" << std::endl;
        }

        std::cout << "PRF        ARF        V        NRA        FID        FV        Value" << std::endl;
        std::cout << "-----      -----      ---      -----      -----      ----      ------------------" << std::endl;

        SimHandle csim = GetCurrentHandle();
        for (int i = 0; i < csim->RAT.GetSize(); i++)
        {
            const RegisterAliasTable::Entry& entry = csim->RAT.GetEntry(i);
            
            if (enFilterV && filterV != entry.GetValid())
                continue;

            if (enFilterNRA && filterNRA != entry.GetNRA())
                continue;

            if (enFilterFV && filterFV != entry.GetFV())
                continue;

            if (enFilterZ && filterZ == (bool)entry.GetValue(csim->PRF))
                continue;

            printf("%-5d      ", entry.GetPRF());
            printf("%-5d      ", entry.GetARF());
            printf("%-3d      ", entry.GetValid());
            printf("%-5d      ", entry.GetNRA());
            printf("%-5d      ", entry.GetFID());
            printf("%-4d      ", entry.GetFV());
            printf("0x%016lx\n"  , entry.GetValue(csim->PRF));
        }

        return true;
    }


    // rat0.diffsim.arf.set.random <count>
    bool _RAT0_DIFFSIM_ARF_SET_RANDOM(void* handle, const std::string& cmd,
                                                    const std::string& paramline,
                                                    const std::vector<std::string>& params)
    {
        if (params.size() != 1)
        {
            std::cout << "Too much or too less parameter(s) for \'rat0.diffsim.arf.set.random\'" << std::endl;
            return false;
        }

        int count;
        std::istringstream(params[0]) >> count;

        //
        SimHandle csim = GetCurrentHandle();

        int i = 0;
        for (; i < count; i++)
        {
            int      index = RandRegIndex(csim);
            uint64_t value = RandRegValue(csim);

            int prf = -1;

            if (!SetMappedARFAndEval(csim, index, value, &prf))
            {
                printf("[%8d] \033[1;31mFailed to allocate RAT entry for ARF #%d.\033[0m\n",
                    i, index);
                break;
            }

            SetRefARF(csim, index, value);

            uint64_t mARF = GetMappedARF(csim, index);
            uint64_t Ref  = GetRefARF(csim, index);

            bool eq = mARF == Ref;

            if (csim->FlagStepInfo)
            {
                if (prf == -1)
                    printf("[%8d] ARF Register set but unmapped. ", i);
                else
                    printf("[%8d] ARF Register set. PRF #%d: 0x%016lx. ", i, prf, csim->PRF.Get(prf));

                if (!eq)
                    printf("\033[1;31m");

                printf("mARF: 0x%016lx. Ref: 0x%016lx.\033[0m\n", mARF, Ref);
            }

            if (!eq)
                break;
        }

        if (i == count)
        {
            printf("\033[1;32mProcedure completed (%d/%d).\033[0m\n", i, count);
            return true;
        }
        else
        {
            printf("\033[1;31mProcedure interrupted (%d/%d).\033[0m\n", i, count);
            return false;
        }
    }


    // rat0.diffsim.insn <insncode> [delay] [dstARF] [srcARF1] [srcARF2]
    bool _RAT0_DIFFSIM_INSN(void* handle, const std::string& cmd,
                                          const std::string& paramline,
                                          const std::vector<std::string>& params)
    {
        // TODO

        return true;
    }


    // rat0.diffsim.insn.random <count>
    bool _RAT0_DIFFSIM_INSN_RANDOM(void* handle, const std::string& cmd,
                                                 const std::string& paramline,
                                                 const std::vector<std::string>& params)
    {
        // TODO

        return true;
    }


    void SetupCommands(VMCHandle handle)
    {
        RegisterCommand(handle, CommandHandler{ std::string("rat0.infobystep")              , &_RAT0_INFOBYSTEP });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.arf0conv")                , &_RAT0_ARF0CONV });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.rand.reg.value")          , &_RAT0_RAND_REG_VALUE });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.rand.reg.index")          , &_RAT0_RAND_REG_INDEX });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.prf.ls")                  , &_RAT0_PRF_LS });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.arf.ls.ref")              , &_RAT0_ARF_LS_REF });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.arf.ls")                  , &_RAT0_ARF_LS});
        RegisterCommand(handle, CommandHandler{ std::string("rat0.arf.set")                 , &_RAT0_ARF_SET});
        RegisterCommand(handle, CommandHandler{ std::string("rat0.arf.set.randomval")       , &_RAT0_ARF_SET_RANDOMVAL });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.arf.set.random")          , &_RAT0_ARF_SET_RANDOM });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.arf.setall.random")       , &_RAT0_ARF_SETALL_RANDOM });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.arf.get")                 , &_RAT0_ARF_GET });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.diffsim.arf.set.random")  , &_RAT0_DIFFSIM_ARF_SET_RANDOM });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.diffsim.insn")            , &_RAT0_DIFFSIM_INSN });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.diffsim.insn.random")     , &_RAT0_DIFFSIM_INSN_RANDOM });
    }
}


// class VMC::RAT::SimRefARF
namespace VMC::RAT {
    /*
    uint64_t*   refARF;
    */

    SimRefARF::SimRefARF(uint64_t* refARF)
        : refARF(refARF)
    { }

    SimRefARF::SimRefARF(const SimRefARF& obj)
        : refARF(obj.refARF)
    { }

    uint64_t SimRefARF::operator[](const int index) const
    {
        return refARF[index];
    }
}


// class VMC::RAT::SimMappedARF
namespace VMC::RAT {
    /*
    RegisterAliasTable*     RAT;
    PhysicalRegisterFile*   PRF;
    */

    SimMappedARF::SimMappedARF(RegisterAliasTable* RAT, PhysicalRegisterFile* PRF)
        : RAT   (RAT)
        , PRF   (PRF)
    { }

    SimMappedARF::SimMappedARF(const SimMappedARF& obj)
        : RAT   (obj.RAT)
        , PRF   (obj.PRF)
    { }

    uint64_t SimMappedARF::operator[](const int index) const
    {
        if (!index)
            return 0;

        int  mappedPRF = RAT->GetAliasPRF(index);
        bool mapped    = mappedPRF >= 0;

        return mapped ? PRF->Get(mappedPRF) : 0;
    }
}


// class VMC::RAT::SimInstruction
namespace VMC::RAT {
    /*
    int     FID;
    int     clkDelay;
    int     dst;
    int     src1;
    int     src2;
    int     insncode;
    */

    SimInstruction::SimInstruction()
        : FID       (-1)
        , delay     (0)
        , dst       (0)
        , src1      (0)
        , src2      (0)
        , insncode  (INSN_CODE_NOP)
    { }

    SimInstruction::SimInstruction(int FID, int clkDelay, int dst)
        : FID       (FID)
        , delay     (delay)
        , dst       (dst)
        , src1      (0)
        , src2      (0)
        , insncode  (INSN_CODE_NOP)
    { }

    SimInstruction::SimInstruction(int FID, int clkDelay, int dst, int src1, int src2)
        : FID       (FID)
        , delay     (delay)
        , dst       (dst)
        , src1      (src1)
        , src2      (src2)
        , insncode  (INSN_CODE_NOP)
    { }

    SimInstruction::SimInstruction(int FID, int clkDelay, int dst, int src1, int src2, int insncode)
        : FID       (FID)
        , delay     (delay)
        , dst       (dst)
        , src1      (src1)
        , src2      (src2)
        , insncode  (INSN_CODE_NOP)
    { }

    SimInstruction::SimInstruction(const SimInstruction& obj)
        : FID       (obj.FID)
        , delay     (delay)
        , dst       (obj.dst)
        , src1      (obj.src1)
        , src2      (obj.src2)
        , insncode  (obj.insncode)
    { }

    SimInstruction::~SimInstruction()
    { }

    inline int SimInstruction::GetFID() const
    {
        return FID;
    }

    inline int SimInstruction::GetDelay() const
    {
        return delay;
    }

    inline int SimInstruction::GetDst() const
    {
        return dst;
    }

    inline int SimInstruction::GetSrc1() const
    {
        return src1;
    }

    inline int SimInstruction::GetSrc2() const
    {
        return src2;
    }

    inline int SimInstruction::GetInsnCode() const
    {
        return insncode;
    }

    inline void SimInstruction::SetDst(int dst)
    {
        this->dst = dst;
    }

    inline void SimInstruction::SetSrc1(int src1)
    {
        this->src1 = src1;
    }

    inline void SimInstruction::SetSrc2(int src2)
    {
        this->src2 = src2;
    }

    inline void SimInstruction::SetInsnCode(int insncode)
    {
        this->insncode = insncode;
    }
}


// class VMC::RAT::SimScoreboard
namespace VMC::RAT {
    /*
    const int   size;

    bool* const busy;
    int*  const FID;
    */

    SimScoreboard::SimScoreboard(int size)
        : size  (size)
        , busy  (new bool[size]())
        , FID   (new int[size])
    { }

    SimScoreboard::SimScoreboard(const SimScoreboard& obj)
        : size  (obj.size)
        , busy  (new bool[obj.size]())
        , FID   (new int[obj.size])
    {
        memcpy(busy, obj.busy, obj.size * sizeof(bool));
        memcpy(FID,  obj.FID,  obj.size * sizeof(int));
    }

    SimScoreboard::~SimScoreboard()
    {
        delete busy;
        delete FID;
    }

    void SimScoreboard::Clear()
    {
        memset(busy, 0, size * sizeof(bool));
    }

    inline bool SimScoreboard::IsBusy(int index) const
    {
        if (!index)
            return false;

        return busy[index];
    }

    inline int SimScoreboard::GetFID(int index) const
    {
        if (!index)
            return -1;

        if (busy[index])
            return FID[index];
        
        return -1;
    }

    inline void SimScoreboard::SetBusy(int index, int FID)
    {
        /*
        if (!index)
            return;
        */

        this->busy[index] = true;
        this->FID[index]  = FID;
    }

    void SimScoreboard::Release(int FID)
    {
        for (int i = 1; i < size; i++)
        {
            if (this->FID[i] == FID && this->busy[i])
            {
                this->busy[i] = false;

                // only one destination register for one instruction
                break;
            }
        }
    }
}


// class VMC::RAT::SimReservation::Entry
namespace VMC::RAT {
    /*
    const SimInstruction    insn;
    bool                    src1rdy;
    bool                    src2rdy;
    */

    SimReservation::Entry::Entry(const SimInstruction& insn)
        : insn      (insn)
        , src1rdy   (false)
        , src2rdy   (false)
    { }

    SimReservation::Entry::Entry(const SimInstruction& insn, bool src1rdy, bool src2rdy)
        : insn      (insn)
        , src1rdy   (src1rdy)
        , src2rdy   (src2rdy)
    { }

    SimReservation::Entry::Entry(const Entry& obj)
        : insn      (obj.insn)
        , src1rdy   (obj.src1rdy)
        , src2rdy   (obj.src2rdy)
    { }

    SimReservation::Entry::~Entry()
    { }

    inline const SimInstruction& SimReservation::Entry::GetInsn() const
    {
        return insn;
    }

    inline bool SimReservation::Entry::IsSrc1Ready() const
    {
        return src1rdy;
    }

    inline bool SimReservation::Entry::IsSrc2Ready() const
    {
        return src2rdy;
    }

    inline bool SimReservation::Entry::IsReady() const
    {
        return src1rdy && src2rdy;
    }

    inline int SimReservation::Entry::GetSrc1() const
    {
        return insn.GetSrc1();
    }

    inline int SimReservation::Entry::GetSrc2() const
    {
        return insn.GetSrc2();
    }

    inline void SimReservation::Entry::SetSrc1Ready(bool rdy)
    {
        src1rdy = rdy;
    }

    inline void SimReservation::Entry::SetSrc2Ready(bool rdy)
    {
        src2rdy = rdy;
    }
}


// class VMC::RAT::SimReservation
namespace VMC::RAT {
    /*
    const SimScoreboard*    scoreboard;
    list<Entry>             entries;
    list<Entry>::iterator   next;
    */

    SimReservation::SimReservation(const SimScoreboard* scoreboard)
        : scoreboard(scoreboard)
        , entries   (list<Entry>())
    { }

    SimReservation::SimReservation(const SimReservation& obj)
        : scoreboard(obj.scoreboard)
        , entries   (obj.entries)
    { }

    SimReservation::~SimReservation()
    { }

    void SimReservation::PushInsn(const SimInstruction& insn)
    {
        Entry entry = Entry(insn);
        entry.SetSrc1Ready(insn.GetSrc1() ? !scoreboard->IsBusy(insn.GetSrc1()) : true);
        entry.SetSrc2Ready(insn.GetSrc2() ? !scoreboard->IsBusy(insn.GetSrc2()) : true);

        entries.push_back(entry);
    }

    inline const SimScoreboard* SimReservation::GetScoreboardRef() const
    {
        return scoreboard;
    }

    bool SimReservation::NextInsn(SimInstruction* insn)
    {
        if (next != entries.end())
        {
            if (insn)
                *insn = next->GetInsn();

            return true;
        }

        std::list<Entry>::iterator iter = entries.begin();
        while (iter != entries.end())
        {
            if (iter->IsReady())
            {
                next = iter;

                if (insn)
                    *insn = iter->GetInsn();

                return true;
            }
            
            iter++;
        }

        return false;
    }

    bool SimReservation::PopInsn()
    {
        if (next == entries.end() && !NextInsn())
            return false;

        entries.erase(next);
        next = entries.end();

        return true;
    }
    
    inline void SimReservation::Clear()
    {
        entries.clear();
    }

    void SimReservation::Eval()
    {
        std::list<Entry>::iterator iter = entries.begin();
        while (iter != entries.end())
        {
            if (iter->IsReady())
                continue;

            if (!iter->IsSrc1Ready() && !scoreboard->IsBusy(iter->GetSrc1()))
                iter->SetSrc1Ready();

            if (!iter->IsSrc2Ready() && !scoreboard->IsBusy(iter->GetSrc2()))
                iter->SetSrc2Ready();
        }

        // always keep the next instruction oldest
        next = entries.end();
    }
}


// class VMC::RAT::SimExecution::Entry
namespace VMC::RAT {
    /*
    const SimInstruction    insn;
    uint64_t                src1val;
    uint64_t                src2val;
    int                     delay;
    uint64_t                dstval;
    */

    SimExecution::Entry::Entry(const SimInstruction& insn)
        : insn      (insn)
        , src1val   (0)
        , src2val   (0)
        , delay     (insn.GetDelay())
        , dstrdy    (false)
        , dstval    (0)
    { }

    SimExecution::Entry::Entry(const SimInstruction& insn, uint64_t src1val, uint64_t src2val)
        : insn      (insn)
        , src1val   (src1val)
        , src2val   (src2val)
        , delay     (insn.GetDelay())
        , dstrdy    (false)
        , dstval    (0)
    { }

    SimExecution::Entry::Entry(const Entry& obj)
        : insn      (obj.insn)
        , src1val   (obj.src1val)
        , src2val   (obj.src2val)
        , delay     (obj.delay)
        , dstrdy    (obj.dstrdy)
        , dstval    (obj.dstval)
    { }

    SimExecution::Entry::~Entry()
    { }

    inline const SimInstruction& SimExecution::Entry::GetInsn() const
    {
        return insn;
    }

    inline uint64_t SimExecution::Entry::GetSrc1Value() const
    {
        return src1val;
    }

    inline uint64_t SimExecution::Entry::GetSrc2Value() const
    {
        return src2val;
    }

    inline bool SimExecution::Entry::IsReady() const
    {
        return dstrdy && !delay;
    }

    inline bool SimExecution::Entry::IsDstReady() const
    {
        return dstrdy;
    }

    inline uint64_t SimExecution::Entry::GetDstValue() const
    {
        return dstval;
    }

    inline void SimExecution::Entry::SetSrc1Value(uint64_t value)
    {
        src1val = value;
    }

    inline void SimExecution::Entry::SetSrc2Value(uint64_t value)
    {
        src1val = value;
    }

    inline void SimExecution::Entry::SetDstReady(bool ready)
    {
        dstrdy = ready;
    }

    inline void SimExecution::Entry::SetDstValue(uint64_t value)
    {
        dstval = value;
    }

    void SimExecution::Entry::Eval()
    {
        if (delay)
            delay--;
        else if (!dstrdy)
        {
            switch (insn.GetInsnCode())
            {
                case INSN_CODE_NOP:
                    dstval = src1val;
                    break;

                case INSN_CODE_AND:
                    dstval = src1val & src2val;
                    break;

                case INSN_CODE_OR:
                    dstval = src1val | src2val;
                    break;

                case INSN_CODE_XOR:
                    dstval = src1val ^ src2val;
                    break;

                case INSN_CODE_ADD:
                    dstval = src1val + src2val;
                    break;

                case INSN_CODE_SUB:
                    dstval = src1val - src2val;
                    break;

                default:
                    ShouldNotReachHere("SimExecution::INVALID_INSN_CODE");
                    break;
            }

            dstrdy = true;
        }
    }
}


// class VMC::RAT::SimExecution
namespace VMC::RAT {
    /*
    bool                    const ref;
    SimRefARF*              const refARF;
    SimMappedARF*           const mappedARF;
    list<Entry>             entries;
    list<Entry>::iterator   next;
    */

    SimExecution::SimExecution(uint64_t* ARF)
        : ref       (true)
        , refARF    (new SimRefARF(ARF))
        , mappedARF (nullptr)
        , entries   (list<Entry>())
        , next      (entries.end())
    { }

    SimExecution::SimExecution(RegisterAliasTable* RAT, PhysicalRegisterFile* PRF)
        : ref       (false)
        , refARF    (nullptr)
        , mappedARF (new SimMappedARF(RAT, PRF))
        , entries   (list<Entry>())
        , next      (entries.end())
    { }

    SimExecution::SimExecution(const SimExecution& obj)
        : ref       (obj.ref)
        , refARF    ( ref ? new SimRefARF(*refARF) : nullptr)
        , mappedARF (!ref ? new SimMappedARF(*mappedARF) : nullptr)
        , entries   (obj.entries)
        , next      (entries.end())
    { }

    SimExecution::~SimExecution()
    {
        if (ref)
            delete refARF;
        else
            delete mappedARF;
    }

    void SimExecution::PushInsn(const SimInstruction& insn, Entry* entry = nullptr)
    {
        uint64_t src1val = ref ? (*refARF)[insn.GetSrc1()] : (*mappedARF)[insn.GetSrc1()];
        uint64_t src2val = ref ? (*refARF)[insn.GetSrc2()] : (*mappedARF)[insn.GetSrc2()];

        Entry newEntry(insn, src1val, src2val);

        if (entry)
            *entry = newEntry;

        entries.push_back(newEntry);
    }

    bool SimExecution::NextInsn(Entry* entry)
    {
        if (next != entries.end())
        {
            if (entry)
                *entry = *next;

            return true;
        }

        std::list<Entry>::iterator iter = entries.begin();
        while (iter != entries.end())
        {
            if (iter->IsReady())
            {
                next = iter;

                if (entry)
                    *entry = *iter;

                return true;
            }

            iter++;
        }

        return false;
    }

    bool SimExecution::PopInsn()
    {
        if (next == entries.end() && !NextInsn())
            return false;

        entries.erase(next);
        next = entries.end();

        return true;
    }

    void SimExecution::Eval()
    {
        std::list<Entry>::iterator iter = entries.begin();
        while (iter != entries.end())
            (iter++)->Eval();
    }
}


// class VMC::RAT::SimReOrderBuffer::Entry
namespace VMC::RAT {
    /*
    SimInstruction  insn;
    bool            rdy;
    uint64_t        dstval;
    */

    SimReOrderBuffer::Entry::Entry(const SimInstruction& insn)
        : insn      (insn)
        , rdy       (false)
        , dstval    (0)
    { }

    SimReOrderBuffer::Entry::Entry(const Entry& obj)
        : insn      (obj.insn)
        , rdy       (obj.rdy)
        , dstval    (obj.dstval)
    { }

    SimReOrderBuffer::Entry::~Entry()
    { }

    inline const SimInstruction& SimReOrderBuffer::Entry::GetInsn() const
    {
        return insn;
    }

    inline int SimReOrderBuffer::Entry::GetFID() const
    {
        return insn.GetFID();
    }

    inline bool SimReOrderBuffer::Entry::IsReady() const
    {
        return rdy;
    }

    inline uint64_t SimReOrderBuffer::Entry::GetDstValue() const
    {
        return dstval;
    }

    inline void SimReOrderBuffer::Entry::SetReady(bool ready)
    {
        rdy = ready;
    }

    inline void SimReOrderBuffer::Entry::SetDstValue(uint64_t value)
    {
        dstval = value;
    }
}


// class VMC::RAT::SimReOrderBuffer
namespace VMC::RAT {
    /*
    std::list<Entry>            entries;
    std::list<Entry>::iterator  next;
    */

    SimReOrderBuffer::SimReOrderBuffer()
        : entries   (std::list<Entry>())
        , next      (entries.end())
    { }

    SimReOrderBuffer::SimReOrderBuffer(const SimReOrderBuffer& obj)
        : entries   (obj.entries)
        , next      (entries.end())
    { }

    SimReOrderBuffer::~SimReOrderBuffer()
    { }

    void SimReOrderBuffer::TouchInsn(const SimInstruction& insn)
    {
        entries.push_back(Entry(insn));
    }

    bool SimReOrderBuffer::WritebackInsn(const SimInstruction& insn, uint64_t value)
    {
        std::list<Entry>::iterator iter = entries.begin();
        while (iter != entries.end())
        {
            if (iter->GetFID() != insn.GetFID())
            {
                iter++;
                continue;
            }

            if (iter->IsReady())
            {
                // writeback overlap, not permitted
                return false;
            }

            iter->SetDstValue(value);
            iter->SetReady();

            return true;
        }

        // FID not found in ROB
        return false;
    }

    bool SimReOrderBuffer::NextInsn(Entry* entry)
    {
        if (next != entries.end())
        {
            if (entry)
                *entry = *next;

            return true;
        }

        std::list<Entry>::iterator iter = entries.begin();
        while (iter != entries.end())
        {
            if (iter->IsReady())
            {
                next = iter;

                if (entry)
                    *entry = *iter;

                return true;
            }

            iter++;
        }

        return false;
    }

    bool SimReOrderBuffer::PopInsn()
    {
        if (next == entries.end() && !NextInsn())
            return false;

        entries.erase(next);
        next = entries.end();
        
        return true;
    }
}
