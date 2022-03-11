#pragma once
//
// Mixed emulation for FIFO modules
//
//

#include <stdexcept>
#include <list>

#include "base.hpp"


using namespace std;

namespace MEMU::Common {

    template<class TPayload>
    class FIFO : public MEMU::Emulated
    {
    public:
        class Modification {
        private:
            int         index;
            TPayload    payload;

        public:
            Modification();
            Modification(int index, const TPayload& payload);
            Modification(const Modification& obj);
            ~Modification();

            int                 GetIndex() const;
            void                SetIndex(int index);

            const TPayload&     GetPayload() const;
            TPayload&           GetPayload();
            void                SetPayload(const TPayload& payload);

            void                Reset();

            void                Apply(TPayload* memory) const;
        };

        class Iterator {
        private:
            FIFO<TPayload>*     owner;

            int                 rptr;
            bool                rptrb;

            void                CheckBound() const;

        public:
            Iterator();
            Iterator(FIFO<TPayload>* owner, int rptr, bool rptrb);
            Iterator(const Iterator& obj);
            ~Iterator();

            operator            bool() const;
            const TPayload&     operator*() const;
            TPayload&           operator*();
            Iterator&           operator++();     // prefix
            Iterator            operator++(int);  // suffix
            bool                operator==(const Iterator& obj) const;
            bool                operator!=(const Iterator& obj) const;

            void                operator=(const Iterator& obj);
        };

    private:
        const int           size;

        TPayload*           memory;

        int                 rptr;
        bool                rptrb;
        int                 wptr;
        bool                wptrb;

        list<Modification>  modification;

        int                 delta_rptr;
        int                 delta_wptr;

        int                 set_rptr;
        int                 set_rptrb;
        int                 set_wptr;
        int                 set_wptrb;

    public:
        FIFO(int size);
        FIFO(const FIFO<TPayload>& obj);
        ~FIFO();

        int                 GetSize() const;
        int                 GetRemainingSize() const;
        int                 GetCount() const;

        bool                IsEmpty() const;
        bool                IsFull() const;

        Iterator            Begin();
        Iterator            End();

        int                 GetReadPointer() const;
        bool                GetReadPointerB() const;
        int                 GetWritePointer() const;
        bool                GetWritePointerB() const;

        void                SetReadPointer(int rptr);
        void                SetReadPointerB(bool rptrb);
        void                SetWritePointer(int wptr);
        void                SetWritePointerB(bool wptrb);

        const TPayload&     GetPayload(int index) const;
        TPayload&           GetPayload(int index);

        void                SetPayload(int index, const TPayload& payload);

        bool                PeekPayload(TPayload* dst) const;
        bool                PopPayload();
        bool                PushPayload(const TPayload& payload);

        void                ResetInput();

        void                Clear();

        virtual void        Eval() override;

        void                operator=(const FIFO<TPayload>& obj) = delete;
    };
}



// class MEMU::Common::FIFO::Modification
namespace MEMU::Common {
    /*
    int         index;
    TPayload    payload;
    */

    template<class TPayload>
    FIFO<TPayload>::Modification::Modification()
        : index     (-1)
        , payload   (TPayload())
    { }

    template<class TPayload>
    FIFO<TPayload>::Modification::Modification(int index, const TPayload& payload)
        : index     (index)
        , payload   (payload)
    { }

    template<class TPayload>
    FIFO<TPayload>::Modification::Modification(const Modification& obj)
        : index     (obj.index)
        , payload   (obj.payload)
    { }

    template<class TPayload>
    FIFO<TPayload>::Modification::~Modification()
    { }

    template<class TPayload>
    inline int FIFO<TPayload>::Modification::GetIndex() const
    {
        return index;
    }

    template<class TPayload>
    inline void FIFO<TPayload>::Modification::SetIndex(int index)
    {
        this->index = index;
    }

    template<class TPayload>
    inline const TPayload& FIFO<TPayload>::Modification::GetPayload() const
    {
        return payload;
    }

    template<class TPayload>
    inline TPayload& FIFO<TPayload>::Modification::GetPayload()
    {
        return payload;
    }

    template<class TPayload>
    inline void FIFO<TPayload>::Modification::SetPayload(const TPayload& payload)
    {
        this->payload = payload;
    }

    template<class TPayload>
    inline void FIFO<TPayload>::Modification::Reset()
    {
        index   = -1;
        payload = TPayload();
    }

    template<class TPayload>
    inline void FIFO<TPayload>::Modification::Apply(TPayload* memory) const
    {
        memory[index] = payload;
    }
}


// class MEMU::Common::FIFO::Iterator
namespace MEMU::Common {
    /*
    FIFO<TPayload>*     owner;

    int                 rptr;
    bool                rptrb;
    */

    template<class TPayload>
    FIFO<TPayload>::Iterator::Iterator()
        : owner (nullptr)
        , rptr  (0)
        , rptrb (false)
    { }

    template<class TPayload>
    FIFO<TPayload>::Iterator::Iterator(FIFO<TPayload>* owner, int rptr, bool rptrb)
        : owner (owner)
        , rptr  (rptr)
        , rptrb (rptrb)
    { }

    template<class TPayload>
    FIFO<TPayload>::Iterator::Iterator(const Iterator& obj)
        : owner (obj.owner)
        , rptr  (obj.rptr)
        , rptrb (obj.rptrb)
    { }

    template<class TPayload>
    FIFO<TPayload>::Iterator::~Iterator()
    { }

    template<class TPayload>
    inline void FIFO<TPayload>::Iterator::CheckBound() const
    {
        if (!((bool)*this))
            throw std::out_of_range("iterator out of range or not initialized");
    }

    template<class TPayload>
    FIFO<TPayload>::Iterator::operator bool() const
    {
        return owner 
            && (rptr != owner->GetWritePointer() || rptrb != owner->GetWritePointerB());
    }

    template<class TPayload>
    const TPayload& FIFO<TPayload>::Iterator::operator*() const
    {
        CheckBound();

        return owner.GetPayload(rptr);
    }

    template<class TPayload>
    TPayload& FIFO<TPayload>::Iterator::operator*()
    {
        CheckBound();

        return owner->GetPayload(rptr);
    }

    template<class TPayload>
    typename FIFO<TPayload>::Iterator& FIFO<TPayload>::Iterator::operator++()
    {
        CheckBound();

        if (++rptr == owner->GetSize())
        {
            rptr  = 0;
            rptrb = !rptrb;
        }

        return *this;
    }

    template<class TPayload>
    typename FIFO<TPayload>::Iterator FIFO<TPayload>::Iterator::operator++(int)
    {
        CheckBound();

        Iterator iter(owner, rptr, rptrb);

        if (++rptr == owner->GetSize())
        {
            rptr  = 0;
            rptrb = !rptrb;
        }

        return iter;
    }

    template<class TPayload>
    bool FIFO<TPayload>::Iterator::operator==(const Iterator& obj) const
    {
        if (owner != obj.owner)
            return false;

        if (rptr != obj.rptr)
            return false;

        if (rptrb != obj.rptrb)
            return false;

        return true;
    }

    template<class TPayload>
    bool FIFO<TPayload>::Iterator::operator!=(const Iterator& obj) const
    {
        return !(this->operator==(obj));
    }

    template<class TPayload>
    void FIFO<TPayload>::Iterator::operator=(const Iterator& obj)
    {
        owner = obj.owner;
        rptr  = obj.rptr;
        rptrb = obj.rptrb;
    }
}


// class MEMU::Common::FIFO
namespace MEMU::Common {
    /*
    const int           size;

    TPayload*           memory;

    int                 rptr;
    bool                rptrb;
    int                 wptr;
    bool                wptrb;

    list<Modification>  modification;

    int                 delta_rptr;
    int                 delta_wptr;

    int                 set_rptr;
    int                 set_rptrb;
    int                 set_wptr;
    int                 set_wptrb;
    */

    template<class TPayload>
    FIFO<TPayload>::FIFO(int size)
        : size          (size)
        , memory        (new TPayload[size])
        , modification  (list<Modification>())
        , rptr          (0)
        , rptrb         (false)
        , wptr          (0)
        , wptrb         (false)
        , delta_rptr    (0)
        , delta_wptr    (0)
        , set_rptr      (-1)
        , set_rptrb     (-1)
        , set_wptr      (-1)
        , set_wptrb     (-1)
    { }

    template<class TPayload>
    FIFO<TPayload>::FIFO(const FIFO<TPayload>& obj)
        : size          (obj.size)
        , memory        (new TPayload[obj.size])
        , modification  (obj.modification)
        , rptr          (obj.rptr)
        , rptrb         (obj.rptrb)
        , wptr          (obj.wptr)
        , wptrb         (obj.wptrb)
        , delta_rptr    (obj.delta_rptr)
        , delta_wptr    (obj.delta_wptr)
        , set_rptr      (obj.set_rptr)
        , set_rptrb     (obj.set_rptrb)
        , set_wptr      (obj.set_wptr)
        , set_wptrb     (obj.set_wptrb)
    {
        for (int i = 0; i < size; i++)
            memory[i] = obj.memory[i];
    }

    template<class TPayload>
    FIFO<TPayload>::~FIFO()
    {
        delete[] memory;
    }

    template<class TPayload>
    inline int FIFO<TPayload>::GetSize() const
    {
        return size;
    }

    template<class TPayload>
    inline int FIFO<TPayload>::GetRemainingSize() const
    {
        return size - GetCount();
    }

    template<class TPayload>
    inline int FIFO<TPayload>::GetCount() const
    {
        return wptr - rptr + (wptrb == rptrb ? 0 : size);
    }

    template<class TPayload>
    inline bool FIFO<TPayload>::IsEmpty() const
    {
        return wptr == rptr && wptrb == rptrb;
    }

    template<class TPayload>
    inline bool FIFO<TPayload>::IsFull() const
    {
        return wptr == rptr && wptrb != rptrb;
    }

    template<class TPayload>
    inline typename FIFO<TPayload>::Iterator FIFO<TPayload>::Begin()
    {
        return Iterator(this, rptr, rptrb);
    }

    template<class TPayload>
    inline typename FIFO<TPayload>::Iterator FIFO<TPayload>::End()
    {
        return Iterator(this, wptr, wptrb);
    }

    template<class TPayload>
    inline int FIFO<TPayload>::GetReadPointer() const
    {
        return rptr;
    }

    template<class TPayload>
    inline bool FIFO<TPayload>::GetReadPointerB() const
    {
        return rptrb;
    }

    template<class TPayload>
    inline int FIFO<TPayload>::GetWritePointer() const
    {
        return wptr;
    }

    template<class TPayload>
    inline bool FIFO<TPayload>::GetWritePointerB() const
    {
        return wptrb;
    }

    template<class TPayload>
    inline void FIFO<TPayload>::SetReadPointer(int rptr)
    {
        set_rptr = rptr;
    }

    template<class TPayload>
    inline void FIFO<TPayload>::SetReadPointerB(bool rptrb)
    {
        set_rptrb = rptrb;
    }

    template<class TPayload>
    inline void FIFO<TPayload>::SetWritePointer(int wptr)
    {
        set_wptr = wptr;
    }

    template<class TPayload>
    inline void FIFO<TPayload>::SetWritePointerB(bool wptrb)
    {
        set_wptrb = wptrb;
    }

    template<class TPayload>
    inline const TPayload& FIFO<TPayload>::GetPayload(int index) const
    {
        return memory[index];
    }

    template<class TPayload>
    inline TPayload& FIFO<TPayload>::GetPayload(int index)
    {
        return memory[index];
    }
    
    template<class TPayload>
    inline void FIFO<TPayload>::SetPayload(int index, const TPayload& payload)
    {
        modification.push_back(Modification(index, payload));
    }

    template<class TPayload>
    bool FIFO<TPayload>::PeekPayload(TPayload* dst) const
    {
        if (IsEmpty())
            return false;

        *dst = memory[rptr];

        return true;
    }

    template<class TPayload>
    bool FIFO<TPayload>::PopPayload()
    {
        if (delta_rptr < GetCount())
        {
            delta_rptr++;

            return true;
        }
        else
            return false;
    }

    template<class TPayload>
    bool FIFO<TPayload>::PushPayload(const TPayload& payload)
    {
        if (delta_wptr < GetRemainingSize())
        {
            modification.push_back(Modification((wptr + delta_wptr) % size, payload));

            delta_wptr++;

            return true;
        }
        else
            return false;
    }

    template<class TPayload>
    void FIFO<TPayload>::ResetInput()
    {
        delta_rptr = 0;
        delta_wptr = 0;

        set_rptr  = -1;
        set_rptrb = -1;
        set_wptr  = -1;
        set_wptrb = -1;

        modification.clear();
    }

    template<class TPayload>
    void FIFO<TPayload>::Clear()
    {
        rptr  = 0;
        rptrb = false;

        wptr  = 0;
        wptrb = false;
    }

    template<class TPayload>
    void FIFO<TPayload>::Eval()
    {
        //
        for (auto iter = modification.begin(); iter != modification.end(); iter++)
            iter->Apply(memory);

        //
        if (set_rptr != -1)
            rptr = set_rptr;
        else
            rptr += delta_rptr;

        if (set_rptrb != -1)
            rptrb = set_rptrb;

        if (rptr >= size)
        {
            rptr -= size;
            rptrb = !rptrb;
        }

        //
        if (set_wptr != -1)
            wptr = set_wptr;
        else
            wptr += delta_wptr;

        if (set_wptrb != -1)
            wptrb = set_wptrb;

        if (wptr >= size)
        {
            wptr -= size;
            wptrb = !wptrb;
        }

        //
        ResetInput();
    }
}
