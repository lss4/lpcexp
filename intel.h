#ifndef INTEL_H
#define INTEL_H

#include "helper.h"

int CheckIntelController();
void Enable_LPC_Config_Forward();
void Configure_LPC_Ranges();
void ProcessArgs(int argc, char* argv[]);
void PrintIntelLPCStates();

#endif
