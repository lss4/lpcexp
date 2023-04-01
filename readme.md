# LPC Experiments
This configures the motherboard's LPC controller to forward ports necessary 
for sound cards to function on RUBY-9719VG2AR or a compatible motherboard 
using [dISAppointment](https://github.com/rasteri/dISAppointment).

## I. Preparation
Requires Open Watcom to build. I'm using Open Watcom 2.0.

To prepare the build environment, assuming using Linux and the install 
location `/opt/watcom`.

```
. /opt/watcom/owsetenv.sh
export INCLUDE=$WATCOM/h
```

If you want the program to print the states of some related registers before 
configuring, add `-dVERBOSE`.

Note that there may be too many lines of output to be displayed when running 
in real DOS. You may want to consider redirecting the output to a file so you 
can examine it later. An example with `intisa`:

```
intisa > output.txt
```

## II. Build for Intel
32-bit (requires DOS4G(W)/DOS32A)
```
wcl386 -bt=dos -l=dos4g -fe=intisaex.exe helper.c lpc.c intel.c main_intel.c
```
16-bit
```
wcl -5 -bt=dos -fe=intisa.exe helper.c lpc.c intel.c main_intel.c
```

## III. Build for AMD
32-bit (requires DOS4G(W)/DOS32A)
```
wcl386 -bt=dos -l=dos4g -fe=amdisaex.exe helper.c lpc.c amd.c main_amd.c`
```
16-bit
```
wcl -5 -bt=dos -fe=amdisa.exe helper.c lpc.c amd.c main_amd.c`
```
## IV. References
Some useful reference documents can be found in the docs folder.

You may find additional documents for other chipsets on Intel and AMD's 
official sites.