#ifndef HELPER_H
#define HELPER_H

#include "base.h"

void listports(uint32_t BASE, uint32_t MASK);
void dec(uint32_t val);
void writepci(uint32_t bus, uint32_t device, uint32_t func, uint32_t pcireg, uint32_t val);
uint32_t readpci(uint32_t bus, uint32_t device, uint32_t func, uint32_t pcireg);
void clearpci();

#endif
