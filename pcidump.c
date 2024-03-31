#include "helper.h"

int main(int argc, char* argv[])
{
    uint32_t bus;
    uint32_t device;
    uint32_t function;
    uint32_t pcireg;
    uint32_t val;

    if (argc < 4)
    {
        // Less than 3 arguments.
        printf("To dump PCI registers: %s [bus] [device] [function] > [file]\n", argv[0]);
        printf("NOTE: Specify bus, device and function in hexadecimal.\n");
        printf("If > [file] not specified, print register values to stdout.\n");
        return 1;
    }
    else
    {
        // 3 arguments.
        // Read from a PCI location.
        bus = strtoul(argv[1], NULL, 16);
        device = strtoul(argv[2], NULL, 16);
        function = strtoul(argv[3], NULL, 16);
        for (pcireg = 0x0; pcireg < 0x100; pcireg += 4)
        {
            val = readpci(bus, device, function, pcireg);
            printf("%02lX: %08lX\n", pcireg, val);
        }
        return 0;
    }
}
