/*
 * (C) Copyright 2012
 * S3 Graphics, Inc <www.s3graphics.com>
 *
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 */

#ifndef _TIMER_H
#define _TIMER_H

/* counter runs at 1MHz */
#define TIMER_CLK	1000000
#define TIMER_LOAD_VAL 0xffffffff

#define ELITE_PMU_BASE 0xd8390000

#define PMU_TIMER_BASE  0x4000
#define PMU_TIMER0      0x0
#define PMU_TIMER1      0x28


#if !(defined(__KERNEL_STRICT_NAMES) || defined(__ASSEMBLY__))
#include <asm/types.h>
#endif /* !(__KERNEL_STRICT_NAMES || __ASSEMBLY__) */

#ifndef __KERNEL_STRICT_NAMES
#ifndef __ASSEMBLY__


struct pmu_timer {
	u32 ir;               /* 0x00 rw */
	u32 tcr;          /* 0x04 rw */
	u32 tc;           /* 0x08 rw */
	u32 pr;               /* 0x0c rw */
	u32 pc;               /* 0x10 rw */
	u32 mr0;               /* 0x14 rw */
	u32 mr1;               /* 0x18 rw */
	u32 mr2;               /* 0x1c rw */
	u32 mr3;               /* 0x20 rw */
	u32 mcr;               /* 0x24 rw */
};


#endif /* __ASSEMBLY__ */
#endif /* __KERNEL_STRICT_NAMES */

#endif //_TIMER_H
