#pragma once

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
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

    static constexpr int INSN_CODE_ANDI     = 6;

    static constexpr int INSN_CODE_ORI      = 7;
    
    static constexpr int INSN_CODE_XORI     = 8;

    static constexpr int INSN_CODE_ADDI     = 9;

    static constexpr int INSN_CODE_SUBI     = 10;

    static constexpr int INSN_COUNT         = 11;

    //
    class SimStageStatus {
    private:
        const std::string str;

    public:
        SimStageStatus(const char* str);
        SimStageStatus(const std::string str);
        ~SimStageStatus();

        const std::string&  GetString() const;
    };

    static const SimStageStatus     STAGE_STATUS_IDLE   = SimStageStatus("\033[1;32mIDLE\033[0m");

    static const SimStageStatus     STAGE_STATUS_WAIT   = SimStageStatus("\033[1;33mWAIT\033[0m");

    static const SimStageStatus     STAGE_STATUS_BUSY   = SimStageStatus("\033[1;31mBUSY\033[0m");

    //
    class SimRefPipeStatus {
    private:
        const SimStageStatus*   fetchStatus;
        const SimStageStatus*   reservationStatus;
        const SimStageStatus*   executionStatus;

    public:
        SimRefPipeStatus();
        ~SimRefPipeStatus();

        const SimStageStatus*   GetFetchStatus() const;
        const SimStageStatus*   GetReservationStatus() const;
        const SimStageStatus*   GetExecutionStatus() const;

        void                    SetFetchStatus(const SimStageStatus* status);
        void                    SetReservationStatus(const SimStageStatus* status);
        void                    SetExecutionStatus(const SimStageStatus* status);
    };

    class SimO3PipeStatus {
    private:
        const SimStageStatus*   fetchStatus;
        const SimStageStatus*   reservationStatus;
        const SimStageStatus*   executionStatus;
        const SimStageStatus*   robStatus;

    public:
        SimO3PipeStatus();
        ~SimO3PipeStatus();

        const SimStageStatus*   GetFetchStatus() const;
        const SimStageStatus*   GetReservationStatus() const;
        const SimStageStatus*   GetExecutionStatus() const;
        const SimStageStatus*   GetROBStatus() const;

        void                    SetFetchStatus(const SimStageStatus* status);
        void                    SetReservationStatus(const SimStageStatus* status);
        void                    SetExecutionStatus(const SimStageStatus* status);
        void                    SetROBStatus(const SimStageStatus* status);
    };

    //
    class SimRefARF
    {
    private:
        uint64_t* const refARF;

    public:
        SimRefARF(uint64_t* refARF);
        SimRefARF(const SimRefARF& obj);

        uint64_t    operator[](const int index) const;
    };

    class SimO3ARF
    {
    private:
        PhysicalRegisterFile*   const PRF;

    public:
        SimO3ARF(PhysicalRegisterFile* PRF);
        SimO3ARF(const SimO3ARF& obj);

        uint64_t    operator[](const int index) const;
    };

    class SimInstruction
    {
    private:
        int         FID;
        int         delay;
        int         dst;
        int         src1;
        int         src2;
        int         insncode;
        uint64_t    imm;

    public:
        SimInstruction();
        SimInstruction(int FID, int delay, int insncode, int dst);
        SimInstruction(int FID, int delay, int insncode, int dst, uint64_t imm);
        SimInstruction(int FID, int delay, int insncode, int dst, int src1, int src2);
        SimInstruction(int FID, int delay, int insncode, int dst, int src1, int src2, uint64_t imm);
        SimInstruction(const SimInstruction& obj);
        ~SimInstruction();

        int         GetFID() const;
        int         GetDelay() const;
        int         GetDst() const;
        int         GetSrc1() const;
        int         GetSrc2() const;
        int         GetInsnCode() const;
        uint64_t    GetImmediate() const;

        void        SetDst(int dst);
        void        SetSrc1(int src1);
        void        SetSrc2(int src2);
        void        SetInsnCode(int insncode);
        void        SetImmediate(uint64_t imm);
    };

    class SimInstructionMemory {

        // TODO

    };

    class SimFetch
    {
    private:
        std::list<SimInstruction>   fetched;

    public:
        SimFetch();
        ~SimFetch();

        const std::list<SimInstruction>& Get() const;

        int     GetCount() const;
        bool    IsEmpty() const;

        void    Clear();

        void    PushInsn(const SimInstruction& insn);

        bool    NextInsn(SimInstruction* insn = nullptr);
        bool    PopInsn();
    };


    static constexpr int SCOREBOARD_STATUS_BUSY     = -1;

    static constexpr int SCOREBOARD_STATUS_IN_ARF   = 0;

    static constexpr int SCOREBOARD_STATUS_IN_ROB   = 1;

    static constexpr int SCOREBOARD_STATUS_FORWARD  = 2; // unused

    class SimScoreboard // Only support ARF0-conv mode
    {
    public:
        class Modification {
        private:
            int index;
            int status;
            int FID;

        public:
            Modification();
            Modification(int index, int status, int FID);
            ~Modification();

            void    SetIndex(int index);
            void    SetStatus(int status);
            void    SetFID(int FID);

            int     GetIndex() const;
            int     GetStatus() const;
            int     GetFID() const;
        };

    private:
        const int               size;

        std::list<Modification> modification;

        int*              const status;
        int*              const FID;

    public:
        SimScoreboard(int size);
        SimScoreboard(const SimScoreboard& obj);
        ~SimScoreboard();

        void    Clear();

        int     GetFID(int index) const;
        bool    IsBusy(int index) const;
        int     GetStatus(int index) const;
        void    SetStatus(int index, int status, int FID = -1);

        void    ResetInput();

        void    Eval();
    };

    class SimReservation // Only support ARF0-conv mode
    {
    public:
        class Entry {
        private:
            SimInstruction  insn;
            bool            src1rdy;
            bool            src2rdy;
            bool            dstrdy;

        public:
            Entry(const SimInstruction& insn);
            Entry(const SimInstruction& insn, bool src1rdy, bool src2rdy, bool dstrdy);
            Entry(const Entry& obj);
            ~Entry();

            const SimInstruction&   GetInsn() const;
            bool                    IsSrc1Ready() const;
            bool                    IsSrc2Ready() const;
            bool                    IsDstReady() const;
            bool                    IsReady() const;
            int                     GetSrc1() const;
            int                     GetSrc2() const;
            int                     GetDst() const;

            void                    SetSrc1Ready(bool rdy = true);
            void                    SetSrc2Ready(bool rdy = true);
            void                    SetDstReady(bool rdy = true);
        };

    private:
        const SimScoreboard*    const scoreboard;
        std::list<Entry>              entries;
        std::list<Entry>::iterator    next;

    public:
        SimReservation(const SimScoreboard* scoreboard);
        SimReservation(const SimReservation& obj);
        ~SimReservation();

        const std::list<Entry>& Get() const;

        const SimScoreboard*    GetScoreboardRef() const;
        

        void                    PushInsn(const SimInstruction& insn);

        bool                    NextInsn(SimInstruction* insn = nullptr);
        bool                    PopInsn();

        void                    Clear();
        int                     GetCount() const;
        bool                    IsEmpty() const;

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
            Entry();
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
        SimO3ARF*             const o3ARF;
        std::list<Entry>            entries;
        std::list<Entry>::iterator  next;

    public:
        SimExecution(uint64_t* ARF);
        SimExecution(PhysicalRegisterFile* PRF);
        SimExecution(const SimExecution& obj);
        ~SimExecution();

        const std::list<Entry>& Get() const;

        int     GetCount() const;
        bool    IsEmpty() const;

        void    PushInsn(const SimInstruction& insn, Entry* entry = nullptr);
        void    PushInsnEx(const SimInstruction& insn, uint64_t src1val, uint64_t src2val, Entry* entry = nullptr);

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
            Entry();
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

    public:
        SimReOrderBuffer();
        SimReOrderBuffer(const SimReOrderBuffer& obj);
        ~SimReOrderBuffer();

        const std::list<Entry>& Get() const;

        int     GetCount() const;
        bool    IsEmpty() const;

        bool    GetDstValue(int FID, uint64_t* dst) const;

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

    static constexpr int            SIM_DEFAULT_RAND_MAX_INSN_DELAY = 32;

    typedef struct {

        RegisterAliasTable          O3RAT                       = RegisterAliasTable();

        PhysicalRegisterFile        O3PRF                       = PhysicalRegisterFile();

        SimFetch                    O3Fetch                     = SimFetch();

        SimScoreboard               O3Scoreboard                = SimScoreboard(EMULATED_PRF_SIZE);

        SimReservation              O3Reservation               = SimReservation(&O3Scoreboard);

        SimExecution                O3Execution                 = SimExecution(&O3PRF);

        SimReOrderBuffer            O3ROB                       = SimReOrderBuffer();

        SimO3PipeStatus             O3Status                    = SimO3PipeStatus();

        int                         O3PC                        = 0;

        uint64_t                    RefARF[EMULATED_ARF_SIZE]   = { 0 };

        SimFetch                    RefFetch                    = SimFetch();

        SimScoreboard               RefScoreboard               = SimScoreboard(EMULATED_ARF_SIZE);

        SimReservation              RefReservation              = SimReservation(&RefScoreboard);

        SimExecution                RefExecution                = SimExecution(RefARF);

        SimRefPipeStatus            RefStatus                   = SimRefPipeStatus();

        int                         RefPC                       = 0;

        int                         GlobalFID                   = 0;

        bool                        FlagStepInfo                = SIM_DEFAULT_FLAG_STEPINFO;

        bool                        FlagARF0Conv                = SIM_DEFAULT_FLAG_ARF0CONV;

        uint64_t                    RandMaxRegValue             = SIM_DEFAULT_RAND_MAX_REG_VALUE;

        unsigned int                RandMaxRegIndex             = SIM_DEFAULT_RAND_MAX_REG_INDEX;

        unsigned int                RandMaxInsnDelay            = SIM_DEFAULT_RAND_MAX_INSN_DELAY;

        //
        std::vector<SimInstruction> FetchHistory                = std::vector<SimInstruction>();

        std::vector<SimInstruction> RenamedHistory              = std::vector<SimInstruction>();

        int                         O3FetchCount                = 0;

        int                         O3CommitCount               = 0;

        int                         RefFetchCount               = 0;

        int                         RefCommitCount              = 0;

        int                         O3ROBCountMax               = 0;

        int                         O3ReservationCountMax       = 0;

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
        uint64_t val =  (uint64_t)rand() 
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

    inline int RandRegIndex(SimHandle handle)
    {
        return rand() % handle->RandMaxRegIndex;
    }

    inline int RandInsnDelay(SimHandle handle)
    {
        return rand() % handle->RandMaxInsnDelay;
    }

    inline int RandInsn(SimHandle handle)
    {
        return rand() % INSN_COUNT;
    }

    inline int NextFID(SimHandle handle)
    {
        return handle->GlobalFID++;
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

    inline uint64_t GetO3ARF(SimHandle handle, int arf, bool* out_mapped = 0, int* out_mappedPRF = 0)
    {
        if (handle->FlagARF0Conv && !arf)
            return 0;

        int  mappedPRF = handle->O3RAT.GetAliasPRF(arf);
        bool mapped    = mappedPRF >= 0;

        if (out_mapped)
            *out_mapped = mapped;

        if (out_mappedPRF)
            *out_mappedPRF = mappedPRF;

        return mapped ? handle->O3PRF.Get(mappedPRF) : 0;
    }

    inline bool SetO3ARF(SimHandle handle, int arf, uint64_t val, int* out_prf = 0)
    {
        if (handle->FlagARF0Conv && !arf)
            return true;

        int prf;
        if (!handle->O3RAT.TouchAndCommit(-1, arf, &prf))
            return false;

        handle->O3PRF.Set(prf, val);

        if (out_prf)
            *out_prf = prf;

        return true;
    }

    inline bool SetO3ARFAndEval(SimHandle handle, int arf, uint64_t val, int* out_prf = 0)
    {
        if (!SetO3ARF(handle, arf, val, out_prf))
            return false;

        handle->O3RAT.Eval();
        handle->O3PRF.Eval();

        return true;
    }

    //
    bool EvalRefFetch(SimHandle csim, bool info, int step)
    {
        if (csim->RefFetch.IsEmpty())
        {
            csim->RefStatus.SetFetchStatus(&STAGE_STATUS_IDLE);

            if (info)
                printf("[ %8d ] RefFetch: Fetch queue empty.\n", step);

            return true;
        }

        if (!csim->RefReservation.IsEmpty())
        {
            csim->RefStatus.SetFetchStatus(&STAGE_STATUS_WAIT);

            if (info)
                printf("[ %8d ] RefFetch: Fetch queue wait for reserved issue.\n", step);

            return true;
        }

        //
        SimInstruction insn;

        if (!csim->RefFetch.NextInsn(&insn))
        {
            ShouldNotReachHere(" RAT::EvalRefFetch ILLEGAL_STATE #FetchFail");
            return false;
        }

        //
        csim->RefReservation.PushInsn(insn);

        csim->RefFetch.PopInsn();

        csim->RefFetchCount++;

        csim->RefStatus.SetFetchStatus(&STAGE_STATUS_BUSY);

        if (info)
            printf("[ %8d ] RefFetch: Instruction fetched, passed to issue. FID: %d.\n", step, insn.GetFID());

        return true;
    }

    bool EvalRefIssue(SimHandle csim, bool info, int step)
    {
        if (csim->RefReservation.IsEmpty())
        {
            csim->RefStatus.SetReservationStatus(&STAGE_STATUS_IDLE);

            if (info)
                printf("[ %8d ] RefIssue: Issue queue empty.\n", step);

            return true;
        }

        //
        SimInstruction insn;

        if (!csim->RefReservation.NextInsn(&insn))
        {
            csim->RefStatus.SetReservationStatus(&STAGE_STATUS_WAIT);

            if (info)
                printf("[ %8d ] RefIssue: Issue pause on scoreboard. ARF not ready.\n", step);

            return true;
        }

        //
        csim->RefExecution.PushInsn(insn);

        if (insn.GetDst()) // arf0conv on scoreboard
            csim->RefScoreboard.SetStatus(insn.GetDst(), SCOREBOARD_STATUS_BUSY, insn.GetFID());

        if (!csim->RefReservation.PopInsn())
        {
            ShouldNotReachHere(" RAT::EvalRefIssue ILLEGAL_STATE #ReservationPopFail");
            return false;
        }

        csim->RefStatus.SetReservationStatus(&STAGE_STATUS_BUSY);

        if (info)
            printf("[ %8d ] RefIssue: Instruction issued. FID: %d.\n", step, insn.GetFID());

        return true;
    }

    bool EvalRefWriteback(SimHandle csim, bool info, int step)
    {
        if (csim->RefExecution.IsEmpty())
        {
            csim->RefStatus.SetExecutionStatus(&STAGE_STATUS_IDLE);

            if (info)
                printf("[ %8d ] RefWriteback: Execution unit empty.\n", step);

            return true;
        }

        //
        SimExecution::Entry entry;

        if (!csim->RefExecution.NextInsn(&entry))
        {
            csim->RefStatus.SetExecutionStatus(&STAGE_STATUS_BUSY);

            if (info)
                printf("[ %8d ] RefWriteback: Instruction writeback not ready.\n", step);

            return true;
        }

        const SimInstruction& insn = entry.GetInsn();

        //
        SetRefARF(csim, insn.GetDst(), entry.GetDstValue());

        if (!csim->RefExecution.PopInsn())
        {
            ShouldNotReachHere(" RAT::EvalRefWriteback ILLEGAL_STATE #ExecutionPopFail");
            return false;
        }

        csim->RefScoreboard.SetStatus(insn.GetDst(), SCOREBOARD_STATUS_IN_ARF);

        csim->RefCommitCount++;

        csim->RefStatus.SetExecutionStatus(&STAGE_STATUS_BUSY);

        if (info)
            printf("[ %8d ] RefWriteback: FID #%d written-back. DstARF #%d.\n", step, insn.GetFID(), insn.GetDst());

        return true;
    }

    void EvalRefCycle(SimHandle csim)
    {
        csim->RefReservation.Eval();
        csim->RefScoreboard.Eval();
        csim->RefExecution.Eval();
    }

    bool EvalRef(SimHandle csim, bool info, int step)
    {
        if (!EvalRefFetch(csim, info, step))
        {
            std::cout << "EvalRef: Interrupted at EvalRefFetch" << std::endl;
            return false;
        }

        if (!EvalRefIssue(csim, info, step))
        {
            std::cout << "EvalRef: Interrupted at EvalRefIssue" << std::endl;
            return false;
        }

        if (!EvalRefWriteback(csim, info, step))
        {
            std::cout << "EvalRef: Interrupted at EvalRefWriteback" << std::endl;
            return false;
        }

        EvalRefCycle(csim);

        return true;
    }


    bool EvalO3Fetch(SimHandle csim, bool info, int step)
    {
        if (csim->O3Fetch.IsEmpty())
        {
            csim->O3Status.SetFetchStatus(&STAGE_STATUS_IDLE);

            if (info)
                printf("[ %8d ] O3Fetch: Fetch queue empty.\n", step);

            return true;
        }

        //
        SimInstruction insn;

        if (!csim->O3Fetch.NextInsn(&insn))
        {
            ShouldNotReachHere(" RAT::EvalO3Fetch ILLEGAL_STATE #FetchFail");
            return false;
        }

        // Get src PRFs
        int src1prf = csim->O3RAT.GetAliasPRF(insn.GetSrc1());
        int src2prf = csim->O3RAT.GetAliasPRF(insn.GetSrc2());

        if (info)
        {
            printf("[ %8d ] O3Fetch: ARF converted. Src1 ARF #%d -> PRF #%d.\n", step, insn.GetSrc1(), src1prf);
            printf("[ %8d ] O3Fetch: ARF converted. Src2 ARF #%d -> PRF #%d.\n", step, insn.GetSrc2(), src2prf);
        }

        // Try to allocate RAT entry
        int dstprf = -1;
        
        if (!csim->O3RAT.Touch(insn.GetFID(), insn.GetDst(), &dstprf))
        {
            csim->O3Status.SetFetchStatus(&STAGE_STATUS_WAIT);

            if (info)
                printf("[ %8d ] O3Fetch: Wait on FID #%d. RAT not available.\n", info, insn.GetFID());

            return true;
        }

        if (info)
            printf("[ %8d ] O3Fetch: Dst ARF renamed. Dst ARF #%d -> PRF #%d.\n", step, insn.GetDst(), dstprf);

        // Re-write/modify instruction after RAT
        insn.SetSrc1(src1prf);
        insn.SetSrc2(src2prf);
        insn.SetDst(dstprf);

        // Rename history
        csim->RenamedHistory.push_back(insn);

        // Push to issue queue and broadcast to ROB
        csim->O3Reservation.PushInsn(insn);
        csim->O3ROB.TouchInsn(insn);

        csim->O3Scoreboard.SetStatus(insn.GetDst(), SCOREBOARD_STATUS_BUSY, insn.GetFID());

        csim->O3Fetch.PopInsn();

        csim->O3FetchCount++;

        csim->O3Status.SetFetchStatus(&STAGE_STATUS_BUSY);

        if (info)
            printf("[ %8d ] O3Fetch: Instruction fetched and renamed, passed to reseravtion. FID #%d.\n", step, insn.GetFID());

        return true;
    }

    bool EvalO3Issue(SimHandle csim, bool info, int step)
    {
        if (csim->O3Reservation.IsEmpty())
        {
            csim->O3Status.SetReservationStatus(&STAGE_STATUS_IDLE);

            if (info)
                printf("[ %8d ] O3Issue: Issue queue empty.\n", step);

            return true;
        }

        //
        SimInstruction insn;

        if (!csim->O3Reservation.NextInsn(&insn))
        {
            csim->O3Status.SetReservationStatus(&STAGE_STATUS_WAIT);

            if (info)
                printf("[ %8d ] O3Issue: Issue pause on scoreboard. PRF not ready or reservation delay.\n", step);

            if (info)
            {
                int count = 0;

                printf("[ %8d ] O3Issue: Not ready PRFs:", step);

                for (int i = 0; i < EMULATED_PRF_SIZE; i++)
                {
                    if (!csim->O3Scoreboard.IsBusy(i))
                        continue;
                    
                    printf(" #%d", i);

                    count++;
                }

                printf(" (%d in total)\n", count);
            }

            return true;
        }

        //
        uint64_t src1val;
        uint64_t src2val;

        int src1status = csim->O3Scoreboard.GetStatus(insn.GetSrc1());
        int src2status = csim->O3Scoreboard.GetStatus(insn.GetSrc2());

        switch (src1status)
        {
            case SCOREBOARD_STATUS_IN_ARF:
                src1val = SimO3ARF(&(csim->O3PRF))[insn.GetSrc1()];

                if (info)
                    printf("[ %8d ] O3Issue: Read src1 PRF #%d value: %ld (0x%016lx).\n", step, insn.GetSrc1(), src1val, src1val);

                break;

            case SCOREBOARD_STATUS_IN_ROB:
                if (!csim->O3ROB.GetDstValue(csim->O3Scoreboard.GetFID(insn.GetSrc1()), &src1val))
                {
                    ShouldNotReachHere(" RAT::EvalO3Issue ILLEGAL_STATE #ROBForwardFailure(src1)");
                    return false;
                }

                if (info)
                    printf("[ %8d ] O3Issue: Read src1 PRF #%d value: %ld (0x%016lx) from ROB.\n", step, insn.GetSrc1(), src1val, src1val);

                break;

            default:
                ShouldNotReachHere(" RAT::EvalO3Issue ILLEGAL_STATUE #ScoreboardStatus(src1)");
                return false;
        }

        switch (src2status)
        {
            case SCOREBOARD_STATUS_IN_ARF:
                src2val = SimO3ARF(&(csim->O3PRF))[insn.GetSrc2()];

                if (info)
                    printf("[ %8d ] O3Issue: Read src2 PRF #%d value: %ld (0x%016lx).\n", step, insn.GetSrc2(), src2val, src2val);

                break;

            case SCOREBOARD_STATUS_IN_ROB:
                if (!csim->O3ROB.GetDstValue(csim->O3Scoreboard.GetFID(insn.GetSrc2()), &src2val))
                {
                    ShouldNotReachHere(" RAT::EvalO3Issue ILLEGAL_STATE #ROBForwardFailure(src2)");
                    return false;
                }

                if (info)
                    printf("[ %8d ] O3Issue: Read src2 PRF #%d value: %ld (0x%016lx) from ROB.\n", step, insn.GetSrc2(), src2val, src2val);

                break;

            default:
                ShouldNotReachHere(" RAT::EvalO3Issue ILLEGAL_STATUE #ScoreboardStatus(src2)");
                return false;
        }

        csim->O3Execution.PushInsnEx(insn, src1val, src2val);

        if (!csim->O3Reservation.PopInsn())
        {
            ShouldNotReachHere(" RAT::EvalO3Issue ILLEGAL_STATE #ReservationPopFail");
            return false;
        }

        csim->O3Status.SetReservationStatus(&STAGE_STATUS_BUSY);

        if (info)
            printf("[ %8d ] O3Issue: Instruction issued. FID: %d.\n", step, insn.GetFID());

        return true;
    }

    bool EvalO3Writeback(SimHandle csim, bool info, int step)
    {
        if (csim->O3Execution.IsEmpty())
        {
            csim->O3Status.SetExecutionStatus(&STAGE_STATUS_IDLE);
            
            if (info)
                printf("[ %8d ] O3Writeback: Execution unit empty.\n", step);

            return true;
        }

        //
        SimExecution::Entry entry;

        if (!csim->O3Execution.NextInsn(&entry))
        {
            csim->O3Status.SetExecutionStatus(&STAGE_STATUS_BUSY);

            if (info)
                printf("[ %8d ] O3Writeback: Instruction writeback not ready.\n", step);

            return true;
        }

        const SimInstruction& insn = entry.GetInsn();

        //
        if (!csim->O3ROB.WritebackInsn(insn, entry.GetDstValue()))
        {
            ShouldNotReachHere(" RAT::EvalO3Writeback ILLEGAL_STATE #ROBWritebackFail");
            return false;
        }

        csim->O3Scoreboard.SetStatus(insn.GetDst(), SCOREBOARD_STATUS_IN_ROB);

        if (!csim->O3Execution.PopInsn())
        {
            ShouldNotReachHere(" RAT::EvalO3Writeback ILLEGAL_STATE #ExecutionPopFail");
            return false;
        }

        csim->O3Status.SetExecutionStatus(&STAGE_STATUS_BUSY);

        if (info)
            printf("[ %8d ] O3Writeback: FID #%d written-back to ROB. DstPRF #%d.\n", step, insn.GetFID(), insn.GetDst());

        return true;
    }

    bool EvalO3Commit(SimHandle csim, bool info, int step)
    {
        if (csim->O3ROB.IsEmpty())
        {
            csim->O3Status.SetROBStatus(&STAGE_STATUS_IDLE);

            if (info)
                printf("[ %8d ] O3Commit: ROB empty.\n", step);

            return true;
        }

        //
        SimReOrderBuffer::Entry entry;

        if (!csim->O3ROB.NextInsn(&entry))
        {
            csim->O3Status.SetROBStatus(&STAGE_STATUS_WAIT);
            
            if (info)
                printf("[ %8d ] O3Commit: ROB wait for further writeback.\n", step);

            return true;
        }

        const SimInstruction& insn = entry.GetInsn();

        //
        if (insn.GetDst() >= 0)
            csim->O3PRF.Set(insn.GetDst(), entry.GetDstValue());

        if (info)
            printf("[ %8d ] O3Commit: Written back to PRF #%d: %ld (0x%016lx).\n", step, insn.GetDst(), entry.GetDstValue(), entry.GetDstValue());

        // assert
        if (insn.GetDst() >= 0 && csim->O3Scoreboard.GetFID(insn.GetDst()) != insn.GetFID())
        {
            printf("[ \033[1;31mASSERT\033[0m   ] O3Commit: Scoreboard commit FID not identical.\n");
            return false;
        }

        csim->O3Scoreboard.SetStatus(insn.GetDst(), SCOREBOARD_STATUS_IN_ARF);

        // assert
        if (insn.GetDst() >= 0 && csim->O3RAT.GetEntry(insn.GetDst()).GetFID() != insn.GetFID())
        {
            printf("[ \033[1;31mASSERT\033[0m   ] O3Commit: RAT commit FID not identical.\n");
            return false;
        }

        if (insn.GetDst() >= 0)
            csim->O3RAT.Commit(insn.GetDst());

        if (!csim->O3ROB.PopInsn())
        {
            ShouldNotReachHere(" RAT::EvalO3Commit ILLEGAL_STATE #ROBPopFail");
            return false;
        }

        csim->O3CommitCount++;

        csim->O3Status.SetROBStatus(&STAGE_STATUS_BUSY);

        if (info)
            printf("[ %8d ] O3Commit: Instruction committed. FID: %d.\n", step, insn.GetFID());

        return true;
    }

    void EvalO3Cycle(SimHandle csim)
    {
        csim->O3Reservation.Eval();
        csim->O3Scoreboard.Eval();
        csim->O3Execution.Eval();
        csim->O3RAT.Eval();
        csim->O3PRF.Eval();

        csim->O3ReservationCountMax = max(csim->O3ReservationCountMax, csim->O3Reservation.GetCount());
        csim->O3ROBCountMax = max(csim->O3ROBCountMax, csim->O3ROB.GetCount());
    }

    bool EvalO3(SimHandle csim, bool info, int step)
    {
        if (!EvalO3Fetch(csim, info, step))
        {
            std::cout << "EvalO3: Interrupted at EvalO3Fetch" << std::endl;
            return false;
        }

        if (!EvalO3Issue(csim, info, step))
        {
            std::cout << "EvalO3: Interrupted at EvalO3Issue" << std::endl;
            return false;    
        }

        if (!EvalO3Writeback(csim, info, step))
        {
            std::cout << "EvalO3: Interrupted at EvalO3Writeback" << std::endl;
            return false;
        }

        if (!EvalO3Commit(csim, info, step))
        {
            std::cout << "EvalO3: Interrupted at EvalO3Commit" << std::endl;
            return false;
        }

        EvalO3Cycle(csim);

        return true;
    }


    inline void DispRefStatus(SimHandle csim)
    {
        std::cout << "Ref:";
        std::cout << " Fetch[" << csim->RefStatus.GetFetchStatus()->GetString()       << "]";
        std::cout << " Issue[" << csim->RefStatus.GetReservationStatus()->GetString() << "]";
        std::cout << " EX/WB[" << csim->RefStatus.GetExecutionStatus()->GetString()   << "]" << std::endl;
    }

    inline bool IsRefAllIdle(SimHandle csim)
    {
        return (&STAGE_STATUS_IDLE == csim->RefStatus.GetFetchStatus())
            && (&STAGE_STATUS_IDLE == csim->RefStatus.GetReservationStatus())
            && (&STAGE_STATUS_IDLE == csim->RefStatus.GetExecutionStatus());
    }

    inline void DispO3Status(SimHandle csim)
    {
        std::cout << "O3: ";
        std::cout << " Fetch[" << csim->O3Status.GetFetchStatus()->GetString()       << "]";
        std::cout << " Issue[" << csim->O3Status.GetReservationStatus()->GetString() << "]";
        std::cout << " EX/WB[" << csim->O3Status.GetExecutionStatus()->GetString()   << "]";
        std::cout << " ROB["   << csim->O3Status.GetROBStatus()->GetString()         << "]" << std::endl;
    }

    inline bool IsO3AllIdle(SimHandle csim)
    {
        return (&STAGE_STATUS_IDLE == csim->O3Status.GetFetchStatus())
            && (&STAGE_STATUS_IDLE == csim->O3Status.GetReservationStatus())
            && (&STAGE_STATUS_IDLE == csim->O3Status.GetExecutionStatus())
            && (&STAGE_STATUS_IDLE == csim->O3Status.GetROBStatus());
    }
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
    std::cout << "- rat0.diffsim.insn.push <insncode> [delay] [dstARF] [srcARF1] [srcARF2] [imm]" << std::endl; \
    std::cout << "                                    Push a specified instruction" << std::endl; \
    std::cout << "- rat0.diffsim.insn.push.random <count>" << std::endl; \
    std::cout << "                                    Push specified count of random instructions" << std::endl; \
    std::cout << "- rat0.diffsim.insn.eval.step       Differential evaluate RAT by one step" << std::endl; \
    std::cout << "- rat0.diffsim.insn.eval.stepout [-SF|-WWDG|-T <count>]" << std::endl; \
    std::cout << "                                    Differential evaluate RAT with all instructions or specified steps" << std::endl; \
    std::cout << "- rat0.diffsim.insn.dump [-R|-range <startFID> <endFID>]" << std::endl; \
    std::cout << "                                    Dump history instructions." << std::endl; \
    std::cout << "- rat0.diffsim.insn.dumpfile <filename> [-R|-range <startFID> <endFID>]" << std::endl; \
    std::cout << "                                    Dump history instructions to file." << std::endl; \
    std::cout << "- rat0.diffsim.o3.fetch.dump        Dump all current O3Fetch entries." << std::endl; \
    std::cout << "- rat0.diffsim.o3.scoreboard.dump   Dump all current O3Scoreboard entries." << std::endl; \
    std::cout << "- rat0.diffsim.o3.reservation.dump  Dump all current O3Reservation entries." << std::endl; \
    std::cout << "- rat0.diffsim.o3.rob.dump          Dump all current O3ReOrderBuffer entries." << std::endl; \


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

            uint64_t val = GetO3ARF(csim, i, &mapped, &mappedPRF);
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
            else if (ref)
                printf("\033[1;33m");

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

        if (!SetO3ARFAndEval(csim, index, value, &prf))
        {
            if (vmc->bWarnOnFalse)
                std::cout << "Failed to allocate entry in RAT for ARF #" << index << "." << std::endl;
            
            return false;
        }

        SetRefARF(csim, index, value);

        //
        uint64_t mARF = GetO3ARF(csim, index);
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
                    index, prf, csim->O3PRF.Get(prf), mARF, Ref);
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
        val = GetO3ARF(csim, index, &mapped, &mappedPRF);

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

        if (enFilterZ)
        {
            if (filterZ)
                std::cout << "(\'+Z\':   Listing all entries with zero PRF)" << std::endl;
            else
                std::cout << "(\'-Z\':   Listing all entries with non-zero PRF)" << std::endl;
        }

        std::cout << "PRF        ARF        V        NRA        FV        FID         Value" << std::endl;
        std::cout << "-----      -----      ---      -----      ----      ------      ------------------" << std::endl;

        SimHandle csim = GetCurrentHandle();
        for (int i = 0; i < csim->O3RAT.GetSize(); i++)
        {
            const RegisterAliasTable::Entry& entry = csim->O3RAT.GetEntry(i);
            
            if (enFilterV && filterV != entry.GetValid())
                continue;

            if (enFilterNRA && filterNRA != entry.GetNRA())
                continue;

            if (enFilterZ && filterZ == (bool)entry.GetValue(csim->O3PRF))
                continue;

            printf("%-5d      ", entry.GetPRF());
            printf("%-5d      ", entry.GetARF());
            printf("%-3d      ", entry.GetValid());
            printf("%-5d      ", entry.GetNRA());
            printf("%-4d      ", entry.GetFV());
            printf("%-6d      ", entry.GetFID());
            printf("0x%016lx\n"  , entry.GetValue(csim->O3PRF));
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

            if (!SetO3ARFAndEval(csim, index, value, &prf))
            {
                printf("[%8d] \033[1;31mFailed to allocate RAT entry for ARF #%d.\033[0m\n",
                    i, index);
                break;
            }

            SetRefARF(csim, index, value);

            uint64_t mARF = GetO3ARF(csim, index);
            uint64_t Ref  = GetRefARF(csim, index);

            bool eq = mARF == Ref;

            if (csim->FlagStepInfo)
            {
                if (prf == -1)
                    printf("[%8d] ARF Register set but unmapped. ", i);
                else
                    printf("[%8d] ARF Register set. PRF #%d: 0x%016lx. ", i, prf, csim->O3PRF.Get(prf));

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


    // rat0.diffsim.insn.push <insncode> [delay] [dstARF] [srcARF1] [srcARF2] [imm]
    bool _RAT0_DIFFSIM_INSN_PUSH(void* handle, const std::string& cmd,
                                               const std::string& paramline,
                                               const std::vector<std::string>& params)
    {
        //
        int      insncode = INSN_CODE_NOP;
        int      delay    = 0;
        int      dstARF   = 0;
        int      srcARF1  = 0;
        int      srcARF2  = 0;
        uint64_t imm      = 0;

        std::string param;
        std::vector<std::string>::const_iterator argiter = params.begin();

        if (argiter == params.end())
        {
            std::cout << "Too much or too less parameter(s) for \'rat0.diffsim.insn.push\'" << std::endl;
            return false;
        }

        // insncode
        param = *argiter;
        if (param.compare("NOP") == 0)
            insncode = INSN_CODE_NOP;
        else if (param.compare("AND") == 0)
            insncode = INSN_CODE_AND;
        else if (param.compare("OR") == 0)
            insncode = INSN_CODE_OR;
        else if (param.compare("XOR") == 0)
            insncode = INSN_CODE_XOR;
        else if (param.compare("ADD") == 0)
            insncode = INSN_CODE_ADD;
        else if (param.compare("SUB") == 0)
            insncode = INSN_CODE_SUB;
        else if (param.compare("ANDI") == 0)
            insncode = INSN_CODE_ANDI;
        else if (param.compare("ORI") == 0)
            insncode = INSN_CODE_ORI;
        else if (param.compare("XORI") == 0)
            insncode = INSN_CODE_XORI;
        else if (param.compare("ADDI") == 0)
            insncode = INSN_CODE_ADDI;
        else if (param.compare("SUBI") == 0)
            insncode = INSN_CODE_SUBI;
        else
        {
            std::cout << "Unknown instruction: " << param << std::endl;
            std::cout << "Param 0 \'" << param << "\' is invalid." << std::endl;
            return false;
        }

        argiter++;

        // delay
        if (argiter == params.end())
            goto END_OF_INSN_PARSE;

        std::istringstream(*argiter) >> delay;

        argiter++;

        // dstARF
        if (argiter == params.end())
            goto END_OF_INSN_PARSE;

        std::istringstream(*argiter) >> dstARF;

        argiter++;

        // srcARF1
        if (argiter == params.end())
            goto END_OF_INSN_PARSE;

        std::istringstream(*argiter) >> srcARF1;
        
        argiter++;

        // srcARF2
        if (argiter == params.end())
            goto END_OF_INSN_PARSE;

        std::istringstream(*argiter) >> srcARF2;

        argiter++;

        // imm
        if (argiter == params.end())
            goto END_OF_INSN_PARSE;

        std::istringstream(*argiter) >> imm;

        argiter++;

        //
        if (argiter != params.end())
        {
            std::cout << "Too much or too less parameter(s) for \'rat0.diffsim.insn.push\'" << std::endl;
            return false;
        }

        //
        END_OF_INSN_PARSE:
            ;

        //
        SimHandle csim = GetCurrentHandle();
        int FID = NextFID(csim);

        SimInstruction insn(FID, delay, insncode, dstARF, srcARF1, srcARF2, imm);

        csim->O3Fetch.PushInsn(insn);
        csim->RefFetch.PushInsn(insn);

        csim->FetchHistory.push_back(insn);

        return true;
    }

    // rat0.diffsim.insn.push.random <count>
    bool _RAT0_DIFFSIM_INSN_PUSH_RANDOM(void* handle, const std::string& cmd,
                                                      const std::string& paramline,
                                                      const std::vector<std::string>& params)
    {
        if (params.size() != 1)
        {
            std::cout << "Too much or too less parameter(s) for \'rat0.diffsim.insn.push.random\'" << std::endl;
            return false;
        }

        SimHandle csim = GetCurrentHandle();

        int count;
        std::istringstream(params[0]) >> count;

        for (int i = 0; i < count; i++)
        {
            int FID = NextFID(csim);

            int      insncode = INSN_CODE_NOP;
            int      delay    = 0;
            int      dstARF   = 0;
            int      srcARF1  = 0;
            int      srcARF2  = 0;
            uint64_t imm      = 0;

            //
            insncode = RandInsn(csim);
            
            //
            delay = RandInsnDelay(csim);

            //
            dstARF = RandRegIndex(csim);

            // 
            srcARF1 = RandRegIndex(csim);
            srcARF2 = RandRegIndex(csim);

            //
            imm = RandRegValue(csim); // stub

            //
            SimInstruction insn(FID, delay, insncode, dstARF, srcARF1, srcARF2, imm);

            if (csim->FlagStepInfo)
                printf("[ %8d ] FID: %d, Delay: %d, Insncode: 0x%08x, DstARF: %d, SrcARF1: %d, SrcARF2: %d, Imm: %lu (0x%016lx)\n",
                    i, FID, delay, insncode, dstARF, srcARF1, srcARF2, imm, imm);

            csim->O3Fetch.PushInsn(insn);
            csim->RefFetch.PushInsn(insn);

            csim->FetchHistory.push_back(insn);
        }

        return true;
    }


    //
    bool __common_RAT0_DIFFSIM_INSN_EVAL(bool info, int step = 0)
    {
        SimHandle csim = GetCurrentHandle();

        // O3 (out-of-order) datapath
        bool O3 = EvalO3(csim, info, step);

        // Ref (in-order) datapath
        bool Ref = EvalRef(csim, info, step);

        return O3 && Ref;
    }


    //
    void __common_RAT0_DIFFSIM_INSN_DUMP(const SimInstruction& insn, 
                                         std::ostream& os)
    {
        os << "FID(" << std::setw(8) << std::setfill(' ') << insn.GetFID() << ") ";
        os << "#" << insn.GetDelay() << " ";

        switch (insn.GetInsnCode())
        {
            case INSN_CODE_NOP:
                os << "NOP  ";
                break;

            case INSN_CODE_AND:
                os << "AND  ";
                break;

            case INSN_CODE_OR:
                os << "OR   ";
                break;

            case INSN_CODE_XOR:
                os << "XOR  ";
                break;

            case INSN_CODE_ADD:
                os << "ADD  ";
                break;

            case INSN_CODE_SUB:
                os << "SUB  ";
                break;

            case INSN_CODE_ANDI:
                os << "ANDI ";
                break;

            case INSN_CODE_ORI:
                os << "ORI  ";
                break;

            case INSN_CODE_XORI:
                os << "XORI ";
                break;

            case INSN_CODE_ADDI:
                os << "ADDI ";
                break;

            case INSN_CODE_SUBI:
                os << "SUBI ";
                break;
            
            default:
                ShouldNotReachHere(" ::InsnDump #UnknownInstructionCode");
        }
    
        os << std::setw(2) << std::setfill(' ') << insn.GetDst()  << " ";
        os << std::setw(2) << std::setfill(' ') << insn.GetSrc1() << " ";
        os << std::setw(2) << std::setfill(' ') << insn.GetSrc2() << " ";

        os << "0x" << std::setw(16) << std::setfill('0') << std::hex << insn.GetImmediate() << std::dec;
        os << std::resetiosflags << std::endl;
    }

    //
    inline void __common_RAT0_DIFFSIM_ROB_DUMP(const SimReOrderBuffer::Entry& entry,
                                               std::ostream& os)
    {
        os << "ROB_READY(" << entry.IsReady() << ") ";
        __common_RAT0_DIFFSIM_INSN_DUMP(entry.GetInsn(), os);
    }

    //
    inline void __common_RAT0_DIFFSIM_RESERVATION_DUMP(const SimReservation::Entry& entry,
                                                       std::ostream& os)
    {
        os << "DST_READY(" << entry.IsDstReady()  << ") ";
        os << "S1_READY("  << entry.IsSrc1Ready() << ") ";
        os << "S2_READY("  << entry.IsSrc2Ready() << ") ";
        __common_RAT0_DIFFSIM_INSN_DUMP(entry.GetInsn(), os);
    } 

    // 
    inline void __common_RAT0_DIFFSIM_SCOREBOARD_DUMP(SimHandle csim,
                                                      int index)
    {
        std::cout << "PRF(" << std::setw(2) << std::setfill(' ') << index << ") ";
        std::cout << "BUSY(1) ";
        std::cout << "FID(" << std::setw(8) << std::setfill(' ') << csim->O3Scoreboard.GetFID(index) << ")" << std::endl;
    }


    // rat0.diffsim.insn.eval.step
    bool _RAT0_DIFFSIM_INSN_EVAL_STEP(void* handle, const std::string& cmd,
                                                    const std::string& paramline,
                                                    const std::vector<std::string>& params)
    {
        SimHandle csim = GetCurrentHandle();

        if (!csim->FlagARF0Conv)
        {
            std::cout << "Only support ARF0-conv mode." << std::endl;
            return false;
        }

        if (!params.empty())
        {
            std::cout << "Too much or too less parameter(s) for \'rat0.diffsim.insn.eval.step\'" << std::endl;
            return false;
        }

        if (!__common_RAT0_DIFFSIM_INSN_EVAL(csim->FlagStepInfo))
        {
            std::cout << "Eval failure." << std::endl;
            return false;
        }

        DispRefStatus(csim);
        DispO3Status(csim);

        return true;
    }


    // rat0.diffsim.insn.eval.stepout [-SF|-WWDG|-T <count>]
    bool _RAT0_DIFFSIM_INSN_EVAL_STEPOUT(void* handle, const std::string& cmd,
                                                       const std::string& paramline,
                                                       const std::vector<std::string>& params)
    {
        // TODO: implement flag -SF if further debug required

        SimHandle csim = GetCurrentHandle();

        if (!csim->FlagARF0Conv)
        {
            std::cout << "Only support ARF0-conv mode." << std::endl;
            return false;
        }

        bool flagT    = false;
        bool flagNWDG = false;

        int varT = 0;

        for (int i = 0; i < params.size(); i++)
        {
            std::string param = params[i];

            if (param.compare("-T") == 0)
            {
                if (i + 1 < params.size())
                {
                    flagT = true;
                    std::istringstream(params[++i]) >> varT;
                }
                else
                {
                    std::cout << "Param " << i << " \'" << param << "\' needs more parameter(s)." << std::endl;
                    return false;
                }
            }
            else if (param.compare("-NWDG") == 0)
                flagNWDG = true;
            else
            {
                std::cout << "Param " << i << " \'" << param << "\' is invalid." << std::endl;
                return false;
            }
        }

        int step = 0;

        int perfc_ref = 0;
        int perfc_o3  = 0;

        //
        int insncnt       = 0;
        int insncnt_digit = 1;

        int updatecnt     = 0;

        bool finish_ref_fetch   = false;
        bool finish_ref_commit  = false;
        bool finish_o3_fetch    = false;
        bool finish_o3_commit   = false;

        bool first_cursor = true;

        //
        csim->O3FetchCount  = 0;
        csim->O3CommitCount = 0;

        csim->RefFetchCount  = 0;
        csim->RefCommitCount = 0;

        if (csim->FlagStepInfo)
            printf("[ -------- ] --------------------------------\n");
        else
        {
            //
            insncnt = csim->RefFetch.GetCount();

            while (insncnt /= 10)
                insncnt_digit++;

            //
            insncnt = csim->RefFetch.GetCount();

            std::cout << "Instruction count " << insncnt << " in total in fetch queue." << std::endl;
        }

        do 
        {
            step++;

            if (!__common_RAT0_DIFFSIM_INSN_EVAL(csim->FlagStepInfo, step))
            {
                std::cout << "Eval failure at step " << step << "." << std::endl;
                return false;
            }

            if (!IsRefAllIdle(csim))
                perfc_ref++;

            if (!IsO3AllIdle(csim))
                perfc_o3++;

            if (csim->FlagStepInfo)
            {
                printf("[ %8d ] ", step);
                DispRefStatus(csim);

                printf("[ %8d ] ", step);
                DispO3Status(csim);

                printf("[ -------- ] --------------------------------\n");
            }

            if (!flagNWDG && perfc_o3 > perfc_ref)
            {
                std::cout << "[ \033[1;33mWARN\033[0m     ] EvalO3 (Out-of-Order) slacked back then EvalRef (In-Order)." << std::endl;
                std::cout << "[ \033[1;33mWARN\033[0m     ] EvalO3 might be in dead loop. Stopped step-out process." << std::endl;

                //
                std::cout << "[ \033[1;33mWARN\033[0m     ] Dump O3Fetch entries." << std::endl;

                std::list<SimInstruction> o3fetch = csim->O3Fetch.Get();
                std::list<SimInstruction>::const_iterator o3fetch_iter = o3fetch.begin();
                for (; o3fetch_iter != o3fetch.end(); o3fetch_iter++)
                {
                    std::cout << "[ \033[1;33mWARN\033[0m     ] - ";
                    __common_RAT0_DIFFSIM_INSN_DUMP(*o3fetch_iter, std::cout);
                }

                //
                std::cout << "[ \033[1;33mWARN\033[0m     ] Dump O3Scoreboard entries." << std::endl;

                for (int i = 0; i < EMULATED_PRF_SIZE; i++)
                {
                    if (csim->O3Scoreboard.IsBusy(i))
                    {
                        std::cout << "[ \033[1;33mWARN\033[0m     ] - ";
                        __common_RAT0_DIFFSIM_SCOREBOARD_DUMP(csim, i);
                    }
                }

                //
                std::cout << "[ \033[1;33mWARN\033[0m     ] Dump O3Reservation entries." << std::endl;

                std::list<SimReservation::Entry> o3reservation = csim->O3Reservation.Get();
                std::list<SimReservation::Entry>::const_iterator o3reservation_iter = o3reservation.begin();
                for (; o3reservation_iter != o3reservation.end(); o3reservation_iter++)
                {
                    std::cout << "[ \033[1;33mWARN\033[0m     ] - ";
                    __common_RAT0_DIFFSIM_RESERVATION_DUMP(*o3reservation_iter, std::cout);
                }

                //
                std::cout << "[ \033[1;33mWARN\033[0m     ] Dump O3ROB entries." << std::endl;

                std::list<SimReOrderBuffer::Entry> o3rob = csim->O3ROB.Get();
                std::list<SimReOrderBuffer::Entry>::const_iterator o3rob_iter = o3rob.begin();
                for (; o3rob_iter != o3rob.end(); o3rob_iter++)
                {
                    std::cout << "[ \033[1;33mWARN\033[0m     ] - ";
                    __common_RAT0_DIFFSIM_ROB_DUMP(*o3rob_iter, std::cout);
                }

                break;
            }

            if (!csim->FlagStepInfo
                && (updatecnt == 22133 
                    || (!finish_ref_fetch  && (finish_ref_fetch  = csim->RefFetchCount == insncnt)) 
                    || (!finish_ref_commit && (finish_ref_commit = csim->RefCommitCount == insncnt))
                    || (!finish_o3_fetch   && (finish_o3_fetch   = csim->O3FetchCount == insncnt))
                    || (!finish_o3_commit  && (finish_o3_commit  = csim->O3CommitCount == insncnt))))
            {
                updatecnt = 0;

                std::cout << "\33[?25l";

                if (!first_cursor)
                    std::cout << "\033[3A" << std::endl;
                else
                    first_cursor = false;

                //
                std::cout << "EvalRef: ";
                std::cout << "Fetch: ";
                if (csim->RefFetchCount == insncnt)
                    std::cout << "\033[1;32m";
                else
                    std::cout << "\033[1;33m";
                std::cout << std::setw(insncnt_digit) << std::setfill(' ') << csim->RefFetchCount << "\033[0m";
                std::cout << "/" << insncnt << "  ";

                std::cout << "Commit: ";
                if (csim->RefCommitCount == insncnt)
                    std::cout << "\033[1;32m";
                else
                    std::cout << "\033[1;33m";
                std::cout << std::setw(insncnt_digit) << std::setfill(' ') << csim->RefCommitCount << "\033[0m";
                std::cout << "/" << insncnt << std::endl;

                //
                std::cout << "EvalO3:  ";
                std::cout << "Fetch: ";
                if (csim->O3FetchCount == insncnt)
                    std::cout << "\033[1;32m";
                else
                    std::cout << "\033[1;33m";
                std::cout << std::setw(insncnt_digit) << std::setfill(' ') << csim->O3FetchCount << "\033[0m";
                std::cout << "/" << insncnt << "  ";

                std::cout << "Commit: ";
                if (csim->O3CommitCount == insncnt)
                    std::cout << "\033[1;32m";
                else
                    std::cout << "\033[1;33m";
                std::cout << std::setw(insncnt_digit) << std::setfill(' ') << csim->O3CommitCount << "\033[0m";
                std::cout << "/" << insncnt << std::endl;

                std::cout << "\33[?25h";
            }
            else
                updatecnt++;

            if (flagT)
            {
                if (step < varT)
                    continue;

                break;
            }
        }
        while (!IsRefAllIdle(csim) || !IsO3AllIdle(csim));

        std::cout << "Finished stepout. " << step << " step(s) walked in total." << std::endl;

        std::cout << "Performance counter:" << std::endl;

        printf("- EvalRef: %d/%d (%.2f%%)\n", perfc_ref, step, (float)perfc_ref / step * 100);
        printf("- EvalO3:  %d/%d (%.2f%%)\n", perfc_o3,  step, (float)perfc_o3  / step * 100);

        printf("- O3ROBCountMax: %d\n", csim->O3ROBCountMax);
        printf("- O3ReservationCountMax: %d\n", csim->O3ReservationCountMax);

        return true;
    }


    //
    bool __common_RAT0_DIFFSIM_INSN_DUMP_EX(int offset, 
                                            const std::vector<std::string>& params,
                                            std::ostream& os)
    {
        bool flagR      = false;
        bool flagRange  = false;

        int startFID = 0;
        int endFID   = 0;
        
        for (int i = offset; i < params.size(); i++)
        {
            std::string param = params[i];
            
            if (param.compare("-R") == 0)
                flagR = true;
            else if (param.compare("-range") == 0)
            {
                flagRange = true;

                if ((i + 2) < params.size())
                {
                    std::istringstream(params[i + 1]) >> startFID;
                    std::istringstream(params[i + 2]) >> endFID;

                    i += 2;
                }
                else
                {
                    std::cout << "Param " << i << " \'-range\' requires more parameters." << std::endl;
                    return false;
                }
            }
            else
            {
                std::cout << "Param " << i << " \'" << param << "\' is invalid." << std::endl;
                return false;
            }
        }

        //
        SimHandle csim = GetCurrentHandle();

        const std::vector<SimInstruction>& history 
            = flagR ? csim->RenamedHistory : csim->FetchHistory;

        for (int i = startFID; (i < history.size()) && (!flagRange || i <= endFID); i++)
            __common_RAT0_DIFFSIM_INSN_DUMP(history[i], os);

        return true;
    }


    // rat0.diffsim.insn.dump [-R|-range <startFID> <endFID>]
    bool _RAT0_DIFFSIM_INSN_DUMP(void* handle, const std::string& cmd,
                                               const std::string& paramline,
                                               const std::vector<std::string>& params)
    {
        return __common_RAT0_DIFFSIM_INSN_DUMP_EX(0, params, std::cout);
    }


    // rat0.diffsim.insn.dumpfile <filename> [-R|-range <startFID> <endFID>]
    bool _RAT0_DIFFSIM_INSN_DUMPFILE(void* handle, const std::string& cmd,
                                                   const std::string& paramline,
                                                   const std::vector<std::string>& params)
    {
        if (params.empty())
        {
            std::cout << "Too much or too less parameter(s) for \'rat0.diffsim.insn.dumpfile\'." << std::endl;
            return false;
        }

        std::ofstream fos(params[0]);

        if (!fos)
        {
            std::cout << "Failed to open or create file." << std::endl;
            return false;
        }

        bool result = __common_RAT0_DIFFSIM_INSN_DUMP_EX(0, params, fos);

        fos.close();

        return result;
    }


    // rat0.diffsim.o3.fetch.dump
    bool _RAT0_DIFFSIM_O3_FETCH_DUMP(void* handle, const std::string& cmd,
                                                   const std::string& paramline,
                                                   const std::vector<std::string>& params)
    {
        if (!params.empty())
        {
            std::cout << "Too much or too less parameter(s) for \'rat0.diffsim.o3.fetch.dump\'." << std::endl;
            return false;
        }

        //
        SimHandle csim = GetCurrentHandle();

        //
        const std::list<SimInstruction>& insns = csim->O3Fetch.Get();
        
        std::list<SimInstruction>::const_iterator iter = insns.begin();
        for (; iter != insns.end(); iter++)
            __common_RAT0_DIFFSIM_INSN_DUMP(*iter, std::cout);

        return true;
    }


    // rat0.diffsim.o3.rob.dump
    bool _RAT0_DIFFSIM_O3_ROB_DUMP(void* handle, const std::string& cmd,
                                                 const std::string& paramline,
                                                 const std::vector<std::string>& params)
    {
        if (!params.empty())
        {
            std::cout << "Too much or too less parameter(s) for \'rat0.diffsim.o3.rob.dump\'." << std::endl;
            return false;
        }

        //
        SimHandle csim = GetCurrentHandle();

        //
        const std::list<SimReOrderBuffer::Entry>& entries = csim->O3ROB.Get();

        std::list<SimReOrderBuffer::Entry>::const_iterator iter = entries.begin();
        for (; iter != entries.end(); iter++)
            __common_RAT0_DIFFSIM_ROB_DUMP(*iter, std::cout);

        return true;
    }


    // rat0.diffsim.o3.reservation.dump
    bool _RAT0_DIFFSIM_O3_RESERVATION_DUMP(void* handle, const std::string& cmd,
                                                         const std::string& paramline,
                                                         const std::vector<std::string>& params)
    {
        if (!params.empty())
        {
            std::cout << "Too much or too less parameter(s) for \'rat0.diffsim.o3.reservation.dump\'." << std::endl;
            return false;
        }

        //
        SimHandle csim = GetCurrentHandle();

        //
        const std::list<SimReservation::Entry>& entries = csim->O3Reservation.Get();

        std::list<SimReservation::Entry>::const_iterator iter = entries.begin();
        for (; iter != entries.end(); iter++)
            __common_RAT0_DIFFSIM_RESERVATION_DUMP(*iter, std::cout);

        return true;
    }


    // rat0.diffsim.o3.scoreboard.dump
    bool _RAT0_DIFFSIM_O3_SCOREBOARD_DUMP(void* handle, const std::string& cmd,
                                                        const std::string& paramline,
                                                        const std::vector<std::string>& params)
    {
        if (!params.empty())
        {
            std::cout << "Too much or too less parameter(s) for \'rat0.diffsim.o3.scoreboard.dump\'." << std::endl;
            return false;
        }

        //
        SimHandle csim = GetCurrentHandle();

        //
        for (int i = 0; i < EMULATED_PRF_SIZE; i++)
        {
            if (csim->O3Scoreboard.IsBusy(i))
                __common_RAT0_DIFFSIM_SCOREBOARD_DUMP(csim, i);
        }

        return true;
    }



    void SetupCommands(VMCHandle handle)
    {
        RegisterCommand(handle, CommandHandler{ std::string("rat0.infobystep")                  , &_RAT0_INFOBYSTEP });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.arf0conv")                    , &_RAT0_ARF0CONV });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.rand.reg.value")              , &_RAT0_RAND_REG_VALUE });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.rand.reg.index")              , &_RAT0_RAND_REG_INDEX });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.prf.ls")                      , &_RAT0_PRF_LS });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.arf.ls.ref")                  , &_RAT0_ARF_LS_REF });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.arf.ls")                      , &_RAT0_ARF_LS});
        RegisterCommand(handle, CommandHandler{ std::string("rat0.arf.set")                     , &_RAT0_ARF_SET});
        RegisterCommand(handle, CommandHandler{ std::string("rat0.arf.set.randomval")           , &_RAT0_ARF_SET_RANDOMVAL });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.arf.set.random")              , &_RAT0_ARF_SET_RANDOM });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.arf.setall.random")           , &_RAT0_ARF_SETALL_RANDOM });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.arf.get")                     , &_RAT0_ARF_GET });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.diffsim.arf.set.random")      , &_RAT0_DIFFSIM_ARF_SET_RANDOM });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.diffsim.insn.push")           , &_RAT0_DIFFSIM_INSN_PUSH });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.diffsim.insn.push.random")    , &_RAT0_DIFFSIM_INSN_PUSH_RANDOM });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.diffsim.insn.eval.step")      , &_RAT0_DIFFSIM_INSN_EVAL_STEP });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.diffsim.insn.eval.stepout")   , &_RAT0_DIFFSIM_INSN_EVAL_STEPOUT });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.diffsim.insn.dump")           , &_RAT0_DIFFSIM_INSN_DUMP });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.diffsim.insn.dumpfile")       , &_RAT0_DIFFSIM_INSN_DUMPFILE });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.diffsim.o3.fetch.dump")       , &_RAT0_DIFFSIM_O3_FETCH_DUMP });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.diffsim.o3.rob.dump")         , &_RAT0_DIFFSIM_O3_ROB_DUMP });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.diffsim.o3.reservation.dump") , &_RAT0_DIFFSIM_O3_RESERVATION_DUMP });
        RegisterCommand(handle, CommandHandler{ std::string("rat0.diffsim.o3.scoreboard.dump")  , &_RAT0_DIFFSIM_O3_SCOREBOARD_DUMP });
    }
}


// class VMC::RAT::SimStageStatus
namespace VMC::RAT {
    /*
    const std::string str;
    */

    SimStageStatus::SimStageStatus(const char* str)
        : str(std::string(str))
    { }

    SimStageStatus::SimStageStatus(const std::string str)
        : str(str)
    { }

    SimStageStatus::~SimStageStatus()
    { }

    const std::string& SimStageStatus::GetString() const
    {
        return str;
    }
};


// class VMC::RAT::SimRefPipeStatus
namespace VMC::RAT {
    /*
    const SimStageStatus*   fetchStatus;
    const SimStageStatus*   reservationStatus;
    const SimStageStatus*   executionStatus;
    */

    SimRefPipeStatus::SimRefPipeStatus()
        : fetchStatus       (&STAGE_STATUS_IDLE)
        , reservationStatus (&STAGE_STATUS_IDLE)
        , executionStatus   (&STAGE_STATUS_IDLE)
    { }

    SimRefPipeStatus::~SimRefPipeStatus()
    { }

    inline const SimStageStatus* SimRefPipeStatus::GetFetchStatus() const
    {
        return fetchStatus;
    }

    inline const SimStageStatus* SimRefPipeStatus::GetReservationStatus() const
    {
        return reservationStatus;
    }

    inline const SimStageStatus* SimRefPipeStatus::GetExecutionStatus() const
    {
        return executionStatus;
    }

    inline void SimRefPipeStatus::SetFetchStatus(const SimStageStatus* status)
    {
        this->fetchStatus = status;
    }

    inline void SimRefPipeStatus::SetReservationStatus(const SimStageStatus* status)
    {
        this->reservationStatus = status;
    }

    inline void SimRefPipeStatus::SetExecutionStatus(const SimStageStatus* status)
    {
        this->executionStatus = status;
    }
};


// class VMC::RAT::SimO3PipeStatus
namespace VMC::RAT {
    /*
    const SimStageStatus*   fetchStatus;
    const SimStageStatus*   reservationStatus;
    const SimStageStatus*   executionStatus;
    const SimStageStatus*   robStatus;
    */

    SimO3PipeStatus::SimO3PipeStatus()
        : fetchStatus       (&STAGE_STATUS_IDLE)
        , reservationStatus (&STAGE_STATUS_IDLE)
        , executionStatus   (&STAGE_STATUS_IDLE)
        , robStatus         (&STAGE_STATUS_IDLE)
    { }

    SimO3PipeStatus::~SimO3PipeStatus()
    { }

    inline const SimStageStatus* SimO3PipeStatus::GetFetchStatus() const
    {
        return fetchStatus;
    }

    inline const SimStageStatus* SimO3PipeStatus::GetReservationStatus() const
    {
        return reservationStatus;
    }

    inline const SimStageStatus* SimO3PipeStatus::GetExecutionStatus() const
    {
        return executionStatus;
    }

    inline const SimStageStatus* SimO3PipeStatus::GetROBStatus() const
    {
        return robStatus;
    }

    inline void SimO3PipeStatus::SetFetchStatus(const SimStageStatus* status)
    {
        this->fetchStatus = status;
    }

    inline void SimO3PipeStatus::SetReservationStatus(const SimStageStatus* status)
    {
        this->reservationStatus = status;
    }

    inline void SimO3PipeStatus::SetExecutionStatus(const SimStageStatus* status)
    {
        this->executionStatus = status;
    }

    inline void SimO3PipeStatus::SetROBStatus(const SimStageStatus* status)
    {
        this->robStatus = status;
    }
};


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
        if (index == -1)
            return 0;

        return refARF[index];
    }
}


// class VMC::RAT::SimO3ARF
namespace VMC::RAT {
    /*
    PhysicalRegisterFile*   PRF;
    */

    SimO3ARF::SimO3ARF(PhysicalRegisterFile* PRF)
        : PRF   (PRF)
    { }

    SimO3ARF::SimO3ARF(const SimO3ARF& obj)
        : PRF   (obj.PRF)
    { }

    uint64_t SimO3ARF::operator[](const int index) const
    {
        if (index == -1)
            return 0;

        return PRF->Get(index);
    }
}


// class VMC::RAT::SimInstruction
namespace VMC::RAT {
    /*
    int         FID;
    int         clkDelay;
    int         dst;
    int         src1;
    int         src2;
    int         insncode;
    uint64_t    imm;
    */

    SimInstruction::SimInstruction()
        : FID       (-1)
        , delay     (0)
        , dst       (0)
        , src1      (0)
        , src2      (0)
        , insncode  (INSN_CODE_NOP)
        , imm       (0)
    { }

    SimInstruction::SimInstruction(int FID, int delay, int insncode, int dst)
        : FID       (FID)
        , delay     (delay)
        , dst       (dst)
        , src1      (0)
        , src2      (0)
        , insncode  (insncode)
        , imm       (0)
    { }

    SimInstruction::SimInstruction(int FID, int delay, int insncode, int dst, uint64_t imm)
        : FID       (FID)
        , delay     (delay)
        , dst       (dst)
        , src1      (0)
        , src2      (0)
        , insncode  (insncode)
        , imm       (imm)
    { }

    SimInstruction::SimInstruction(int FID, int delay, int insncode, int dst, int src1, int src2)
        : FID       (FID)
        , delay     (delay)
        , dst       (dst)
        , src1      (src1)
        , src2      (src2)
        , insncode  (insncode)
        , imm       (0)
    { }

    SimInstruction::SimInstruction(int FID, int delay, int insncode, int dst, int src1, int src2, uint64_t imm)
        : FID       (FID)
        , delay     (delay)
        , dst       (dst)
        , src1      (src1)
        , src2      (src2)
        , insncode  (insncode)
        , imm       (imm)
    { }

    SimInstruction::SimInstruction(const SimInstruction& obj)
        : FID       (obj.FID)
        , delay     (obj.delay)
        , dst       (obj.dst)
        , src1      (obj.src1)
        , src2      (obj.src2)
        , insncode  (obj.insncode)
        , imm       (obj.imm)
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

    inline uint64_t SimInstruction::GetImmediate() const
    {
        return imm;
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

    inline void SimInstruction::SetImmediate(uint64_t imm)
    {
        this->imm = imm;
    }
}


// class VMC::RAT::SimFetch
namespace VMC::RAT {
    /*
    std::list<SimInstruction>   fetched;
    */

    SimFetch::SimFetch()
        : fetched(std::list<SimInstruction>())
    { }

    SimFetch::~SimFetch()
    { }

    inline const std::list<SimInstruction>& SimFetch::Get() const
    {
        return fetched;
    }

    inline int SimFetch::GetCount() const
    {
        return fetched.size();
    }

    inline bool SimFetch::IsEmpty() const
    {
        return fetched.empty();
    }

    inline void SimFetch::Clear()
    {
        fetched.clear();
    }

    inline void SimFetch::PushInsn(const SimInstruction& insn)
    {
        fetched.push_back(insn);
    }

    inline bool SimFetch::NextInsn(SimInstruction* insn)
    {
        if (fetched.empty())
            return false;

        if (insn)
            *insn = fetched.front();

        return true;
    }

    inline bool SimFetch::PopInsn()
    {
        if (fetched.empty())
            return false;

        fetched.pop_front();

        return true;
    }
}


// class VMC::RAT::SimScoreboard::Modification
namespace VMC::RAT {
    /*
    int index;
    int status;
    int FID;
    */

    SimScoreboard::Modification::Modification()
        : index     (-1)
        , status    (SCOREBOARD_STATUS_IN_ARF)
        , FID       (-1)
    { }

    SimScoreboard::Modification::Modification(int index, int status, int FID)
        : index     (index)
        , status    (status)
        , FID       (FID)
    { }

    SimScoreboard::Modification::~Modification()
    { }

    inline void SimScoreboard::Modification::SetIndex(int index)
    {
        this->index = index;
    }

    inline void SimScoreboard::Modification::SetStatus(int status)
    {
        this->status = status;
    }

    inline void SimScoreboard::Modification::SetFID(int FID)
    {
        this->FID = FID;
    }

    inline int SimScoreboard::Modification::GetIndex() const
    {
        return index;
    }

    inline int SimScoreboard::Modification::GetStatus() const
    {
        return status;
    }

    inline int SimScoreboard::Modification::GetFID() const
    {
        return FID;
    }
}


// class VMC::RAT::SimScoreboard
namespace VMC::RAT {
    /*
    const int               size;

    std::list<Modification> modification;

    int*              const status;
    int*              const FID;
    */

    SimScoreboard::SimScoreboard(int size)
        : size          (size)
        , modification  (std::list<Modification>())
        , status        (new int[size]())
        , FID           (new int[size])
    { }

    SimScoreboard::SimScoreboard(const SimScoreboard& obj)
        : size          (obj.size)
        , modification  (obj.modification)
        , status        (new int[obj.size]())
        , FID           (new int[obj.size])
    {
        memcpy(status, obj.status, obj.size * sizeof(int));
        memcpy(FID,    obj.FID,    obj.size * sizeof(int));
    }

    SimScoreboard::~SimScoreboard()
    {
        delete status;
        delete FID;
    }

    inline void SimScoreboard::Clear()
    {
        memset(status, 0, size * sizeof(int));
        modification.clear();
    }

    inline bool SimScoreboard::IsBusy(int index) const
    {
        if (index == -1)
            return false;

        return status[index] == SCOREBOARD_STATUS_BUSY;
    }

    inline int SimScoreboard::GetFID(int index) const
    {
        if (index == -1)
            return -1;

        return FID[index];
    }

    inline int SimScoreboard::GetStatus(int index) const
    {
        if (index == -1)
            return SCOREBOARD_STATUS_IN_ARF;

        return status[index];
    }

    inline void SimScoreboard::SetStatus(int index, int status, int FID)
    {
        if (index == -1)
            return;

        modification.push_back(Modification(index, status, FID));
    }

    inline void SimScoreboard::ResetInput()
    {
        modification.clear();
    }

    void SimScoreboard::Eval()
    {
        std::list<Modification>::iterator iter = modification.begin();
        for (; iter != modification.end(); iter++)
        {
            status[iter->GetIndex()] = iter->GetStatus();

            if (iter->GetFID() != -1)
                FID[iter->GetIndex()] = iter->GetFID();
        }

        ResetInput();
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
        , dstrdy    (false)
    { }

    SimReservation::Entry::Entry(const SimInstruction& insn, bool src1rdy, bool src2rdy, bool dstrdy)
        : insn      (insn)
        , src1rdy   (src1rdy)
        , src2rdy   (src2rdy)
        , dstrdy    (dstrdy)
    { }

    SimReservation::Entry::Entry(const Entry& obj)
        : insn      (obj.insn)
        , src1rdy   (obj.src1rdy)
        , src2rdy   (obj.src2rdy)
        , dstrdy    (obj.dstrdy)
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

    inline bool SimReservation::Entry::IsDstReady() const
    {
        return dstrdy;
    }

    inline bool SimReservation::Entry::IsReady() const
    {
        return src1rdy && src2rdy && dstrdy;
    }

    inline int SimReservation::Entry::GetSrc1() const
    {
        return insn.GetSrc1();
    }

    inline int SimReservation::Entry::GetSrc2() const
    {
        return insn.GetSrc2();
    }

    inline int SimReservation::Entry::GetDst() const
    {
        return insn.GetDst();
    }

    inline void SimReservation::Entry::SetSrc1Ready(bool rdy)
    {
        src1rdy = rdy;
    }

    inline void SimReservation::Entry::SetSrc2Ready(bool rdy)
    {
        src2rdy = rdy;
    }

    inline void SimReservation::Entry::SetDstReady(bool rdy)
    {
        dstrdy = rdy;
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
        , next      (entries.end())
    { }

    SimReservation::SimReservation(const SimReservation& obj)
        : scoreboard(obj.scoreboard)
        , entries   (obj.entries)
        , next      (entries.end())
    { }

    SimReservation::~SimReservation()
    { }

    inline const std::list<SimReservation::Entry>& SimReservation::Get() const
    {
        return entries;
    }

    void SimReservation::PushInsn(const SimInstruction& insn)
    {
        Entry entry = Entry(insn);
        /*
        entry.SetSrc1Ready(insn.GetSrc1() ? !scoreboard->IsBusy(insn.GetSrc1()) : true);
        entry.SetSrc2Ready(insn.GetSrc2() ? !scoreboard->IsBusy(insn.GetSrc2()) : true);
        entry.SetDstReady( insn.GetDst()  ? !scoreboard->IsBusy(insn.GetDst())  : true);
        */

        entries.push_back(entry);
    }

    inline const SimScoreboard* SimReservation::GetScoreboardRef() const
    {
        return scoreboard;
    }

    inline int SimReservation::GetCount() const
    {
        return entries.size();
    }

    inline bool SimReservation::IsEmpty() const
    {
        return entries.empty();
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
            {
                iter++;
                continue;
            }

            if (!iter->IsSrc1Ready() && !scoreboard->IsBusy(iter->GetSrc1()))
                iter->SetSrc1Ready();

            if (!iter->IsSrc2Ready() && !scoreboard->IsBusy(iter->GetSrc2()))
                iter->SetSrc2Ready();

            if (!iter->IsDstReady() && !scoreboard->IsBusy(iter->GetDst()))
                iter->SetDstReady();

            iter++;
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

    SimExecution::Entry::Entry()
        : insn      ()
        , src1val   (0)
        , src2val   (0)
        , delay     (0)
        , dstrdy    (false)
        , dstval    (0)
    { }

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

                case INSN_CODE_ANDI:
                    dstval = src1val & insn.GetImmediate();
                    break;

                case INSN_CODE_ORI:
                    dstval = src1val | insn.GetImmediate();
                    break;

                case INSN_CODE_XORI:
                    dstval = src1val ^ insn.GetImmediate();
                    break;

                case INSN_CODE_ADDI:
                    dstval = src1val + insn.GetImmediate();
                    break;

                case INSN_CODE_SUBI:
                    dstval = src1val - insn.GetImmediate();
                    break;

                default:
                    printf("Invalid insncode: %d (0x%08x)\n", insn.GetInsnCode(), insn.GetInsnCode());
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
    SimO3ARF*               const o3ARF;
    list<Entry>             entries;
    list<Entry>::iterator   next;
    */

    SimExecution::SimExecution(uint64_t* ARF)
        : ref       (true)
        , refARF    (new SimRefARF(ARF))
        , o3ARF     (nullptr)
        , entries   (list<Entry>())
        , next      (entries.end())
    { }

    SimExecution::SimExecution(PhysicalRegisterFile* PRF)
        : ref       (false)
        , refARF    (nullptr)
        , o3ARF     (new SimO3ARF(PRF))
        , entries   (list<Entry>())
        , next      (entries.end())
    { }

    SimExecution::SimExecution(const SimExecution& obj)
        : ref       (obj.ref)
        , refARF    ( ref ? new SimRefARF(*refARF) : nullptr)
        , o3ARF     (!ref ? new SimO3ARF(*o3ARF) : nullptr)
        , entries   (obj.entries)
        , next      (entries.end())
    { }

    SimExecution::~SimExecution()
    {
        if (ref)
            delete refARF;
        else
            delete o3ARF;
    }

    inline const std::list<SimExecution::Entry>& SimExecution::Get() const
    {
        return entries;
    }

    inline int SimExecution::GetCount() const
    {
        return entries.size();
    }

    inline bool SimExecution::IsEmpty() const
    {
        return entries.empty();
    }

    void SimExecution::PushInsn(const SimInstruction& insn, Entry* entry)
    {
        uint64_t src1val = ref ? (*refARF)[insn.GetSrc1()] : (*o3ARF)[insn.GetSrc1()];
        uint64_t src2val = ref ? (*refARF)[insn.GetSrc2()] : (*o3ARF)[insn.GetSrc2()];

        Entry newEntry(insn, src1val, src2val);

        if (entry)
            *entry = newEntry;

        entries.push_back(newEntry);
    }

    void SimExecution::PushInsnEx(const SimInstruction& insn, uint64_t src1val, uint64_t src2val, Entry* entry)
    {
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

    SimReOrderBuffer::Entry::Entry()
        : insn      ()
        , rdy       (false)
        , dstval    (0)
    { }

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
    { }

    SimReOrderBuffer::SimReOrderBuffer(const SimReOrderBuffer& obj)
        : entries   (obj.entries)
    { }

    SimReOrderBuffer::~SimReOrderBuffer()
    { }

    inline const std::list<SimReOrderBuffer::Entry>& SimReOrderBuffer::Get() const
    {
        return entries;
    }

    inline int SimReOrderBuffer::GetCount() const
    {
        return entries.size();
    }

    inline bool SimReOrderBuffer::IsEmpty() const
    {
        return entries.empty();
    }

    bool SimReOrderBuffer::GetDstValue(int FID, uint64_t* dst) const
    {
        std::list<Entry>::const_iterator iter = entries.begin();
        for(; iter != entries.end(); iter++)
        {
            if (iter->IsReady() && iter->GetFID() == FID)
            {
                *dst = iter->GetDstValue();

                return true;
            }
        }

        return false;
    }

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
        if (entries.front().IsReady())
        {
            if (entry)
                *entry = entries.front();

            return true;
        }

        return false;
    }

    bool SimReOrderBuffer::PopInsn()
    {
        if (entries.empty())
            return false;

        entries.pop_front();

        return true;
    }
}
