#include "helper.h"
#include "vendor.h"
#include "lpc.h"
#include "intel.h"
#include "amd.h"

int CheckChipset()
{
    // Check the vendor ID of the root complex to determine the
    // system's chipset.

    uint32_t tmp = readpci(0, 0, 0, 0x00);
    uint32_t ven = tmp & 0x0000FFFF;

    switch (ven)
    {
        case VEN_INTEL_LONG: 
            printf("Detected Intel chipset.\n");
            return IS_INTEL_CHIPSET;
        case VEN_AMD_LONG:
            printf("Detected AMD chipset.\n");
            return IS_AMD_CHIPSET;
        default:
            printf("Detected unsupported chipset. Vendor ID: %04lX\n", ven);
            return CHIPSET_UNSUPPORTED;
    }
}

int main(int argc, char* argv[])
{
    // Usage : lpcisa [base1 mask1] [base2 mask2] [base3 mask3] [base4 mask4]
    // For Intel: up to 4 ranges and masks can be accepted.
    // For AMD: up to 3 ranges and masks can be accepted. However, masks behave differently.

    // First determine the chipset.

    int chipset = CheckChipset();
    int ret;

    switch (chipset)
    {
        case IS_INTEL_CHIPSET:
            ret = main_intel(argc, argv);
            break;
        case IS_AMD_CHIPSET:
            ret = main_amd(argc, argv);
            break;
        case CHIPSET_UNSUPPORTED:
            return 8;
    }

    if (ret)
    {
        // The chipset-specific routine failed to complete.
        return ret;
    }

    // LPC-ISA bridge part is now unified as address decoders do not need to be configured.
    // Only some misc config needed.

    LPC_Enable();

    // Check LPC.
    if (!LPC_Check())
    {
        clearpci();
        return 2;
    }

    #ifdef VERBOSE
    printf("Current LPC-ISA Bridge status before processing:\n");
    LPC_View_Current_State();
    LPC_View_Range_Tables();
    #endif

    LPC_Misc_Config();

    LPC_Clear_DMA();

    #ifdef VERBOSE
    printf("Current LPC-ISA Bridge status after processing:\n");
    LPC_View_Current_State();
    LPC_View_Range_Tables();
    #endif

    return 0;
}
