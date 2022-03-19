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

#include "riscvdef.hpp"
#include "riscvmem.hpp"


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

        // TODO

    public:
        RVArchitectural(XLen XLEN, RVMemoryInterface* MI);
        RVArchitectural(const RVArchitectural& obj);
        ~RVArchitectural();

        XLen                            XLEN() const;

        pc_t                            PC() const;
        pc_t&                           PC();
        void                            SetPC32(arch32_t pc);
        void                            SetPC64(arch64_t pc);

        void                            SetMI(RVMemoryInterface* MI);

        bool                            IsGR32() const;
        bool                            IsGR64() const;

        const RVGeneralRegisters32&     GR32() const;
        RVGeneralRegisters32&           GR32();
        const RVGeneralRegisters64&     GR64() const;
        RVGeneralRegisters64&           GR64();

        const RVMemoryInterface*        MI() const;
        RVMemoryInterface*              MI();

        // TODO
    };


    // Enumeration of RISC-V special operands
    typedef enum {
        SHAMT5 = 0,
        SHAMT6,
        CSR,
        CSR_UIMM
    } RVSpecialOperand;

    // RISC-V Decoded Instruction Textualizer
    typedef std::string (*RVInstructionTextualizer)(const RVInstruction&);

    // RISC-V Decoded Instruction Executor
    typedef void        (*RVInstructionExecutor)(const RVInstruction&, RVArchitectural&);

    // RISC-V Decoded Instruction
    class RVInstruction {
    private:
        insnraw_t                   insn;

        imm_t                       imm;
        int                         rd;
        int                         rs1;
        int                         rs2;

        std::string                 name;
        
        RVInstructionTextualizer    textualizer;
        RVInstructionExecutor       executor;

    public:
        RVInstruction(
            insnraw_t                   insn,
            imm_t                       imm,
            int                         rd, 
            int                         rs1, 
            int                         rs2, 
            std::string                 name,
            RVInstructionTextualizer    textualizer,
            RVInstructionExecutor       executor);

        RVInstruction();
        RVInstruction(const RVInstruction& obj);
        ~RVInstruction();

        insnraw_t                   GetRaw() const;
        imm_t                       GetImmediate() const;
        int                         GetRD() const;
        int                         GetRS1() const;
        int                         GetRS2() const;
        int                         GetOperand(RVSpecialOperand operand) const;

        const std::string&          GetName() const;

        RVInstructionTextualizer    GetTextualizer() const;
        RVInstructionExecutor       GetExecutor() const;

        void                        SetRaw(insnraw_t insn);
        void                        SetImmediate(imm_t imm);
        void                        SetRD(int rd);
        void                        SetRS1(int rs1);
        void                        SetRS2(int rs2);

        void                        SetName(const std::string& name);

        void                        SetTextualizer(RVInstructionTextualizer textualizer);
        void                        SetExecutor(RVInstructionExecutor executor);

        void                        Execute(RVArchitectural& arch) const;
        std::string                 ToString() const;
    };


    // RISC-V Instruction Decoder
    class RVDecoder {
    private:
        const std::string   name;
        const std::string   name_canonical;

    public:
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


    // RISC-V Instance
    class RVInstance {

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
}


// Implementation of: class RVArchitectural
namespace Jasse {
    /*
    const XLen              _XLEN;

    pc_t                    _PC;

    RVGeneralRegisters32*   _GR32;

    RVGeneralRegisters64*   _GR64;

    RVMemoryInterface*      _MI;
    */

    RVArchitectural::RVArchitectural(XLen XLEN, RVMemoryInterface* MI)
        : _XLEN (XLEN)
        , _PC   (pc_t())
        , _MI   (MI)
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

    const RVMemoryInterface* RVArchitectural::MI() const
    {
        return _MI;
    }

    RVMemoryInterface* RVArchitectural::MI()
    {
        return _MI;
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
        insnraw_t                   insn,
        imm_t                       imm,
        int                         rd,
        int                         rs1,
        int                         rs2,
        std::string                 name,
        RVInstructionTextualizer    textualizer,
        RVInstructionExecutor       executor)
        : insn          (insn)
        , imm           (imm)
        , rd            (rd)
        , rs1           (rs1)
        , rs2           (rs2)
        , name          (name)
        , textualizer   (textualizer)
        , executor      (executor)
    { }

    RVInstruction::RVInstruction()
        : insn          (0)
        , imm           ({ 0 })
        , rd            (0)
        , rs1           (0)
        , rs2           (0)
        , name          ("")
        , textualizer   (nullptr)
        , executor      (nullptr)
    { }

    RVInstruction::RVInstruction(const RVInstruction& obj)
        : insn          (obj.insn)
        , imm           (obj.imm)
        , rd            (obj.rd)
        , rs1           (obj.rs1)
        , rs2           (obj.rs2)
        , name          (obj.name)
        , textualizer   (obj.textualizer)
        , executor      (obj.executor)
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

    inline int RVInstruction::GetOperand(RVSpecialOperand operand) const
    {
        switch (operand)
        {
            case SHAMT5:
                return GET_OPERAND(insn, RV_OPERAND_SHAMT5_MASK, RV_OPERAND_SHAMT5_OFFSET);

            case SHAMT6:
                return GET_OPERAND(insn, RV_OPERAND_SHAMT6_MASK, RV_OPERAND_SHAMT6_OFFSET);

            case CSR:
                return GET_OPERAND(insn, RV_OPERAND_CSR_MASK, RV_OPERAND_CSR_OFFSET);

            case CSR_UIMM:
                return GET_OPERAND(insn, RV_OPERAND_CSR_UIMM_MASK, RV_OPERAND_CSR_UIMM_OFFSET);

            default:
                return 0;
        }
    }

    inline const std::string& RVInstruction::GetName() const
    {
        return name;
    }

    inline RVInstructionTextualizer RVInstruction::GetTextualizer() const
    {
        return textualizer;
    }

    inline RVInstructionExecutor RVInstruction::GetExecutor() const
    {
        return executor;
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

    inline void RVInstruction::SetName(const std::string& name)
    {
        this->name = name;
    }

    inline void RVInstruction::SetTextualizer(RVInstructionTextualizer textualizer)
    {
        this->textualizer = textualizer;
    }

    inline void RVInstruction::SetExecutor(RVInstructionExecutor executor)
    {
        this->executor = executor;
    }

    inline void RVInstruction::Execute(RVArchitectural& arch) const
    {
        (*executor)(*this, arch);
    }

    inline std::string RVInstruction::ToString() const
    {
        return (*textualizer)(*this);
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
                return true;

        return false;
    }
}

