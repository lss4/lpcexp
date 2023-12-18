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

USE_2E := -dLPC_CFG_ALT

CL16     := wcl
FLAGS16  := -5 -bt=dos $(USE_2E)
AFLAGS16 := $(USE_2E) $(FLAGS16)
VFLAGS16 := $(VERBOSE) $(FLAGS16)

CL32     := wcl386
FLAGS32  := -bt=dos -l=dos4g $(USE_2E)
AFLAGS32 := $(USE_2E) $(FLAGS32)
VFLAGS32 := $(VERBOSE) $(FLAGS32)

## Individual Files

### LPCEN
LPCEN_FILES := lpcen.c lpc.c
LPCEN_OBJ   := lpcen.o lpc.o
LPCEN_EXE   := lpcen.exe
LPCEN32_EXE := lpcenx.exe
LPCEN2_EXE  := lpcen2.exe
LPCEN232_EXE := lpcen2x.exe

### LPCCMD
LPCCMD_FILES := lpccmd.c lpc.c
LPCCMD_OBJ   := lpccmd.o lpc.o
LPCCMD_EXE   := lpccmd.exe
LPCCMD32_EXE := lpccmdx.exe
LPCCMD2_EXE  := lpccmd2.exe
LPCCMD232_EXE := lpccmd2x.exe

### LPCFCHK
LPCFCHK_FILES := lpcfchk.c lpc.c fintek.c
LPCFCHK_OBJ   := lpcfchk.o lpc.o fintek.o
LPCFCHK_EXE   := lpcfchk.exe
LPCFCHK32_EXE := lpcfchkx.exe
LPCFCHK2_EXE  := lpcfchk2.exe
LPCFCHK232_EXE := lpcfck2x.exe

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

all:   lpcen lpccmd lpcfchk lpcen2 lpccmd2 lpcfchk2 pcicmd lpcisa lpcisav

lpcen:
	$(CL16) $(FLAGS16) -fe=$(LPCEN_EXE) $(LPCEN_FILES)

lpccmd:
	$(CL16) $(FLAGS16) -fe=$(LPCCMD_EXE) $(LPCCMD_FILES)

lpcfchk:
	$(CL16) $(FLAGS16) -fe=$(LPCFCHK_EXE) $(LPCFCHK_FILES)

lpcen2:
	$(CL16) $(AFLAGS16) -fe=$(LPCEN2_EXE) $(LPCEN_FILES)

lpccmd2:
	$(CL16) $(AFLAGS16) -fe=$(LPCCMD2_EXE) $(LPCCMD_FILES)

lpcfchk2:
	$(CL16) $(AFLAGS16) -fe=$(LPCFCHK2_EXE) $(LPCFCHK_FILES)

pcicmd:
	$(CL16) $(FLAGS16) -fe=$(PCICMD_EXE) $(PCICMD_FILES)

lpcisa:
	$(CL16) $(FLAGS16) -fe=$(LPCISA_EXE) $(LPCISA_FILES)

lpcisav:
	$(CL16) $(VFLAGS16) -fe=$(LPCISAV_EXE) $(LPCISA_FILES)

all32: lpcen32 lpccmd32 lpcfchk32 lpcen232 lpccmd232 lpcfchk232 pcicmd32 lpcisa32 lpcisav32

lpcen32:
	$(CL32) $(FLAGS32) -fe=$(LPCEN32_EXE) $(LPCEN_FILES)

lpccmd32:
	$(CL32) $(FLAGS32) -fe=$(LPCCMD32_EXE) $(LPCCMD_FILES)

lpcfchk32:
	$(CL32) $(FLAGS32) -fe=$(LPCFCHK32_EXE) $(LPCFCHK_FILES)

lpcen232:
	$(CL32) $(AFLAGS32) -fe=$(LPCEN232_EXE) $(LPCEN_FILES)

lpccmd232:
	$(CL32) $(AFLAGS32) -fe=$(LPCCMD232_EXE) $(LPCCMD_FILES)

lpcfchk232:
	$(CL32) $(AFLAGS32) -fe=$(LPCFCHK232_EXE) $(LPCFCHK_FILES)

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
	-rm $(LPCEN2_EXE)
	-rm $(LPCEN232_EXE)
	-rm $(LPCCMD_EXE)
	-rm $(LPCCMD32_EXE)
	-rm $(LPCCMD2_EXE)
	-rm $(LPCCMD232_EXE)
	-rm $(LPCFCHK_EXE)
	-rm $(LPCFCHK32_EXE)
	-rm $(LPCFCHK2_EXE)
	-rm $(LPCFCHK232_EXE)
	-rm $(PCICMD_EXE)
	-rm $(PCICMD32_EXE)
	-rm $(LPCISA_EXE)
	-rm $(LPCISAV_EXE)
	-rm $(LPCISA32_EXE)
	-rm $(LPCISAV32_EXE)
