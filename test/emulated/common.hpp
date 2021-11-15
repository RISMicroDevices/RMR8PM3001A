#pragma once
//
// Mixed emulation for common algorithm and modules
//
//  @see 'rat_freelist_checkpoint.v'
//

#include "base.hpp"

namespace MEMU::Common {

    /*pure virtual*/
    template<typename __PayloadType> 
    class BypassBuffer : public MEMU::Emulated
    {
    public:
        virtual bool    IsWritable() const = 0;
        virtual bool    IsReadable() const = 0;
        virtual bool    IsInputValid() const = 0;
        virtual bool    IsOutputValid() const = 0;

        virtual void    SetInput(__PayloadType src) = 0;
        virtual void    ResetInput() = 0;
        virtual bool    GetInput(__PayloadType& dst) const = 0;
        virtual bool    GetOutput(__PayloadType& dst) const = 0;

        virtual bool    ReadOutput(__PayloadType& dst) = 0;

        virtual void    Eval() override = 0;
    };

    template<typename __PayloadType>
    class NormalBypassBuffer final : public BypassBuffer<__PayloadType>
    {
    private:
        __PayloadType   buffer_input;
        bool            buffer_input_valid;
        __PayloadType   buffer_bypass;
        bool            buffer_bypass_valid;
        bool            buffer_readed;

    public:
        NormalBypassBuffer();
        NormalBypassBuffer(const NormalBypassBuffer<__PayloadType>& obj);
        ~NormalBypassBuffer();

        virtual bool    IsWritable() const override;
        virtual bool    IsReadable() const override;
        virtual bool    IsInputValid() const override;
        virtual bool    IsOutputValid() const override;

        virtual void    SetInput(__PayloadType src) override; // write-through exists (independent to Eval())
        virtual void    ResetInput() override;
        virtual bool    GetInput(__PayloadType& dst) const override;
        virtual bool    GetOutput(__PayloadType& dst) const override;

        virtual bool    ReadOutput(__PayloadType& dst) override;

        virtual void    Eval() override;
    };

    template<typename __PayloadType>
    class FlushableBypassBuffer final : public BypassBuffer<__PayloadType>
    {
    private:
        __PayloadType   buffer_input;
        bool            buffer_input_valid;
        __PayloadType   buffer_bypass;
        bool            buffer_bypass_valid;
        bool            buffer_flush;
        bool            buffer_readed;
        bool            buffer_flushed;
        
    public:
        FlushableBypassBuffer();
        FlushableBypassBuffer(const FlushableBypassBuffer<__PayloadType>& obj);
        ~FlushableBypassBuffer();

        virtual bool    IsWritable() const override;
        virtual bool    IsReadable() const override;
        virtual bool    IsInputValid() const override;
        virtual bool    IsOutputValid() const override;

        virtual void    SetInput(__PayloadType src) override; // write-through exists (independent to Eval())
        virtual void    ResetInput() override;
        virtual bool    GetInput(__PayloadType& dst) const override;
        virtual bool    GetOutput(__PayloadType& dst) const override;

        virtual bool    ReadOutput(__PayloadType& dst) override;

        void            Flush();

        virtual void    Eval() override;
    };

    class PseudoLRUSwap final
    {

    };

    class PseudoLRUPick final 
    {

    };
}
