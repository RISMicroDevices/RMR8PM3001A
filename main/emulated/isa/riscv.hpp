#pragma once
//
// RISC-V Instruction Set Architecture 
//
// (Jasse: Just Another Simple Single-cycle Emulator)
//

#include <cstdint>
#include <stdexcept>
#include <string>
#include <iomanip>
#include <sstream>
#include <list>
#include <vector>
#include <initializer_list>
#include <memory>
#include <algorithm>

#include "jasse.hpp"

#include "base/riscvdef.hpp"
#include "base/riscvooc.hpp"
#include "base/riscvcode.hpp"
#include "base/riscvcodeset.hpp"
#include "base/riscvdecode.hpp"
#include "base/riscvencode.hpp"
#include "base/riscvgen.hpp"
#include "base/riscvmem.hpp"
#include "base/riscvcsr.hpp"
#include "base/riscvtrap.hpp"


namespace Jasse {
    
    // RISC-V General Registers container template
    template<class TArch>
    class RVGeneralRegisters : public RVGeneralRegisterSetOOC<TArch> {
    private:
        static constexpr int    SIZE    = RV_ARCH_REG_COUNT;

        TArch*  registers;

    public:
        RVGeneralRegisters() noexcept;
        RVGeneralRegisters(const RVGeneralRegisters<TArch>& obj) noexcept;
        ~RVGeneralRegisters() noexcept;

        virtual int     GetSize() const noexcept override;
        virtual bool    CheckBound(int address) const noexcept override;

        virtual TArch   Get(int address) const override;
        virtual void    Set(int address, TArch value) override;

        void    operator=(const RVGeneralRegisters<TArch>& obj);
    };

    // RISC-V General Registers container of XLEN=32
    using RVGeneralRegisters32  = RVGeneralRegisters<arch32_t>;

    // RISC-V General Registers container of XLEN=64
    using RVGeneralRegisters64  = RVGeneralRegisters<arch64_t>;
    

    // RISC-V Architectural State Container
    class RVArchitectural : public RVArchitecturalOOC {
    private:
        const XLen              _XLEN;

        pc_t                    _PC;

        RVGeneralRegisters32*   _GR32;

        RVGeneralRegisters64*   _GR64;

        // TODO ... More architectural states could be appended here ...

    public:
        RVArchitectural() = delete;
        RVArchitectural(XLen XLEN);
        RVArchitectural(const RVArchitectural& obj);
        ~RVArchitectural();

        virtual XLen                            XLEN() const noexcept override;

        virtual pc_t                            PC() const noexcept override;
        virtual void                            SetPC(pc_t pc) noexcept override;
        virtual void                            SetPC32(arch32_t pc) override;
        virtual void                            SetPC64(arch64_t pc) override;

        virtual bool                            IsGR32() const noexcept override;
        virtual bool                            IsGR64() const noexcept override;

        virtual RVGeneralRegisters32*           GR32() override;
        virtual const RVGeneralRegisters32*     GR32() const override;
        virtual RVGeneralRegisters64*           GR64() override;
        virtual const RVGeneralRegisters64*     GR64() const override;

        virtual arch64_t                        GetGRx64Zext(int addr) const noexcept override;
        virtual arch64_t                        GetGRx64Sext(int addr) const noexcept override;
        virtual arch32_t                        GetGRx32(int addr) const noexcept override;

        virtual void                            SetGRx64(int addr, arch64_t val64) noexcept override;
        virtual void                            SetGRx32Zext(int addr, arch32_t val32) noexcept override;
        virtual void                            SetGRx32Sext(int addr, arch32_t val32) noexcept override;

        // TODO ... More architectural states could be appended here ...
    };


    //
    class RVInstance;

    // RISC-V Instance Exception (Special Exec Status) Handler Status
    typedef enum {
        EEI_BYPASS = 0,
        EEI_HANDLED
    } RVEEIStatus;

    // RISC-V Instance Exception (Special Exec Status) Handler # EEI-defined interface
    // *NOTICE: RVInstruction* parameter might be NULL, depending on whether the instruction
    //          was properly fetched or decoded.
    typedef     RVEEIStatus     (*RVExecEEIHandler)(RVInstance&, RVExecStatus, RVInstruction*);

    // RISC-V Instance
    class RVInstance {
    public:
        class Builder;

    private:
        RVDecoderCollection     decoders;

        RVArchitectural         arch;

        RVMemoryInterface*      MI;

        RVCSRSpace              CSRs;

        RVTrapProcedures        trap_procedures;

        RVExecEEIHandler        exec_handler;

    public:
        RVInstance(const RVDecoderCollection&   decoders,
                   RVArchitectural&&            arch,
                   RVMemoryInterface*           MI,
                   const RVCSRList&             CSRs,
                   const RVTrapProcedures&      trap_procedures,
                   RVExecEEIHandler             exec_handler) noexcept;

        RVInstance() = delete;
        RVInstance(const RVInstance& obj) = delete;
        
        ~RVInstance() noexcept;

        RVDecoderCollection&            GetDecoders() noexcept;
        const RVDecoderCollection&      GetDecoders() const noexcept;

        RVArchitectural&                GetArch() noexcept;
        const RVArchitectural&          GetArch() const noexcept;

        RVMemoryInterface*              GetMI() noexcept;
        const RVMemoryInterface*        GetMI() const noexcept;
        void                            SetMI(RVMemoryInterface* MI) noexcept;

        RVCSRSpace&                     GetCSRs() noexcept;
        const RVCSRSpace&               GetCSRs() const noexcept;

        RVExecEEIHandler&               GetExecEEI() noexcept;
        RVExecEEIHandler                GetExecEEI() const noexcept;
        void                            SetExecEEI(RVExecEEIHandler exec_handler) noexcept;

        RVTrapProcedures&               GetTrapProcedures() noexcept;
        const RVTrapProcedures&         GetTrapProcedures() const noexcept;
        void                            SetTrapProcedures(const RVTrapProcedures& trap_procedures) noexcept;

        void                            Interrupt(RVTrapCause cause);

        RVExecStatus                    Eval();

        void    operator=(const RVInstance& obj) = delete;
    };

    class RVInstance::Builder {
    private:
        XLen                    xlen;
        pc_t                    startup_pc;

        RVMemoryInterface*      _MI;
            
        RVCSRList               _CSR;        // inner-constructed

        RVDecoderCollection     _decoders;   // inner-constructed

        RVGeneralRegisters64    _GR;         // inner-constructed, transient

        RVTrapProcedures        trap_procedures;

        RVExecEEIHandler        exec_handler;
        
    public:
        Builder() noexcept;
        ~Builder() noexcept;

        Builder&                    XLEN(XLen xlen) noexcept;

        Builder&                    StartupPC(pc_t pc) noexcept;
        Builder&                    StartupPC32(arch32_t pc) noexcept;
        Builder&                    StartupPC64(arch64_t pc) noexcept;

        Builder&                    GR32(int address, arch32_t init_value) noexcept;
        Builder&                    GR64(int address, arch64_t init_value) noexcept;

        Builder&                    Decoder(const RVDecoder* decoder) noexcept;
        Builder&                    Decoder(std::initializer_list<const RVDecoder*> decoders) noexcept;
        Builder&                    Decoder(const RVDecoderCollection& decoders) noexcept;

        Builder&                    MI(RVMemoryInterface* MI) noexcept;

        Builder&                    CSR(const RVCSRDefinition& CSR) noexcept;
        Builder&                    CSR(const RVCSRDefinition& CSR, csr_t init_value) noexcept;
        Builder&                    CSR(std::initializer_list<RVCSRDefinition> CSRs) noexcept;
        Builder&                    CSR(const RVCSRList& CSRs) noexcept;

        Builder&                    TrapProcedures(const RVTrapProcedures& trap_procedures) noexcept;

        Builder&                    ExecEEI(RVExecEEIHandler exec_handler) noexcept;

        arch32_t                    GR32(int address) const noexcept;
        arch64_t                    GR64(int address) const noexcept;
        RVCSRList&                  CSR() noexcept;
        const RVCSRList&            CSR() const noexcept;
        RVDecoderCollection&        Decoder() noexcept;
        const RVDecoderCollection&  Decoder() const noexcept;
        RVTrapProcedures&           TrapProcedures() noexcept;
        const RVTrapProcedures&     TrapProcedures() const noexcept;
        RVExecEEIHandler&           ExecEEI() noexcept;
        RVExecEEIHandler            ExecEEI() const noexcept;

        RVInstance*                 Build() const noexcept;
    };
}



// Implementation of: class RVGeneralRegisters<class TArch>
namespace Jasse {
    /*
    TArch*  registers;
    */

    template<class TArch>
    RVGeneralRegisters<TArch>::RVGeneralRegisters() noexcept
        : registers (new TArch[SIZE]())
    { }

    template<class TArch>
    RVGeneralRegisters<TArch>::RVGeneralRegisters(const RVGeneralRegisters<TArch>& obj) noexcept
        : registers (new TArch[SIZE]())
    { 
        // should be replaced as "memcpy", but suppressing warnings
        for (int i = 0; i < SIZE; i++)
            registers[i] = obj.registers[i];
    }

    template<class TArch>
    RVGeneralRegisters<TArch>::~RVGeneralRegisters() noexcept
    {
        delete[] registers;
    }

    template<class TArch>
    inline int RVGeneralRegisters<TArch>::GetSize() const noexcept
    {
        return SIZE;
    }

    template<class TArch>
    inline bool RVGeneralRegisters<TArch>::CheckBound(int address) const noexcept
    {
        return address >= 0 && address < SIZE;
    }

    template<class TArch>
    inline TArch RVGeneralRegisters<TArch>::Get(int address) const
    {
        return !address ? 0 : registers[address];
    }

    template<class TArch>
    inline void RVGeneralRegisters<TArch>::Set(int address, TArch value)
    {
        registers[address] = value;
    }

    template<class TArch>
    void RVGeneralRegisters<TArch>::operator=(const RVGeneralRegisters<TArch>& obj)
    {
        std::copy(obj.registers, obj.registers + SIZE, registers);
    }
}


// Implementation of: class RVArchitectural
namespace Jasse {
    /*
    const XLen              _XLEN;

    pc_t                    _PC;

    RVGeneralRegisters32*   _GR32;

    RVGeneralRegisters64*   _GR64;
    */

    RVArchitectural::RVArchitectural(XLen XLEN)
        : _XLEN (XLEN)
        , _PC   (pc_t())
    {
        switch (XLEN)
        {
        case XLEN32:
            _GR32 = new RVGeneralRegisters32();
            _GR64 = nullptr;
            break;

        case XLEN64:
            _GR32 = nullptr;
            _GR64 = new RVGeneralRegisters64();
            break;

        [[unlikely]] default:
            _GR32 = nullptr;
            _GR64 = nullptr;
        }
    }

    RVArchitectural::RVArchitectural(const RVArchitectural& obj)
        : _XLEN (obj._XLEN)
        , _PC   (obj._PC)
    {
        _GR32 = obj._GR32 ? new RVGeneralRegisters32(*obj._GR32) : nullptr;
        _GR64 = obj._GR64 ? new RVGeneralRegisters64(*obj._GR64) : nullptr;
    }

    RVArchitectural::~RVArchitectural()
    {
        if (_GR32)
            delete _GR32;

        if (_GR64)
            delete _GR64;
    }

    XLen RVArchitectural::XLEN() const noexcept
    {
        return _XLEN;
    }

    pc_t RVArchitectural::PC() const noexcept
    {
        return _PC;
    }

    void RVArchitectural::SetPC(pc_t pc) noexcept
    {
        _PC = pc;
    }

    void RVArchitectural::SetPC32(arch32_t pc)
    {
        [[unlikely]] if (_XLEN != XLEN32)
            throw std::logic_error("set 32-bit PC in non-32-XLEN arch");

        _PC.pc32 = pc;
    }

    void RVArchitectural::SetPC64(arch64_t pc)
    {
        [[unlikely]] if (_XLEN != XLEN64)
            throw std::logic_error("set 64-bit PC in non-64-XLEN arch");

        _PC.pc64 = pc;
    }

    bool RVArchitectural::IsGR32() const noexcept
    {
        return _GR32;
    }

    bool RVArchitectural::IsGR64() const noexcept
    {
        return _GR64;
    }

    const RVGeneralRegisters32* RVArchitectural::GR32() const
    {
        [[unlikely]] if (!_GR32)
            throw std::logic_error("non-GR32 arch");

        return _GR32;
    }

    RVGeneralRegisters32* RVArchitectural::GR32()
    {
        [[unlikely]] if (!_GR32)
            throw std::logic_error("non-GR32 arch");

        return _GR32;
    }

    const RVGeneralRegisters64* RVArchitectural::GR64() const
    {
        [[unlikely]] if (!_GR64)
            throw std::logic_error("non-GR64 arch");
        
        return _GR64;
    }

    RVGeneralRegisters64* RVArchitectural::GR64()
    {
        [[unlikely]] if (!_GR64)
            throw std::logic_error("non-GR64 arch");
        
        return _GR64;
    }

    // *NOTICE: Get 64-bit value from General Register.
    //          If XLEN is less than 64, the value read is zero-extended,
    //          and wouldn't raise any exception.
    arch64_t RVArchitectural::GetGRx64Zext(int addr) const noexcept
    {
        if (_GR64)
            return _GR64->Get(addr);
        else if (_GR32)
            return ZEXT_W(_GR32->Get(addr));
        else [[unlikely]] 
            return 0;
    }

    // *NOTICE: Get 64-bit value from General Register.
    //          If XLEN is less than 64, the value read is sign-extended,
    //          and wouldn't raise any exception.
    arch64_t RVArchitectural::GetGRx64Sext(int addr) const noexcept
    {
        if (_GR64)
            return _GR64->Get(addr);
        else if (_GR32)
            return SEXT_W(_GR32->Get(addr));
        else [[unlikely]] 
            return 0;
    }

    // *NOTICE: Get 32-bit value from General Register.
    //          If XLEN is greater than 32, the value read is truncated,
    //          and wouldn't raise any exception.
    arch32_t RVArchitectural::GetGRx32(int addr) const noexcept
    {
        if (_GR64)
            return (arch32_t) _GR64->Get(addr);
        else if (_GR32)
            return _GR32->Get(addr);
        else [[unlikely]] 
            return 0;
    }

    // *NOTICE: Set General Register with 64-bit value.
    //          If XLEN is less than 64, the value passed through is truncated,
    //          and wouldn't raise any exception.
    void RVArchitectural::SetGRx64(int addr, arch64_t val64) noexcept
    {
        if (_GR64)
            _GR64->Set(addr, val64);
        else if (_GR32)
            _GR32->Set(addr, (arch32_t) val64); // truncate
    }

    // *NOTICE: Set General Register with 32-bit value.
    //          If XLEN is greater than 32, the value passed through is zero-extended,
    //          and wouldn't raise any exception.
    void RVArchitectural::SetGRx32Zext(int addr, arch32_t val32) noexcept
    {
        if (_GR64)
            _GR64->Set(addr, ZEXT_W(val32));
        else if (_GR32)
            _GR32->Set(addr, val32);
    }

    // *NOTICE: Set General Register with 32-bit value.
    //          If XLEN is greater than 32, the value passed through is sign-extended,
    //          and wouldn't raise any exception.
    void RVArchitectural::SetGRx32Sext(int addr, arch32_t val32) noexcept
    {
        if (_GR64)
            _GR64->Set(addr, SEXT_W(val32));
        else if (_GR32)
            _GR32->Set(addr, val32);
    }
}


// Implementation of: class RVInstance
namespace Jasse {
    /*
    RVDecoderCollection     decoders;

    RVArchitectural         arch;
    
    RVMemoryInterface*      MI;

    RVCSRSpace              CSRs;

    RVExecEEIHandler        exec_handler;

    RVTrapProcedures        trap;
    */

    RVInstance::RVInstance(const RVDecoderCollection&   decoders,
                           RVArchitectural&&            arch,
                           RVMemoryInterface*           MI,
                           const RVCSRList&             CSRs,
                           const RVTrapProcedures&      trap_procedures,
                           RVExecEEIHandler             exec_handler) noexcept
        : decoders          (decoders)
        , arch              (arch)
        , MI                (MI)
        , CSRs              (CSRs)
        , trap_procedures   (trap_procedures)
        , exec_handler      (exec_handler)
    { }

    RVInstance::~RVInstance() noexcept
    { }

    inline RVDecoderCollection& RVInstance::GetDecoders() noexcept
    {
        return decoders;
    }

    inline const RVDecoderCollection& RVInstance::GetDecoders() const noexcept
    {
        return decoders;
    }

    inline RVArchitectural& RVInstance::GetArch() noexcept
    {
        return arch;
    }

    inline const RVArchitectural& RVInstance::GetArch() const noexcept
    {
        return arch;
    }

    inline RVMemoryInterface* RVInstance::GetMI() noexcept
    {
        return MI;
    }

    inline const RVMemoryInterface* RVInstance::GetMI() const noexcept
    {
        return MI;
    }

    inline void RVInstance::SetMI(RVMemoryInterface* MI) noexcept
    {
        this->MI = MI;
    }

    inline RVCSRSpace& RVInstance::GetCSRs() noexcept
    {
        return CSRs;
    }

    inline const RVCSRSpace& RVInstance::GetCSRs() const noexcept
    {
        return CSRs;
    }

    inline RVExecEEIHandler& RVInstance::GetExecEEI() noexcept
    {
        return exec_handler;
    }

    inline RVExecEEIHandler RVInstance::GetExecEEI() const noexcept
    {
        return exec_handler;
    }

    inline void RVInstance::SetExecEEI(RVExecEEIHandler exec_handler) noexcept
    {
        this->exec_handler = exec_handler;
    }

    inline RVTrapProcedures& RVInstance::GetTrapProcedures() noexcept
    {
        return trap_procedures;
    }

    inline const RVTrapProcedures& RVInstance::GetTrapProcedures() const noexcept
    {
        return trap_procedures;
    }

    inline void RVInstance::SetTrapProcedures(const RVTrapProcedures& trap_procedures) noexcept
    {
        this->trap_procedures = trap_procedures;
    }

    inline void RVInstance::Interrupt(RVTrapCause cause)
    {
        trap_procedures.TrapEnter(&arch, &CSRs, TRAP_INTERRUPT, cause);
    }

    RVExecStatus RVInstance::Eval()
    {
        // instruction fetch
        data_t fetched;
        RVMOPStatus mopstatus;

        if (arch.XLEN() == XLEN32) // XLEN=32
            mopstatus = MI->ReadInsn(arch.PC().pc32, MOPW_WORD, &fetched);
        else // XLEN=64
            mopstatus = MI->ReadInsn(arch.PC().pc64, MOPW_WORD, &fetched);

        if (mopstatus != MOP_SUCCESS)
        {
            RVExecStatus rstatus;

            switch (mopstatus)
            {
                case MOP_ACCESS_FAULT:
                    rstatus = EXEC_FETCH_ACCESS_FAULT;
                    break;

                case MOP_ADDRESS_MISALIGNED:
                    rstatus = EXEC_FETCH_ADDRESS_MISALIGNED;
                    break;

                [[unlikely]] default:
                    SHOULD_NOT_REACH_HERE;
            }

            if (exec_handler)
                exec_handler(*this, rstatus, nullptr);

            return rstatus;
        }

        //
        RVExecContext ctx { &arch, MI, &CSRs, trap_procedures };

        // instruction decode
        RVInstruction decoded;

        if (!decoders.Decode(fetched.data32, decoded))
        {
            if (exec_handler)
                exec_handler(*this, EXEC_NOT_DECODED, nullptr);

            return EXEC_NOT_DECODED;
        }

        // execution
        RVExecStatus exec_status = decoded.Execute(ctx);
        RVEEIStatus  eei_status  = EEI_BYPASS;

        // - note: @see RVExecStatus
        ASSERT(exec_status != EXEC_FETCH_ACCESS_FAULT);
        ASSERT(exec_status != EXEC_FETCH_ADDRESS_MISALIGNED);
        ASSERT(exec_status != EXEC_NOT_DECODED);

        if (exec_handler)
            eei_status = exec_handler(*this, exec_status, &decoded);

        if (eei_status == EEI_BYPASS)
        {
            if (exec_status == EXEC_SEQUENTIAL)
            {
                if (arch.XLEN() == XLEN32) // XLEN=32
                    arch.SetPC32(arch.PC().pc32 + 4);
                else // XLEN=64
                    arch.SetPC64(arch.PC().pc64 + 4);
            }

            // ...
        }

        return exec_status;
    }
}


// Implementation of: class RVInstance::Builder
namespace Jasse {
    /*
    XLen                    xlen;
    pc_t                    startup_pc;

    RVMemoryInterface*      _MI;
            
    RVCSRList               _CSR;        // inner-constructed

    RVDecoderCollection     _decoders;   // inner-constructed

    RVGeneralRegisters64    _GR;


    */

    RVInstance::Builder::Builder() noexcept
        : xlen          (XLEN64)
        , startup_pc    ({ 0 })
        , _MI           ()
        , _CSR          ()
        , _decoders     ()
        , exec_handler  (nullptr)
    { }

    RVInstance::Builder::~Builder() noexcept
    { }

    inline RVInstance::Builder& RVInstance::Builder::XLEN(XLen xlen) noexcept
    {
        this->xlen = xlen;
        return *this;
    }

    inline RVInstance::Builder& RVInstance::Builder::StartupPC(pc_t pc) noexcept
    {
        this->startup_pc = pc;
        return *this;
    }

    inline RVInstance::Builder& RVInstance::Builder::StartupPC32(arch32_t pc32) noexcept
    {
        this->startup_pc.pc32 = pc32;
        return *this;
    }

    inline RVInstance::Builder& RVInstance::Builder::StartupPC64(arch64_t pc64) noexcept
    {
        this->startup_pc.pc64 = pc64;
        return *this;
    }

    inline RVInstance::Builder& RVInstance::Builder::GR32(int address, arch32_t init_value) noexcept
    {
        this->_GR.Set(address, (arch64_t) init_value);
        return *this;
    }

    inline RVInstance::Builder& RVInstance::Builder::GR64(int address, arch64_t init_value) noexcept
    {
        this->_GR.Set(address, init_value);
        return *this;
    }

    inline RVInstance::Builder& RVInstance::Builder::Decoder(const RVDecoder* decoder) noexcept
    {
        this->_decoders.Add(decoder);
        return *this;
    }

    inline RVInstance::Builder& RVInstance::Builder::Decoder(std::initializer_list<const RVDecoder*> decoders) noexcept
    {
        for (auto iter = decoders.begin(); iter != decoders.end(); iter++)
            this->_decoders.Add(*iter);
        return *this;
    }

    inline RVInstance::Builder& RVInstance::Builder::Decoder(const RVDecoderCollection& decoders) noexcept
    {
        this->_decoders.AddAll(decoders);
        return *this;
    }

    inline RVInstance::Builder& RVInstance::Builder::MI(RVMemoryInterface* MI) noexcept
    {
        this->_MI = MI;
        return *this;
    }

    inline RVInstance::Builder& RVInstance::Builder::CSR(const RVCSRDefinition& CSR) noexcept
    {
        this->_CSR.Add(CSR);
        return *this;
    }

    inline RVInstance::Builder& RVInstance::Builder::CSR(std::initializer_list<RVCSRDefinition> CSRs) noexcept
    {
        this->_CSR.AddAll(CSRs);
        return *this;
    }

    inline RVInstance::Builder& RVInstance::Builder::TrapProcedures(const RVTrapProcedures& trap_procedures) noexcept
    {
        this->trap_procedures = trap_procedures;
        return *this;
    }

    inline RVInstance::Builder& RVInstance::Builder::ExecEEI(RVExecEEIHandler exec_handler) noexcept
    {
        this->exec_handler = exec_handler;
        return *this;
    }

    inline arch32_t RVInstance::Builder::GR32(int address) const noexcept
    {
        return (arch32_t) _GR.Get(address);
    }

    inline arch64_t RVInstance::Builder::GR64(int address) const noexcept
    {
        return _GR.Get(address);
    }

    inline RVCSRList& RVInstance::Builder::CSR() noexcept
    {
        return _CSR;
    }

    inline const RVCSRList& RVInstance::Builder::CSR() const noexcept
    {
        return _CSR;
    }

    inline RVDecoderCollection& RVInstance::Builder::Decoder() noexcept
    {
        return _decoders;
    }

    inline const RVDecoderCollection& RVInstance::Builder::Decoder() const noexcept
    {
        return _decoders;
    }

    inline RVTrapProcedures& RVInstance::Builder::TrapProcedures() noexcept
    {
        return trap_procedures;
    }

    inline const RVTrapProcedures& RVInstance::Builder::TrapProcedures() const noexcept
    {
        return trap_procedures;
    }

    inline RVExecEEIHandler& RVInstance::Builder::ExecEEI() noexcept
    {
        return exec_handler;
    }

    inline RVExecEEIHandler RVInstance::Builder::ExecEEI() const noexcept
    {
        return exec_handler;
    }

    RVInstance* RVInstance::Builder::Build() const noexcept
    {
        // copy-on-build
        RVInstance* instance = new RVInstance(
            _decoders,
            RVArchitectural(xlen),
            _MI,
            _CSR,
            trap_procedures,
            exec_handler);

        instance->arch.SetPC({ startup_pc.pc64 });
        
        switch (xlen)
        {
            case XLEN32:
                for (int i = 0; i < instance->arch.GR32()->GetSize(); i++)
                    instance->arch.GR32()->Set(i, (arch32_t) _GR.Get(i));
                break;

            case XLEN64:
                *(instance->arch.GR64()) = _GR;
                break;

            default:
                break;
        }

        return instance;
    }
}
