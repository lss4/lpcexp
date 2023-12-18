# LPC Experiments
This configures the motherboard's LPC controller to forward ports necessary 
for sound cards to function on RUBY-9719VG2AR or a compatible motherboard 
using [dISAppointment](https://github.com/rasteri/dISAppointment).

Currently implemented for Intel and AMD chipsets. However, be noted that the 
LDRQ# signal needed for LPC DMA has been removed since Intel 100 series 
(Skylake) and AMD Socket AM5/SP5 (Raphael/Genoa). While this program can still 
be used on those and newer systems as usual, DMA-related functions, namely 
digitized audio in DOS games, will not work. FM Synth, on the other hand, will 
still be able to work as it does not rely on DMA to function.

Currently only the Fintek F85226 LPC-ISA bridge is supported. It may be 
possible to support Winbond W83626, but the documentation on the 
configuration registers is a bit confusing, as it's currently uncertain 
how `CRxx` corresponds to the actual register address.

**NOTE:** AMD chipset support is based on existing documentations regarding 
the LPC bridge and is still WIP. So far no positive results with 
dISAppointment on AMD motherboards yet.

## I. Preparation
Requires Open Watcom to build. I'm currently using Open Watcom 2.0.

You need to export the path where you installed Open Watcom into. For example:

```
export WATCOM=/opt/watcom
```

## II. Building using Makefile

Currently the Makefile is meant for building the program on Linux.

Use `make` or `make all` to build the 16-bit variant of all programs.

You may also build the 32-bit variant of the programs by using `make all32`. 
However, be noted that the 32-bit variant requires a DOS extender to run, such 
as `DOS32A` or `DOS4GW`.

The 32-bit variant will have a `x` after the initial filename. 
For example, the 32-bit variant of `lpcisa` will be called `lpcisax`.

For `lpcen`, `lpccmd`, `lpcfchk`, an additional build using the `2Eh`/`2Fh` 
address pair instead of `4Eh`/`4Fh` are provided, which will have a `2` after 
the initial filename.

Details on how to use each of the programs will be explained in Usage section.

## III. Usage

**DISCLAIMER: Be careful when using these programs as it may cause your system 
to malfunction if incorrect values were specified. All arguments in the 
programs are hexadecimal.**

1. `lpcen`

This program enables the LPC-ISA bridge's configuration registers 
by writing `26h` to the location `4Eh` twice.

You need to run this once per system startup before you can use `lpccmd`.

2. `lpccmd`

```
lpccmd address [value]
```

This program reads/writes the value from/to the specified register address on 
the LPC-ISA bridge.

Enter only address to read, and enter both address and value to write.

You need to run `lpcen` or `lpcfchk` at least once before you can use it.

3. `lpcfchk`

This program checks the presence of Fintek F85226 LPC-ISA bridge, and displays 
some information about the states of some registers if the bridge is detected.

This program does not need `lpcen` to be run beforehand. It will enable the 
configuration registers on its own so you may also use `lpccmd` later on if 
needed.

4. `pcicmd`

```
pcicmd bus device function reg [value]
```

Similar to `lpccmd`, this program reads/writes the value from/to the register 
on the PCI device specified by bus, device and function number.

Enter bus, device, function and register to read. To write, additionally enter 
the value you need to write.

5. `lpcisa` / `lpcisav`

This program configures the chipset's LPC controller for decoding specified 
address ranges. By default it configures the ranges useful for sound cards.

**NOTE:** This program no longer configures the LPC-ISA bridge, as it is 
optional to do so in most cases. You need to use other utilities such as 
`lpcen`, `lpccmd` and `lpcfchk` to inspect or make changes to the registers 
in the LPC-ISA bridge if needed.

`lpcisa` is the normal version. `lpcisav` contains some verbose information. 
Both programs are functionally identical.

```
lpcisa [base1 mask1] [base2 mask2] [base3 mask3] [base4 mask4]
```

Note that there may be too many lines of output to be displayed when running 
the program, especially with verbose output. You may want to consider 
redirecting the output to a file so you can examine it later, for example 
with `lpcisav`:

```
lpcisav > output.txt
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