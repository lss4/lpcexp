#include "helper.h"

#define PCI_ENABLE_BIT  0x80000000

#ifdef _M_I86
// NOTE: When using 16-bit compiler, _inpd and _outpd are not available.
//       Use .386 directive in inline assembler to enable access to 32-bit
//       registers such as eax.

void _outpd(uint16_t addr, uint32_t value)
{    
    _asm {
        .386
        mov dx, addr
        mov eax, value
        out dx, eax
    };
}

uint32_t _inpd(uint16_t addr)
{
    uint32_t value;

    _asm {
        .386
        mov dx, addr
        in eax, dx
        mov value, eax
    };

    return value;
}
#endif

void listports(uint32_t BASE, uint32_t MASK)
{
    uint32_t cnt;
    uint32_t lastOne = 0;
    uint32_t thisOne = 0;
    //each bit in the mask is ignored in the address
    MASK |= 0x03;
    for (cnt = 0; cnt < 0xFFFF; cnt++)
    {
        if ((cnt | MASK) == (BASE | MASK))
        {
            // starting new run, print first
            if (lastOne == 0){
                printf("%X-", cnt);
                lastOne = cnt;
            }
            else if (cnt == lastOne + 1)
            {
                // don't print consecutive ports
                lastOne = cnt;
            }
            else // we've broken a sequence, print the last one and start a new range
            {
                printf("%X,", lastOne);
                printf("%X-", cnt);
                lastOne = cnt;
            }
        }
    }
    printf("%X", lastOne);
    printf("\n");
}

void dec(uint32_t val)
{
    uint32_t BASE = (val & 0xFFFC);
    uint32_t MASK = (val & 0xFC0000) >> 16;
    printf("BASE : %lX\n", BASE);
    printf("MASK : %lX\n", MASK);

    listports(BASE, MASK);
}

void writepci_addr(uint32_t address, uint32_t val)
{
    _outpd(0xCF8, address);
    _outpd(0xCFC, val);
    _outpd(0xCF8, 0x00000000);
}

uint32_t readpci_addr(uint32_t address)
{
	uint32_t val;
	_outpd(0xCF8, address);
	val = _inpd(0xCFC);
	_outpd(0xCF8, 0x00000000);
    return val;
}

uint8_t read_amd_pm(uint8_t address)
{
    uint8_t val;
	_outp(0xCD6, address);
	val = _inp(0xCD7);
	_outp(0xCD6, 0x00);
    return val;
}

void write_amd_pm(uint8_t address, uint8_t val)
{
    _outp (0xCD6, address);
    _outpd(0xCD7, val);
    _outp (0xCD6, 0x00);
}

uint32_t getaddr(uint32_t bus, uint32_t device, uint32_t func, uint32_t pcireg)
{
    // Limitations.
    uint32_t bus_a = bus & 0xFFFF;
    uint32_t device_a = device & 0x1F;
    uint32_t func_a = func & 0x7;
    uint32_t pcireg_a = pcireg & 0xFF;

    return (PCI_ENABLE_BIT | (bus_a << 16) | (device_a << 11) | (func_a << 8) | (pcireg_a));
}

void writepci(uint32_t bus, uint32_t device, uint32_t func, uint32_t pcireg, uint32_t val)
{
    uint32_t addr = getaddr(bus, device, func, pcireg);
    writepci_addr(addr, val);
}

uint32_t readpci(uint32_t bus, uint32_t device, uint32_t func, uint32_t pcireg)
{
    uint32_t addr = getaddr(bus, device, func, pcireg);
    return readpci_addr(addr);
}

void clearpci()
{
    _outpd(0xCF8, 0x00000000);
}

void dma_reset()
{
    // This resets DMA-related registers.
    // Not required for RUBY-9719VG2AR, but likely needed for others.
    
    _outp(0x00, 0x00);
    _outp(0x01, 0x00);
    _outp(0x04, 0x00);
    _outp(0x05, 0x00);
    _outp(0x06, 0x00);
    _outp(0x07, 0x00);
    _outp(0x08, 0x00);
    // _outp(0x21, 0x00);
    _outp(0x82, 0x00);
    _outp(0x87, 0x00);
    _outp(0x89, 0x00);
    _outp(0x8A, 0x00);
    _outp(0x8B, 0x00);
    _outp(0xC0, 0x00);
    _outp(0xC1, 0x00);
    _outp(0xC2, 0x00);
    _outp(0xC3, 0x00);
    _outp(0xC4, 0x00);
    _outp(0xC5, 0x00);
    _outp(0xC6, 0x00);
    _outp(0xC7, 0x00);
    _outp(0xC8, 0x00);
    _outp(0xC9, 0x00);
    _outp(0xCA, 0x00);
    _outp(0xCB, 0x00);
    _outp(0xCC, 0x00);
    _outp(0xCD, 0x00);
    _outp(0xCE, 0x00);
    _outp(0xCF, 0x00);
    _outp(0xD0, 0x00);
    _outp(0xD1, 0x00);
    _outp(0xDE, 0x0E);
    _outp(0xDF, 0x0E);
}
