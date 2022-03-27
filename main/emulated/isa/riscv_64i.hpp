#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Base RV64I
//

#ifndef __INCLUDE_RV64I
#define __INCLUDE_RV64I
#endif

#include "riscv.hpp"
#include "riscvmisc.hpp"
#include "riscvexcept.hpp"
#include "riscv_32i.hpp"


// Function-3
#define RV64I_FUNCT3_MASK                       0x00007000
#define RV64I_FUNCT3_OFFSET                     12

#define RV64I_FUNCT3_ADDIW                      0b000
#define RV64I_FUNCT3_SLLIW                      0b001
#define RV64I_FUNCT3_SRLIW                      0b101
#define RV64I_FUNCT3_SRAIW                      0b101
#define RV64I_FUNCT3_SRLIW__SRAIW               0b101

#define RV64I_FUNCT3_ADDW                       0b000
#define RV64I_FUNCT3_SUBW                       0b000
#define RV64I_FUNCT3_ADDW__SUBW                 0b000
#define RV64I_FUNCT3_SLLW                       0b001
#define RV64I_FUNCT3_SRLW                       0b101
#define RV64I_FUNCT3_SRAW                       0b101
#define RV64I_FUNCT3_SRLW__SRAW                 0b101

#define RV64I_FUNCT3_LD                         0b011
#define RV64I_FUNCT3_LWU                        0b110
#define RV64I_FUNCT3_SD                         0b011


// Function-6
#define RV64I_FUNCT6_MASK                       0xFC000000
#define RV64I_FUNCT6_OFFSET                     26

#define RV64I_FUNCT6_SLLI                       0b000000
#define RV64I_FUNCT6_SRLI                       0b000000
#define RV64I_FUNCT6_SRAI                       0b010000


// Function-7
#define RV64I_FUNCT7_MASK                       0xFE000000
#define RV64I_FUNCT7_OFFSET                     25

#define RV64I_FUNCT7_SLLIW                      0b0000000
#define RV64I_FUNCT7_SRLIW                      0b0000000
#define RV64I_FUNCT7_SRAIW                      0b0100000

#define RV64I_FUNCT7_ADDW                       0b0000000
#define RV64I_FUNCT7_SUBW                       0b0100000

#define RV64I_FUNCT7_SLLW                       0b0000000
#define RV64I_FUNCT7_SRLW                       0b0000000
#define RV64I_FUNCT7_SRAW                       0b0100000





// executors
namespace Jasse {

    // ADDI
    RVExecStatus RV64IExecutor_ADDI(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()] 
            = arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        
        return EXEC_SEQUENTIAL;
    }

    // SLTI
    RVExecStatus RV64IExecutor_SLTI(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = (int64_t)arch.GR64()[insn.GetRS1()] < (int64_t)insn.GetImmediate().imm64;

        return EXEC_SEQUENTIAL;
    }

    // SLTIU
    RVExecStatus RV64IExecutor_SLTIU(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = (uint64_t)arch.GR64()[insn.GetRS1()] < (uint64_t)insn.GetImmediate().imm64;

        return EXEC_SEQUENTIAL;
    }

    // ANDI
    RVExecStatus RV64IExecutor_ANDI(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.GR64()[insn.GetRS1()] & insn.GetImmediate().imm64;

        return EXEC_SEQUENTIAL;
    }

    // ORI
    RVExecStatus RV64IExecutor_ORI(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.GR64()[insn.GetRS1()] | insn.GetImmediate().imm64;

        return EXEC_SEQUENTIAL;
    }

    // XORI
    RVExecStatus RV64IExecutor_XORI(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.GR64()[insn.GetRS1()] ^ insn.GetImmediate().imm64;

        return EXEC_SEQUENTIAL;
    }

    
    // SLLI
    RVExecStatus RV64IExecutor_SLLI(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.GR64()[insn.GetRS1()] << GET_STD_OPERAND(insn.GetRaw(), RV_OPERAND_SHAMT6);
        
        return EXEC_SEQUENTIAL;
    }

    // SRLI
    RVExecStatus RV64IExecutor_SRLI(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = (uint64_t)arch.GR64()[insn.GetRS1()] >> GET_STD_OPERAND(insn.GetRaw(), RV_OPERAND_SHAMT6);

        return EXEC_SEQUENTIAL;
    }

    // SRAI
    RVExecStatus RV64IExecutor_SRAI(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = (int64_t)arch.GR64()[insn.GetRS1()] >> GET_STD_OPERAND(insn.GetRaw(), RV_OPERAND_SHAMT6);

        return EXEC_SEQUENTIAL;
    }


    // ADDIW
    RVExecStatus RV64IExecutor_ADDIW(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = SEXT_W((uint32_t)arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm32);

        return EXEC_SEQUENTIAL;
    }

    // SLLIW
    RVExecStatus RV64IExecutor_SLLIW(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = SEXT_W((uint32_t)arch.GR64()[insn.GetRS1()] << GET_STD_OPERAND(insn.GetRaw(), RV_OPERAND_SHAMT5));

        return EXEC_SEQUENTIAL;
    }

    // SRLIW
    RVExecStatus RV64IExecutor_SRLIW(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = SEXT_W((uint32_t)arch.GR64()[insn.GetRS1()] >> GET_STD_OPERAND(insn.GetRaw(), RV_OPERAND_SHAMT5));

        return EXEC_SEQUENTIAL;
    }

    // SRAIW
    RVExecStatus RV64IExecutor_SRAIW(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = SEXT_W((int32_t)arch.GR64()[insn.GetRS1()] >> GET_STD_OPERAND(insn.GetRaw(), RV_OPERAND_SHAMT5));

        return EXEC_SEQUENTIAL;
    }


    // ADD
    RVExecStatus RV64IExecutor_ADD(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.GR64()[insn.GetRS1()] + arch.GR64()[insn.GetRS2()];

        return EXEC_SEQUENTIAL;
    }

    // SUB
    RVExecStatus RV64IExecutor_SUB(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.GR64()[insn.GetRS1()] - arch.GR64()[insn.GetRS2()];

        return EXEC_SEQUENTIAL;
    }

    // SLT
    RVExecStatus RV64IExecutor_SLT(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = (int64_t)arch.GR64()[insn.GetRS1()] < (int64_t)arch.GR64()[insn.GetRS2()];

        return EXEC_SEQUENTIAL;
    }

    // SLTU
    RVExecStatus RV64IExecutor_SLTU(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = (uint64_t)arch.GR64()[insn.GetRS1()] < (uint64_t)arch.GR64()[insn.GetRS2()];

        return EXEC_SEQUENTIAL;
    }

    // AND
    RVExecStatus RV64IExecutor_AND(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.GR64()[insn.GetRS1()] & arch.GR64()[insn.GetRS2()];

        return EXEC_SEQUENTIAL;
    }

    // OR
    RVExecStatus RV64IExecutor_OR(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.GR64()[insn.GetRS1()] | arch.GR64()[insn.GetRS2()];

        return EXEC_SEQUENTIAL;
    }

    // XOR
    RVExecStatus RV64IExecutor_XOR(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.GR64()[insn.GetRS1()] ^ arch.GR64()[insn.GetRS2()];

        return EXEC_SEQUENTIAL;
    }

    // SLL
    RVExecStatus RV64IExecutor_SLL(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.GR64()[insn.GetRS1()] << (arch.GR64()[insn.GetRS2()] & 0x003F);

        return EXEC_SEQUENTIAL;
    }

    // SRL
    RVExecStatus RV64IExecutor_SRL(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = (uint64_t)arch.GR64()[insn.GetRS1()] >> (arch.GR64()[insn.GetRS2()] & 0x003F);

        return EXEC_SEQUENTIAL;
    }

    // SRA
    RVExecStatus RV64IExecutor_SRA(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = (int64_t)arch.GR64()[insn.GetRS1()] >> (arch.GR64()[insn.GetRS2()] & 0x003F);

        return EXEC_SEQUENTIAL;
    }


    // ADDW
    RVExecStatus RV64IExecutor_ADDW(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = SEXT_W((uint32_t)arch.GR64()[insn.GetRS1()] + (uint32_t)arch.GR64()[insn.GetRS2()]);

        return EXEC_SEQUENTIAL;
    }

    // SUBW
    RVExecStatus RV64IExecutor_SUBW(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = SEXT_W((uint32_t)arch.GR64()[insn.GetRS1()] - (uint32_t)arch.GR64()[insn.GetRS2()]);

        return EXEC_SEQUENTIAL;
    }

    // SLLW
    RVExecStatus RV64IExecutor_SLLW(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = SEXT_W((uint32_t)arch.GR64()[insn.GetRS1()] << (arch.GR64()[insn.GetRS2()] & 0x001F));

        return EXEC_SEQUENTIAL;
    }

    // SRLW
    RVExecStatus RV64IExecutor_SRLW(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = SEXT_W((uint32_t)arch.GR64()[insn.GetRS1()] >> (arch.GR64()[insn.GetRS2()] & 0x001F));

        return EXEC_SEQUENTIAL;
    }

    // SRAW
    RVExecStatus RV64IExecutor_SRAW(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = SEXT_W((int32_t)arch.GR64()[insn.GetRS1()] >> (arch.GR64()[insn.GetRS2()] & 0x001F));

        return EXEC_SEQUENTIAL;
    }


    // LUI
    RVExecStatus RV64IExecutor_LUI(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()] 
            = insn.GetImmediate().imm64;

        return EXEC_SEQUENTIAL;
    }

    // AUIPC
    RVExecStatus RV64IExecutor_AUIPC(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()] 
            = insn.GetImmediate().imm64 + arch.PC().pc64;

        return EXEC_SEQUENTIAL;
    }


    // JAL
    RVExecStatus RV64IExecutor_JAL(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.PC().pc64 + 4;

        arch.SetPC64(
            arch.PC().pc64 + insn.GetImmediate().imm64);

        return EXEC_PC_JUMP;
    }

    // JALR
    RVExecStatus RV64IExecutor_JALR(const RVInstruction& insn, RVArchitectural& arch)
    {
        arch.GR64()[insn.GetRD()]
            = arch.PC().pc64 + 4;

        arch.SetPC64(
            (arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64) & 0xFFFFFFFFFFFFFFFELU);

        return EXEC_PC_JUMP;
    }


    // BEQ
    RVExecStatus RV64IExecutor_BEQ(const RVInstruction& insn, RVArchitectural& arch)
    {
        if (arch.GR64()[insn.GetRS1()] == arch.GR64()[insn.GetRS2()])
        {
            arch.SetPC64(arch.PC().pc64 + insn.GetImmediate().imm64);
            return EXEC_PC_JUMP;
        }
        else
            return EXEC_SEQUENTIAL;
    }

    // BNE
    RVExecStatus RV64IExecutor_BNE(const RVInstruction& insn, RVArchitectural& arch)
    {
        if (arch.GR64()[insn.GetRS1()] != arch.GR64()[insn.GetRS2()])
        {
            arch.SetPC64(arch.PC().pc64 + insn.GetImmediate().imm64);
            return EXEC_PC_JUMP;
        }
        else
            return EXEC_SEQUENTIAL;
    }

    // BLT
    RVExecStatus RV64IExecutor_BLT(const RVInstruction& insn, RVArchitectural& arch)
    {
        if ((int64_t)arch.GR64()[insn.GetRS1()] < (int64_t)arch.GR64()[insn.GetRS2()])
        {
            arch.SetPC64(arch.PC().pc64 + insn.GetImmediate().imm64);
            return EXEC_PC_JUMP;
        }
        else
            return EXEC_SEQUENTIAL;
    }

    // BLTU
    RVExecStatus RV64IExecutor_BLTU(const RVInstruction& insn, RVArchitectural& arch)
    {
        if ((uint64_t)arch.GR64()[insn.GetRS1()] < (uint64_t)arch.GR64()[insn.GetRS2()])
        {
            arch.SetPC64(arch.PC().pc64 + insn.GetImmediate().imm64);
            return EXEC_PC_JUMP;
        }
        else
            return EXEC_SEQUENTIAL;
    }

    // BGE
    RVExecStatus RV64IExecutor_BGE(const RVInstruction& insn, RVArchitectural& arch)
    {
        if ((int64_t)arch.GR64()[insn.GetRS1()] >= (int64_t)arch.GR64()[insn.GetRS2()])
        {
            arch.SetPC64(arch.PC().pc64 + insn.GetImmediate().imm64);
            return EXEC_PC_JUMP;
        }
        else
            return EXEC_SEQUENTIAL;
    }

    // BGEU
    RVExecStatus RV64IExecutor_BGEU(const RVInstruction& insn, RVArchitectural& arch)
    {
        if ((uint64_t)arch.GR64()[insn.GetRS1()] >= (uint64_t)arch.GR64()[insn.GetRS2()])
        {
            arch.SetPC64(arch.PC().pc64 + insn.GetImmediate().imm64);
            return EXEC_PC_JUMP;
        }
        else
            return EXEC_SEQUENTIAL;
    }



    inline void __RV64I_MEMORYIO_EXCEPTION(const RVInstruction& insn, RVArchitectural& arch, RVTrapCause cause, addr_t address)
    {
        // Writing into 'mtval'
        // *NOTICE: Action of writing address into 'mtval' is just an implementation-decided
        //          behaviour, not necessary according to privileged specification.
        //          If this action is not included in your processor or implementation, you
        //          could disable this action by "commenting" this part of code.
        RVCSR* mtval = arch.CSR()->GetCSR(CSR_mtval);

        if (mtval)
            mtval->SetValue(address);
        //

        TrapEnter(arch, TRAP_EXCEPTION, cause);
    }

    inline void __RV64I_LOAD_EXCEPTION(const RVInstruction& insn, RVArchitectural& arch, RVMOPStatus status, addr_t address)
    {
        RVTrapCause cause;

        switch (status)
        {
            case MOP_ADDRESS_MISALIGNED:
                cause = EXCEPTION_LOAD_ADDRESS_MISALIGNED;
                break;

            case MOP_ACCESS_FAULT:
                cause = EXCEPTION_LOAD_ACCESS_FAULT;
                break;

            default:
                ; // SHOULD_NOT_REACH_HERE()
        }

        __RV64I_MEMORYIO_EXCEPTION(insn, arch, cause, address);
    }

    // LD
    RVExecStatus RV64IExecutor_LD(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr = arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data;

        RVMOPStatus status 
            = arch.MI()->Read(addr, MOPW_DOUBLE_WORD, &data);

        if (status == MOP_SUCCESS)
        {
            arch.GR64()[insn.GetRD()] = data.data64;
            return EXEC_SEQUENTIAL;
        }
        else
        {
            __RV64I_LOAD_EXCEPTION(insn, arch, status, addr);
            return EXEC_TRAP_ENTER;
        }
    }

    // LW
    RVExecStatus RV64IExecutor_LW(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr = arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data;

        RVMOPStatus status 
            = arch.MI()->Read(addr, MOPW_WORD, &data);

        if (status == MOP_SUCCESS)
        {
            arch.GR64()[insn.GetRD()] = SEXT_W(data.data32);
            return EXEC_SEQUENTIAL;
        }
        else
        {
            __RV64I_LOAD_EXCEPTION(insn, arch, status, addr);
            return EXEC_TRAP_ENTER;
        }
    }

    // LH
    RVExecStatus RV64IExecutor_LH(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr = arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data;

        RVMOPStatus status 
            = arch.MI()->Read(addr, MOPW_HALF_WORD, &data);

        if (status == MOP_SUCCESS)
        {
            arch.GR64()[insn.GetRD()] = SEXT_H(data.data16);
            return EXEC_SEQUENTIAL;
        }
        else
        {
            __RV64I_LOAD_EXCEPTION(insn, arch, status, addr);
            return EXEC_TRAP_ENTER;
        }
    }

    // LB
    RVExecStatus RV64IExecutor_LB(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr = arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data;

        RVMOPStatus status
            = arch.MI()->Read(addr, MOPW_BYTE, &data);

        if (status == MOP_SUCCESS)
        {
            arch.GR64()[insn.GetRD()] = SEXT_B(data.data8);
            return EXEC_SEQUENTIAL;
        }
        else
        {
            __RV64I_LOAD_EXCEPTION(insn, arch, status, addr);
            return EXEC_TRAP_ENTER;
        }
    }

    // LWU
    RVExecStatus RV64IExecutor_LWU(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr = arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data;

        RVMOPStatus status 
            = arch.MI()->Read(addr, MOPW_WORD, &data);

        if (status == MOP_SUCCESS)
        {
            arch.GR64()[insn.GetRD()] = ZEXT_W(data.data32);
            return EXEC_SEQUENTIAL;
        }
        else
        {
            __RV64I_LOAD_EXCEPTION(insn, arch, status, addr);
            return EXEC_TRAP_ENTER;
        }
    }

    // LHU
    RVExecStatus RV64IExecutor_LHU(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr = arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data;

        RVMOPStatus status 
            = arch.MI()->Read(addr, MOPW_HALF_WORD, &data);

        if (status == MOP_SUCCESS)
        {
            arch.GR64()[insn.GetRD()] = ZEXT_H(data.data16);
            return EXEC_SEQUENTIAL;
        }
        else
        {
            __RV64I_LOAD_EXCEPTION(insn, arch, status, addr);
            return EXEC_TRAP_ENTER;
        }
    }

    // LBU
    RVExecStatus RV64IExecutor_LBU(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr = arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data;

        RVMOPStatus status
            = arch.MI()->Read(addr, MOPW_BYTE, &data);

        if (status == MOP_SUCCESS)
        {
            arch.GR64()[insn.GetRD()] = ZEXT_B(data.data8);
            return EXEC_SEQUENTIAL;
        }
        else
        {
            __RV64I_LOAD_EXCEPTION(insn, arch, status, addr);
            return EXEC_TRAP_ENTER;
        }
    }


    inline void __RV64I_STORE_EXCEPTION(const RVInstruction& insn, RVArchitectural& arch, RVMOPStatus status, addr_t address)
    {
        RVTrapCause cause;

        switch (status)
        {
            case MOP_ADDRESS_MISALIGNED:
                cause = EXCEPTION_STORE_ADDRESS_MISALIGNED;
                break;

            case MOP_ACCESS_FAULT:
                cause = EXCEPTION_STORE_ACCESS_FAULT;
                break;

            default:
                ; // SHOULD_NOT_REACH_HERE()
        }

        __RV64I_MEMORYIO_EXCEPTION(insn, arch, cause, address);
    }
    
    // SD
    RVExecStatus RV64IExecutor_SD(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr =   arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data = { arch.GR64()[insn.GetRS2()] };

        RVMOPStatus status 
            = arch.MI()->Write(addr, MOPW_DOUBLE_WORD, data);

        if (status != MOP_SUCCESS)
        {
            __RV64I_STORE_EXCEPTION(insn, arch, status, addr);
            return EXEC_TRAP_ENTER;
        }
        else
            return EXEC_SEQUENTIAL;
    }

    // SW
    RVExecStatus RV64IExecutor_SW(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr =   arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data = { arch.GR64()[insn.GetRS2()] };

        RVMOPStatus status 
            = arch.MI()->Write(addr, MOPW_WORD, data);

        if (status != MOP_SUCCESS)
        {
            __RV64I_STORE_EXCEPTION(insn, arch, status, addr);
            return EXEC_TRAP_ENTER;
        }
        else
            return EXEC_SEQUENTIAL;
    }

    // SH
    RVExecStatus RV64IExecutor_SH(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr =   arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data = { arch.GR64()[insn.GetRS2()] };

        RVMOPStatus status
            = arch.MI()->Write(addr, MOPW_HALF_WORD, data);

        if (status != MOP_SUCCESS)
        {
            __RV64I_STORE_EXCEPTION(insn, arch, status, addr);
            return EXEC_TRAP_ENTER;
        }
        else
            return EXEC_SEQUENTIAL;
    }

    // SB
    RVExecStatus RV64IExecutor_SB(const RVInstruction& insn, RVArchitectural& arch)
    {
        addr_t addr =   arch.GR64()[insn.GetRS1()] + insn.GetImmediate().imm64;
        data_t data = { arch.GR64()[insn.GetRS2()] };

        RVMOPStatus status
            = arch.MI()->Write(addr, MOPW_BYTE, data);

        if (status != MOP_SUCCESS)
        {
            __RV64I_STORE_EXCEPTION(insn, arch, status, addr);
            return EXEC_TRAP_ENTER;
        }
        else
            return EXEC_SEQUENTIAL;
    }


    // FENCE
    RVExecStatus RV64IExecutor_FENCE(const RVInstruction& insn, RVArchitectural& arch)
    {
        // nothing to be done in emulator

        return EXEC_SEQUENTIAL;
    }


    // ECALL
    RVExecStatus RV64IExecutor_ECALL(const RVInstruction& insn, RVArchitectural& arch)
    {
        // only M-mode supported currently

        TrapEnter(arch, TRAP_EXCEPTION, EXCEPTION_ECALL_FROM_M);

        return EXEC_TRAP_ENTER;
    }

    // EBREAK
    RVExecStatus RV64IExecutor_EBREAK(const RVInstruction& insn, RVArchitectural& arch)
    {
        TrapEnter(arch, TRAP_EXCEPTION, EXCEPTION_BREAKPOINT);

        return EXEC_TRAP_ENTER;
    }

    // MRET
    RVExecStatus RV64IExecutor_MRET(const RVInstruction& insn, RVArchitectural& arch)
    {
        TrapReturn(arch);

        return EXEC_TRAP_RETURN;
    }

    // SRET
    RVExecStatus RV64IExecutor_SRET(const RVInstruction& insn, RVArchitectural& arch)
    {
        // TODO to be implemented

        return EXEC_NOT_IMPLEMENTED;
    }

    // WFI
    RVExecStatus RV64IExecutor_WFI(const RVInstruction& insn, RVArchitectural& arch)
    {
        return EXEC_WAIT_FOR_INTERRUPT;
    }

    // TODO ... Add RV64I instructions here ...
}

// RV Codepoint instances
namespace Jasse {

//  static const RV64IDecoder* const    RV64I = new RV64IDecoder;   // RV64I Decoder Instance

    static const RVCodepoint RV64I_ADDI     = RVCodepoint("addi"    , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_ADDI);
    static const RVCodepoint RV64I_SLTI     = RVCodepoint("slti"    , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_SLTI);
    static const RVCodepoint RV64I_SLTIU    = RVCodepoint("sltiu"   , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_SLTIU);
    static const RVCodepoint RV64I_ANDI     = RVCodepoint("andi"    , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_ANDI);
    static const RVCodepoint RV64I_ORI      = RVCodepoint("ori"     , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_ORI);
    static const RVCodepoint RV64I_XORI     = RVCodepoint("xori"    , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_XORI);
    static const RVCodepoint RV64I_SLLI     = RVCodepoint("slli"    , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_SLLI);
    static const RVCodepoint RV64I_SRLI     = RVCodepoint("srli"    , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_SRLI);
    static const RVCodepoint RV64I_SRAI     = RVCodepoint("srai"    , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_SRAI);

    static const RVCodepoint RV64I_ADDIW    = RVCodepoint("addiw"   , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_ADDIW);
    static const RVCodepoint RV64I_SLLIW    = RVCodepoint("slliw"   , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_SLLIW);
    static const RVCodepoint RV64I_SRLIW    = RVCodepoint("srliw"   , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_SRLIW);
    static const RVCodepoint RV64I_SRAIW    = RVCodepoint("sraiw"   , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_SRAIW);

    static const RVCodepoint RV64I_ADD      = RVCodepoint("add"     , RVTYPE_R, &TextualizeRVTypeR, &RV64IExecutor_ADD);
    static const RVCodepoint RV64I_SUB      = RVCodepoint("sub"     , RVTYPE_R, &TextualizeRVTypeR, &RV64IExecutor_SUB);
    static const RVCodepoint RV64I_SLT      = RVCodepoint("slt"     , RVTYPE_R, &TextualizeRVTypeR, &RV64IExecutor_SLT);
    static const RVCodepoint RV64I_SLTU     = RVCodepoint("sltu"    , RVTYPE_R, &TextualizeRVTypeR, &RV64IExecutor_SLTU);
    static const RVCodepoint RV64I_AND      = RVCodepoint("and"     , RVTYPE_R, &TextualizeRVTypeR, &RV64IExecutor_AND);
    static const RVCodepoint RV64I_OR       = RVCodepoint("or"      , RVTYPE_R, &TextualizeRVTypeR, &RV64IExecutor_OR);
    static const RVCodepoint RV64I_XOR      = RVCodepoint("xor"     , RVTYPE_R, &TextualizeRVTypeR, &RV64IExecutor_XOR);
    static const RVCodepoint RV64I_SLL      = RVCodepoint("sll"     , RVTYPE_R, &TextualizeRVTypeR, &RV64IExecutor_SLL);
    static const RVCodepoint RV64I_SRL      = RVCodepoint("srl"     , RVTYPE_R, &TextualizeRVTypeR, &RV64IExecutor_SRL);
    static const RVCodepoint RV64I_SRA      = RVCodepoint("sra"     , RVTYPE_R, &TextualizeRVTypeR, &RV64IExecutor_SRA);

    static const RVCodepoint RV64I_ADDW     = RVCodepoint("addw"    , RVTYPE_R, &TextualizeRVTypeR, &RV64IExecutor_ADDW);
    static const RVCodepoint RV64I_SUBW     = RVCodepoint("subw"    , RVTYPE_R, &TextualizeRVTypeR, &RV64IExecutor_SUBW);
    static const RVCodepoint RV64I_SLLW     = RVCodepoint("sllw"    , RVTYPE_R, &TextualizeRVTypeR, &RV64IExecutor_SLLW);
    static const RVCodepoint RV64I_SRLW     = RVCodepoint("srlw"    , RVTYPE_R, &TextualizeRVTypeR, &RV64IExecutor_SRLW);
    static const RVCodepoint RV64I_SRAW     = RVCodepoint("sraw"    , RVTYPE_R, &TextualizeRVTypeR, &RV64IExecutor_SRAW);

    static const RVCodepoint RV64I_BEQ      = RVCodepoint("beq"     , RVTYPE_B, &TextualizeRVTypeB, &RV64IExecutor_BEQ);
    static const RVCodepoint RV64I_BNE      = RVCodepoint("bne"     , RVTYPE_B, &TextualizeRVTypeB, &RV64IExecutor_BNE);
    static const RVCodepoint RV64I_BLT      = RVCodepoint("blt"     , RVTYPE_B, &TextualizeRVTypeB, &RV64IExecutor_BLT);
    static const RVCodepoint RV64I_BLTU     = RVCodepoint("bltu"    , RVTYPE_B, &TextualizeRVTypeB, &RV64IExecutor_BLTU);
    static const RVCodepoint RV64I_BGE      = RVCodepoint("bge"     , RVTYPE_B, &TextualizeRVTypeB, &RV64IExecutor_BGE);
    static const RVCodepoint RV64I_BGEU     = RVCodepoint("bgeu"    , RVTYPE_B, &TextualizeRVTypeB, &RV64IExecutor_BGEU);

    static const RVCodepoint RV64I_LD       = RVCodepoint("ld"      , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_LD);
    static const RVCodepoint RV64I_LW       = RVCodepoint("lw"      , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_LW);
    static const RVCodepoint RV64I_LH       = RVCodepoint("lh"      , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_LH);
    static const RVCodepoint RV64I_LB       = RVCodepoint("lb"      , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_LB);
    static const RVCodepoint RV64I_LWU      = RVCodepoint("lwu"     , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_LWU);
    static const RVCodepoint RV64I_LHU      = RVCodepoint("lhu"     , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_LHU);
    static const RVCodepoint RV64I_LBU      = RVCodepoint("lbu"     , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_LBU);

    static const RVCodepoint RV64I_SD       = RVCodepoint("sd"      , RVTYPE_S, &TextualizeRVTypeS, &RV64IExecutor_SD);
    static const RVCodepoint RV64I_SW       = RVCodepoint("sw"      , RVTYPE_S, &TextualizeRVTypeS, &RV64IExecutor_SW);
    static const RVCodepoint RV64I_SH       = RVCodepoint("sh"      , RVTYPE_S, &TextualizeRVTypeS, &RV64IExecutor_SH);
    static const RVCodepoint RV64I_SB       = RVCodepoint("sb"      , RVTYPE_S, &TextualizeRVTypeS, &RV64IExecutor_SB);

    static const RVCodepoint RV64I_ECALL    = RVCodepoint("ecall"   , RVTYPE_I, &TextualizeRVZeroOperand, &RV64IExecutor_ECALL);
    static const RVCodepoint RV64I_EBREAK   = RVCodepoint("ebreak"  , RVTYPE_I, &TextualizeRVZeroOperand, &RV64IExecutor_EBREAK);
    static const RVCodepoint RV64I_MRET     = RVCodepoint("mret"    , RVTYPE_I, &TextualizeRVZeroOperand, &RV64IExecutor_MRET);
    static const RVCodepoint RV64I_SRET     = RVCodepoint("sret"    , RVTYPE_I, &TextualizeRVZeroOperand, &RV64IExecutor_SRET);
    static const RVCodepoint RV64I_WFI      = RVCodepoint("wfi"     , RVTYPE_I, &TextualizeRVZeroOperand, &RV64IExecutor_WFI);

    static const RVCodepoint RV64I_FENCE    = RVCodepoint("fence"   , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_FENCE);

    static const RVCodepoint RV64I_LUI      = RVCodepoint("lui"     , RVTYPE_U, &TextualizeRVTypeU, &RV64IExecutor_LUI);
    static const RVCodepoint RV64I_AUIPC    = RVCodepoint("auipc"   , RVTYPE_U, &TextualizeRVTypeU, &RV64IExecutor_AUIPC);
    
    static const RVCodepoint RV64I_JAL      = RVCodepoint("jal"     , RVTYPE_J, &TextualizeRVTypeJ, &RV64IExecutor_JAL);
    static const RVCodepoint RV64I_JALR     = RVCodepoint("jalr"    , RVTYPE_I, &TextualizeRVTypeI, &RV64IExecutor_JALR);
}


#define RV64I_DECINSN(T, codepoint) \
    (DecodeNormalRV64Type##T(insnraw, insn), insn.SetTrait(codepoint), true)

// codepoints - decoder facilities
namespace Jasse {

    //
    bool RV64ICodePoint_Funct3_ADDI(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_ADDI);
    }

    bool RV64ICodePoint_Funct3_SLTI(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_SLTI);
    }

    bool RV64ICodePoint_Funct3_SLTIU(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_SLTIU);
    }

    bool RV64ICodePoint_Funct3_ANDI(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_ANDI);
    }

    bool RV64ICodePoint_Funct3_ORI(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_ORI);
    }

    bool RV64ICodePoint_Funct3_XORI(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_XORI);
    }

    bool RV64ICodePoint_Funct3_SLLI(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct6 = GET_STD_OPERAND(insnraw, RV64I_FUNCT6);

        if (funct6 == RV64I_FUNCT6_SLLI)
            return RV64I_DECINSN(I, RV64I_SLLI);
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_SRLI__SRAI(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct6 = GET_STD_OPERAND(insnraw, RV64I_FUNCT6);

        if (funct6 == RV64I_FUNCT6_SRLI)
            return RV64I_DECINSN(I, RV64I_SRLI);
        else if (funct6 == RV64I_FUNCT6_SRAI)
            return RV64I_DECINSN(I, RV64I_SRAI);
        else
            return false;
    }

    //
    bool RV64ICodePoint_Funct3_ADDIW(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_ADDIW);
    }

    bool RV64ICodePoint_Funct3_SLLIW(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_SLLIW)
            return RV64I_DECINSN(I, RV64I_SLLIW);
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_SRLIW__SRAIW(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_SRLIW)
            return RV64I_DECINSN(I, RV64I_SRLIW);
        else if (funct7 == RV64I_FUNCT7_SRAIW)
            return RV64I_DECINSN(I, RV64I_SRAIW);
        else
            return false;
    }

    //
    bool RV64ICodePoint_Funct3_ADD__SUB(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV32I_FUNCT7);

        if (funct7 == RV32I_FUNCT7_ADD)
            return RV64I_DECINSN(R, RV64I_ADD);
        else if (funct7 == RV32I_FUNCT7_SUB)
            return RV64I_DECINSN(R, RV64I_SUB);
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_SLT(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV32I_FUNCT7);

        if (funct7 == RV32I_FUNCT7_SLT)
            return RV64I_DECINSN(R, RV64I_SLT);
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_SLTU(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV32I_FUNCT7);

        if (funct7 == RV32I_FUNCT7_SLTU)
            return RV64I_DECINSN(R, RV64I_SLTU);
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_AND(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV32I_FUNCT7);

        if (funct7 == RV32I_FUNCT7_AND)
            return RV64I_DECINSN(R, RV64I_AND);
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_OR(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV32I_FUNCT7);

        if (funct7 == RV32I_FUNCT7_OR)
            return RV64I_DECINSN(R, RV64I_OR);
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_XOR(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV32I_FUNCT7);

        if (funct7 == RV32I_FUNCT7_XOR)
            return RV64I_DECINSN(R, RV64I_XOR);
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_SLL(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV32I_FUNCT7);

        if (funct7 == RV32I_FUNCT7_SLL)
            return RV64I_DECINSN(R, RV64I_SLL);
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_SRL__SRA(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV32I_FUNCT7);

        if (funct7 == RV32I_FUNCT7_SRL)
            return RV64I_DECINSN(R, RV64I_SRL);
        else if (funct7 == RV32I_FUNCT7_SRA)
            return RV64I_DECINSN(R, RV64I_SRA);
        else
            return false;
    }

    //
    bool RV64ICodePoint_Funct3_ADDW__SUBW(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_ADDW)
            return RV64I_DECINSN(R, RV64I_ADDW);
        else if (funct7 == RV64I_FUNCT7_SUBW)
            return RV64I_DECINSN(R, RV64I_SUBW);
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_SLLW(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_SLLW)
            return RV64I_DECINSN(R, RV64I_SLLW);
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_SRLW__SRAW(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_SRLW)
            return RV64I_DECINSN(R, RV64I_SRLW);
        else if (funct7 == RV64I_FUNCT7_SRAW)
            return RV64I_DECINSN(R, RV64I_SRAW);
        else
            return false;
    }


    //
    bool RV64ICodePoint_Funct3_BEQ(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(B, RV64I_BEQ);
    }

    bool RV64ICodePoint_Funct3_BNE(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(B, RV64I_BNE);
    }

    bool RV64ICodePoint_Funct3_BLT(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(B, RV64I_BLT);
    }

    bool RV64ICodePoint_Funct3_BLTU(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(B, RV64I_BLTU);
    }

    bool RV64ICodePoint_Funct3_BGE(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(B, RV64I_BGE);
    }

    bool RV64ICodePoint_Funct3_BGEU(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(B, RV64I_BGEU);
    }

    //
    bool RV64ICodePoint_Funct3_LD(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_LD);
    }

    bool RV64ICodePoint_Funct3_LW(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_LW);
    }

    bool RV64ICodePoint_Funct3_LH(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_LH);
    }

    bool RV64ICodePoint_Funct3_LB(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_LB);
    }

    bool RV64ICodePoint_Funct3_LWU(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_LWU);
    }

    bool RV64ICodePoint_Funct3_LHU(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_LHU);
    }

    bool RV64ICodePoint_Funct3_LBU(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_LBU);
    }

    //
    bool RV64ICodePoint_Funct3_SD(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(S, RV64I_SD);
    }

    bool RV64ICodePoint_Funct3_SW(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(S, RV64I_SW);
    }

    bool RV64ICodePoint_Funct3_SH(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(S, RV64I_SH);
    }

    bool RV64ICodePoint_Funct3_SB(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(S, RV64I_SB);
    }

    //
    bool RV64ICodePoint_Funct12_ECALL(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_ECALL);
    }

    bool RV64ICodePoint_Funct12_EBREAK(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_EBREAK);
    }

    bool RV64ICodePoint_Funct12_MRET(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_MRET);
    }

    bool RV64ICodePoint_Funct12_SRET(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_SRET);
    }

    bool RV64ICodePoint_Funct12_WFI(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_WFI);
    }

    // TODO ... Add RV64I instruction codepoints here ...
}



//
namespace Jasse {

    // RV64I codepoint
    typedef bool (*RV64ICodepoint)(insnraw_t insnraw, RVInstruction& insn);

    // RV64I codegroup (OPCODE base)
    class RV64ICodegroup {
    public:
        virtual bool    Decode(insnraw_t insnraw, RVInstruction& insn) const = 0;
    };


    // RV64I unique codegroup
    class RV64IUniqueCodegroup : public RV64ICodegroup {
    private:
        RV64ICodepoint   codepoint;
    
    public:
        RV64IUniqueCodegroup(RV64ICodepoint codepoint);
        ~RV64IUniqueCodegroup();

        virtual bool    Decode(insnraw_t insnraw, RVInstruction& insn) const override;
    };

    // RV64I FUNCT3 codegroup
    class RV64IFunct3Codegroup : public RV64ICodegroup {
    private:
        RV64ICodepoint*  codepoints; // 8 (2 ^ 3)

    public:
        RV64IFunct3Codegroup();
        ~RV64IFunct3Codegroup();

        void            Define(int funct3, RV64ICodepoint codepoint);
        virtual bool    Decode(insnraw_t insnraw, RVInstruction& insn) const override;
    };


    // RV64I Decoder
    class RV64IDecoder : public RVDecoder {
    private:
        RV64ICodegroup**    codegroups; // 32 (2 ^ (7 - 2))

    public:
        RV64IDecoder();
        ~RV64IDecoder();

        virtual bool    Decode(insnraw_t insnraw, RVInstruction& insn) const override;
    };

    // RV64I Decoder Instance
    static const RV64IDecoder* const    RV64I = new RV64IDecoder;
}


namespace Jasse {

#define __RV64I_UNIQUE_CODEGROUP(name) \
    RV64IUniqueCodegroup_##name

#define __RV64I_UNIQUE_CODEGROUP_DEFINE(name, function) \
    __RV64I_UNIQUE_CODEGROUP(name)::__RV64I_UNIQUE_CODEGROUP(name)() \
        : RV64IUniqueCodegroup (&function) \
    { } \

#define __RV64I_UNIQUE_CODEGROUP_DEFINE_FUNC(name, function) \
    bool function(insnraw_t insnraw, RVInstruction& insn); \
    __RV64I_UNIQUE_CODEGROUP(name)::__RV64I_UNIQUE_CODEGROUP(name)() \
        : RV64IUniqueCodegroup (&function) \
    { } \
    bool function(insnraw_t insnraw, RVInstruction& insn) \

#define __RV64I_UNIQUE_CODEGROUP_DECLARE(name) \
    class __RV64I_UNIQUE_CODEGROUP(name) : public RV64IUniqueCodegroup { \
    public: \
        __RV64I_UNIQUE_CODEGROUP(name)(); \
    }; \


#define __RV64I_FUNCT3_CODEGROUP(name) \
    RV64IFunct3Codegroup_##name

#define __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(name) \
    __RV64I_FUNCT3_CODEGROUP(name)::__RV64I_FUNCT3_CODEGROUP(name)()

#define __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(name) \
    __RV64I_FUNCT3_CODEGROUP(name)::~__RV64I_FUNCT3_CODEGROUP(name)()


#define __RV64I_FUNCT3_CODEGROUP_DECLARE(name) \
    class __RV64I_FUNCT3_CODEGROUP(name) : public RV64IFunct3Codegroup { \
    public: \
        __RV64I_FUNCT3_CODEGROUP(name)(); \
        ~__RV64I_FUNCT3_CODEGROUP(name)(); \
    }; \


    // Codepoint declarations
    __RV64I_FUNCT3_CODEGROUP_DECLARE(OP)
    __RV64I_FUNCT3_CODEGROUP_DECLARE(OP_32)
    __RV64I_FUNCT3_CODEGROUP_DECLARE(OP_IMM)
    __RV64I_FUNCT3_CODEGROUP_DECLARE(OP_IMM_32)

    __RV64I_FUNCT3_CODEGROUP_DECLARE(BRANCH)
    __RV64I_FUNCT3_CODEGROUP_DECLARE(LOAD)
    __RV64I_FUNCT3_CODEGROUP_DECLARE(STORE)

    __RV64I_UNIQUE_CODEGROUP_DECLARE(MISC_MEM)
    __RV64I_UNIQUE_CODEGROUP_DECLARE(SYSTEM)

    __RV64I_UNIQUE_CODEGROUP_DECLARE(LUI)
    __RV64I_UNIQUE_CODEGROUP_DECLARE(AUIPC)
    __RV64I_UNIQUE_CODEGROUP_DECLARE(JAL)
    __RV64I_UNIQUE_CODEGROUP_DECLARE(JALR)
};


// Implementation of: class RV64IUniqueCodegroup
namespace Jasse {
    RV64IUniqueCodegroup::RV64IUniqueCodegroup(RV64ICodepoint codepoint)
        : codepoint (codepoint)
    { }

    RV64IUniqueCodegroup::~RV64IUniqueCodegroup()
    { }

    bool RV64IUniqueCodegroup::Decode(insnraw_t insnraw, RVInstruction& insn) const
    {
        return (*codepoint)(insnraw, insn);
    }
}


// Implementation of: class RV64IFunct3Codegroup
namespace Jasse {
    RV64IFunct3Codegroup::RV64IFunct3Codegroup()
        : codepoints(new RV64ICodepoint[8]())
    { }

    RV64IFunct3Codegroup::~RV64IFunct3Codegroup()
    { 
        delete[] codepoints;
    }

    bool RV64IFunct3Codegroup::Decode(insnraw_t insnraw, RVInstruction& insn) const
    {
        int funct3 = GET_OPERAND(insnraw, RV64I_FUNCT3_MASK, RV64I_FUNCT3_OFFSET);

        if (codepoints[funct3])
            return (*(codepoints[funct3]))(insnraw, insn);
        else
            return false;
    }

    inline void RV64IFunct3Codegroup::Define(int funct3, RV64ICodepoint codepoint)
    {
        codepoints[funct3] = codepoint;
    }
}

// codegroups
namespace Jasse {
    //
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(OP_IMM)
    {
        Define(RV32I_FUNCT3_ADDI,           &RV64ICodePoint_Funct3_ADDI);
        Define(RV32I_FUNCT3_SLTI,           &RV64ICodePoint_Funct3_SLTI);
        Define(RV32I_FUNCT3_SLTIU,          &RV64ICodePoint_Funct3_SLTIU);
        Define(RV32I_FUNCT3_ANDI,           &RV64ICodePoint_Funct3_ANDI);
        Define(RV32I_FUNCT3_ORI,            &RV64ICodePoint_Funct3_ORI);
        Define(RV32I_FUNCT3_XORI,           &RV64ICodePoint_Funct3_XORI);
        Define(RV32I_FUNCT3_SLLI,           &RV64ICodePoint_Funct3_SLLI);
        Define(RV32I_FUNCT3_SRLI__SRAI,     &RV64ICodePoint_Funct3_SRLI__SRAI);
    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(OP_IMM)
    { }

    //
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(OP_IMM_32)
    {
        Define(RV64I_FUNCT3_ADDIW,          &RV64ICodePoint_Funct3_ADDIW);
        Define(RV64I_FUNCT3_SLLIW,          &RV64ICodePoint_Funct3_SLLIW);
        Define(RV64I_FUNCT3_SRLIW__SRAIW,   &RV64ICodePoint_Funct3_SRLIW__SRAIW);
    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(OP_IMM_32)
    { }

    //
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(OP)
    {
        Define(RV32I_FUNCT3_ADD__SUB,       &RV64ICodePoint_Funct3_ADD__SUB);
        Define(RV32I_FUNCT3_SLT,            &RV64ICodePoint_Funct3_SLT);
        Define(RV32I_FUNCT3_SLTU,           &RV64ICodePoint_Funct3_SLTU);
        Define(RV32I_FUNCT3_AND,            &RV64ICodePoint_Funct3_AND);
        Define(RV32I_FUNCT3_OR,             &RV64ICodePoint_Funct3_OR);
        Define(RV32I_FUNCT3_XOR,            &RV64ICodePoint_Funct3_XOR);
        Define(RV32I_FUNCT3_SLL,            &RV64ICodePoint_Funct3_SLL);
        Define(RV32I_FUNCT3_SRL__SRA,       &RV64ICodePoint_Funct3_SRL__SRA);
    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(OP)
    { }

    //
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(OP_32)
    {
        Define(RV64I_FUNCT3_ADDW__SUBW,     &RV64ICodePoint_Funct3_ADDW__SUBW);
        Define(RV64I_FUNCT3_SLLW,           &RV64ICodePoint_Funct3_SLLW);
        Define(RV64I_FUNCT3_SRLW__SRAW,     &RV64ICodePoint_Funct3_SRLW__SRAW);
    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(OP_32)
    { }

    // 
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(BRANCH)
    {
        Define(RV32I_FUNCT3_BEQ,            &RV64ICodePoint_Funct3_BEQ);
        Define(RV32I_FUNCT3_BNE,            &RV64ICodePoint_Funct3_BNE);
        Define(RV32I_FUNCT3_BLT,            &RV64ICodePoint_Funct3_BLT);
        Define(RV32I_FUNCT3_BLTU,           &RV64ICodePoint_Funct3_BLTU);
        Define(RV32I_FUNCT3_BGE,            &RV64ICodePoint_Funct3_BGE);
        Define(RV32I_FUNCT3_BGEU,           &RV64ICodePoint_Funct3_BGEU);
    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(BRANCH)
    { }

    //
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(LOAD)
    {
        Define(RV64I_FUNCT3_LD,             &RV64ICodePoint_Funct3_LD);
        Define(RV32I_FUNCT3_LW,             &RV64ICodePoint_Funct3_LW);
        Define(RV32I_FUNCT3_LH,             &RV64ICodePoint_Funct3_LH);
        Define(RV32I_FUNCT3_LB,             &RV64ICodePoint_Funct3_LB);
        Define(RV64I_FUNCT3_LWU,            &RV64ICodePoint_Funct3_LWU);
        Define(RV32I_FUNCT3_LHU,            &RV64ICodePoint_Funct3_LHU);
        Define(RV32I_FUNCT3_LBU,            &RV64ICodePoint_Funct3_LBU);
    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(LOAD)
    { }

    // 
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(STORE)
    {
        Define(RV64I_FUNCT3_SD,             &RV64ICodePoint_Funct3_SD);
        Define(RV32I_FUNCT3_SW,             &RV64ICodePoint_Funct3_SW);
        Define(RV32I_FUNCT3_SH,             &RV64ICodePoint_Funct3_SH);
        Define(RV32I_FUNCT3_SB,             &RV64ICodePoint_Funct3_SB);
    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(STORE)
    { }

    //
    __RV64I_UNIQUE_CODEGROUP_DEFINE_FUNC(MISC_MEM, RV64ICodeGroup_Unique_MISC_MEM)
    {
        if (GET_STD_OPERAND(insnraw, RV_OPERAND_RD) != 0)
            return false;

        if (GET_STD_OPERAND(insnraw, RV_OPERAND_RS1) != 0)
            return false;

        if (GET_STD_OPERAND(insnraw, RV32I_FUNCT3) != RV32I_FUNCT3_FENCE)
            return false;

        return RV64I_DECINSN(I, RV64I_FENCE);
    }

    // 
    __RV64I_UNIQUE_CODEGROUP_DEFINE_FUNC(SYSTEM, RV64ICodeGroup_Unique_SYSTEM)
    {
        if (GET_STD_OPERAND(insnraw, RV_OPERAND_RD) != 0)
            return false;

        if (GET_STD_OPERAND(insnraw, RV_OPERAND_RS1) != 0)
            return false;

        if (GET_STD_OPERAND(insnraw, RV32I_FUNCT3) != 0)
            return false;

        switch (GET_STD_OPERAND(insnraw, RV32I_FUNCT12))
        {
            case RV32I_FUNCT12_ECALL:
                return RV64ICodePoint_Funct12_ECALL(insnraw, insn);

            case RV32I_FUNCT12_EBREAK:
                return RV64ICodePoint_Funct12_EBREAK(insnraw, insn);

            case RV32I_FUNCT12_MRET:
                return RV64ICodePoint_Funct12_MRET(insnraw, insn);

            case RV32I_FUNCT12_SRET:
                return RV64ICodePoint_Funct12_SRET(insnraw, insn);

            case RV32I_FUNCT12_WFI:
                return RV64ICodePoint_Funct12_WFI(insnraw, insn);

            default:
                return false;
        }
    }

    //
    __RV64I_UNIQUE_CODEGROUP_DEFINE_FUNC(LUI, RV64ICodeGroup_Unique_LUI)
    {
        return RV64I_DECINSN(U, RV64I_LUI);
    }

    //
    __RV64I_UNIQUE_CODEGROUP_DEFINE_FUNC(AUIPC, RV64ICodeGroup_Unique_AUIPC)
    {
        return RV64I_DECINSN(U, RV64I_AUIPC);
    }

    //
    __RV64I_UNIQUE_CODEGROUP_DEFINE_FUNC(JAL, RV64ICodeGroup_Unique_JAL)
    {
        return RV64I_DECINSN(J, RV64I_JAL);
    }

    //
    __RV64I_UNIQUE_CODEGROUP_DEFINE_FUNC(JALR, RV64ICodeGroup_Unique_JALR)
    {
        if (GET_STD_OPERAND(insnraw, RV32I_FUNCT3) == RV32I_FUNCT3_JALR)
            return RV64I_DECINSN(I, RV64I_JALR);
        else
            return false;
    }
}





// Implementation of: class RV64IDecoder
namespace Jasse {
    /*
    RV64IFunct3Codegroup**  codegroups; // 32 (2 ^ (7 - 2))
    */

    RV64IDecoder::RV64IDecoder()
        : RVDecoder     ("I", "RV64I")
        , codegroups    (new RV64ICodegroup*[32]())
    { 
        codegroups[RV_OPCODE_OP         >> 2]   = new __RV64I_FUNCT3_CODEGROUP(OP);
        codegroups[RV_OPCODE_OP_32      >> 2]   = new __RV64I_FUNCT3_CODEGROUP(OP_32);
        codegroups[RV_OPCODE_OP_IMM     >> 2]   = new __RV64I_FUNCT3_CODEGROUP(OP_IMM);
        codegroups[RV_OPCODE_OP_IMM_32  >> 2]   = new __RV64I_FUNCT3_CODEGROUP(OP_IMM_32);

        codegroups[RV_OPCODE_BRANCH     >> 2]   = new __RV64I_FUNCT3_CODEGROUP(BRANCH);
        codegroups[RV_OPCODE_LOAD       >> 2]   = new __RV64I_FUNCT3_CODEGROUP(LOAD);
        codegroups[RV_OPCODE_STORE      >> 2]   = new __RV64I_FUNCT3_CODEGROUP(STORE);

        codegroups[RV_OPCODE_MISC_MEM   >> 2]   = new __RV64I_UNIQUE_CODEGROUP(MISC_MEM);
        codegroups[RV_OPCODE_SYSTEM     >> 2]   = new __RV64I_UNIQUE_CODEGROUP(SYSTEM);

        codegroups[RV_OPCODE_LUI        >> 2]   = new __RV64I_UNIQUE_CODEGROUP(LUI);
        codegroups[RV_OPCODE_AUIPC      >> 2]   = new __RV64I_UNIQUE_CODEGROUP(AUIPC);
        codegroups[RV_OPCODE_JAL        >> 2]   = new __RV64I_UNIQUE_CODEGROUP(JAL);
        codegroups[RV_OPCODE_JALR       >> 2]   = new __RV64I_UNIQUE_CODEGROUP(JALR);
    }

    RV64IDecoder::~RV64IDecoder()
    {
        for (int i = 0; i < 32; i++)
            if (codegroups[i])
                delete codegroups[i];

        delete[] codegroups;
    }

    bool RV64IDecoder::Decode(insnraw_t insnraw, RVInstruction& insn) const
    {
        if ((insnraw & 0x00000003U) != 0x03)
            return false;

        RV64ICodegroup* codegroup 
            = codegroups[GET_STD_OPERAND(insnraw, RV_OPCODE) >> 2];

        if (!codegroup)
            return false;

        return codegroup->Decode(insnraw, insn);
    }
}

