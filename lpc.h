#ifndef LPC_H
#define LPC_H

#include "base.h"

void LPC_Enable();
uint8_t LPC_Read(uint8_t addr);
void LPC_Write(uint8_t addr, uint8_t value);

#endif
