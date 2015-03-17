/*
 * Copyright (C) 2012 S3 Graphics, Inc
 *
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <common.h>
#include <serial.h>
#include <linux/compiler.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <asm/arch/elite.h>
#include <asm/arch/uart.h>
#include <asm/arch/pmc_clock.h>

DECLARE_GLOBAL_DATA_PTR;

#if CONFIG_CONS_INDEX == 1
PUART_REG	pUart_Reg = (PUART_REG)ELITE_UART0_BASE;
#elif CONFIG_CONS_INDEX == 2
PUART_REG	pUart_Reg = (PUART_REG)ELITE_UART1_BASE;
#endif

int elite_serial_init(void)
{
#if CONFIG_CONS_INDEX == 1
	clk_enable("uart0");
#elif CONFIG_CONS_INDEX == 2
	clk_enable("uart1");
#endif

	serial_setbrg();

	return 0;
}


void elite_serial_putc(const char c)
{
	while (1) {
		if ((pUart_Reg->URUSR & URUSR_TXDBSY) == 0)
		break;
	}

	pUart_Reg->URTDR = c;
	if (c == '\n')
		serial_putc('\r');
}


void elite_serial_puts(const char *s)
{
	while (*s)
		serial_putc(*s++);
}


int elite_serial_getc(void)
{
	while (!(pUart_Reg->URUSR & URUSR_RXDRDY))
		;

	return pUart_Reg->URRDR & 0xff;
}


int elite_serial_tstc(void)
{
	return (pUart_Reg->URUSR & URUSR_RXDRDY) ? 1 : 0;
}


void elite_serial_setbrg(void)
{
	unsigned long TmpDiv = 0;
	unsigned long TmpBkr = 0;

	switch (gd->baudrate) {
	/*
	case BR_921K6:
		TmpDiv = UART_BR_921K6;
		TmpBkr = UART_BK_921K6;
		break;

	case BR_460K8:
		TmpDiv = UART_BR_460K8;
		TmpBkr = UART_BK_460K8;
		break;

	case BR_230K4:
		TmpDiv = UART_BR_230K4;
		TmpBkr = UART_BK_230K4;
		break;
	*/

	case BR_57K6:
		TmpDiv = UART_BR_57K6;
		TmpBkr = UART_BK_57K6;
		break;
	/*
	case BR_28K8:
		TmpDiv = UART_BR_28K8;
		TmpBkr = UART_BK_28K8;
		break;

	case BR_14K4:
		TmpDiv = UART_BR_14K4;
		TmpBkr = UART_BK_14K4;
		break;

	case BR_7K2:
		TmpDiv = UART_BR_7K2;
		TmpBkr = UART_BK_7K2;
		break;

	case BR_3K6:
		TmpDiv = UART_BR_3K6;
		TmpBkr = UART_BK_3K6;
		break;
	*/

	case BR_38K4:
		TmpDiv = UART_BR_38K4;
		TmpBkr = UART_BK_38K4;
		break;

	case BR_19K2:
		TmpDiv = UART_BR_19K2;
		TmpBkr = UART_BK_19K2;
		break;

	case BR_9K6:
		TmpDiv = UART_BR_9K6;
		TmpBkr = UART_BK_9K6;
		break;

	case BR_115K2:
	default:
		TmpDiv = UART_BR_115K2;
		TmpBkr = UART_BK_115K2;
		break;
	}

	pUart_Reg->URDIV = TmpDiv;
	pUart_Reg->URBKR = TmpBkr;

	/* Disable TX,RX */
	pUart_Reg->URLCR = 0;

	/* Disable all interrupt */
	pUart_Reg->URIER = 0;

	/* Reset Tx,Rx Fifo until all zeros */
	while (pUart_Reg->URFCR) {
		/* Reset TX,RX Fifo */
		pUart_Reg->URFCR = URFCR_TXFRST | URFCR_RXFRST;
	}

	/* Disable Fifo */
	pUart_Reg->URFCR &= (~URFCR_FIFOEN);

	/* 8-N-1 */
	pUart_Reg->URLCR |= (URLCR_DLEN8B & ~URLCR_STBLEN2b & ~URLCR_PYTEN);

	/* Enable TX,RX */
	pUart_Reg->URLCR |= URLCR_RXEN | URLCR_TXEN;
}


static struct serial_device elite_serial_drv = {
	.name	= "elite_serial",
	.start	= elite_serial_init,
	.stop	= NULL,
	.setbrg	= elite_serial_setbrg,
	.putc	= elite_serial_putc,
	.puts	= elite_serial_puts,
	.getc	= elite_serial_getc,
	.tstc	= elite_serial_tstc,
};

void elite_serial_initialize(void)
{
	serial_register(&elite_serial_drv);
}

__weak struct serial_device *default_serial_console(void)
{
	return &elite_serial_drv;
}
