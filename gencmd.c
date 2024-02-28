#include "helper.h"

int main(int argc, char* argv[])
{
    uint8_t  size;
    uint16_t addr;
    uint32_t val;

    if (argc < 3)
    {
        // Less than two arguments.
        printf("To read port:  %s [size] [addr]\n", argv[0]);
        printf("To write port: %s [size] [addr] [value]\n", argv[0]);
        printf("[size] can be 1-4 bytes (3 is the same as 4).\n");
        printf("If [size] is not valid, assume operating 1 data byte.\n");
        printf("NOTE: Specify all input arguments in hexadecimal.\n");
        return 1;
    }
    else if (argc < 4)
    {
        // Two arguments.
        size = (uint8_t)strtoul(argv[1], NULL, 16);
        addr = (uint16_t)strtoul(argv[2], NULL, 16);

        switch (size)
        {
            case 2: // 2-byte
                val = _inpw(addr);
                printf("%04lX\n", val);
                break;
            case 3:
            case 4: // both 3 and 4 means 4-byte
                val = _inpd(addr);
                printf("%08lX\n", val);
                break;
            // case 1: // 1-byte
            // Assume 1 if invalid.
            default:
                val = _inp(addr);
                printf("%02lX\n", val);
                break;
        }
        return 0;
    }
    else
    {
        // More than two arguments. The first three will be used.
        size = (uint8_t)strtoul(argv[1], NULL, 16);
        addr = (uint16_t)strtoul(argv[2], NULL, 16);
        val =  (uint32_t)strtoul(argv[3], NULL, 16);
        
        switch (size)
        {
            case 2:
                _outpw(addr, (uint16_t)val);
                break;
            case 3:
                _outpd(addr, val);
                break;
            // case 1:
            // Assume 1 if invalid.
            default:
                _outp(addr, (uint8_t)val);
                break;
        }

        return 0;
    }
}
