/*
 * (C) Copyright 2011-2013 S3 Graphics, <www.s3graphics.com>
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

#include <common.h>
#include <mtd/mtd-abi.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/bbm.h>
#include <asm/io.h>
#include <asm/errno.h>
#include <asm/arch/elite.h>

#include "elite_nand.h"

static int hardware_ecc = 1;

extern int ecc_type;
//extern int bootmode;
int bootmode = 0;

extern volatile void *ctrl_base;
extern int nand_dma_config(struct mtd_info *mtd, unsigned int len, unsigned int wr, int data_flag, int nand_bank);

static uint8_t elite_bbt_pattern_2048[] = {'B', 'b', 't', '0'};
static uint8_t elite_mirror_pattern_2048[] = {'1', 't', 'b', 'B'};
static int test_page=0;
static struct nand_ecclayout elite_nand_oobinfo_512 = {
	.eccbytes = 8,
	.eccpos = { 4, 5, 6, 8, 9, 10, 12, 13 },
	.oobfree = {{0, 4}, {7, 1}, {11, 1}, {14, 2}}
};

static struct nand_ecclayout elite_12bit_oobinfo_4096 = {
	/*nand flash old structure and Harming ECC oob info */
	.eccbytes = 20,
	.eccpos = { 24, 25, 26, 27, 28, 29, 30,
			31, 32, 33, 34, 35, 36, 37,
			38, 39, 40, 41, 42, 43},
	.oobavail = 24,
	.oobfree = {{0, 24} }
};

static struct nand_ecclayout elite_nand_8bit_oobinfo_4096 = {
	.eccbytes = 16,
	.eccpos = {24, 25, 26, 27, 28, 29, 30,
			31, 32, 33, 34, 35, 36},
	.oobavail = 24,
	.oobfree = {{0, 24}}
};

static struct nand_ecclayout elite_nand_oobinfo_4096 = {
	.eccbytes = 7,
	.eccpos = {24, 25, 26, 27, 28, 29, 30},
	.oobavail = 24,
	.oobfree = {{0, 24}}
};

static struct nand_ecclayout elite_nand_oobinfo_2048 = {
	.eccbytes = 7,
	.eccpos = {24, 25, 26, 27, 28, 29, 30},
	.oobavail = 24,
	.oobfree = {{0, 24}}
};
static struct nand_ecclayout elite_nand_24bit_oobinfo_8192 = {
	.eccbytes = 42,
	.eccpos = { 24, 25, 26, 27, 28, 29, 30,31, 
	                 32, 33, 34, 35, 36,37,38, 39, 
	                 40, 41, 42, 43,44,45,46, 47, 
	                },
	.oobavail = 24,
	.oobfree = {{0, 24}}
};
static struct nand_ecclayout elite_nand_40bit_oobinfo_8192 = {
	.eccbytes = 70,
	.eccpos = {24, 25, 26, 27, 28, 29, 30,
			31, 32, 33, 34, 35, 36},
	.oobavail = 24,
	.oobfree = {{0, 24}}
};
/* bad block table are stored in the last 4 block of NAND flash */
static struct nand_bbt_descr elite_bbt_main_descr_2048 = {
	.options = NAND_BBT_LASTBLOCK | NAND_BBT_CREATE | NAND_BBT_WRITE
			| NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
	.offs =	4,
	.len = 4,
	.veroffs = 0,
	.maxblocks = 4,
	.pattern = elite_bbt_pattern_2048,
};

static struct nand_bbt_descr elite_bbt_mirror_descr_2048 = {
	.options = NAND_BBT_LASTBLOCK | NAND_BBT_CREATE | NAND_BBT_WRITE
			| NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
	.offs =	4,
	.len = 4,
	.veroffs = 0,
	.maxblocks = 4,
	.pattern = elite_mirror_pattern_2048,
};

static struct nand_bbt_descr elite_bbt_main_descr_512 = {
	.options = NAND_BBT_LASTBLOCK | NAND_BBT_CREATE | NAND_BBT_WRITE
			| NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
	.offs =	0,
	.len = 4,
	.veroffs = 14,
	.maxblocks = 4,
	.pattern = elite_bbt_pattern_2048,
};

static struct nand_bbt_descr elite_bbt_mirror_descr_512 = {
	.options = NAND_BBT_LASTBLOCK | NAND_BBT_CREATE | NAND_BBT_WRITE
			| NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
	.offs =	0,
	.len = 4,
	.veroffs = 14,
	.maxblocks = 4,
	.pattern = elite_mirror_pattern_2048,
};

static void elite_set_ecc_engine(int ecc_type_local)
{
	ENTER();
	printf("ECC_MODE: %d, ecc_type_local: %d\n",NAND_ECC_MODE,ecc_type_local);
	writel(readl(ctrl_base + ELITE_NFC_REG_23) & 0xfffffffc, ctrl_base + ELITE_NFC_REG_23);
	writel(readl(ctrl_base + ELITE_NFC_REG_23) | ecc_type_local, ctrl_base + ELITE_NFC_REG_23);      


	/* enable interrupt when ECC decoder finish error correction */
	writew(0x101, ctrl_base + ELITE_NFC_REG_24);
	writel(readl(ctrl_base + ELITE_NFC_REG_23) | 0x100, ctrl_base + ELITE_NFC_REG_23);

	if (ecc_type == 1)
		writel(readl(ctrl_base + ELITE_NFC_REG_12) & (~BIT(2)), ctrl_base + ELITE_NFC_REG_12);
	else
		writel(readl(ctrl_base + ELITE_NFC_REG_12) | BIT(2), ctrl_base + ELITE_NFC_REG_12);
       
	LEAVE();
}

static void nand_hwecc_on_off(int on_off)
{
	ENTER();
	if (on_off == 1) // hardware ECC on
	{
		writeb(readb(ctrl_base + ELITE_NFC_REG_12) & (~0x40), ctrl_base + ELITE_NFC_REG_12);
	}	
	else if (on_off == 0) // hardware ECC off
	{
		writeb(readb(ctrl_base + ELITE_NFC_REG_12) | 0x40, ctrl_base + ELITE_NFC_REG_12);
	}
	LEAVE();
}

static int elite_nand_hwecc_writeoob(struct mtd_info *mtd, struct nand_chip *chip, int page)
{
	int pos, status;
	struct elite_nand_priv *priv = chip->priv;
	ENTER();
	priv->datalen = 0;
	chip->write_buf(mtd, chip->oob_poi, mtd->oobsize);
	nand_dma_config(mtd, mtd->oobsize, 1, -1, -1);
	NAND_PRINTF("ECC.SIZE:%d\n",chip->ecc.size);
	NAND_PRINTF("ECC.STEP:%d\n",chip->ecc.steps);
	NAND_PRINTF("ECC.bytes:%d\n",chip->ecc.bytes);

	pos=(chip->ecc.steps-1)*chip->ecc.bytes+mtd->writesize;

	chip->cmdfunc(mtd, NAND_CMD_SEQIN, pos,page);
	chip->cmdfunc(mtd, NAND_CMD_PAGEPROG, -1, -1);

	status = chip->waitfunc(mtd, chip);
	if (status & NAND_STATUS_FAIL)
	{
		LEAVE();
		while(1);
		return -EIO;
	}
	LEAVE();
	return 0;
}

static int elite_nand_writepage(struct mtd_info *mtd, struct nand_chip *chip, 
		const uint8_t *buf, int oob_required, int page, int cached, int raw)
{
	int status;
	ENTER();
	test_page=page;
	chip->ecc.write_page(mtd, chip, buf, oob_required);
	chip->cmdfunc(mtd, NAND_CMD_SEQIN, 0x00, page);

	cached = 0;

	if (!cached || !(chip->options & NAND_CACHEPRG))
	{
		chip->cmdfunc(mtd, NAND_CMD_PAGEPROG, -1, -1);

		status = chip->waitfunc(mtd, chip);
		if ((status & NAND_STATUS_FAIL) && (chip->errstat))
			status = chip->errstat(mtd, chip, FL_WRITING, status, page);

		if (status & NAND_STATUS_FAIL)
		{
			LEAVE();
			return -EIO;
		}
	}
	else 
	{
		chip->cmdfunc(mtd, NAND_CMD_CACHEDPROG, -1, -1);
		status = chip->waitfunc(mtd, chip);
	}

#ifdef CONFIG_MTD_NAND_VERIFY_WRITE
	chip->cmdfunc(mtd, NAND_CMD_READ0, 0, page);

	if (chip->verify_buf(mtd, buf, mtd->writesize))
	{
		LEAVE();
		return -EIO;
	}
#endif
	LEAVE();
	return 0;
}

static void elite_nand_writepage_lowlevel(struct mtd_info *mtd, struct nand_chip *chip, const uint8_t *buf,int oob_required)
{
	struct elite_nand_priv *priv = chip->priv;
	ENTER();
	priv->datalen = 0;
	chip->write_buf(mtd, buf, mtd->writesize);
	nand_dma_config(mtd, mtd->writesize, 1, 0, -1);  /*  2048bytes  */
    if(oob_required)
    {
		if (mtd->writesize == 2048) 
		{
			memcpy(ctrl_base+ECC_FIFO_0, chip->oob_poi, mtd->oobsize);
			/* solve a hardware bug --- bank 3, byte 7, bit 7 error  */
		} 
		else if (mtd->writesize == 4096) 
		{
			memcpy(ctrl_base+ECC_FIFO_0, chip->oob_poi, 64);
		} 
		else if (mtd->writesize == 8192) 
		{
				memcpy(ctrl_base+ECC_FIFO_0, chip->oob_poi, 64);	
				writel(readl((volatile char *)ctrl_base + ELITE_NFC_REG_D) | 0x08, (volatile char *)ctrl_base + ELITE_NFC_REG_D);
				memcpy((volatile char *)ctrl_base+ECC_FIFO_0, chip->oob_poi+64, 64);
				writel(readl((volatile char *)ctrl_base + ELITE_NFC_REG_D) & 0xF7, (volatile char *)ctrl_base + ELITE_NFC_REG_D);
		} 
		else 
		{
			memcpy(ctrl_base+ECC_FIFO_0, chip->oob_poi, mtd->oobsize);
		}
    }
	else
	{	
		memset(ctrl_base+ECC_FIFO_0,0xff, mtd->oobsize);
	}
	if((!bootmode)||(NAND_ECC_MODE!=40))
	{
		*((volatile char *)ctrl_base+ECC_FIFO_0+60)=0x00;
		*((volatile char *)ctrl_base+ECC_FIFO_0+61)=0x00;
		*((volatile char *)ctrl_base+ECC_FIFO_0+62)=0x00;
		*((volatile char *)ctrl_base+ECC_FIFO_0+63)=0x00;
	}
	LEAVE();
}


static int elite_nand_hwecc_readoob(struct mtd_info *mtd, struct nand_chip *chip, int page)
{
	ENTER();

	chip->cmdfunc(mtd, NAND_CMD_READOOB, 0, page);	
	chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);

	LEAVE();
	return 0;
}

static int elite_nand_hwecc_readpage(struct mtd_info *mtd, struct nand_chip *chip, uint8_t *buf, int oob_required,int page)
{
	uint8_t calc_ctrl;
	uint8_t smc_enable;
	struct elite_nand_priv *priv = chip->priv;
	ENTER();
	priv->datalen = 0;
	chip->read_buf(mtd, buf, mtd->writesize);
	if(oob_required)
	{
		chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);
	}
	else
	{
		memset(chip->oob_poi,0xff,mtd->oobsize);
	}
	LEAVE();
	return 0;
}

static void elite_nand_hwecc_hwctl(struct mtd_info *mtd, int mode)
{
	uint8_t ecc_tst_mode;
	ENTER();
	ecc_tst_mode = readb(ctrl_base + ELITE_NFC_REG_12);

	if (mode == hardware_ecc)
		writeb(ecc_tst_mode & (~0x40), ctrl_base + ELITE_NFC_REG_12);
	else
		writeb(ecc_tst_mode | 0x40, ctrl_base + ELITE_NFC_REG_12);
	LEAVE();
	return;
}

/* 
 * writesize, erasesize, oobsize of NAND flash are hard-encoded in the 4th 
 * byte of NAND flash ID. So these information are stored in struct mtd_info
 * after reading NAND flash ID and parsing it. However, ECC information should
 * be specified according to writesize while writesize is unavailable at
 * the time. A macro is defined to solve this issue.
 */
void elite_nand_hwecc_init(struct nand_chip *chip)
{
	ENTER();
	/* set hardware ECC on */
	nand_hwecc_on_off(1);

	if (NAND_BYTES_PER_PAGE == 2048)
	{
		chip->ecc.size = 512;
		chip->ecc.bytes = 8;
		chip->ecc.steps = 4;
		chip->ecc.prepad = 1;
		chip->ecc.postpad = 8;
		chip->ecc.layout = &elite_nand_oobinfo_2048;
		chip->bbt_td = &elite_bbt_main_descr_2048;
		chip->bbt_md = &elite_bbt_mirror_descr_2048;
	}
	else if (NAND_BYTES_PER_PAGE == 4096)
	{
		chip->ecc.size = 512;
		if (NAND_ECC_MODE == 8)
		{
			chip->ecc.bytes = 16;
			chip->ecc.layout = &elite_nand_8bit_oobinfo_4096;
		}
		else if (NAND_ECC_MODE == 12)
		{
			chip->ecc.bytes = 20;
			chip->ecc.layout = &elite_12bit_oobinfo_4096;
		}
		else
		{
			chip->ecc.bytes = 8;
			chip->ecc.layout = &elite_nand_oobinfo_4096;
		}
		chip->ecc.steps = 8;
		chip->ecc.prepad = 1;
		chip->ecc.postpad = 8;
		chip->bbt_td = &elite_bbt_main_descr_2048;
		chip->bbt_md = &elite_bbt_mirror_descr_2048;
	}
	else if (NAND_BYTES_PER_PAGE == 8192)
	{
		chip->ecc.size = 1024;
		if (NAND_ECC_MODE == 24)
		{
			chip->ecc.bytes = 42;
			chip->ecc.layout = &elite_nand_24bit_oobinfo_8192;
		}
		else if (NAND_ECC_MODE == 40)
		{
			chip->ecc.bytes = 70;
			chip->ecc.layout = &elite_nand_40bit_oobinfo_8192;
		}
		else
		{
			printf("error 111111111\n");
			while(1);
		}
		chip->ecc.steps = 8;
		chip->ecc.prepad = 1;
		chip->ecc.postpad = 8;
		chip->bbt_td = &elite_bbt_main_descr_2048;
		chip->bbt_md = &elite_bbt_mirror_descr_2048;

	}
	else
	{
		chip->ecc.size = 512;
		chip->ecc.bytes = 3;
		chip->ecc.steps = 1;
		chip->ecc.prepad = 4;
		chip->ecc.postpad = 9;
		chip->ecc.layout = &elite_nand_oobinfo_512;
		chip->bbt_td = &elite_bbt_main_descr_512;
		chip->bbt_md = &elite_bbt_mirror_descr_512;
	}

	chip->write_page      = elite_nand_writepage;
	chip->ecc.write_page  = elite_nand_writepage_lowlevel;
	chip->ecc.write_oob   = elite_nand_hwecc_writeoob;
	chip->ecc.read_page   = elite_nand_hwecc_readpage;
	chip->ecc.read_oob    = elite_nand_hwecc_readoob;
	chip->ecc.hwctl       = elite_nand_hwecc_hwctl;
	chip->ecc.mode        = NAND_ECC_HW;
	chip->ecc.strength    = NAND_ECC_MODE;
	chip->badblock_pattern =NULL;
	ecc_type = 1;
	if (NAND_ECC_MODE == 8)
		elite_set_ecc_engine(ECC8BIT); // 8bits BCH/512 bytes
	else if (NAND_ECC_MODE == 12)
		elite_set_ecc_engine(ECC12BIT); // 12bits BCH/512 bytes
	else if (NAND_ECC_MODE == 24)
		elite_set_ecc_engine(ECC24BIT); // 24bits BCH/1024 bytes
	else if (NAND_ECC_MODE == 40)
		elite_set_ecc_engine(ECC40BIT); // 40bits BCH/1024 bytes
	else  // 4bits BCH/512 bytes
		elite_set_ecc_engine(ECC4BIT);
	LEAVE();
}

