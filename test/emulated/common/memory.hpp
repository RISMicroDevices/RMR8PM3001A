#pragma once
//
// Mixed emulations for common memory modules
//
//

#include <cstring>
#include <stdexcept>

#include "base.hpp"

namespace MEMU::Common {

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
    public:
        class Entry final {
        private:
            Entry*                      next;

            const bool                  end;

            const int                   port;
            const int                   address;
            __PayloadType* const        destination;

            const __PayloadType         payload;

        public:
            Entry();
            Entry(int port, int address, __PayloadType* destination, __PayloadType payload);
            Entry(const Entry& obj);
            ~Entry();   

            bool                    IsEnd() const;

            Entry*                  GetNextEntry();
            void                    SetNextEntry(Entry* next);

            int                     GetPort() const;
            int                     GetAddress() const;
            __PayloadType*          GetDestination() const;
            
            __PayloadType           GetReadPayload() const;
        };

    private:
        Entry*  head;
        Entry*  current;
        int     size;

        static const Entry&     EndEntry();

    public:
        MemoryReadSnapshot();
        MemoryReadSnapshot(const MemoryReadSnapshot& obj);
        ~MemoryReadSnapshot();

        bool                    IsEmpty() const;
        int                     GetSize() const;

        const Entry&            GetEntry();
        bool                    HasEntry();
        const Entry&            NextEntry();
        bool                    HasNextEntry();

        void                    AddEntry(int port, int address, __PayloadType* destination, __PayloadType payload);

        void                    ResetIteration();
        void                    ResetSnapshot();

        MemoryReadSnapshot&     operator++();
        MemoryReadSnapshot      operator++(int);

        const Entry&            operator*();
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
        W1RDRandomAccessMemory(__PayloadType* const memory, const int size, const MemoryReadMode rmode);
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


// class MEMU::Common::MemoryReadSnapshot
namespace MEMU::Common {
    /*
    Entry*  head;
    Entry*  current;
    int     size;
    */

    template<typename __PayloadType>
    static const MemoryReadSnapshot<__PayloadType>::Entry& MemoryReadSnapshot<__PayloadType>::EndEntry()
    {
        static const Entry END_ENTRY;

        return END_ENTRY;
    }


}


// class MEMU::Common::MemoryReadSnapshot::Entry
namespace MEMU::Common {
    /*
    Entry*                      next;

    const bool                  end;

    const int                   port;
    const int                   address;
    __PayloadType* const        destination;

    const __PayloadType         payload;
    */

    template<typename __PayloadType>
    MemoryReadSnapshot<__PayloadType>::Entry::Entry()
        : next          (nullptr)
        , end           (true)
        , port          (-1)
        , address       (-1)
        , destination   (nullptr)
    { }

    template<typename __PayloadType>
    MemoryReadSnapshot<__PayloadType>::Entry::Entry(int port, int address, __PayloadType* destination, __PayloadType payload)
        : next          (nullptr)
        , end           (false)
        , port          (port)
        , address       (address)
        , destination   (destination)
        , payload       (payload)
    { }

    template<typename __PayloadType>
    MemoryReadSnapshot<__PayloadType>::Entry::Entry(const MemoryReadSnapshot<__PayloadType>::Entry& obj)
        : next          (nullptr)
        , end           (obj.end)
        , port          (obj.port)
        , address       (obj.address)
        , destination   (obj.destination)
        , payload       (obj.payload)
    { }

    template<typename __PayloadType>
    MemoryReadSnapshot<__PayloadType>::Entry::~Entry()
    { }

    // TODO
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
    DelayedMemoryRead<__PayloadType>::DelayedMemoryRead(int port, int addr, __PayloadType* const dst)
        : next              (nullptr)
        , target_port       (port)
        , target_address    (addr)
        , target_destination(dst)
    { }

    template<typename __PayloadType>
    DelayedMemoryRead<__PayloadType>::DelayedMemoryRead(const DelayedMemoryRead& obj)
        : next              (nullptr)
        , target_port       (obj.target_port)
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
    inline void DelayedMemoryRead<__PayloadType>::SetPort(const int port)
    {
        this->target_port = port;
    }

    template<typename __PayloadType>
    inline void DelayedMemoryRead<__PayloadType>::SetAddress(const int addr)
    {
        this->target_address = addr;
    }

    template<typename __PayloadType>
    inline void DelayedMemoryRead<__PayloadType>::SetDestination(__PayloadType* const dst)
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
    W1RTRandomAccessMemory<__PayloadType>::W1RTRandomAccessMemory(__PayloadType* const memory, const int size, const MemoryReadMode rmode)
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
    inline bool W1RTRandomAccessMemory<__PayloadType>::CheckBound(const int address) const
    {
        return address >= 0 && address < size;
    }

    template<typename __PayloadType>
    void W1RTRandomAccessMemory<__PayloadType>::ReadThrough(const int address, __PayloadType* const dst) const
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
    void W1RTRandomAccessMemory<__PayloadType>::SetWrite(const int address, const __PayloadType src)
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
