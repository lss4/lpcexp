#include "lpc.h"

#define LPC_ADDR 	0x4E
#define LPC_RW   	0x4F

#define DEV_FINTEK_F85226 			0x03051934
#define NUM_GENERAL_DECODE_RANGES 	8

// ADDR1-ADDR4: 0x20 0x23, 0x30, 0x33
// KBC, MC, RTC, IOH: 0x36, 0x39, 0x3C, 0x3F
uint8_t ISAGenericDecode[NUM_GENERAL_DECODE_RANGES] = {0x20, 0x23, 0x30, 0x33, 0x36, 0x39, 0x3C, 0x3F};
char* ISAGeneralDecodeNames[NUM_GENERAL_DECODE_RANGES] = {"ADDR1", "ADDR2", "ADDR3", "ADDR4", "KBC", "MC", "RTC", "IOH"};

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

void LPC_PrintRange(uint32_t index)
{
	uint8_t base;
	uint8_t mask;
	uint16_t addr;

	if (index >= NUM_GENERAL_DECODE_RANGES) return;

	base = ISAGenericDecode[index];
	addr = 0;
	mask = LPC_Read(base);
	addr |= LPC_Read(base+2);
	addr <<= 8;
	addr |= LPC_Read(base+1);
	
	printf("%s range: %X, mask: %X\n", ISAGeneralDecodeNames[index], addr, mask);
}

void LPC_SetRange(uint32_t index, range range)
{
	uint8_t base;
	if (index >= NUM_GENERAL_DECODE_RANGES) return;

	base = ISAGenericDecode[index];
	LPC_Write(base+1, range.base & 0xFF);
	LPC_Write(base+2, range.base >> 8 & 0xFF);
	LPC_Write(base, range.mask | 0x03);
}

int LPC_Check()
{
	uint32_t tmp;
	uint32_t orn;
	uint8_t ver;

	// check the ISA bridge is there
    tmp = 0;
	orn = LPC_Read(0x5A);
	orn <<= 24;
	tmp |= orn;
	orn = LPC_Read(0x5B);
	orn <<= 16;
	tmp |= orn;
	ver = LPC_Read(0x5C);
	orn = LPC_Read(0x5D);
	orn <<= 8;
	tmp |= orn;
	tmp |= LPC_Read(0x5E);

    if (tmp != DEV_FINTEK_F85226)
    {
        printf("Can't find Fintek F85226FG LPC-ISA Bridge\n");
        printf("(Got %08lX, expected %08lX)\n", tmp, DEV_FINTEK_F85226);
        return 0;
    }
	else
	{
		printf("Found Fintek F85226FG LPC-ISA Bridge. Version: %X\n", ver);
		return 1;
	}
}

void LPC_View_Current_State()
{
	uint8_t reg05h; 
	uint8_t reg06h; 
	uint8_t reg50h;
	uint8_t reg51h;
	uint8_t reg52h;

	reg05h = LPC_Read(0x05);
	reg06h = LPC_Read(0x06);
	reg50h = LPC_Read(0x50);
	reg51h = LPC_Read(0x51);
	reg52h = LPC_Read(0x52);

	printf("LPC-ISA State Registers (05H,06H,50H,51H,52H)\n");
	printf("%02X %02X %02X %02X %02X\n", reg05h, reg06h, reg50h, reg51h, reg52h);
}

void LPC_View_Range_Tables()
{
	uint32_t i;

	for (i = 0; i < NUM_GENERAL_DECODE_RANGES; i++)
	{
		LPC_PrintRange(i);
	}
}

void LPC_Misc_Config()
{
    // Enable A17/A18/A19
	LPC_Write(0x05, 0x0E);

    // 8MHz clock, 3 8bit waitstates, 1 16bit waitstate
	LPC_Write(0x06, 0x5D);

    // Output sysclk
	LPC_Write(0x50, 0x00);

    // Disable power management (PWRDN and all IRQs)
	LPC_Write(0x51, 0x00);
	LPC_Write(0x52, 0x00);
}

void LPC_Clear_DMA()
{
    // Reset some DMA registers because the BIOS doesn't do this
	// Not sure how much of this is actually required but hey
	_outp(0x00, 0x00);
	_outp(0x01, 0x00);
	_outp(0x04, 0x00);
	_outp(0x05, 0x00);
	_outp(0x06, 0x00);
	_outp(0x07, 0x00);
	_outp(0x08, 0x00);
	_outp(0x21, 0x00);
	_outp(0x82, 0x00);
	_outp(0x87, 0x00);
	_outp(0x89, 0x00);
	_outp(0x8a, 0x00);
	_outp(0x8b, 0x00);
	_outp(0xc0, 0x00);
	_outp(0xc1, 0x00);
	_outp(0xc2, 0x00);
	_outp(0xc3, 0x00);
	_outp(0xc4, 0x00);
	_outp(0xc5, 0x00);
	_outp(0xc6, 0x00);
	_outp(0xc7, 0x00);
	_outp(0xc8, 0x00);
	_outp(0xc9, 0x00);
	_outp(0xca, 0x00);
	_outp(0xcb, 0x00);
	_outp(0xcc, 0x00);
	_outp(0xcd, 0x00);
	_outp(0xce, 0x00);
	_outp(0xcf, 0x00);
	_outp(0xd0, 0x00);
	_outp(0xd1, 0x00);
	_outp(0xde, 0x0e);
	_outp(0xdf, 0x0e);
}
