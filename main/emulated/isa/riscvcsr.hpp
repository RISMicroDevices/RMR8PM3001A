#pragma once
//
// RISC-V Instruction Set Architecture Privileged
//
// CSR registers and Privileged basis
//

#include <cstdint>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <initializer_list>


#define CSR_ADDRESS_MASK            0xFFF

//
// Ratified Standard CSRs (based on riscv-privileged-20211203)

// .. Currently allocated RISC-V unprivileged CSR addresses
// .... Unprivileged Floating-Point CSRs
#define CSR_fflags                  0x001       // Floating-Point Accrued Exceptions
#define CSR_frm                     0x002       // Floating-Point Dynamic Rounding Mode
#define CSR_fcsr                    0x003       // Floating-Point Control and Status Register

// .... Unprivileged Counter/Timers
#define CSR_cycle                   0xC00       // Cycle counter for RDCYCLE instruction
#define CSR_time                    0xC01       // Timer for RDTIME instruction
#define CSR_instret                 0xC02       // Instruction-retired counter for RDINSTRET instruction
#define CSR_hpmcounter3             0xC03       // Performance-monitoring counter
#define CSR_hpmcounter4             0xC04       // Performance-monitoring counter
#define CSR_hpmcounter5             0xC05       // Performance-monitoring counter
#define CSR_hpmcounter6             0xC06       // Performance-monitoring counter
#define CSR_hpmcounter7             0xC07       // Performance-monitoring counter
#define CSR_hpmcounter8             0xC08       // Performance-monitoring counter
#define CSR_hpmcounter9             0xC09       // Performance-monitoring counter
#define CSR_hpmcounter10            0xC0A       // Performance-monitoring counter
#define CSR_hpmcounter11            0xC0B       // Performance-monitoring counter
#define CSR_hpmcounter12            0xC0C       // Performance-monitoring counter
#define CSR_hpmcounter13            0xC0D       // Performance-monitoring counter
#define CSR_hpmcounter14            0xC0E       // Performance-monitoring counter
#define CSR_hpmcounter15            0xC0F       // Performance-monitoring counter
#define CSR_hpmcounter16            0xC10       // Performance-monitoring counter
#define CSR_hpmcounter17            0xC11       // Performance-monitoring counter
#define CSR_hpmcounter18            0xC12       // Performance-monitoring counter
#define CSR_hpmcounter19            0xC13       // Performance-monitoring counter
#define CSR_hpmcounter20            0xC14       // Performance-monitoring counter
#define CSR_hpmcounter21            0xC15       // Performance-monitoring counter
#define CSR_hpmcounter22            0xC16       // Performance-monitoring counter
#define CSR_hpmcounter23            0xC17       // Performance-monitoring counter
#define CSR_hpmcounter24            0xC18       // Performance-monitoring counter
#define CSR_hpmcounter25            0xC19       // Performance-monitoring counter
#define CSR_hpmcounter26            0xC1A       // Performance-monitoring counter
#define CSR_hpmcounter27            0xC1B       // Performance-monitoring counter
#define CSR_hpmcounter28            0xC1C       // Performance-monitoring counter
#define CSR_hpmcounter29            0xC1D       // Performance-monitoring counter
#define CSR_hpmcounter30            0xC1E       // Performance-monitoring counter
#define CSR_hpmcounter31            0xC1F       // Performance-monitoring counter

#define CSR_cycleh                  0xC80       // Upper 32 bits of cycle, RV32 only
#define CSR_timeh                   0xC81       // Upper 32 bits of time, RV32 only
#define CSR_instreth                0xC82       // Upper 32 bits of instret, RV32 only
#define CSR_hpmcounter3h            0xC83       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter4h            0xC84       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter5h            0xC85       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter6h            0xC86       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter7h            0xC87       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter8h            0xC88       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter9h            0xC89       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter10h           0xC8A       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter11h           0xC8B       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter12h           0xC8C       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter13h           0xC8D       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter14h           0xC8E       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter15h           0xC8F       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter16h           0xC90       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter17h           0xC91       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter18h           0xC92       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter19h           0xC93       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter20h           0xC94       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter21h           0xC95       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter22h           0xC96       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter23h           0xC97       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter24h           0xC98       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter25h           0xC99       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter26h           0xC9A       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter27h           0xC9B       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter28h           0xC9C       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter29h           0xC9D       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter30h           0xC9E       // Performance-monitoring counter, RV32 only
#define CSR_hpmcounter31h           0xC9F       // Performance-monitoring counter, RV32 only

// .. Currently allocated RISC-V supervisor-level CSR addresses
// .... Supervisor Trap Setup
#define CSR_sstatus                 0x100       // Supervisor status regsiter
#define CSR_sie                     0x104       // Supervisor interrupt-enable register
#define CSR_stvec                   0x105       // Supervisor trap handler base address
#define CSR_scounteren              0x106       // Supervisor counter enable

// .... Supervisor Configuration
#define CSR_senvcfg                 0x10A       // Supervisor environment configuration register

// .... Supervisor Trap Handling
#define CSR_sscratch                0x140       // Scratch register for supervisor trap handlers
#define CSR_sepc                    0x141       // Supervisor exception program counter
#define CSR_scause                  0x142       // Supervisor trap cause
#define CSR_stval                   0x143       // Supervisor bad address or instruction
#define CSR_sip                     0x144       // Supervisor interrupt pending

// .... Supervisor Protection and Translation
#define CSR_satp                    0x180       // Supervisor address translation and protection

// .... Debug/Trace registers
#define CSR_scontext                0x5A8       // Supervisor-mode context reigster


// .. Currently allocated RISC-V hypervisor and VS CSR addresses
// .... Hypervisor Trap Setup
#define CSR_hstatus                 0x600       // Hypervisor status register
#define CSR_hedeleg                 0x602       // Hypervisor excetpion delegation register
#define CSR_hideleg                 0x603       // Hypervisor interrupt delegation register
#define CSR_hie                     0x604       // Hypervisor interrupt-enable register
#define CSR_hcounteren              0x606       // Hypervisor counter enable
#define CSR_hgeie                   0x607       // Hypervisor guest external interrupt-enable register

// .... Hypervisor Trap Handling
#define CSR_htval                   0x643       // Hypervisor bad guest physical address
#define CSR_hip                     0x644       // Hypervisor interrupt pending
#define CSR_hvip                    0x645       // Hypervisor virtual interrupt pending
#define CSR_htinst                  0x64A       // Hypervisor trap instruction (transformed)
#define CSR_hgeip                   0xE12       // Hypervisor guest external interrupt pending

// .... Hypervisor Configuration
#define CSR_henvcfg                 0x60A       // Hypervisor environment configuration register
#define CSR_henvcfgh                0x61A       // Additional hypervisor env. conf. register, RV32 only

// .... Hypervisor Protection and Translation
#define CSR_hgatp                   0x680       // Hypervisor guest address translation and protection

// .... Debug/Trace Registers
#define CSR_hcontext                0x6A8       // Hypervisor-mode context register

// .... Hypervisor Counter/Timer Virtualization Registers
#define CSR_htimedelta              0x605       // Delta for VS/VU-mode timer
#define CSR_htimedeltah             0x615       // Upper 32 bits of htimedelta, HSXLEN=32 only

// .... Virtual Supervisor Registers
#define CSR_vsstatus                0x200       // Virtual supervisor status register
#define CSR_vsie                    0x204       // Virtual supervisor interrupt-enable register
#define CSR_vstvec                  0x205       // Virtual supervisor trap handler base address
#define CSR_vsscratch               0x240       // Virtual supervisor scratch register
#define CSR_vsepc                   0x241       // Virtual supervisor exception program counter
#define CSR_vscause                 0x242       // Virtual supervisor trap cause
#define CSR_vstval                  0x243       // Virtual supervisor bad address or instruction
#define CSR_vsip                    0x244       // Virtual supervisor interrupt pending
#define CSR_vsatp                   0x280       // Virtual supervisor address translation and protection


// .. Currently allocated RISC-V machine-level CSR addresses
// .... Machine Information Registers
#define CSR_mvendorid               0xF11       // Vendor ID
#define CSR_marchid                 0xF12       // Architecture ID
#define CSR_mimpid                  0xF13       // Implementation ID
#define CSR_mhartid                 0xF14       // Hardware thread ID
#define CSR_mconfigptr              0xF15       // Pointer to configuration data structure

// .... Machine Trap Setup
#define CSR_mstatus                 0x300       // Machine status register
#define CSR_misa                    0x301       // ISA and extensions
#define CSR_medeleg                 0x302       // Machine exception delegation register
#define CSR_mideleg                 0x303       // Machine interrupt delegation register
#define CSR_mie                     0x304       // Machine interrupt-enable register
#define CSR_mtvec                   0x305       // Machine trap-handler base address
#define CSR_mcounteren              0x306       // Machine counter enable
#define CSR_mstatush                0x310       // Additional machine status register, RV32 only

// .... Machine Trap Handling
#define CSR_mscratch                0x340       // Scratch register for machine trap handlers
#define CSR_mepc                    0x341       // Machine exception program counter
#define CSR_mcause                  0x342       // Machine trap cause
#define CSR_mtval                   0x343       // Machine bad address or instruction
#define CSR_mip                     0x344       // Machine interrupt pending
#define CSR_mtinst                  0x34A       // Machine trap instruction (transformed)
#define CSR_mtval2                  0x34B       // Machine bad guest physical address

// .... Machine Configuration
#define CSR_menvcfg                 0x30A       // Machine environment configuration register
#define CSR_menvcfgh                0x31A       // Additional machine env. conf. register, RV32 only
#define CSR_mseccfg                 0x747       // Machine security configuration register
#define CSR_mseccfgh                0x757       // Additional machine security conf. register, RV32 only

// .... Machine Memory Protection
#define CSR_pmpcfg0                 0x3A0       // Physical memory protection configuration
#define CSR_pmpcfg1                 0x3A1       // Physical memory protection configuration, RV32 only
#define CSR_pmpcfg2                 0x3A2       // Physical memory protection configuration
#define CSR_pmpcfg3                 0x3A3       // Physical memory protection configuration, RV32 only
#define CSR_pmpcfg4                 0x3A4       // Physical memory protection configuration
#define CSR_pmpcfg5                 0x3A5       // Physical memory protection configuration, RV32 only
#define CSR_pmpcfg6                 0x3A6       // Physical memory protection configuration
#define CSR_pmpcfg7                 0x3A7       // Physical memory protection configuration, RV32 only
#define CSR_pmpcfg8                 0x3A8       // Physical memory protection configuration
#define CSR_pmpcfg9                 0x3A9       // Physical memory protection configuration, RV32 only
#define CSR_pmpcfg10                0x3AA       // Physical memory protection configuration
#define CSR_pmpcfg11                0x3AB       // Physical memory protection configuration, RV32 only
#define CSR_pmpcfg12                0x3AC       // Physical memory protection configuration
#define CSR_pmpcfg13                0x3AD       // Physical memory protection configuration, RV32 only
#define CSR_pmpcfg14                0x3AE       // Physical memory protection configuration
#define CSR_pmpcfg15                0x3AF       // Physical memory protection configuration, RV32 only

#define CSR_pmpaddr0                0x3B0       // Physical memory protection address register
#define CSR_pmpaddr1                0x3B1       // Physical memory protection address register
#define CSR_pmpaddr2                0x3B2       // Physical memory protection address register
#define CSR_pmpaddr3                0x3B3       // Physical memory protection address register
#define CSR_pmpaddr4                0x3B4       // Physical memory protection address register
#define CSR_pmpaddr5                0x3B5       // Physical memory protection address register
#define CSR_pmpaddr6                0x3B6       // Physical memory protection address register
#define CSR_pmpaddr7                0x3B7       // Physical memory protection address register
#define CSR_pmpaddr8                0x3B8       // Physical memory protection address register
#define CSR_pmpaddr9                0x3B9       // Physical memory protection address register
#define CSR_pmpaddr10               0x3BA       // Physical memory protection address register
#define CSR_pmpaddr11               0x3BB       // Physical memory protection address register
#define CSR_pmpaddr12               0x3BC       // Physical memory protection address register
#define CSR_pmpaddr13               0x3BD       // Physical memory protection address register
#define CSR_pmpaddr14               0x3BE       // Physical memory protection address register
#define CSR_pmpaddr15               0x3BF       // Physical memory protection address register
#define CSR_pmpaddr16               0x3C0       // Physical memory protection address register
#define CSR_pmpaddr17               0x3C1       // Physical memory protection address register
#define CSR_pmpaddr18               0x3C2       // Physical memory protection address register
#define CSR_pmpaddr19               0x3C3       // Physical memory protection address register
#define CSR_pmpaddr20               0x3C4       // Physical memory protection address register
#define CSR_pmpaddr21               0x3C5       // Physical memory protection address register
#define CSR_pmpaddr22               0x3C6       // Physical memory protection address register
#define CSR_pmpaddr23               0x3C7       // Physical memory protection address register
#define CSR_pmpaddr24               0x3C8       // Physical memory protection address register
#define CSR_pmpaddr25               0x3C9       // Physical memory protection address register
#define CSR_pmpaddr26               0x3CA       // Physical memory protection address register
#define CSR_pmpaddr27               0x3CB       // Physical memory protection address register
#define CSR_pmpaddr28               0x3CC       // Physical memory protection address register
#define CSR_pmpaddr29               0x3CD       // Physical memory protection address register
#define CSR_pmpaddr30               0x3CE       // Physical memory protection address register
#define CSR_pmpaddr31               0x3CF       // Physical memory protection address register
#define CSR_pmpaddr32               0x3D0       // Physical memory protection address register
#define CSR_pmpaddr33               0x3D1       // Physical memory protection address register
#define CSR_pmpaddr34               0x3D2       // Physical memory protection address register
#define CSR_pmpaddr35               0x3D3       // Physical memory protection address register
#define CSR_pmpaddr36               0x3D4       // Physical memory protection address register
#define CSR_pmpaddr37               0x3D5       // Physical memory protection address register
#define CSR_pmpaddr38               0x3D6       // Physical memory protection address register
#define CSR_pmpaddr39               0x3D7       // Physical memory protection address register
#define CSR_pmpaddr40               0x3D8       // Physical memory protection address register
#define CSR_pmpaddr41               0x3D9       // Physical memory protection address register
#define CSR_pmpaddr42               0x3DA       // Physical memory protection address register
#define CSR_pmpaddr43               0x3DB       // Physical memory protection address register
#define CSR_pmpaddr44               0x3DC       // Physical memory protection address register
#define CSR_pmpaddr45               0x3DD       // Physical memory protection address register
#define CSR_pmpaddr46               0x3DE       // Physical memory protection address register
#define CSR_pmpaddr47               0x3DF       // Physical memory protection address register
#define CSR_pmpaddr48               0x3E0       // Physical memory protection address register
#define CSR_pmpaddr49               0x3E1       // Physical memory protection address register
#define CSR_pmpaddr50               0x3E2       // Physical memory protection address register
#define CSR_pmpaddr51               0x3E3       // Physical memory protection address register
#define CSR_pmpaddr52               0x3E4       // Physical memory protection address register
#define CSR_pmpaddr53               0x3E5       // Physical memory protection address register
#define CSR_pmpaddr54               0x3E6       // Physical memory protection address register
#define CSR_pmpaddr55               0x3E7       // Physical memory protection address register
#define CSR_pmpaddr56               0x3E8       // Physical memory protection address register
#define CSR_pmpaddr57               0x3E9       // Physical memory protection address register
#define CSR_pmpaddr58               0x3EA       // Physical memory protection address register
#define CSR_pmpaddr59               0x3EB       // Physical memory protection address register
#define CSR_pmpaddr60               0x3EC       // Physical memory protection address register
#define CSR_pmpaddr61               0x3ED       // Physical memory protection address register
#define CSR_pmpaddr62               0x3EE       // Physical memory protection address register
#define CSR_pmpaddr63               0x3EF       // Physical memory protection address register

// .... Machine Counter/Timers
#define CSR_mcycle                  0xB00       // Machine cycle counter
#define CSR_minstret                0xB02       // Machine instructions-retired counter
#define CSR_mhpmcounter3            0xB03       // Machine performance-monitoring counter
#define CSR_mhpmcounter4            0xB04       // Machine performance-monitoring counter
#define CSR_mhpmcounter5            0xB05       // Machine performance-monitoring counter
#define CSR_mhpmcounter6            0xB06       // Machine performance-monitoring counter
#define CSR_mhpmcounter7            0xB07       // Machine performance-monitoring counter
#define CSR_mhpmcounter8            0xB08       // Machine performance-monitoring counter
#define CSR_mhpmcounter9            0xB09       // Machine performance-monitoring counter
#define CSR_mhpmcounter10           0xB0A       // Machine performance-monitoring counter
#define CSR_mhpmcounter11           0xB0B       // Machine performance-monitoring counter
#define CSR_mhpmcounter12           0xB0C       // Machine performance-monitoring counter
#define CSR_mhpmcounter13           0xB0D       // Machine performance-monitoring counter
#define CSR_mhpmcounter14           0xB0E       // Machine performance-monitoring counter
#define CSR_mhpmcounter15           0xB0F       // Machine performance-monitoring counter
#define CSR_mhpmcounter16           0xB10       // Machine performance-monitoring counter
#define CSR_mhpmcounter17           0xB11       // Machine performance-monitoring counter
#define CSR_mhpmcounter18           0xB12       // Machine performance-monitoring counter
#define CSR_mhpmcounter19           0xB13       // Machine performance-monitoring counter
#define CSR_mhpmcounter20           0xB14       // Machine performance-monitoring counter
#define CSR_mhpmcounter21           0xB15       // Machine performance-monitoring counter
#define CSR_mhpmcounter22           0xB16       // Machine performance-monitoring counter
#define CSR_mhpmcounter23           0xB17       // Machine performance-monitoring counter
#define CSR_mhpmcounter24           0xB18       // Machine performance-monitoring counter
#define CSR_mhpmcounter25           0xB19       // Machine performance-monitoring counter
#define CSR_mhpmcounter26           0xB1A       // Machine performance-monitoring counter
#define CSR_mhpmcounter27           0xB1B       // Machine performance-monitoring counter
#define CSR_mhpmcounter28           0xB1C       // Machine performance-monitoring counter
#define CSR_mhpmcounter29           0xB1D       // Machine performance-monitoring counter
#define CSR_mhpmcounter30           0xB1E       // Machine performance-monitoring counter
#define CSR_mhpmcounter31           0xB1F       // Machine performance-monitoring counter

#define CSR_mcycleh                 0xB80       // Machine cycle counter
#define CSR_minstreth               0xB82       // Machine instructions-retired counter
#define CSR_mhpmcounter3h           0xB83       // Machine performance-monitoring counter
#define CSR_mhpmcounter4h           0xB84       // Machine performance-monitoring counter
#define CSR_mhpmcounter5h           0xB85       // Machine performance-monitoring counter
#define CSR_mhpmcounter6h           0xB86       // Machine performance-monitoring counter
#define CSR_mhpmcounter7h           0xB87       // Machine performance-monitoring counter
#define CSR_mhpmcounter8h           0xB88       // Machine performance-monitoring counter
#define CSR_mhpmcounter9h           0xB89       // Machine performance-monitoring counter
#define CSR_mhpmcounter10h          0xB8A       // Machine performance-monitoring counter
#define CSR_mhpmcounter11h          0xB8B       // Machine performance-monitoring counter
#define CSR_mhpmcounter12h          0xB8C       // Machine performance-monitoring counter
#define CSR_mhpmcounter13h          0xB8D       // Machine performance-monitoring counter
#define CSR_mhpmcounter14h          0xB8E       // Machine performance-monitoring counter
#define CSR_mhpmcounter15h          0xB8F       // Machine performance-monitoring counter
#define CSR_mhpmcounter16h          0xB90       // Machine performance-monitoring counter
#define CSR_mhpmcounter17h          0xB91       // Machine performance-monitoring counter
#define CSR_mhpmcounter18h          0xB92       // Machine performance-monitoring counter
#define CSR_mhpmcounter19h          0xB93       // Machine performance-monitoring counter
#define CSR_mhpmcounter20h          0xB94       // Machine performance-monitoring counter
#define CSR_mhpmcounter21h          0xB95       // Machine performance-monitoring counter
#define CSR_mhpmcounter22h          0xB96       // Machine performance-monitoring counter
#define CSR_mhpmcounter23h          0xB97       // Machine performance-monitoring counter
#define CSR_mhpmcounter24h          0xB98       // Machine performance-monitoring counter
#define CSR_mhpmcounter25h          0xB99       // Machine performance-monitoring counter
#define CSR_mhpmcounter26h          0xB9A       // Machine performance-monitoring counter
#define CSR_mhpmcounter27h          0xB9B       // Machine performance-monitoring counter
#define CSR_mhpmcounter28h          0xB9C       // Machine performance-monitoring counter
#define CSR_mhpmcounter29h          0xB9D       // Machine performance-monitoring counter
#define CSR_mhpmcounter30h          0xB9E       // Machine performance-monitoring counter
#define CSR_mhpmcounter31h          0xB9F       // Machine performance-monitoring counter

// .... Machine Counter Setup
#define CSR_mcountinhibit           0x320       // Machine counter-inhibit register
#define CSR_mhpmevent3              0x323       // Machine performance-monitoring event selector
#define CSR_mhpmevent4              0x324       // Machine performance-monitoring event selector
#define CSR_mhpmevent5              0x325       // Machine performance-monitoring event selector
#define CSR_mhpmevent6              0x326       // Machine performance-monitoring event selector
#define CSR_mhpmevent7              0x327       // Machine performance-monitoring event selector
#define CSR_mhpmevent8              0x328       // Machine performance-monitoring event selector
#define CSR_mhpmevent9              0x329       // Machine performance-monitoring event selector
#define CSR_mhpmevent10             0x32A       // Machine performance-monitoring event selector
#define CSR_mhpmevent11             0x32B       // Machine performance-monitoring event selector
#define CSR_mhpmevent12             0x32C       // Machine performance-monitoring event selector
#define CSR_mhpmevent13             0x32D       // Machine performance-monitoring event selector
#define CSR_mhpmevent14             0x32E       // Machine performance-monitoring event selector
#define CSR_mhpmevent15             0x32F       // Machine performance-monitoring event selector
#define CSR_mhpmevent16             0x330       // Machine performance-monitoring event selector
#define CSR_mhpmevent17             0x331       // Machine performance-monitoring event selector
#define CSR_mhpmevent18             0x332       // Machine performance-monitoring event selector
#define CSR_mhpmevent19             0x333       // Machine performance-monitoring event selector
#define CSR_mhpmevent20             0x334       // Machine performance-monitoring event selector
#define CSR_mhpmevent21             0x335       // Machine performance-monitoring event selector
#define CSR_mhpmevent22             0x336       // Machine performance-monitoring event selector
#define CSR_mhpmevent23             0x337       // Machine performance-monitoring event selector
#define CSR_mhpmevent24             0x338       // Machine performance-monitoring event selector
#define CSR_mhpmevent25             0x339       // Machine performance-monitoring event selector
#define CSR_mhpmevent26             0x33A       // Machine performance-monitoring event selector
#define CSR_mhpmevent27             0x33B       // Machine performance-monitoring event selector
#define CSR_mhpmevent28             0x33C       // Machine performance-monitoring event selector
#define CSR_mhpmevent29             0x33D       // Machine performance-monitoring event selector
#define CSR_mhpmevent30             0x33E       // Machine performance-monitoring event selector
#define CSR_mhpmevent31             0x33F       // Machine performance-monitoring event selector

// .... Debug/Trace Registers (shared with Debug Mode)
#define CSR_tselect                 0x7A0       // Debug/Trace trigger register select
#define CSR_tdata1                  0x7A1       // First Debug/Trace trigger data register
#define CSR_tdata2                  0x7A2       // Second Debug/Trace trigger data register
#define CSR_tdata3                  0x7A3       // Third Debug/Trace trigger data register
#define CSR_mcontext                0x7A8       // Machine-mode context register

// .... Debug Mode Registers
#define CSR_dcsr                    0x7B0       // Debug control and status register
#define CSR_dpc                     0x7B1       // Debug PC
#define CSR_dscratch0               0x7B2       // Debug scratch register 0
#define CSR_dscratch1               0x7B3       // Debug scratch register 1


//
#define GET_CSR_FIELD(src, name) \
    ((src & name##_MASK) >> name##_OFFSET)

#define GET_CSR_MXFIELD(src, name, mxlen) \
    ((src & name##_##mxlen##_MASK) >> name##_##mxlen##_OFFSET)

#define SET_CSR_FIELD(dst, name, value) \
    dst = ((dst & ~name##_MASK) | ((value << name##_OFFSET) & name##_MASK))

#define SET_CSR_MXFIELD(dst, name, value, mxlen) \
    dst = ((dst & ~name##_##mxlen##_MASK) | ((value << name##_##mxlen##_OFFSET) & name##_##mxlen##_MASK))

//
#define SET_CSR_BITS(value, mask) \
    (value | mask)

#define CLEAR_CSR_BITS(value, mask) \
    (value & ~mask)


namespace Jasse {

    // RV Privileged Level (for future implementation)
    typedef     uint32_t        RVPrivLevel;

    static constexpr RVPrivLevel    PRIV_USER           = 0b00; // U: User/Application
    static constexpr RVPrivLevel    PRIV_SUPERVISOR     = 0b01; // S: Supervisor
    static constexpr RVPrivLevel    PRIV_MACHINE        = 0b11; // M: Machine

    // Type definition of normal CSR value
    // *Notice: In Jasse implementation, MXLEN is bounded to XLEN.
    //          The actual width of CSR in memory is decided by the maximum XLEN supported.
    //          CSR width modulation is simpilifed, and any MXLEN related implementation
    //          should refer to XLEN value.
    typedef     uint64_t        csr_t;

    // CSR Access
    typedef enum {
        CSR_READ_WRITE = 0,
        CSR_READ_ONLY
    } RVCSRAccess;

    // CSR Field Spec
    typedef enum {
        CSR_WPRI = 0,   // Reserved Writes Preserve Values, Reads Ignore Values
        CSR_WLRL,       // Write/Read Only Legal Values
        CSR_WARL        // Write Any Values, Reads Legal Values
    } RVCSRFieldSpec;

    // CSR Priv Level
    typedef enum {
        CSR_USER = 0,   // Unprivileged and User-Level CSRs
        CSR_SUPERVISOR, // Supervisor-Level CSRs
        CSR_HYPERVISOR, // Hypervisor and VS CSRs
        CSR_MACHINE     // Machine-Level CSRs
    } RVCSRPrivLevel;

    // CSR
    class RVCSR {
    private:
        const int           address;

        const std::string   name;

    public:
        RVCSR(int address, std::string name);
        RVCSR(int address, const char* name);
        RVCSR(const RVCSR& obj);
        ~RVCSR();

        int                     GetAddress() const noexcept;
        RVCSRPrivLevel          GetPrivLevel() const noexcept;
        RVCSRAccess             GetAccess() const noexcept;
        const std::string&      GetName() const noexcept;

        virtual RVCSR*          Clone() = 0;

        virtual bool            CheckBitBound(int bit) noexcept;

        virtual csr_t           GetValue() noexcept = 0;
        virtual void            SetValue(csr_t value) noexcept = 0;
        virtual void            ResetValue() noexcept; // only implemented when RESET action is SPECIFIED

        void                    operator=(const RVCSR& obj) = delete;
    };

    // CSR Allocator
    typedef     RVCSR*      (*RVCSRAllocator)();

    // CSR Definition
    typedef struct {
        uint16_t        address : 12;   // CSR address
        RVCSRAllocator  allocator;      // CSR allocator function
    } RVCSRDefinition;

    // CSR space
    class RVCSRSpace {

#define __RVCSRSPACE_SUB_L2_SIZE            16
#define __RVCSRSPACE_SUB_L1_SIZE            16
#define __RVCSRSPACE_ARR_SIZE               16

    private:
        class SubspaceL2 { // Space -> L1 -> L2, address splitted every 4 bits
        private:
            RVCSR**     csrs;

        public:
            SubspaceL2();
            SubspaceL2(const SubspaceL2& obj);
            ~SubspaceL2();

            RVCSR*  SetCSR(int index, const RVCSRAllocator allocator) noexcept;
            RVCSR*  GetCSR(int index) noexcept;

            void    operator=(const SubspaceL2& obj) = delete;
        };

        class SubspaceL1 {
        private:
            SubspaceL2**    subspaces;

        public:
            SubspaceL1();
            SubspaceL1(const SubspaceL1& obj);
            ~SubspaceL1();

            RVCSR*  SetCSR(int index, const RVCSRAllocator allocator) noexcept;
            RVCSR*  GetCSR(int index) noexcept;

            void    operator=(const SubspaceL1& obj) = delete;
        };

        SubspaceL1**    subspaces;

    public:
        RVCSRSpace();
        RVCSRSpace(std::initializer_list<const RVCSRDefinition> list);
        RVCSRSpace(const RVCSRSpace& obj);
        ~RVCSRSpace();

        bool    CheckBound(int index) const noexcept;

        void    SetCSRs(std::initializer_list<const RVCSRDefinition> list) noexcept;
        RVCSR*  SetCSR(const RVCSRDefinition definition) noexcept;
        RVCSR*  SetCSR(int address, const RVCSRAllocator allocator) noexcept;
        RVCSR*  GetCSR(int address) const noexcept;
        
        RVCSR*  RequireCSR(int address, const char* hint_name = nullptr) const;

        void    operator=(const RVCSRSpace& obj);
    };
}



// Implementation of: class RVCSR
namespace Jasse {
    /*
    const int           address;

    const std::string   name;
    */

    RVCSR::RVCSR(int address, std::string name)
        : address   (address)
        , name      (name)
    { }

    RVCSR::RVCSR(int address, const char* name)
        : address   (address)
        , name      (std::string(name))
    { }

    RVCSR::RVCSR(const RVCSR& obj)
        : address   (obj.address)
        , name      (obj.name)
    { }

    RVCSR::~RVCSR()
    { }

    inline int RVCSR::GetAddress() const noexcept
    {
        return address;
    }

    RVCSRPrivLevel RVCSR::GetPrivLevel() const noexcept
    {
        return RVCSRPrivLevel((address & 0x300) >> 8);
    }

    RVCSRAccess RVCSR::GetAccess() const noexcept
    {
        switch ((address & 0xC00) >> 10)
        {
            case 0b00:
            case 0b01:
            case 0b10:
                return CSR_READ_WRITE;

            case 0b11:
                return CSR_READ_ONLY;

            default:
                std::terminate(); // should not reach here
        }
    }

    inline const std::string& RVCSR::GetName() const noexcept
    {
        return name;
    }

    bool RVCSR::CheckBitBound(int bit) noexcept
    {
        return bit >= 0 && bit < 64;
    }

    void RVCSR::ResetValue() noexcept
    { }
}


// Implementation of: class RVCSRSpace::SubspaceL2
namespace Jasse {
    /*
    RVCSR**     csrs;
    */

    RVCSRSpace::SubspaceL2::SubspaceL2()
        : csrs  (new RVCSR*[__RVCSRSPACE_SUB_L2_SIZE]())
    { }

    RVCSRSpace::SubspaceL2::SubspaceL2(const SubspaceL2& obj)
        : csrs  (new RVCSR*[__RVCSRSPACE_SUB_L2_SIZE])
    {
        for (int i = 0; i < __RVCSRSPACE_SUB_L2_SIZE; i++)
            if (obj.csrs[i])
                csrs[i] = obj.csrs[i]->Clone();
            else
                csrs[i] = nullptr;
    }

    RVCSRSpace::SubspaceL2::~SubspaceL2()
    { 
        for (int i = 0; i < __RVCSRSPACE_SUB_L2_SIZE; i++)
            if (csrs[i])
                delete csrs[i];

        delete[] csrs;
    }

    inline RVCSR* RVCSRSpace::SubspaceL2::SetCSR(int index, const RVCSRAllocator allocator) noexcept
    {
        return (csrs[index & 0x00F] = allocator());
    }

    inline RVCSR* RVCSRSpace::SubspaceL2::GetCSR(int index) noexcept
    {
        return csrs[index & 0x00F];
    }
}

 
// Implementation of: class RVCSRSpace::SubspaceL1
namespace Jasse {
    /*
    SubspaceL2**    subspaces;
    */

    RVCSRSpace::SubspaceL1::SubspaceL1()
        : subspaces (new SubspaceL2*[__RVCSRSPACE_SUB_L1_SIZE]())
    { }

    RVCSRSpace::SubspaceL1::SubspaceL1(const SubspaceL1& obj)
        : subspaces (new SubspaceL2*[__RVCSRSPACE_SUB_L1_SIZE])
    { 
        for (int i = 0; i < __RVCSRSPACE_SUB_L1_SIZE; i++)
            if (obj.subspaces[i])
                subspaces[i] = new SubspaceL2(*obj.subspaces[i]);
            else
                subspaces[i] = nullptr;
    }

    RVCSRSpace::SubspaceL1::~SubspaceL1()
    {
        for (int i = 0; i < __RVCSRSPACE_SUB_L1_SIZE; i++)
            if (subspaces[i])
                delete subspaces[i];

        delete[] subspaces;
    }

    inline RVCSR* RVCSRSpace::SubspaceL1::SetCSR(int index, const RVCSRAllocator allocator) noexcept
    {
        int indexL1 = (index & 0x0F0) >> 4;

        if (!subspaces[indexL1])
            subspaces[indexL1] = new SubspaceL2();

        return subspaces[indexL1]->SetCSR(index, allocator);
    }

    inline RVCSR* RVCSRSpace::SubspaceL1::GetCSR(int index) noexcept
    {
        int indexL1 = (index & 0x0F0) >> 4;

        if (!subspaces[indexL1])
            return nullptr;
        else
            return subspaces[indexL1]->GetCSR(index);
    }
}


// Implementation of: class RVCSRSpace
namespace Jasse {
    /*
    SubspaceL1**    subspaces;
    */

    RVCSRSpace::RVCSRSpace()
        : subspaces (new SubspaceL1*[__RVCSRSPACE_ARR_SIZE]())
    { }

    RVCSRSpace::RVCSRSpace(std::initializer_list<const RVCSRDefinition> list)
        : subspaces (new SubspaceL1*[__RVCSRSPACE_ARR_SIZE]())
    { 
        SetCSRs(list);
    }

    RVCSRSpace::RVCSRSpace(const RVCSRSpace& obj)
        : subspaces (new SubspaceL1*[__RVCSRSPACE_ARR_SIZE]())
    {
        for (int i = 0; i < __RVCSRSPACE_ARR_SIZE; i++)
            if (obj.subspaces[i])
                subspaces[i] = new SubspaceL1(*obj.subspaces[i]);
            else
                subspaces[i] = nullptr;
    }

    RVCSRSpace::~RVCSRSpace()
    {
        for (int i = 0; i < __RVCSRSPACE_ARR_SIZE; i++)
            if (subspaces[i])
                delete subspaces[i];

        delete[] subspaces;
    }

    inline bool RVCSRSpace::CheckBound(int index) const noexcept
    {
        return index >= 0 && index < (1 << 12);
    }

    void RVCSRSpace::SetCSRs(std::initializer_list<const RVCSRDefinition> list) noexcept
    {
        for (auto iter = list.begin(); iter != list.end(); iter++)
            SetCSR(*iter);
    }

    inline RVCSR* RVCSRSpace::SetCSR(const RVCSRDefinition definition) noexcept
    {
        return SetCSR(definition.address, definition.allocator);
    }

    RVCSR* RVCSRSpace::SetCSR(int address, const RVCSRAllocator allocator) noexcept
    {
        int index = (address & 0xF00) >> 8;

        if (!subspaces[index])
            subspaces[index] = new SubspaceL1();

        return subspaces[index]->SetCSR(address, allocator);
    }

    RVCSR* RVCSRSpace::GetCSR(int address) const noexcept
    {
        int index = (address & 0xF00) >> 8;

        if (!subspaces[index])
            return nullptr;
        else
            return subspaces[index]->GetCSR(address);
    }

    RVCSR* RVCSRSpace::RequireCSR(int address, const char* hint_name) const
    {
        int index = (address & 0xF00) >> 8;

        RVCSR* csr;
        if (!subspaces[index])
            csr = nullptr;
        else
            csr = subspaces[index]->GetCSR(address);

        if (!csr)
        {
            std::ostringstream oss;

            oss << "undefined CSR: ";
            oss << "0x" << std::hex << std::setw(3) << std::setfill('0') << address;

            if (hint_name)
                oss << " (" << std::string(hint_name) << ")";

            throw std::out_of_range(oss.str());
        }
        
        return csr; 
    }

    void RVCSRSpace::operator=(const RVCSRSpace& obj)
    {
        for (int i = 0; i < __RVCSRSPACE_ARR_SIZE; i++)
        {
            if (subspaces[i])
                delete subspaces[i];

            if (obj.subspaces[i])
                subspaces[i] = new SubspaceL1(*obj.subspaces[i]);
            else
                subspaces[i] = nullptr;
        }
    }
}

