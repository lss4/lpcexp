# LPC Experiments
This configures the motherboard's LPC controller to forward ports necessary 
for sound cards to function on RUBY-9719VG2AR or a compatible motherboard 
using [dISAppointment](https://github.com/rasteri/dISAppointment).

Currently supports Intel and AMD chipsets. However, be noted that the LDRQ# 
signal needed for LPC DMA has been removed since Intel 100 series (Skylake) 
and AMD Socket AM5/SP5 (Raphael/Genoa). While this program can still be used 
on those and newer systems as usual, DMA-related functions, namely digitized 
audio in DOS games, will not work. FM Synth, on the other hand, will still 
be able to work as it does not rely on DMA to function.

## I. Preparation
Requires Open Watcom to build. I'm using Open Watcom 2.0.

To prepare the build environment, assuming using Linux and the install 
location `/opt/watcom`.

```
. /opt/watcom/owsetenv.sh
export INCLUDE=$WATCOM/h
```

If you want the program to print the detailed states of some related registers 
before and after configuring, add `-dVERBOSE` when building.

Note that there may be too many lines of output to be displayed when running 
in real DOS. You may want to consider redirecting the output to a file so you 
can examine it later:

```
lpcisa > output.txt
```

## II. Building
32-bit (requires DOS4G(W)/DOS32A)
```
wcl386 -bt=dos -l=dos4g -fe=lpcisaex.exe helper.c lpc.c intel.c amd.c main.c
```
16-bit
```
wcl -5 -bt=dos -fe=lpcisa.exe helper.c lpc.c intel.c amd.c main.c
```

## III. Usage

```
lpcisa [base1 mask1] [base2 mask2] [base3 mask3] [base4 mask4]
```

The program will try to determine the system's chipset vendor by querying the 
root complex, and perform configurations accordingly.

For Intel chipsets, up to 4 sets of base addresses and masks can be configured.

For AMD chipsets, up to 3 sets of base addresses and masks can be configured.

Note that for AMD chipsets, masks behave differently. If set to 0, the range 
will be configured in alternative mode which limits to 16 bytes. Otherwise, 
it will be configured in normal mode which covers up to 512 bytes.

## IV. References
Some useful reference documents can be found in the docs folder.

You may find additional documents for other chipsets on Intel and AMD's 
official sites.