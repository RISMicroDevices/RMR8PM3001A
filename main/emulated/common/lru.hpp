#pragma once
//
// Mixed emulations for common lru algorithms
//
//

#include <bitset>

#include "base.hpp"

using namespace std;

namespace MEMU::Common {

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
        void            ResetUpdate();

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
        void            ResetUpdate();

        void            SetValid(int index);
        void            ResetValid();

        virtual void    Eval() override;
    };    
}


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
        , lru_picked_index  (obj.lru_picked_index)
        , lru_update_index  (obj.lru_update_index)
    { }

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
    inline void PseudoLRUSwap<__size_log2>::ResetUpdate()
    {
        lru_update_index = -1;
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
        , lru_update_index      (obj.lru_update_index)
        , lru_sub               (obj.lru_sub)
        , lru_sub_picked_index  (obj.lru_sub_picked_index)
    {  }

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
    inline void PseudoLRUPick<__size_log2>::ResetUpdate()
    {
        lru_update_index = -1;
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
