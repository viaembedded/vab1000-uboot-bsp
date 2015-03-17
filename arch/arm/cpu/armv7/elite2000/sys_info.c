/*
 * (C) Copyright 2010,2011
 * S3 Graphics <www.s3graphics.com>
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
#include <linux/ctype.h>
#include <asm/arch/sys_proto.h>

/**
 * get_cpu_rev(void) - extract rev info
 */
u32 get_cpu_rev(void)
{
	u32 id;
	u32 rev;

	id = readl(0xD80A8500);
	rev = (id >> 28) & 0xff;

	return rev;
}

/**
 * get_board_rev() - setup to pass kernel board revision information
 * returns:(bit[0-7] chip version, higher bit[15-8] is board type)
 */
u32 get_board_rev(void)
{
	return BOARD_REV_ID;
}

#ifdef CONFIG_DISPLAY_CPUINFO
void upstring(char *s)
{
	while (*s) {
		*s = toupper(*s);
		s++;
	}
}

/* Print CPU information */
int print_cpuinfo(void)
{
	char soc_name[10];

	strncpy(soc_name, CONFIG_SYS_SOC, 10);
	upstring(soc_name);
	//puts(soc_name);
	//puts("\n");

	/* TBD: Add printf of major/minor rev info, stepping, etc. */
	return 0;
}
#endif	/* CONFIG_DISPLAY_CPUINFO */

