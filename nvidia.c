#include "helper.h"
#include "nvidia.h"
#include "vendor.h"

#define DEV_NVIDIA_LPC_BUS         0x0
#define DEV_NVIDIA_LPC_DEVICE      0x1
#define DEV_NVIDIA_LPC_FUNC        0x0

#define REG_NVIDIA_LPC_CTRL_INFO         0x00

uint32_t Read_Nvidia_LPC(uint32_t reg)
{
    return readpci(DEV_NVIDIA_LPC_BUS, DEV_NVIDIA_LPC_DEVICE, DEV_NVIDIA_LPC_FUNC, reg);
}

void Write_Nvidia_LPC(uint32_t reg, uint32_t val)
{
    writepci(DEV_NVIDIA_LPC_BUS, DEV_NVIDIA_LPC_DEVICE, DEV_NVIDIA_LPC_FUNC, reg, val);
}

int CheckNvidiaLPCController()
{
    uint32_t tmp = Read_Nvidia_LPC(REG_NVIDIA_LPC_CTRL_INFO);
    uint32_t ven = tmp & 0x0000FFFF;

    // Check the LPC controller is there
    if (ven != VEN_NVIDIA_LONG)
    {
        printf("Can't find nVidia LPC controller.\n");
        printf("(Got %04lX, expected %04X)\n", ven, VEN_NVIDIA);
        return 0; // false
    }
    else
    {
        uint32_t dev = tmp >> 16;
        printf("Found nVidia LPC Controller. Dev ID: %04lX\n", dev);
        return 1; // true
    }
}

int main_nvidia(int argc, char* argv[])
{
    return 0;
}