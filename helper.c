#include "helper.h"

#ifdef _M_I86
// NOTE: When using 16-bit compiler, _inpd and _outpd are not available.
//       Opcodes involving EAX must be emitted by hand when using 16-bit compiler.
//       32-bit compiler natively supports _inpd and _outpd so they are not needed there.

void _outpd(uint16_t addr, uint32_t value)
{
    uint16_t high = (uint16_t)(value >> 16);
    uint16_t low = (uint16_t)(value & 0xFFFF);
    
    // 66C1C810 = ROR EAX, 0x10
    // 66EF = OUT DX, EAX
    _asm {
        mov dx, addr
        mov ax, high
        db 0x66
        db 0xC1
        db 0xC8
        db 0x10
        mov ax, low
        db 0x66
        db 0xEF
    };

}

uint32_t _inpd(uint16_t addr)
{
    uint32_t val;
    uint16_t low;
    uint16_t high;

    // 66ED = IN EAX, DX
    // 66C1C810 = ROR EAX, 0x10
    _asm {
        mov dx, addr
        db 0x66
        db 0xED
        mov low, ax
        db 0x66
        db 0xC1
        db 0xC8
        db 0x10
        mov high, ax
    };

    val = high;
    val <<= 16;
    val |= low;

    return val;
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

uint32_t LPCEnc(uint32_t BASE, uint32_t MASK)
{
    return BASE | (MASK << 16) | 0x01;
}

void writepci(uint32_t address, uint32_t val)
{
    _outpd(0xCF8, address);
    _outpd(0xCFC, val);
    _outpd(0xCF8, 0x00000000);
}

uint32_t readpci(uint32_t address)
{
	uint32_t val;
	_outpd(0xCF8, address);
	val = _inpd(0xCFC);
	_outpd(0xCF8, 0x00000000);
    return val;
}

void clearpci()
{
    _outpd(0xCF8, 0x00000000);
}
