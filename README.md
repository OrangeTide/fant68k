# Fantasy 68K #

## Introduction ##

This is a anachronistic fantasy home computer system, representing a
combination of a peripherials and cababilities that I feel is useful for
writing small games and toy programs.

The intention of this environment is to have a system that is specified with
enough detail that others may construct compatible implementations.

## TODO ##

Things that are not yet implemented:
  * Framebuffer (8-bits per pixel, programmable palette)
  * Mouse / Pen. (relative and absolute modes)
  * Floppy Disk Controller (720k floppy and QIC-40 tape)
  * Gamepad (D-pad plus some buttons)
  * Sound card (8-channel PCM 16-bit 48kHz)
  * MIDI in/out
  * Ethernet (NE2000 or AMD LANCE. BIOS exposes as a packet driver)

## Supported Operating Systems ##

### CP/M-68K ###

Primary support is for CP/M-68K v1.3:
  http://www.uxpro.com/cpm/
  http://www.cpm.z80.de/binary.html

### FORTH 83 ###

FORTH 83 for CP/M-68K
  http://www.cpm.z80.de/binary.html

### TBD ###

(would like to add something that is freeware/public domain for developers)

## Building Tools ##

You will need some of these tools to build the BIOS and examples.

### vasm 1.8 ###

Download vasm1\_8a.tar.gz or later:
  http://sun.hasenbraten.de/vasm/

Build:
  make CPU=m68k SYNTAX=mot

Copy vasmm68k\_mot and vobjdump

### srec ###

Download srec\_146\_src.zip :
  http://www.s-record.com/

Copy srec2bin bin2srec and binsplit
