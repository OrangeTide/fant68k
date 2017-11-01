/* mem.c - Memory I/O routines */
/*
 * Copyright (c) 2017 Jon Mayo <jon@rm-f.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <endian.h>

#include "fant.h"
#include "m68k.h"

/* use && 1 to enable */
#if !defined(NDEBUG) && 0
#define TRACE_MEM(addr) fprintf(stderr, "%s:addr=$%06X\n", __func__, (addr))
#else
#define TRACE_MEM(addr) /* nothing */
#endif

static uint8_t ram0[RAM0_LEN];
static uint8_t bootrom[BOOTROM_LEN];
static uint8_t ram1[RAM1_LEN];

static void
io_error(const char *msg, unsigned int address)
{
	fprintf(stderr, "ERR: %s ($%X)\n", msg, address);
}

static inline void *
mem_access(unsigned int address)
{
	static uint32_t dummy;

	if (address < RAM0_LEN)
		return &ram0[address - RAM0_BASE];
	else if (address < VIDMEM_BASE + VIDMEM_LEN)
		return &dummy; /* NOTE: should never see this */
	else if (address < BOOTROM_BASE + BOOTROM_LEN)
		return &bootrom[address - BOOTROM_BASE];
	else if (address < RAM1_BASE + RAM1_LEN)
		return &ram1[address - RAM1_BASE];
	else if (address < IO_BASE + IO_LEN)
		return &dummy; /* NOTE: should never see this */
	io_error("illegal access", address);
#ifndef NDEBUG
	abort();
#endif
	dummy = ~0U;
	return &dummy;
}

void
m68k_write_memory_8(unsigned int address, unsigned int value)
{
	TRACE_MEM(address);
	switch (address) {
	case SIM_EXIT:
		fant_done(1);
		return;
	case MC6850_DATA:
		console_out(value);
		return;
	case S_TIME:
	case (S_TIME + 1):
	case (S_TIME + 2):
	case (S_TIME + 3):
		timer_update8(address & 3, value);
		return;
	case DIP_SWITCH:
	case (DIP_SWITCH + 1):
	case (DIP_SWITCH + 2):
	case (DIP_SWITCH + 3):
		return; /* ignored */
	}

	if (address >= VIDMEM_BASE && address < VIDMEM_BASE + VIDMEM_LEN) {
		display_write8(address - VIDMEM_BASE, value);
		return;
	}

	if (address >= BOOTROM_BASE && address < BOOTROM_BASE + BOOTROM_LEN)
		return; /* ignore writes to ROM */

	uint8_t *p = mem_access(address);
	*p = value;
}

void
m68k_write_memory_16(unsigned int address, unsigned int value)
{
	TRACE_MEM(address);
	switch (address) {
	case SIM_EXIT:
		fant_done(1);
		return;
	case MC6850_DATA:
		console_out(value);
		io_error("illegal access", address);
#ifndef NDEBUG
		abort();
#endif
		return;
	case S_TIME:
	case (S_TIME + 2):
		timer_update16(address & 2, value);
		return;
	case DIP_SWITCH:
	case (DIP_SWITCH + 2):
		return; /* ignored */
	}

	if (address >= VIDMEM_BASE && address < VIDMEM_BASE + VIDMEM_LEN) {
		display_write16(address - VIDMEM_BASE, value);
		return;
	}

	if (address >= BOOTROM_BASE && address < BOOTROM_BASE + BOOTROM_LEN)
		return; /* ignore writes to ROM */

	uint16_t *p = mem_access(address);
	uint16_t v = value;
	*p = htobe16(v);
}

void
m68k_write_memory_32(unsigned int address, unsigned int value)
{
	TRACE_MEM(address);
	switch (address) {
	case SIM_EXIT:
		fant_done(1);
		return;
	case MC6850_DATA:
		console_out(value);
		io_error("illegal access", address);
#ifndef NDEBUG
		abort();
#endif
		return;
	case S_TIME:
		timer_update32(value);
		return;
	case DIP_SWITCH:
		return; /* ignored */
	}

	if (address >= VIDMEM_BASE && address < VIDMEM_BASE + VIDMEM_LEN) {
		display_write32(address - VIDMEM_BASE, value);
		return;
	}

	if (address >= BOOTROM_BASE && address < BOOTROM_BASE + BOOTROM_LEN)
		return; /* ignore writes to ROM */

	uint32_t *p = mem_access(address);
	uint32_t v = value;
	*p = htobe32(v);
}

#if 0
void
m68k_write_memory_32_pd(unsigned int address, unsigned int value)
{
	// TODO: implement this
	io_error("not implemented");
}
#endif

unsigned int
m68k_read_memory_8(unsigned int address)
{
	TRACE_MEM(address);
	switch (address) {
	case SIM_EXIT:
		fprintf(stderr, "WARN: access to simulator exit\n");
#ifndef NDEBUG
		abort();
#endif
		return 0;
	case MC6850_DATA:
		return console_in();
	case S_TIME:
	case (S_TIME + 1):
	case (S_TIME + 2):
	case (S_TIME + 3):
		return timer_read_stime8(address & 3);
	case DIP_SWITCH:
	case (DIP_SWITCH + 1):
	case (DIP_SWITCH + 2):
	case (DIP_SWITCH + 3):
		return dip_read8(address & 3);
	}

	if (address >= VIDMEM_BASE && address < VIDMEM_BASE + VIDMEM_LEN)
		return display_read8(address - VIDMEM_BASE);

	uint8_t *p = mem_access(address);
	return *p;
}

unsigned int
m68k_read_memory_16(unsigned int address)
{
	TRACE_MEM(address);
	switch (address) {
	case SIM_EXIT:
		fprintf(stderr, "WARN: access to simulator exit\n");
#ifndef NDEBUG
		abort();
#endif
		return 0;
	case MC6850_DATA:
		io_error("illegal access", address);
		return console_in();
	case S_TIME:
	case (S_TIME + 2):
		return timer_read_stime16(address & 2);
	case DIP_SWITCH:
	case (DIP_SWITCH + 2):
		return dip_read16(address & 2);
	}

	if (address >= VIDMEM_BASE && address < VIDMEM_BASE + VIDMEM_LEN)
		return display_read16(address - VIDMEM_BASE);

	uint16_t *p = mem_access(address);
	uint16_t v = *p;
	return be16toh(v);
}

unsigned int
m68k_read_memory_32(unsigned int address)
{
	TRACE_MEM(address);
	switch (address) {
	case SIM_EXIT:
		fprintf(stderr, "WARN: access to simulator exit\n");
#ifndef NDEBUG
		abort();
#endif
		return 0;
	case MC6850_DATA:
		io_error("illegal access", address);
		return console_in();
	case S_TIME:
		return timer_read_stime32();
	case DIP_SWITCH:
		return dip_read32();
	}

	if (address >= VIDMEM_BASE && address < VIDMEM_BASE + VIDMEM_LEN)
		return display_read32(address - VIDMEM_BASE);

	uint32_t *p = mem_access(address);
	uint32_t v = *p;
	return be32toh(v);
}

int
bootrom_load(const char *filename)
{
	FILE *f = fopen(filename, "rb");
	if (!f) {
		perror(filename);
		return -1;
	}
	size_t bootrom_len = fread(bootrom, 1, sizeof(bootrom), f);
	fclose(f);

	/* fill the remainder with 0xff */
	memset(bootrom + bootrom_len, 0xff, BOOTROM_LEN - bootrom_len);

	/* init trap vector */
	uint32_t boot[] = {
		be32toh(0x00000600), /* stack at $600 */
		be32toh(0x00680000), /* bootroom start at $68000 */
	};
	memcpy(ram0, boot, sizeof(boot));

	/* print memory map info */
	fprintf(stderr,
		"Memory Map:\n"
		"$%06X - $%06X : RAM0 (%uMb)\n"
		"$%06X - $%06X : VIDMEM\n"
		"$%06X - $%06X : BOOTROM\n"
		"$%06X - $%06X : RAM1 (%uMb)\n"
		"$%06X - $%06X : I/O memory\n",
		RAM0_BASE, RAM0_BASE + RAM0_LEN - 1, RAM0_LEN / 1048576,
		VIDMEM_BASE, VIDMEM_BASE + VIDMEM_LEN - 1,
		BOOTROM_BASE, BOOTROM_BASE + BOOTROM_LEN - 1,
		RAM1_BASE, RAM1_BASE + RAM1_LEN - 1, RAM1_LEN / 1048576,
		IO_BASE, IO_BASE + IO_LEN - 1);

	return 0;
}
