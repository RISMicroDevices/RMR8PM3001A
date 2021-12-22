#pragma once
//
// Mixed emulation for common algorithm and modules
//
//

#include <cstring>
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

    //
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

    //
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

    //
    template<int __size_log2>
    class PseudoLRUSwap final : public MEMU::Emulated
    {
    private:
        constexpr static int    lru_bit_count = (1 << __size_log2) - 1;
        constexpr static int    lru_sub_count = (1 << __size_log2);

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

        virtual void    Eval() override;
    };

    //
    template<int __size_log2>
    class PseudoLRUPick final : public MEMU::Emulated
    {
    private:
        constexpr static int    lru_bit_count = (1 << __size_log2) - 1;
        constexpr static int    lru_sub_count = (1 << __size_log2);

        bitset<lru_bit_count>   lru_bits;
        int                     lru_update_index;

        int                     lru_sub[lru_sub_count] = { -1 };
        int                     lru_sub_picked_index;

    public:
        PseudoLRUPick();
        PseudoLRUPick(const PseudoLRUPick<__size_log2>& obj);
        ~PseudoLRUPick();
        
        constexpr int   GetSize() const;
        int             GetPicked() const;
        bool            IsPicked(int index) const;

        void            Update(int index);

        void            SetValid(int index);
        void            ResetValid();

        virtual void    Eval() override;
    };

    // Handle of Delayed Memory Read Operation
    template<typename __PayloadType>
    class DelayedMemoryRead final {
        private:
            DelayedMemoryRead<__PayloadType>*   next;

            int                                 target_port;
            int                                 target_address;
            __PayloadType*                      target_destination;

        public:
            DelayedMemoryRead();
            DelayedMemoryRead(int port, int addr, __PayloadType* dst);
            DelayedMemoryRead(const DelayedMemoryRead& obj);
            ~DelayedMemoryRead();

            void                                Reset();

            bool                                IsValid() const;

            DelayedMemoryRead<__PayloadType>*   GetNext() const;
            void                                SetNext(DelayedMemoryRead<__PayloadType>* next);

            int                                 GetPort() const;
            int                                 GetAddress() const;
            __PayloadType*                      GetDestination() const;

            void                                SetPort(int port);
            void                                SetAddress(int addr);
            void                                SetDestination(__PayloadType* dst);
    };

    // Snapshot of Delayed Memory Read Operation
    template<typename __PayloadType>
    class MemoryReadSnapshot final {

    };

    // Snapshot of Delayed Memory Write Operation
    template<typename __PayloadType>
    class MemoryWriteSnapshot final {

    };

    // ENUM Memory Read Mode
    //
    typedef enum __tag_MemoryReadMode 
    {
        READ_FIRST = 0,
        WRITE_FIRST

    } MemoryReadMode;

    // Interface of Read Through Random Access Memory
    // *pure virtual*
    template<typename __PayloadType>
    class ReadThroughRandomAccessMemory
    {
    public:
        virtual void    ReadThrough(int addr, __PayloadType* dst) const = 0;
    };

    // Interface of Read Delayed Random Access Memory
    // *pure virtual*
    template<typename __PayloadType>
    class ReadDelayedRandomAccessMemory
    {
    public:
        virtual void    ReadDelayed(int port, int addr, __PayloadType* dst) = 0;
    };

    // Single write port, read through
    //
    template<typename __PayloadType>
    class W1RTRandomAccessMemory final : public MEMU::Emulated, public ReadThroughRandomAccessMemory<__PayloadType>
    {
    private:
        __PayloadType*          memory;
        const bool              memory_inalloc;
        const int               size;
        const MemoryReadMode    rmode;

        int                     write_p0_address;
        __PayloadType           write_p0_payload;

    public:
        W1RTRandomAccessMemory(__PayloadType* memory, const int size, const MemoryReadMode rmode);
        W1RTRandomAccessMemory(const int size, const MemoryReadMode rmode);
        W1RTRandomAccessMemory(const W1RTRandomAccessMemory<__PayloadType>& obj);
        ~W1RTRandomAccessMemory();

        int             GetSize() const;
        MemoryReadMode  GetReadMode() const;
        __PayloadType*  GetMedium() const;
        bool            CheckBound(int address) const;

        virtual void    ReadThrough(int address, __PayloadType* dst) const override;
            
        void            SetWrite(int address, __PayloadType src);
        void            ResetWrite();

        void            Eval() override;
    };

    // Dual write ports, read through
    //
    class W2RTRandomAccessMemory final : public MEMU::Emulated
    {
        // not implemented
    };

    // Multiple write ports, read through
    //
    class WnRTRandomAccessMemory final : public MEMU::Emulated
    {
        // not implemented
    };

    // Single write port, read delayed
    //
    template<typename __PayloadType>
    class W1RDRandomAccessMemory final : public MEMU::Emulated, public ReadDelayedRandomAccessMemory<__PayloadType>
    {
    private:
        __PayloadType*                      memory;
        const bool                          memory_inalloc;
        const int                           size;
        const MemoryReadMode                rmode;

        int                                 write_p0_address;
        __PayloadType                       write_p0_payload;

        DelayedMemoryRead<__PayloadType>*   rop_head;
        DelayedMemoryRead<__PayloadType>*   rop_tail;

        void            EvalRead();
        void            EvalWrite();

    public:
        W1RDRandomAccessMemory(__PayloadType* memory, const int size, const MemoryReadMode rmode);
        W1RDRandomAccessMemory(const int size, const MemoryReadMode rmode);
        W1RDRandomAccessMemory(const W1RDRandomAccessMemory<__PayloadType>& obj);
        ~W1RDRandomAccessMemory();

        int             GetSize() const;
        MemoryReadMode  GetReadMode() const;
        __PayloadType*  GetMedium() const;
        bool            CheckBound(int address) const;

        virtual void    ReadDelayed(int port, int address, __PayloadType* dst) override;
        void            ResetReadPort(int port);
        void            ResetRead();

        void            SetWrite(int address, __PayloadType src);
        void            ResetWrite();

        void            Eval() override;
    };

    // Dual write ports, read delayed
    //
    class W2RDRandomAccessMemory final : public MEMU::Emulated
    {
        // not implemented
    };

    // Multiple write ports, read delayed
    //
    class WnRDRandomAccessMemory final : public MEMU::Emulated
    {
        // not implemented
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
        : buffer_input_valid    (false)
        , buffer_bypass_valid   (false)
        , buffer_readed         (false)
    { }

    template<typename __PayloadType>
    NormalBypassBuffer<__PayloadType>::NormalBypassBuffer(const NormalBypassBuffer<__PayloadType>& obj)
        : buffer_input_valid    (false)
        , buffer_bypass_valid   (obj.buffer_bypass_valid)
        , buffer_bypass         (obj.buffer_bypass)
        , buffer_readed         (false)
    { }

    template<typename __PayloadType>
    NormalBypassBuffer<__PayloadType>::~NormalBypassBuffer()
    { }

    template<typename __PayloadType>
    inline bool NormalBypassBuffer<__PayloadType>::IsWritable() const
    {
        return !buffer_bypass_valid;
    }

    template<typename __PayloadType>
    inline bool NormalBypassBuffer<__PayloadType>::IsReadable() const
    {
        return buffer_input_valid || buffer_bypass_valid;
    }

    template<typename __PayloadType>
    inline bool NormalBypassBuffer<__PayloadType>::IsInputValid() const
    {
        return buffer_input_valid;
    }

    template<typename __PayloadType>
    inline bool NormalBypassBuffer<__PayloadType>::IsOutputValid() const
    {
        return buffer_input_valid || buffer_bypass_valid;
    }

    template<typename __PayloadType>
    inline void NormalBypassBuffer<__PayloadType>::SetInput(__PayloadType src) 
    {
        buffer_input        = src;
        buffer_input_valid  = true;
    }

    template<typename __PayloadType>
    inline void NormalBypassBuffer<__PayloadType>::ResetInput()
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
        : buffer_input_valid    (false)
        , buffer_bypass_valid   (false)
        , buffer_flush          (false)
        , buffer_flushed        (false)
        , buffer_readed         (false)
    { }

    template<typename __PayloadType>
    FlushableBypassBuffer<__PayloadType>::FlushableBypassBuffer(const FlushableBypassBuffer& obj)
        : buffer_input_valid    (false)
        , buffer_bypass_valid   (obj.buffer_bypass_valid)
        , buffer_bypass         (obj.buffer_bypass)
        , buffer_flush          (false)
        , buffer_flushed        (false)
        , buffer_readed         (false)
    { }

    template<typename __PayloadType>
    FlushableBypassBuffer<__PayloadType>::~FlushableBypassBuffer()
    { }

    template<typename __PayloadType>
    inline bool FlushableBypassBuffer<__PayloadType>::IsWritable() const
    {
        return !buffer_bypass_valid && !buffer_flush;
    }

    template<typename __PayloadType>
    inline bool FlushableBypassBuffer<__PayloadType>::IsReadable() const
    {
        return (buffer_bypass_valid || buffer_input_valid) && !buffer_flush;
    }

    template<typename __PayloadType>
    inline bool FlushableBypassBuffer<__PayloadType>::IsInputValid() const
    {
        return buffer_input_valid;
    }

    template<typename __PayloadType>
    inline bool FlushableBypassBuffer<__PayloadType>::IsOutputValid() const
    {
        return (buffer_bypass_valid || buffer_input_valid) && !buffer_flush;
    }

    template<typename __PayloadType>
    inline void FlushableBypassBuffer<__PayloadType>::SetInput(__PayloadType src)
    {
        buffer_input        = src;
        buffer_input_valid  = true;
    }

    template<typename __PayloadType>
    inline void FlushableBypassBuffer<__PayloadType>::ResetInput()
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


// class MEMU::Common::PseudoLRUSwap /* TODO: SLOW EMULATION. To be optimized.  */
namespace MEMU::Common {
    /*
    constexpr int           lru_bit_count = (1 << __size_log2) - 1;
    bitset<lru_bit_count>   lru_bits;
    int                     lru_picked_index;
    int                     lru_update_index;
    */

    template<int __size_log2>
    PseudoLRUSwap<__size_log2>::PseudoLRUSwap()
        : lru_picked_index  (-1)
        , lru_update_index  (-1)
    { 
        Eval();
    }

    template<int __size_log2>
    PseudoLRUSwap<__size_log2>::PseudoLRUSwap(const PseudoLRUSwap<__size_log2>& obj)
        : lru_bits          (obj.lru_bits)
        , lru_picked_index  (-1)
        , lru_update_index  (-1)
    { 
        Eval();
    }

    template<int __size_log2>
    PseudoLRUSwap<__size_log2>::~PseudoLRUSwap()
    { }

    template<int __size_log2>
    constexpr int PseudoLRUSwap<__size_log2>::GetSize() const
    {
        return lru_sub_count;
    }

    template<int __size_log2>
    inline int PseudoLRUSwap<__size_log2>::GetPicked() const
    {
        return lru_picked_index;        
    }

    template<int __size_log2>
    inline bool PseudoLRUSwap<__size_log2>::IsPicked(int index) const
    {
        return lru_picked_index == index;
    }

    template<int __size_log2>
    inline void PseudoLRUSwap<__size_log2>::Update(int index)
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

                int lru = !(((unsigned) lru_update_index >> i) & 0x01);
                int index = ((unsigned) lru_update_index >> (i + 1));

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

            picked = nbase + ((picked - pbase) << 1) + lru;
        }

        picked -= lru_bit_count;

        lru_picked_index = picked;
    }

}


// class MEMU::Common::PseudoLRUPick /* TODO: SLOW EMULATION. To be optimized.  */
namespace MEMU::Common {
    /*
    constexpr static int    lru_bit_count = (1 << __size_log2) - 1;
    constexpr static int    lru_sub_count = (1 << __size_log2);

    bitset<lru_bit_count>   lru_bits;
    int                     lru_update_index;
        
    int                     lru_sub[lru_sub_count];
    int                     lru_sub_picked_index;
    */

    template<int __size_log2>
    PseudoLRUPick<__size_log2>::PseudoLRUPick()
        : lru_update_index      (-1)
        , lru_sub_picked_index  (-1)
    { 
        Eval();
    }

    template<int __size_log2>
    PseudoLRUPick<__size_log2>::PseudoLRUPick(const PseudoLRUPick<__size_log2>& obj)
        : lru_bits              (obj.lru_bits)
        , lru_update_index      (-1)
        , lru_sub_picked_index  (-1)
    {  
        Eval();
    }

    template<int __size_log2>
    PseudoLRUPick<__size_log2>::~PseudoLRUPick()
    {  }

    template<int __size_log2>
    constexpr int PseudoLRUPick<__size_log2>::GetSize() const
    {
        return lru_sub_count;
    }

    template<int __size_log2>
    int PseudoLRUPick<__size_log2>::GetPicked() const
    {
        if (lru_sub_picked_index < lru_sub_count)
            return lru_sub[lru_sub_picked_index];

        return false;
    }

    template<int __size_log2>
    bool PseudoLRUPick<__size_log2>::IsPicked(int index) const
    {
        if (lru_sub_picked_index < lru_sub_count)
            return lru_sub[lru_sub_picked_index] == index;
        
        return false;
    }

    template<int __size_log2>
    inline void PseudoLRUPick<__size_log2>::Update(int index)
    {
        lru_update_index = index;
    }

    template<int __size_log2>
    void PseudoLRUPick<__size_log2>::SetValid(int index)
    {
        for (int i = 0; i < lru_sub_picked_index; i++)
            if (lru_sub[i] == index)
            {
                lru_sub_picked_index = i;
                break;
            }
    }

    template<int __size_log2>
    inline void PseudoLRUPick<__size_log2>::ResetValid()
    {
        lru_sub_picked_index = lru_sub_count;
    }

    template<int __size_log2>
    void PseudoLRUPick<__size_log2>::Eval()
    {
        // 
        if (lru_update_index >= 0)
        {
            for (int i = 0; i < __size_log2; i++)
            {
                int pbase = (1 << (__size_log2 - i - 1)) - 1;

                int lru = !(((unsigned) lru_update_index >> i) & 0x01);
                int index = ((unsigned) lru_update_index >> (i + 1));

                lru_bits[pbase + index] = lru;
            }

            lru_update_index = -1;
        }

        //
        lru_sub[0] =  lru_bits[0];
        lru_sub[1] = !lru_bits[0];

        for (int i = 1; i < __size_log2; i++)
        {
            int psize = 1 << i;
            int pbase = psize - 1;

            for (int j = 0; j < psize; j++)
            {
                int k = psize - j - 1; // reverse

                int lru = lru_bits[pbase + lru_sub[k]];

                //
                if (!lru)
                {
                    lru_sub[(k << 1) + 1] = (lru_sub[k] << 1) + 1;
                    lru_sub[(k << 1)]     = (lru_sub[k] << 1);
                }
                else
                {
                    lru_sub[(k << 1) + 1] = (lru_sub[k] << 1);
                    lru_sub[(k << 1)]     = (lru_sub[k] << 1) + 1;
                }
            }
        }

        lru_sub_picked_index = lru_sub_count;
    }
}


// class MEMU::Common::DelayedMemoryRead
namespace MEMU::Common {
    /*
    DelayedMemoryRead<__PayloadType>*   next;

    int                                 target_port;
    int                                 target_address;
    __PayloadType*                      target_destination;
    */

    template<typename __PayloadType>
    DelayedMemoryRead<__PayloadType>::DelayedMemoryRead()
        : next              (nullptr)
        , target_port       (-1)
        , target_address    (-1)
        , target_destination(nullptr)
    { }

    template<typename __PayloadType>
    DelayedMemoryRead<__PayloadType>::DelayedMemoryRead(int port, int addr, __PayloadType* dst)
        : next              (nullptr)
        , target_port       (port)
        , target_address    (addr)
        , target_destination(dst)
    { }

    template<typename __PayloadType>
    DelayedMemoryRead<__PayloadType>::DelayedMemoryRead(const DelayedMemoryRead& obj)
        : next              (nullptr)
        , target_port       (obj,target_port)
        , target_address    (obj.target_address)
        , target_destination(obj.target_destination)
    { }

    template<typename __PayloadType>
    inline void DelayedMemoryRead<__PayloadType>::Reset()
    {
        target_port         = -1;
        target_address      = -1;
        target_destination  = nullptr;
    }

    template<typename __PayloadType>
    inline bool DelayedMemoryRead<__PayloadType>::IsValid() const
    {
        return target_address >= 0;
    }

    template<typename __PayloadType>
    inline DelayedMemoryRead<__PayloadType>* DelayedMemoryRead<__PayloadType>::GetNext() const
    {
        return next;
    }

    template<typename __PayloadType>
    inline void DelayedMemoryRead<__PayloadType>::SetNext(DelayedMemoryRead<__PayloadType>* next)
    {
        this->next = next;
    }

    template<typename __PayloadType>
    inline int DelayedMemoryRead<__PayloadType>::GetPort() const
    {
        return target_port;
    }

    template<typename __PayloadType>
    inline int DelayedMemoryRead<__PayloadType>::GetAddress() const
    {
        return target_address;
    }

    template<typename __PayloadType>
    inline __PayloadType* DelayedMemoryRead<__PayloadType>::GetDestination() const
    {
        return target_destination;
    }

    template<typename __PayloadType>
    inline void DelayedMemoryRead<__PayloadType>::SetPort(int port)
    {
        this->target_port = port;
    }

    template<typename __PayloadType>
    inline void DelayedMemoryRead<__PayloadType>::SetAddress(int addr)
    {
        this->target_address = addr;
    }

    template<typename __PayloadType>
    inline void DelayedMemoryRead<__PayloadType>::SetDestination(__PayloadType* dst)
    {
        this->target_destination = dst;
    }
}


// class MEMU::Common::W1RTRandomAccessMemory
namespace MEMU::Common {
    /*
    __PayloadType*          memory;
    const bool              memory_inalloc;
    const int               size;
    const MemoryReadMode    rmode;

    int                     write_p0_address;
    __PayloadType           write_p0_payload;
    */

    template<typename __PayloadType>
    W1RTRandomAccessMemory<__PayloadType>::W1RTRandomAccessMemory(__PayloadType* memory, const int size, const MemoryReadMode rmode)
        : size              (size)
        , rmode             (rmode)
        , memory            (memory)
        , memory_inalloc    (false)
        , write_p0_address  (-1)
    { }

    template<typename __PayloadType>
    W1RTRandomAccessMemory<__PayloadType>::W1RTRandomAccessMemory(const int size, const MemoryReadMode rmode)
        : size              (size)
        , rmode             (rmode)
        , memory            (new __PayloadType[size])
        , memory_inalloc    (true)
        , write_p0_address  (-1)
    { }

    template<typename __PayloadType>
    W1RTRandomAccessMemory<__PayloadType>::W1RTRandomAccessMemory(const W1RTRandomAccessMemory<__PayloadType>& obj)
        : size              (obj.size)
        , rmode             (obj.rmode)
        , memory            (new __PayloadType[obj.size])
        , memory_inalloc    (true)
        , write_p0_address  (-1)
    { 
        memcpy(memory, obj.memory, obj.size * sizeof(__PayloadType));
    }

    template<typename __PayloadType>
    W1RTRandomAccessMemory<__PayloadType>::~W1RTRandomAccessMemory()
    {
        if (memory_inalloc)
            delete[] memory;
    }

    template<typename __PayloadType>
    inline int W1RTRandomAccessMemory<__PayloadType>::GetSize() const
    {
        return size;
    }

    template<typename __PayloadType>
    inline MemoryReadMode W1RTRandomAccessMemory<__PayloadType>::GetReadMode() const
    {
        return rmode;
    }

    template<typename __PayloadType>
    inline __PayloadType* W1RTRandomAccessMemory<__PayloadType>::GetMedium() const
    {
        return memory;
    }

    template<typename __PayloadType>
    inline bool W1RTRandomAccessMemory<__PayloadType>::CheckBound(int address) const
    {
        return address >= 0 && address < size;
    }

    template<typename __PayloadType>
    void W1RTRandomAccessMemory<__PayloadType>::ReadThrough(int address, __PayloadType* dst) const
    {
        switch (rmode)
        {
            case READ_FIRST:
                *dst = memory[address];
                break;

            case WRITE_FIRST:
                if (address == write_p0_address)
                    *dst = write_p0_payload;
                else
                    *dst = memory[address];
                break;

            default:
                break;
        }
    }

    template<typename __PayloadType>
    void W1RTRandomAccessMemory<__PayloadType>::SetWrite(int address, __PayloadType src)
    {
        write_p0_address = address;
        write_p0_payload = payload;
    }

    template<typename __PayloadType>
    void W1RTRandomAccessMemory<__PayloadType>::ResetWrite()
    {
        write_p0_address = -1;
    }

    template<typename __PayloadType>
    void W1RTRandomAccessMemory<__PayloadType>::Eval()
    {
        if (write_p0_address >= 0)
            memory[write_p0_address] = write_p0_payload;

        write_p0_address = -1;
    }
}


// class MEMU::Common::W1RDRandomAccessMemory
namespace MEMU::Common {
    /*
    __PayloadType*                      memory;
    const bool                          memory_inalloc;
    const int                           size;
    const MemoryReadMode                rmode;

    int                                 write_p0_address;
    __PayloadType                       write_p0_payload;

    DelayedMemoryRead<__PayloadType>*   rop_head;
    DelayedMemoryRead<__PayloadType>*   rop_tail;
    */

    template<typename __PayloadType>
    W1RDRandomAccessMemory<__PayloadType>::W1RDRandomAccessMemory(__PayloadType* memory, const int size, const MemoryReadMode rmode)
        : size              (size)
        , rmode             (rmode)
        , memory            (memory)
        , memory_inalloc    (false)
        , write_p0_address  (-1)
        , rop_head          (new DelayedMemoryRead<__PayloadType>)
        , rop_tail          (nullptr)
    { }

    template<typename __PayloadType>
    W1RDRandomAccessMemory<__PayloadType>::W1RDRandomAccessMemory(const int size, const MemoryReadMode rmode)
        : size              (size)
        , rmode             (rmode)
        , memory            (new __PayloadType[size])
        , memory_inalloc    (true)
        , write_p0_address  (-1)
        , rop_head          (new DelayedMemoryRead<__PayloadType>)
        , rop_tail          (nullptr)
    { }

    template<typename __PayloadType>
    W1RDRandomAccessMemory<__PayloadType>::W1RDRandomAccessMemory(const W1RDRandomAccessMemory<__PayloadType>& obj)
        : size              (obj.size)
        , rmode             (obj.rmode)
        , memory            (new __PayloadType[obj.size])
        , memory_inalloc    (true)
        , write_p0_address  (-1)
        , rop_head          (new DelayedMemoryRead<__PayloadType>)
        , rop_tail          (nullptr)
    {
        memcpy(memory, obj.memory, obj.size * sizeof(__PayloadType));
    }

    template<typename __PayloadType>
    W1RDRandomAccessMemory<__PayloadType>::~W1RDRandomAccessMemory()
    {
        if (memory_inalloc)
            delete[] memory;

        //
        DelayedMemoryRead<__PayloadType>* rop = rop_head;
        while (rop)
        {
            DelayedMemoryRead<__PayloadType>* current_rop = rop;

            rop = current_rop.next;
            delete current_rop;
        }
    }

    template<typename __PayloadType>
    inline int W1RDRandomAccessMemory<__PayloadType>::GetSize() const
    {
        return size;
    }

    template<typename __PayloadType>
    inline MemoryReadMode W1RDRandomAccessMemory<__PayloadType>::GetReadMode() const
    {
        return rmode;
    }

    template<typename __PayloadType>
    inline __PayloadType* W1RDRandomAccessMemory<__PayloadType>::GetMedium() const
    {
        return memory;
    }

    template<typename __PayloadType>
    inline bool W1RDRandomAccessMemory<__PayloadType>::CheckBound(int address) const
    {
        return address >= 0 && address < size;
    }

    template<typename __PayloadType>
    void W1RDRandomAccessMemory<__PayloadType>::ReadDelayed(int port, int addr, __PayloadType* dst)
    {
        DelayedMemoryRead<__PayloadType>* rop_new;

        if (!rop_tail)
            rop_new = rop_head;
        else if (rop_tail->GetNext())
            rop_new = rop_tail->GetNext();
        else
            rop_tail->SetNext(rop_new = new DelayedMemoryRead<__PayloadType>);

        rop_new->SetPort(port);
        rop_new->SetAddress(address);
        rop_new->SetDestination(dst);

        rop_tail = rop_new;
    }

    template<typename __PayloadType>
    void W1RDRandomAccessMemory<__PayloadType>::ResetReadPort(int port)
    {
        if (!top_tail)
            return;

        DelayedMemoryRead<__PayloadType>* rop = rop_head;
        while (1)
        {
            if (rop->GetPort() == port)
                rop->Reset();

            if (rop == rop_tail)
                break;

            rop = rop->GetNext();

            if (!rop)
                break;
        }

        rop_tail = nullptr;
    }

    template<typename __PayloadType>
    void W1RDRandomAccessMemory<__PayloadType>::ResetRead()
    {
        if (!rop_tail)
            return;

        DelayedMemoryRead<__PayloadType>* rop = rop_head;
        while (1)
        {
            rop->Reset();

            if (rop == rop_tail)
                break;

            rop = rop->GetNext();

            if (!rop)
                break;
        }

        rop_tail = nullptr;
    }

    template<typename __PayloadType>
    void W1RDRandomAccessMemory<__PayloadType>::SetWrite(int addr, __PayloadType src)
    {
        write_p0_address = addr;
        write_p0_payload = src;
    }

    template<typename __PayloadType>
    void W1RDRandomAccessMemory<__PayloadType>::ResetWrite()
    {
        write_p0_address = -1;
    }

    template<typename __PayloadType>
    void W1RDRandomAccessMemory<__PayloadType>::Eval()
    {
        switch (rmode)
        {
            case READ_FIRST:
                EvalRead();
                EvalWrite();
                break;

            case WRITE_FIRST:
                EvalWrite();
                EvalRead();
                break;

            default:
                break;
        }
    }

    // private:
    template<typename __PayloadType>
    void W1RDRandomAccessMemory<__PayloadType>::EvalRead()
    {
        DelayedMemoryRead<__PayloadType>* rop = rop_head;

        if (!rop_tail)
            ;
        else while (1)
        {
            if (rop->IsValid())
            {
                *(rop->GetDestination()) = memory[rop->GetAddress()]; 

                rop->Reset();
            }

            if (rop == rop_tail)
                break;

            rop = rop->GetNext();

            if (!rop)
                break;
        }
    }

    // private:
    template<typename __PayloadType>
    void W1RDRandomAccessMemory<__PayloadType>::EvalWrite()
    {
        if (write_p0_address >= 0)
            memory[write_p0_address] = write_p0_payload;

        write_p0_address = -1;
    }
}

