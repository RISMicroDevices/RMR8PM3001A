#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

namespace VMC {

#define ECHO_COUT_VMC_VERSION \
    std::cout << "RISMD VMC (Verification Module Console) v0.1" << std::endl; \
    std::cout << "- Build time: " __DATE__ << " " << __TIME__   << std::endl; \
    std::cout << "- Author:     Kumonda221"                     << std::endl;

    typedef bool    (*CommandExecutor)(void* handle, const std::string& cmd, 
                                                     const std::string& paramline, 
                                                     const std::vector<std::string>& params);

    typedef struct {

        std::string     cmd;

        CommandExecutor executor;
    } CommandHandler;

    typedef struct {

        std::string                 name        = std::string("VMC");

        std::vector<CommandHandler> handlers    = std::vector<CommandHandler>();
    } VMCEntity, *VMCHandle;

    
    //
    bool Next(VMCHandle handle)
    {
        std::string cmdline;

        std::cout << "#" << handle->name << "> ";
        std::getline(std::cin, cmdline);

        //
        std::istringstream is(cmdline);

        if (!is.good())
            return false;

        std::string cmd;
        std::getline(is, cmd, ' ');

        if (cmd.empty())
            return false;

        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

        //
        CommandExecutor* executor = nullptr;
        std::vector<CommandHandler>::iterator iter = handle->handlers.begin();
        while (iter != handle->handlers.end())
        {
            if (cmd.compare(iter->cmd) == 0)
            {
                executor = &(iter->executor);
                break;
            }
            
            iter++;
        }

        if (!executor)
        {
            std::cout << "Unknown command \'" << cmd << "\', type \'?\' for help." << std::endl;
            return false;
        }
        
        //
        std::vector<std::string> params;

        std::string param;
        while (is.good())
        {
            std::getline(is, param, ' ');

            if (!param.empty())
                params.push_back(param);
        }

        size_t spindex = cmdline.find(' ');
        std::string paramline;
        if (spindex == std::string::npos)
            paramline = "";
        else
            paramline = cmdline.substr(spindex + 1);

        bool success = (*executor)(handle, cmd, paramline, params);

        if (!success)
            std::cout << "Command \'" << cmd << "\' exited abnormally." << std::endl;

        return true;
    }

}

namespace VMC::Basic {

#define ECHO_COUT_VMC_BASIC_HELP \
    std::cout << "Basic command usages:" << std::endl; \
    std::cout << "- version                           Display version of VMC console" << std::endl; \
    std::cout << "- exit                              Exit VMC console"               << std::endl; \
    std::cout << "- echo <content...>                 Display information on console" << std::endl;

    //
    bool Version(void* handle, const std::string& cmd, 
                               const std::string& paramline, 
                               const std::vector<std::string>& params)
    {
        ECHO_COUT_VMC_VERSION
        return true;
    }

    //
    bool Exit(void* handle, const std::string& cmd, 
                            const std::string& paramline, 
                            const std::vector<std::string>& params)
    {
        exit(EXIT_SUCCESS);
        return true;
    }

    //
    bool Echo(void* handle, const std::string& cmd, 
                            const std::string& paramline, 
                            const std::vector<std::string>& params)
    {
        std::cout << paramline << std::endl;
        return true;
    }


    //
    void SetupCommands(VMCHandle handle)
    {
        handle->handlers.push_back(VMC::CommandHandler { std::string("version"), &Version });
        handle->handlers.push_back(VMC::CommandHandler { std::string("exit")   , &Exit });
        handle->handlers.push_back(VMC::CommandHandler { std::string("echo")   , &Echo }); 
    }
    
}
