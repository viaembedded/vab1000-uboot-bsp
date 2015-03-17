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

#ifndef __NAND_H__
#define __NAND_H__

#define MAX_PRODUCT_NAME_LENGTH 0x20
#define NAND_TYPE_MLC 1
#define NAND_TYPE_SLC 0

#define	ELITE_NFC_REG_0           0x00 
#define	ELITE_NFC_REG_1           0x04 
#define	ELITE_NFC_REG_2           0x08 
#define	ELITE_NFC_REG_3           0x0c 
#define	ELITE_NFC_REG_4           0x10 
#define	ELITE_NFC_REG_5           0x14 
#define	ELITE_NFC_REG_6           0x18 
#define	ELITE_NFC_REG_7           0x1c 
#define	ELITE_NFC_REG_8           0x20 
#define	ELITE_NFC_REG_9           0x24 
#define	ELITE_NFC_REG_A           0x28
#define	ELITE_NFC_REG_B           0x2c
#define	ELITE_NFC_REG_C           0x30
#define	ELITE_NFC_REG_D           0x34
#define	ELITE_NFC_REG_E           0x38
#define	ELITE_NFC_REG_F           0x3c
#define	ELITE_NFC_REG_10         0x40
#define	ELITE_NFC_REG_11         0x44
#define	ELITE_NFC_REG_12         0x48 
#define	ELITE_NFC_REG_13         0x4c 
#define	ELITE_NFC_REG_14         0x50 
#define	ELITE_NFC_REG_15         0x54 
#define	ELITE_NFC_REG_16         0x58 
#define	ELITE_NFC_REG_17         0x5c 
#define	ELITE_NFC_REG_18         0x60 
#define	ELITE_NFC_REG_19         0x64
#define	ELITE_NFC_REG_1A         0x68
#define	ELITE_NFC_REG_1B         0x6c
#define	ELITE_NFC_REG_1C         0x70
#define	ELITE_NFC_REG_1D         0x74
#define	ELITE_NFC_REG_1E         0x78
#define	ELITE_NFC_REG_1F         0x7c
#define	ELITE_NFC_REG_20         0x80
#define	ELITE_NFC_REG_21         0x84
#define	ELITE_NFC_REG_22         0x88
#define	ELITE_NFC_REG_23         0x8c
#define	ELITE_NFC_REG_24         0x90
#define	ELITE_NFC_REG_25         0x94
#define	ELITE_NFC_REG_26         0x98
#define	ELITE_NFC_REG_27         0x9c
#define	ELITE_NFC_REG_28         0xa0
#define	ELITE_NFC_REG_29         0xa4
#define	ELITE_NFC_REG_2A         0xa8
#define	ELITE_NFC_REG_2B         0xac
#define	ELITE_NFC_REG_2C         0xb0
#define	ELITE_NFC_REG_2D         0xb4
#define	ELITE_NFC_REG_2E         0xb8
#define	ELITE_NFC_REG_2F         0xbc
#define	ELITE_NFC_REG_30         0xc0
#define	ELITE_NFC_REG_31         0xc4
#define	ELITE_NFC_REG_32         0xc8


/*NAND PDMA*/
#define ELITE_NFC_DMA_GCR		0x00
#define ELITE_NFC_DMA_IER		0x04
#define ELITE_NFC_DMA_ISR		0x08
#define ELITE_NFC_DMA_DESPR	0x0C
#define ELITE_NFC_DMA_RBR		0x10
#define ELITE_NFC_DMA_DAR		0x14
#define ELITE_NFC_DMA_BAR		0x18
#define ELITE_NFC_DMA_CPR		0x1C
#define ELITE_NFC_DMA_CCR		0X20

/*NAND PDMA - DMA_GCR : DMA Global Control Register*/
#define NAND_PDMA_GCR_DMA_EN	0x00000001      /* [0] -- DMA controller enable */
#define NAND_PDMA_GCR_SOFTRESET	0x00000100      /* [8] -- Software rest */

/*NAND PDMA - DMA_IER : DMA Interrupt Enable Register*/
#define NAND_PDMA_IER_INT_EN		0x00000001      /* [0] -- DMA interrupt enable */

/*NAND PDMA - DMA_ISR : DMA Interrupt Status Register*/
#define NAND_PDMA_IER_INT_STS		0x00000001      /* [0] -- DMA interrupt status */

/*NAND PDMA - DMA_DESPR : DMA Descriptor base address Pointer Register*/
/*NAND PDMA - DMA_RBR : DMA Residual Bytes Register*/
#define NAND_PDMA_RBR_End			0x80000000      /* [31] -- DMA descriptor end flag */
#define NAND_PDMA_RBR_Format		0x40000000      /* [30] -- DMA descriptor format */

/*NAND PDMA - DMA_DAR : DMA Data Address Register*/
/*NAND PDMA - DMA_BAR : DMA Rbanch Address Register*/
/*NAND PDMA - DMA_CPR : DMA Command Pointer Register*/
/*NAND PDMA - DMA_CCR : DMAContext Control Register for Channel 0*/
#define NAND_PDMA_READ					0x00
#define NAND_PDMA_WRITE					0x01
#define NAND_PDMA_CCR_RUN				0x00000080
#define NAND_PDMA_CCR_IF_TO_PERIPHERAL   0x00000000
#define NAND_PDMA_CCR_PERIPHERAL_TO_IF	0x00400000
#define NAND_PDMA_CCR_EVT_CODE			0x0000000f
#define NAND_PDMA_CCR_EVT_NO_STATUS		0x00000000
#define NAND_PDMA_CCR_EVT_FF_UNDERRUN	0x00000001
#define NAND_PDMA_CCR_EVT_FF_OVERRUN	0x00000002
#define NAND_PDMA_CCR_EVT_DESP_READ		0x00000003
#define NAND_PDMA_CCR_EVT_DATA_RW		0x00000004
#define NAND_PDMA_CCR_EVT_EARLY_END		0x00000005
#define NAND_PDMA_CCR_EVT_SUCCESS		0x0000000f


#define ECC_FIFO_0   0x1c0
#define ECC_FIFO_1   0x1c4
#define ECC_FIFO_2   0x1c8
#define ECC_FIFO_3   0x1cc
#define ECC_FIFO_4   0x1d0
#define ECC_FIFO_5   0x1d4
#define ECC_FIFO_6   0x1d8
#define ECC_FIFO_7   0x1dc
#define ECC_FIFO_8   0x1e0
#define ECC_FIFO_9   0x1e4
#define ECC_FIFO_A   0x1e8
#define ECC_FIFO_B   0x1ec
#define ECC_FIFO_C   0x1f0
#define ECC_FIFO_D   0x1f4
#define ECC_FIFO_E   0x1f8
#define ECC_FIFO_F   0x1fc

/* PDMA Descriptor short*/
typedef struct {
	unsigned long volatile req_count:16;		  /* bit 0 -15 -Request count   */
	unsigned long volatile i:1;				  /* bit 16    -interrupt      */
	unsigned long volatile reserve:13;		  /* bit 17-29 -reserved     */
	unsigned long volatile format:1;		         /* bit 30    -The descriptor format    */
	unsigned long volatile end:1;			  /* bit 31    -End flag of descriptor list*/
	unsigned long volatile data_buffer_addr:32;/* bit 31    -Data Buffer address  */
} nand_pdma_desc_short;

/*PDMA Descriptor long*/
typedef struct 
{
	unsigned long volatile req_count:16;		  /* bit 0 -15 -Request count    */
	unsigned long volatile i:1;				  /* bit 16    -interrupt    */
	unsigned long volatile reserve:13;		  /* bit 17-29 -reserved      */
	unsigned long volatile format:1;		         /* bit 30    -The descriptor format     */
	unsigned long volatile end:1;			  /* bit 31    -End flag of descriptor list*/
	unsigned long volatile data_buffer_addr:32;/* bit 31-0  -Data Buffer address       */
	unsigned long volatile branch_addr:32;	   /* bit 31-2  -Descriptor Branch address	*/
	unsigned long volatile reserve0:32;		   /* bit 31-0  -reserved */
} nand_pdma_desc_long;


#define ECC1BIT 0
#define ECC4BIT 1
#define ECC8BIT 2
#define ECC12BIT 3
#define ECC16BIT 4
#define ECC24BIT 5
#define ECC40BIT 6
#define ECC24BIT_PER_1K 5
#define ECC40BIT_PER_1K 6
#define ECC44BIT_PER_1K 7
#define ECC44BIT               8
#define ECC1BIT_BIT_COUNT   32
#define ECC4BIT_BIT_COUNT   52
#define ECC8BIT_BIT_COUNT   104
#define ECC12BIT_BIT_COUNT 156
#define ECC16BIT_BIT_COUNT 208
#define ECC24BIT_PER_1K_BIT_COUNT 336
#define ECC40BIT_PER_1K_BIT_COUNT 560
#define ECC44BIT_PER_1K_BIT_COUNT 616
#define ECC44BIT_BIT_COUNT    576
#define ECC1BIT_BYTE_COUNT    4
#define ECC4BIT_BYTE_COUNT    8
#define ECC8BIT_BYTE_COUNT    16
#define ECC12BIT_BYTE_COUNT  20
#define ECC16BIT_BYTE_COUNT  26
#define ECC24BIT_PER_1K_BYTE_COUNT 42
#define ECC40BIT_PER_1K_BYTE_COUNT 70
#define ECC44BIT_PER_1K_BYTE_COUNT 77
#define ECC44BIT_BYTE_COUNT 72
#define ECC_MOD_SEL 7
#define MAX_ERR_BIT 44

/* ECC BCH interrupt mask */
#define ECC_BCH_INTERRUPT_ENABLE   0x101
#define ECC_BCH_INTERRUPT_DISABLE  0x0

/* cfg_1 */
#define SP_CMD_EN              0x400/*reid set cmd registers 0x08~0x1c*/  
#define DPAHSE_DISABLE     0x80  /* disable data phase */
#define DATA_READ_EN        0x40  /* enable data read */
#define END_WITH_CMD_EN 0x10  /* enable end with command */
#define DPAHSE_DISABLE     0x80  /* disable data phase */
#define NAND2NFC                0x40  /* direction : nand to controller */
#define NFC2NAND                0x00  /* direction : controller to controller */
#define SING_RW	                 0x20  /* enable signal read/ write command */
#define MUL_CMDS               0x10  /* support cmd+addr+cmd */
#define NFC_TRIGGER           0x01   /* start cmd&addr sequence */

/* cfg_9 */
#define NFC_CMD_RDY 0x04
#define OOB_RW           0x02

/* cfg_a */
#define NFC_BUSY	0x02  /* command and data is being transfer in flash I/O */
#define FLASH_RDY    0x01  /* flash is ready */

/* cfg_b */
#define B2R	              0x08 /* status form busy to ready */

/* cfg_12 */
#define WP_DISABLE   (1<<4) /* disable write protection */
#define OLDDATA_EN   (1<<2) /* enable old data structure */
#define WIDTH_8	        0
#define WIDTH_16      (1<<3)
#define PAGE_512       0
#define PAGE_2K         1
#define PAGE_4K         2
#define DIRECT_MAP   (1<<5)
#define USE_SW_ECC  0x40
#define USE_HW_ECC  0
//#define CHECK_ALLFF  (1<<6)

/* cfg_1d */
#define NFC_IDLE	0x01
#define DMA_SINGNAL 0
#define DMA_INC4 0x10
#define DMA_INC8 0x20

/* cfg_23 */
#define READ_RESUME 0x100
#define ECC_MODE       7

/* cfg_25 status */
#define ERR_CORRECT 0x100
#define BCH_ERR         0x1

/* cfg_26 status */
#define BANK_DR         0x800
#define BANK_NUM       0x700
#define BCH_ERR_CNT  0x3F

#define ADDR_COLUMN           1
#define ADDR_PAGE                2
#define ADDR_COLUMN_PAGE 3

#define WRITE_NAND_COMMAND(d, adr) do { *(volatile unsigned char *)(adr) = (unsigned char)(d); } while (0)
#define WRITE_NAND_ADDRESS(d, adr)   do { *(volatile unsigned char *)(adr) = (unsigned char)(d); } while (0)

#define SOURCE_CLOCK 25
#define MAX_SPEED_MHZ 65
#define MAX_READ_DELAY 9   /* 8.182 = tSKEW 3.606 + tDLY 4.176 + tSETUP 0.4 */
#define MAX_WRITE_DELAY 9 /* 8.72 = tDLY 10.24 - tSKEW 1.52*/

#define FIRST_4K_218     0
#define SECOND_4K_218 4314 /*4096 + 218 */

#define NF_CTRL_CFG_BASE_ADDR            (0xD8009000 )
#define GPIO_BASE_ADDR                          (0xD8110000)  /* 64K  */
#define PM_CTRL_BASE_ADDR                    (0xD8130000 )  /* 64K  */
#define SYSTEM_CFG_CTRL_BASE_ADDR    (0xD8120000 ) 
#define NF_CTRL_CFG_BASE_ADDR            (0xD8009000 )
#define PMPMA_ADDR       (PM_CTRL_BASE_ADDR + 0x0200)
#define PMPMB_ADDR       (PM_CTRL_BASE_ADDR + 0x0204)
#define PMPMC_ADDR       (PM_CTRL_BASE_ADDR + 0x0208)
#define PMPMD_ADDR       (PM_CTRL_BASE_ADDR + 0x020C)
#define PMPME_ADDR       (PM_CTRL_BASE_ADDR + 0x0210)
#define PMPMF_ADDR       (PM_CTRL_BASE_ADDR + 0x0214)     /* PLL Audio(I2S) Control Register */
#define PMNAND_ADDR    (PM_CTRL_BASE_ADDR + 0x0330)     /* NAND */
#define PMNANDH_ADDR  (PM_CTRL_BASE_ADDR + 0x0319)
#define PMCS_ADDR         (PM_CTRL_BASE_ADDR + 0x0000)
#define PMCEU_ADDR       (PM_CTRL_BASE_ADDR + 0x0254)


#define NAND_PAGES_PER_BLOCK   256//32

#if  NAND_BYTES_PER_PAGE > 4096
#define NAND_BANK_SIZE         1024
#else 
#define NAND_BANK_SIZE         512
#endif

#define RW_TIMING                          0x140A0F64

#ifdef CONFIG_JFFS2_NAND
int elite_read_jffs2_nand(size_t start, size_t len,size_t * retlen, u_char * buf, int nanddev);
#endif /* CONFIG_JFFS2_NAND */

/* selected NAND flash parameters */
struct elite_nand_priv {
	int cur_chip_n;
	unsigned long dmaaddr;
	void *dmabuf;
	int datalen;
};

static inline volatile void *elite_get_nfc_regs_base(void) {
	return (volatile void *)NF_CTRL_CFG_BASE_ADDR;
}

static inline volatile void *elite_get_pdma_regs_base(void) {
	return (volatile void *)(NF_CTRL_CFG_BASE_ADDR+0x100);
}

//#define  NAND_DEBUG
#ifdef NAND_DEBUG
#define NAND_PRINTF(fmt, args...)   printf("%s: " fmt, __func__ , ## args)      
#define ENTER()  printf("Enter %s, file:%s line:%d\n", __func__, __FILE__, __LINE__)    
#define LEAVE()  printf("Exit %s, file:%s line:%d\n", __func__, __FILE__, __LINE__)   
#else
#define NAND_PRINTF(fmt, args...)   
#define ENTER() 
#define LEAVE()   
#endif

#endif
