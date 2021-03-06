/*
 * (C) Copyright 2012
 * Dirk Eibach,  Guntermann & Drunck GmbH, dirk.eibach@gdsys.cc
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

/* Parade Technologies Inc. DP501 DisplayPort DVI/HDMI Transmitter */

#include <common.h>
#include <asm/io.h>
#include <errno.h>
#include <i2c.h>

static void dp501_setbits(u8 addr, u8 reg, u8 mask)
{
	u8 val;

	val = i2c_reg_read(addr, reg);
	setbits_8(&val, mask);
	i2c_reg_write(addr, reg, val);
}

static void dp501_clrbits(u8 addr, u8 reg, u8 mask)
{
	u8 val;

	val = i2c_reg_read(addr, reg);
	clrbits_8(&val, mask);
	i2c_reg_write(addr, reg, val);
}

static int dp501_detect_cable_adapter(u8 addr)
{
	u8 val = i2c_reg_read(addr, 0x00);

	return !(val & 0x04);
}

static void dp501_link_training(u8 addr)
{
	u8 val;

	val = i2c_reg_read(addr, 0x51);
	i2c_reg_write(addr, 0x5d, val); /* set link_bw */
	val = i2c_reg_read(addr, 0x52);
	i2c_reg_write(addr, 0x5e, val); /* set lane_cnt */
	val = i2c_reg_read(addr, 0x53);
	i2c_reg_write(addr, 0x5c, val); /* set downspread_ctl */

	i2c_reg_write(addr, 0x5f, 0x0d); /* start training */
}

void dp501_powerup(u8 addr)
{
	dp501_clrbits(addr, 0x0a, 0x30); /* power on encoder */
	i2c_reg_write(addr, 0x27, 0x30); /* Hardware auto detect DVO timing */
	dp501_setbits(addr, 0x72, 0x80); /* DPCD read enable */
	dp501_setbits(addr, 0x30, 0x20); /* RS polynomial select */
	i2c_reg_write(addr, 0x71, 0x20); /* Enable Aux burst write */
	dp501_setbits(addr, 0x78, 0x30); /* Disable HPD2 IRQ */
	dp501_clrbits(addr, 0x2f, 0x40); /* Link FIFO reset selection */
	i2c_reg_write(addr, 0x24, 0xc0); /* SDR mode 0, ext. H/VSYNC */
	i2c_reg_write(addr + 2, 0x24, 0x02); /* clock input single ended */

	if (dp501_detect_cable_adapter(addr)) {
		printf("DVI/HDMI cable adapter detected\n");
		i2c_reg_write(addr, 0x5e, 0x04); /* enable 4 channel */
		dp501_clrbits(addr, 0x00, 0x08); /* DVI/HDMI HDCP operation */
	} else {
		printf("no DVI/HDMI cable adapter detected\n");
		i2c_reg_write(addr + 2, 0x00, 0x18); /* driving strength */
		i2c_reg_write(addr + 2, 0x03, 0x06); /* driving strength */
		i2c_reg_write(addr, 0x2c, 0x00); /* configure N value */
		i2c_reg_write(addr, 0x2d, 0x00); /* configure N value */
		i2c_reg_write(addr, 0x2e, 0x0c); /* configure N value */
		i2c_reg_write(addr, 0x76, 0xff); /* clear all interrupt */
		dp501_setbits(addr, 0x78, 0x03); /* clear all interrupt */
		i2c_reg_write(addr, 0x75, 0xf8); /* aux channel reset */
		i2c_reg_write(addr, 0x75, 0x00); /* clear aux channel reset */
		i2c_reg_write(addr, 0x87, 0x70); /* set retry counter as 7 */
		dp501_setbits(addr, 0x00, 0x08); /* for DP HDCP operation */

		dp501_link_training(addr);
	}
}

void dp501_powerdown(u8 addr)
{
	dp501_setbits(addr, 0x0a, 0x30); /* power down encoder, standby mode */
}
