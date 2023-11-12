#include "lpc.h"

#define DEV_FINTEK_F85226 			0x03051934
#define NUM_GENERAL_DECODE_RANGES 	8

// ADDR1-ADDR4: 0x20 0x23, 0x30, 0x33
// KBC, MC, RTC, IOH: 0x36, 0x39, 0x3C, 0x3F
uint8_t ISAGenericDecode[NUM_GENERAL_DECODE_RANGES] = {0x20, 0x23, 0x30, 0x33, 0x36, 0x39, 0x3C, 0x3F};
char* ISAGeneralDecodeNames[NUM_GENERAL_DECODE_RANGES] = {"ADDR1", "ADDR2", "ADDR3", "ADDR4", "KBC", "MC", "RTC", "IOH"};

void Fintek_PrintRange(uint32_t index)
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

void Fintek_SetRange(uint32_t index, range range)
{
	uint8_t base;
	if (index >= NUM_GENERAL_DECODE_RANGES) return;

	base = ISAGenericDecode[index];
	LPC_Write(base+1, range.base & 0xFF);
	LPC_Write(base+2, range.base >> 8 & 0xFF);
	LPC_Write(base, range.mask | 0x03);
}

int Fintek_Check()
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

void Fintek_View_Current_State()
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

void Fintek_View_Range_Tables()
{
	uint32_t i;

	for (i = 0; i < NUM_GENERAL_DECODE_RANGES; i++)
	{
		Fintek_PrintRange(i);
	}
}
