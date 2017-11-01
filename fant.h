/* Public Domain or CC0 - No Copyright Claimed
 * Headers and interfaces aren't something you can copyright in my jurisdiction.
 */
#ifndef FANT_H
#define FANT_H
#include <stdint.h>

#define MC6850_STAT	0xff1000L	/* command/status register */
#define MC6850_DATA	0xff1002L	/* receive/transmit data register */

#define DIP_SWITCH	(0xff7ff0)	/* DIP switch settings */
#define DIP_SWITCH_GRAPHICS_BIT	(1u << 0)	/* serial(0) or graphics(1) */
#define DIP_SWITCH_KEYBOARD_BIT	(1u << 1)	/* keyboard present? */
#define DIP_SWITCH_MOUSE_BIT	(1u << 2)	/* keyboard only or mouse(1) */
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

/*~* dipswitch.c *~*/
void dip_init(uint32_t dip);
uint32_t dip_read32(void);
uint16_t dip_read16(unsigned addr);
uint8_t dip_read8(unsigned addr);

/*~* tim.c *~*/
void timer_init(void);
uint32_t timer_read_stime32(void);
uint16_t timer_read_stime16(unsigned addr);
uint8_t timer_read_stime8(unsigned addr);
void timer_update32(unsigned value);
void timer_update16(unsigned addr, unsigned value);
void timer_update8(unsigned addr, unsigned value);

#endif
