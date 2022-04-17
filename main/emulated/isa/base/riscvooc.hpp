#pragma once
//
// RISC-V Instruction Set Architecture Emulator (Jasse)
//
// Out-Of-Context emulation components
//

#include "riscvdef.hpp"


#define __restricted(restriction)


namespace Jasse {


    // RISC-V General Registers Out-Of-Context container template
    template<class TArch>
    class RVGeneralRegisterSetOOC {
    public:
        virtual int         GetSize() const noexcept = 0;
        virtual bool        CheckBound(int address) const noexcept = 0;

        virtual TArch       Get(int address) const = 0;
        virtual void        Set(int address, TArch value) = 0;
    };

    // RISC-V General Registers Out-Of-Context container of XLEN=32
    using RVGeneralRegisterSet32OOC     = RVGeneralRegisterSetOOC<arch32_t>;

    // RISC-V General Registers Out-Of-Context container of XLEN=64
    using RVGeneralRegisterSet64OOC     = RVGeneralRegisterSetOOC<arch64_t>;



    // RISC-V Architectural State and Interface Out-Of-Context container
    class RVArchitecturalOOC {
    public:
        // XLEN
        virtual XLen        XLEN() const noexcept = 0;

        // PC register
        virtual pc_t        PC() const noexcept = 0;
        virtual void        SetPC(pc_t pc) noexcept = 0;

        virtual void        SetPC32(arch32_t pc) __restricted(XLEN32) = 0;
        virtual void        SetPC64(arch64_t pc) __restricted(XLEN64) = 0;

        // General register operations
        virtual bool        IsGR32() const noexcept = 0;
        virtual bool        IsGR64() const noexcept = 0;

        virtual RVGeneralRegisterSet32OOC*          GR32() __restricted(XLEN32) = 0;
        virtual const RVGeneralRegisterSet32OOC*    GR32() __restricted(XLEN32) const = 0;
        virtual RVGeneralRegisterSet64OOC*          GR64() __restricted(XLEN64) = 0;
        virtual const RVGeneralRegisterSet64OOC*    GR64() __restricted(XLEN64) const = 0;

        virtual arch64_t    GetGRx64Zext(int addr) const noexcept = 0;
        virtual arch64_t    GetGRx64Sext(int addr) const noexcept = 0;
        virtual arch32_t    GetGRx32(int addr) const noexcept = 0;

        virtual void        SetGRx64(int addr, arch64_t val64) noexcept = 0;
        virtual void        SetGRx32Zext(int addr, arch32_t val32) noexcept = 0;
        virtual void        SetGRx32Sext(int addr, arch32_t val32) noexcept = 0;
    };
}
