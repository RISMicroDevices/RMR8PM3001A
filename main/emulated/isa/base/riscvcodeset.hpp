#pragma once
//
// RISC-V Instruction Set Architecture Emulator (Jasse)
//
// RISC-V Codepoint Collection infrastructure
//

#include <vector>
#include <string>

#include "riscvdef.hpp"

namespace Jasse {
    // RISC-V Codepoint Collection Iterator
    using RVCodepointIterator      = std::vector<const RVCodepoint*>::iterator;
    using RVCodepointConstIterator = std::vector<const RVCodepoint*>::const_iterator;

    // RISC-V Codepoint Collection
    class RVCodepointCollection {
    private:
        // *NOTICE: "RVCodepoint" are stored as constant pointers in the collection.
        //          This means that any modification of any copy of RVCodepoint instance would not 
        //          reflect in this collection.
        //          This is based on the principle that any RVCodepoint instance should have its own
        //          static constant instance. And any global modification of RVCodepoint definition
        //          should be done by modifying the source code in header files.
        std::vector<const RVCodepoint*> codepoints;

    public:
        RVCodepointCollection(std::initializer_list<const RVCodepoint*> codepoints);
        RVCodepointCollection(const RVCodepointCollection& obj);
        ~RVCodepointCollection();

        size_t                      GetSize() const;

        const RVCodepoint*          Get(int index) const;
        void                        Set(int index, const RVCodepoint* codepoint);
        void                        Add(const RVCodepoint* codepoint);
        void                        AddAll(const RVCodepointCollection& another);
        void                        Remove(int index);
        void                        Remove(RVCodepointConstIterator pos);
        void                        Remove(RVCodepointConstIterator first, RVCodepointConstIterator last);
        void                        Clear();

        RVCodepointIterator         Begin();
        RVCodepointConstIterator    Begin() const;
        RVCodepointIterator         End();
        RVCodepointConstIterator    End() const;
    };
}



// Implementation of: class RVCodepointCollection
namespace Jasse {
    /*
    std::vector<const RVCodepoint*> codepoints;
    */

    RVCodepointCollection::RVCodepointCollection(std::initializer_list<const RVCodepoint*> codepoints)
        : codepoints(codepoints)
    { }

    RVCodepointCollection::RVCodepointCollection(const RVCodepointCollection& obj)
        : codepoints(obj.codepoints)
    { }

    RVCodepointCollection::~RVCodepointCollection()
    { }

    inline size_t RVCodepointCollection::GetSize() const
    {
        return codepoints.size();
    }

    inline const RVCodepoint* RVCodepointCollection::Get(int index) const
    {
        return codepoints[index];
    }

    inline void RVCodepointCollection::Set(int index, const RVCodepoint* codepoint)
    {
        codepoints[index] = codepoint;
    }

    inline void RVCodepointCollection::Add(const RVCodepoint* codepoint)
    {
        codepoints.push_back(codepoint);
    }

    inline void RVCodepointCollection::Remove(int index)
    {
        codepoints.erase(codepoints.begin() + index);
    }

    inline void RVCodepointCollection::Remove(RVCodepointConstIterator pos)
    {
        codepoints.erase(pos);
    }

    inline void RVCodepointCollection::Remove(RVCodepointConstIterator first, RVCodepointConstIterator last)
    {
        codepoints.erase(first, last);
    }

    inline void RVCodepointCollection::Clear()
    {
        codepoints.clear();
    }

    inline RVCodepointIterator RVCodepointCollection::Begin()
    {
        return codepoints.begin();
    }

    inline RVCodepointConstIterator RVCodepointCollection::Begin() const
    {
        return codepoints.begin();
    }

    inline RVCodepointIterator RVCodepointCollection::End()
    {
        return codepoints.end();
    }

    inline RVCodepointConstIterator RVCodepointCollection::End() const
    {
        return codepoints.end();
    }

    void RVCodepointCollection::AddAll(const RVCodepointCollection& another)
    {
        codepoints.insert(codepoints.end(), another.Begin(), another.End());
    }
}
