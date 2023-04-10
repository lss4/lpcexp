#include "helper.h"
#include "intel.h"
#include "lpc.h"
#include "vendor.h"

#define DEV_INTEL_LPC_BUS       0
#define DEV_INTEL_LPC_DEVICE    31
#define DEV_INTEL_LPC_FUNC      0

#define REG_INTEL_LPC_CTRL_INFO         0x00
#define REG_INTEL_LPC_IO_DECODE_RANGE   0x80

#define INTEL_ENABLE_FORWARD_4E4F       0x20000000

uint32_t LPCGenericDecode[] = {0x84, 0x88, 0x8C, 0x90};
int num_intel_ranges = 0;

// default ranges
range intel_ranges[] = {
    {0x200, 0xFC}, //200-2FF
    {0x300, 0x70}, //300/310/320/330/340/350/360/370+3
    {0x388, 0x1C}, //380-38F, 390-39F
    {0xA00, 0xFC}  //A00-AFF
};

uint32_t Read_Intel_LPC(uint32_t reg)
{
    return readpci(DEV_INTEL_LPC_BUS, DEV_INTEL_LPC_DEVICE, DEV_INTEL_LPC_FUNC, reg);
}

void Write_Intel_LPC(uint32_t reg, uint32_t val)
{
    writepci(DEV_INTEL_LPC_BUS, DEV_INTEL_LPC_DEVICE, DEV_INTEL_LPC_FUNC, reg, val);
}

void PrintIntelLPCStates()
{
    int i;

    uint32_t decodeRange = Read_Intel_LPC(REG_INTEL_LPC_IO_DECODE_RANGE);
    printf("LPC I/O Decode Range Config: %08lX\n", decodeRange);

    for (i = 0; i < 4; i++)
    {
        uint32_t genRange;
        genRange = Read_Intel_LPC(LPCGenericDecode[i]);
        printf("LPC Generic Decode Range %d Config: %08lX\n", i, genRange);
    }
}

int CheckIntelLPCController()
{
    uint32_t tmp = Read_Intel_LPC(REG_INTEL_LPC_CTRL_INFO);
    uint32_t ven = tmp & 0x0000FFFF;

    // Check the LPC controller is there
    if (ven != VEN_INTEL_LONG)
    {
        printf("Can't find Intel LPC controller.\n");
        printf("(Got %04lX, expected %04X)\n", ven, VEN_INTEL);
        return 0; // false
    }
    else
    {
        uint32_t dev = tmp >> 16;
        printf("Found Intel LPC Controller. Dev ID: %04lX\n", dev);
        return 1; // true
    }
}

void Enable_LPC_Config_Forward()
{
    // forward addresses 4E and 4F (Fintek ISA bridge config port) to the LPC bus
    Write_Intel_LPC(REG_INTEL_LPC_IO_DECODE_RANGE, Read_Intel_LPC(REG_INTEL_LPC_IO_DECODE_RANGE) | INTEL_ENABLE_FORWARD_4E4F);
}

void Configure_LPC_Ranges()
{
    int i;

    for (i = 0; i < num_intel_ranges; i++)
    {
        printf("Enabling Range %d : Base %04lX, Mask %02lX, LPC %08lX\n", i, intel_ranges[i].base, intel_ranges[i].mask, LPCEnc(intel_ranges[i].base, intel_ranges[i].mask));
        printf ("Ports : ");
        listports(intel_ranges[i].base, intel_ranges[i].mask);

        // forward the address ranges to the ISA bridge
        Write_Intel_LPC(LPCGenericDecode[i], LPCEnc(intel_ranges[i].base, intel_ranges[i].mask));
    }
}

void ProcessIntelArgs(int argc, char* argv[])
{
    // Usage : lpcisa [base1 mask1] [base2 mask2] [base3 mask3] [base4 mask4]

    // By default it forwards the following ports : 
    // 200-2FF -- Soundblaster DSP
    // 300/310/320/330/340/350/360/370 (+1/2/3) -- MPU-401 MIDI
    // 380-38F, 390-39F -- Adlib
    // A00-AFF -- PnP

    int i;

    if (argc < 3 || !(argc & 0x01))
    {
        printf("No or malformed args, using defaults\n");
        num_intel_ranges = 4;
    }
    else
    {
        num_intel_ranges = (argc - 1) / 2;
        if (num_intel_ranges > 4) num_intel_ranges = 4;
        
        for (i = 0; i < num_intel_ranges; i++)
        {
            intel_ranges[i].base = strtoul(argv[(i * 2) + 1], NULL, 16);
            intel_ranges[i].mask = strtoul(argv[(i * 2) + 2], NULL, 16);
        }
    }
}

int main_intel(int argc, char* argv[])
{
    // Check Intel.
    if (!CheckIntelLPCController())
    {
        clearpci();
        return 1;
    }

    #ifdef VERBOSE
    printf("Current LPC Controller status before processing:\n");
    PrintIntelLPCStates();
    #endif

    ProcessIntelArgs(argc, argv);

    Configure_LPC_Ranges();

	// Enable forwarding 4E and 4F.
    Enable_LPC_Config_Forward();

    #ifdef VERBOSE
    printf("Current LPC Controller status after processing:\n");
    PrintIntelLPCStates();
    #endif

    return 0;
}
