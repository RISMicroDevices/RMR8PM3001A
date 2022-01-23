
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

bool Version(void* handle, const std::string& cmd, 
                           const std::string& paramline, 
                           const std::vector<std::string>& params)
{
    ECHO_COUT_VMC_VERSION
    return true;
}

bool Help(void* handle, const std::string& cmd,
                        const std::string& paramline,
                        const std::vector<std::string>& params)
{
    std::cout << "Command usages: " << std::endl;
    std::cout << "- help                          Display help information" << std::endl;
    std::cout << "- version                       Display version of VMC console" << std::endl;
    std::cout << "- exit                          Exit VMC console" << std::endl;
    std::cout << "- echo <content...>             Display information on console" << std::endl;
    return true;
}

bool Exit(void* handle, const std::string& cmd, 
                        const std::string& paramline, 
                        const std::vector<std::string>& params)
{
    exit(EXIT_SUCCESS);
    return true;
}

bool Echo(void* handle, const std::string& cmd, 
                        const std::string& paramline, 
                        const std::vector<std::string>& params)
{
    std::cout << paramline << std::endl;
    return true;
}

void InitBasicCommands(VMC::VMCHandle handle)
{
    handle->handlers.push_back(VMC::CommandHandler { std::string("?")      , &Help });
    handle->handlers.push_back(VMC::CommandHandler { std::string("help")   , &Help });
    handle->handlers.push_back(VMC::CommandHandler { std::string("version"), &Version });
    handle->handlers.push_back(VMC::CommandHandler { std::string("exit")   , &Exit });
    handle->handlers.push_back(VMC::CommandHandler { std::string("echo")   , &Echo }); 
}

int main()
{
    InitBasicCommands(&con);

    ECHO_COUT_VMC_VERSION
    std::cout << std::endl;

    while (1)
        VMC::Next(&con);

    return 0;
}
