#include "helper.h"

int main(int argc, char* argv[])
{
    uint8_t addr;
    uint8_t val;

    if (argc < 2)
    {
        // No arguments.
        printf("To read AMD PM:  %s [addr]\n", argv[0]);
        printf("To write AMD PM: %s [addr] [value]\n", argv[0]);
        printf("NOTE: Specify all input arguments in hexadecimal.\n");
        return 1;
    }
    else if (argc < 3)
    {
        // One argument.
        // Read from AMD PM.
        addr = (uint8_t)strtoul(argv[1], NULL, 16);
        val = read_amd_pm(addr);
        printf("%02X\n", val);
        return 0;
    }
    else
    {
        // More than one argument. The first two will be used.
        // Write to AMD PM.
        addr = (uint8_t)strtoul(argv[1], NULL, 16);
        val =  (uint8_t)strtoul(argv[2], NULL, 16);
        write_amd_pm(addr, val);
        return 0;
    }
}
