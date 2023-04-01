#ifndef AMD_H
#define AMD_H

#include "helper.h"

int CheckAMDController();
void ViewAMDLPCStates();
void EnableLDRQ();
void ConfigureLPCIOMapping();
void ConfigureLPCWideRanges();
void ConfigureLPCBridge();

#endif
