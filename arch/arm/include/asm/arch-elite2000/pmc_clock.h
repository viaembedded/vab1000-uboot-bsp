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

#ifndef __INCLUDE_CLOCK_H__
#define __INCLUDE_CLOCK_H__

void init_pmc_clock(void);
unsigned long clk_get_parent_rate(const char *dev_name);
int clk_set_parent_rate(const char *dev_name, unsigned long rate);
unsigned long clk_get_rate(const char *dev_name);
int clk_set_rate(const char *dev_name, unsigned long rate);
void clk_disable(const char *dev_name);
void clk_enable(const char *dev_name);

/**
 * @ c = 1: power down PCIE
 * @ c = 0: power up PCIE
 */
void pcie_set_pwr_ctrl(int c);
/**
 * @ p = 0: get PCIE0 status
 * @ p = 1: get PCIE1 status
 * ret: 1: power down; 0: power up
 */
int pcie_get_pwr_stat(unsigned int p);

#endif //__INCLUDE_CLOCK_H__
