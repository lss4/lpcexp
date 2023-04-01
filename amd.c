#include "amd.h"
#include "lpc.h"

#define VEN_AMD                         0x1022
#define VEN_AMD_LONG                    0x00001022

#define REG_AMD_LPC_CTRL_INFO           0x8000A300
#define REG_AMD_LPC_PCI_CTRL            0x8000A340 // PCI Control
#define REG_AMD_LPC_IO_PORT_DECODE      0x8000A344 // For common ports
#define REG_AMD_LPC_IOMEM_DECODE        0x8000A348 // For some other ranges as well as enabling Wide I/O.
#define REG_AMD_LPC_WIDE_IO_0_AND_1     0x8000A364
#define REG_AMD_LPC_MISC_CTRL           0x8000A378
#define REG_AMD_LPC_WIDE_IO_2           0x8000A390

#define AMD_ENABLE_LDRQ                 0x0000000C
#define AMD_ENABLE_AUDIO_IO_MAP         0x93FFD700
#define AMD_ENABLE_ALTCFG_AND_WIDE_IO   0x03000006

// Forwarding ranges
#define NUM_RANGES      8
range ranges[] = {
    {0x200, 0xFF}, // IO_PORT_DECODE register ports.
    {0x300, 0xFF}, // IO_PORT_DECODE register ports.
    {0x500, 0xFF}, // IO_PORT_DECODE register ports.
    {0xA00, 0xFF}, // WIDE_IO_2, for PnP.
    {0x600, 0xFF}, // WIDE_IO_0, for AWE32/64.
    {0xE00, 0xFF}, // WIDE_IO_1, for AWE32/64.
    {0x700, 0xFF}, // WIDE_IO_0, second half.
    {0xF00, 0xFF}, // WIDE_IO_1, second half.
};

int CheckAMDController()
{
    uint32_t tmp = readpci(REG_AMD_LPC_CTRL_INFO);
    uint32_t ven = tmp & 0x0000FFFF;

    // Check the LPC controller is there
    if (ven != VEN_AMD_LONG)
    {
        printf("Can't find AMD LPC controller.\n");
        printf("(Got %04lX, expected %04X)\n", ven, VEN_AMD);
        return 0; // false
    }
    else
    {
        uint32_t dev = tmp >> 16;
        printf("Found AMD LPC Controller. Dev ID: %04lX\n", dev);
        return 1; // true
    }
}

void ViewAMDLPCStates()
{
    uint32_t pcictrl = readpci(REG_AMD_LPC_PCI_CTRL);
    uint32_t miscctrl = readpci(REG_AMD_LPC_MISC_CTRL);
    uint32_t ioport = readpci(REG_AMD_LPC_IO_PORT_DECODE);
    uint32_t iomem = readpci(REG_AMD_LPC_IOMEM_DECODE);
    uint32_t wide01 = readpci(REG_AMD_LPC_WIDE_IO_0_AND_1);
    uint32_t wide2 = readpci(REG_AMD_LPC_WIDE_IO_2);

    printf("PCI Control: %08lX\n", pcictrl);
    printf("Misc Control: %08lX\n", miscctrl);
    printf("IO Port Decode: %08lX\n", ioport);
    printf("IO/Mem Decode: %08lX\n", iomem);
    printf("Wide IO 0 and 1: %08lX\n", wide01);
    printf("Wide IO 2: %08lX\n", wide2);
}

void EnableLDRQ()
{
    writepci(REG_AMD_LPC_MISC_CTRL, readpci(REG_AMD_LPC_MISC_CTRL) | AMD_ENABLE_LDRQ);
}

void ConfigureLPCIOMapping()
{
    writepci(REG_AMD_LPC_IO_PORT_DECODE, readpci(REG_AMD_LPC_IO_PORT_DECODE) | AMD_ENABLE_AUDIO_IO_MAP);
    writepci(REG_AMD_LPC_IOMEM_DECODE, readpci(REG_AMD_LPC_IOMEM_DECODE) | AMD_ENABLE_ALTCFG_AND_WIDE_IO);
}

void ConfigureLPCWideRanges()
{
    // Range 600-7FF, Range E00-FFF
    writepci(REG_AMD_LPC_WIDE_IO_0_AND_1, 0x0E000600);
    // Range A00-BFF
    writepci(REG_AMD_LPC_WIDE_IO_2, readpci(REG_AMD_LPC_WIDE_IO_2) | 0x00000A00);
}

void ConfigureLPCBridge()
{
    int i;
    for (i = 0; i < NUM_RANGES; i++)
    {
        LPC_SetRange(i, ranges[i]);
    }
}
