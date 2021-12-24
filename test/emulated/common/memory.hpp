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

    // Snapshot of Delayed Memory (Read/Write) Operation
    template<typename __PayloadType>
    class MemoryOperationSnapshot final {
    public:
        class Entry final {
        private:
            const Entry*                next;

            const bool                  end;

            const int                   port;
            const int                   address;
            __PayloadType* const        subject;

            const __PayloadType         payload;

        public:
            Entry();
            Entry(int port, int address, __PayloadType* subject, __PayloadType payload);
            Entry(const Entry& obj);
            ~Entry();   

            const Entry*            GetNextEntry();
            void                    SetNextEntry(const Entry* next);

            bool                    IsEnd() const;

            int                     GetPort() const;
            int                     GetAddress() const;
            __PayloadType*          GetSubject() const;
            
            __PayloadType           GetPayload() const;
        };

        class Iterator final {
        private:
            const Entry*    current;

        public:
            Iterator(const Entry* head);
            Iterator(const Iterator& obj);
            ~Iterator();

            const Entry&    GetEntry() const;
            bool            HasEntry() const;
            const Entry&    NextEntry();
            bool            HasNextEntry() const;

            Iterator&       operator++();
            Iterator        operator++(int);

            const Entry&    operator*();
        };

    private:
        Entry*  head;
        Entry*  tail;
        int     size;

        static const Entry*     EndEntry();

    public:
        MemoryOperationSnapshot();
        MemoryOperationSnapshot(const MemoryOperationSnapshot& obj);
        ~MemoryOperationSnapshot();

        bool                    IsEmpty() const;
        int                     GetSize() const;

        Iterator                GetIterator() const;

        void                    AddEntry(int port, int address, __PayloadType* subject, __PayloadType payload);

        void                    Reset();
    };

    // Snapshot of Delayed Memory Write Operation
    //
    template<typename __PayloadType>
    using MemoryWriteSnapshot = MemoryOperationSnapshot<__PayloadType>;

    // Snapshot of Delayed Memory Read Operation
    //
    template<typename __PayloadType>
    using MemoryReadSnapshot  = MemoryOperationSnapshot<__PayloadType>;

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
    class ReadThroughRandomAccessable
    {
    public:
        virtual void    ReadThrough(int addr, __PayloadType* dst) const = 0;
    };

    // Interface of Read Delayed Random Access Memory
    // *pure virtual*
    template<typename __PayloadType>
    class ReadDelayedRandomAccessable
    {
    public:
        virtual void    ReadDelayed(int port, int addr, __PayloadType* dst) = 0;
    };

    // Single write port, read through
    //
    template<typename __PayloadType>
    class W1RTRandomAccessMemory final : public MEMU::Emulated, public ReadThroughRandomAccessable<__PayloadType>
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

        void            ReadThrough(int address, __PayloadType* dst) const override;
            
        void            SetWrite(int address, __PayloadType src);
        void            ResetWrite();

        void            Eval() override;
        void            EvalEx(MemoryWriteSnapshot<__PayloadType>* wrsnpsht);
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
    class W1RDRandomAccessMemory final : public MEMU::Emulated, public ReadDelayedRandomAccessable<__PayloadType>
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

        void            EvalRead(MemoryReadSnapshot<__PayloadType>* rdsnpsht);
        void            EvalWrite(MemoryWriteSnapshot<__PayloadType>* wrsnpsht);

    public:
        W1RDRandomAccessMemory(__PayloadType* const memory, const int size, const MemoryReadMode rmode);
        W1RDRandomAccessMemory(const int size, const MemoryReadMode rmode);
        W1RDRandomAccessMemory(const W1RDRandomAccessMemory<__PayloadType>& obj);
        ~W1RDRandomAccessMemory();

        int             GetSize() const;
        MemoryReadMode  GetReadMode() const;
        __PayloadType*  GetMedium() const;
        bool            CheckBound(int address) const;

        void            ReadDelayed(int port, int address, __PayloadType* dst) override;
        void            ResetReadPort(int port);
        void            ResetRead();

        void            SetWrite(int address, __PayloadType src);
        void            ResetWrite();

        void            Eval() override;
        void            EvalEx(MemoryWriteSnapshot<__PayloadType>* wrsnpsht, MemoryReadSnapshot<__PayloadType>* rdsnpsht);
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

    
    // Content addressable memory (Read through)
    // 
    template<typename __PayloadType>
    class ContentAddressableMemory final
    {
    private:
        ReadThroughRandomAccessable<__PayloadType>* const   memory;
        const int                                           memory_offset;
        ReadThroughRandomAccessable<bool>* const            validf;
        const int                                           validf_offset;

        const int                                           size;

    public:
        ContentAddressableMemory(
            ReadThroughRandomAccessable<__PayloadType>* memory, 
            int                                         memory_offset,
            ReadThroughRandomAccessable<__PayloadType>* validf,
            int                                         validf_offset,
            int                                         size);
        
        ~ContentAddressableMemory();

        ReadThroughRandomAccessable<__PayloadType>* GetMemoryMedium() const;
        ReadThroughRandomAccessable<bool>*          GetValidBitMedium() const;
        int                                         GetMemoryOffset() const;
        int                                         GetValidBitOffset() const;
        int                                         GetSize() const;

        int                                         QueryAddress(const __PayloadType& content) const;
    };
}


// class MEMU::Common::MemoryOperationSnapshot
namespace MEMU::Common {
    /*
    Entry*  head;
    Entry*  tail;
    int     size;
    */

    template<typename __PayloadType>
    static const MemoryOperationSnapshot<__PayloadType>::Entry* MemoryOperationSnapshot<__PayloadType>::EndEntry()
    {
        static const Entry END_ENTRY;

        return &END_ENTRY;
    }

    template<typename __PayloadType>
    MemoryOperationSnapshot<__PayloadType>::MemoryOperationSnapshot()
        : head      (nullptr)
        , tail      (nullptr)
        , size      (0)
    { }

    template<typename __PayloadType>
    MemoryOperationSnapshot<__PayloadType>::MemoryOperationSnapshot(const MemoryOperationSnapshot& obj)
        : head      (nullptr)
        , tail      (nullptr)
        , size      (0)
    {
        const MemoryOperationSnapshot<__PayloadType>::Entry* entry = obj.head;
        if (entry)
        {
            // copy the entire singly link

            MemoryOperationSnapshot<__PayloadType>::Entry* copied_entry
                = new MemoryOperationSnapshot<__PayloadType>::Entry(*entry);

            entry = this->head = copied_entry;

            while (!entry->GetNextEntry()->IsEnd())
            {
                copied_entry 
                    = new MemoryOperationSnapshot<__PayloadType>::Entry(*(entry->GetNextEntry()));

                entry->SetNextEntry(copied_entry);
                copied_entry->SetNextEntry(entry->GetNextEntry()->GetNextEntry());

                entry = copied_entry;
            }

            this->tail = entry;
        }
    }

    template<typename __PayloadType>
    MemoryOperationSnapshot<__PayloadType>::~MemoryOperationSnapshot()
    {
        Reset();
    }

    template<typename __PayloadType>
    inline bool MemoryOperationSnapshot<__PayloadType>::IsEmpty() const
    {
        return !size;
    }

    template<typename __PayloadType>
    inline int MemoryOperationSnapshot<__PayloadType>::GetSize() const
    {
        return size;
    }

    template<typename __PayloadType>
    MemoryOperationSnapshot<__PayloadType>::Iterator MemoryOperationSnapshot<__PayloadType>::GetIterator() const
    {
        if (head)
            return Iterator(head);

        return Iterator(EndEntry());            
    }

    template<typename __PayloadType>
    void MemoryOperationSnapshot<__PayloadType>::AddEntry(int port, int address, __PayloadType* subject, __PayloadType payload)
    {
        MemoryOperationSnapshot<__PayloadType>::Entry* new_entry
            = new MemoryOperationSnapshot<__PayloadType>::Entry(port, address, subject, payload);

        new_entry->SetNextEntry(EndEntry());

        if (!head)
        {
            head = new_entry;
            tail = new_entry;
        }
        else
        {
            tail->SetNextEntry(new_entry);
            tail = new_entry;
        }

        size++;
    }

    template<typename __PayloadType>
    void MemoryOperationSnapshot<__PayloadType>::Reset()
    {
        // recycle the entire link

        const MemoryOperationSnapshot<__PayloadType>::Entry* entry = this->head;

        if (entry)
        {
            do 
            {
                const MemoryOperationSnapshot<__PayloadType>::Entry* next = entry->GetNextEntry();

                delete entry;

                entry = next;
            }
            while (!entry->IsEnd());

            head = nullptr;
            tail = nullptr;
            size = 0;
        }
    }
}


// class MEMU::Common::MemoryOperationSnapshot::Entry
namespace MEMU::Common {
    /*
    const Entry*                next;

    const bool                  end;

    const int                   port;
    const int                   address;
    __PayloadType* const        subject;

    const __PayloadType         payload;
    */

    template<typename __PayloadType>
    MemoryOperationSnapshot<__PayloadType>::Entry::Entry()
        : next      (this)
        , end       (true)
        , port      (-1)
        , address   (-1)
        , subject   (nullptr)
    { }

    template<typename __PayloadType>
    MemoryOperationSnapshot<__PayloadType>::Entry::Entry(int port, int address, __PayloadType* subject, __PayloadType payload)
        : next      (nullptr)
        , end       (false)
        , port      (port)
        , address   (address)
        , subject   (subject)
        , payload   (payload)
    { }

    template<typename __PayloadType>
    MemoryOperationSnapshot<__PayloadType>::Entry::Entry(const MemoryOperationSnapshot<__PayloadType>::Entry& obj)
        : next      (nullptr)
        , end       (obj.end)
        , port      (obj.port)
        , address   (obj.address)
        , subject   (obj.subject)
        , payload   (obj.payload)
    { }

    template<typename __PayloadType>
    MemoryOperationSnapshot<__PayloadType>::Entry::~Entry()
    { }

    template<typename __PayloadType>
    inline const MemoryOperationSnapshot<__PayloadType>::Entry* MemoryOperationSnapshot<__PayloadType>::Entry::GetNextEntry()
    {
        return next;
    }

    template<typename __PayloadType>
    inline void MemoryOperationSnapshot<__PayloadType>::Entry::SetNextEntry(const MemoryOperationSnapshot<__PayloadType>::Entry* const next)
    {
        this->next = next;
    }

    template<typename __PayloadType>
    inline bool MemoryOperationSnapshot<__PayloadType>::Entry::IsEnd() const
    {
        return end;
    }

    template<typename __PayloadType>
    inline int MemoryOperationSnapshot<__PayloadType>::Entry::GetPort() const
    {
        return port;
    }

    template<typename __PayloadType>
    inline int MemoryOperationSnapshot<__PayloadType>::Entry::GetAddress() const
    {
        return address;
    }

    template<typename __PayloadType>
    inline __PayloadType* MemoryOperationSnapshot<__PayloadType>::Entry::GetSubject() const
    {
        return subject;
    }

    template<typename __PayloadType>
    inline __PayloadType MemoryOperationSnapshot<__PayloadType>::Entry::GetPayload() const
    {
        return payload;
    }
}


// class MEMU::Common::MemoryOperationSnapshot::Iterator
namespace MEMU::Common {
    /*
    const Entry*    current;
    */

    template<typename __PayloadType>
    MemoryOperationSnapshot<__PayloadType>::Iterator::Iterator(const Entry* head)
        : current(head)
    { }

    template<typename __PayloadType>
    MemoryOperationSnapshot<__PayloadType>::Iterator::Iterator(const Iterator& obj)
        : current(obj.current)
    { }

    template<typename __PayloadType>
    MemoryOperationSnapshot<__PayloadType>::Iterator::~Iterator()
    { }

    template<typename __PayloadType>
    inline const MemoryOperationSnapshot<__PayloadType>::Entry& MemoryOperationSnapshot<__PayloadType>::Iterator::GetEntry() const
    {
        return *current;
    }

    template<typename __PayloadType>
    inline bool MemoryOperationSnapshot<__PayloadType>::Iterator::HasEntry() const
    {
        return !current->IsEnd();
    }

    template<typename __PayloadType>
    const MemoryOperationSnapshot<__PayloadType>::Entry& MemoryOperationSnapshot<__PayloadType>::Iterator::NextEntry()
    {
        return *(current = current->GetNextEntry());
    }

    template<typename __PayloadType>
    bool MemoryOperationSnapshot<__PayloadType>::Iterator::HasNextEntry() const
    {
        return !current->GetNextEntry()->IsEnd();
    }

    template<typename __PayloadType>
    MemoryOperationSnapshot<__PayloadType>::Iterator& MemoryOperationSnapshot<__PayloadType>::Iterator::operator++()
    {
        // post-iterate
        NextEntry();
        return *this;
    }

    template<typename __PayloadType>
    MemoryOperationSnapshot<__PayloadType>::Iterator MemoryOperationSnapshot<__PayloadType>::Iterator::operator++(int)
    {
        // pre-iterate
        MemoryOperationSnapshot<__PayloadType>::Iterator preiter(*this);
        NextEntry();
        return preiter;
    }

    template<typename __PayloadType>
    const MemoryOperationSnapshot<__PayloadType>::Entry& MemoryOperationSnapshot<__PayloadType>::Iterator::operator*()
    {
        return GetEntry();
    }
}


// class MEMU::Common::DelayedMemoryRead (INTERNAL)
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
        EvalEx(nullptr);
    }

    template<typename __PayloadType>
    void W1RTRandomAccessMemory<__PayloadType>::EvalEx(MemoryWriteSnapshot<__PayloadType>* const wrsnpsht)
    {
        if (write_p0_address >= 0)
        {
            memory[write_p0_address] = write_p0_payload;

            if (wrsnpsht)
                wrsnpsht->AddEntry(0, write_p0_address, &write_p0_payload, write_p0_payload);
        }

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
        EvalEx(nullptr, nullptr);
    }

    template<typename __PayloadType>
    void W1RDRandomAccessMemory<__PayloadType>::EvalEx(
        MemoryWriteSnapshot<__PayloadType>* const wrsnpsht,
        MemoryReadSnapshot<__PayloadType>*  const rdsnpsht)
    {
        switch (rmode)
        {
            case READ_FIRST:
                EvalRead(rdsnpsht);
                EvalWrite(wrsnpsht);
                break;

            case WRITE_FIRST:
                EvalWrite(wrsnpsht);
                EvalRead(rdsnpsht);
                break;

            default:
                break;
        }
    }

    // private:
    template<typename __PayloadType>
    void W1RDRandomAccessMemory<__PayloadType>::EvalRead(MemoryReadSnapshot<__PayloadType>* const rdsnpsht)
    {
        DelayedMemoryRead<__PayloadType>* rop = rop_head;

        if (!rop_tail)
            ;
        else while (1)
        {
            if (rop->IsValid())
            {
                __PayloadType* destination = rop->GetDestination();

                *destination = memory[rop->GetAddress()]; 

                if (rdsnpsht)
                    rdsnpsht->AddEntry(rop->GetPort(), rop->GetAddress(), destination, *destination);

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
    void W1RDRandomAccessMemory<__PayloadType>::EvalWrite(MemoryWriteSnapshot<__PayloadType>* const wrsnpsht)
    {
        if (write_p0_address >= 0)
        {
            memory[write_p0_address] = write_p0_payload;

            if (wrsnpsht)
                wrsnpsht->AddEntry(0, write_p0_address, &write_p0_payload, write_p0_payload)
        }

        write_p0_address = -1;
    }
}


// class MEMU::Common::ContentAddressableMemory
namespace MEMU::Common {
    /*
    ReadThroughRandomAccessable<__PayloadType>* const   memory;
    const int                                           memory_offset;
    ReadThroughRandomAccessable<bool>* const            validf;
    const int                                           validf_offset;

    const int                                           size;
    */

    template<typename __PayloadType>
    ContentAddressableMemory<__PayloadType>::ContentAddressableMemory(
        ReadThroughRandomAccessable<__PayloadType>* const   memory, 
        int                                         const   memory_offset,
        ReadThroughRandomAccessable<__PayloadType>* const   validf,
        int                                         const   validf_offset,
        int                                         const   size)
        : memory        (memory)
        , memory_offset (memory_offset)
        , validf        (validf)
        , validf_offset (validf_offset)
        , size          (size)
    { }

    template<typename __PayloadType>
    ContentAddressableMemory<__PayloadType>::~ContentAddressableMemory()
    { }

    template<typename __PayloadType>
    inline ReadThroughRandomAccessable<__PayloadType>* ContentAddressableMemory<__PayloadType>::GetMemoryMedium() const
    {
        return memory;
    }

    template<typename __PayloadType>
    inline ReadThroughRandomAccessable<bool>* ContentAddressableMemory<__PayloadType>::GetValidBitMedium() const
    {
        return validf;
    }

    template<typename __PayloadType>
    inline int ContentAddressableMemory<__PayloadType>::GetMemoryOffset() const
    {
        return memory_offset;
    }

    template<typename __PayloadType>
    inline int ContentAddressableMemory<__PayloadType>::GetValidBitOffset() const
    {
        return validf_offset;
    }

    template<typename __PayloadType>
    inline int ContentAddressableMemory<__PayloadType>::GetSize() const
    {
        return size;
    }

    template<typename __PayloadType>
    int ContentAddressableMemory<__PayloadType>::QueryAddress(const __PayloadType& content) const
    {
        __PayloadType payload = { 0 };
        bool          valid   = 0;

        for (int i = 0; i < size; i++)
        {
            memory->ReadThrough(memory_offset + i, &payload);
            validf->ReadThrough(validf_offset + i, &valid);

            if (valid && payload == content)
                return i;
        }

        return -1;
    }
}
