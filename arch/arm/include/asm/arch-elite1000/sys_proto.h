/*
 * sys_proto.h
 *
 * System information header
 *
 * Copyright (C) 2011, S3 Graphics - http://www.s3graphics.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR /PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _SYS_PROTO_H_
#define _SYS_PROTO_H_

#define BOARD_REV_ID	0x71a2

typedef struct {
	char *board_string;
	char *nand_string;
} elite_sysinfo;

u32 get_cpu_rev(void);
u32 get_board_rev(void);

#ifdef CONFIG_DISPLAY_CPUINFO
int print_cpuinfo(void);
#endif

#endif //_SYS_PROTO_H_

