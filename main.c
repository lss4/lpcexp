#include "helper.h"
#include "vendor.h"
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
        case VEN_AMD_ATI_LONG:
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
        default:
            return 8;
    }

    if (ret)
    {
        // The chipset-specific routine failed to complete.
        return ret;
    }

    printf("Resetting DMA registers...\n");
    dma_reset();

    return 0;
}
