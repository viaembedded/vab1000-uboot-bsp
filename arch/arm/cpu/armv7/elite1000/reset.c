/*
 *
 * Common layer for reset related functionality of ELITE based socs.
 *
 * (C) Copyright 2012
 * S3 Graphics, <www.s3graphics.com>
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
 */
#include <config.h>
#include <asm/io.h>
#include <asm/arch/elite.h>
#include <linux/compiler.h>

void __weak reset_cpu(unsigned long ignored)
{
	u32 val = readl(PMU_ARM_SOFT_RESET) | 0x07;
	
	writel(val, PMU_ARM_SOFT_RESET);
}

u32 __weak warm_reset(void)
{
	return (readl(PMU_ARM_SOFT_RESET) & 0x07);
}

void __weak setup_warmreset_time(void)
{
}
