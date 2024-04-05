#include "lpc.h"

#define DEV_FINTEK_F85226 			0x03051934
#define NUM_GENERAL_DECODE_RANGES 	8
#define NUM_ROM_DECODE_RANGES		2
#define NUM_STATUS_REGISTERS		26

// ADDR1-ADDR4: 0x20 0x23, 0x30, 0x33
// KBC, MC, RTC, IOH: 0x36, 0x39, 0x3C, 0x3F
uint8_t ISAGenericDecode[NUM_GENERAL_DECODE_RANGES] = 
	{0x20, 0x23, 0x30, 0x33, 0x36, 0x39, 0x3C, 0x3F};
char* ISAGeneralDecodeNames[NUM_GENERAL_DECODE_RANGES] = 
	{"ADDR1", "ADDR2", "ADDR3", "ADDR4", "KBC", "MC", "RTC", "IOH"};

// ROM decode ranges.
uint8_t ROMDecode[NUM_ROM_DECODE_RANGES] = { 0x28, 0x2C };
char* ROMDecodeNames[NUM_ROM_DECODE_RANGES] = { "ROM1", "ROM2" };

// Other registers.
uint8_t LPCRegisterList[NUM_STATUS_REGISTERS] = 
	{0x03, 0x04, 0x05, 0x06, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 
	 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	 0x50, 0x51, 0x52, 0x53, 0x54, 0x55};

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

void Fintek_PrintROM(uint32_t index)
{
	uint8_t base;
	uint16_t mask;
	uint16_t addr;

	if (index >= NUM_ROM_DECODE_RANGES) return;

	base = ROMDecode[index];
	mask = 0;
	addr = 0;
	mask |= LPC_Read(base+1);
	mask <<= 8;
	mask |= LPC_Read(base);
	addr |= LPC_Read(base+3);
	addr <<= 8;
	addr |= LPC_Read(base+2);

	printf("%s range: %X, mask: %X\n", ROMDecodeNames[index], addr, mask);
}

void Fintek_PrintSBHE()
{
	// NOTE: SBHE (F1h) is specific to F85226AF.
	printf("SBHE: %02X\n", LPC_Read(0xF1));
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
        printf("Can't find Fintek F85226 LPC-ISA Bridge\n");
        printf("(Got %08lX, expected %08lX)\n", tmp, DEV_FINTEK_F85226);
        return 0;
    }
	else
	{
		printf("Found Fintek F85226 LPC-ISA Bridge. Version: %X\n", ver);
		return 1;
	}
}

void Fintek_View_Current_State()
{
	int i = 0;

	printf("F85226 State Registers\n");

	for (i = 0; i < NUM_STATUS_REGISTERS; i++)
	{
		printf("%02X ", LPCRegisterList[i]);
	}

	printf("\n");

	for (i = 0; i < NUM_STATUS_REGISTERS; i++)
	{
		printf("%02X ", LPC_Read(LPCRegisterList[i]));
	}

	printf("\n");

	Fintek_PrintSBHE();
}

void Fintek_View_Range_Tables()
{
	uint32_t i;

	for (i = 0; i < NUM_GENERAL_DECODE_RANGES; i++)
	{
		Fintek_PrintRange(i);
	}

	for (i = 0; i < NUM_ROM_DECODE_RANGES; i++)
	{
		Fintek_PrintROM(i);
	}
}
