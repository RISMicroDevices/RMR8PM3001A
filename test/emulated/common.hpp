#pragma once
//
// Mixed emulation for common algorithm and modules
//
//  @see 'rat_freelist_checkpoint.v'
//

#include <bitset>

#include "base.hpp"

using namespace std;

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
        virtual bool    GetInput(__PayloadType* dst) const = 0;
        virtual bool    GetOutput(__PayloadType* dst) const = 0;

        virtual bool    ReadOutput(__PayloadType* dst) = 0;

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
        virtual bool    GetInput(__PayloadType* dst) const override;
        virtual bool    GetOutput(__PayloadType* dst) const override;

        virtual bool    ReadOutput(__PayloadType* dst) override;

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
        bool            buffer_flushed;
        bool            buffer_readed;
        
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
        virtual bool    GetInput(__PayloadType* dst) const override;
        virtual bool    GetOutput(__PayloadType* dst) const override;

        virtual bool    ReadOutput(__PayloadType* dst) override;

        void            Flush();

        virtual void    Eval() override;
    };

    template<int __size_log2>
    class PseudoLRUSwap final : public MEMU::Emulated
    {
    private:
        constexpr static int    lru_bit_count = (1 << __size_log2) - 1;
        bitset<lru_bit_count>   lru_bits;
        int                     lru_picked_index;
        int                     lru_update_index;

    public:
        PseudoLRUSwap();
        PseudoLRUSwap(const PseudoLRUSwap<__size_log2>& obj);
        ~PseudoLRUSwap();

        constexpr int   GetSize() const;
        int             GetPicked() const;
        bool            IsPicked(int index) const;

        void            Update(int index);

        virtual void    Eval() override = 0;
    };


    class PseudoLRUPick final 
    {

    };
}


// class MEMU::Common::NormalBypassBuffer
namespace MEMU::Common {
    /*
    __PayloadType   buffer_input;
    bool            buffer_input_valid;
    __PayloadType   buffer_bypass;
    bool            buffer_bypass_valid;
    bool            buffer_readed;
    */

    template<typename __PayloadType>
    NormalBypassBuffer<__PayloadType>::NormalBypassBuffer()
        : buffer_input_valid(false)
        , buffer_bypass_valid(false)
        , buffer_readed(false)
    { }

    template<typename __PayloadType>
    NormalBypassBuffer<__PayloadType>::NormalBypassBuffer(const NormalBypassBuffer<__PayloadType>& obj)
        : buffer_input_valid(false)
        , buffer_bypass_valid(obj.buffer_bypass_valid)
        , buffer_bypass(obj.buffer_bypass)
        , buffer_readed(false)
    { }

    template<typename __PayloadType>
    NormalBypassBuffer<__PayloadType>::~NormalBypassBuffer()
    { }

    template<typename __PayloadType>
    bool NormalBypassBuffer<__PayloadType>::IsWritable() const
    {
        return !buffer_bypass_valid;
    }

    template<typename __PayloadType>
    bool NormalBypassBuffer<__PayloadType>::IsReadable() const
    {
        return buffer_input_valid || buffer_bypass_valid;
    }

    template<typename __PayloadType>
    bool NormalBypassBuffer<__PayloadType>::IsInputValid() const
    {
        return buffer_input_valid;
    }

    template<typename __PayloadType>
    bool NormalBypassBuffer<__PayloadType>::IsOutputValid() const
    {
        return buffer_input_valid || buffer_bypass_valid;
    }

    template<typename __PayloadType>
    void NormalBypassBuffer<__PayloadType>::SetInput(__PayloadType src) 
    {
        buffer_input        = src;
        buffer_input_valid  = true;
    }

    template<typename __PayloadType>
    void NormalBypassBuffer<__PayloadType>::ResetInput()
    {
        buffer_input_valid  = false;
    }

    template<typename __PayloadType>
    bool NormalBypassBuffer<__PayloadType>::GetInput(__PayloadType* dst) const
    {
        if (buffer_input_valid)
        {
            *dst = buffer_input;
            return true;
        }

        return false;
    }

    template<typename __PayloadType>
    bool NormalBypassBuffer<__PayloadType>::GetOutput(__PayloadType* dst) const
    {
        if (buffer_bypass_valid)
        {
            *dst = buffer_bypass;
            return true;
        }
        else if (buffer_input_valid)
        {
            *dst = buffer_input;
            return true;
        }

        return false;
    }

    template<typename __PayloadType>
    bool NormalBypassBuffer<__PayloadType>::ReadOutput(__PayloadType* dst)
    {
        buffer_readed = true;

        return GetOutput(dst);
    }

    template<typename __PayloadType>
    void NormalBypassBuffer<__PayloadType>::Eval()
    {
        if (buffer_readed)
        {
            buffer_bypass_valid = false;
        }
        else if (buffer_input_valid && !buffer_bypass_valid)
        {
            buffer_bypass       = buffer_input;
            buffer_bypass_valid = true;
        }

        buffer_readed = false;
    }
};


// class MEMU::Common::FlushableBypassBuffer
namespace MEMU::Common {
    /*
    __PayloadType   buffer_input;
    bool            buffer_input_valid;
    __PayloadType   buffer_bypass;
    bool            buffer_bypass_valid;
    bool            buffer_flush;
    bool            buffer_readed;
    bool            buffer_flushed;
    */

    template<typename __PayloadType>
    FlushableBypassBuffer<__PayloadType>::FlushableBypassBuffer()
        : buffer_input_valid(false)
        , buffer_bypass_valid(false)
        , buffer_flush(false)
        , buffer_flushed(false)
        , buffer_readed(false)
    { }

    template<typename __PayloadType>
    FlushableBypassBuffer<__PayloadType>::FlushableBypassBuffer(const FlushableBypassBuffer& obj)
        : buffer_input_valid(false)
        , buffer_bypass_valid(obj.buffer_bypass_valid)
        , buffer_bypass(obj.buffer_bypass)
        , buffer_flush(false)
        , buffer_flushed(false)
        , buffer_readed(false)
    { }

    template<typename __PayloadType>
    FlushableBypassBuffer<__PayloadType>::~FlushableBypassBuffer()
    { }

    template<typename __PayloadType>
    bool FlushableBypassBuffer<__PayloadType>::IsWritable() const
    {
        return !buffer_bypass_valid && !buffer_flush;
    }

    template<typename __PayloadType>
    bool FlushableBypassBuffer<__PayloadType>::IsReadable() const
    {
        return (buffer_bypass_valid || buffer_input_valid) && !buffer_flush;
    }

    template<typename __PayloadType>
    bool FlushableBypassBuffer<__PayloadType>::IsInputValid() const
    {
        return buffer_input_valid;
    }

    template<typename __PayloadType>
    bool FlushableBypassBuffer<__PayloadType>::IsOutputValid() const
    {
        return (buffer_bypass_valid || buffer_input_valid) && !buffer_flush;
    }

    template<typename __PayloadType>
    void FlushableBypassBuffer<__PayloadType>::SetInput(__PayloadType src)
    {
        buffer_input        = src;
        buffer_input_valid  = true;
    }

    template<typename __PayloadType>
    void FlushableBypassBuffer<__PayloadType>::ResetInput()
    {
        buffer_input_valid  = false;
    }

    template<typename __PayloadType>
    bool FlushableBypassBuffer<__PayloadType>::GetInput(__PayloadType* dst) const
    {
        if (buffer_input_valid)
        {
            *dst = buffer_input;
            return true;
        }

        return false;
    }

    template<typename __PayloadType>
    bool FlushableBypassBuffer<__PayloadType>::GetOutput(__PayloadType* dst) const
    {
        if (buffer_flush)
        {
            return false;
        }
        else if (buffer_bypass_valid)
        {
            *dst = buffer_bypass;
            return true;
        }
        else if (buffer_input_valid)
        {
            *dst = buffer_input;
            return true;
        }

        return false;
    }

    template<typename __PayloadType>
    bool FlushableBypassBuffer<__PayloadType>::ReadOutput(__PayloadType* dst)
    {
        buffer_readed = true;

        return GetOutput(dst);
    }

    template<typename __PayloadType>
    void FlushableBypassBuffer<__PayloadType>::Flush()
    {
        buffer_flushed = true;
    }

    template<typename __PayloadType>
    void FlushableBypassBuffer<__PayloadType>::Eval()
    {
        if (buffer_flushed || buffer_readed)
        {
            buffer_bypass_valid = false;
        }
        else if (buffer_input_valid && !buffer_bypass_valid)
        {
            buffer_bypass       = buffer_input;
            buffer_bypass_valid = true;
        }

        buffer_flush    = buffer_flushed;

        buffer_readed   = false;
        buffer_flushed  = false;
    }
};


// class MEMU::Common::PseudoLRUSwap
namespace MEMU::Common {
    /*
    constexpr int           lru_bit_count = (1 << __size_log2) - 1;
    bitset<lru_bit_count>   lru_bits;
    int                     lru_picked_index;
    int                     lru_update_index;
    */

    template<int __size_log2>
    PseudoLRUSwap<__size_log2>::PseudoLRUSwap()
        : lru_picked_index(0)
        , lru_update_index(-1)
    { }

    template<int __size_log2>
    PseudoLRUSwap<__size_log2>::PseudoLRUSwap(const PseudoLRUSwap<__size_log2>& obj)
        : lru_bits(obj.lru_bits)
        , lru_picked_index(0)
        , lru_update_index(-1)
    { }

    template<int __size_log2>
    PseudoLRUSwap<__size_log2>::~PseudoLRUSwap()
    { }

    template<int __size_log2>
    constexpr int PseudoLRUSwap<__size_log2>::GetSize() const
    {
        return lru_bit_count + 1;
    }

    template<int __size_log2>
    int PseudoLRUSwap<__size_log2>::GetPicked() const
    {
        return lru_picked_index;        
    }

    template<int __size_log2>
    bool PseudoLRUSwap<__size_log2>::IsPicked(int index) const
    {
        return lru_picked_index == index;
    }

    template<int __size_log2>
    void PseudoLRUSwap<__size_log2>::Update(int index)
    {
        lru_update_index = index;
    }
    
    template<int __size_log2>
    void PseudoLRUSwap<__size_log2>::Eval()
    {
        // 
        if (lru_update_index >= 0)
        {
            for (int i = 0; i < __size_log2; i++)
            {
                int pbase = (1 << (__size_log2 - i - 1)) - 1;

                int lru = !((lru_update_index >>> i) & 0x01);
                int index = (lru_update_index >>> (i + 1));

                lru_bits[pbase + index] = lru;
            }

            lru_update_index = -1;
        }

        //
        int picked = 0;

        for (int i = 0; i < __size_log2; i++)
        {
            int nbase = (1 << (i + 1)) - 1;
            int pbase = (1 << i) - 1;

            int lru = lru_bits[picked];

            picked = nbase + (picked - pbase) * 2 + lru;
        }

        picked -= lru_bit_count;

        lru_picked_index = picked;
    }

}
