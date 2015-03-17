/*
 * elite1k.h - Default configuration for ELITE2000 EVM FPGA board.
 *
 * Copyright (C) 2013 S3 Graphics, Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * High Level Configuration Options
 * (easy to change)
 */
#define	CONFIG_SYS_TEXT_BASE	0x3F80000
#define CONFIG_SYS_SOC			"elite2000"
//#define CONFIG_SYS_DCACHE_OFF
#define CONFIG_L2_OFF

#include <asm/arch/elite.h>
#include <asm/sizes.h>
//#include <config_cmd_default.h>
/* partition support */
#define CONFIG_DOS_PARTITION	      /* to support MSFT Master Boot Record */
#define FPGA_BRINGUP
/*
 * Display CPU and Board information
 */
#define CONFIG_DISPLAY_CPUINFO		1
#define CONFIG_DISPLAY_BOARDINFO	1

#define CONFIG_CMDLINE_TAG		1	/* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS	1
#define CONFIG_INITRD_TAG		1
#define CONFIG_REVISION_TAG		1

/* new uImage format support */
#define CONFIG_FIT		1
#define CONFIG_OF_LIBFDT	1
#define CONFIG_FIT_VERBOSE	1 /* enable fit_format_{error,warning}() */

#define CONFIG_BOARD_LATE_INIT	1 /* for fastboot preboot */

/* Clock settings */
#define CONFIG_SYS_HZ			1000 /* Ticks per second */

#define CONFIG_CMD_MEMORY	/* md mm nm mw cp cmp crc base loop */

/*
 * Hardware drivers
 */
//#define CONFIG_ELITE_GPIO

/* NAND driver */
/*
#define CONFIG_SYS_NAND_QUIET_TEST
#define CONFIG_ELITE_NAND		1
#define CONFIG_SYS_NAND_BASE		0xd8009000 
#define CONFIG_SYS_MAX_NAND_DEVICE	1 
#define DEBUG_FPGA 
#define CONFIG_ELITE_NAND_HWECC
#define	CONFIG_SYS_NAND_USE_FLASH_BBT

#define NAND_BYTES_PER_PAGE    4096
#define NAND_OOB_SIZE          218
#define NAND_ECC_MODE          12
*/

/*
 * select serial console configuration
 */
#define CONFIG_CONS_INDEX		1
#define CONFIG_ELITE_SERIAL		1

#define CONFIG_SYS_NO_FLASH            1
/* 
 * SPI Flash and e-load/u-boot layout
 *
   -----------------------------
   |                           |
   |   e-load (1 sector size)  |
   |                           |
   ----------------------------
   |                           |
   |     u-boot parameters     |
   |      (2 sector size)      |
   ----------------------------
   |                           |
   |       u-boot image        |
   |      (5 sector size)      |
   ----------------------------
 */

#define SPI_FLASH_SIZE                 (SZ_4M)
#define SPI_BANK0_ADDR                 (0xffffffff - (SPI_FLASH_SIZE) + 1)  
#define SPI_BANK1_ADDR                 ((SPI_BANK0_ADDR - SPI_FLASH_SIZE))

#define SPI_BANK_NUM                   1

#if (SPI_BANK_NUM == 1)
#define SPI_FLASH_BASE                 (SPI_BANK0_ADDR)
#else
#define SPI_FLASH_BASE                 (SPI_BANK1_ADDR)
#endif

#define CONFIG_SYS_FLASH_BASE          (SPI_FLASH_BASE)        
#define CONFIG_SYS_MAX_FLASH_BANKS     1
#define CONFIG_SYS_MAX_FLASH_SECT      69

/* SPI Flash */
#define CONFIG_MAX_SPI_FLASH_BANKS     1

/* 
 * ELITE designs seperated module for SPI flash rather than 
 * attaching SPI flash on SPI bus
 * So sepcified functions should be provided for ELITE
 */
#define CONFIG_ELITE_SPI_FLASH         1
#define CONFIG_SYS_FLASH_ERASE_TOUT    (3*CONFIG_SYS_HZ)
#define CONFIG_SYS_FLASH_WRITE_TOUT    (3*CONFIG_SYS_HZ)

/* commands to include */
#include <config_cmd_default.h>

/* elf and fat format */
#define CONFIG_CMD_ELF
#define CONFIG_CMD_FAT
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_ILI

/* save environment */
#define CONFIG_CMD_SAVEENV

/* SPI flash */
#define CONFIG_CMD_SF

/* Linux zImage support */
#define CONFIG_CMD_BOOTZ

/* to support fastboot booti */
#define CONFIG_SUPPORT_RAW_INITRD

/* EXT4 filesystem support */
#define CONFIG_FS_EXT4
#define CONFIG_EXT4_WRITE
#define CONFIG_CMD_EXT4


#ifndef FPGA_BRINGUP
/* PCI support */
/*
#define CONFIG_ELITE_PCIE
#define CONFIG_SYS_PCI_MEMORY_BUS      0x00000000
#define CONFIG_SYS_PCI_MEM_BUS         0x80000000
#define CONFIG_SYS_PCI_IO_BUS          0x00800000

#define CONFIG_PCI_SCAN_SHOW
#define CONFIG_CMD_PCI_ENUM
#define CONFIG_CMD_PCI
#define CONFIG_PCI
*/

/* i2c support */
/*
#define CONFIG_SYS_I2C_SPEED		100000
#define CONFIG_SYS_I2C_SLAVE		1
#define CONFIG_ELITE_I2C	1
#define CONFIG_HARD_I2C		1
#define CONFIG_CMD_I2C		1
*/

#endif

//#ifndef FPGA_BRINGUP
/* SD/MMC support */
#define CONFIG_MMC
#define CONFIG_CMD_MMC
#define CONFIG_SDHCI
#define CONFIG_ELITE_SDHCI
#define CONFIG_GENERIC_MMC
//#else
//#define HAVE_BLOCK_DEVICE
//#endif

/* eth support */
/*
#define CONFIG_RTL8168
#define CONFIG_ARP_TIMEOUT		(5*CONFIG_SYS_HZ)
#define CONFIG_CMD_NET
#define CONFIG_NET_MULTI
*/

#ifndef FPGA_BRINGUP
/* nand-related commands */
#define CONFIG_CMD_NAND			1
#define CONFIG_MTD_NAND_YAFFS2		1
#define CONFIG_CMD_MTDPARTS		1
#define CONFIG_MTD_DEVICE		1
#define CONFIG_MTD_PARTITIONS		1
#define MTDIDS_DEFAULT			"nand0=nand"
#define CONFIG_CMD_MTDPARTS_SHOW_NET_SIZES

#define MTDPARTS_DEFAULT		"mtdparts=nand:4M(secureboot),"\
					"4M(secureos),4M(audiofirmware),"\
					"2M(uboot_env),4M(uboot),"\
					"4M(boot_logo),4M(nvram),"\
					"12M(devicetree),32M(otaloader),"\
					"32M(iploader),16M(device_info),"\
					"16M(misc),16M(boot),"\
					"16M(recovery),"\
					"384M(system),"\
					"32M(package),"\
					"384M(cache),"\
					"-(data)"
#endif

/* Environment information */
#define SPI_SECTOR_SIZE                (SZ_64K)
#define ELOAD_START_ADDR               (0xffffffff - (SPI_SECTOR_SIZE) + 1)
#define UBOOT_PARA_ADDR                ((ELOAD_START_ADDR) - 2*(SPI_SECTOR_SIZE))

#define CONFIG_ENV_IS_IN_SPI_FLASH     1
#define CONFIG_ENV_SECT_SIZE           (SPI_SECTOR_SIZE)
#define CONFIG_ENV_SIZE                (SPI_SECTOR_SIZE)
#define CONFIG_ENV_ADDR                (UBOOT_PARA_ADDR)
#define CONFIG_ENV_OFFSET              ((UBOOT_PARA_ADDR) - (SPI_FLASH_BASE))

/* Address and size of Redundant Environment Sector */
#define CONFIG_ENV_OFFSET_REDUND       ((CONFIG_ENV_OFFSET) + (SPI_SECTOR_SIZE))
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT

/*
 * Shell Settings
 */
#define CONFIG_CMDLINE_EDITING			1 //command line histroy
#define CONFIG_SYS_LONGHELP
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_HUSH_PARSER
#ifdef CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#endif

/* Stack size */
#define CONFIG_STACK_SIZE              (SIZE_128KB)
#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ           (SIZE_4KB)   /* IRQ stack */
#define CONFIG_STACKSIZE_FIQ           (SIZE_4KB)   /* FIQ stack */
#endif

/* Size of malloc() Pool */
#ifdef CONFIG_ENV_OFFSET_REDUND
#define CONFIG_SYS_MALLOC_LEN          ((CONFIG_ENV_SIZE)*3 + 5*(1024 << 10))
#else
#define CONFIG_SYS_MALLOC_LEN          ((CONFIG_ENV_SIZE) +  5*(1024 << 10))
#endif
#define CONFIG_SYS_GBL_DATA_SIZE       128     /* bytes reserved for global parameters */

/* Allow to Overwrite Serial and Ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_BAUDRATE                115200
#define CONFIG_SYS_BAUDRATE_TABLE      {4800, 9600, 19200, 38400, 57600, 115200}

#define CONFIG_BOOTDELAY               3
#define CONFIG_BOOTCOMMAND             "ext4load mmc 0:1 0x2800000 uImage; ext4load mmc 0:1 0x6000000 elite2000-fpga.dtb; bootm 0x2800000 - 0x6000000"
#define CONFIG_BOOTARGS                "console=ttyS0,115200n8 root=/dev/mmcblk0p1 rootflags=errors=remount-ro,commit=0 rootfstype=ext4 rw init=/init rootdelay=3"

#ifndef CONFIG_SPL_BUILD
#define CONFIG_EXTRA_ENV_SETTINGS \
	"loadaddr=0x80200000\0" \
	"fdtaddr=0x10000000\0" \
	"fdt_high=0xffffffff\0" \
	"rdaddr=0x81000000\0" \
	"bootdir=/boot\0" \
	"bootfile=uImage\0" \
	"fdtfile=undefined\0" \
	"initrd_high=0xffffffff\0" \
	"ethaddr=00:1a:32:b0:12:63\0" \
	"kernelargs=console=ttyS0,115200n8 rootdelay=1\0" \
	"bootargs_mmc=" \
		"setenv bootargs ${kernelargs} " \
		"root=/dev/mmcblk0p1 rootwait rootflags=errors=remount-ro,commit=0 rootfstype=ext4 rw ${mtdparts} init=/init rw\0" \
	"bootcmd_mmc=" \
		"run bootargs_mmc && " \
		" " CONFIG_BOOTCOMMAND " \0"

#endif

/* Physical memory map */
#define RESERVED_MEMORY					SZ_32M	/* Reserved memory for DSP */
#define CONFIG_NR_DRAM_BANKS			1       /* CS1 may or may not be populated */
#define CONFIG_MAX_RAM_BANK_SIZE 		SZ_2G   /* 2 GB */

#define CONFIG_SYS_MEMTEST_START       0x00000000
#define CONFIG_SYS_MEMTEST_END         ((CONFIG_SYS_MEMTEST_START) + 0x10000000) /* 512MB */
#define CONFIG_SYS_SDRAM_BASE          (RESERVED_MEMORY)
#define CONFIG_SYS_DMA_ALLOC_LEN		SZ_16M /* Reserve a region of memory for DMA */

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_PROMPT              "S3 # "
#define CONFIG_SYS_CBSIZE              512             /* Console I/O Buffer Size      */
#define CONFIG_SYS_PBSIZE              (CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16) /* Print Buffer Size */
#define CONFIG_SYS_MAXARGS             16              /* max number of command args   */

#define CONFIG_SYS_LOAD_ADDR		0x1000000	/* default load address */


#define CONFIG_SYS_INIT_RAM_SIZE        (250 * 1024)
#define CONFIG_SYS_INIT_RAM_ADDR        (CONFIG_SYS_TEXT_BASE - \
					 CONFIG_SYS_INIT_RAM_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR         (CONFIG_SYS_INIT_RAM_ADDR + \
					 CONFIG_SYS_INIT_RAM_SIZE)


#ifndef FPGA_BRINGUP
/* nand-related commands */
/*--------------------------------------------------------------------------------------
 * Fastboot settings
 */
/*
 * fastboot related configurations
 */
#define CONFIG_FASTBOOT				1 /* Android fast boot */
/* Another macro may also be used or instead used to take care of the case
 * where fastboot is started at boot (to be incorporated) based on key press
 */
#define	CONFIG_CMD_FASTBOOT
#define	CONFIG_FASTBOOT_TRANSFER_BUFFER		(CONFIG_SYS_MEMTEST_START + SZ_16M)
#define	CONFIG_FASTBOOT_TRANSFER_BUFFER_SIZE	(SZ_512M - SZ_16M)
/* Fastboot product name */
#define	FASTBOOT_PRODUCT_NAME			"dtv-elite"
/* Fastboot reboot paramenter address, it's currently put at
 * PUBLIC_SAR_RAM1_FREE
 */
#define FASTBOOT_REBOOT_PARAMETER_ADDR		(0x4a326000 + 0xA0C)
/* Address of the kernel's ramconsole so we can dump it.  This is
 * used by the 'fastboot oem kmsg' command.  It needs to be done
 * early in fastboot (before large amount of transfer buffer is used,
 * since they overlap).
 */
#define CONFIG_FASTBOOT_RAMCONSOLE_START	(CONFIG_SYS_MEMTEST_START + SZ_512M)

/* device to use */
#define FASTBOOT_BLKDEV				"mmc1"


#define CONFIG_USB_DEVICE			1
#define CONFIG_ELITE_UDC			1

/* Change this to suit your needs */
#define CONFIG_USBD_VENDORID			0x18D1
#define CONFIG_USBD_PRODUCTID			0x0D02
#define CONFIG_USBD_MANUFACTURER		"VIA"
#define CONFIG_USBD_PRODUCT_NAME		"Elite2000"

/* should be define in UDC driver header file */
#define EP0_MAX_PACKET_SIZE			64
/*
 * End of fastboot related configurations
 */

#endif //#if FPGA_BRINGUP

#endif /* __CONFIG_H */
