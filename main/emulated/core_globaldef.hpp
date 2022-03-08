#pragma once
//
// Global definitions
//
//

#include <cstdint>


#define EMULATED_GC_COUNT                           8
#define EMULATED_ARF_SIZE                           32
#define EMULATED_PRF_SIZE                           96
#define EMULATED_ROB_SIZE                           32

#define EMULATED_SCOREBOARD_GC_COUNT                EMULATED_GC_COUNT

#define EMULATED_RAT_SIZE                           EMULATED_PRF_SIZE
#define EMULATED_RAT_GC_COUNT                       EMULATED_GC_COUNT 


// Type definition of Architectural Register Value
typedef     uint64_t                arch_t;

// Type definition of PC Register Value
typedef     uint64_t                pc_t;

// Type definition of FGR Value
typedef     uint16_t                fgr_t;
