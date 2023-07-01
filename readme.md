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
Requires Open Watcom to build. I'm currently using Open Watcom 2.0.

You need to export the path where you installed Open Watcom into. For example:

```
export WATCOM=/opt/watcom
```

## II. Building using Makefile

Currently the Makefile is meant for building the program on Linux. The Makefile 
will build both 16-bit and 32-bit versions.

Use `make` to build them. If you want to enable verbose output (which is mainly 
about related register states), use `make debug`.

`lpcisa.exe` is the 16-bit version that can be run directly.

`lpcisaex.exe` is the 32-bit version, which requires DOS32A or DOS4GW.

## III. Usage

```
lpcisa [base1 mask1] [base2 mask2] [base3 mask3] [base4 mask4]
```

Note that there may be too many lines of output to be displayed when running 
the program, especially if you enabled verbose output. You may want to consider 
redirecting the output to a file so you can examine it later, for example:

```
lpcisa > output.txt
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