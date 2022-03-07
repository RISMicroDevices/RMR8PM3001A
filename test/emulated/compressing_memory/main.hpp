#pragma once

//

#include "vmc.hpp"
#include "vmc_compressing_memory.hpp"


void OnHelp(void* handle, const std::string& cmd,
                          const std::string& paramline,
                          const std::vector<std::string>& params)
{

    // Add extended VMC command helps here ...


}


void OnHello(VMC::VMCHandle handle)
{

    // Display hello information here ...

    std::cout << " Specified DUT: Compressing Memory " << std::endl;

}


void OnSetup(VMC::VMCHandle handle)
{

    // Add extended VMC command registeration and set-up code here ...

    SetupDUT();
    SetupCommands(handle);

}
