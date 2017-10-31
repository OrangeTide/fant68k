/* Public Domain or CC0 - No Copyright Claimed
 * Headers and interfaces aren't something you can copyright in my jurisdiction.
 */
#ifndef FANT_H
#define FANT_H
#include <stdint.h>

#define MC6850_STAT	0xff1000L	/* command/status register */
#define MC6850_DATA	0xff1002L	/* receive/transmit data register */

#define S_TIME		(0xff7ff8)	/* read long to get time in seconds */
#define SIM_EXIT	(0xff7ffc)	/* write long to exit simulator */

/*~* mem.c *~*/
int bootrom_load(const char *);

/*~* fant.c *~*/
void fant_done(int);

/*~* con.c *~*/
uint8_t console_in(void);
void console_out(uint8_t);
unsigned console_stat(void);

/*~* tim.c *~*/
void timer_init(void);
uint32_t timer_read_stime32(void);
uint16_t timer_read_stime16(unsigned addr);
uint8_t timer_read_stime8(unsigned addr);
void timer_update32(unsigned value);
void timer_update16(unsigned addr, unsigned value);
void timer_update8(unsigned addr, unsigned value);

#endif
