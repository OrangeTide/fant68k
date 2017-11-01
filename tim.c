/* timer.c - time and timers */
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

/* writing the current value of S_TIME back into S_TIME will adjust future
 * reads by that amount. */
static uint32_t timer_offset = 0;

void
timer_init(void)
{
	/* Nothing */
}

uint32_t
timer_read_stime32(void)
{
	time_t now = time(0);
	struct tm *tm = localtime(&now);
	uint32_t sec = tm->tm_sec + tm->tm_min * 60ul + tm->tm_hour * 3600ul;
	return sec - timer_offset; /* seconds since midnight */
}

uint16_t
timer_read_stime16(unsigned addr)
{
	uint32_t sec = timer_read_stime32();
	switch (addr & 2u) {
	case 0:
		return (sec >> 16) & 65535u;
	case 2:
		return sec & 65535u;
	}
	abort();
}

uint8_t
timer_read_stime8(unsigned addr)
{
	uint32_t sec = timer_read_stime32();
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

void
timer_update32(unsigned value)
{
	timer_offset = value;
}

void
timer_update16(unsigned addr, unsigned value)
{
	switch (addr & 2u) {
	case 0:
		timer_offset &= ~0xffff0000u;
		timer_offset |= (uint32_t)(value & 65535) << 16;
		break;
	case 2:
		timer_offset &= ~0x0000ffffu;
		timer_offset |= (uint32_t)(value & 65535);
		break;
	}
}

void
timer_update8(unsigned addr, unsigned value)
{
	switch (addr & 3u) {
	case 0:
		timer_offset &= ~0xff000000u;
		timer_offset |= (uint32_t)(value & 255) << 24;
		break;
	case 1:
		timer_offset &= ~0x00ff0000u;
		timer_offset |= (uint32_t)(value & 255) << 16;
		break;
	case 2:
		timer_offset &= ~0x0000ff00u;
		timer_offset |= (uint32_t)(value & 255) << 8;
		break;
	case 3:
		timer_offset &= ~0x000000ffu;
		timer_offset |= (uint32_t)(value & 255);
		break;
	}
}

/** UNTESTED: None of this has been tested. Please write a unit test **/
