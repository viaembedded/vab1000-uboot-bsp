#ifndef __ELITE_MMC_H__
#define __ELITE_MMC_H__


#define ELITE_MAX_MMC_DEV	3

//bit definition for SD controller register

// 0x0 Control register
#define		FIFO_RESET		0x08
#define		CMD_START		0x01
#define		CMD_READ		0x00
#define		CMD_WRITE		0x04
#define		CMD_SWRITE		0x10
#define		CMD_SREAD		0x20
#define		CMD_MWRITE		0x30
#define		CMD_MREAD		0x40

//0x08 BusMode register
#define		SOFT_RESET		0x80
#define		SD_POWER	  	0x40
#define		SPI_CS		  	0x20
#define		SD_OFF			0x10
#define		FOURBIT_MODE	0x02
#define   	ONEBIT_MODE   	0xFD
#define 	SPI_MODE		0x01
#define   	SD_MODE       	0x00
#define   	EIGHTBIT_MODE	0x04

//0x0C BlkLen
#define		INT_ENABLE		0x80
#define		DATA3_CD	  	0x40
#define		GPI_CD		  	0x20
#define   	CD_POL_HIGH   	0x10
#define		CRCERR_ABORT	0x08 //abort multiple-blk-transfer when CRC-Err


//0x24 IntMask0
#define 		DI_INT_EN             	0x80
#define 		CD_INT_EN				0x40
#define 		BLK_TRAN_DONE_INT_EN  	0x20
#define 		MBLK_TRAN_DONE_INT_EN 	0x10

//0x25 IntMask0
#define 		WCRC_ERR_INT_EN            	0x80
#define 		RCRC_ERR_INT_EN            	0x40
#define 		RESCRC_ERR_INT_EN          	0x20
#define 		DATA_TOUT_INT_EN           	0x10
#define 		MBLK_AUTO_STOP_INT_EN		0x08
#define 		CMD_RES_TOUT_INT_EN        	0x04
#define 		CMD_RES_TRAN_DONE_INT_EN	0x02

//0x28 Status0 register
#define		DEVICE_INS		0x80
#define		CARD_DETECT		0x40
#define		BLK_DONE		0x20
#define		MBLK_DONE		0x10
#define		CD_GPI			0x08
#define		CD_DATA3		0x04
#define		Write_Protect	0x02


//0x29 Status1 register
#define		WCRC_ERR		0x80
#define		RCRC_ERR		0x40
#define		RSP_CRC_ERR		0x20
#define		DATA_TIMEOUT	0x10
#define		AUTOSTOP_DONE	0x08
#define		RSP_TIMEOUT		0x04
#define		CMDRSP_DONE		0x02
#define		SDIO_INT		0x01

//0x2A Status2 register
#define		DIS_FORCECLK	0x80
#define		DATARSP_BUSY	0x20
#define   	CMD_RES_BUSY  	0x10


#define SD_Clk_15MHz			15000000UL
#define SD_Clk_20MHz			20000000UL
#define SD_Clk_25MHz			25000000UL
#define SD_Clk_33MHz			33000000UL
#define SD_Clk_40MHz			40000000UL
#define SD_Clk_44MHz			44000000UL
#define SD_Clk_50MHz			50000000UL
#define SD_Clk_400KHz			400000UL


#define CFG_MMC_BASE		0xF0000000


#define	FOURBIT_MODE	0x02


/*
 *	PDMA Descriptor short
 */
struct sd_pdma_desc {
	unsigned long volatile req_count:16;		/* bit 0 -15 -Request count             */
	unsigned long volatile i:1;			/* bit 16    -interrupt                 */
	unsigned long volatile reserve:13;		/* bit 17-29 -reserved                  */
	unsigned long volatile format:1;		/* bit 30    -The descriptor format     */
	unsigned long volatile end:1;			/* bit 31    -End flag of descriptor list*/
	unsigned long volatile *databuffer_addr;	/* bit 31-0  -Data Buffer address       */
} ;



struct  sd_pdma_reg{
	volatile unsigned long dma_gcr;		/*	Rx00	*/
	volatile unsigned long dma_ier;		/*	Rx04	*/
	volatile unsigned long dma_isr;		/*	Rx08	*/
	volatile unsigned long *dma_despr;	/*	Rx0C	*/
	volatile unsigned long dma_rbp;		/*	Rx10	*/
	volatile unsigned long dma_dar;		/*	Rx14	*/
	volatile unsigned long dma_bar;		/*	Rx18	*/
	volatile unsigned long dma_cpr;		/*	Rx1C	*/
	volatile unsigned long dma_ccr;		/*	RX20	*/
	volatile unsigned long resv[5];    	/*	RX2C-3C	*/
} ;
/*
 *	SD PDMA - DMA_GCR : DMA Global Control Register
 */
#define DMA_GCR_DMA_EN			0x00000001      /* [0] -- DMA controller enable */
#define DMA_GCR_SOFTRESET		0x00000100      /* [8] -- Software rest */

/*
 *	SD PDMA - DMA_IER : DMA Interrupt Enable Register
 */
#define DMA_IER_INT_EN			0x00000001      /* [0] -- DMA interrupt enable */
/*
 *	SD PDMA - DMA_ISR : DMA Interrupt Status Register
 */
#define DMA_IER_INT_STS			0x00000001      /* [0] -- DMA interrupt status */
/*
 *	SD PDMA - DMA_DESPR : DMA Descriptor base address Pointer Register
 */

/*
 *	SD PDMA - DMA_RBR : DMA Residual Bytes Register
 */
#define DMA_RBR_End				0x80000000      /* [0] -- DMA interrupt status */
#define DMA_RBR_Format			0x40000000      /* [0] -- DMA interrupt status */
/*
 *	SD PDMA - DMA_DAR : DMA Data Address Register
 */

/*
 *	SD PDMA - DMA_BAR : DMA Rbanch Address Register
 */

/*
 *	SD PDMA - DMA_CPR : DMA Command Pointer Register
 */

/*
 *	SD PDMA - DMA_CCR : DMAContext Control Register for Channel 0
 */
#define DMA_READ					0x00
#define DMA_WRITE					0x01
#define DMA_CCR_RUN					0x00000080
#define DMA_CCR_IF_TO_PERIPHERAL	0x00000000
#define DMA_CCR_PERIPHERAL_TO_IF	0x00400000
#define DMA_CCR_EVTCODE			0x0000000f
#define DMA_CCR_EVT_NO_STATUS		0x00000000
#define DMA_CCR_EVT_FF_UNDERRUN		0x00000001
#define DMA_CCR_EVT_FF_OVERRUN		0x00000002
#define DMA_CCR_EVT_DESP_READ		0x00000003
#define DMA_CCR_EVT_DATA_RW			0x00000004
#define DMA_CCR_EVT_EARLY_END		0x00000005
#define DMA_CCR_EVT_SUCCESS			0x0000000f

#define		WRITE_SINGLE_BLOCK      	24
#define		WRITE_MULTIPLE_BLOCK    	25

//;elite1k-520016c1-YSW-04 /*SDMMC control registers.*/
//;elite1k-520016c1-YSW-04 struct elite_sdmmc_reg{
//;elite1k-520016c1-YSW-04     volatile unsigned char      ctlr;           /*Control Register*/
//;elite1k-520016c1-YSW-04     volatile unsigned char      cmdi;           /*Command Index Register*/
//;elite1k-520016c1-YSW-04     volatile unsigned char      respt;          /*Response Type Register*/
//;elite1k-520016c1-YSW-04     volatile unsigned char      Reserved1;      /*Reserved1*/
//;elite1k-520016c1-YSW-04     //volatile unsigned int		cmdarg;      /*Command Argument Register*/
//;elite1k-520016c1-YSW-04     volatile unsigned char      cmdarg0;        /*Command Argument Register 0x03*/
//;elite1k-520016c1-YSW-04     volatile unsigned char      cmdarg1;        /*Command Argument Register 0x04*/
//;elite1k-520016c1-YSW-04     volatile unsigned char      cmdarg2;        /*Command Argument Register 0x05*/
//;elite1k-520016c1-YSW-04     volatile unsigned char      cmdarg3;        /*Command Argument Register 0x06*/
//;elite1k-520016c1-YSW-04     volatile unsigned char      busm;           /*Bus Mode Register*/
//;elite1k-520016c1-YSW-04     volatile unsigned char      Reserved2[3];   /*Reserved2*/
//;elite1k-520016c1-YSW-04     volatile unsigned short     blklen;         /*Block Length Register*/
//;elite1k-520016c1-YSW-04     volatile unsigned short     blkcnt;         /*Block Count Register*/
//;elite1k-520016c1-YSW-04     volatile unsigned char      resr[16];      /*Response Register*/
//;elite1k-520016c1-YSW-04     volatile unsigned short     cbc;            /*Current Block Count*/
//;elite1k-520016c1-YSW-04     volatile unsigned short     Reserved3;      /*Reserved3*/
//;elite1k-520016c1-YSW-04     volatile unsigned char      imr0;           /*Interrupt Mask Register 0*/
//;elite1k-520016c1-YSW-04     volatile unsigned char      imr1;           /*Interrupt Mask Register 1*/
//;elite1k-520016c1-YSW-04     volatile unsigned char      Reserved4[2];   /*Reserved3*/
//;elite1k-520016c1-YSW-04     volatile unsigned char      str0;           /*SD Status Register 0*/
//;elite1k-520016c1-YSW-04     volatile unsigned char      str1;           /*SD Status Register 1*/
//;elite1k-520016c1-YSW-04     volatile unsigned char      str2;           /*SD Status Register 2*/
//;elite1k-520016c1-YSW-04     volatile unsigned char      str3;           /*SD Status Register 3 (SPI Data Error)*/
//;elite1k-520016c1-YSW-04     volatile unsigned char      rtor;           /*Response Time Out Register*/
//;elite1k-520016c1-YSW-04     volatile unsigned char      Reserved5[3];   /*Reserved4*/
//;elite1k-520016c1-YSW-04     volatile unsigned long      Reserved6;      /*Reserved5*/
//;elite1k-520016c1-YSW-04     volatile unsigned char      extctl;         /*Extend Control Register*/
//;elite1k-520016c1-YSW-04     volatile unsigned char      Reserved7[3];   /*Reserved6*/
//;elite1k-520016c1-YSW-04     volatile unsigned short     sblklen;       /*Shadowed Block Length Register*/
//;elite1k-520016c1-YSW-04     volatile unsigned short     Reserved8;     /*Reserved7*/
//;elite1k-520016c1-YSW-04     volatile unsigned short     timeval;       /*Timer Value Register*/
//;elite1k-520016c1-YSW-04 };

//;elite1k-520016c1-YSW-04 - start
/*SDMMC control registers.*/
struct elite_sdmmc_reg{
    volatile unsigned char      ctlr;           /*Control Register*/	/* 0x00 */
    volatile unsigned char      cmdi;           /*Command Index Register*/	/* 0x01 */
    volatile unsigned char      respt;          /*Response Type Register*/		/* 0x02 */
    volatile unsigned char      Reserved1;      /*Reserved1*/		/* 0x03 */
    //volatile unsigned int		cmdarg;      /*Command Argument Register*/
    volatile unsigned char      cmdarg0;        /*Command Argument Register 0x04*/
    volatile unsigned char      cmdarg1;        /*Command Argument Register 0x05*/
    volatile unsigned char      cmdarg2;        /*Command Argument Register 0x06*/
    volatile unsigned char      cmdarg3;        /*Command Argument Register 0x07*/
    volatile unsigned char      busm;           /*Bus Mode Register*/	/* 0x08 */
    volatile unsigned char      Reserved2[3];   /*Reserved2*/	/* 0x09: EBMR, 0x0A~0x0B HC2R */
    volatile unsigned short     blklen;         /*Block Length Register*/	/* 0x0C~0x0D */
    volatile unsigned short     blkcnt;         /*Block Count Register*/	/* 0x0E ~ 0x0F */
    volatile unsigned char      resr[16];      /*Response Register*/		/* 0x10 ~ 0x1F */
    volatile unsigned short     cbc;            /*Current Block Count*/	/* 0x20 ~0x21 */
    volatile unsigned short     Reserved3;      /*Reserved3*/		/* 0x22 ~ 0x23 : Bus Level and FSM state register */
    volatile unsigned char      imr0;           /*Interrupt Mask Register 0*/	/* 0x24 */
    volatile unsigned char      imr1;           /*Interrupt Mask Register 1*/	/* 0x25 */
    volatile unsigned char      Reserved4[2];   /*Reserved3*/		/* 0x26~27 */
    volatile unsigned char      str0;           /*SD Status Register 0*/	/* 0x28 */
    volatile unsigned char      str1;           /*SD Status Register 1*/	/* 0x29 */
    volatile unsigned char      str2;           /*SD Status Register 2*/	/* 0x2A */
    volatile unsigned char      str3;           /*SD Status Register 3 (SPI Data Error)*/	/* 0x2B */
    volatile unsigned char      rtor;           /*Response Time Out Register*/	/* 0x2C */
    volatile unsigned char      Reserved5[3];   /*Reserved4*/	/* 0x2D ~0x2F */
    volatile unsigned long      Reserved6;      /*Reserved5*/		/* 0x30~0x33 */
    volatile unsigned char      extctl;         /*Extend Control Register*/		/* 0x34 */
    volatile unsigned char      Reserved7[3];   /*Reserved6*/		/* 0x35~ 0x37 */
    volatile unsigned short     sblklen;       /*Shadowed Block Length Register*/		/* 0x38 ~ 0x39 */
    volatile unsigned short     Reserved8;     /*Reserved7*/		/* 0x3A~0x3B */
    volatile unsigned short     timeval;       /*Timer Value Register*/	/* 0x3C */
};
//;elite1k-520016c1-YSW-04 - end

struct mmc_host {
	struct elite_sdmmc_reg *reg;
	struct  sd_pdma_reg *dma_reg;
	unsigned int version;	/* SDHCI spec. version */
	unsigned int clock;	/* Current clock (MHz) */
	int dev_index;
	unsigned int blkcounts;
	unsigned int offset;
	struct sd_pdma_desc desc;
	struct mmc mmc_dev[ELITE_MAX_MMC_DEV];
};

int elite_mmc_init(int dev_index, int bus_width);

#endif //__ELITE_MMC_H__

