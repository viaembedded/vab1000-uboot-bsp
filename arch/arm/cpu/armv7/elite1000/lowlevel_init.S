/*
 * Board specific setup info
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>
#include <version.h>
#include <linux/linkage.h>
#include <asm/arch/elite.h>

_TEXT_BASE:
	.word	CONFIG_SYS_TEXT_BASE

ENTRY(lowlevel_init)
	ldr	sp, SRAM_STACK
	str	ip, [sp]	/* stash ip register */
	mov	ip, lr		/* save link reg across call */

	mov	lr, ip		/* restore link reg */
	ldr	ip, [sp]	/* restore save ip */
	/* tail-call s_init to setup pll, mux, memory */
	b	s_init
ENDPROC(lowlevel_init)

SRAM_STACK:
	.word LOW_LEVEL_SRAM_STACK
