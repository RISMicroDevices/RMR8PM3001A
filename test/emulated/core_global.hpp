#pragma once
//
// Mixed emulation for Global Core Resources
//
//

#include "bitset"

#include "base.hpp"

#define EMULATED_GC_COUNT                                           8

using namespace std;

namespace MEMU::Core {
    class GlobalCheckpointTable : public MEMU::Emulated
    {
    private:
        bitset<EMULATED_GC_COUNT>   gc_valid;

    public:
        GlobalCheckpointTable();
        GlobalCheckpointTable(const GlobalCheckpointTable& obj);
        ~GlobalCheckpointTable();

        constexpr int   GetSize();

        // TODO

        virtual void    Eval() override;
    };
}
