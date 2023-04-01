#ifndef LPC_H
#define LPC_H

#include "helper.h"

void LPC_Enable();
uint8_t LPC_Read(uint8_t addr);
void LPC_Write(uint8_t addr, uint8_t value);
void LPC_PrintRange(uint32_t index);
void LPC_SetRange(uint32_t index, range range);
int LPC_Check();
void LPC_View_Current_State();
void LPC_View_Range_Tables();
void LPC_Misc_Config();
void LPC_Clear_DMA();

#endif
