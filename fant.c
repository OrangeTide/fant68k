/* fant.c - Fantasy Home Computer System */
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

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "fant.h"
#include "m68k.h"

#define ALINETRAPNUMBER 0xA /* Unimplemented Instruction (Line-A) */
#define FLINETRAPNUMBER 0xB /* Unimplemented Instruction (Line-F) */
#define TRAP15 0xF /* I/O trap used by Sim68K */

static sig_atomic_t keep_going;

void fant_done(int code)
{
	if (code)
		fprintf(stderr, "Exiting with code %d\n", code);
	else
		fprintf(stderr, "DONE: normal termination\n");
	keep_going = 0;
	m68k_end_timeslice();
}

static void
quit_handler(int s __attribute__((unused)))
{
	keep_going = 0;
}

static void
init(void)
{
	m68k_init();
	m68k_set_cpu_type(M68K_CPU_TYPE_68000);
	m68k_pulse_reset();
}

static void
run(void)
{
	while (keep_going) {
		m68k_execute(1000);
	}
}

int
main(int argc __attribute__((unused)), char **argv __attribute__((unused)))
{
	setlocale(LC_ALL, NULL);

	keep_going = 1;

	signal(SIGTERM, quit_handler);
	signal(SIGHUP, quit_handler);

	int self_test = 0;
	if (self_test) {
		// TODO: implement something real
	}

	bootrom_load("hello.bin");

	timer_init();

	init();

	run();

	return 0;
}
