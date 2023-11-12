#include "lpc.h"

int main(int argc, char* argv[])
{
    uint8_t addr;
    uint8_t val;

    if (argc < 2)
    {
        // No arguments.
        printf("To read LPC:  %s [addr]\n", argv[0]);
        printf("To write LPC: %s [addr] [value]\n", argv[0]);
        printf("NOTE: Specify all input arguments in hexadecimal.\n");
        return 1;
    }
    else if (argc < 3)
    {
        // One argument.
        // Read from LPC.
        addr = (uint8_t)strtoul(argv[1], NULL, 16);
        val = LPC_Read(addr);
        printf("%02X\n", val);
        return 0;
    }
    else
    {
        // More than one argument. The first two will be used.
        // Write to LPC.
        addr = (uint8_t)strtoul(argv[1], NULL, 16);
        val =  (uint8_t)strtoul(argv[2], NULL, 16);
        LPC_Write(addr, val);
        return 0;
    }
}
