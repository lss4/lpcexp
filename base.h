#ifndef BASE_H
#define BASE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <conio.h>
//#include <stdint.h>

#define uint32_t unsigned long
#define uint16_t unsigned short
#define uint8_t unsigned char

typedef struct _range
{
    // base address, only LS 14 bits are used
    uint32_t base;

    // only MS 6 bits are used
    // any digits that are 1 will be ignored in the decode comparison logic
    uint32_t mask;
} range;

#endif
