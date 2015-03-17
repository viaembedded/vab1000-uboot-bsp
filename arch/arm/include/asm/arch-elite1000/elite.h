/*
 * (C) Copyright 2012
 * S3 Graphics, <www.s3graphics.com>
 *
 * Authors:
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

#ifndef _ELITE1000_H_
#define _ELITE1000_H_

#define BIT(x)						(1UL<<(x))

#define LOW_LEVEL_SRAM_STACK		0x4000000

#define PMU_ARM_SOFT_RESET			0xd839cb00

/*
 * Internal APB Slaves Memory Address Map
 */

#define ELITE_DMA_BASE       0xD8001800  /* System Dma ELITE_se Address */
#define ELITE_SF_BASE        0xD8002000  /* Serial Flash Memory Controller Configuration */
#define ELITE_CIPHER_BASE    0xD8006000  /* Security Acceleration Engine Configuration */
#define ELITE_UHC_BASE       0xD8007000  /* USB 2.0 Host Controller Configuration */
#define ELITE_PATA_BASE      0xD8008000  /* PATA Controller Configuration */
#define ELITE_NFC_BASE       0xD8009000  /* NANAD Controller Configuration */
#define ELITE_SDC_BASE       0xD800A000  /* SD/SDIO/MMC Controller Configuration */
#define ELITE_MSC_BASE       0xD800B000  /* Memory Stick/PRO Configuration */
#define ELITE_SATA_BASE      0xD800D000  /* SATA Controller Configuration */
#define ELITE_RTC_BASE       0xD8100000  /* Real Time Clock (RTC) */
#define ELITE_GPIO_BASE      0xD8110000  /* GPIO */
#define ELITE_SCC_BASE       0xD8120000  /* System Configuration Control */
#define ELITE_PMC_BASE       0xD8130000  /* Power Management Control */
#define ELITE_IC_BASE        0xD8140000  /* Interrupt Controller */
#define ELITE_UART0_BASE     0xD8200000  /* UART0 */
#define ELITE_UART1_BASE     0xD82B0000  /* UART1 */
#define ELITE_SPI_BASE       0xD8240000  /* SPI */
#define ELITE_I2C_BASE       0xD8280000  /* I2C */

#ifndef __ASSEMBLY__
#endif /* __ASSEMBLY__ */
#endif
