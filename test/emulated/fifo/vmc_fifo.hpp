
//

#include "vmc.hpp"
#include "common.hpp"

#define     DUT_SIZE                8

using namespace MEMU::Common;


//
static FIFO<int>* dut;



// fifo.ls
bool _FIFO_LS(void* handle, const std::string& cmd,
                            const std::string& paramline,
                            const std::vector<std::string>& params)
{
    VMC_PARAMLIST_COUNT_ZERO(params, "fifo.ls")

    int count = 0;
    int index = dut->GetReadPointer();

    std::cout << "Index        Value" << std::endl;
    std::cout << "-------      -------" << std::endl;

    for (FIFO<int>::Iterator iter = dut->Begin(); iter != dut->End(); iter++)
    {
        printf("%-7d      ", index);
        printf("%-7d\n"    , *iter);

        count++;

        if (++index == dut->GetSize())
            index = 0;
    }

    for (; count < dut->GetSize(); count++)
    {
        printf("%-7d      ", index);
        printf("-\n");

        if (++index == dut->GetSize())
            index = 0;
    }

    return true;
}


// fifo.lsd
bool _FIFO_LSD(void* handle, const std::string& cmd,
                             const std::string& paramline,
                             const std::vector<std::string>& params)
{
    VMC_PARAMLIST_COUNT_ZERO(params, "fifo.lsd")

    std::cout << "Index        Value" << std::endl;
    std::cout << "-------      -------" << std::endl;

    for (int i = 0; i < dut->GetSize(); i++)
    {
        printf("%-7d      ", i);
        printf("%-7d\n"    , dut->GetPayload(i));
    }

    return true;
}


// fifo.clr
bool _FIFO_CLR(void* handle, const std::string& cmd,
                             const std::string& paramline,
                             const std::vector<std::string>& params)
{
    VMC_PARAMLIST_COUNT_ZERO(params, "fifo.clr")

    dut->Clear();

    std::cout << "FIFO reset." << std::endl;

    return true;
}


// fifo.clri
bool _FIFO_CLRI(void* handle, const std::string& cmd,
                              const std::string& paramline,
                              const std::vector<std::string>& params)
{
    VMC_PARAMLIST_COUNT_ZERO(params, "fifo.clri")

    dut->ResetInput();

    std::cout << "FIFO input reset." << std::endl;

    return true;
}


// fifo.push <value>
bool _FIFO_PUSH(void* handle, const std::string& cmd,
                              const std::string& paramline,
                              const std::vector<std::string>& params)
{
    VMC_PARAMLIST_COUNT_EQUALS(params, 1, "fifo.push")

    int value;

    std::istringstream(params[0]) >> value;

    if (dut->PushPayload(value))
        std::cout << "FIFO pushed: " << value << std::endl;
    else
        std::cout << "FIFO push not available." << std::endl;


    return true;
}


// fifo.peek
bool _FIFO_PEEK(void* handle, const std::string& cmd,
                              const std::string& paramline,
                              const std::vector<std::string>& params)
{
    VMC_PARAMLIST_COUNT_ZERO(params, "fifo.peek")

    int value;

    if (dut->PeekPayload(&value))
        std::cout << "FIFO peek: " << value << "." << std::endl;
    else
        std::cout << "FIFO empty." << std::endl;

    return true;
}


// fifo.pop
bool _FIFO_POP(void* handle, const std::string& cmd,
                             const std::string& paramline,
                             const std::vector<std::string>& params)
{
    VMC_PARAMLIST_COUNT_ZERO(params, "fifo.pop")

    if (dut->PopPayload())
        std::cout << "FIFO popped." << std::endl;
    else
        std::cout << "FIFO not popped." << std::endl;

    return true;
}


// fifo.eval
bool _FIFO_EVAL(void* handle, const std::string& cmd,
                              const std::string& paramline,
                              const std::vector<std::string>& params)
{
    VMC_PARAMLIST_COUNT_ZERO(params, "fifo.eval")

    dut->Eval();

    return true;
}


//
void SetupDUT()
{
    dut = new FIFO<int>(DUT_SIZE);
}

void SetupCommands(VMC::VMCHandle handle)
{
    VMC_COMMAND(handle, "fifo.ls"   , _FIFO_LS);
    VMC_COMMAND(handle, "fifo.lsd"  , _FIFO_LSD);
    VMC_COMMAND(handle, "fifo.clr"  , _FIFO_CLR);
    VMC_COMMAND(handle, "fifo.clri" , _FIFO_CLRI);
    VMC_COMMAND(handle, "fifo.push" , _FIFO_PUSH);
    VMC_COMMAND(handle, "fifo.peek" , _FIFO_PEEK);
    VMC_COMMAND(handle, "fifo.pop"  , _FIFO_POP);
    VMC_COMMAND(handle, "fifo.eval" , _FIFO_EVAL);
}
