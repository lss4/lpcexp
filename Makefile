ifdef WATCOM
export WATCOM := $(WATCOM)
else
export WATCOM := /opt/watcom
endif

export PATH    := $(WATCOM)/binl64:$(WATCOM)/binl:$(PATH)
export INCLUDE := $(WATCOM)/h

VERBOSE  := -dVERBOSE

CL16     := wcl
TARGET16 := lpcisa.exe
FLAGS16  := -5 -bt=dos -fe=$(TARGET16)
VFLAGS16 := $(VERBOSE) $(FLAGS16)

CL32     := wcl386
TARGET32 := lpcisaex.exe
FLAGS32  := -bt=dos -l=dos4g -fe=$(TARGET32)
VFLAGS32 := $(VERBOSE) $(FLAGS32)

FILES := helper.c lpc.c intel.c amd.c main.c

all: build16 build32

debug: build16v build32v

build16:
	$(CL16) $(FLAGS16) $(FILES)

build16v:
	$(CL16) $(VFLAGS16) $(FILES)

build32:
	$(CL32) $(FLAGS32) $(FILES)

build32v:
	$(CL32) $(VFLAGS32) $(FILES)

clean:
	rm $(TARGET16)
	rm $(TARGET32)
