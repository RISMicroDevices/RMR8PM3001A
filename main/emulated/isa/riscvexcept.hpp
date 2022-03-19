#pragma once
//
// RISC-V Instruction Set Architecture Privileged
//
// Privileged exception codes (Includes ratified standard only)
//

// Interrupt codes
#define     INTERRUPT_S_SOFTWARE                        1       // Supervisor software interrupt
#define     INTERRUPT_M_SOFTWARE                        3       // Machine software interrupt

#define     INTERRUPT_S_TIMER                           5       // Supervisor timer interrupt
#define     INTERRUPT_M_TIMER                           7       // Machine timer interrupt

#define     INTERRUPT_S_EXTERNAL                        9       // Supervisor external interrupt
#define     INTERRUPT_M_EXTERNAL                        11      // Machine external interrupt


// Exception codes
#define     EXCEPTION_INSTRUCTION_ADDRESS_MISALIGNED    0       // Instruction address misaligned
#define     EXCEPTION_INSTRUCTION_ACCESS_FAULT          1       // Instruction access fault
#define     EXCEPTION_ILLEGAL_INSTRUCTION               2       // Illegal instruction

#define     EXCEPTION_BREAKPOINT                        3       // Breakpoint

#define     EXCEPTION_LOAD_ADDRESS_MISALIGNED           4       // Load address misaligned
#define     EXCEPTION_LOAD_ACCESS_FAULT                 5       // Load access fault

#define     EXCEPTION_STORE_ADDRESS_MISALIGNED          6       // Store address misaligned
#define     EXCEPTION_STORE_ACCESS_FAULT                7       // Store access fault

#define     EXCEPTION_AMO_ADDRESS_MISALIGNED            6       // AMO address misaligned
#define     EXCEPTION_AMO_ACCESS_FAULT                  7       // AMO access fault

#define     EXCEPTION_ECALL_FROM_U                      8       // Environment call from U-mode
#define     EXCEPTION_ECALL_FROM_S                      9       // Environment call from S-mode
#define     EXCEPTION_ECALL_FROM_M                      11      // Environment call from M-mode

#define     EXCEPTION_PAGE_FAULT_INSTRUCTION            12      // Instruction page fault
#define     EXCEPTION_PAGE_FAULT_LOAD                   13      // Load page fault
#define     EXCEPTION_PAGE_FAULT_STORE                  15      // Store page fault
#define     EXCEPTION_PAGE_FAULT_AMO                    15      // AMO page fault


