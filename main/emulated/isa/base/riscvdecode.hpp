#pragma once
//
// RISC-V Instruction Set Architecture Emulator (Jasse)
//
// Decoder infrastructure
//

#include <list>
#include <string>

#include "riscvdef.hpp"
#include "riscvgen.hpp"
#include "riscvcode.hpp"

namespace Jasse {
    // RISC-V Decoded Instruction
    class RVInstruction {
    private:
        insnraw_t           insn;

        imm_t               imm;
        int                 rd;
        int                 rs1;
        int                 rs2;

        const RVCodepoint*  codepoint; // eliminated copy-on-construct

    public:
        RVInstruction(
            insnraw_t           insn,
            imm_t               imm,
            int                 rd, 
            int                 rs1, 
            int                 rs2, 
            const RVCodepoint*  codepoint);

        RVInstruction();
        RVInstruction(const RVInstruction& obj);
        ~RVInstruction();

        insnraw_t                   GetRaw() const;
        imm_t                       GetImmediate() const;
        int                         GetRD() const;
        int                         GetRS1() const;
        int                         GetRS2() const;

        const RVCodepoint*          GetCodepoint() const;

        const std::string&          GetName() const;
        RVCodepoint::Textualizer    GetTextualizer() const;
        RVCodepoint::Executor       GetExecutor() const;

        void                        SetRaw(insnraw_t insn);
        void                        SetImmediate(imm_t imm);
        void                        SetRD(int rd);
        void                        SetRS1(int rs1);
        void                        SetRS2(int rs2);

        void                        SetCodepoint(const RVCodepoint* trait);

        RVExecStatus                Execute(const RVExecContext& ctx) const;

        std::string                 ToString() const;
    };

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

        virtual const RVCodepointCollection&    GetAllCodepoints() const = 0;

        void    operator=(const RVDecoder& obj) = delete;
    };

    // RISC-V Instruction Decoder Collection
    typedef std::list<const RVDecoder*>::iterator           RVDecoderIterator;
    typedef std::list<const RVDecoder*>::const_iterator     RVDecoderConstIterator;

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

        RVDecoderIterator       Begin();
        RVDecoderConstIterator  Begin() const;
        RVDecoderIterator       End();
        RVDecoderConstIterator  End() const;

        bool                Has(const std::string& name) const;
        bool                Has(const char* name) const;
        bool                HasCanonical(const std::string& name_canonical) const;
        bool                HasCanonical(const char* name) const;

        bool                Add(const RVDecoder* decoder);
        void                AddAll(const RVDecoderCollection& collection);

        bool                Remove(const std::string& name);
        bool                Remove(const char* name);
        bool                RemoveCanonical(const std::string& name_canonical);
        bool                RemoveCanonical(const char* name_canonical);

        bool                Decode(insnraw_t insnraw, RVInstruction& insn) const;
    };
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

    const RVCodepoint*          codepoint
    */

    RVInstruction::RVInstruction(
        insnraw_t           insn,
        imm_t               imm,
        int                 rd,
        int                 rs1,
        int                 rs2,
        const RVCodepoint*  codepoint)
        : insn          (insn)
        , imm           (imm)
        , rd            (rd)
        , rs1           (rs1)
        , rs2           (rs2)
        , codepoint     (codepoint)
    { }

    RVInstruction::RVInstruction()
        : insn          (0)
        , imm           (0)
        , rd            (0)
        , rs1           (0)
        , rs2           (0)
        , codepoint     (nullptr)
    { }

    RVInstruction::RVInstruction(const RVInstruction& obj)
        : insn          (obj.insn)
        , imm           (obj.imm)
        , rd            (obj.rd)
        , rs1           (obj.rs1)
        , rs2           (obj.rs2)
        , codepoint     (obj.codepoint)
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

    inline const RVCodepoint* RVInstruction::GetCodepoint() const
    {
        return codepoint;
    }

    inline const std::string& RVInstruction::GetName() const
    {
        return codepoint->GetName();
    }

    inline RVCodepoint::Textualizer RVInstruction::GetTextualizer() const
    {
        return codepoint->GetTextualizer();
    }

    inline RVCodepoint::Executor RVInstruction::GetExecutor() const
    {
        return codepoint->GetExecutor();
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

    inline void RVInstruction::SetCodepoint(const RVCodepoint* codepoint)
    {
        this->codepoint = codepoint;
    }

    inline RVExecStatus RVInstruction::Execute(const RVExecContext& ctx) const
    {
        return GetExecutor()(*this, ctx);
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

    inline RVDecoderIterator RVDecoderCollection::Begin()
    {
        return decoders.begin();
    }

    inline RVDecoderConstIterator RVDecoderCollection::Begin() const
    {
        return decoders.begin();
    }

    inline RVDecoderIterator RVDecoderCollection::End()
    {
        return decoders.end();
    }

    inline RVDecoderConstIterator RVDecoderCollection::End() const
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

    void RVDecoderCollection::AddAll(const RVDecoderCollection& collection)
    {
        RVDecoderConstIterator iter = collection.Begin();
        for (; iter != collection.End(); iter++)
            Add(*iter);
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
                iter = decoders.erase(iter);
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
                iter = decoders.erase(iter);
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
