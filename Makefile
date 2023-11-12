## WATCOM Environment

ifdef WATCOM
export WATCOM := $(WATCOM)
else
export WATCOM := /opt/watcom
endif

export PATH    := $(WATCOM)/binl64:$(WATCOM)/binl:$(PATH)
export INCLUDE := $(WATCOM)/h

## Common build flags

VERBOSE  := -dVERBOSE

ifdef LPC_USE_2E
USE_2E := -dLPC_CFG_ALT
else
USE_2E :=
endif

CL16     := wcl
FLAGS16  := -5 -bt=dos $(USE_2E)
VFLAGS16 := $(VERBOSE) $(FLAGS16)

CL32     := wcl386
FLAGS32  := -bt=dos -l=dos4g $(USE_2E)
VFLAGS32 := $(VERBOSE) $(FLAGS32)

## Individual Files

### LPCEN
LPCEN_FILES := lpcen.c lpc.c
LPCEN_OBJ   := lpcen.o lpc.o
LPCEN_EXE   := lpcen.exe
LPCEN32_EXE := lpcenx.exe

### LPCCMD
LPCCMD_FILES := lpccmd.c lpc.c
LPCCMD_OBJ   := lpccmd.o lpc.o
LPCCMD_EXE   := lpccmd.exe
LPCCMD32_EXE := lpccmdx.exe

### LPCFCHK
LPCFCHK_FILES := lpcfchk.c lpc.c fintek.c
LPCFCHK_OBJ   := lpcfchk.o lpc.o fintek.o
LPCFCHK_EXE   := lpcfchk.exe
LPCFCHK32_EXE := lpcfchkx.exe

### PCICMD
PCICMD_FILES := pcicmd.c helper.c
PCICMD_OBJ   := pcicmd.o helper.o
PCICMD_EXE   := pcicmd.exe
PCICMD32_EXE := pcicmdx.exe

### LPCISA/LPCISAV
LPCISA_FILES  := helper.c intel.c amd.c main.c
LPCISA_OBJ    := helper.o intel.o amd.o main.o
LPCISA_EXE    := lpcisa.exe
LPCISAV_EXE   := lpcisav.exe
LPCISA32_EXE  := lpcisax.exe
LPCISAV32_EXE := lpcisavx.exe

all:   lpcen lpccmd lpcfchk pcicmd lpcisa lpcisav

lpcen:
	$(CL16) $(FLAGS16) -fe=$(LPCEN_EXE) $(LPCEN_FILES)

lpccmd:
	$(CL16) $(FLAGS16) -fe=$(LPCCMD_EXE) $(LPCCMD_FILES)

lpcfchk:
	$(CL16) $(FLAGS16) -fe=$(LPCFCHK_EXE) $(LPCFCHK_FILES)

pcicmd:
	$(CL16) $(FLAGS16) -fe=$(PCICMD_EXE) $(PCICMD_FILES)

lpcisa:
	$(CL16) $(FLAGS16) -fe=$(LPCISA_EXE) $(LPCISA_FILES)

lpcisav:
	$(CL16) $(VFLAGS16) -fe=$(LPCISAV_EXE) $(LPCISA_FILES)

all32: lpcen32 lpccmd32 lpcfchk32 pcicmd32 lpcisa32 lpcisav32

lpcen32:
	$(CL32) $(FLAGS32) -fe=$(LPCEN32_EXE) $(LPCEN_FILES)

lpccmd32:
	$(CL32) $(FLAGS32) -fe=$(LPCCMD32_EXE) $(LPCCMD_FILES)

lpcfchk32:
	$(CL32) $(FLAGS32) -fe=$(LPCFCHK32_EXE) $(LPCFCHK_FILES)

pcicmd32:
	$(CL32) $(FLAGS32) -fe=$(PCICMD32_EXE) $(PCICMD_FILES)

lpcisa32:
	$(CL32) $(FLAGS32) -fe=$(LPCISA32_EXE) $(LPCISA_FILES)

lpcisav32:
	$(CL32) $(VFLAGS32) -fe=$(LPCISAV32_EXE) $(LPCISA_FILES)

.PHONY : clean
clean:
	-rm $(LPCEN_OBJ)
	-rm $(LPCCMD_OBJ)
	-rm $(LPCFCHK_OBJ)
	-rm $(PCICMD_OBJ)
	-rm $(LPCISA_OBJ)
	-rm $(LPCEN_EXE)
	-rm $(LPCEN32_EXE)
	-rm $(LPCCMD_EXE)
	-rm $(LPCCMD32_EXE)
	-rm $(LPCFCHK_EXE)
	-rm $(LPCFCHK32_EXE)
	-rm $(PCICMD_EXE)
	-rm $(PCICMD32_EXE)
	-rm $(LPCISA_EXE)
	-rm $(LPCISAV_EXE)
	-rm $(LPCISA32_EXE)
	-rm $(LPCISAV32_EXE)
