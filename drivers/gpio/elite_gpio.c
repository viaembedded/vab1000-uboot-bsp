/*
 * Copyright (C) S3 Graphics, Inc. 2013.
 * Author: OpenGL-Linux-Platform
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
 *
 * with the reference on libata and ahci drvier in kernel
 *
 */
#include <common.h>

#include <command.h>
#include <asm/processor.h>
#include <asm/errno.h>
#include <asm/io.h>
#include <asm/arch/gpio.h>
#include <malloc.h>
#include <linux/ctype.h>


void *regs_base = 0xd8110000;

static inline void elite_gpio_writeb(u32 val, u32 reg)
{
	__raw_writeb(val, regs_base + reg);
}

static inline u8 elite_gpio_readb(u32 reg)
{
	return __raw_readb(regs_base + reg);
}

static inline void elite_gpio_writel(u32 val, u32 reg)
{
	__raw_writel(val, regs_base + reg);
}

static inline u32 elite_gpio_readl(u32 reg)
{
	return __raw_readl(regs_base + reg);
}

static void elite_gpio_bit_writeb(u32 reg, int gpio, int value)
{
	u8 val;

	val = elite_gpio_readb(reg);
	if(value)
		elite_gpio_writeb((1 << REG_BIT(gpio)) | val, reg);
	else
		elite_gpio_writeb((~(1 << REG_BIT(gpio))) & val, reg);
}

static int elite_gpio_bit_readb(u32 reg, int gpio)
{
	return (elite_gpio_readb(reg) >> REG_BIT(gpio)) & 0x01;
}

int gpio_set_value(unsigned gpio, int value)
{
	elite_gpio_bit_writeb(REG_OUTPUT_DAT(gpio), gpio, value);
}

int gpio_get_value(unsigned gpio)
{
	return elite_gpio_bit_readb(REG_INPUT_DAT(gpio), gpio);
}

int gpio_direction_input(unsigned gpio)
{
	elite_gpio_bit_writeb(REG_OUTPUT_EN(gpio), gpio, 0);
	return 0;
}

int gpio_direction_output(unsigned gpio, int value)
{
	elite_gpio_bit_writeb(REG_OUTPUT_EN(gpio), gpio, 1);
	return 0;
}

static inline int gpio_valid(int gpio)
{
	if(gpio < 0)
		return -EINVAL;
	if(gpio < 145)
		return 0;
	return -EINVAL;
}

static int check_gpio(int gpio)
{
	if (gpio_valid(gpio) < 0) {
		printf("ERROR: check_gpio: invalid GPIO %d\n", gpio);
		return -EINVAL;
	}
	return 0;
}

/**
 * Request a gpio before using it
 *
 * NOTE: Argument 'label' is unused
 */
int gpio_request(unsigned gpio, const char *label)
{
	if (check_gpio(gpio) < 0)
		return -EINVAL;
	return 0;
}

/**
 * Reset and free the gpio after using it
 *
 * 1) Request target gpio
 * 2) Get/Set/Toggle target gpio
 * 3) Free target gpio
 */
void gpio_free(unsigned gpio)
{
}

////////////////////////////////////////////////////////////////////////////////////
// gpio pin-control driver

void elite_gpio_func_en(unsigned gpio, int value)
{
	elite_gpio_bit_writeb(REG_ENABLE(gpio), gpio, value);
}

void elite_gpio_pullupdown_en(unsigned gpio, int value)
{
	/* 
	 * map from range between gpio488 and gpio495 to range between gpio16 and gpio23
	 * due to the bad design of GPIO module
	 */
	if((gpio >= 16) && (gpio <= 23)) 
		gpio += 472;
	
	elite_gpio_bit_writeb(REG_PULL_UP_DOWN_EN(gpio), gpio, value);
}

void elite_gpio_pullupdown_ctrl0_set(unsigned gpio, enum elite_pinconf_pull value)
{
	if((gpio >= 16) && (gpio <= 23)) 
		gpio += 472;

	elite_gpio_bit_writeb(REG_PULL_UP_DOWN_CTRL0(gpio), gpio, value);
}

u8 elite_gpio_pullupdown_ctrl0_get(unsigned gpio)
{
	if((gpio >= 16) && (gpio <= 23)) 
		gpio += 472;

	return elite_gpio_bit_readb(REG_PULL_UP_DOWN_CTRL0(gpio), gpio);
}

void elite_gpio_pinmux_set(unsigned int value)
{
	elite_gpio_writel(value, REG_PIN_SHARE);
}

u32 elite_gpio_pinmux_get(void)
{
	return elite_gpio_readl(REG_PIN_SHARE);
}

void __sd0_drive_strength(u32 reg, enum elite_pinconf_drvstrength drv)
{
	u8 value = (drv << 3) | drv;
	elite_gpio_writeb(value, reg);
}

void elite_gpio_sd0_drive_strength_set(enum elite_pinconf_drvstrength drv)
{
	u32 reg;
	
	for(reg= 0; reg < REG_PAD_DRV_REG_NUM; reg++) 
		__sd0_drive_strength(reg + REG_PAD_SD0_DRV_BASE, drv);
}

u8 elite_gpio_sd0_drive_strength_get(void)
{
	return elite_gpio_readb(REG_PAD_SD0_DRV_BASE) & 0x07;
}

void elite_gpio_sd0_signal_voltage_set(
	enum elite_pinconf_voltage volt)
{
	elite_gpio_writeb(volt, REG_PAD_SIG_VOLT);
}

u8 elite_gpio_sd0_signal_voltage_get(void)
{
	return elite_gpio_readb(REG_PAD_SIG_VOLT) & 0x01;
}

void elite_gpio_usb_mode_set(enum elite_pinconf_usb_mode mode)
{
	elite_gpio_writel((mode & 3) << 16, REG_USB_OP_MODE);
}

u32 elite_gpio_usb_mode_get(void)
{
	return (elite_gpio_readl(REG_USB_OP_MODE) >> 16) & 3;
}


