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
#include <initializer_list>
#include <memory>
#include <algorithm>

#include "jasse.hpp"

#include "riscvdef.hpp"
#include "riscvmem.hpp"
#include "riscvcsr.hpp"


namespace Jasse {
    
    // RISC-V General Registers container template
    template<class TArch>
    class RVGeneralRegisters {
    private:
        static constexpr int    SIZE    = RV_ARCH_REG_COUNT;

        TArch*  registers;

    public:
        RVGeneralRegisters();
        RVGeneralRegisters(const RVGeneralRegisters<TArch>& obj);
        ~RVGeneralRegisters();

        int     GetSize() const;
        bool    CheckBound(int address) const;

        TArch   Get(int address) const;
        void    Set(int address, TArch value);

        TArch   operator[](int address) const;
        TArch&  operator[](int address);

        void    operator=(const RVGeneralRegisters<TArch>& obj);
    };

    // RISC-V General Registers container of XLEN=32
    using RVGeneralRegisters32  = RVGeneralRegisters<arch32_t>;

    // RISC-V General Registers container of XLEN=64
    using RVGeneralRegisters64  = RVGeneralRegisters<arch64_t>;
    

    // X-Len Enumeration
    typedef enum {
        XLEN32 = 0,
        XLEN64
    } XLen;

    // RISC-V Architectural State Container
    class RVArchitectural {
    private:
        const XLen              _XLEN;

        pc_t                    _PC;

        RVGeneralRegisters32*   _GR32;

        RVGeneralRegisters64*   _GR64;

        RVMemoryInterface*      _MI;

        RVCSRSpace              _CSR;

        // TODO ... More architectural states could be appended here ...

    public:
        RVArchitectural() = delete;
        RVArchitectural(XLen XLEN, const RVCSRSpace& CSRs, RVMemoryInterface* MI);
        RVArchitectural(const RVArchitectural& obj);
        ~RVArchitectural();

        XLen                            XLEN() const;

        pc_t                            PC() const;
        pc_t&                           PC();
        void                            SetPC32(arch32_t pc);
        void                            SetPC64(arch64_t pc);

        void                            SetMI(RVMemoryInterface* MI);
        void                            SetCSR(const RVCSRSpace& CSR);

        bool                            IsGR32() const;
        bool                            IsGR64() const;

        const RVGeneralRegisters32&     GR32() const;
        RVGeneralRegisters32&           GR32();
        const RVGeneralRegisters64&     GR64() const;
        RVGeneralRegisters64&           GR64();

        arch64_t                        GetGRx64Zext(int addr) const;
        arch64_t                        GetGRx64Sext(int addr) const;
        arch32_t                        GetGRx32(int addr) const;

        void                            SetGRx64(int addr, arch64_t val64);
        void                            SetGRx32Zext(int addr, arch32_t val32);
        void                            SetGRx32Sext(int addr, arch32_t val32);

        const RVMemoryInterface*        MI() const;
        RVMemoryInterface*              MI();

        const RVCSRSpace&               CSR() const;
        RVCSRSpace&                     CSR();

        // TODO ... More architectural states could be appended here ...
    };


    // RISC-V Instruction Execution Status
    // *NOTICE: Special actions (e.g. WFI-Wait For Interrupt) should be handled by EEI or 
    //          external EEI-defined components.
    //          They aren't and shouldn't be embedded in the instruction execution procedures.
    //          The processing procedure of external interrupts MUST run ahead of any instruction
    //          execution procedure.
    typedef enum {
        // Naturally increase the PC. Execute instruction stream in sequence normally.
        EXEC_SEQUENTIAL = 0,

        // PC Hold. PC shouldn't be changed in instruction execution procedure.
        // And PC value MUST NOT be manipulated in EEI instance or out of iustruction
        // execution procedure.
        EXEC_PC_HOLD,

        // PC Jump. PC is changed in instruction execution procedure.
        // And PC value MUST NOT be manipulated in EEI instance or out of iustruction
        // execution procedure.
        EXEC_PC_JUMP,

        // Trap Enter. Results of instruction execution procedure shouldn't be commited
        // to the architectural states.
        // Trap-related CSRs or EEI-defined related registers were manipulated in
        // instruction execution procedure.
        // @see Jasse::TrapEnter (in header "riscvexcept.hpp")
        EXEC_TRAP_ENTER,

        // Trap Return. 
        // Trap-related CSRs or EEI-defined related registers were manipulated in
        // instruction execution procedure.
        // @see Jasse::TrapReturn (in header "riscvexcept.hpp")
        EXEC_TRAP_RETURN,

        // Fetch Access Fault. *EEI-defined*
        // MOP_ACCESS_FAULT when fetching instruction by PC from Memory Interface.
        // This status SHOULDN'T be produced in any execution procedure.
        // The way of handling this status (Raising an exception, or .etc) is EEI-defined.
        EXEC_FETCH_ACCESS_FAULT,

        // Fetch Address Misaligned. *EEI-defined*
        // MOP_ADDRESS_MISALIGNED when fetching instruction by PC from Memory Interface.
        // This status SHOULDN'T be produced in any execution procedure.
        // The way of handling this status (Raising an exception, or .etc) is EEI-defined.
        EXEC_FETCH_ADDRESS_MISALIGNED,

        // WFI. Wait-For-Interrupt. *EEI-defined*
        // WFI function is implemented in EEI. Interrupt-related procedures were not
        // necessary in instruction execution procedure. 
        // For the simplest implementation, PC is held still till a interrupt occurs.
        EXEC_WAIT_FOR_INTERRUPT,

        // Not Implemented. *EEI-defined*
        // Instruction codepoint exists, but the execution procedure not implemented.
        // The way of handling this status (Raising an exception, or .etc) is EEI-defined.
        EXEC_NOT_IMPLEMENTED,

        // Not Decoded. (Not implemented in codepoint & decoders). *EEI-defined*
        // Instruction codepoint doesn't exist. This usually indicates that an instruction
        // was properly fetched but it was illegal.
        // This status SHOULDN'T be produced in any execution procedure.
        // The way of handling this status (Raising an exception, or .etc) is EEI-defined.
        EXEC_NOT_DECODED
    } RVExecStatus;


    // RISC-V Codepoint Type
    typedef enum {
        RVTYPE_R = 0,
        RVTYPE_I,
        RVTYPE_S,
        RVTYPE_B,
        RVTYPE_U,
        RVTYPE_J
    } RVCodepointType;

    
    //
    class RVInstruction;

    // RISC-V Codepoint
    class RVCodepoint {
    public:
        // RISC-V Instruction Textualizer
        typedef std::string     (*Textualizer)(const RVInstruction&);

        // RISC-V Instruction Executor
        typedef RVExecStatus    (*Executor)(const RVInstruction&, RVArchitectural&);

    private:
        std::string     name;
        RVCodepointType type;

        Textualizer     textualizer;
        Executor        executor;

    public:
        RVCodepoint(const char*             name, 
                    const RVCodepointType   type,
                    const Textualizer       textualizer, 
                    const Executor          executor);

        RVCodepoint(const std::string&      name,
                    const RVCodepointType   type,
                    const Textualizer       textualizer,
                    const Executor          executor);

        RVCodepoint();
        RVCodepoint(const RVCodepoint& obj);
        ~RVCodepoint();

        void                    SetName(const std::string& name);
        void                    SetType(const RVCodepointType type);
        void                    SetTextualizer(const Textualizer textualizer);
        void                    SetExecutor(const Executor executor);

        const std::string&      GetName() const;
        RVCodepointType         GetType() const;
        const Textualizer       GetTextualizer() const;
        const Executor          GetExecutor() const;
    };

    // RISC-V Decoded Instruction
    class RVInstruction {
    private:
        insnraw_t       insn;

        imm_t           imm;
        int             rd;
        int             rs1;
        int             rs2;

        RVCodepoint     trait;

    public:
        RVInstruction(
            insnraw_t           insn,
            imm_t               imm,
            int                 rd, 
            int                 rs1, 
            int                 rs2, 
            const RVCodepoint&  trait);

        RVInstruction();
        RVInstruction(const RVInstruction& obj);
        ~RVInstruction();

        insnraw_t                   GetRaw() const;
        imm_t                       GetImmediate() const;
        int                         GetRD() const;
        int                         GetRS1() const;
        int                         GetRS2() const;

        RVCodepoint&                GetTrait();
        const RVCodepoint&          GetTrait() const;

        const std::string&          GetName() const;
        RVCodepoint::Textualizer    GetTextualizer() const;
        RVCodepoint::Executor       GetExecutor() const;

        void                        SetRaw(insnraw_t insn);
        void                        SetImmediate(imm_t imm);
        void                        SetRD(int rd);
        void                        SetRS1(int rs1);
        void                        SetRS2(int rs2);

        void                        SetTrait(const RVCodepoint& trait);

        void                        SetName(const std::string& name);
        void                        SetTextualizer(RVCodepoint::Textualizer textualizer);
        void                        SetExecutor(RVCodepoint::Executor executor);

        RVExecStatus                Execute(RVArchitectural& arch) const;
        std::string                 ToString() const;
    };


    // RISC-V Instruction Decoder Trait Function
    typedef bool        (*RVDecoderTrait)(insnraw_t insnraw, RVInstruction& insn);

    // RISC-V Instruction Decoder
    class RVDecoder {
    private:
        const std::string   name;
        const std::string   name_canonical;

    public:
        RVDecoder(const char* name, const char* name_canonical);
        RVDecoder(std::string name, std::string name_canonical);
        ~RVDecoder();

        const std::string&  GetName() const;
        const std::string&  GetCanonicalName() const;

        virtual bool        Decode(insnraw_t insnraw, RVInstruction& insn) const = 0;

        void    operator=(const RVDecoder& obj) = delete;
    };

    // RISC-V Instruction Decoder Collection
    typedef std::list<const RVDecoder*>::const_iterator     RVDecoderIterator;

    //
    class RVDecoderCollection {
    private:
        std::list<const RVDecoder*>     decoders;

    public:
        RVDecoderCollection();
        RVDecoderCollection(std::initializer_list<const RVDecoder*> list);
        RVDecoderCollection(const RVDecoderCollection& obj);
        ~RVDecoderCollection();

        int                 GetSize() const;
        void                Clear();

        RVDecoderIterator   Begin() const;
        RVDecoderIterator   End() const;

        bool                Has(const std::string& name) const;
        bool                Has(const char* name) const;
        bool                HasCanonical(const std::string& name_canonical) const;
        bool                HasCanonical(const char* name) const;

        bool                Add(const RVDecoder* decoder);

        bool                Remove(const std::string& name);
        bool                Remove(const char* name);
        bool                RemoveCanonical(const std::string& name_canonical);
        bool                RemoveCanonical(const char* name_canonical);

        bool                Decode(insnraw_t insnraw, RVInstruction& insn) const;
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

        RVExecEEIHandler        exec_handler;

    public:
        RVInstance(const RVDecoderCollection&   decoders,
                   RVArchitectural&&            arch,
                   RVExecEEIHandler             exec_handler) noexcept;

        RVInstance() = delete;
        RVInstance(const RVInstance& obj) noexcept;
        ~RVInstance() noexcept;

        RVDecoderCollection&            GetDecoders() noexcept;
        const RVDecoderCollection&      GetDecoders() const noexcept;

        RVArchitectural&                GetArch() noexcept;
        const RVArchitectural&          GetArch() const noexcept;

        RVExecEEIHandler&               GetExecEEI() noexcept;
        RVExecEEIHandler                GetExecEEI() const noexcept;
        void                            SetExecEEI(RVExecEEIHandler exec_handler) noexcept;

        RVExecStatus                    Eval();

        void    operator=(const RVInstance& obj) = delete;
    };

    class RVInstance::Builder {
    private:
        XLen                    xlen;
        pc_t                    startup_pc;

        RVMemoryInterface*      _MI;
            
        RVCSRSpace              _CSR;        // inner-constructed

        RVDecoderCollection     _decoders;   // inner-constructed

        RVGeneralRegisters64    _GR;         // inner-constructed, transient

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

        Builder&                    MI(RVMemoryInterface* MI) noexcept;

        Builder&                    CSR(const RVCSRDefinition& CSR) noexcept;
        Builder&                    CSR(const RVCSRDefinition& CSR, csr_t init_value) noexcept;
        Builder&                    CSR(std::initializer_list<const RVCSRDefinition> CSRs) noexcept;

        Builder&                    ExecEEI(RVExecEEIHandler exec_handler) noexcept;

        arch32_t&                   GR32(int address) noexcept;
        arch32_t                    GR32(int address) const noexcept;
        arch64_t&                   GR64(int address) noexcept;
        arch64_t                    GR64(int address) const noexcept;
        RVCSRSpace&                 CSR() noexcept;
        const RVCSRSpace&           CSR() const noexcept;
        RVDecoderCollection&        Decoder() noexcept;
        const RVDecoderCollection&  Decoder() const noexcept;
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
    RVGeneralRegisters<TArch>::RVGeneralRegisters()
        : registers (new TArch[SIZE]())
    { }

    template<class TArch>
    RVGeneralRegisters<TArch>::RVGeneralRegisters(const RVGeneralRegisters<TArch>& obj)
        : registers (new TArch[SIZE]())
    { 
        // should be replaced as "memcpy", but suppressing warnings
        for (int i = 0; i < SIZE; i++)
            registers[i] = obj.registers[i];
    }

    template<class TArch>
    RVGeneralRegisters<TArch>::~RVGeneralRegisters()
    {
        delete[] registers;
    }

    template<class TArch>
    inline int RVGeneralRegisters<TArch>::GetSize() const
    {
        return SIZE;
    }

    template<class TArch>
    inline bool RVGeneralRegisters<TArch>::CheckBound(int address) const
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
    TArch RVGeneralRegisters<TArch>::operator[](int address) const
    {
        return !address ? TArch() : registers[address];
    }

    template<class TArch>
    TArch& RVGeneralRegisters<TArch>::operator[](int address)
    {
        return !address ? (registers[address] = TArch()) : registers[address];
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

    RVMemoryInterface*      _MI;

    RVCSRSpace              _CSR;
    */

    RVArchitectural::RVArchitectural(XLen XLEN, const RVCSRSpace& CSRs, RVMemoryInterface* MI)
        : _XLEN (XLEN)
        , _PC   (pc_t())
        , _MI   (MI)
        , _CSR  (CSRs)
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

        default:
            _GR32 = nullptr;
            _GR64 = nullptr;
        }
    }

    RVArchitectural::RVArchitectural(const RVArchitectural& obj)
        : _XLEN (obj._XLEN)
        , _PC   (obj._PC)
        , _MI   (obj._MI)
        , _CSR  (obj._CSR)
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

    inline XLen RVArchitectural::XLEN() const
    {
        return _XLEN;
    }

    inline pc_t RVArchitectural::PC() const
    {
        return _PC;
    }

    inline pc_t& RVArchitectural::PC()
    {
        return _PC;
    }

    void RVArchitectural::SetPC32(arch32_t pc)
    {
        if (_XLEN != XLEN32)
            throw std::logic_error("set 32-bit PC in non-32-XLEN arch");

        _PC.pc32 = pc;
    }

    void RVArchitectural::SetPC64(arch64_t pc)
    {
        if (_XLEN != XLEN64)
            throw std::logic_error("set 64-bit PC in non-64-XLEN arch");

        _PC.pc64 = pc;
    }

    inline void RVArchitectural::SetMI(RVMemoryInterface* MI)
    {
        _MI = MI;
    }

    inline void RVArchitectural::SetCSR(const RVCSRSpace& CSR)
    {
        _CSR = CSR;
    }

    inline bool RVArchitectural::IsGR32() const
    {
        return _GR32;
    }

    inline bool RVArchitectural::IsGR64() const
    {
        return _GR64;
    }

    const RVGeneralRegisters32& RVArchitectural::GR32() const
    {
        if (!_GR32)
            throw std::logic_error("non-GR32 arch");

        return *_GR32;
    }

    RVGeneralRegisters32& RVArchitectural::GR32()
    {
        if (!_GR32)
            throw std::logic_error("non-GR32 arch");

        return *_GR32;
    }

    const RVGeneralRegisters64& RVArchitectural::GR64() const
    {
        if (!_GR64)
            throw std::logic_error("non-GR64 arch");
        
        return *_GR64;
    }

    RVGeneralRegisters64& RVArchitectural::GR64()
    {
        if (!_GR64)
            throw std::logic_error("non-GR64 arch");
        
        return *_GR64;
    }

    // *NOTICE: Get 64-bit value from General Register.
    //          If XLEN is less than 64, the value read is zero-extended,
    //          and wouldn't raise any exception.
    inline arch64_t RVArchitectural::GetGRx64Zext(int addr) const
    {
        if (_GR64)
            return _GR64->Get(addr);
        else if (_GR32)
            return ZEXT_W(_GR32->Get(addr));
        else
            return 0;
    }

    // *NOTICE: Get 64-bit value from General Register.
    //          If XLEN is less than 64, the value read is sign-extended,
    //          and wouldn't raise any exception.
    inline arch64_t RVArchitectural::GetGRx64Sext(int addr) const
    {
        if (_GR64)
            return _GR64->Get(addr);
        else if (_GR32)
            return SEXT_W(_GR32->Get(addr));
        else
            return 0;
    }

    // *NOTICE: Get 32-bit value from General Register.
    //          If XLEN is greater than 32, the value read is truncated,
    //          and wouldn't raise any exception.
    inline arch32_t RVArchitectural::GetGRx32(int addr) const
    {
        if (_GR64)
            return (arch32_t) _GR64->Get(addr);
        else if (_GR32)
            return _GR32->Get(addr);
        else
            return 0;
    }

    // *NOTICE: Set General Register with 64-bit value.
    //          If XLEN is less than 64, the value passed through is truncated,
    //          and wouldn't raise any exception.
    inline void RVArchitectural::SetGRx64(int addr, arch64_t val64)
    {
        if (_GR64)
            _GR64->Set(addr, val64);
        else if (_GR32)
            _GR32->Set(addr, (arch32_t) val64); // truncate
    }

    // *NOTICE: Set General Register with 32-bit value.
    //          If XLEN is greater than 32, the value passed through is zero-extended,
    //          and wouldn't raise any exception.
    inline void RVArchitectural::SetGRx32Zext(int addr, arch32_t val32)
    {
        if (_GR64)
            _GR64->Set(addr, ZEXT_W(val32));
        else if (_GR32)
            _GR32->Set(addr, val32);
    }

    // *NOTICE: Set General Register with 32-bit value.
    //          If XLEN is greater than 32, the value passed through is sign-extended,
    //          and wouldn't raise any exception.
    inline void RVArchitectural::SetGRx32Sext(int addr, arch32_t val32)
    {
        if (_GR64)
            _GR64->Set(addr, SEXT_W(val32));
        else if (_GR32)
            _GR32->Set(addr, val32);
    }

    inline const RVMemoryInterface* RVArchitectural::MI() const
    {
        return _MI;
    }

    inline RVMemoryInterface* RVArchitectural::MI()
    {
        return _MI;
    }

    inline const RVCSRSpace& RVArchitectural::CSR() const
    {
        return _CSR;
    }

    inline RVCSRSpace& RVArchitectural::CSR()
    {
        return _CSR;
    }
}


// Implementation of: class RVCodepoint
namespace Jasse {
    /*
    std::string     name;
    RVCodepointType type;

    Textualizer     textualizer;
    Executor        executor;
    */

    RVCodepoint::RVCodepoint(const char*             name, 
                             const RVCodepointType   type,
                             const Textualizer       textualizer, 
                             const Executor          executor)
        : name          (std::string(name))
        , type          (type)
        , textualizer   (textualizer)
        , executor      (executor)
    { }

    RVCodepoint::RVCodepoint(const std::string&      name,
                             const RVCodepointType   type,
                             const Textualizer       textualizer,
                             const Executor          executor)
        : name          (name)
        , type          (type)
        , textualizer   (textualizer)
        , executor      (executor)
    { }

    RVCodepoint::RVCodepoint()
        : name          ()
        , type          ()
        , textualizer   (nullptr)
        , executor      (nullptr)
    { }

    RVCodepoint::RVCodepoint(const RVCodepoint& obj)
        : name          (obj.name)
        , type          (obj.type)
        , textualizer   (obj.textualizer)
        , executor      (obj.executor)
    { }

    RVCodepoint::~RVCodepoint()
    { }

    inline void RVCodepoint::SetName(const std::string& name)
    {
        this->name = name;
    }

    inline void RVCodepoint::SetType(const RVCodepointType type)
    {
        this->type = type;
    }

    inline void RVCodepoint::SetTextualizer(const Textualizer textualizer)
    {
        this->textualizer = textualizer;
    }

    inline void RVCodepoint::SetExecutor(const Executor executor)
    {
        this->executor = executor;
    }

    inline const std::string& RVCodepoint::GetName() const
    {
        return name;
    }

    inline RVCodepointType RVCodepoint::GetType() const
    {
        return type;
    }

    inline const RVCodepoint::Textualizer RVCodepoint::GetTextualizer() const
    {
        return textualizer;
    }

    inline const RVCodepoint::Executor RVCodepoint::GetExecutor() const
    {
        return executor;
    }
}


// Implementation of: class RVInstruction
namespace Jasse {
    /*
    insnraw_t                   insn;

    imm_t                       imm;
    int                         rd;
    int                         rs1;
    int                         rs2;

    std::string                 name;
        
    RVInstructionTextualizer    textualizer;
    RVInstructionExecutor       executor;
    */

    RVInstruction::RVInstruction(
        insnraw_t           insn,
        imm_t               imm,
        int                 rd,
        int                 rs1,
        int                 rs2,
        const RVCodepoint&  trait)
        : insn          (insn)
        , imm           (imm)
        , rd            (rd)
        , rs1           (rs1)
        , rs2           (rs2)
        , trait         (trait)
    { }

    RVInstruction::RVInstruction()
        : insn          (0)
        , imm           ({ 0 })
        , rd            (0)
        , rs1           (0)
        , rs2           (0)
        , trait         ()
    { }

    RVInstruction::RVInstruction(const RVInstruction& obj)
        : insn          (obj.insn)
        , imm           (obj.imm)
        , rd            (obj.rd)
        , rs1           (obj.rs1)
        , rs2           (obj.rs2)
        , trait         ()
    { }

    RVInstruction::~RVInstruction()
    { }

    inline insnraw_t RVInstruction::GetRaw() const
    {
        return insn;
    }

    inline imm_t RVInstruction::GetImmediate() const
    {
        return imm;
    }

    inline int RVInstruction::GetRD() const
    {
        return rd;
    }

    inline int RVInstruction::GetRS1() const 
    {
        return rs1;
    }

    inline int RVInstruction::GetRS2() const
    {
        return rs2;
    }

    inline RVCodepoint& RVInstruction::GetTrait()
    {
        return trait;
    }

    inline const RVCodepoint& RVInstruction::GetTrait() const
    {
        return trait;
    }

    inline const std::string& RVInstruction::GetName() const
    {
        return trait.GetName();
    }

    inline RVCodepoint::Textualizer RVInstruction::GetTextualizer() const
    {
        return trait.GetTextualizer();
    }

    inline RVCodepoint::Executor RVInstruction::GetExecutor() const
    {
        return trait.GetExecutor();
    }

    inline void RVInstruction::SetRaw(insnraw_t insn)
    {
        this->insn = insn;
    }

    inline void RVInstruction::SetImmediate(imm_t imm)
    {
        this->imm = imm;
    }

    inline void RVInstruction::SetRD(int rd)
    {
        this->rd = rd;
    }

    inline void RVInstruction::SetRS1(int rs1)
    {
        this->rs1 = rs1;
    }

    inline void RVInstruction::SetRS2(int rs2)
    {
        this->rs2 = rs2;
    }

    inline void RVInstruction::SetTrait(const RVCodepoint& trait)
    {
        this->trait = trait;
    }

    inline void RVInstruction::SetName(const std::string& name)
    {
        trait.SetName(name);
    }

    inline void RVInstruction::SetTextualizer(RVCodepoint::Textualizer textualizer)
    {
        trait.SetTextualizer(textualizer);
    }

    inline void RVInstruction::SetExecutor(RVCodepoint::Executor executor)
    {
        trait.SetExecutor(executor);
    }

    inline RVExecStatus RVInstruction::Execute(RVArchitectural& arch) const
    {
        return GetExecutor()(*this, arch);
    }

    inline std::string RVInstruction::ToString() const
    {
        return GetTextualizer()(*this);
    }
}


// Implementation of: class RVDecoder
namespace Jasse {
    /*
    const std::string   name;
    const std::string   name_canonical;
    */

    RVDecoder::RVDecoder(std::string name, std::string name_canonical)
        : name              (name)
        , name_canonical    (name_canonical)
    { }

    RVDecoder::RVDecoder(const char* name, const char* name_canonical)
        : name              (std::string(name))
        , name_canonical    (std::string(name_canonical))
    { }

    RVDecoder::~RVDecoder()
    { }

    inline const std::string& RVDecoder::GetName() const
    {
        return name;
    }

    inline const std::string& RVDecoder::GetCanonicalName() const
    {
        return name_canonical;
    }
}


// Implementation of: class RVDecoderCollection
namespace Jasse {
    /*
    std::list<const RVDecoder*>     decoders;
    */

    RVDecoderCollection::RVDecoderCollection()
        : decoders  (std::list<const RVDecoder*>())
    { }

    RVDecoderCollection::RVDecoderCollection(std::initializer_list<const RVDecoder*> list)
        : decoders  (std::list<const RVDecoder*>(list))
    { }

    RVDecoderCollection::RVDecoderCollection(const RVDecoderCollection& obj)
        : decoders  (obj.decoders)
    { }

    RVDecoderCollection::~RVDecoderCollection()
    { }

    inline int RVDecoderCollection::GetSize() const
    {
        return decoders.size();
    }

    inline void RVDecoderCollection::Clear()
    {
        decoders.clear();
    }

    inline RVDecoderIterator RVDecoderCollection::Begin() const
    {
        return decoders.begin();
    }

    inline RVDecoderIterator RVDecoderCollection::End() const
    {
        return decoders.end();
    }

    inline bool RVDecoderCollection::Has(const std::string& name) const
    {
        return Has(name.c_str());
    }

    bool RVDecoderCollection::Has(const char* name) const
    {
        std::list<const RVDecoder*>::const_iterator iter = decoders.begin();
        for (; iter != decoders.end(); iter++)
            if ((*iter)->GetName().compare(name) == 0)
                return true;

        return false;
    }

    inline bool RVDecoderCollection::HasCanonical(const std::string& name_canonical) const
    {
        return Has(name_canonical.c_str());
    }

    bool RVDecoderCollection::HasCanonical(const char* name_canonical) const
    {
        std::list<const RVDecoder*>::const_iterator iter = decoders.begin();
        for (; iter != decoders.end(); iter++)
            if ((*iter)->GetCanonicalName().compare(name_canonical) == 0)
                return true;

        return false;
    }

    bool RVDecoderCollection::Add(const RVDecoder* decoder)
    {
        std::list<const RVDecoder*>::const_iterator iter = decoders.begin();
        for (; iter != decoders.end(); iter++)
            if ((*iter)->GetName().compare(decoder->GetName()) == 0
                || (*iter)->GetCanonicalName().compare(decoder->GetCanonicalName()) == 0)
                return false;

        decoders.push_back(decoder);

        return true;
    }

    inline bool RVDecoderCollection::Remove(const std::string& name)
    {
        return Remove(name.c_str());
    }

    bool RVDecoderCollection::Remove(const char* name)
    {
        std::list<const RVDecoder*>::const_iterator iter = decoders.begin();
        for (; iter != decoders.end(); iter++)
            if ((*iter)->GetName().compare(name) == 0)
            {
                decoders.erase(iter);
                return true;
            }
        
        return false;
    }

    inline bool RVDecoderCollection::RemoveCanonical(const std::string& name_canonical)
    {
        return RemoveCanonical(name_canonical.c_str());
    }

    bool RVDecoderCollection::RemoveCanonical(const char* name_canonical)
    {
        std::list<const RVDecoder*>::const_iterator iter = decoders.begin();
        for (; iter != decoders.end(); iter++)
            if ((*iter)->GetCanonicalName().compare(name_canonical) == 0)
            {
                decoders.erase(iter);
                return true;
            }

        return false;
    }

    bool RVDecoderCollection::Decode(insnraw_t insnraw, RVInstruction& insn) const
    {
        std::list<const RVDecoder*>::const_iterator iter = decoders.begin();
        for (; iter != decoders.end(); iter++)
            if ((*iter)->Decode(insnraw, insn))
            {
                insn.SetRaw(insnraw);
                return true;
            }

        return false;
    }
}


// Implementation of: class RVInstance
namespace Jasse {
    /*
    RVDecoderCollection     decoders;

    RVArchitectural         arch;

    RVExecEEIHandler        exec_handler;
    */

    RVInstance::RVInstance(const RVDecoderCollection&   decoders,
                           RVArchitectural&&            arch,
                           RVExecEEIHandler             exec_handler) noexcept
        : decoders      (decoders)
        , arch          (arch)
        , exec_handler  (exec_handler)
    { }

    RVInstance::RVInstance(const RVInstance& obj) noexcept
        : decoders      (obj.decoders)
        , arch          (obj.arch)
        , exec_handler  (obj.exec_handler)
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

    RVExecStatus RVInstance::Eval()
    {
        // TODO interrupt inception

        // instruction fetch
        data_t fetched;
        RVMOPStatus mopstatus;

        if (arch.XLEN() == XLEN32) // XLEN=32
            mopstatus = arch.MI()->Read(arch.PC().pc32, MOPW_WORD, &fetched);
        else // XLEN=64
            mopstatus = arch.MI()->Read(arch.PC().pc64, MOPW_WORD, &fetched);

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

                default:
                    SHOULD_NOT_REACH_HERE;
            }

            if (exec_handler)
                exec_handler(*this, rstatus, nullptr);

            return rstatus;
        }

        // instruction decode
        RVInstruction decoded;

        if (!decoders.Decode(fetched.data32, decoded))
        {
            if (exec_handler)
                exec_handler(*this, EXEC_NOT_DECODED, nullptr);

            return EXEC_NOT_DECODED;
        }

        // execution
        RVExecStatus exec_status = decoded.Execute(arch);
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
            
    RVCSRSpace              _CSR;        // inner-constructed

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
        this->_GR[address] = (arch64_t)init_value;
        return *this;
    }

    inline RVInstance::Builder& RVInstance::Builder::GR64(int address, arch64_t init_value) noexcept
    {
        this->_GR[address] = init_value;
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

    inline RVInstance::Builder& RVInstance::Builder::MI(RVMemoryInterface* MI) noexcept
    {
        this->_MI = MI;
        return *this;
    }

    inline RVInstance::Builder& RVInstance::Builder::CSR(const RVCSRDefinition& CSR) noexcept
    {
        this->_CSR.SetCSR(CSR);
        return *this;
    }

    inline RVInstance::Builder& RVInstance::Builder::CSR(const RVCSRDefinition& CSR, csr_t init_value) noexcept
    {
        this->_CSR.SetCSR(CSR)->SetValue(init_value);
        return *this;
    }

    inline RVInstance::Builder& RVInstance::Builder::CSR(std::initializer_list<const RVCSRDefinition> CSRs) noexcept
    {
        this->_CSR.SetCSRs(CSRs);
        return *this;
    }

    inline RVInstance::Builder& RVInstance::Builder::ExecEEI(RVExecEEIHandler exec_handler) noexcept
    {
        this->exec_handler = exec_handler;
        return *this;
    }

    inline arch32_t& RVInstance::Builder::GR32(int address) noexcept
    {
        return (arch32_t&)_GR[address];
    }

    inline arch32_t RVInstance::Builder::GR32(int address) const noexcept
    {
        return (arch32_t)_GR[address];
    }

    inline arch64_t& RVInstance::Builder::GR64(int address) noexcept
    {
        return _GR[address];
    }

    inline arch64_t RVInstance::Builder::GR64(int address) const noexcept
    {
        return _GR[address];
    }

    inline RVCSRSpace& RVInstance::Builder::CSR() noexcept
    {
        return _CSR;
    }

    inline const RVCSRSpace& RVInstance::Builder::CSR() const noexcept
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
            RVArchitectural(xlen, _CSR, _MI),
            exec_handler);

        instance->arch.PC().pc64 = startup_pc.pc64;
        
        switch (xlen)
        {
            case XLEN32:
                for (int i = 0; i < instance->arch.GR32().GetSize(); i++)
                    instance->arch.GR32()[i] = (arch32_t)_GR[i];
                break;

            case XLEN64:
                instance->arch.GR64() = _GR;
                break;

            default:
                break;
        }

        return instance;
    }
}
