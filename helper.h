#ifndef HELPER_H
#define HELPER_H

#include "base.h"

#ifdef _M_I86
void _outpd(uint16_t addr, uint32_t value);
uint32_t _inpd(uint16_t addr);
#endif

void listports(uint32_t BASE, uint32_t MASK);
void dec(uint32_t val);
void writepci(uint32_t bus, uint32_t device, uint32_t func, uint32_t pcireg, uint32_t val);
uint32_t readpci(uint32_t bus, uint32_t device, uint32_t func, uint32_t pcireg);
void clearpci();
void dma_reset();

uint8_t read_amd_pm(uint8_t address);
void write_amd_pm(uint8_t address, uint8_t val);

#endif
