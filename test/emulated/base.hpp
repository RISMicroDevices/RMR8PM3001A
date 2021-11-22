#pragma once
//
// Mixed emulation base definitions and includes
//
//  @see 'rat_freelist_checkpoint.v'
//

#include <cstdint>

namespace MEMU {

    /*pure virtual*/
    class Emulated
    {
    public:
        virtual void Eval() = 0;
    };
};
