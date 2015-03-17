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
#include <linux/mtd/nand.h>
#include <linux/mtd/bbm.h>
#include <asm-generic/errno.h>
#include <asm/io.h>
#include <asm/dma-mapping.h>

#include "elite_nand.h"

#define MAX_CHIPS  CONFIG_SYS_MAX_NAND_DEVICE

struct elite_nand_priv nand_priv[MAX_CHIPS];
static int chip_n = 0;

volatile void *ctrl_base;
volatile void *pdma_base;

/* BCH ECC algorithm */
int ecc_type = 1;

static int test_page=0;
static int dump_count =0;

extern void elite_nand_hwecc_init(struct nand_chip *chip);
static __inline__ unsigned long log2 (unsigned long word)
{
#ifdef __I386__
  __asm__ ("bsfl %1,%0"
	   : "=r" (word)
	   : "r" (word));
  return word;
#else
  int i;

  for(i=0; i<(8*sizeof(word)); i++)
    if ((1<<i) & word)
      return i;

  return 0;
#endif
}
static int elite_nfc_transfer_ready(void)
{
	int i = 0;
	ENTER();
	while (1)	
	{
		if (!(readb(ctrl_base+ ELITE_NFC_REG_A) & NFC_BUSY))
			break;

		if (++i>>23)
		{
			printf("NFC err : transfer status not ready\n");
			LEAVE();
			return -3;
		}
	}
	LEAVE();
	return 0;
	
}

static int elite_nand_ready(void)
{
	unsigned int b2r_stat;
	int i = 0;
	ENTER();
	while (1)	
	{
		if (readb(ctrl_base + ELITE_NFC_REG_B) & B2R)
			break;
		if ((++i>>23)) 
		{
			printf("nand flash is not ready\n");
			LEAVE();
			return -1;
		}
	}
	b2r_stat = readb(ctrl_base + ELITE_NFC_REG_B);
	writeb(B2R|b2r_stat, ctrl_base + ELITE_NFC_REG_B);
	if (readb(ctrl_base + ELITE_NFC_REG_B) & B2R)	
	{
		printf("NFC err : B2R status not clean\n");
		LEAVE();
		return -2;
	}
	LEAVE();
	return 0;
	
}

static int elite_wait_chip_ready(void)
{
	int i = 0;
	ENTER();
	while (1) 
	{
		if ((readb(ctrl_base + ELITE_NFC_REG_A) & FLASH_RDY))
			break;
		if (++i>>23)
		{
			printf("NFC err : chip status not ready\n");
			LEAVE();
			return -3;
		}
	}
	LEAVE();
	return 0;
	
}

static int elite_wait_cmd_ready(void)
{
	int i = 0;
	ENTER();
	while (1)	
	{
		if (!(readb(ctrl_base+ ELITE_NFC_REG_A) & NFC_CMD_RDY))
			break;
		if (++i>>23)
		{
			printf("NFC err : cmd status not ready\n");
			LEAVE();
			return -3;
		}
	}
	LEAVE();
	return 0;
}

static void elite_wait_nfc_ready(void)
{

	unsigned int  i = 0;
	ENTER();
	while (1) 
	{
		if (!(readb(ctrl_base + ELITE_NFC_REG_A) & NFC_BUSY))
			break;
		else if ((readw(ctrl_base + ELITE_NFC_REG_25) & (ERR_CORRECT | BCH_ERR)) == (ERR_CORRECT | BCH_ERR))
			break;

		if (++i>>21)
		{
			printf("NFC err : nfc status not ready\n");
			LEAVE();
			return;
		}
	}
	LEAVE();
	return 0;

}

/*****************************************************
* flag = 0, check BCH ECC
* flag = 1, dont check ECC
* flag = 2, check Harming ECC
*****************************************************/
static int elite_nfc_wait_idle(struct mtd_info *mtd, unsigned int flag, int command, int column, unsigned int page_addr)
{
	int i = 0, k;
	int page_step, oob_step;
	unsigned int bank_stat1, bank_stat2;
	unsigned int data_redunt_flag;
	struct nand_chip *nand = (struct nand_chip *)mtd->priv;
	struct elite_nand_priv *info = (struct elite_nand_priv *)nand->priv;

	page_step = 1 + mtd->writesize/NAND_BANK_SIZE;
	oob_step = 1;
	ENTER();
	while (1)
	{
		if (readb(ctrl_base + ELITE_NFC_REG_1D) & NFC_IDLE)
					break;
		if ((flag != 1) && (flag != 2))
		{
			if (command == NAND_CMD_READ0)
			{
			
				unsigned int failed =mtd->ecc_stats.failed;
				for (k = 0; k < page_step; k++)
				{
				
					elite_wait_nfc_ready();
					bank_stat1 = readw(ctrl_base + ELITE_NFC_REG_25);
					NAND_PRINTF("bank_stat1 page_step:0x%x, READ0:0x%x\n",k,bank_stat1);
					if ((bank_stat1 & (ERR_CORRECT | BCH_ERR)) == (ERR_CORRECT | BCH_ERR))
					{
						bank_stat2 = readw(ctrl_base + ELITE_NFC_REG_26);
						data_redunt_flag = bank_stat2 & 0x800;

						if (data_redunt_flag)
						{
							bch4bit_redunt_ecc_correct(mtd);	
						}
						else
						{
							bch4bit_data_ecc_correct(mtd);
							
						}
					}
				}
				if ((mtd->ecc_stats.failed !=failed)&&!(
				(((*((volatile char *)ctrl_base+ECC_FIFO_0+60)) != 0xFF)||
				((*((volatile char *)ctrl_base+ECC_FIFO_0+61)) != 0xFF))&&
				(((*((volatile char *)ctrl_base+ECC_FIFO_0+62)) != 0xFF)||
				((*((volatile char *)ctrl_base+ECC_FIFO_0+63)) != 0xFF))))
				{
					mtd->ecc_stats.failed=failed;
					writew(readw((volatile char *)ctrl_base + ELITE_NFC_REG_25) | (ERR_CORRECT | BCH_ERR), (volatile char *)ctrl_base + ELITE_NFC_REG_25);
					writew(readw((volatile char *)ctrl_base  + ELITE_NFC_REG_23) | READ_RESUME, (volatile char *)ctrl_base  + ELITE_NFC_REG_23);

				}
				else if(mtd->ecc_stats.failed !=failed)
				{    
			
					printk("\tdump:%d page:0x%x,column:0x%x\n",dump_count,page_addr,column);
					writew(readw((volatile char *)ctrl_base+ ELITE_NFC_REG_23) | READ_RESUME, (volatile char *)ctrl_base+ ELITE_NFC_REG_23);                        
					return -2;	
				}
				
			}
			else if (command == NAND_CMD_READOOB)
			{
				unsigned int failed =mtd->ecc_stats.failed;
				bank_stat1 = readw(ctrl_base + ELITE_NFC_REG_25);
				NAND_PRINTF("bank_stat1 OOB:0x%x\n",bank_stat1);
				if ((bank_stat1 & (ERR_CORRECT | BCH_ERR)) == (ERR_CORRECT | BCH_ERR))
				{
					elite_wait_nfc_ready();
					bch4bit_redunt_ecc_correct(mtd);
				}
				if ((mtd->ecc_stats.failed !=failed)&&!(
					(((*((volatile char *)ctrl_base+ECC_FIFO_0+60)) != 0xFF)||
					((*((volatile char *)ctrl_base+ECC_FIFO_0+61)) != 0xFF))&&
					(((*((volatile char *)ctrl_base+ECC_FIFO_0+62)) != 0xFF)||
					((*((volatile char *)ctrl_base+ECC_FIFO_0+63)) != 0xFF))))
				{
					mtd->ecc_stats.failed=failed;
					writew(readw((volatile char *)ctrl_base + ELITE_NFC_REG_25) | (ERR_CORRECT | BCH_ERR), (volatile char *)ctrl_base+ ELITE_NFC_REG_25);
					writew(readw((volatile char *)ctrl_base  + ELITE_NFC_REG_23) | READ_RESUME, (volatile char *)ctrl_base + ELITE_NFC_REG_23);
				}
				else if(mtd->ecc_stats.failed !=failed)
				{    
						printk("\tdump:%d page:0x%x,column:0x%x\n",dump_count,page_addr,column);		
						writew(readw((volatile char *)ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, (volatile char *)ctrl_base  + ELITE_NFC_REG_23);
						return -2;   	
				}
			}
			writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);
		}
		if (i >> 21)
		{
			printf("NFC err : nfc status not idle\n");
			LEAVE();
			return -1;
		}
		i++;
	}
	LEAVE();
	return 0;
}

/*
* bit_correct - correct 1-bit error in one byte
* @c   -bytes address of NAND flash data in which bit error occurs
* @pos -offset of bit which errors in the byte
*/
static void bit_correct(uint8_t *c, uint8_t pos)
{
        
  	NAND_PRINTF("bit_correct :begin,c:0x%x,pos:%d\n",c[0],pos);
	c[0] = ((c[0] ^ (0x01 << pos)) & (0x01 << pos)) | (c[0] & (~(0x01 << pos)));
	NAND_PRINTF("bit_correct :end,c:0x%x,pos:%d\n",c[0],pos);
}

void bch4bit_data_ecc_correct(struct mtd_info *mtd)
{
	int i;
	unsigned int bch_err_pos[8], bank_stat1, bank_stat2, bch_ecc_idx, bank, ecc_engine;
	struct nand_chip *nand = (struct nand_chip *)mtd->priv;
	struct elite_nand_priv *info = (struct elite_nand_priv *)nand->priv;
	ENTER();
	bank_stat1 = readw(ctrl_base + ELITE_NFC_REG_25);
	if ((bank_stat1 & (ERR_CORRECT | BCH_ERR)) == (ERR_CORRECT | BCH_ERR))
	{
		bank_stat2 = readw(ctrl_base + ELITE_NFC_REG_26);
		bch_ecc_idx = bank_stat2 & BCH_ERR_CNT;
		bank = (bank_stat2 & 0x700) >> 8; /* bank number of current decoder */

		ecc_engine = readl(ctrl_base + ELITE_NFC_REG_23) & ECC_MODE;
		NAND_PRINTF("bch_ecc_idx:0x%x,ecc_engine:%d\n",bch_ecc_idx,ecc_engine);
		/* ERROR bit number is beyond the capacity of error correct */
		if (((ecc_engine <5 )&&(bch_ecc_idx > (ecc_engine*4))) || ((ecc_engine == 5) && (bch_ecc_idx > 24))|| ((ecc_engine == 6) && (bch_ecc_idx > 40))) 
		{
			NAND_PRINTF("bch_ecc_idx exceed ecc_engine:0x%x\n",(unsigned int)test_page*NAND_BYTES_PER_PAGE);
			writew(readw(ctrl_base + ELITE_NFC_REG_25) | (ERR_CORRECT | BCH_ERR), ctrl_base + ELITE_NFC_REG_25);
			writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);
			mtd->ecc_stats.failed++;

			LEAVE();
			return;
		}

		/* BCH ecc correct */
		for (i = 0; i < bch_ecc_idx; i++)
		{
			bch_err_pos[i] = (readw((volatile char *)ctrl_base + ELITE_NFC_REG_27 + 2*i) & 0x3fff);

			NAND_PRINTF("bit_correct:0x%x,pos:%d\n",(unsigned int)test_page*NAND_BYTES_PER_PAGE+(unsigned int) (NAND_BANK_SIZE*bank + (unsigned int)(bch_err_pos[i] >> 3)),bch_err_pos[i] & 0x07);
			if ((bch_err_pos[i] >> 3) < NAND_BANK_SIZE)
				bit_correct((unsigned char *)info->dmabuf + (NAND_BANK_SIZE*bank + (bch_err_pos[i] >> 3)), bch_err_pos[i] & 0x07);
			else if(bank==7)
			{
				 bit_correct(((volatile char *)ctrl_base +ECC_FIFO_0 +(bch_err_pos[i] >> 3)-NAND_BANK_SIZE),(bch_err_pos[i] & 0x07));						 
			}
		}
	}

	writew(readw(ctrl_base + ELITE_NFC_REG_25) | (ERR_CORRECT | BCH_ERR), ctrl_base + ELITE_NFC_REG_25);
	writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);
	LEAVE();
}

void bch4bit_redunt_ecc_correct(struct mtd_info *mtd)
{
	int i;
	unsigned int bch_err_pos[12], bank_stat1, bank_stat2, bch_ecc_idx, ecc_engine;
	struct elite_nand_priv *priv = (struct elite_nand_priv *)((struct nand_chip *)mtd->priv)->priv;
	ENTER();
	bank_stat2 = readw(ctrl_base + ELITE_NFC_REG_26);
	bch_ecc_idx = bank_stat2 & BCH_ERR_CNT;
	bank_stat1 = readw(ctrl_base + ELITE_NFC_REG_25);
	if ((bank_stat1 & (ERR_CORRECT | BCH_ERR)) == (ERR_CORRECT | BCH_ERR)) /* has ECC error and BCH decoder finish */
	{
		/* ECC mode select: 1 bit Hamming, 4bits BCH, 8bits BCH, 12bits BCH, 16bits BCH per 512bytes */
		ecc_engine = readl(ctrl_base + ELITE_NFC_REG_23) & ECC_MODE;
		/* ERROR bit number is beyond the capacity of error correct */
		if (((ecc_engine <5 )&&(bch_ecc_idx > (ecc_engine*4))) || ((ecc_engine == 5) && (bch_ecc_idx > 24))|| ((ecc_engine == 6) && (bch_ecc_idx > 40)))
		{	
			NAND_PRINTF("redunt_bch_ecc_idx exceed ecc_engine:0x%x\n",(unsigned int)test_page*NAND_BYTES_PER_PAGE);

			/* 
			* for example, if ECC mode is 4bits BCH/512bytes (bch_ecc_idx = 1), it has been beyond 
			* the capacity of correct error when greater than 4 bits error
			*/
			writew(readw(ctrl_base + ELITE_NFC_REG_25) |(ERR_CORRECT | BCH_ERR), ctrl_base + ELITE_NFC_REG_25);
			writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);

			mtd->ecc_stats.failed++;
			LEAVE();
			return;
		}

		/* OK, error is within the capacity of correction BCH ecc correct */
		for (i = 0; i < bch_ecc_idx; i++)
		{
			bch_err_pos[i] = (readw((volatile char *)ctrl_base + ELITE_NFC_REG_27 + 2*i) & 0x3fff);

			printf("redunt_bit_correct:0x%x,pos:%d\n",(unsigned int)test_page*NAND_BYTES_PER_PAGE+(unsigned int) ((unsigned int)(bch_err_pos[i] >> 3)),bch_err_pos[i] & 0x07);

			//if ((bch_err_pos[i] >> 3) < NAND_BANK_SIZE)
			//	/* for WM8650 bit 0-2: error bit offset, bit 3-13: error byte offset */
			//	bit_correct((unsigned char *)(priv->dmabuf + mtd->writesize) + (bch_err_pos[i] >> 3), bch_err_pos[i] & 0x07);
			if((bch_err_pos[i] >> 3 )>=NAND_BANK_SIZE )
				bit_correct(((volatile char *)ctrl_base+ECC_FIFO_0 +(bch_err_pos[i] >> 3)-NAND_BANK_SIZE),(bch_err_pos[i] & 0x07));
		}
	}

	writew(readw(ctrl_base + ELITE_NFC_REG_25) | (ERR_CORRECT | BCH_ERR), ctrl_base + ELITE_NFC_REG_25);
	writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);
	LEAVE();
}

static void nand_hamming_ecc_1bit_correct(struct mtd_info *mtd)
{
	struct nand_chip *nand = (struct nand_chip *)mtd->priv;
	struct elite_nand_priv *priv = (struct elite_nand_priv *)nand->priv;
	unsigned int ecc_err_pos, bank_stat, redunt_stat;
	ENTER();
	bank_stat = readl(ctrl_base + ELITE_NFC_REG_20);
	redunt_stat = readb(ctrl_base + ELITE_NFC_REG_1F);

	/* bank data has uncorrectable errors */
	if (bank_stat & 0x5555) 
	{
		mtd->ecc_stats.failed++;
		LEAVE();
		return;
	} /* redundant area has uncorrectable errors */
	else if (redunt_stat & 0x05) 
	{
		mtd->ecc_stats.failed++;
		LEAVE();
		return;
	} 
	else if (((mtd->writesize == 2048) && (bank_stat & 0x2222)) ||
	((mtd->writesize == 4096) && (bank_stat & 0x22222222)) ||
	((mtd->writesize == 512) && (bank_stat & 0x22))) 
	{
		printf("************enter here 111************\n");
		writeb(readb(ctrl_base + ELITE_NFC_REG_15) & 0xfc, ctrl_base + ELITE_NFC_REG_15);

		if (bank_stat & 0x02) 
		{
			ecc_err_pos = readw(ctrl_base + ELITE_NFC_REG_1A);
			bit_correct((unsigned char *)priv->dmabuf + (ecc_err_pos & 0x1ff), (ecc_err_pos >> 9) & 0x07);
		} 
		else if (bank_stat & 0x20) 
		{
			ecc_err_pos = readw(ctrl_base + ELITE_NFC_REG_1B);			
			bit_correct((unsigned char *)priv->dmabuf + (512 + (ecc_err_pos & 0x1ff)), (ecc_err_pos >> 9) & 0x07);
		}

		if (mtd->writesize == 2048) 
		{
			writeb(readb(ctrl_base + ELITE_NFC_REG_15) | 0x1, ctrl_base + ELITE_NFC_REG_15);
			if (bank_stat & 0x0200) 
			{
				ecc_err_pos = readw(ctrl_base + ELITE_NFC_REG_1A);
				bit_correct((unsigned char *)priv->dmabuf + (1024 + (ecc_err_pos & 0x1ff)), (ecc_err_pos >> 9) & 0x07);
			} 
			else if (bank_stat & 0x2000) 
			{
				ecc_err_pos = readw(ctrl_base + ELITE_NFC_REG_1B);
				bit_correct((unsigned char *)priv->dmabuf + (1536 + (ecc_err_pos & 0x1ff)), (ecc_err_pos >> 9) & 0x07);
			}
		} 
		else if (mtd->writesize == 4096) 
		{
			writeb(readb(ctrl_base + ELITE_NFC_REG_15) | 0x1 , ctrl_base + ELITE_NFC_REG_15);

			if (bank_stat & 0x0200) 
			{
				ecc_err_pos = readw(ctrl_base + ELITE_NFC_REG_1A);
				bit_correct((unsigned char *)priv->dmabuf + (1024 + (ecc_err_pos & 0x1ff)), (ecc_err_pos >> 9) & 0x07);
			} 
			else if (bank_stat & 0x2000) 
			{
				ecc_err_pos = readw(ctrl_base + ELITE_NFC_REG_1B);
				bit_correct((unsigned char *)priv->dmabuf + (1536 + (ecc_err_pos & 0x1ff)), (ecc_err_pos >> 9) & 0x07);
			} 
			else if (bank_stat & 0x020000) 
			{
				writeb((readb(ctrl_base + ELITE_NFC_REG_15) & 0xFC) | 0x2, ctrl_base + ELITE_NFC_REG_15);
				ecc_err_pos = readw(ctrl_base + ELITE_NFC_REG_1A);
				bit_correct((unsigned char *)priv->dmabuf + (2048 + (ecc_err_pos & 0x1ff)), (ecc_err_pos >> 9) & 0x07);
			} 
			else if (bank_stat & 0x200000) 
			{
				writeb((readb(ctrl_base + ELITE_NFC_REG_15) & 0xFC) | 0x2, ctrl_base + ELITE_NFC_REG_15);
				ecc_err_pos = readw(ctrl_base + ELITE_NFC_REG_1B);
				bit_correct((unsigned char *)priv->dmabuf + (2560 + (ecc_err_pos & 0x1ff)), (ecc_err_pos >> 9) & 0x07);
			} 
			else if (bank_stat & 0x02000000) 
			{
				writeb(readb(ctrl_base + ELITE_NFC_REG_15) | 0x3, ctrl_base + ELITE_NFC_REG_15);
				ecc_err_pos = readw(ctrl_base + ELITE_NFC_REG_1A);
				bit_correct((unsigned char *)priv->dmabuf + (3072 + (ecc_err_pos & 0x1ff)), (ecc_err_pos >> 9) & 0x07);
			} 
			else if (bank_stat & 0x20000000) 
			{
				writeb(readb(ctrl_base + ELITE_NFC_REG_15) | 0x3, ctrl_base + ELITE_NFC_REG_15);
				ecc_err_pos = readw(ctrl_base + ELITE_NFC_REG_1B);
				bit_correct((unsigned char *)priv->dmabuf + (3584 + (ecc_err_pos & 0x1ff)), (ecc_err_pos >> 9) & 0x07);
			}
		}
	} 
	else if (redunt_stat & 0x02) /* redundant area has one error */
	{
		printf("************enter here 222222222************\n");
		ecc_err_pos = readw(ctrl_base + ELITE_NFC_REG_1C);
		bit_correct((unsigned char *)(priv->dmabuf + mtd->writesize) + (ecc_err_pos & 0x3f),(ecc_err_pos >> 8) & 0x07);
	}
	LEAVE();
}

static void  nand_cmd_get_paramter_func(struct mtd_info *mtd,unsigned int maf_id)
{

	struct nand_chip *nand = (struct nand_chip *)mtd->priv;
	struct elite_nand_priv *info = (struct elite_nand_priv *)nand->priv;
	unsigned int cfg = 0;
	int status = -1;
	int i=0;
	ENTER();
	NAND_PRINTF("enter nand_cmd_get_paramter_func maf_id:0x%x\n",maf_id);
	if(maf_id==NAND_MFR_MICRON)
	{
		info->datalen = 0;
		//writeb(readb(ctrl_base + ELITE_NFC_REG_B) | B2R, ctrl_base + ELITE_NFC_REG_B);
		writel(NAND_CMD_PARAM, ctrl_base + ELITE_NFC_REG_2);
		writel(0xff00, ctrl_base + ELITE_NFC_REG_3);
		cfg = DPAHSE_DISABLE|(0x02<<1);
		writel(cfg|NFC_TRIGGER|SP_CMD_EN,ctrl_base + ELITE_NFC_REG_1);

		status = elite_nand_ready();
		if (status)
		{
			printf("in nand_cmd_get_paramter_func(): wait cmd is not ready\n");
			writeb(readb(ctrl_base + ELITE_NFC_REG_9) & 0xfd, ctrl_base + ELITE_NFC_REG_9);
			LEAVE();
			return;
		}
		status = elite_nfc_transfer_ready();
		if (status) 
		{
			writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);
			LEAVE();
			return;
		}
		for (i = 0; i < 56; i++) 
		{
			writel(0xff00+i, ctrl_base + ELITE_NFC_REG_3);
			cfg = NAND2NFC|SING_RW;
			writel(cfg|NFC_TRIGGER|SP_CMD_EN, ctrl_base + ELITE_NFC_REG_1);
			status = elite_wait_cmd_ready();
			if (status)
			{
				printf("in nand_cmd_get_paramter_func(): wait cmd is not ready\n");
				writeb(readb(ctrl_base + ELITE_NFC_REG_9) & 0xfd, ctrl_base + ELITE_NFC_REG_9);
				LEAVE();
				return;
			}
			status = elite_nfc_transfer_ready();
			if (status) 
			{
				printf("in nand_cmd_get_paramter_func(): wait transfer cmd is not ready\n");
				LEAVE();
				return;
			}
			info->datalen = 0;
		}
		if((readb(ctrl_base + ELITE_NFC_REG_0)& 0xff) ==0x42) //B
		{
			printf("enter B branch\n");
			((unsigned char *)info->dmabuf)[0]=1;
		}
		else if( (readb(ctrl_base + ELITE_NFC_REG_0)& 0xff) ==0x46) //F
		{
			printf("enter F branch\n");
			((unsigned char *)info->dmabuf)[0]=2;
		}
		else if( (readb(ctrl_base + ELITE_NFC_REG_0)& 0xff) ==0x45) //E
		{
			printf("enter E branch\n");
			((unsigned char *)info->dmabuf)[0]=2;
		}
		else if( (readb(ctrl_base + ELITE_NFC_REG_0)& 0xff) ==0x4A) //J
		{
			printf("enter J branch\n");
			((unsigned char *)info->dmabuf)[0]=2;
		}
		else if( (readb(ctrl_base + ELITE_NFC_REG_0)& 0xff) ==0x4B) //K
		{
			printf("enter K branch\n");
			((unsigned char *)info->dmabuf)[0]=2;
		}
		else if( (readb(ctrl_base + ELITE_NFC_REG_0)& 0xff) ==0x4D) //M
		{
			printf("enter M branch\n");
			((unsigned char *)info->dmabuf)[0]=4;
		}
		else if( (readb(ctrl_base + ELITE_NFC_REG_0)& 0xff) ==0x55) //U
		{
			printf("enter U branch\n");
			((unsigned char *)info->dmabuf)[0]=4;
		}
		else
		{
			printf("bad micron id\n");
			while(1);
		}
	}
	else if(maf_id==NAND_MFR_TOSHIBA)
	{
		info->datalen = 0;
		//writeb(readb(ctrl_base + ELITE_NFC_REG_B) | B2R, ctrl_base + ELITE_NFC_REG_B);
		writel(NAND_CMD_READID, ctrl_base + ELITE_NFC_REG_2);
		writel(0xff00, ctrl_base + ELITE_NFC_REG_3);
		cfg = DPAHSE_DISABLE|(0x02<<1);
		writel(cfg|NFC_TRIGGER|SP_CMD_EN,ctrl_base + ELITE_NFC_REG_1);
		status = elite_wait_cmd_ready();
		if (status)
		{
			printf("in nand_cmd_get_paramter_func(): wait cmd is not ready\n");
			writeb(readb(ctrl_base + ELITE_NFC_REG_9) & 0xfd, ctrl_base + ELITE_NFC_REG_9);
			LEAVE();
			return;
		}

		status = elite_nfc_transfer_ready();
		if (status) 
		{
			writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);
			LEAVE();
			return;
		}
		for (i = 0; i < 5; i++) 
		{
			writel(0xff00+i, ctrl_base + ELITE_NFC_REG_3);
			cfg = NAND2NFC|SING_RW;
			writel(cfg|NFC_TRIGGER|SP_CMD_EN, ctrl_base + ELITE_NFC_REG_1);
			status = elite_wait_cmd_ready();
			if (status)
			{
				printf("in nand_cmd_get_paramter_func(): wait cmd is not ready\n");
				writeb(readb(ctrl_base + ELITE_NFC_REG_9) & 0xfd, ctrl_base + ELITE_NFC_REG_9);
				LEAVE();
				return;
			}
			status = elite_nfc_transfer_ready();
			if (status) 
			{
				printf("in nand_cmd_get_paramter_func(): wait transfer cmd is not ready\n");
				LEAVE();
				return;
			}
			info->datalen = 0;
		}
		((unsigned char *)info->dmabuf)[0]=0x1<<((readb(ctrl_base +ELITE_NFC_REG_0)& 0xC)>>2);
	}
	else
	{
		((unsigned char *)info->dmabuf)[0]=1;
	}
	LEAVE();
}

static void nand_cmd_readid_func(struct mtd_info *mtd)
{
	struct nand_chip *nand = (struct nand_chip *)mtd->priv;
	struct elite_nand_priv *priv = (struct elite_nand_priv *)nand->priv;
	unsigned int cfg = 0, i = 0;
	int status = -1;
	ENTER();
	writel(NAND_CMD_READID, ctrl_base + ELITE_NFC_REG_2);
	writel(0xff00, ctrl_base + ELITE_NFC_REG_3);
	cfg = DPAHSE_DISABLE|(0x02<<1);
	writel(cfg|NFC_TRIGGER|SP_CMD_EN,ctrl_base + ELITE_NFC_REG_1);

	status = elite_nfc_transfer_ready();
	if (status) 
	{
		writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);
		LEAVE();
		return;
	}

	cfg = NAND2NFC|SING_RW;
	for (i = 0; i < 5; i++) 
	{
		writel(cfg|NFC_TRIGGER|SP_CMD_EN, ctrl_base + ELITE_NFC_REG_1);
		status = elite_wait_cmd_ready();
		if (status)
		{
			printf("in elite_nand_readID(): wait cmd is not ready\n");
			LEAVE();
			return;
		}
		status = elite_nfc_transfer_ready();
		if (status) 
		{
			printf("in elite_nand_readID(): wait transfer cmd is not ready\n");
			LEAVE();
			return;
		}
		((unsigned char *)priv->dmabuf)[i] = readb(ctrl_base + ELITE_NFC_REG_0)& 0xff;
		NAND_PRINTF("readID is %x\n", readb(ctrl_base +ELITE_NFC_REG_0)& 0xff);
	}

	priv->datalen = 0;
	LEAVE();
	return;
}
static unsigned int  nand_address(struct mtd_info *mtd,unsigned int addr_cycle,int column, int page_addr)
{
	struct nand_chip *chip = mtd->priv;
	ENTER();
	if (column != -1)
	{
		writeb(column, ctrl_base + ELITE_NFC_REG_3);
		addr_cycle++;

		if (mtd->writesize != 512)
		{
			writeb(column >> 8, (unsigned char *)(ctrl_base + ELITE_NFC_REG_3) + 1);
			addr_cycle++;
		}
		if (page_addr != -1)
		{
			if (mtd->writesize != 512)
			{
				writeb(page_addr, ctrl_base + ELITE_NFC_REG_4);
				page_addr >>= 8;
				writeb(page_addr, (unsigned char *)(ctrl_base + ELITE_NFC_REG_4) + 1);
				addr_cycle += 2;
			}
			else
			{
				writeb(page_addr, (unsigned char *)(ctrl_base + ELITE_NFC_REG_3) + 1);
				page_addr >>= 8;
				writeb(page_addr, ctrl_base + ELITE_NFC_REG_4);
				addr_cycle += 2;
			}
			if (mtd->writesize == 2048)
			{
				/* 2048 bytes/per page, one more address cycle needed for 128Mb-larger devices */
				if (chip->chipsize > (128 << 20))
				{
					page_addr >>= 8;
					writeb(page_addr, ctrl_base + ELITE_NFC_REG_5);
					addr_cycle++;
				}
			}
			else if (mtd->writesize == 4096)
			{
				if (chip->chipsize > (256 << 20))
				{
					page_addr >>= 8;
					writeb(page_addr, ctrl_base + ELITE_NFC_REG_5);
					addr_cycle++;
				}
			}
			else
			{
				if (chip->chipsize > (32 << 20))
				{
					page_addr >>= 8;
					writeb(page_addr, ctrl_base + ELITE_NFC_REG_5);
					addr_cycle++;
				}
			}
		}
	}
	else if ((page_addr != -1) && (column == -1))
	{
		writeb(page_addr & 0xff, ctrl_base + ELITE_NFC_REG_3);
		page_addr >>= 8;
		writeb(page_addr, (unsigned char *)(ctrl_base + ELITE_NFC_REG_3) + 1);
		addr_cycle += 2;

		if (mtd->writesize == 2048)
		{
			/* 2048 bytes/per page, one more address cycle needed for 128Mb-larger devices */
			if (chip->chipsize > (128 << 20))
			{
				page_addr >>= 8;
				writeb(page_addr, ctrl_base + ELITE_NFC_REG_4);
				addr_cycle++;
			}
		}
		else if (mtd->writesize == 4096)
		{
			if (chip->chipsize > (256 << 20))
			{
				page_addr >>= 8;
				writeb(page_addr, ctrl_base + ELITE_NFC_REG_4);
				addr_cycle++;
			}
		}
		else
		{
			if (chip->chipsize > (32 << 20))
			{
				page_addr >>= 8;
				writeb(page_addr, ctrl_base + ELITE_NFC_REG_4);
				addr_cycle++;
			}
		}

	}
	LEAVE();
	return addr_cycle;
}
static void nand_cmd_erase1_func(struct mtd_info *mtd, unsigned int cycle, unsigned cmd, int col, int page)
{
	int addr_cycle = cycle, command = cmd, column = col, page_addr = page;
	struct nand_chip *chip = mtd->priv;
	int status;
	ENTER();

 	addr_cycle= nand_address(mtd, addr_cycle, column,  page_addr);

	writeb(command, ctrl_base + ELITE_NFC_REG_2);
	if (command == NAND_CMD_SEQIN)
		writel(SP_CMD_EN|((addr_cycle + 1) << 1) | NFC_TRIGGER, ctrl_base + ELITE_NFC_REG_1);
	else
		writel(SP_CMD_EN|DPAHSE_DISABLE | ((addr_cycle + 1) << 1) | NFC_TRIGGER, ctrl_base + ELITE_NFC_REG_1);

	if (command == NAND_CMD_ERASE1)
	{
		status = elite_wait_cmd_ready();
		if (status)
			writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);
		else
		{
			elite_wait_nfc_ready();
			status = elite_nfc_transfer_ready();
			if (status)
			{
				writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);	
			}
		}
	}
	LEAVE();
	return;
}

static void nand_cmd_erase2_func(struct mtd_info *mtd, unsigned command)
{
	int b2r_stat, status;
	struct nand_chip *nand = (struct nand_chip *)mtd->priv;
	struct elite_nand_priv *priv = (struct elite_nand_priv *)nand->priv;
	ENTER();
	writeb(command, ctrl_base + ELITE_NFC_REG_2);

	b2r_stat = readb(ctrl_base + ELITE_NFC_REG_B);
	writeb(b2r_stat | B2R, ctrl_base + ELITE_NFC_REG_B);

	elite_wait_chip_ready();
	writel(SP_CMD_EN|DPAHSE_DISABLE | (1 << 1) | NFC_TRIGGER, ctrl_base + ELITE_NFC_REG_1);
	priv->datalen = 0;

	status = elite_nand_ready();
	if (status)
		writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);

	status = elite_nfc_wait_idle(mtd, 1, 1, -1, -1);
	if (status)
		printf("NAND_CMD_ERASE2 ERROR : Time out waiting for NAND flash controller idle!\n");

	LEAVE();
	if(command==NAND_CMD_PAGEPROG)
	{
	    int i=0;
	    while(i>>23)
	    {
	        i++;
	    }
	}
	return;
}

static int nand_cmd_status_func(struct mtd_info *mtd, unsigned char cmd)
{
	int cfg = 0, status = -1;
	unsigned int b2r_stat;
	struct nand_chip *nand = (struct nand_chip *)mtd->priv;
	struct elite_nand_priv *priv = (struct elite_nand_priv *)nand->priv;
	ENTER();
	writel(cmd, ctrl_base + ELITE_NFC_REG_2);
	cfg = SP_CMD_EN|DPAHSE_DISABLE|NFC2NAND|(1<<1);	

	b2r_stat = readb(ctrl_base + ELITE_NFC_REG_B);
	writeb(b2r_stat | B2R, ctrl_base + ELITE_NFC_REG_B);

	writel(cfg | NFC_TRIGGER, ctrl_base + ELITE_NFC_REG_1);
	status = elite_nfc_transfer_ready();
	if (status)
	{
		writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);
		LEAVE();
		return status;
	}
	b2r_stat = readb(ctrl_base + ELITE_NFC_REG_B);
	writeb(b2r_stat | B2R, ctrl_base + ELITE_NFC_REG_B);

	cfg = SING_RW|NAND2NFC;
	writel(cfg | NFC_TRIGGER, ctrl_base + ELITE_NFC_REG_1);

	status = elite_nfc_transfer_ready();
	if (status)
	{
		writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);
		LEAVE();
		return status;
	}

	status = elite_nfc_wait_idle(mtd, 1, -1, -1, -1);

	if (status)
	{
		writew(readw(ctrl_base + ELITE_NFC_REG_23) | 0x100, ctrl_base + ELITE_NFC_REG_23);
		LEAVE();
		return status;
	}

	priv->datalen = 0;
	((unsigned char *)priv->dmabuf)[0] = readb(ctrl_base + ELITE_NFC_REG_0);

	status = ((unsigned char *)priv->dmabuf)[0];
	
	LEAVE();
	return  status;
}

static int nand_cmd_reset_func(struct mtd_info *mtd, unsigned command)
{
	int b2r_stat, status;
	struct nand_chip *chip = mtd->priv;
	ENTER();

	udelay(chip->chip_delay);

	writel(command, ctrl_base + ELITE_NFC_REG_2);

	b2r_stat = readb(ctrl_base + ELITE_NFC_REG_B);
	writeb(b2r_stat | B2R, ctrl_base + ELITE_NFC_REG_B);

	writel(SP_CMD_EN|DPAHSE_DISABLE|(1<<1)|NFC_TRIGGER, ctrl_base + ELITE_NFC_REG_1);

	status = elite_nand_ready();
	if (status)
		writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);

	nand_cmd_status_func(mtd, NAND_CMD_STATUS);

	while(!(readb(ctrl_base + ELITE_NFC_REG_0) & 0x40))
		;

	LEAVE();
	return 0;
}

static void nand_cmd_rndout_func(struct mtd_info *mtd, unsigned cmd, unsigned int cycle, int col)
{
	int b2r_stat, column = col;
	unsigned command = cmd, addr_cycle = cycle;
	ENTER();
	if (column != -1)
	{
		writeb(column, ctrl_base + ELITE_NFC_REG_3);
		writeb(column, (unsigned char *)(ctrl_base + ELITE_NFC_REG_3) + 1);
		addr_cycle += 2;
	}

	/* clear ECC bit*/
	writeb(0x07, ctrl_base + ELITE_NFC_REG_1F);
	writel(0xffffffff, ctrl_base + ELITE_NFC_REG_20);

	/* write to clear B2R */
	b2r_stat = readb(ctrl_base + ELITE_NFC_REG_B);
	writeb(b2r_stat | 0x08, ctrl_base + ELITE_NFC_REG_B);

	/* set command 1 cycle */
	writeb(command, ctrl_base + ELITE_NFC_REG_2);
	writel(SP_CMD_EN|DPAHSE_DISABLE | ((addr_cycle + 1) << 1) | 0x01, ctrl_base + ELITE_NFC_REG_1);

	elite_wait_cmd_ready();
	writeb(NAND_CMD_RNDOUTSTART, ctrl_base + ELITE_NFC_REG_2);

	b2r_stat = readb(ctrl_base + ELITE_NFC_REG_B);
	writeb(b2r_stat | 0x08, ctrl_base + ELITE_NFC_REG_B);
	writel(0x40 | (1 << 1) | 0x01, ctrl_base + ELITE_NFC_REG_1);

	elite_wait_cmd_ready();
	elite_nfc_wait_idle(mtd, 0, -1, -1, -1);
	
	LEAVE();
	return;
}

#ifndef DEBUG_FPGA
static int elite_calc_clock(unsigned int spec_clk, int *t, int *divisor, int *thold)
{
	unsigned int i, div1=0, div2, clk1, clk2=0, comp, t1=0, t2=0, clk, pllb;
	unsigned int t_rea,t_reh, thold_2, t_tmp, t_adl, t_wp;
       ENTER();

	pllb = *(volatile unsigned int *)PMPMB_ADDR;
	pllb = (((pllb>>16)&0xFF)+1)/((((pllb>>8)&0x3F)+1)*(1<<(pllb&7)));
	
	NAND_PRINTF("PLLB=0x%x, spec_clk=0x%x\n", pllb, spec_clk);
	
	t_rea = (spec_clk>>24)&0xFF;
	t_reh = (spec_clk>>16)&0xFF;
	t_wp  = (spec_clk>>8)&0xFF;
	t_adl = spec_clk&0xFF;
	for (i = 1; i < 16; i++) 
	{
		if (MAX_SPEED_MHZ >= (pllb*SOURCE_CLOCK)/i) 
		{
			div1 = i;
			break;
		}
	}

	clk1 = (1000 * div1)/(pllb*SOURCE_CLOCK);
	for (t1 = 1; t1 < 10; t1++) 
	{
		if ((t1*clk1) >= (t_rea + MAX_READ_DELAY))
			break;
	}
	i = 1;
	while (i*clk1 <= t_reh) 
	{
		i++;
	}
	*thold = i;
	NAND_PRINTF("T1=%d, clk1=%d, div1=%d, Thold=%d, tREA=%d+delay(%d)\n", t1, clk1, div1, *thold, t_rea, MAX_READ_DELAY);
	t_tmp = *t = t1;
	clk = clk1;
	*divisor = div1;
	div2 = div1;
	while (t_tmp > 1 && clk != 0) 
	{
		div2++;
		clk2 = (1000 * div2)/(pllb*SOURCE_CLOCK);
		comp = 0;
		for (t2 = 1; t2 < t_tmp; t2++) 
		{
			if ((t2*clk2) >= (t_rea + MAX_READ_DELAY)) 
			{
				t_tmp = t2;
				comp = 1;
				i = 1;
				while (i*clk2 <= t_reh) 
				{
					i++;
				}
				thold_2 = i;
				NAND_PRINTF("T2=%d, clk2=%d, div2=%d, Thold2=%d, comp=1\n", t2, clk2, div2, thold_2);
				break;
			}
		}
		if (comp == 1) 
		{
			clk1 = clk * (*t+*thold) * NAND_BYTES_PER_PAGE;
			div1 = clk2 * (t2+thold_2) * NAND_BYTES_PER_PAGE;
			NAND_PRINTF("Tim1=%d , Tim2=%d\n", clk1, div1);
			if ((clk * (*t+*thold) * NAND_BYTES_PER_PAGE) > (clk2 * (t2+thold_2) * NAND_BYTES_PER_PAGE)) 
			{
				*t = t2;
				clk = clk2;
				*divisor = div2;
				*thold = thold_2;
			} 
			else 
			{
				NAND_PRINTF("t2 is greater and not use\n");
			}
		}
	} /* end of while */

	i = 1;
	*thold |= 0x100; /* set write setup/hold time */
	while (((i*2+2)*clk) <= (t_adl-t_wp) || (i*clk) <= (t_wp+MAX_WRITE_DELAY)) 
	{
		*thold += 0x100;
		i++;
	}
	
	NAND_PRINTF("T=%d, clk=%d, divisor=%d, Thold=0x%x\n", *t, clk, *divisor, *thold);
	if ((MAX_SPEED_MHZ < (pllb*SOURCE_CLOCK)/(*divisor)) || clk == 0 || *t == 0 || clk > 45)
	{
		LEAVE();
		return 1;
	}

	LEAVE();
	return 0;
}

#endif

static void nfc_init(void)
{
	unsigned int i;
	unsigned int spec_clk = RW_TIMING, t, thold, divisor, status = 0;
	uint8_t block_size_select = (NAND_PAGES_PER_BLOCK >> 5);
	uint8_t page_size_select = 0;
	uint32_t nfc_rw_timming = 0x2424;
	uint32_t nfc_clock_mask = 0x1ff;
	
	ENTER();
	
	//writel(0x00000020,ctrl_base+ ELITE_NFC_REG_22);
	writel(0x800, ctrl_base + ELITE_NFC_REG_10);
	if(NAND_ECC_MODE==8)
	{
		writel(0x1042, ctrl_base + ELITE_NFC_REG_10);
	}
	else if(NAND_ECC_MODE==12)
	{
		writel(0x1422, ctrl_base + ELITE_NFC_REG_10);
	}
	else if(NAND_ECC_MODE==24)
	{
		writel(0x2A2E, ctrl_base + ELITE_NFC_REG_10);
	}
	else if(NAND_ECC_MODE==40)
	{
		writel(0x2A0E, ctrl_base + ELITE_NFC_REG_10);
	}
	writel(0, ctrl_base + ELITE_NFC_REG_22);
	writel(0xF<<4, ctrl_base + ELITE_NFC_REG_D);
	writeb(0, ctrl_base + ELITE_NFC_REG_13);

	writel(readl(ctrl_base + ELITE_NFC_REG_14) & 0xffff0000, ctrl_base + ELITE_NFC_REG_14);
	writel(readl(ctrl_base + ELITE_NFC_REG_14) | nfc_rw_timming, ctrl_base + ELITE_NFC_REG_14);	

	i = 0;
	do	
	{
		if ((block_size_select >> i) == 0)
		{
			break;
		}
		else
			i++;
	}while(1);

	if (i <= 5)
		writel((i << 5) | 0x1f, ctrl_base + ELITE_NFC_REG_17);
	else
		printf("ERROR : Unsupported NAND flash for its number of pages in one block!\n");

	page_size_select = (NAND_BYTES_PER_PAGE > 512) ? (log2(NAND_BYTES_PER_PAGE >> 10)) : 0;
	writeb(page_size_select|WIDTH_8|WP_DISABLE, ctrl_base + ELITE_NFC_REG_12);

#ifndef DEBUG_FPGA
	status = elite_calc_clock(spec_clk, &t, &divisor, &thold);
	if (status)
	{
		LEAVE();
		return;
	}

	nfc_rw_timming = ((thold & 0xFF) << 12) + ((t + (thold & 0xFF)) << 8) + (((thold >> 8) & 0xFF) << 4) + 2*((thold >> 8) & 0xFF);
	*(volatile unsigned long *)PMNAND_ADDR = (divisor & nfc_clock_mask);
	while ((*(volatile unsigned long *)PMCS_ADDR) & (1 << 25))
		;

	divisor = *(volatile unsigned long *)PMNAND_ADDR;
	writel(readl(ctrl_base + ELITE_NFC_REG_14) & 0xffff0000, ctrl_base + ELITE_NFC_REG_14);
	writel(readl(ctrl_base + ELITE_NFC_REG_14) | nfc_rw_timming, ctrl_base + ELITE_NFC_REG_14);
#endif	

	/* chip disable */
	writeb(0xff, ctrl_base + ELITE_NFC_REG_11);

	LEAVE();
}

static int dma_init(void)
{
	ENTER();
	writel(NAND_PDMA_GCR_SOFTRESET, pdma_base + ELITE_NFC_DMA_GCR);
	writel(NAND_PDMA_GCR_DMA_EN, pdma_base + ELITE_NFC_DMA_GCR);

	if ((readl(pdma_base + ELITE_NFC_DMA_GCR)) & NAND_PDMA_GCR_DMA_EN)
	{
		LEAVE();
		return 0;
	}
	else
	{
		LEAVE();
		return 1;
	}
	
}

/* 
* The descriptor of 4 double-word size is where real data buffer address is situated.
* The address of this descriptor should be written to 'DMA Memory-Register Pointer Register (offset 0x10c)'.
*/
static void dma_init_desc(unsigned long *desc_addr, unsigned int reqcount, unsigned long *buffer,
				unsigned long *branch_addr, int end)
{
	nand_pdma_desc_long *desc;
	ENTER();
	desc = (nand_pdma_desc_long *)desc_addr;
	desc->req_count = reqcount;
	desc->i = 0;
	desc->format = 1;
	desc->data_buffer_addr = (unsigned long)buffer;
	desc->branch_addr = (unsigned long)branch_addr;
	if (end)
	{
		desc->end = 1;
		desc->i = 1;
	}
	LEAVE();
}

static void dma_init_addr(unsigned long *dma_addr, unsigned int dir)
{
	ENTER();
	writel((unsigned long)NAND_PDMA_IER_INT_EN, pdma_base + ELITE_NFC_DMA_IER);

	writel((unsigned long)dma_addr, pdma_base + ELITE_NFC_DMA_DESPR);
	/* 
	* set direction of DMA operation 
	* read: peripheral to IF0
	* write: IF0 to periferal
	*/
	if (dir == NAND_PDMA_READ) 
		writel(readl(pdma_base + ELITE_NFC_DMA_CCR) | NAND_PDMA_CCR_PERIPHERAL_TO_IF, pdma_base + ELITE_NFC_DMA_CCR);
	else //write
		writel(readl(pdma_base + ELITE_NFC_DMA_CCR) & (~NAND_PDMA_CCR_IF_TO_PERIPHERAL), pdma_base + ELITE_NFC_DMA_CCR);

	writel(readl(pdma_base + ELITE_NFC_DMA_CCR) | NAND_PDMA_CCR_RUN, pdma_base + ELITE_NFC_DMA_CCR);
	LEAVE();
	return;

}

static void dma_free(void)
{
	ENTER();
	writel(0, pdma_base + ELITE_NFC_DMA_DESPR);
	writel(0, pdma_base + ELITE_NFC_DMA_GCR);
	LEAVE();
}

int nand_dma_config(struct mtd_info *mtd, unsigned int len, unsigned int wr, int data_flag, int Nbank)
{
	int i, status;
	unsigned long *dma_addr;
	struct nand_chip *nand = (struct nand_chip *)mtd->priv;
	struct elite_nand_priv *priv = (struct elite_nand_priv *)nand->priv;
	unsigned long *read_desc, *write_desc;
	ENTER();

	read_desc = (unsigned long *)(priv->dmabuf + mtd->writesize + 0x100);
	write_desc = (unsigned long *)(priv->dmabuf + mtd->writesize + 0x200);
	if (len == 0)
	{
		LEAVE();
		return 1;
	}
	writel(len - 1, ctrl_base + ELITE_NFC_REG_8);

	if (readl(pdma_base + ELITE_NFC_DMA_ISR) & NAND_PDMA_IER_INT_STS)
		writel(NAND_PDMA_IER_INT_STS, pdma_base + ELITE_NFC_DMA_ISR);
	if ((readl(pdma_base + ELITE_NFC_DMA_ISR) )& NAND_PDMA_IER_INT_STS) 
	{
		NAND_PRINTF("PDMA interrupt status can't be clear ");
		NAND_PRINTF("pNand_PDma_Reg->DMA_ISR = 0x%8.8x \n", pdma_base + ELITE_NFC_DMA_ISR);
	}

	status = dma_init();
	if (status)
	{
		printf("ERROR : DMA controller cannot be enabled!\n");
	}

	memset((wr)?write_desc:read_desc, 0, 0x100);
	dma_init_desc((wr)?write_desc:read_desc, len, (unsigned long *)priv->dmaaddr, 0, 1);

	dma_addr = (wr) ? (unsigned long *)(priv->dmaaddr + mtd->writesize + 0x200) \
				: (unsigned long *)(priv->dmaaddr + mtd->writesize + 0x100);

	dma_init_addr(dma_addr, wr);
    flush_dcache_range(dma_addr,dma_addr+mtd->writesize + 0x300);
	LEAVE();
	return 0;
}

static void nand_pdma_handler(void)
{
	int status = 0;
	unsigned long count = 0;
	ENTER();
	count = 0x100000;

	do 
	{
		count--;
		if (readl(pdma_base + ELITE_NFC_DMA_ISR) & NAND_PDMA_IER_INT_STS)
		{
			status = readl(pdma_base + ELITE_NFC_DMA_CCR) & NAND_PDMA_CCR_EVT_CODE;
			writel(readl(pdma_base + ELITE_NFC_DMA_ISR) & NAND_PDMA_IER_INT_STS, pdma_base + ELITE_NFC_DMA_ISR);
			break;
		}

		if (count == 0)
			break;
	} while (1);

	if (status == NAND_PDMA_CCR_EVT_FF_UNDERRUN)
		printf("ERROR : PDMA buffer under run!\n");
	else if (status == NAND_PDMA_CCR_EVT_FF_OVERRUN)
		printf("ERROR : PDMA buffer over run!\n");
	else if (status == NAND_PDMA_CCR_EVT_DESP_READ)
		printf("ERROR : PDMA read descriptor error!\n");
	else if (status == NAND_PDMA_CCR_EVT_DATA_RW)
		printf("ERROR : PDMA read/write memory descriptor error!\n");
	else if (status == NAND_PDMA_CCR_EVT_EARLY_END)
		printf("ERROR : PDMA read early end!\n");

	if (count == 0)
	{
		printf("ERROR : PDMA time out!\n");
		while(1) ;
	}
	LEAVE();
	return;
}

static int elite_nand_chip_ready(struct mtd_info *mtd)
{
	return readb(ctrl_base + ELITE_NFC_REG_A) & FLASH_RDY;
}

static void elite_nand_readbuf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	struct nand_chip *nand = (struct nand_chip *)mtd->priv;
	struct elite_nand_priv *priv = (struct elite_nand_priv *)nand->priv;
	ENTER();
	memcpy(buf, (unsigned char *)priv->dmabuf + priv->datalen, len);
	priv->datalen += len;
	LEAVE();
	return;
}

static u_char elite_nand_readbyte(struct mtd_info *mtd)
{
	uint8_t byte = 0;
	ENTER();
	elite_nand_readbuf(mtd, &byte, 1);
	LEAVE();
	return byte;
}

static void elite_nand_writebuf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	struct nand_chip *nand = (struct nand_chip *)mtd->priv;
	struct elite_nand_priv *priv = (struct elite_nand_priv *)nand->priv;
	ENTER();
	memcpy((unsigned char *)priv->dmabuf + priv->datalen, buf, len);
	priv->datalen += len;
	LEAVE();
	return;
}

static void elite_nand_select(struct mtd_info *mtd, int chip)
{
	#if 0
	unsigned bsy_rdy;
	ENTER();
	/* clear status bit which indicats NAND flash chaning from busy to ready */
	bsy_rdy = readb(ctrl_base + ELITE_NFC_REG_B);
	writeb(0x08 | bsy_rdy, ctrl_base + ELITE_NFC_REG_B); 
    NAND_PRINTF("chip:%d\n",chip);

	/* enable NAND flash */
	writeb(0&0x0f,ctrl_base + ELITE_NFC_REG_11);

	LEAVE();
	#endif
	unsigned bsy_rdy;
	 struct nand_chip *this = mtd->priv;
	 struct elite_nand_priv *info = (struct elite_nand_priv *)this->priv;
 
	 ENTER();
	 /* clear status bit which indicats NAND flash chaning from busy to ready */
	 bsy_rdy = readb(ctrl_base + ELITE_NFC_REG_B);
	 writeb(0x08 | bsy_rdy, ctrl_base + ELITE_NFC_REG_B); 
  	   NAND_PRINTF("chip:%d\n",chip);
 	 info->cur_chip_n=chip;
 
 	if (chip == -1)
 	{
 	   writeb(0xff,ctrl_base + ELITE_NFC_REG_11);
 	}
 	else if ((chip >= 0) && (chip <= 7))
 	{
 	 /* enable NAND flash */
 	 writeb(chip&0x0f,ctrl_base + ELITE_NFC_REG_11);
	 }
 	LEAVE();

	return;
}


static void disable_redunt_out_bch_ctrl(int flag)
{
	ENTER();
	if (flag == 1)
		writeb(readb(ctrl_base + ELITE_NFC_REG_D) | 0x02, ctrl_base + ELITE_NFC_REG_D);
	else	
		writeb(readb(ctrl_base + ELITE_NFC_REG_D) & 0xfd, ctrl_base + ELITE_NFC_REG_D);
	LEAVE();
}

/*
* elite_nand_cmdfunc - Send command to NAND large page device
* @mtd:           MTD device info structure
* @command:   the command to be deliverred to device
* @column:      the column address for this command
* @page_addr:  the page address for this command
* 
* Send command to NAND device. This is the version for the new large page devices.
*/
static void elite_nand_cmdfunc(struct mtd_info *mtd, unsigned command, int column, int page_addr)
{
	struct nand_chip *this = mtd->priv;
	struct elite_nand_priv *info = (struct elite_nand_priv *)this->priv;

	unsigned int addr_cycle = 0, b2r_stat;
	int status = -1;
	unsigned int ecc_err_pos, redunt_stat, bank_stat1;
	int readcmd;
	int mycolumn = column, mypage_addr = page_addr;
	int cur_chip = info->cur_chip_n;
	ENTER();
	NAND_PRINTF("writesize:0x%x,command:0x%x,column:0x%x,page_addr:0x%x\n",mtd->writesize,command,column,page_addr);
	test_page=page_addr;
	switch (command) 
	{
		case NAND_CMD_READOOB:
		{
			int failed=mtd->ecc_stats.failed;
			mycolumn = column=(NAND_BANK_SIZE+this->ecc.bytes)*(this->ecc.steps-1);
			writel(readl((volatile char *)ctrl_base + ELITE_NFC_REG_23) |0x10, (volatile char *)ctrl_base + ELITE_NFC_REG_23);
			dump_count=0;
read_oob_again:
			
			dump_count++;

			addr_cycle = 0;
			/* command to read data area */
			if (mtd->writesize == 512)
			{
				b2r_stat = readb(ctrl_base + ELITE_NFC_REG_B);
				writeb(b2r_stat | B2R, ctrl_base + ELITE_NFC_REG_B);
				writeb(cur_chip&0x0f,ctrl_base + ELITE_NFC_REG_11);

			}
			nand_dma_config(mtd, this->ecc.size, 0, -1, -1);

			info->datalen = 0;
			b2r_stat = readb(ctrl_base + ELITE_NFC_REG_B);
			writeb(b2r_stat | B2R, ctrl_base + ELITE_NFC_REG_B);

			addr_cycle= nand_address(mtd, addr_cycle, column,  page_addr);

			bank_stat1 = readw(ctrl_base + ELITE_NFC_REG_25);
			writew(bank_stat1 | (ERR_CORRECT | BCH_ERR), ctrl_base + ELITE_NFC_REG_25);
			 writeb(0x0,ctrl_base + ELITE_NFC_REG_9);

			if (mtd->writesize == 512)
			{
				writeb(command, ctrl_base + ELITE_NFC_REG_2);
				
				if (addr_cycle == 4)
					writeb(NAND_CMD_READSTART, ctrl_base + ELITE_NFC_REG_5);
				else if (addr_cycle == 5)
					writeb(NAND_CMD_READSTART, (unsigned char *)(ctrl_base + ELITE_NFC_REG_5) + 1);

				writel(SP_CMD_EN|DATA_READ_EN|END_WITH_CMD_EN | (addr_cycle + 2) << 1 | NFC_TRIGGER, ctrl_base + ELITE_NFC_REG_1);
				status = elite_nand_ready();
				if (status)
				{
					writeb(readb(ctrl_base + ELITE_NFC_REG_9) & 0xfd, ctrl_base + ELITE_NFC_REG_9);
				}

				status = elite_nfc_transfer_ready();
				if (status)
				{
					writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);
					writeb(readb(ctrl_base + ELITE_NFC_REG_9) & 0xfd, ctrl_base + ELITE_NFC_REG_9);
				}			

				status = elite_nfc_wait_idle(mtd, 1, -1, -1, -1);
				if (status)
				{
					writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);
					writeb(readb(ctrl_base + ELITE_NFC_REG_9) & 0xfd, ctrl_base + ELITE_NFC_REG_9);
				}
		
				writel(readl((volatile char *)ctrl_base + ELITE_NFC_REG_23) & ((~0x10)&0xFFFF), (volatile char *)ctrl_base+ ELITE_NFC_REG_23);
				nand_pdma_handler();
				dma_free();
				
				/* disable current chip */
				writeb(cur_chip|0xF0,ctrl_base + ELITE_NFC_REG_11);

			}
			else
			{
				writeb(cur_chip&0x0f,ctrl_base + ELITE_NFC_REG_11);
				elite_wait_chip_ready();
				writeb(NAND_CMD_READ0, ctrl_base + ELITE_NFC_REG_2);

				if (addr_cycle == 4)
					writeb(NAND_CMD_READSTART, ctrl_base + ELITE_NFC_REG_5);
				else if (addr_cycle == 5)
					writeb(NAND_CMD_READSTART, (unsigned char *)(ctrl_base + ELITE_NFC_REG_5) + 1);

				writel(SP_CMD_EN|DATA_READ_EN|END_WITH_CMD_EN | (addr_cycle + 2) << 1 | NFC_TRIGGER, ctrl_base + ELITE_NFC_REG_1);
				status = elite_nand_ready();
			

				if (status)
				{
					/* write 1 into the bit to let nfc continue to read next bank from the device */
					writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);
				}
				elite_wait_nfc_ready();
				status = elite_nfc_wait_idle(mtd, 0, command, column, mypage_addr);
			

				if (status)
				{
					printf("NAND_CMD_READ0 ERROR : Time out waiting for NAND flash controlling idle!\n");
					writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);
					if (status == -2)
					{	
						if(dump_count <=3)
						{
							goto read_oob_again;
						}
						else
						{
							printf("ECC error exceed dump count!\n");
						}
					}
					LEAVE();
					return;
				}
				writel(readl((volatile char *)ctrl_base + ELITE_NFC_REG_23) & ((~0x10)&0xFFFF), (volatile char *)ctrl_base+ ELITE_NFC_REG_23);

				/* read both data and redundant data */
				nand_pdma_handler();
				dma_free();
			
			}
			{
				int i=0;
				uint8_t *buf = info->dmabuf + mtd->writesize;
				memcpy(buf, ctrl_base+ECC_FIFO_0, 64);
				memcpy((uint8_t *) this->oob_poi, ctrl_base+ECC_FIFO_0, 64);
				buf = info->dmabuf;
				memcpy(buf, ctrl_base+ECC_FIFO_0, 64);	
				writeb(readb(ctrl_base + ELITE_NFC_REG_9) & 0xfd,ctrl_base + ELITE_NFC_REG_9);
				for(i=0;i<64;i++)
				{
					NAND_PRINTF("page buf[%d]= 0x%x\n", i, buf[i]);
				}
			
			}
			if (mtd->writesize == 512)
			{
				redunt_stat = readb(ctrl_base + ELITE_NFC_REG_1F);
				if (redunt_stat & 0x05)
				{
					mtd->ecc_stats.failed++;
					LEAVE();
					return;
				}
				else if (redunt_stat & 0x02)
				{
					ecc_err_pos = readw(ctrl_base + ELITE_NFC_REG_1C);
					bit_correct((unsigned char *)(info->dmabuf + mtd->writesize) + (ecc_err_pos & 0x3f), (ecc_err_pos >> 8) & 0x07);
				}
				nand_hamming_ecc_1bit_correct(mtd);
			}
			else
			{
				nand_hamming_ecc_1bit_correct(mtd);
				
			}
			if(dump_count >1)
			{	 
			
				mtd->ecc_stats.failed=failed;
				printk("\tdump:%d page:0x%x,column:0x%x\n",dump_count,page_addr,column);
			
			}
			LEAVE();
			return;
		}
		case NAND_CMD_READ0:
		{

			int failed=mtd->ecc_stats.failed;
			dump_count=0;
read_page_again:			

			dump_count++;
			addr_cycle = 0;
				
			/* command to read data area */
			if (mtd->writesize == 512)
			{
				b2r_stat = readb(ctrl_base + ELITE_NFC_REG_B);
				writeb(b2r_stat | B2R, ctrl_base + ELITE_NFC_REG_B);
				writeb(cur_chip&0x0f,ctrl_base + ELITE_NFC_REG_11);

			}
			nand_dma_config(mtd, mtd->writesize, 0, -1, -1);

			info->datalen = 0;
			b2r_stat = readb(ctrl_base + ELITE_NFC_REG_B);
			writeb(b2r_stat | B2R, ctrl_base + ELITE_NFC_REG_B);

			addr_cycle= nand_address(mtd, addr_cycle, column,  page_addr);

			bank_stat1 = readw(ctrl_base + ELITE_NFC_REG_25);
			writew(bank_stat1 | (ERR_CORRECT | BCH_ERR), ctrl_base + ELITE_NFC_REG_25);
		         writeb(0x0,ctrl_base + ELITE_NFC_REG_9);

			if (mtd->writesize == 512)
			{
				writeb(command, ctrl_base + ELITE_NFC_REG_2);			
				if (addr_cycle == 4)
					writeb(NAND_CMD_READSTART, ctrl_base + ELITE_NFC_REG_5);
				else if (addr_cycle == 5)
					writeb(NAND_CMD_READSTART, (unsigned char *)(ctrl_base + ELITE_NFC_REG_5) + 1);

				writel(SP_CMD_EN|DATA_READ_EN|END_WITH_CMD_EN | (addr_cycle + 2) << 1 | NFC_TRIGGER, ctrl_base + ELITE_NFC_REG_1);
				status = elite_nand_ready();
				if (status)
				{
					writeb(readb(ctrl_base + ELITE_NFC_REG_9) & 0xfd, ctrl_base + ELITE_NFC_REG_9);
				}
				status = elite_nfc_transfer_ready();
				if (status)
				{
					writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);
					writeb(readb(ctrl_base + ELITE_NFC_REG_9) & 0xfd, ctrl_base + ELITE_NFC_REG_9);
				}			

				status = elite_nfc_wait_idle(mtd, 1, -1, -1, -1);
				if (status)
				{
					writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);
					writeb(readb(ctrl_base + ELITE_NFC_REG_9) & 0xfd, ctrl_base + ELITE_NFC_REG_9);
				}				
				nand_pdma_handler();
				dma_free();
				
				/* disable current chip */
				writeb(cur_chip|0xF0,ctrl_base + ELITE_NFC_REG_11);

			}
			else
			{
				writeb(cur_chip&0x0f,ctrl_base + ELITE_NFC_REG_11);

				elite_wait_chip_ready();
				writeb(NAND_CMD_READ0, ctrl_base + ELITE_NFC_REG_2);

				if (addr_cycle == 4)
					writeb(NAND_CMD_READSTART, ctrl_base + ELITE_NFC_REG_5);
				else if (addr_cycle == 5)
					writeb(NAND_CMD_READSTART, (unsigned char *)(ctrl_base + ELITE_NFC_REG_5) + 1);

				writel(SP_CMD_EN|DATA_READ_EN|END_WITH_CMD_EN | (addr_cycle + 2) << 1 | NFC_TRIGGER, ctrl_base + ELITE_NFC_REG_1);
				status = elite_nand_ready();
			
				if (status)
				{
					/* write 1 into the bit to let nfc continue to read next bank from the device */
					writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);
					LEAVE();	//
					return;		// nand not ready to exit the command
				}
				elite_wait_nfc_ready();
				status = elite_nfc_wait_idle(mtd, 0, command, column, mypage_addr);
				if (status)
				{
					printf("NAND_CMD_READ0 ERROR : Time out waiting for NAND flash controlling idle!\n");
					writew(readw(ctrl_base + ELITE_NFC_REG_23) | READ_RESUME, ctrl_base + ELITE_NFC_REG_23);
					if(status ==-2)
					{
						if(dump_count <=3)
						{
							goto read_page_again;
						}
						else
						{
							printf("ECC error exceed dump count!\n");
						}
					}
					LEAVE();
					return;
				}
				/* read both data and redundant data */
				nand_pdma_handler();
				dma_free();
				
			}
			{
				int i=0;
				uint8_t *buf = info->dmabuf + mtd->writesize;
				memcpy(buf, ctrl_base+ECC_FIFO_0, 64);
				if(mtd->writesize >=8192)
				{	
					writel(readl((volatile char *)ctrl_base + ELITE_NFC_REG_D) | 0x08, (volatile char *)ctrl_base + ELITE_NFC_REG_D);
					memcpy(buf+ 64,(volatile char *)ctrl_base+ECC_FIFO_0, 64);
					writel(readl((volatile char *)ctrl_base + ELITE_NFC_REG_D) & 0xF7, (volatile char *)ctrl_base + ELITE_NFC_REG_D);
				}
				memcpy((uint8_t *) this->oob_poi, ctrl_base+ECC_FIFO_0, 64);
				if(mtd->writesize >=8192)
				{
					writel(readl((volatile char *)ctrl_base + ELITE_NFC_REG_D) | 0x08, (volatile char *)ctrl_base + ELITE_NFC_REG_D);
					memcpy((uint8_t *) this->oob_poi+64,(volatile char *)ctrl_base+ECC_FIFO_0, 64);
					writel(readl((volatile char *)ctrl_base + ELITE_NFC_REG_D) & 0xF7, (volatile char *)ctrl_base + ELITE_NFC_REG_D);
				}
				writeb(readb(ctrl_base + ELITE_NFC_REG_9) & 0xfd,ctrl_base + ELITE_NFC_REG_9);
				for(i=0;i<64;i++)
				{
					NAND_PRINTF("page buf[%d]= 0x%x\n", i, buf[i]);
				}
			
			}
			if (mtd->writesize == 512)
			{
				redunt_stat = readb(ctrl_base + ELITE_NFC_REG_1F);
				if (redunt_stat & 0x05)
				{
					mtd->ecc_stats.failed++;
					LEAVE();
					return;
				}
				else if (redunt_stat & 0x02)
				{
					ecc_err_pos = readw(ctrl_base + ELITE_NFC_REG_1C);
					bit_correct((unsigned char *)(info->dmabuf + mtd->writesize) + (ecc_err_pos & 0x3f), (ecc_err_pos >> 8) & 0x07);
				}
				nand_hamming_ecc_1bit_correct(mtd);
			}
			else
			{
				nand_hamming_ecc_1bit_correct(mtd);
				
			}
			if(dump_count >1)
			{   
				mtd->ecc_stats.failed=failed;
				printk("\tdump:%d page:0x%x,column:0x%x\n",dump_count,page_addr,column);
			}	  
			LEAVE();
			return;
		}
		case NAND_CMD_SEQIN:
		{
			if (mtd->writesize == 512)
			{
				if (column >= mtd->writesize)
				{
					column -= mtd->writesize;
					readcmd = NAND_CMD_READOOB;
				}
				else if (column < 256)
				{
					readcmd = NAND_CMD_READ0;
				}
				else 
				{	
					column -= 256;
					readcmd = NAND_CMD_READ1;
				}
				writeb(readcmd, ctrl_base + ELITE_NFC_REG_2);
				writeb((1 << 7) | (1 << 1) | 1, ctrl_base + ELITE_NFC_REG_2);
				elite_wait_nfc_ready();
			}	
		}
		case NAND_CMD_ERASE1:
		{
			nand_cmd_erase1_func(mtd, addr_cycle, command, column, page_addr);
			LEAVE();
			return;
		}
		case NAND_CMD_READID:
		{
			nand_cmd_readid_func(mtd);
			LEAVE();
			return;
		}
		case NAND_CMD_PARAM:
		{
			nand_cmd_get_paramter_func(mtd,column);
			LEAVE();
			return;
		}
		case NAND_CMD_PAGEPROG:
		case NAND_CMD_ERASE2:
		{
			nand_cmd_erase2_func(mtd, command);
			LEAVE();
			return;
		}
		case NAND_CMD_STATUS:
		{
			nand_cmd_status_func(mtd, command);
			LEAVE();
			return;
		}
		case NAND_CMD_RESET:
		{
			status = nand_cmd_reset_func(mtd, command);
			if (status == 1)
				goto done;
			LEAVE();
			return;
		}
		case NAND_CMD_RNDIN:
		{
			if (column != -1)
			{
				writeb(column, ctrl_base + ELITE_NFC_REG_3);
				writeb(column, (unsigned char *)(ctrl_base + ELITE_NFC_REG_3) + 1);
				addr_cycle += 2;
			}

			writeb(command, ctrl_base + ELITE_NFC_REG_2);
			writel(0x80 | ((addr_cycle + 1) << 1) | 0x01, ctrl_base + ELITE_NFC_REG_1);
			status = elite_nfc_wait_idle(mtd, 1, -1, -1, -1);
			if (status)
				printf("NAND_CMD_RNDIN ERROR : Time out waiting for NAND flash controller idle!\n");
			LEAVE();
			return;
		}
		case NAND_CMD_RNDOUT:
		{
			nand_cmd_rndout_func(mtd, command, addr_cycle, column);
			LEAVE();
			return;
		}
		case NAND_CMD_STATUS_ERROR:
		case NAND_CMD_STATUS_ERROR0:
		{
			udelay(this->chip_delay);
			LEAVE();
			return;
		}
		default:
		if (!this->dev_ready)
		{
			udelay(this->chip_delay);
			LEAVE();
			return;
		}
	}

done:
	LEAVE();
	return;
}

int board_nand_init(struct nand_chip *nand)
{
	if (chip_n >= MAX_CHIPS)
	{
		LEAVE();
		return -ENODEV;
	}
	ENTER();
#ifdef CONFIG_SYS_NAND_USE_FLASH_BBT
	nand->bbt_options  |= NAND_BBT_USE_FLASH;
#endif
	ctrl_base = elite_get_nfc_regs_base();
	pdma_base = elite_get_pdma_regs_base();

	nand->cmdfunc       = elite_nand_cmdfunc;
	nand->dev_ready     = elite_nand_chip_ready;
	nand->read_byte     = elite_nand_readbyte;
	nand->read_buf      = elite_nand_readbuf;
	nand->write_buf     = elite_nand_writebuf;
	nand->select_chip   = elite_nand_select;
	nand->chip_delay    = 20;
	nand->options       = NAND_BBT_LASTBLOCK | NAND_BBT_PERCHIP;
	nand->page_shift =11;

	nand_priv[chip_n].dmabuf = dma_alloc_coherent(NAND_BYTES_PER_PAGE + NAND_OOB_SIZE + 0x300,&(nand_priv[chip_n].dmaaddr));
	if (nand_priv[chip_n].dmabuf == NULL)
		printf("ERROR : Memory allocation for NAND DMA failed!\n");

	nand_priv[chip_n].datalen = 0;
	nand_priv[chip_n].cur_chip_n = chip_n;
	nand->priv = &nand_priv[chip_n];
	nfc_init();
	chip_n++;
#ifdef  CONFIG_ELITE_NAND_HWECC
	elite_nand_hwecc_init(nand);
#else
	nand->ecc.mode = NAND_ECC_SOFT;
#endif
	
	LEAVE();
	return 0;
}

