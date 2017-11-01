/* dipswitch.c - DIP switch settings */
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

#include <stdlib.h>
#include <time.h>

#include "fant.h"

static uint32_t dip_value = 0;

void
dip_init(uint32_t dip)
{
	dip_value = dip;
}

uint32_t
dip_read32(void)
{
	return dip_value;
}

uint16_t
dip_read16(unsigned addr)
{
	uint32_t sec = dip_read32();
	switch (addr & 2u) {
	case 0:
		return (sec >> 16) & 65535u;
	case 2:
		return sec & 65535u;
	}
	abort();
}

uint8_t
dip_read8(unsigned addr)
{
	uint32_t sec = dip_read32();
	switch (addr & 3u) {
	case 0:
		return (sec >> 24) & 255u;
	case 1:
		return (sec >> 16) & 255u;
	case 2:
		return (sec >> 8) & 255u;
	case 3:
		return sec & 255u;
	}
	abort();
}

/** UNTESTED: None of this has been tested. Please write a unit test **/
