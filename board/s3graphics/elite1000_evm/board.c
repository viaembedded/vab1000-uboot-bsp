/*
 * board.c
 *
 * Board functions for S3 Elite1k based boards
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

#include <common.h>
#include <errno.h>
#include <libfdt.h>
#include <netdev.h>
#include <fastboot.h>
#include <asm/arch/elite.h>
#include <asm/arch/pmc_clock.h>
#include <asm/arch/gpio.h>
#include <asm/arch/mmc_host_defs.h>
#include <asm/arch/sys_proto.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <i2c.h>
#include <miiphy.h>
#include "board.h"

DECLARE_GLOBAL_DATA_PTR;

static int force_fastboot = 0;

const elite_sysinfo sysinfo = {
	"ELITE1000 EVM",
	"NAND",
};

//elite1k-520016e-BreakSong-01-start
#include "fill_driving.h"
#ifdef USE_SATA_GEN1
#include "A900_driving_table.h"
#else
#include "none_driving_table.h"
#endif
static unsigned int driving_map_size= sizeof(driving_map);
//elite1k-520016e-BreakSong-01-end

static void gpio_init(void)
{
	int i;

#ifdef CONFIG_USE_EMMC
	/* Select SD2, KPAD, etc */
	elite_gpio_pinmux_set(1<<10);
#else
	/* Select NAND, KPAD, etc */
	elite_gpio_pinmux_set(0);
#endif

	/* serial flash config */
	elite_gpio_func_en(73, 0); //DI
	elite_gpio_pullupdown_en(73, 1);
	elite_gpio_pullupdown_ctrl0_set(73, ELITE_PINCONF_PULL_DOWN);
	elite_gpio_func_en(74, 0); //DO
	elite_gpio_pullupdown_en(74, 1);
	elite_gpio_pullupdown_ctrl0_set(74, ELITE_PINCONF_PULL_DOWN);
	elite_gpio_func_en(77, 0); //CLK
	elite_gpio_pullupdown_en(77, 1);
	elite_gpio_pullupdown_ctrl0_set(77, ELITE_PINCONF_PULL_DOWN);
	elite_gpio_func_en(75, 0); //CS0N
	elite_gpio_pullupdown_en(75, 1);
	elite_gpio_pullupdown_ctrl0_set(75, ELITE_PINCONF_PULL_UP);
	elite_gpio_func_en(76, 0); //CS1N
	elite_gpio_pullupdown_en(76, 1);
	elite_gpio_pullupdown_ctrl0_set(76, ELITE_PINCONF_PULL_UP);

	/* uart1 */
	elite_gpio_func_en(93, 0); //TXD
	elite_gpio_func_en(94, 0); //RXD
	elite_gpio_pullupdown_en(93, 1);
	elite_gpio_pullupdown_en(94, 1);
	elite_gpio_pullupdown_ctrl0_set(93, ELITE_PINCONF_PULL_UP); //TXD
	elite_gpio_pullupdown_ctrl0_set(94, ELITE_PINCONF_PULL_DOWN); //RXD

	/* PCIE */
	elite_gpio_func_en(40, 0); //RST0Z
	elite_gpio_pullupdown_en(40, 1);
	elite_gpio_pullupdown_ctrl0_set(40, ELITE_PINCONF_PULL_DOWN);
	elite_gpio_func_en(41, 0); //RST1Z
	elite_gpio_pullupdown_en(41, 1);
	elite_gpio_pullupdown_ctrl0_set(41, ELITE_PINCONF_PULL_DOWN);
	elite_gpio_func_en(42, 0); //REQ0Z
	elite_gpio_pullupdown_en(42, 1);
	elite_gpio_pullupdown_ctrl0_set(42, ELITE_PINCONF_PULL_UP);
	elite_gpio_func_en(16, 1); //wakeup0(As GPIO)
	elite_gpio_pullupdown_en(16, 1);
	elite_gpio_pullupdown_ctrl0_set(16, ELITE_PINCONF_PULL_UP);
	gpio_direction_input(16);
	elite_gpio_func_en(17, 1); //wakeup1(As GPIO)
	elite_gpio_pullupdown_en(17, 1);
	elite_gpio_pullupdown_ctrl0_set(17, ELITE_PINCONF_PULL_UP);
	gpio_direction_input(17);

#ifdef CONFIG_USE_EMMC
	//sdmmc2 (eMMC)
	/* clk pin */
	elite_gpio_func_en(43, 0);
	elite_gpio_pullupdown_en(43, 1);
	elite_gpio_pullupdown_ctrl0_set(43, ELITE_PINCONF_PULL_UP);//CLK
	/* cmd pin */
	elite_gpio_func_en(44, 0);
	elite_gpio_pullupdown_en(44, 1);
	elite_gpio_pullupdown_ctrl0_set(44, ELITE_PINCONF_PULL_UP);//CMD
	/* WP pin */
	elite_gpio_func_en(49, 0);
	elite_gpio_pullupdown_en(49, 1);
	elite_gpio_pullupdown_ctrl0_set(49, ELITE_PINCONF_PULL_UP);//WP
	/* SD2 pin config */
	elite_gpio_func_en(50, 0);
	elite_gpio_pullupdown_en(50, 1);
	elite_gpio_pullupdown_ctrl0_set(50, ELITE_PINCONF_PULL_UP);//CD
	/* Power up SD card connected to SDC2 */
	elite_gpio_func_en(48, 1); //As GPIO
	elite_gpio_pullupdown_en(48, 1);
	elite_gpio_pullupdown_ctrl0_set(48, ELITE_PINCONF_PULL_UP);
	gpio_direction_output(48, 0);
	for (i = 56; i < 64; i++) { //data
		elite_gpio_func_en(i, 0);
		elite_gpio_pullupdown_en(i, 1);
		elite_gpio_pullupdown_ctrl0_set(i, ELITE_PINCONF_PULL_UP);
	}
#elif defined(CONFIG_USE_NAND)
	/* NAND pin config */
	for (i = 56; i < 64; i++) {
		elite_gpio_func_en(i, 0);
		elite_gpio_pullupdown_en(i , 1);
		elite_gpio_pullupdown_ctrl0_set(i, ELITE_PINCONF_PULL_DOWN);//data0~data7
	}
	elite_gpio_func_en(43, 0);
	elite_gpio_pullupdown_en(43, 1);
	elite_gpio_pullupdown_ctrl0_set(43, ELITE_PINCONF_PULL_UP);//ale
	elite_gpio_func_en(44, 0);
	elite_gpio_pullupdown_en(44, 1);
	elite_gpio_pullupdown_ctrl0_set(44, ELITE_PINCONF_PULL_DOWN);//cmd
	elite_gpio_func_en(45, 0);
	elite_gpio_pullupdown_en(45, 1);
	elite_gpio_pullupdown_ctrl0_set(45, ELITE_PINCONF_PULL_UP);//WE
	elite_gpio_func_en(46, 0);
	elite_gpio_pullupdown_en(46, 1);
	elite_gpio_pullupdown_ctrl0_set(46, ELITE_PINCONF_PULL_UP);//RE
	elite_gpio_func_en(47, 0);
	elite_gpio_pullupdown_en(47, 1);
	elite_gpio_pullupdown_ctrl0_set(47, ELITE_PINCONF_PULL_DOWN);//DQS
	elite_gpio_func_en(48, 0);
	elite_gpio_pullupdown_en(48, 1);
	elite_gpio_pullupdown_ctrl0_set(48, ELITE_PINCONF_PULL_UP);//WP
	elite_gpio_func_en(49, 0);
	elite_gpio_pullupdown_en(49 , 1);
	elite_gpio_pullupdown_ctrl0_set(49, ELITE_PINCONF_PULL_UP);//WPDET
	elite_gpio_func_en(50, 0);
	elite_gpio_pullupdown_en(50 , 1);
	elite_gpio_pullupdown_ctrl0_set(50, ELITE_PINCONF_PULL_UP);//RB0
	elite_gpio_func_en(51, 0);
	elite_gpio_pullupdown_en(51 , 1);
	elite_gpio_pullupdown_ctrl0_set(51, ELITE_PINCONF_PULL_UP);//RB1
	elite_gpio_func_en(52, 0);
	elite_gpio_pullupdown_en(52, 1);
	elite_gpio_pullupdown_ctrl0_set(52, ELITE_PINCONF_PULL_UP);//CE0
	elite_gpio_func_en(53, 0);
	elite_gpio_pullupdown_en(53, 1);
	elite_gpio_pullupdown_ctrl0_set(53, ELITE_PINCONF_PULL_UP);//CE1
#endif //CONFIG_USE_EMMC

	/* SD0 pin config */
	elite_gpio_func_en(28, 0);
	elite_gpio_pullupdown_en(28, 1);
	elite_gpio_pullupdown_ctrl0_set(28, ELITE_PINCONF_PULL_UP);//CD
	/* Power up SD card connected to SDC0 */
	elite_gpio_func_en(32, 1); //As GPIO
	elite_gpio_pullupdown_en(32, 1);
	elite_gpio_pullupdown_ctrl0_set(32, ELITE_PINCONF_PULL_UP);
	gpio_direction_output(32, 0);
	for (i = 33; i < 40; i++) {
		elite_gpio_func_en(i, 0);
		elite_gpio_pullupdown_en(i, 1);
		elite_gpio_pullupdown_ctrl0_set(i, ELITE_PINCONF_PULL_UP);
	}

	/* SD1 pin config */
	/* Power up SD card connected to SDC1 */
	elite_gpio_func_en(64, 1); //As GPIO
	elite_gpio_pullupdown_en(64, 1);
	elite_gpio_pullupdown_ctrl0_set(64, ELITE_PINCONF_PULL_UP);
	gpio_direction_output(64, 0);
	for (i = 65; i < 73; i++) {
		elite_gpio_func_en(i, 0);
		elite_gpio_pullupdown_en(i, 1);
		elite_gpio_pullupdown_ctrl0_set(i, ELITE_PINCONF_PULL_UP);
	}

	/* key pad pin config */
	for (i = 80; i < 88; i++) {
		elite_gpio_func_en(i, 0);
		elite_gpio_pullupdown_en(i, 1);
		elite_gpio_pullupdown_ctrl0_set(i, ELITE_PINCONF_PULL_DOWN);
	}
}

/*
 * early system init of muxing and clocks.
 */
void s_init(void)
{
	init_pmc_clock();
	gpio_init();
	i2c_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);
}

/*
 * Basic board specific setup.  Pinmux has been handled already.
 */
int board_init(void)
{
#ifdef CONFIG_OF_LIBFDT
	gd->bd->bi_arch_number = ~0;
#else
	gd->bd->bi_arch_number = MACH_TYPE_ELITE1000;
#endif

	/* adress of boot parameters */
	gd->bd->bi_boot_params = 0x00000100 + RESERVED_MEMORY;

	return 0;
}

#ifdef CONFIG_FASTBOOT
int board_fbt_key_pressed(void)
{
	int is_pressed = 0;

	if (force_fastboot) {
		printf("Forcing fastboot\n");
		return 1;
	}

	/* check for the mute key to be pressed as an indicator
	 * to enter fastboot mode in preboot mode.
	 */
	while(1) {
		break;
	}
	
	printf("Returning key pressed %s\n", is_pressed ? "true" : "false");
	return is_pressed;
}

/* we only check for a long press of mute as an indicator to
 * go into recovery.
 */
enum fbt_reboot_type board_fbt_key_command(void)
{
	return FASTBOOT_REBOOT_NONE;
	//return FASTBOOT_REBOOT_RECOVERY_WIPE_DATA;
}

void board_fbt_start(void)
{
	/* Light up LEDs or something */
}

void board_fbt_end(void)
{
}

/* eMMC partition definitions */
struct fbt_partition {
	const char *name;
	const char *type;
	unsigned size_kb;
};

/* For the 16GB eMMC part used in elite100 SoC board, the erase group size is 512KB.
 * So every partition should be at least 512KB to make it possible to use
 * the mmc erase operation when doing 'fastboot erase'.
 * However, the xloader is an exception because in order for the OMAP4 ROM
 * bootloader to find it, it must be at offset 0KB, 128KB, 256KB, or 384KB.
 * Since the partition table is at 0KB, we choose 128KB.  Special care
 * must be kept to prevent erase the partition table when/if the xloader
 * partition is being erased.
 */
struct fbt_partition fbt_partitions[] = {
	{ "--ptable", NULL,  17},  /* partition table in
					* first 34 sectors */
	{ "environment", "raw", 95 },  /* partition used to u-boot environment,
					* which is also where we store
					* oem lock/unlock state.  size
					* must match CONFIG_ENV_SIZE.
					*/
	{ "crypto", "raw", 16},        /* 16KB partition for crypto keys.
					* used when userdata is encrypted.
					*/
	{ "bl2", "raw", 384 },	/* must start at 128KB offset into eMMC
					 * for ROM bootloader to find it.
					 * pad out to fill whole erase group */
	{ "bootloader", "raw", 512 },  /* u-boot, one erase group in size */
	{ "device_info", "raw", 512 }, /* device specific info like MAC
					* addresses.  read-only once it has
					* been written to.  bootloader parses
					* this at boot and sends the contents
					* to the kernel via cmdline args.
					*/
//;elite1k-520016c-YSW-07	{ "bootloader2", "raw", 512 }, /* u-boot, alternate copy */
//;elite1k-520016c-YSW-07 - start
	{ "devicetree", "raw", 512 }, /* u-boot, alternate copy,temp used for devicetree */
//;elite1k-520016c-YSW-07 - end
	{ "misc", "raw", 512 }, 	/* misc partition used by recovery for
					 * storing parameters in the case of a
					 * power failure during recovery
					 * operation.
					 */
	{ "recovery", "boot", 8*1024 },
	{ "boot", "boot", 8*1024 },
	{ "efs", "ext4", 8*1024 },      /* for factory programmed keys,
					 * minimum size for a ext4 fs is
					 * about 8MB
					 */
//;elite1k-520016c-YSW-14	{ "system", "ext4", 1024*1024 },
//;elite1k-520016c-YSW-14	{ "cache", "ext4", 512*1024 },
//;elite1k-520016c1-YSW-01 //;elite1k-520016c-YSW-14 - start
//;elite1k-520016c1-YSW-01	{ "system", "ext4", 512 },
//;elite1k-520016c1-YSW-01	{ "cache", "ext4", 512 },
//;elite1k-520016c1-YSW-01 //;elite1k-520016c-YSW-14 - end
//;elite1k-520016c1-YSW-01 - start
	{ "system", "ext4", 512*1024 },
	{ "cache", "ext4", 256*1024 },
//;elite1k-520016c1-YSW-01 - end
	{ "userdata", "ext4", 0},
	{ 0, 0, 0 },
};

void board_fbt_finalize_bootargs(char* args, size_t buf_sz) {
	int used = strlen(args);

	/* Add board_id */
	used += snprintf(args + used,
			 buf_sz - used,
			 " board_dtv.dtv_hw_rev=0x100");

	args[buf_sz-1] = 0;

	/* this is called just before booting normal image.  we
	 * use opportunity to start boot animation.
	 */
	board_fbt_end();
}

void board_turn_off_display()
{
	*(volatile unsigned int*)REG_DISPLAY_ONOFF = 1;
}

void board_turn_on_display()
{
	*(volatile unsigned int*)REG_DISPLAY_ONOFF = 0;
}

int is_display_on(void)
{
	return !(*(volatile unsigned int*)REG_DISPLAY_ONOFF);
}

void board_set_display()
{
	if (0x1) {
		board_turn_on_display();
	} else {
		board_turn_off_display();
	}
}

int abort_fastboot(int bootdelay)
{
	int abort = 0;
	printf("press any key to abort fastboot!\n");
	while ((bootdelay > 0) && (!abort)) {
		int i;

		--bootdelay;
		/* delay 100 * 10ms */
		for (i=0; !abort && i<100; ++i) {
			if (tstc()) {	/* we got a key press	*/
				abort  = 1;	/* don't auto boot	*/
				bootdelay = 0;	/* no more delay	*/
# ifdef CONFIG_MENUKEY
				menukey = getc();
# else
				(void) getc();  /* consume input	*/
# endif
				break;
			}
			udelay(10000);
		}

		printf("\b\b\b%2d ", bootdelay);
	}

	putc('\n');

	return abort;
}

enum fbt_reboot_type board_get_boot_method(void)
{
/*
 * The Kernel use Vsus power domain GENERAL_CFG1_WAKEUP register
 * to pass booting message to bootloader
 */
#define PMU_GENERAL_CFG1_WAKEUP 0xd839c06c
	enum fbt_reboot_type frt;

	frt = FASTBOOT_REBOOT_NORMAL;

	switch (readl((void*)PMU_GENERAL_CFG1_WAKEUP)) {
	case 0:
		frt = FASTBOOT_REBOOT_NORMAL;
		break;
	case 2:
		frt = FASTBOOT_REBOOT_RECOVERY;
		break;
	default:
		frt = FASTBOOT_REBOOT_NORMAL;
		break;
	}

	if (!fbt_check_bootloader_message())
		frt = FASTBOOT_REBOOT_RECOVERY;

	return  frt;

}
#endif

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
#ifdef CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
	char safe_string[HDR_NAME_LEN + 1];

	/* Now set variables based on the header. */
	strncpy(safe_string, (char *)header.name, sizeof(header.name));
	safe_string[sizeof(header.name)] = 0;
	setenv("board_name", safe_string);

	strncpy(safe_string, (char *)header.version, sizeof(header.version));
	safe_string[sizeof(header.version)] = 0;
	setenv("board_rev", safe_string);
#endif

//elite1k-520016e-BreakSong-01-start
	fill_driving_value(SATA_PCI_BASE, driving_map_size, driving_map);
//elite1k-520016e-BreakSong-01-end

#ifdef CONFIG_FASTBOOT
	if (!abort_fastboot(1)) {
//;elite1k-520016c-YSW-03		fbt_preboot();
		fbt_preboot();   //;elite1k-520016c-YSW-09
	}
#endif

	return 0;
}
#endif

int board_show_bootlogo(void)
{
	board_set_display();

#ifdef CONFIG_VIDEO_S3G
	if (is_display_on())
		s3g_init_adapter();
#endif
}

int board_get_enetaddr(u8 *mac_addr)
{
	return 0;
}


int board_eth_init(bd_t *bis)
{
	return pci_eth_init(bis);
}

