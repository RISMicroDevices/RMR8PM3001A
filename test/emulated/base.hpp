#pragma once
//
// Mixed emulation base definitions and includes
//
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
