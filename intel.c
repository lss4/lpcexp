#include "intel.h"
#include "lpc.h"

#define VEN_INTEL                       0x8086
#define VEN_INTEL_LONG                  0x00008086

#define REG_INTEL_LPC_CTRL_INFO         0x8000F800
#define REG_INTEL_LPC_IO_DECODE_RANGE   0x8000F880

#define INTEL_ENABLE_FORWARD_4E4F       0x20000000

uint32_t LPCGenericDecode[] = {0x8000F884, 0x8000F888, 0x8000F88C, 0x8000F890};
int numRanges = 0;

// default ranges
range ranges[] = {
    {0x200, 0xFC}, //200-2FF
    {0x300, 0x70}, //300/310/320/330/340/350/360/370+3
    {0x388, 0x1C}, //380-38F, 390-39F
    {0xA00, 0xFC}  //A00-AFF
};

void PrintIntelLPCStates()
{
    int i;

    uint32_t decodeRange = readpci(REG_INTEL_LPC_IO_DECODE_RANGE);
    printf("LPC I/O Decode Range Config: %08lX\n", decodeRange);

    for (i = 0; i < 4; i++)
    {
        uint32_t genRange;
        genRange = readpci(LPCGenericDecode[i]);
        printf("LPC Generic Decode Range %d Config: %08lX\n", i, genRange);
    }
}

int CheckIntelController()
{
    uint32_t tmp = readpci(REG_INTEL_LPC_CTRL_INFO);
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
    writepci(REG_INTEL_LPC_IO_DECODE_RANGE, readpci(REG_INTEL_LPC_IO_DECODE_RANGE) | INTEL_ENABLE_FORWARD_4E4F);
}

void Configure_LPC_Ranges()
{
    // forward the address ranges to the ISA bridge
    int i;

    for (i = 0; i < numRanges; i++)
    {
        printf("Enabling Range %d : Base %04lX, Mask %02lX, LPC %08lX\n", i, ranges[i].base, ranges[i].mask, LPCEnc(ranges[i].base, ranges[i].mask));
        printf ("Ports : ");
        listports(ranges[i].base, ranges[i].mask);

        // first forward the port in the LPC controller
        writepci(LPCGenericDecode[i], LPCEnc(ranges[i].base, ranges[i].mask));

        // After some testing, it seems the decoders on the F85226 side
        // do not need to be configured.
		// LPC_SetRange(i, ranges[i]);
    }
}

void ProcessArgs(int argc, char* argv[])
{
    // Usage : intisa [base1 mask1] [base2 mask2] [base3 mask3] [base4 mask4]

    // By default it forwards the following ports : 
    // 200-2FF -- Soundblaster DSP
    // 300/310/320/330/340/350/360/370 (+1/2/3) -- MPU-401 MIDI
    // 380-38F, 390-39F -- Adlib
    // A00-AFF -- PnP

    int i;

    if (argc < 3 || !(argc & 0x01))
    {
        printf("No or malformed args, using defaults\n");
        numRanges = 4;
    }
    else
    {
        numRanges = (argc - 1) / 2;
        if (numRanges > 4) numRanges = 4;
        
        for (i = 0; i < numRanges; i++)
        {
            ranges[i].base = strtoul(argv[(i * 2) + 1], NULL, 16);
            ranges[i].mask = strtoul(argv[(i * 2) + 2], NULL, 16);
        }
    }
}
