/*
 * (C) Copyright 2013 S3 Graphics
 * Scotty Yi <scottyyi@s3graphics.com>
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

#ifndef __ASM_ARCH_GPIO_H
#define __ASM_ARCH_GPIO_H


/* calculate register offset */
#define REG_INPUT_DAT(x)	       ((x)/8)
#define REG_ENABLE(x)			(((x)/8) + 0x40) //pin mux register
#define REG_OUTPUT_EN(x)		(((x)/8) + 0x80)
#define REG_OUTPUT_DAT(x)		(((x)/8) + 0xc0)
#define REG_PULL_UP_DOWN_EN(x)		(((x)/8) + 0x480)
#define REG_PULL_UP_DOWN_CTRL0(x)	(((x)/8) + 0x4c0)
#define REG_PULL_UP_DOWN_CTRL1(x)	(((x)/8) + 0x5c0)

#define REG_BIT(x)			((x) % 8)
/* registers offset */
#define REG_USB_OP_MODE			0x10c
#define REG_PIN_SHARE			0x200
#define REG_PAD_SD0_DRV_BASE		0x620 //drive strength
#define REG_PAD_DRV_REG_NUM		4
#define REG_PAD_SIG_VOLT		0x624

#ifndef __ASSEMBLY__

enum elite_pinconf_param {
	ELITE_PINCONF_PARAM_FUNC_EN,
	/* arg: elite_pinconf_pull */
	ELITE_PINCONF_PARAM_PULL,
	ELITE_PINCONF_PARAM_DRV,
	ELITE_PINCONF_PARAM_VOLT,
	ELITE_PINCONF_PARAM_USB,
};

enum elite_pinconf_pull {
	ELITE_PINCONF_PULL_NONE = -1,
	ELITE_PINCONF_PULL_DOWN = 0,
	ELITE_PINCONF_PULL_UP = 1,
};

enum elite_pinconf_drvstrength {
	ELITE_PINCONF_PAD_DRV_TYPE_B = 0,
	ELITE_PINCONF_PAD_DRV_TYPE_A,
	ELITE_PINCONF_PAD_DRV_TYPE_C,
	ELITE_PINCONF_PAD_DRV_TYPE_D,
};

enum elite_pinconf_voltage {
	ELITE_PINCONF_PAD_1_8 = 0, //1.8V
	ELITE_PINCONF_PAD_3_3, //3.3v
};

enum elite_pinconf_usb_mode {
	ELITE_PINCONF_DEVICE = 0,
	ELITE_PINCONF_HOST = 1,
	ELITE_PINCONF_IDPIN = 2,
};


void elite_gpio_func_en(unsigned gpio, int value);
void elite_gpio_pullupdown_en(unsigned gpio, int value);
void elite_gpio_pullupdown_ctrl0_set(unsigned gpio, enum elite_pinconf_pull value);
u8 elite_gpio_pullupdown_ctrl0_get(unsigned gpio);
void elite_gpio_pinmux_set(unsigned int value);
u32 elite_gpio_pinmux_get(void);
void elite_gpio_sd0_drive_strength_set(enum elite_pinconf_drvstrength drv);
u8 elite_gpio_sd0_drive_strength_get(void);
void elite_gpio_sd0_signal_voltage_set(enum elite_pinconf_voltage volt);
u8 elite_gpio_sd0_signal_voltage_get(void);
void elite_gpio_usb_mode_set(enum elite_pinconf_usb_mode mode);
u32 elite_gpio_usb_mode_get(void);

#endif

#endif //__ASM_ARCH_GPIO_H

