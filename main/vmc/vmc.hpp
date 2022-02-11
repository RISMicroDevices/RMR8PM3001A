#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "vmc_misc.hpp"

namespace VMC {

#define VMC_VARNAME_LAST_RETURN                                 "$0"

#define VMC_VAR_DEFAULT_LAST_RETURN_BOOL                        false
#define VMC_VAR_DEFAULT_LAST_RETURN_INT                         (uint64_t)0

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
        bool                        last_mute   = false;

        //
        std::map<std::string, BoolVariable> mapBoolVars     = std::map<std::string, BoolVariable>();

        std::map<std::string, IntVariable>  mapIntVars      = std::map<std::string, IntVariable>();

        bool                                bWarnOnFalse    = true;
    } VMCEntity, *VMCHandle;

    inline void RegisterCommand(VMCHandle handle, const CommandHandler&& command)
    {
        handle->handlers.push_back(command);
    }

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

    inline void SetLastReturnInt(VMCHandle handle, uint64_t val)
    {
        handle->mapIntVars[VMC_VARNAME_LAST_RETURN] = IntVariable(val);
    }

    // 
    void Setup(VMCHandle handle)
    {
        //
        handle->mapBoolVars[VMC_VARNAME_LAST_RETURN] = BoolVariable(VMC_VAR_DEFAULT_LAST_RETURN_BOOL);
        handle->mapIntVars [VMC_VARNAME_LAST_RETURN] = IntVariable(VMC_VAR_DEFAULT_LAST_RETURN_INT);
        
        //
        handle->mapBoolVars["vmc.WarnOnFalse"] = BoolVariable(&(handle->bWarnOnFalse));
    }

    
    //
    bool Next(VMCHandle handle, std::istream& is, bool continuous = false)
    {
        std::string cmdline;

        if (!continuous)
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

            // erase line-splits
            int pos;
            while ((pos = param.find('\r')) != std::string::npos || (pos = param.find('\n')) != std::string::npos)
                param.erase(pos, 1);

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

        if (!handle->last_mute)
        {
            SetLastReturnBool(handle, success);

            if (handle->bWarnOnFalse && !success)
                std::cout << "Command \'" << cmd << "\' exited abnormally maybe." << std::endl;
        }
        else
            handle->last_mute = false;

        return true;
    }

    //
    inline void ShouldNotReachHere(const char* msg = nullptr)
    {
        printf("VMC failure ::ShouldNotReachHere");

        if (msg)
            printf("(\"%s\")", msg);

        printf("\n");

        printf("VMC exit.\n");

        exit(0xDEADBEEF);
    }

}

namespace VMC::Basic {

#define ECHO_COUT_VMC_BASIC_HELP \
    std::cout << "Basic command usages:" << std::endl; \
    std::cout << "- version                           Display version of VMC console " << std::endl; \
    std::cout << "- exit                              Exit VMC console               " << std::endl; \
    std::cout << "- exec <filename>                   Execute VMC script file        " << std::endl; \
    std::cout << "- echo <content...>                 Display information on console " << std::endl; \
    std::cout << "- setbool [name] [value] [-N]       Set or list boolean variables  " << std::endl; \
    std::cout << "- movbool <dst> <src> [-N]          Get boolean variable           " << std::endl; \
    std::cout << "- delbool <name>                    Delete boolean variable        " << std::endl; \
    std::cout << "- setivar [name] [value] [-N]       Set or list integer variables  " << std::endl; \
    std::cout << "- movivar <dst> <src> [-N]          Get integer variable           " << std::endl; \
    std::cout << "- delivar <name>                    Delete integer variable        " << std::endl; \
    std::cout << "- srand <value>                     Set global random seed         " << std::endl; \

    //
    bool Nop(void* handle, const std::string& cmd, 
                           const std::string& paramline, 
                           const std::vector<std::string>& params)
    {
        ((VMCHandle) handle)->last_mute = true;

        return true;
    }

    //
    bool Version(void* handle, const std::string& cmd, 
                               const std::string& paramline, 
                               const std::vector<std::string>& params)
    {
        ((VMCHandle) handle)->last_mute = true;

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

    bool Exec(void* handle, const std::string& cmd, 
                            const std::string& paramline, 
                            const std::vector<std::string>& params)
    {
        std::ifstream ifs(paramline);

        if (!ifs)
        {
            std::cout << "exec: failed to open file" << std::endl;
            return false;
        }

        while (Next((VMCHandle) handle, ifs, true));

        ifs.close();

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
        VMCHandle vmc = (VMCHandle) handle;

        if (params.empty())
        {
            std::cout << "Listing all boolean variables:" << std::endl;

            std::map<std::string, BoolVariable>::iterator iter = vmc->mapBoolVars.begin();
            while (iter != vmc->mapBoolVars.end())
            {
                std::cout << iter->first << " = " << iter->second.Get() << std::endl;
                iter++;
            }

            return true;
        }
        else if (params.size() == 1)
        {
            std::string name = params[0];

            std::map<std::string, BoolVariable>::iterator iter = vmc->mapBoolVars.find(name);
            if (iter == vmc->mapBoolVars.end())
            {
                if (vmc->bWarnOnFalse)
                    std::cout << "No such boolean variable: \'" << name << "\'." << std::endl;

                return false;
            }

            std::cout << iter->first << " = " << iter->second.Get() << std::endl;

            return true;
        }
        else
        {
            std::string name = params[0];
            std::string sval = params[1];

            bool bval;
            std::istringstream(sval) >> std::boolalpha >> bval;

            bool flagN = false;

            for (int i = 2; i < params.size(); i++)
            {
                std::string param = params[i];

                if (param.compare("-N") == 0)
                    flagN = true;
                else
                {
                    std::cout << "Param " << i << " \'" << param << "\' is invalid." << std::endl;
                    return false;
                }
            }

            if (flagN)
            {
                std::map<std::string, BoolVariable>::iterator iter = vmc->mapBoolVars.find(name);
                if (iter != vmc->mapBoolVars.end())
                {
                    if (vmc->bWarnOnFalse)
                        std::cout << "Boolean variable \'" << name << "\' already exists." << std::endl;

                    return false;
                }
            }

            vmc->mapBoolVars[name] = bval;

            std::cout << "Set: " << name << " = " << bval << std::endl;

            return true;
        }
    }

    //
    bool MovBool(void* handle, const std::string& cmd, 
                               const std::string& paramline, 
                               const std::vector<std::string>& params)
    {
        if (params.size() < 2)
        {
            std::cout << "Too much or too less parameter(s) for \'movbool\'" << std::endl;
            return false;
        }

        VMCHandle vmc = (VMCHandle) handle;

        std::string dst = params[0];
        std::string src = params[1];

        bool flagN = false;

        for (int i = 2; i < params.size(); i++)
        {
            std::string param = params[i];

            if (param.compare("-N") == 0)
                flagN = true;
            else
            {
                std::cout << "Param " << i << " \'" << param << "\' is invalid." << std::endl;
                return false;
            }
        }

        if (flagN)
        {
            std::map<std::string, BoolVariable>::iterator iter = vmc->mapBoolVars.find(dst);
            if (iter != vmc->mapBoolVars.end())
            {
                if (vmc->bWarnOnFalse)
                    std::cout << "Boolean variable \'" << dst << "\' already exists." << std::endl;

                return false;
            }
        }

        std::map<std::string, BoolVariable>::iterator iter = vmc->mapBoolVars.find(src);
        if (iter == vmc->mapBoolVars.end())
        {
            std::cout << "Boolean variable \'" << src << "\' does not exist." << std::endl;
            return false;
        }

        vmc->mapBoolVars[dst] = iter->second;

        std::cout << "Set: " << dst << " = " << iter->second.Get() << std::endl;

        return true;
    }

    //
    bool DelBool(void* handle, const std::string& cmd, 
                               const std::string& paramline, 
                               const std::vector<std::string>& params)
    {
        if (params.size() != 1)
        {
            std::cout << "Too much or too less parameter(s) for \'delbool\'" << std::endl;
            return false;
        }

        VMCHandle vmc = (VMCHandle) handle;

        std::string name = params[0];

        vmc->mapBoolVars.erase(name);

        std::cout << "Deleted boolean variable \'" << name << "\'." << std::endl;

        return true;
    }

    //
    bool SetIVar(void* handle, const std::string& cmd, 
                               const std::string& paramline, 
                               const std::vector<std::string>& params)
    {
        VMCHandle vmc = (VMCHandle) handle;

        if (params.empty())
        {
            std::cout << "Listing all integer variables:" << std::endl;

            std::map<std::string, IntVariable>::iterator iter = vmc->mapIntVars.begin();
            while (iter != vmc->mapIntVars.end())
            {
                std::cout << iter->first << " = " << iter->second.Get() << std::endl;
                iter++;
            }

            return true;
        }
        else if (params.size() == 1)
        {
            std::string name = params[0];

            std::map<std::string, IntVariable>::iterator iter = vmc->mapIntVars.find(name);
            if (iter == vmc->mapIntVars.end())
            {
                if (vmc->bWarnOnFalse)
                    std::cout << "No such integer variable: \'" << name << "\'." << std::endl;

                return false;
            }

            std::cout << iter->first << " = " << iter->second.Get() << std::endl;

            return true;
        }
        else
        {
            std::string name = params[0];
            std::string sval = params[1];

            int64_t ival;
            std::istringstream(sval) >> ival;

            bool flagN = false;

            for (int i = 2; i < params.size(); i++)
            {
                std::string param = params[i];

                if (param.compare("-N") == 0)
                    flagN = true;
                else
                {
                    std::cout << "Param " << i << " \'" << param << "\' is invalid." << std::endl;
                    return false;
                }
            }

            if (flagN)
            {
                std::map<std::string, IntVariable>::iterator iter = vmc->mapIntVars.find(name);
                if (iter != vmc->mapIntVars.end())
                {
                    if (vmc->bWarnOnFalse)
                        std::cout << "Integer variable \'" << name << "\' already exists." << std::endl;

                    return false;
                }
            }

            vmc->mapIntVars[name] = ival;

            std::cout << "Set: " << name << " = " << ival << std::endl;

            return true;
        }
    }

    //
    bool MovIVar(void* handle, const std::string& cmd, 
                               const std::string& paramline, 
                               const std::vector<std::string>& params)
    {
        if (params.size() < 2)
        {
            std::cout << "Too much or too less parameter(s) for \'movivar\'" << std::endl;
            return false;
        }

        VMCHandle vmc = (VMCHandle) handle;

        std::string dst = params[0];
        std::string src = params[1];

        bool flagN = false;

        for (int i = 2; i < params.size(); i++)
        {
            std::string param = params[i];

            if (param.compare("-N") == 0)
                flagN = true;
            else
            {
                std::cout << "Param " << i << " \'" << param << "\' is invalid." << std::endl;
                return false;
            }
        }

        if (flagN)
        {
            std::map<std::string, IntVariable>::iterator iter = vmc->mapIntVars.find(dst);
            if (iter != vmc->mapIntVars.end())
            {
                if (vmc->bWarnOnFalse)
                    std::cout << "Integer variable \'" << dst << "\' already exists." << std::endl;

                return false;
            }
        }

        std::map<std::string, IntVariable>::iterator iter = vmc->mapIntVars.find(src);
        if (iter == vmc->mapIntVars.end())
        {
            std::cout << "Integer variable \'" << src << "\' does not exist." << std::endl;
            return false;
        }

        vmc->mapIntVars[dst] = iter->second;

        std::cout << "Set: " << dst << " = " << iter->second.Get() << std::endl;

        return true;
    }

    //
    bool DelIVar(void* handle, const std::string& cmd, 
                               const std::string& paramline, 
                               const std::vector<std::string>& params)
    {
        if (params.size() != 1)
        {
            std::cout << "Too much or too less parameter(s) for \'delivar\'" << std::endl;
            return false;
        }

        VMCHandle vmc = (VMCHandle) handle;

        std::string name = params[0];

        vmc->mapIntVars.erase(name);

        std::cout << "Deleted integer variable \'" << name << "\'." << std::endl;

        return true;
    }

    //
    bool Srand(void* handle, const std::string& cmd, 
                             const std::string& paramline, 
                             const std::vector<std::string>& params)
    {
        if (params.size() != 1)
        {
            std::cout << "Too much or too less parameter(s) for \'srand\'" << std::endl;
            return false;
        }

        std::string param = params[0];
        uint32_t seed;

        if (param.compare("$time") == 0)
            seed = time(NULL);
        else
            std::istringstream(param) >> seed;

        printf("Random seed set: %u (0x%08x)\n", seed, seed);

        return true;
    }

    //
    void SetupCommands(VMCHandle handle)
    {
        RegisterCommand(handle, VMC::CommandHandler { std::string("#")      , &Nop     });
        RegisterCommand(handle, VMC::CommandHandler { std::string("nop")    , &Nop     });
        RegisterCommand(handle, VMC::CommandHandler { std::string("rem")    , &Nop     });
        RegisterCommand(handle, VMC::CommandHandler { std::string("version"), &Version });
        RegisterCommand(handle, VMC::CommandHandler { std::string("exit")   , &Exit });
        RegisterCommand(handle, VMC::CommandHandler { std::string("exec")   , &Exec });
        RegisterCommand(handle, VMC::CommandHandler { std::string("echo")   , &Echo }); 
        RegisterCommand(handle, VMC::CommandHandler { std::string("setbool"), &SetBool });
        RegisterCommand(handle, VMC::CommandHandler { std::string("movbool"), &MovBool });
        RegisterCommand(handle, VMC::CommandHandler { std::string("delbool"), &DelBool });
        RegisterCommand(handle, VMC::CommandHandler { std::string("setivar"), &SetIVar });
        RegisterCommand(handle, VMC::CommandHandler { std::string("movivar"), &MovIVar });
        RegisterCommand(handle, VMC::CommandHandler { std::string("delivar"), &DelIVar });
        RegisterCommand(handle, VMC::CommandHandler { std::string("srand"),   &Srand });
    }
    
}
