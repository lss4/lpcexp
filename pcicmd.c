#include "helper.h"

int main(int argc, char* argv[])
{
    uint32_t bus;
    uint32_t device;
    uint32_t function;
    uint32_t pcireg;
    uint32_t val;

    if (argc < 5)
    {
        // Less than 4 arguments.
        printf("To read PCI:  %s [bus] [device] [function] [reg]\n", argv[0]);
        printf("To write PCI: %s [bus] [device] [function] [reg] [value]\n", argv[0]);
        printf("NOTE: Specify all input arguments in hexadecimal.\n");
        return 1;
    }
    else if (argc < 6)
    {
        // 4 arguments.
        // Read from a PCI location.
        bus = strtoul(argv[1], NULL, 16);
        device = strtoul(argv[2], NULL, 16);
        function = strtoul(argv[3], NULL, 16);
        pcireg = strtoul(argv[4], NULL, 16);
        val = readpci(bus, device, function, pcireg);
        printf("%08lX\n", val);
        return 0;
    }
    else
    {
        // More than 4 arguments. The first 5 will be used.
        // Write to a PCI location.
        bus = strtoul(argv[1], NULL, 16);
        device = strtoul(argv[2], NULL, 16);
        function = strtoul(argv[3], NULL, 16);
        pcireg = strtoul(argv[4], NULL, 16);
        val =  strtoul(argv[5], NULL, 16);
        writepci(bus, device, function, pcireg, val);
        return 0;
    }
}
