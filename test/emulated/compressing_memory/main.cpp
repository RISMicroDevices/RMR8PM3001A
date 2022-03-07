
// Verification Module Console for Compressing Memory

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>

#include "main.hpp"


VMC::VMCEntity con;

bool Help(void* handle, const std::string& cmd,
                        const std::string& paramline,
                        const std::vector<std::string>& params)
{
    std::cout << "Command usages: " << std::endl;
    std::cout << "- help                              Display help information" << std::endl;

    ECHO_COUT_VMC_BASIC_HELP

    OnHelp(handle, cmd, paramline, params);

    return true;
}



inline void InitBasicCommands(VMC::VMCHandle handle)
{
    VMC::RegisterCommand(handle, VMC::CommandHandler { std::string("?")      , &Help });
    VMC::RegisterCommand(handle, VMC::CommandHandler { std::string("help")   , &Help });

    VMC::Basic::SetupCommands(handle);
}

int main()
{
    //
    InitBasicCommands(&con);
    VMC::Setup(&con);

    OnSetup(&con);

    //
    ECHO_COUT_VMC_VERSION

    OnHello(&con);

    std::cout << std::endl;

    //
    while (1)
        VMC::Next(&con, std::cin);

    return 0;
}
