/* display_sdl.c - SDL2 display driver */
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

#include <assert.h>
#include <endian.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL.h>

#include "fant.h"

static SDL_Window *main_window;
static uint8_t *vidmem;
static unsigned vid_width = 768, vid_height = 512;
// TODO: vid_stride = 1024;

void
display_init(void)
{
	// TODO: create an SDL-friendly surface
	vidmem = calloc(1, VIDMEM_LEN);
	// TBD: initialize SDL somewhere for input and timers to also use
	SDL_Init(SDL_INIT_EVERYTHING);

	main_window = SDL_CreateWindow("Fant68k",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		vid_width, vid_height,
		SDL_WINDOW_OPENGL);
	if (!main_window)
		exit(1);

#if 0
	/* TODO: move this into the central loop */
	SDL_Event e;
	while (!SDL_PollEvent(&e))
		; /* TODO: implement event handling instead */
#endif

	fprintf(stderr, "%s: initialized\n", __func__);
}

void
display_quit(void)
{
	SDL_DestroyWindow(main_window);

	SDL_Quit();

	free(vidmem);
	vidmem = NULL;
}

uint32_t
display_read32(unsigned addr)
{
	assert(addr < VIDMEM_LEN && vidmem != NULL);
	return htobe32(*(uint32_t*)&vidmem[addr]);
}

uint16_t
display_read16(unsigned addr)
{
	assert(addr < VIDMEM_LEN && vidmem != NULL);
	return htobe16(*(uint32_t*)&vidmem[addr]);
}

uint8_t
display_read8(unsigned addr)
{
	assert(addr < VIDMEM_LEN && vidmem != NULL);
	return vidmem[addr];
}

void
display_write32(unsigned addr, unsigned value)
{
	assert(addr < VIDMEM_LEN && vidmem != NULL);
	*(uint32_t*)&vidmem[addr] = be32toh(value);
}

void
display_write16(unsigned addr, unsigned value)
{
	assert(addr < VIDMEM_LEN && vidmem != NULL);
	*(uint16_t*)&vidmem[addr] = be16toh(value);
}

void
display_write8(unsigned addr, unsigned value)
{
	assert(addr < VIDMEM_LEN && vidmem != NULL);
	vidmem[addr] = value;
}

/** UNTESTED: None of this has been tested. Please write a unit test **/
