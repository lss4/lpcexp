#include "helper.h"
#include "amd.h"
#include "vendor.h"

#define DEV_AMD_LPC_BUS         0x0
#define DEV_AMD_LPC_DEVICE      0x14
#define DEV_AMD_LPC_FUNC        0x3

#define REG_AMD_LPC_CTRL_INFO           0x00
#define REG_AMD_LPC_PCI_CTRL            0x40 // PCI Control
#define REG_AMD_LPC_IO_PORT_DECODE      0x44 // For common ports
#define REG_AMD_LPC_IOMEM_DECODE        0x48 // For some other ranges as well as enabling Wide I/O.
#define REG_AMD_LPC_WIDE_IO_0_AND_1     0x64
#define REG_AMD_LPC_ALT_WIDEIO          0x74
#define REG_AMD_LPC_MISC_CTRL           0x78
#define REG_AMD_LPC_TPM                 0x7C
#define REG_AMD_LPC_WIDE_IO_2           0x90
#define REG_AMD_LPC_CLKCNTRL            0xD0
#define REG_AMD_LPC_CLKRUNOPTION        0xD4

#define AMD_ENABLE_LDRQ                 0x0000000C
#define AMD_ENABLE_AUDIO_IO_MAP         0x93FFD700
#define AMD_ENABLE_ALTCFG_AND_WIDE_IO   0x03000006
#define AMD_ENABLE_ALTCFG               0x00000002

#define AMD_LPCCLK1_GPIO                0x00000800

// Forwarding ranges
// Default: Range 600-7FF, E00-FFF, A00-BFF
// Mask Behavior: If mask is set to 0, enable the respective alt bit to limit the range to 16 bytes.
//                Otherwise, disable the respective alt bit so all 512 bytes are accessible.

int num_amd_ranges = 0;
uint32_t alt_wideio_enable[] = { 0x00000001, 0x00000004, 0x00000008 };
uint32_t wideio_enable[] = {0x00000004, 0x01000000, 0x02000000 };

range amd_ranges[] = {
    {0x600, 0xFF}, // WIDE_IO_0, for AWE32/64.
    {0xE00, 0xFF}, // WIDE_IO_1, for AWE32/64.
    {0xA00, 0xFF}, // WIDE_IO_2, for PnP.    
};

uint32_t Read_AMD_LPC(uint32_t reg)
{
    return readpci(DEV_AMD_LPC_BUS, DEV_AMD_LPC_DEVICE, DEV_AMD_LPC_FUNC, reg);
}

void Write_AMD_LPC(uint32_t reg, uint32_t val)
{
    writepci(DEV_AMD_LPC_BUS, DEV_AMD_LPC_DEVICE, DEV_AMD_LPC_FUNC, reg, val);
}

uint32_t Get_WideIO_Enable_Bits()
{
    int i;
    uint32_t tmp = 0;
    
    for (i = 0; i < num_amd_ranges; i++)
    {
        tmp |= wideio_enable[i];
    }
    
    return tmp;
}

uint32_t Get_Alt_WideIO_Range_Bits()
{
    int i;
    uint32_t tmp = 0;
    
    for (i = 0; i < num_amd_ranges; i++)
    {
        if (!amd_ranges[i].mask) // Mask is set to 0.
        {
            // Enable the bit to limit the range to 16 bytes.
            tmp |= alt_wideio_enable[i];
        }
    }

    return tmp;
}

int CheckAMDLPCController()
{
    uint32_t tmp = Read_AMD_LPC(REG_AMD_LPC_CTRL_INFO);
    uint32_t ven = tmp & 0x0000FFFF;
    uint32_t dev;

    // Check the LPC controller is there

    switch (ven)
    {
        case VEN_AMD_LONG:
            {
                dev = tmp >> 16;
                printf("Found AMD LPC Controller. Dev ID: %04lX\n", dev);
            }
            return 1;
        case VEN_AMD_ATI_LONG:
            {
                dev = tmp >> 16;
                printf("Found AMD/ATI LPC Controller. Dev ID: %04lX\n", dev);
            }
            return 1;
        default:
            {
                printf("Can't find AMD/ATI LPC controller.\n");
                printf("(Got %04lX, expected %04X or %04X)\n", ven, VEN_AMD, VEN_AMD_ATI);
            }
            return 0;
    }
}

void ViewAMDLPCStates()
{
    uint32_t pcictrl = Read_AMD_LPC(REG_AMD_LPC_PCI_CTRL);
    uint32_t miscctrl = Read_AMD_LPC(REG_AMD_LPC_MISC_CTRL);
    uint32_t ioport = Read_AMD_LPC(REG_AMD_LPC_IO_PORT_DECODE);
    uint32_t iomem = Read_AMD_LPC(REG_AMD_LPC_IOMEM_DECODE);
    uint32_t wide01 = Read_AMD_LPC(REG_AMD_LPC_WIDE_IO_0_AND_1);
    uint32_t wide2 = Read_AMD_LPC(REG_AMD_LPC_WIDE_IO_2);
    uint32_t alt_wideio = Read_AMD_LPC(REG_AMD_LPC_ALT_WIDEIO);

    uint32_t tpm = Read_AMD_LPC(REG_AMD_LPC_TPM);
    uint32_t clk_ctrl = Read_AMD_LPC(REG_AMD_LPC_CLKCNTRL);
    uint32_t clkrun_opt = Read_AMD_LPC(REG_AMD_LPC_CLKRUNOPTION);

    printf("PCI Control: %08lX\n", pcictrl);
    printf("Misc Control: %08lX\n", miscctrl);
    printf("IO Port Decode: %08lX\n", ioport);
    printf("IO/Mem Decode: %08lX\n", iomem);
    printf("Wide IO 0 and 1: %08lX\n", wide01);
    printf("Wide IO 2: %08lX\n", wide2);
    printf("Alt Wide IO: %08lX\n", alt_wideio);

    printf("TPM: %08lX\n", tpm);
    printf("LPCCLK Control: %08lX\n", clk_ctrl);
    printf("CLKRUN Option: %08lX\n", clkrun_opt);
}

void EnableLDRQ()
{
    Write_AMD_LPC(REG_AMD_LPC_MISC_CTRL, Read_AMD_LPC(REG_AMD_LPC_MISC_CTRL) | AMD_ENABLE_LDRQ);
}

void DisableLPCCLK1GPIO()
{
    Write_AMD_LPC(REG_AMD_LPC_TPM, Read_AMD_LPC(REG_AMD_LPC_TPM) & ~AMD_LPCCLK1_GPIO);
}

void ConfigureLPCIOMapping()
{
    uint32_t tmp = Read_AMD_LPC(REG_AMD_LPC_IOMEM_DECODE) & ~AMD_ENABLE_ALTCFG_AND_WIDE_IO;

    Write_AMD_LPC(REG_AMD_LPC_IO_PORT_DECODE, Read_AMD_LPC(REG_AMD_LPC_IO_PORT_DECODE) | AMD_ENABLE_AUDIO_IO_MAP);

    tmp |= AMD_ENABLE_ALTCFG;
    tmp |= Get_WideIO_Enable_Bits();

    Write_AMD_LPC(REG_AMD_LPC_IOMEM_DECODE, tmp);
}

void ConfigureLPCWideRanges()
{
    uint32_t iotmp = 0;

    switch(num_amd_ranges)
    {
        case 3:
            printf("Enabling Wide IO Range 2: %04lX %s\n", amd_ranges[2].base,
                amd_ranges[2].mask ? "(Normal)" : "(Alt)");
            Write_AMD_LPC(REG_AMD_LPC_WIDE_IO_2, amd_ranges[2].base);
            // fall-through
        case 2:
            printf("Enabling Wide IO Range 1: %04lX %s\n", amd_ranges[1].base,
                amd_ranges[1].mask ? "(Normal)" : "(Alt)");
            iotmp = amd_ranges[1].base;
            iotmp <<= 16;
            // fall-through
        case 1:
            printf("Enabling Wide IO Range 0: %04lX %s\n", amd_ranges[0].base,
                amd_ranges[0].mask ? "(Normal)" : "(Alt)");
            iotmp |= amd_ranges[0].base;
            Write_AMD_LPC(REG_AMD_LPC_WIDE_IO_0_AND_1, iotmp);
            break;
        default:
            break;
    }

    Write_AMD_LPC(REG_AMD_LPC_ALT_WIDEIO, Get_Alt_WideIO_Range_Bits());
}

void ProcessAMDArgs(int argc, char* argv[])
{
    // Usage : lpcisa [base1 mask1] [base2 mask2] [base3 mask3]

    // By default it forwards the following ports : 
    // 600-7FF -- AWE32/64
    // E00-FFF -- AWE32/64
    // A00-AFF -- AWE32/64, PnP
    // SB, Adlib, MIDI, WSS ranges are already covered by 
    // register 44h so no need to be configured here.

    // If mask set to 0, the range will be limited to 16 bytes.
    // Otherwise, the range covers up to 512 bytes.

    int i;

    if (argc < 3 || !(argc & 0x01))
    {
        printf("No or malformed args, using defaults\n");
        num_amd_ranges = 3;
    }
    else
    {
        num_amd_ranges = (argc - 1) / 2;
        if (num_amd_ranges > 3) num_amd_ranges = 3;
        
        for (i = 0; i < num_amd_ranges; i++)
        {
            amd_ranges[i].base = strtoul(argv[(i * 2) + 1], NULL, 16);
            amd_ranges[i].mask = strtoul(argv[(i * 2) + 2], NULL, 16);
        }
    }
}

int main_amd(int argc, char* argv[])
{
    // Check AMD.
    if (!CheckAMDLPCController())
    {
        clearpci();
        return 1;
    }

    #ifdef VERBOSE
    printf("Current LPC Controller status before processing:\n");
    ViewAMDLPCStates();
    #endif

    ProcessAMDArgs(argc, argv);

    EnableLDRQ();
    DisableLPCCLK1GPIO();
    ConfigureLPCIOMapping();
    ConfigureLPCWideRanges();

    #ifdef VERBOSE
    printf("Current LPC Controller status after processing:\n");
    ViewAMDLPCStates();
    #endif

    return 0;
}
