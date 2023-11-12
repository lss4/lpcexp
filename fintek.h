#ifndef FINTEK_H
#define FINTEK_H

#include "base.h"

void Fintek_PrintRange(uint32_t index);
void Fintek_SetRange(uint32_t index, range range);
int Fintek_Check();
void Fintek_View_Current_State();
void Fintek_View_Range_Tables();
void Fintek_Misc_Config();
void Fintek_Clear_DMA();

#endif
