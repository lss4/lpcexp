#include "helper.h"
#include "lpc.h"
#include "intel.h"

int main(int argc, char* argv[])
{
    // Check Intel.
    if (!CheckIntelController())
    {
        clearpci();
        return 1;
    }

    #ifdef VERBOSE
    PrintIntelLPCStates();
    #endif

	// Enable forwarding 4E and 4F.
    Enable_LPC_Config_Forward();

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

    ProcessArgs(argc, argv);

    Configure_LPC_Ranges();

    LPC_Misc_Config();

    LPC_Clear_DMA();

    clearpci();

    return 0;
}
