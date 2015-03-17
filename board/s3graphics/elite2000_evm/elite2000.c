/*
 * board.c
 *
 * Board functions for S3 Elite2000 based boards
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
#include "elite2000.h"

DECLARE_GLOBAL_DATA_PTR;

static int force_fastboot = 0;

const elite_sysinfo sysinfo = {
	"ELITE2000 EVM",
	"FPGA",
};


/*
 * early system init of muxing and clocks.
 */
void s_init(void)
{
	//init_pmc_clock();
	//gpio_init();
	//i2c_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);
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

/* useless in NAND */
struct fbt_partition fbt_partitions[] = {
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
#endif //CONFIG_FASTBOOT

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

#ifdef CONFIG_FASTBOOT
	fbt_preboot();
#endif 

	return 0;
}
#endif

int board_get_enetaddr(u8 *mac_addr)
{
	return 0;
}


int board_eth_init(bd_t *bis)
{
	//return pci_eth_init(bis);
}

