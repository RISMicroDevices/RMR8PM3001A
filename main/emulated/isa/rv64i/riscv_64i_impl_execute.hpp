#pragma once
//
// RISC-V Instruction Set Architecture Emulator (Jasse)
//
// Base RV64I, bahaviour function implementations
//

#include "riscv_64i_def_execute.hpp"
#include "riscvexcept.hpp"

// executors
namespace Jasse {

    // ADDI
    RVExecStatus RV64IExecutor_ADDI(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            ctx.arch->GR64()->Get(insn.GetRS1()) + SEXT_W(insn.GetImmediate()));
        
        return EXEC_SEQUENTIAL;
    }

    // SLTI
    RVExecStatus RV64IExecutor_SLTI(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            (int64_t)ctx.arch->GR64()->Get(insn.GetRS1()) < (int64_t)SEXT_W(insn.GetImmediate()));

        return EXEC_SEQUENTIAL;
    }

    // SLTIU
    RVExecStatus RV64IExecutor_SLTIU(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            (uint64_t)ctx.arch->GR64()->Get(insn.GetRS1()) < (uint64_t)SEXT_W(insn.GetImmediate()));

        return EXEC_SEQUENTIAL;
    }

    // ANDI
    RVExecStatus RV64IExecutor_ANDI(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            ctx.arch->GR64()->Get(insn.GetRS1()) & SEXT_W(insn.GetImmediate()));

        return EXEC_SEQUENTIAL;
    }

    // ORI
    RVExecStatus RV64IExecutor_ORI(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            ctx.arch->GR64()->Get(insn.GetRS1()) | SEXT_W(insn.GetImmediate()));

        return EXEC_SEQUENTIAL;
    }

    // XORI
    RVExecStatus RV64IExecutor_XORI(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            ctx.arch->GR64()->Get(insn.GetRS1()) ^ SEXT_W(insn.GetImmediate()));

        return EXEC_SEQUENTIAL;
    }

    
    // SLLI
    RVExecStatus RV64IExecutor_SLLI(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            ctx.arch->GR64()->Get(insn.GetRS1()) << GET_STD_OPERAND(insn.GetRaw(), RV_OPERAND_SHAMT6));
        
        return EXEC_SEQUENTIAL;
    }

    // SRLI
    RVExecStatus RV64IExecutor_SRLI(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            (uint64_t)ctx.arch->GR64()->Get(insn.GetRS1()) >> GET_STD_OPERAND(insn.GetRaw(), RV_OPERAND_SHAMT6));

        return EXEC_SEQUENTIAL;
    }

    // SRAI
    RVExecStatus RV64IExecutor_SRAI(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            (int64_t)ctx.arch->GR64()->Get(insn.GetRS1()) >> GET_STD_OPERAND(insn.GetRaw(), RV_OPERAND_SHAMT6));

        return EXEC_SEQUENTIAL;
    }


    // ADDIW
    RVExecStatus RV64IExecutor_ADDIW(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            SEXT_W((uint32_t)ctx.arch->GR64()->Get(insn.GetRS1()) + insn.GetImmediate()));

        return EXEC_SEQUENTIAL;
    }

    // SLLIW
    RVExecStatus RV64IExecutor_SLLIW(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            SEXT_W((uint32_t)ctx.arch->GR64()->Get(insn.GetRS1()) << GET_STD_OPERAND(insn.GetRaw(), RV_OPERAND_SHAMT5)));

        return EXEC_SEQUENTIAL;
    }

    // SRLIW
    RVExecStatus RV64IExecutor_SRLIW(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            SEXT_W((uint32_t)ctx.arch->GR64()->Get(insn.GetRS1()) >> GET_STD_OPERAND(insn.GetRaw(), RV_OPERAND_SHAMT5)));

        return EXEC_SEQUENTIAL;
    }

    // SRAIW
    RVExecStatus RV64IExecutor_SRAIW(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            SEXT_W((int32_t)ctx.arch->GR64()->Get(insn.GetRS1()) >> GET_STD_OPERAND(insn.GetRaw(), RV_OPERAND_SHAMT5)));

        return EXEC_SEQUENTIAL;
    }


    // ADD
    RVExecStatus RV64IExecutor_ADD(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            ctx.arch->GR64()->Get(insn.GetRS1()) + ctx.arch->GR64()->Get(insn.GetRS2()));

        return EXEC_SEQUENTIAL;
    }

    // SUB
    RVExecStatus RV64IExecutor_SUB(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            ctx.arch->GR64()->Get(insn.GetRS1()) - ctx.arch->GR64()->Get(insn.GetRS2()));

        return EXEC_SEQUENTIAL;
    }

    // SLT
    RVExecStatus RV64IExecutor_SLT(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            (int64_t)ctx.arch->GR64()->Get(insn.GetRS1()) < (int64_t)ctx.arch->GR64()->Get(insn.GetRS2()));

        return EXEC_SEQUENTIAL;
    }

    // SLTU
    RVExecStatus RV64IExecutor_SLTU(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            (uint64_t)ctx.arch->GR64()->Get(insn.GetRS1()) < (uint64_t)ctx.arch->GR64()->Get(insn.GetRS2()));

        return EXEC_SEQUENTIAL;
    }

    // AND
    RVExecStatus RV64IExecutor_AND(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            ctx.arch->GR64()->Get(insn.GetRS1()) & ctx.arch->GR64()->Get(insn.GetRS2()));

        return EXEC_SEQUENTIAL;
    }

    // OR
    RVExecStatus RV64IExecutor_OR(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            ctx.arch->GR64()->Get(insn.GetRS1()) | ctx.arch->GR64()->Get(insn.GetRS2()));

        return EXEC_SEQUENTIAL;
    }

    // XOR
    RVExecStatus RV64IExecutor_XOR(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            ctx.arch->GR64()->Get(insn.GetRS1()) ^ ctx.arch->GR64()->Get(insn.GetRS2()));

        return EXEC_SEQUENTIAL;
    }

    // SLL
    RVExecStatus RV64IExecutor_SLL(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            ctx.arch->GR64()->Get(insn.GetRS1()) << (ctx.arch->GR64()->Get(insn.GetRS2()) & 0x003F));

        return EXEC_SEQUENTIAL;
    }

    // SRL
    RVExecStatus RV64IExecutor_SRL(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            (uint64_t)ctx.arch->GR64()->Get(insn.GetRS1()) >> (ctx.arch->GR64()->Get(insn.GetRS2()) & 0x003F));

        return EXEC_SEQUENTIAL;
    }

    // SRA
    RVExecStatus RV64IExecutor_SRA(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            (int64_t)ctx.arch->GR64()->Get(insn.GetRS1()) >> (ctx.arch->GR64()->Get(insn.GetRS2()) & 0x003F));

        return EXEC_SEQUENTIAL;
    }


    // ADDW
    RVExecStatus RV64IExecutor_ADDW(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            SEXT_W((uint32_t)ctx.arch->GR64()->Get(insn.GetRS1()) + (uint32_t)ctx.arch->GR64()->Get(insn.GetRS2())));

        return EXEC_SEQUENTIAL;
    }

    // SUBW
    RVExecStatus RV64IExecutor_SUBW(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            SEXT_W((uint32_t)ctx.arch->GR64()->Get(insn.GetRS1()) - (uint32_t)ctx.arch->GR64()->Get(insn.GetRS2())));

        return EXEC_SEQUENTIAL;
    }

    // SLLW
    RVExecStatus RV64IExecutor_SLLW(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            SEXT_W((uint32_t)ctx.arch->GR64()->Get(insn.GetRS1()) << (ctx.arch->GR64()->Get(insn.GetRS2()) & 0x001F)));

        return EXEC_SEQUENTIAL;
    }

    // SRLW
    RVExecStatus RV64IExecutor_SRLW(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            SEXT_W((uint32_t)ctx.arch->GR64()->Get(insn.GetRS1()) >> (ctx.arch->GR64()->Get(insn.GetRS2()) & 0x001F)));

        return EXEC_SEQUENTIAL;
    }

    // SRAW
    RVExecStatus RV64IExecutor_SRAW(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            SEXT_W((int32_t)ctx.arch->GR64()->Get(insn.GetRS1()) >> (ctx.arch->GR64()->Get(insn.GetRS2()) & 0x001F)));

        return EXEC_SEQUENTIAL;
    }


    // LUI
    RVExecStatus RV64IExecutor_LUI(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(), 
            SEXT_W(insn.GetImmediate()));

        return EXEC_SEQUENTIAL;
    }

    // AUIPC
    RVExecStatus RV64IExecutor_AUIPC(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(), 
            SEXT_W(insn.GetImmediate()) + ctx.arch->PC().pc64);

        return EXEC_SEQUENTIAL;
    }


    // JAL
    RVExecStatus RV64IExecutor_JAL(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            ctx.arch->PC().pc64 + 4);

        ctx.arch->SetPC64(
            ctx.arch->PC().pc64 + SEXT_W(insn.GetImmediate()));

        return EXEC_PC_JUMP;
    }

    // JALR
    RVExecStatus RV64IExecutor_JALR(RV64I_EXECUTOR_PARAMS)
    {
        ctx.arch->GR64()->Set(insn.GetRD(),
            ctx.arch->PC().pc64 + 4);

        ctx.arch->SetPC64(
            (ctx.arch->GR64()->Get(insn.GetRS1()) + SEXT_W(insn.GetImmediate())) & 0xFFFFFFFFFFFFFFFELU);

        return EXEC_PC_JUMP;
    }


    // BEQ
    RVExecStatus RV64IExecutor_BEQ(RV64I_EXECUTOR_PARAMS)
    {
        if (ctx.arch->GR64()->Get(insn.GetRS1()) == ctx.arch->GR64()->Get(insn.GetRS2()))
        {
            ctx.arch->SetPC64(ctx.arch->PC().pc64 + SEXT_W(insn.GetImmediate()));
            return EXEC_PC_JUMP;
        }
        else
            return EXEC_SEQUENTIAL;
    }

    // BNE
    RVExecStatus RV64IExecutor_BNE(RV64I_EXECUTOR_PARAMS)
    {
        if (ctx.arch->GR64()->Get(insn.GetRS1()) != ctx.arch->GR64()->Get(insn.GetRS2()))
        {
            ctx.arch->SetPC64(ctx.arch->PC().pc64 + SEXT_W(insn.GetImmediate()));
            return EXEC_PC_JUMP;
        }
        else
            return EXEC_SEQUENTIAL;
    }

    // BLT
    RVExecStatus RV64IExecutor_BLT(RV64I_EXECUTOR_PARAMS)
    {
        if ((int64_t)ctx.arch->GR64()->Get(insn.GetRS1()) < (int64_t)ctx.arch->GR64()->Get(insn.GetRS2()))
        {
            ctx.arch->SetPC64(ctx.arch->PC().pc64 + SEXT_W(insn.GetImmediate()));
            return EXEC_PC_JUMP;
        }
        else
            return EXEC_SEQUENTIAL;
    }

    // BLTU
    RVExecStatus RV64IExecutor_BLTU(RV64I_EXECUTOR_PARAMS)
    {
        if ((uint64_t)ctx.arch->GR64()->Get(insn.GetRS1()) < (uint64_t)ctx.arch->GR64()->Get(insn.GetRS2()))
        {
            ctx.arch->SetPC64(ctx.arch->PC().pc64 + SEXT_W(insn.GetImmediate()));
            return EXEC_PC_JUMP;
        }
        else
            return EXEC_SEQUENTIAL;
    }

    // BGE
    RVExecStatus RV64IExecutor_BGE(RV64I_EXECUTOR_PARAMS)
    {
        if ((int64_t)ctx.arch->GR64()->Get(insn.GetRS1()) >= (int64_t)ctx.arch->GR64()->Get(insn.GetRS2()))
        {
            ctx.arch->SetPC64(ctx.arch->PC().pc64 + SEXT_W(insn.GetImmediate()));
            return EXEC_PC_JUMP;
        }
        else
            return EXEC_SEQUENTIAL;
    }

    // BGEU
    RVExecStatus RV64IExecutor_BGEU(RV64I_EXECUTOR_PARAMS)
    {
        if ((uint64_t)ctx.arch->GR64()->Get(insn.GetRS1()) >= (uint64_t)ctx.arch->GR64()->Get(insn.GetRS2()))
        {
            ctx.arch->SetPC64(ctx.arch->PC().pc64 + SEXT_W(insn.GetImmediate()));
            return EXEC_PC_JUMP;
        }
        else
            return EXEC_SEQUENTIAL;
    }



    inline void __RV64I_MEMORYIO_EXCEPTION(const RVInstruction& insn, const RVExecContext& ctx, RVTrapCause cause, addr_t address)
    {
        // Writing into 'mtval'
        // *NOTICE: Action of writing address into 'mtval' is just an implementation-decided
        //          behaviour, not necessary according to privileged specification.
        //          If this action is not included in your processor or implementation, you
        //          could disable this action by "commenting" this part of code.
        RVCSR* mtval = ctx.CSRs->GetCSR(CSR_mtval);

        if (mtval)
            mtval->SetValue(address);
        //

        ctx.trap.TrapEnter(ctx.arch, ctx.CSRs, TRAP_EXCEPTION, cause);
    }

    inline void __RV64I_LOAD_EXCEPTION(const RVInstruction& insn, const RVExecContext& ctx, RVMOPStatus status, addr_t address)
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
                SHOULD_NOT_REACH_HERE;
        }

        __RV64I_MEMORYIO_EXCEPTION(insn, ctx, cause, address);
    }

    // LD
    RVExecStatus RV64IExecutor_LD(RV64I_EXECUTOR_PARAMS)
    {
        addr_t addr = ctx.arch->GR64()->Get(insn.GetRS1()) + SEXT_W(insn.GetImmediate());
        data_t data;

        RVMOPStatus status 
            = ctx.MI->ReadData(addr, MOPW_DOUBLE_WORD, &data);

        if (status == MOP_SUCCESS)
        {
            ctx.arch->GR64()->Set(insn.GetRD(), data.data64);
            return EXEC_SEQUENTIAL;
        }
        else
        {
            __RV64I_LOAD_EXCEPTION(insn, ctx, status, addr);
            return EXEC_TRAP_ENTER;
        }
    }

    // LW
    RVExecStatus RV64IExecutor_LW(RV64I_EXECUTOR_PARAMS)
    {
        addr_t addr = ctx.arch->GR64()->Get(insn.GetRS1()) + SEXT_W(insn.GetImmediate());
        data_t data;

        RVMOPStatus status 
            = ctx.MI->ReadData(addr, MOPW_WORD, &data);

        if (status == MOP_SUCCESS)
        {
            ctx.arch->GR64()->Set(insn.GetRD(), SEXT_W(data.data32));
            return EXEC_SEQUENTIAL;
        }
        else
        {
            __RV64I_LOAD_EXCEPTION(insn, ctx, status, addr);
            return EXEC_TRAP_ENTER;
        }
    }

    // LH
    RVExecStatus RV64IExecutor_LH(RV64I_EXECUTOR_PARAMS)
    {
        addr_t addr = ctx.arch->GR64()->Get(insn.GetRS1()) + SEXT_W(insn.GetImmediate());
        data_t data;

        RVMOPStatus status 
            = ctx.MI->ReadData(addr, MOPW_HALF_WORD, &data);

        if (status == MOP_SUCCESS)
        {
            ctx.arch->GR64()->Set(insn.GetRD(), SEXT_H(data.data16));
            return EXEC_SEQUENTIAL;
        }
        else
        {
            __RV64I_LOAD_EXCEPTION(insn, ctx, status, addr);
            return EXEC_TRAP_ENTER;
        }
    }

    // LB
    RVExecStatus RV64IExecutor_LB(RV64I_EXECUTOR_PARAMS)
    {
        addr_t addr = ctx.arch->GR64()->Get(insn.GetRS1()) + SEXT_W(insn.GetImmediate());
        data_t data;

        RVMOPStatus status
            = ctx.MI->ReadData(addr, MOPW_BYTE, &data);

        if (status == MOP_SUCCESS)
        {
            ctx.arch->GR64()->Set(insn.GetRD(), SEXT_B(data.data8));
            return EXEC_SEQUENTIAL;
        }
        else
        {
            __RV64I_LOAD_EXCEPTION(insn, ctx, status, addr);
            return EXEC_TRAP_ENTER;
        }
    }

    // LWU
    RVExecStatus RV64IExecutor_LWU(RV64I_EXECUTOR_PARAMS)
    {
        addr_t addr = ctx.arch->GR64()->Get(insn.GetRS1()) + SEXT_W(insn.GetImmediate());
        data_t data;

        RVMOPStatus status 
            = ctx.MI->ReadData(addr, MOPW_WORD, &data);

        if (status == MOP_SUCCESS)
        {
            ctx.arch->GR64()->Set(insn.GetRD(), ZEXT_W(data.data32));
            return EXEC_SEQUENTIAL;
        }
        else
        {
            __RV64I_LOAD_EXCEPTION(insn, ctx, status, addr);
            return EXEC_TRAP_ENTER;
        }
    }

    // LHU
    RVExecStatus RV64IExecutor_LHU(RV64I_EXECUTOR_PARAMS)
    {
        addr_t addr = ctx.arch->GR64()->Get(insn.GetRS1()) + SEXT_W(insn.GetImmediate());
        data_t data;

        RVMOPStatus status 
            = ctx.MI->ReadData(addr, MOPW_HALF_WORD, &data);

        if (status == MOP_SUCCESS)
        {
            ctx.arch->GR64()->Set(insn.GetRD(), ZEXT_H(data.data16));
            return EXEC_SEQUENTIAL;
        }
        else
        {
            __RV64I_LOAD_EXCEPTION(insn, ctx, status, addr);
            return EXEC_TRAP_ENTER;
        }
    }

    // LBU
    RVExecStatus RV64IExecutor_LBU(RV64I_EXECUTOR_PARAMS)
    {
        addr_t addr = ctx.arch->GR64()->Get(insn.GetRS1()) + SEXT_W(insn.GetImmediate());
        data_t data;

        RVMOPStatus status
            = ctx.MI->ReadData(addr, MOPW_BYTE, &data);

        if (status == MOP_SUCCESS)
        {
            ctx.arch->GR64()->Set(insn.GetRD(), ZEXT_B(data.data8));
            return EXEC_SEQUENTIAL;
        }
        else
        {
            __RV64I_LOAD_EXCEPTION(insn, ctx, status, addr);
            return EXEC_TRAP_ENTER;
        }
    }


    inline void __RV64I_STORE_EXCEPTION(const RVInstruction& insn, const RVExecContext& ctx, RVMOPStatus status, addr_t address)
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
                SHOULD_NOT_REACH_HERE;
        }

        __RV64I_MEMORYIO_EXCEPTION(insn, ctx, cause, address);
    }
    
    // SD
    RVExecStatus RV64IExecutor_SD(RV64I_EXECUTOR_PARAMS)
    {
        addr_t addr =   ctx.arch->GR64()->Get(insn.GetRS1()) + SEXT_W(insn.GetImmediate());
        data_t data = { ctx.arch->GR64()->Get(insn.GetRS2()) };

        RVMOPStatus status 
            = ctx.MI->WriteData(addr, MOPW_DOUBLE_WORD, data);

        if (status != MOP_SUCCESS)
        {
            __RV64I_STORE_EXCEPTION(insn, ctx, status, addr);
            return EXEC_TRAP_ENTER;
        }
        else
            return EXEC_SEQUENTIAL;
    }

    // SW
    RVExecStatus RV64IExecutor_SW(RV64I_EXECUTOR_PARAMS)
    {
        addr_t addr =   ctx.arch->GR64()->Get(insn.GetRS1()) + SEXT_W(insn.GetImmediate());
        data_t data = { ctx.arch->GR64()->Get(insn.GetRS2()) };

        RVMOPStatus status 
            = ctx.MI->WriteData(addr, MOPW_WORD, data);

        if (status != MOP_SUCCESS)
        {
            __RV64I_STORE_EXCEPTION(insn, ctx, status, addr);
            return EXEC_TRAP_ENTER;
        }
        else
            return EXEC_SEQUENTIAL;
    }

    // SH
    RVExecStatus RV64IExecutor_SH(RV64I_EXECUTOR_PARAMS)
    {
        addr_t addr =   ctx.arch->GR64()->Get(insn.GetRS1()) + SEXT_W(insn.GetImmediate());
        data_t data = { ctx.arch->GR64()->Get(insn.GetRS2()) };

        RVMOPStatus status
            = ctx.MI->WriteData(addr, MOPW_HALF_WORD, data);

        if (status != MOP_SUCCESS)
        {
            __RV64I_STORE_EXCEPTION(insn, ctx, status, addr);
            return EXEC_TRAP_ENTER;
        }
        else
            return EXEC_SEQUENTIAL;
    }

    // SB
    RVExecStatus RV64IExecutor_SB(RV64I_EXECUTOR_PARAMS)
    {
        addr_t addr =   ctx.arch->GR64()->Get(insn.GetRS1()) + SEXT_W(insn.GetImmediate());
        data_t data = { ctx.arch->GR64()->Get(insn.GetRS2()) };

        RVMOPStatus status
            = ctx.MI->WriteData(addr, MOPW_BYTE, data);

        if (status != MOP_SUCCESS)
        {
            __RV64I_STORE_EXCEPTION(insn, ctx, status, addr);
            return EXEC_TRAP_ENTER;
        }
        else
            return EXEC_SEQUENTIAL;
    }


    // FENCE
    RVExecStatus RV64IExecutor_FENCE(RV64I_EXECUTOR_PARAMS)
    {
        // nothing to be done in emulator

        return EXEC_SEQUENTIAL;
    }


    // ECALL
    RVExecStatus RV64IExecutor_ECALL(RV64I_EXECUTOR_PARAMS)
    {
        // only M-mode supported currently

        ctx.trap.TrapEnter(ctx.arch, ctx.CSRs, TRAP_EXCEPTION, EXCEPTION_ECALL_FROM_M);

        return EXEC_TRAP_ENTER;
    }

    // EBREAK
    RVExecStatus RV64IExecutor_EBREAK(RV64I_EXECUTOR_PARAMS)
    {
        ctx.trap.TrapEnter(ctx.arch, ctx.CSRs, TRAP_EXCEPTION, EXCEPTION_BREAKPOINT);

        return EXEC_TRAP_ENTER;
    }

    // MRET
    RVExecStatus RV64IExecutor_MRET(RV64I_EXECUTOR_PARAMS)
    {
        ctx.trap.TrapReturn(ctx.arch, ctx.CSRs);

        return EXEC_TRAP_RETURN;
    }

    // SRET
    RVExecStatus RV64IExecutor_SRET(RV64I_EXECUTOR_PARAMS)
    {
        // TODO to be implemented

        return EXEC_NOT_IMPLEMENTED;
    }

    // WFI
    RVExecStatus RV64IExecutor_WFI(RV64I_EXECUTOR_PARAMS)
    {
        return EXEC_WAIT_FOR_INTERRUPT;
    }

    // TODO ... Add RV64I instructions here ...
}
