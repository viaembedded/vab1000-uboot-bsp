/*
 * PCI initialisation for Elite 1000 board.
 *
 * (C) Copyright 2011
 * S3 Graphics
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

#ifndef __ELITE_PCIE_H__
#define __ELITE_PCIE_H__

#include <asm/sizes.h>


#define ELITE_PCIE0_BASE_ADDR		0xd80b0000
#define ELITE_PCIE1_BASE_ADDR		0xd80c0000
#define ELITE_PCIE2_BASE_ADDR		0xd80d0000
#define ELITE_PCIE_PORTS_SIZE		SZ_64K	

#define BUS_DEV_FUNC_ID_SECTION_OFFSET  0x0260

#define DEVICE_CFG_SIZE				SZ_4K


#define ELITE_PCIE_IO_PHYS_BASE(i)  (0xcfff0000 + ((i) << 16))
#define ELITE_PCIE_IO_PHYS_SIZE 	 SZ_64K 

#define SZ_255M						(SZ_256M - SZ_1M)

#if 0
# define ELITE_PCIE_MEM_PHYS_BASE  0xc1800000
# define ELITE_PCIE_MEM_SIZE		SZ_232M
#else
# define ELITE_PCIE_MEM_PHYS_BASE	0xc0000000
# define ELITE_PCIE_MEM_SIZE		SZ_255M 
#endif

#if 0
#define SZ_232M		0xce800000	
#define SZ_116M		0xc7400000	
#define SZ_77M		0xc4d00000
#endif

#define IRQ_ELITE_PCIE_PORT0	(9 + 32)
#define IRQ_ELITE_PCIE_PORT1	(11 + 32)
#define IRQ_ELITE_PCIE_PORT2	(13 + 32)


#define PCI_BASE0 0x10
#define PCI_BASE1 0x14
#define PCI_BASE2 0x18
#define PCI_BASE3 0x1c
#define PCI_BASE4 0x20

#endif //__ELITE_PCIE_H__

