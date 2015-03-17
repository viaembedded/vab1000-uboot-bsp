/*
 * (C) Copyright 2000-2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2010 S3 Graphics Co.,Ltd.
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
#ifndef _ELITE_SPI_FLASH
#define _ELITE_SPI_FLASH

#define ELITE_SPI_FLASH_REG_BASE    0xD8002000

/* offset of registers for SPI flash */

#define CHIP_SEL_0_CFG         0x00
#define CHIP_SEL_1_CFG         0x08
#define SPI_IF_CFG             0x40
#define SPI_RD_WR_CTRL         0x50
#define SPI_WR_EN_CTRL         0x60
#define SPI_ERASE_CTRL         0x70
#define SPI_ERASE_START_ADDR     0x74
#define SPI_ERROR_STATUS       0x80
#define SPI_MEM0_SR_ACC        0x100
#define SPI_MEM1_SR_ACC        0x110
#define SPI_PDWN_CTRL_0        0x180
#define SPI_PDWN_CTRL_1        0x190
#define SPI_PROG_CMD_CTRL      0x200
#define SPI_USER_CMD_VAL       0x210
#define SPI_PROG_CMD_WBF       0x300
#define SPI_PROG_CMD_RBF       0x380
 

#define SF_BIT_WR_PROT_ERR     0x20 /* [5:5] */
#define SF_BIT_MEM_REGION_ERR  0x10 /* [4:4] */
#define SF_BIT_PWR_DWN_ACC_ERR 0x8  /* [3:3] */
#define SF_BIT_PCMD_OP_ERR     0x4  /* [2:2] */
#define SF_BIT_PCMD_ACC_ERR    0x2  /* [1:1] */
#define SF_BIT_MASLOCK_ERR     0x1  /* [0:0] */
#define BIT_TIMEOUT	       0x80000000

static inline volatile void *get_spi_flash_reg_base(void)
{
	return (volatile void * const)(ELITE_SPI_FLASH_REG_BASE);
}

#endif
