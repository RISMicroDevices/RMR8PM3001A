
// Verification module (console) for Issue Stage Register Alias Table

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>

#include "vmc.hpp"
#include "vmc_rat.hpp"


VMC::VMCEntity con;

bool Help(void* handle, const std::string& cmd,
                        const std::string& paramline,
                        const std::vector<std::string>& params)
{
    std::cout << "Command usages: " << std::endl;
    std::cout << "- help                              Display help information" << std::endl;
    ECHO_COUT_VMC_BASIC_HELP
    ECHO_COUT_VMC_RAT_HELP
    return true;
}



void InitBasicCommands(VMC::VMCHandle handle)
{
    handle->handlers.push_back(VMC::CommandHandler { std::string("?")      , &Help });
    handle->handlers.push_back(VMC::CommandHandler { std::string("help")   , &Help });

    VMC::Basic::SetupCommands(handle);
}

int main()
{
    InitBasicCommands(&con);
    
    VMC::Setup(&con);
    VMC::RAT::Setup(&con);

    ECHO_COUT_VMC_VERSION
    std::cout << std::endl;

    while (1)
        VMC::Next(&con, std::cin);

    return 0;
}
