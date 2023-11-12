#include "lpc.h"

#ifdef LPC_CFG_ALT
	#define LPC_ADDR	0x2E
	#define LPC_RW  	0x2F
#else
	#define LPC_ADDR 	0x4E
	#define LPC_RW  	0x4F
#endif

void LPC_Enable()
{
	// Enable ISA bridge config registers
    _outp(LPC_ADDR, 0x26);
    _outp(LPC_ADDR, 0x26);
}

uint8_t LPC_Read(uint8_t addr)
{
	_outp(LPC_ADDR, addr);
	return _inp(LPC_RW);
}

void LPC_Write(uint8_t addr, uint8_t value)
{
	_outp(LPC_ADDR, addr);
	_outp(LPC_RW, value);
}
