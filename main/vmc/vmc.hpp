#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

#include "vmc_misc.hpp"

namespace VMC {

#define VMC_VARNAME_LAST_RETURN                                 "$0"

#define VMC_VAR_DEFAULT_LAST_RETURN_BOOL                        false
#define VMC_VAR_DEFAULT_LAST_RETURN_INT                         0

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

        //
        std::map<std::string, BoolVariable> mapBoolVars     = std::map<std::string, BoolVariable>();

        std::map<std::string, IntVariable>  mapIntVars      = std::map<std::string, IntVariable>();

        bool                                bWarnOnFalse    = true;
    } VMCEntity, *VMCHandle;


    inline bool GetLastReturnBool(VMCHandle handle)
    {
        std::map<std::string, BoolVariable>::iterator iter
            = handle->mapBoolVars.find(VMC_VARNAME_LAST_RETURN);
        
        if (iter == handle->mapBoolVars.end())
            return VMC_VAR_DEFAULT_LAST_RETURN_BOOL;

        return *(iter->second);
    }

    inline void SetLastReturnBool(VMCHandle handle, bool val)
    {
        handle->mapBoolVars[VMC_VARNAME_LAST_RETURN] = BoolVariable(val);
    }

    inline int GetLastReturnInt(VMCHandle handle)
    {
        std::map<std::string, IntVariable>::iterator iter
            = handle->mapIntVars.find(VMC_VARNAME_LAST_RETURN);

        if (iter == handle->mapIntVars.end())
            return VMC_VAR_DEFAULT_LAST_RETURN_INT;

        return *(iter->second);
    }

    inline void SetLastReturnInt(VMCHandle handle, int val)
    {
        handle->mapIntVars[VMC_VARNAME_LAST_RETURN] = IntVariable(val);
    }
    
    // 
    void Setup(VMCHandle handle)
    {
        //
        handle->mapBoolVars[VMC_VARNAME_LAST_RETURN] = BoolVariable(true);
        handle->mapIntVars [VMC_VARNAME_LAST_RETURN] = IntVariable(0);
        
        //
        handle->mapBoolVars["vmc.WarnOnFalse"] = BoolVariable(&(handle->bWarnOnFalse));
    }

    
    //
    bool Next(VMCHandle handle, std::istream& is)
    {
        std::string cmdline;

        std::cout << "#" << handle->name << "> ";
        std::getline(is, cmdline);

        //
        std::istringstream iss(cmdline);

        if (!iss.good())
            return false;

        std::string cmd;
        std::getline(iss, cmd, ' ');

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
        while (iss.good())
        {
            std::getline(iss, param, ' ');

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

        SetLastReturnBool(handle, success);

        if (handle->bWarnOnFalse && !success)
            std::cout << "Command \'" << cmd << "\' exited abnormally maybe." << std::endl;

        return true;
    }

}

namespace VMC::Basic {

#define ECHO_COUT_VMC_BASIC_HELP \
    std::cout << "Basic command usages:" << std::endl; \
    std::cout << "- version                           Display version of VMC console " << std::endl; \
    std::cout << "- exit                              Exit VMC console               " << std::endl; \
    std::cout << "- echo <content...>                 Display information on console " << std::endl; \
    std::cout << "- setbool [name] [value]            Set or list boolean variables  " << std::endl; \
    std::cout << "- delbool <name>                    Delete boolean variable        " << std::endl; \
    std::cout << "- setivar [name] [value]            Set or list integer variables  " << std::endl; \
    std::cout << "- delivar <name>                    Delete integer variable        " << std::endl; \

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
    bool SetBool(void* handle, const std::string& cmd, 
                               const std::string& paramline, 
                               const std::vector<std::string>& params)
    {
        // TODO

        return true;
    }

    //
    bool DelBool(void* handle, const std::string& cmd, 
                               const std::string& paramline, 
                               const std::vector<std::string>& params)
    {
        // TODO

        return true;
    }

    //
    bool SetIVar(void* handle, const std::string& cmd, 
                               const std::string& paramline, 
                               const std::vector<std::string>& params)
    {
        // TODO

        return true;
    }

    //
    bool DelIVar(void* handle, const std::string& cmd, 
                               const std::string& paramline, 
                               const std::vector<std::string>& params)
    {
        // TODO

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
