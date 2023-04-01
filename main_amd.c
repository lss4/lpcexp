#include "helper.h"
#include "lpc.h"
#include "amd.h"

int main(int argc, char* argv[])
{
    // Check AMD.
    if (!CheckAMDController())
    {
        clearpci();
        return 1;
    }

    #ifdef VERBOSE
    ViewAMDLPCStates();
    #endif

    ConfigureLPCIOMapping();
    ConfigureLPCWideRanges();

    LPC_Enable();

    // Check LPC.
    if (!LPC_Check())
    {
        clearpci();
        return 2;
    }

    #ifdef VERBOSE
    LPC_View_Current_State();
    LPC_View_Range_Tables();
    #endif

    // After some testing, it seems the decoders on the F85226 side
    // do not need to be configured.
    // ConfigureLPCBridge();
    
	LPC_Misc_Config();

    LPC_Clear_DMA();

    return 0;
}

