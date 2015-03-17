/*
 * (C) Copyright 2012
 * S3 Graphics
 *
 * (C) Copyright 2002
 * Gary Jennejohn, DENX Software Engineering, <garyj@denx.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/timer.h>


DECLARE_GLOBAL_DATA_PTR;

static struct pmu_timer *timer_base = (struct pmu_timer *)(ELITE_PMU_BASE + PMU_TIMER_BASE);


/* macro to read the 32 bit timer */
ulong timer_get_us(void)
{
	return readl(&timer_base->tc);
}


/* nothing really to do with interrupts, just starts up a counter. */
int timer_init(void)
{	
	writel(0x2, &timer_base->tcr);
	writel(0x0, &timer_base->pr);
	writel(0x1, &timer_base->tcr);

	/* reset time, capture current incrementer value time */
	gd->arch.lastinc = timer_get_us();
	gd->arch.tbl = 0;	/* start "advancing" time stamp from 0 */

	return 0;
}

ulong get_timer(ulong base)
{
	return get_timer_masked() - base;
}

void __udelay(unsigned long usec)
{
	long tmo = usec * (TIMER_CLK / 1000) / 1000;
	unsigned long now, last = timer_get_us();

	while (tmo > 0) {
		now = timer_get_us();
		if (last > now) /* count up timer overflow */
			tmo -= TIMER_LOAD_VAL - last + now + 1;
		else
			tmo -= now - last;
		last = now;
	}
}

ulong get_timer_masked(void)
{
	ulong now;

	/* current tick value */
	now = timer_get_us() / (TIMER_CLK / CONFIG_SYS_HZ);

	if (now >= gd->arch.lastinc) {	/* normal mode (non roll) */
		/* move stamp fordward with absoulte diff ticks */
		gd->arch.tbl += (now - gd->arch.lastinc);
	} else {	/* we have rollover of incrementer */
		gd->arch.tbl += ((TIMER_LOAD_VAL / (TIMER_CLK / CONFIG_SYS_HZ))
				- gd->arch.lastinc) + now;
	}
	gd->arch.lastinc = now;
	return gd->arch.tbl;
}

/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On ARM it just returns the timer value.
 */
unsigned long long get_ticks(void)
{
	return get_timer(0);
}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On ARM it returns the number of timer ticks per second.
 */
ulong get_tbclk(void)
{
	return CONFIG_SYS_HZ;
}


