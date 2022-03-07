
//

#include "vmc.hpp"
#include "core_global.hpp"

#define     DUT_SIZE                8

using namespace MEMU::Core;


// 
static CompressingMemory<int>* dut;


// cpmem.push <value> <valid>
bool _CPMEM_PUSH(void* handle, const std::string& cmd,
                               const std::string& paramline,
                               const std::vector<std::string>& params)
{
    //
    if (params.size() != 2)
    {
        std::cout << "Too much or too less parameter(s) for \'cpmem.push\'." << std::endl;
        return false;
    }

    //
    int next = dut->NextTopAddress();

    if (next == -1)
    {
        std::cout << "Out of memory." << std::endl;
        return false;
    }

    //
    int  value;
    bool valid;

    std::istringstream(params[0]) >> value;
    std::istringstream(params[1]) >> valid;

    dut->SetPayload(next, value);
    dut->SetValid(next, valid);

    std::cout << "Pushed (#" << next << "). Value: " << value << ". Valid: " << valid << "." << std::endl;

    return true;
}


// cpmem.set <index> <value> <valid>
bool _CPMEM_SET(void* handle, const std::string& cmd,
                              const std::string& paramline,
                              const std::vector<std::string>& params)
{
    //
    if (params.size() != 3)
    {
        std::cout << "Too much or too less parameter(s) for \'cpmem.set\'." << std::endl;
        return false;
    }

    //
    int  index;
    int  value;
    bool valid;

    std::istringstream(params[0]) >> index;
    std::istringstream(params[1]) >> value;
    std::istringstream(params[2]) >> valid;

    dut->SetPayload(index, value);
    dut->SetValid(index, valid);

    //
    std::cout << "Set #" << index << ". Value: " << value << ". Valid: " << valid << "." << std::endl;

    return true;
}


// cpmem.setvalue <index> <value>
bool _CPMEM_SETVALUE(void* handle, const std::string& cmd,
                                   const std::string& paramline,
                                   const std::vector<std::string>& params)
{
    //
    if (params.size() != 2)
    {
        std::cout << "Too much or too less parameter(s) for \'cpmem.setvalue\'." << std::endl;
        return false;
    }

    //
    int index;
    int value;

    std::istringstream(params[0]) >> index;
    std::istringstream(params[1]) >> value;

    dut->SetPayload(index, value);

    //
    std::cout << "Set #" << index << ". Value: " << value << "." << std::endl;

    return true;
}


// cpmem.setvalid <index> <value>
bool _CPMEM_SETVALID(void* handle, const std::string& cmd,
                                   const std::string& paramline,
                                   const std::vector<std::string>& params)
{
    //
    if (params.size() != 2)
    {
        std::cout << "Too much or too less parameter(s) for \'cpmem.setvalid\'." << std::endl;
        return false;
    }

    // 
    int  index;
    bool valid;

    std::istringstream(params[0]) >> index;
    std::istringstream(params[1]) >> valid;

    dut->SetValid(index, valid);

    //
    std::cout << "Set #" << index << ". Valid: " << valid << "." << std::endl;

    return true;
}


// cpmem.ls
bool _CPMEM_LS(void* handle, const std::string& cmd,
                             const std::string& paramline,
                             const std::vector<std::string>& params)
{
    if (params.size() != 0)
    {
        std::cout << "Too much or too less parameter(s) for \'cpmem.ls\'." << std::endl;
        return false;
    }

    //
    std::cout << "Addr       Valid        Value" << std::endl;
    std::cout << "-----      -------      -------" << std::endl;

    for (int i = 0; i < dut->GetSize(); i++)
    {
        printf("%-5d      ", i);
        printf("%-7d      ", dut->GetValid(i));

        if (dut->GetValid(i))
            printf("%-7d\n", dut->GetPayload(i));
        else
            printf("-\n");
    }

    return true;
}


// cpmem.eval
bool _CPMEM_EVAL(void* handle, const std::string& cmd,
                               const std::string& paramline,
                               const std::vector<std::string>& params)
{
    if (params.size() != 0)
    {
        std::cout << "Too much or too less parameter(s) for \'cpmem.eval\'." << std::endl;
        return false;
    }


    dut->Eval();

    return true;
}



//
void SetupDUT()
{
    dut = new CompressingMemory<int>(DUT_SIZE);
}

void SetupCommands(VMC::VMCHandle handle)
{
    VMC_COMMAND(handle, "cpmem.push"    ,   _CPMEM_PUSH);
    VMC_COMMAND(handle, "cpmem.set"     ,   _CPMEM_SET);
    VMC_COMMAND(handle, "cpmem.setvalue",   _CPMEM_SETVALUE);
    VMC_COMMAND(handle, "cpmem.setvalid",   _CPMEM_SETVALID);
    VMC_COMMAND(handle, "cpmem.ls"      ,   _CPMEM_LS);
    VMC_COMMAND(handle, "cpmem.eval"    ,   _CPMEM_EVAL);
}
