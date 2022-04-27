#pragma once
//
// RISC-V Instruction Set Architecture Emulator (Jasse)
//
// RISC-V Codepoint infrastructure
//

#include <string>

#include "riscvdef.hpp"
#include "riscvooc.hpp"
#include "riscvmem.hpp"
#include "riscvcsr.hpp"
#include "riscvtrap.hpp"
#include "riscvgen.hpp"

namespace Jasse {
    // RISC-V Codepoint Decode Path
    typedef bool    (*RVDecodePath)(insnraw_t insnraw, RVInstruction& insn);

    typedef struct {
        RVArchitecturalOOC*     arch;
        RVMemoryInterface*      MI;
        RVCSRSpace*             CSRs;
        RVTrapProcedures        trap;
    } RVExecContext;

    // RISC-V Codepoint
    class RVCodepoint {
    public:
        // RISC-V Instruction Textualizer
        typedef std::string     (*Textualizer)(const RVInstruction&);

        // RISC-V Instruction Executor
        typedef RVExecStatus    (*Executor)(const RVInstruction& insn, const RVExecContext& ctx);

    private:
        std::string     name;
        RVCodepointType type;

        RVDecodePath    decodepath;
        RVCodeGen       codegen;

        Executor        executor;
        Textualizer     textualizer;

    public:
        RVCodepoint(const char*                 name, 
                    const RVCodepointType       type,
                    const RVDecodePath          decodepath,
                    const RVCodeGen             codegen,
                    const Executor              executor,
                    const Textualizer           textualizer);

        RVCodepoint(const std::string&          name,
                    const RVCodepointType       type,
                    const RVDecodePath          decodepath,
                    const RVCodeGen             codegen,
                    const Executor              executor,
                    const Textualizer           textualizer);

        RVCodepoint();
        RVCodepoint(const RVCodepoint& obj);
        ~RVCodepoint();

        void                    SetName(const std::string& name);
        void                    SetType(const RVCodepointType type);
        void                    SetTextualizer(const Textualizer textualizer);
        void                    SetExecutor(const Executor executor);

        void                    SetDecodePath(const RVDecodePath decodepath);
        void                    SetCodeGen(const RVCodeGen codegen);

        const std::string&      GetName() const;
        RVCodepointType         GetType() const;
        const Textualizer       GetTextualizer() const;
        const Executor          GetExecutor() const;

        const RVDecodePath      GetDecodePath() const;
        const RVCodeGen         GetCodeGen() const;

        bool                    CompareCanonical(const RVCodepoint& obj) const;
        bool                    Compare(const RVCodepoint& obj) const;

        bool                    operator==(const RVCodepoint& obj) const;
        bool                    operator!=(const RVCodepoint& obj) const;
    };
}



// Implementation of: class RVCodepoint
namespace Jasse {
    /*
    std::string     name;
    RVCodepointType type;

    Executor        executor;
    Textualizer     textualizer;
    */

    RVCodepoint::RVCodepoint(const char*                name, 
                             const RVCodepointType      type,
                             const RVDecodePath         decodepath,
                             const RVCodeGen            codegen,
                             const Executor             executor,
                             const Textualizer          textualizer)
        : name          (std::string(name))
        , type          (type)
        , decodepath    (decodepath)
        , codegen       (codegen)
        , executor      (executor)
        , textualizer   (textualizer)
    { }

    RVCodepoint::RVCodepoint(const std::string&         name,
                             const RVCodepointType      type,
                             const RVDecodePath         decodepath,
                             const RVCodeGen            codegen,
                             const Executor             executor,
                             const Textualizer          textualizer)
        : name          (name)
        , type          (type)
        , decodepath    (decodepath)
        , codegen       (codegen)
        , executor      (executor)
        , textualizer   (textualizer)
    { }

    RVCodepoint::RVCodepoint()
        : name          ()
        , type          ()
        , decodepath    (nullptr)
        , codegen       (nullptr)
        , executor      (nullptr)
        , textualizer   (nullptr)
    { }

    RVCodepoint::RVCodepoint(const RVCodepoint& obj)
        : name          (obj.name)
        , type          (obj.type)
        , decodepath    (obj.decodepath)
        , codegen       (obj.codegen)
        , executor      (obj.executor)
        , textualizer   (obj.textualizer)
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

    inline void RVCodepoint::SetDecodePath(const RVDecodePath decodepath)
    {
        this->decodepath = decodepath;
    }

    inline void RVCodepoint::SetCodeGen(const RVCodeGen codegen)
    {
        this->codegen = codegen;
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

    inline const RVDecodePath RVCodepoint::GetDecodePath() const
    {
        return decodepath;
    }

    inline const RVCodeGen RVCodepoint::GetCodeGen() const
    {
        return codegen;
    }

    bool RVCodepoint::CompareCanonical(const RVCodepoint& obj) const
    {
        if (name != obj.name)
            return false;

        if (type != obj.type)
            return false;

        return true;
    }

    bool RVCodepoint::Compare(const RVCodepoint& obj) const
    {
        if (!CompareCanonical(obj))
            return false;

        if (textualizer != obj.textualizer)
            return false;

        if (executor != obj.executor)
            return false;

        return true;
    }

    inline bool RVCodepoint::operator==(const RVCodepoint& obj) const
    {
        return Compare(obj);
    }

    inline bool RVCodepoint::operator!=(const RVCodepoint& obj) const
    {
        return !(*this == obj);
    }
}

