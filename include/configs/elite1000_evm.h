/*
 * elite1k.h - Default configuration for ELITE1000 EVM board.
 *
 * Copyright (C) 2010 S3 Graphics, Inc
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

/* elite1k-520016e-BreakSong-01-start */
//#define USE_SATA_GEN1 1
#define SATA_PCI_BASE 0xd800d000
/* elite1k-520016e-BreakSong-01-end */

/*
 * High Level Configuration Options
 * (easy to change)
 */
#define	CONFIG_SYS_TEXT_BASE	0x3F80000
#define CONFIG_SYS_SOC			"elite1000"
//#define CONFIG_SYS_DCACHE_OFF
#define CONFIG_L2_OFF

#include <asm/arch/elite.h>
#include <asm/sizes.h>

/* partition support */
#define CONFIG_DOS_PARTITION	      /* to support MSFT Master Boot Record */
#ifdef CONFIG_USE_EMMC
//;elite1k-520016c-YSW-07 - start
#define CONFIG_MAX_PARTITION_NUM 13
//;elite1k-520016c-YSW-07 - end
# define CONFIG_EFI_PARTITION
#endif
/*
 * Display CPU and Board information
 */
#define CONFIG_DISPLAY_CPUINFO		1
#define CONFIG_DISPLAY_BOARDINFO	1

#define REG_DISPLAY_ONOFF		0xd8393410

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

/*
 * Hardware drivers
 */
#define CONFIG_ELITE_GPIO

//;elite1k-520016c-YSW-07 - start
#define CONFIG_SYS_MMC_MAX_BLK_COUNT  64
//;elite1k-520016c-YSW-07 - end

/* NAND driver */
#ifndef CONFIG_USE_EMMC
# define CONFIG_SYS_NAND_QUIET_TEST
# define CONFIG_ELITE_NAND		1
# define CONFIG_SYS_NAND_BASE		0xd8009000 
# define CONFIG_SYS_MAX_NAND_DEVICE	1 
# define DEBUG_FPGA 
# define CONFIG_ELITE_NAND_HWECC
# define	CONFIG_SYS_NAND_USE_FLASH_BBT

# define NAND_BYTES_PER_PAGE    4096
# define NAND_OOB_SIZE          218
# define NAND_ECC_MODE          12
#endif

/*
 * select serial console configuration
 */
#define CONFIG_CONS_INDEX		2
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

/* PCI support */
#define CONFIG_ELITE_PCIE
#define CONFIG_SYS_PCI_MEMORY_BUS      0x00000000
#define CONFIG_SYS_PCI_MEM_BUS         0x80000000
#define CONFIG_SYS_PCI_IO_BUS          0x00800000

#define CONFIG_PCI_SCAN_SHOW
#define CONFIG_CMD_PCI_ENUM
#define CONFIG_CMD_PCI
#define CONFIG_PCI

/* i2c support */
#define CONFIG_SYS_I2C_SPEED		100000
#define CONFIG_SYS_I2C_SLAVE		1
#define CONFIG_ELITE_I2C	1
#define CONFIG_HARD_I2C		1
#define CONFIG_CMD_I2C		1

#define CONFIG_VIDEO_S3G		1
//;elite1k-560016c-GLU-10-start
#define CONFIG_VIDEO_LOGO		1
//;elite1k-560016c-GLU-10-end
//;elite1k-320118d-YSW-09 #define CONFIG_VIDEO_LOGO		1
#define CONFIG_SECURITY			1
#define BOOT_LOGO_OFFSET		0x1200000

/* SD/MMC support */
#define CONFIG_MMC
#define CONFIG_CMD_MMC
#define CONFIG_SUPPORT_EMMC_BOOT
#define CONFIG_ELITE_MMC
#define CONFIG_GENERIC_MMC

/* eth support */
#define CONFIG_RTL8168
#define CONFIG_ARP_TIMEOUT		(5*CONFIG_SYS_HZ)
#define CONFIG_CMD_NET
#define CONFIG_NET_MULTI

/* nand-related commands */
#ifndef CONFIG_USE_EMMC
# define CONFIG_CMD_NAND			1
# define CONFIG_MTD_NAND_YAFFS2		1
# define CONFIG_CMD_MTDPARTS		1
# define CONFIG_MTD_DEVICE		1
# define CONFIG_MTD_PARTITIONS		1
#endif
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
#define CONFIG_ANDROIDNO "030a3d8408e49475"
#define CONFIG_BOOTDELAY               3
//;elite1k-520016c-YSW-02 #define CONFIG_BOOTCOMMAND             "ext4load mmc 0:1 0x2800000 uImage; ext4load mmc 0:1 0x6000000 elite1000-nand.dtb; bootm 0x2800000 - 0x6000000"
//;elite1k-520016c-YSW-04 //;elite1k-520016c-YSW-02 - start
//;elite1k-520016c-YSW-04 #ifdef CONFIG_USE_EMMC
//;elite1k-520016c-YSW-04 #define CONFIG_BOOTCOMMAND             "ext4load mmc 0:1 0x2800000 uImage; ext4load mmc 0:1 0x6000000 elite1000-emmc.dtb; bootm 0x2800000 - 0x6000000"
//;elite1k-520016c-YSW-04 #else //CONFIG_USE_NAND
//;elite1k-520016c-YSW-04 #define CONFIG_BOOTCOMMAND             "ext4load mmc 0:1 0x2800000 uImage; ext4load mmc 0:1 0x6000000 elite1000-nand.dtb; bootm 0x2800000 - 0x6000000"
//;elite1k-520016c-YSW-04 #endif
//;elite1k-520016c-YSW-04 //;elite1k-520016c-YSW-02 - end
//;elite1k-520016c-YSW-04 - start
#define CONFIG_BOOTCOMMAND             "run bootargs_mmc\; ext4load mmc 0:1 0x0 scriptcmd\; if iminfo 0x0\; then source 0\; else run bootcmd_mmc\; fi"
//;elite1k-520016c-YSW-04 - end

#define CONFIG_BOOTARGS                "console=ttyS1,115200n8 root=/dev/mmcblk0p1 rootflags=errors=remount-ro,commit=0 rootfstype=ext4 rw init=/init rootdelay=1 androidboot.serialno="CONFIG_ANDROIDNO""

#ifndef CONFIG_SPL_BUILD
//;elite1k-520016c-YSW-04 #define CONFIG_EXTRA_ENV_SETTINGS \
//;elite1k-520016c-YSW-04 	"loadaddr=0x80200000\0" \
//;elite1k-520016c-YSW-04 	"fdtaddr=0x10000000\0" \
//;elite1k-520016c-YSW-04 	"fdt_high=0xffffffff\0" \
//;elite1k-520016c-YSW-04 	"rdaddr=0x81000000\0" \
//;elite1k-520016c-YSW-04 	"bootdir=/boot\0" \
//;elite1k-520016c-YSW-04 	"bootfile=uImage\0" \
//;elite1k-520016c-YSW-04 	"fdtfile=undefined\0" \
//;elite1k-520016c-YSW-04 	"initrd_high=0xffffffff\0" \
//;elite1k-520016c-YSW-04 	"androidno="CONFIG_ANDROIDNO"\0" \
//;elite1k-520016c-YSW-04 	"ethaddr=00:1a:32:b0:12:63\0" \
//;elite1k-520016c-YSW-04 	"kernelargs=console=ttyS1,115200n8 rootdelay=1\0" \
//;elite1k-520016c-YSW-04 	"mtdids=" MTDIDS_DEFAULT "\0" \
//;elite1k-520016c-YSW-04 	"mtdparts=" MTDPARTS_DEFAULT "\0" \
//;elite1k-520016c-YSW-04 	"bootargs_nand=" \
//;elite1k-520016c-YSW-04 		"setenv bootargs ${kernelargs} androidboot.serialno=${androidno} " \
//;elite1k-520016c-YSW-04 		"root=/dev/mtdblock15 rootfstype=yaffs2 init=/init rw\0" \
//;elite1k-520016c-YSW-04 	"bootcmd_nand=" \
//;elite1k-520016c-YSW-04 		"booti boot\0" \
//;elite1k-520016c-YSW-04 	"bootargs_mmc=" \
//;elite1k-520016c-YSW-04 		"setenv bootargs ${kernelargs}  androidboot.serialno=${androidno} " \
//;elite1k-520016c-YSW-04 		"root=/dev/mmcblk0p1 rootwait rootflags=errors=remount-ro,commit=0 rootfstype=ext4 rw ${mtdparts} init=/init rw\0" \
//;elite1k-520016c-YSW-04 	"bootcmd_mmc=" \
//;elite1k-520016c-YSW-04 		"run bootargs_mmc && " \
//;elite1k-520016c-YSW-04 		" " CONFIG_BOOTCOMMAND " \0" \
//;elite1k-520016c-YSW-04 	"nand_setup=" \
//;elite1k-520016c-YSW-04 		"ext4load mmc 0:1 0x2000000 boot_script.uimg; source 0x2000000\0"
//;elite1k-520016c-YSW-04 - start
 #define CONFIG_EXTRA_ENV_SETTINGS \
 	"loadaddr=0x80200000\0" \
 	"fdtaddr=0x10000000\0" \
 	"fdt_high=0xffffffff\0" \
 	"rdaddr=0x81000000\0" \
 	"bootdir=/boot\0" \
 	"bootfile=uImage\0" \
 	"fdtfile=undefined\0" \
 	"initrd_high=0xffffffff\0" \
 	"androidno="CONFIG_ANDROIDNO"\0" \
 	"ethaddr=00:1a:32:b0:12:63\0" \
 	"kernelargs=console=ttyS1,115200n8 rootdelay=1\0" \
 	"mtdids=" MTDIDS_DEFAULT "\0" \
 	"mtdparts=" MTDPARTS_DEFAULT "\0" \
 	"bootargs_default=" CONFIG_BOOTARGS "\0" \
 	"bootcmd_default=" CONFIG_BOOTCOMMAND "\0" \
 	"bootargs_mmc=" \
 		"setenv bootargs ${kernelargs}  androidboot.serialno=${androidno} " \
 		"root=/dev/mmcblk0p1 rootwait rootflags=errors=remount-ro,commit=0 rootfstype=ext4 rw ${mtdparts} init=/init rw\0" \
 	"bootcmd_mmc=" \
 		"run bootargs_mmc && " \
 		"ext4load mmc 0:1 0x6000000 elite1000-emmc.dtb\; ext4load mmc 0:1 0x2800000 uImage\; if iminfo 0x2800000\; then bootm 0x2800000 - 0x6000000\; else run bootcmd_emmc\; fi\0" \
	"bootargs_emmc=" \
		"setenv bootargs ${kernelargs} androidboot.serialno=${androidno} " \
		"root=/dev/mmcblk0p13 rootwait rootflags=errors=remount-ro,commit=0 rootfstype=ext4 rw ${mtdparts} init=/init rw\0" \	
	"bootcmd_emmc=" \
		"run bootargs_emmc && " \
		"ext4load mmc 2:d 0x6000000 elite1000-emmc.dtb\; ext4load mmc 2:d 0x2800000 uImage\; if iminfo 0x2800000\; then bootm 0x2800000 - 0x6000000\; else run bootcmd_nand\; fi\0" \
 	"bootargs_nand=" \
 		"setenv bootargs ${kernelargs} androidboot.serialno=${androidno} " \
 		"root=/dev/mtdblock15 rootfstype=yaffs2 init=/init rw\0" \
 	"bootcmd_nand=" \
 		"run bootargs_nand; setenv bootcmd \"booti boot\"; boot;\0" \		
 	"nand_setup=" \
 		"ext4load mmc 0:1 0x2000000 boot_script.uimg; source 0x2000000\0" \
	"setup_bootloader=" \
		"ext4load mmc 0:1 0x1000 bootloader_setup.uimg; source 0x1000;\0" \
	"setup_nand=" \
		"ext4load mmc 0:1 0x1000 bootloader_setup.uimg; source 0x1000; ext4load mmc 0:1 0x1000 uboot_nand_setup.uimg; source 0x1000;\0" \
	"setup_emmc=" \
		"ext4load mmc 0:1 0x1000 bootloader_setup.uimg; source 0x1000; ext4load mmc 0:1 0x1000 uboot_emmc_setup.uimg; source 0x1000;\0" \
	"other_env=" \
		"ext4load mmc 0:1 0x1000 other_env.uimg\; source 0x1000\;\0" \
//	"boot_mmc=" \
//		"setenv bootargs \"console=ttyS1,115200n8 rootdelay=3 root=/dev/mmcblk0p1 init=/init rw\"; " \
//		"setenv bootcmd \"ext4load mmc 0:1 0x2800000 uImage; bootm 0x2800000 - 0x6000000\"; saveenv; boot;\0" \
//	"boot_nand=" \
//		"setenv bootargs \"console=ttyS1,115200n8 rootdelay=3\"; setenv bootcmd \"booti boot\"; saveenv; boot;\0" \
//	"boot_emmc=" \
//		"setenv bootargs \"console=ttyS1,115200n8 rootdelay=3 root=/dev/mmcblk0p13 init=/init\"; " \
//		"setenv bootcmd \"ext4load mmc 2:d 0x2800000 uImage; bootm 0x2800000 - 0x6000000\"; saveenv; boot;\0" \
//;elite1k-520016c-YSW-04 - end 		
#endif

/* Physical memory map */
#define RESERVED_MEMORY					SZ_32M	/* Reserved memory for DSP */
#define CONFIG_NR_DRAM_BANKS			1       /* CS1 may or may not be populated */
//;elite1k-520016c-YSW-01 #define CONFIG_MAX_RAM_BANK_SIZE 		SZ_2G   /* 2 GB */
//;elite1k-520016c1.d1-GLU-01 #define CONFIG_MAX_RAM_BANK_SIZE 		SZ_2G   /* 1 GB */		//;elite1k-520016c-YSW-01
#define CONFIG_MAX_RAM_BANK_SIZE              SZ_2G   /* 2 GB */ //;elite1k-520016c-GLU-01



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
#ifdef CONFIG_USE_EMMC
#define FASTBOOT_BLKDEV			"mmc2"
#else //CONFIG_USE_NAND
#define FASTBOOT_BLKDEV			"nand0"
#endif


#define CONFIG_USB_DEVICE			1
#define CONFIG_ELITE1000_UDC			1

/* Change this to suit your needs */
#define CONFIG_USBD_VENDORID			0x18D1
#define CONFIG_USBD_PRODUCTID			0x0D02
#define CONFIG_USBD_MANUFACTURER		"VIA"
#define CONFIG_USBD_PRODUCT_NAME		"Elite1k"

/* should be define in UDC driver header file */
#define EP0_MAX_PACKET_SIZE			64
/*
 * End of fastboot related configurations
 */

//;elite1k-320118d-YSW-08 - start
#define CONFIG_CMD_DISPLAY		1
//;elite1k-320118d-YSW-08 - end

#endif /* __CONFIG_H */
