#include <config.h>
#include <common.h>
#include <malloc.h>
#include <mmc.h>
#include <part.h>
#include <ata.h>
#include <part.h>
#include <fat.h>
#include <post.h>
#include <fs.h>
#include <errno.h>
#include <asm/dma-mapping.h>
#include <asm/gpio.h>
#include <asm/arch/elite.h>
#include <asm/arch/pmc_clock.h>

#include "elite_mmc.h"

#define max_block 127
#define DMA_BUFF_SIZE (1024*32)	//;elite1k-520016c-YSW-07

#define BA_SDC_2 		0xD800A800	// SD Host Controller 2 Base Address
#define BA_SDCDMA_2		0xD800A900	// SD Host Controller 2 DMA Base Address
#define BA_SDC_1 		0xD800A400	// SD Host Controller 1 Base Address
#define BA_SDCDMA_1		0xD800A500	// SD Host Controller 1 DMA Base Address
#define BA_SDC_0 			0xD800A000	// SD Host Controller 0 Base Address
#define BA_SDCDMA_0 		0xD800A100	// SD Host Controller 0 DMA Base Address

/* support 4 mmc hosts */
struct mmc_host mmc_host[ELITE_MAX_MMC_DEV];

//;elite1k-520016c-YSW-07 - start
/*buff for dma*/
static char dma_buff[DMA_BUFF_SIZE] __attribute__((aligned(64)));
//;elite1k-520016c-YSW-07 - end

static inline struct elite_sdmmc_reg *elite_get_base_mmc(int dev_index)
{
    unsigned long offset = dev_index * 0x400;
    return (struct elite_sdmmc_reg *)(BA_SDC_0 + offset);//phy addr
}

static inline struct sd_pdma_reg *elite_get_base_dma_mmc(int dev_index)
{
    unsigned long offset = dev_index * 0x400;
    return (struct sd_pdma_reg *)(BA_SDCDMA_0 + offset);//phy addr
}


int elite_mci_set_clock(int dev_num, unsigned long freq)
{
    if (!dev_num)
        clk_set_rate("sd0", freq);
    else if (1 == dev_num)
        clk_set_rate("sd1", freq);
    else if (2 == dev_num)
        clk_set_rate("sd2", freq);
    return 0;
}

static int mmc_core_init(struct mmc *mmc)
{
    struct mmc_host *host = (struct mmc_host *)mmc->priv;
    int i;

    //clk ,gpio,power setting
	if (!host->dev_index)
    	clk_enable("sd0");
	else if (1 == host->dev_index)
    	clk_enable("sd1");
	else if (2 == host->dev_index)
		clk_enable("sd2");

    host->reg->busm = SOFT_RESET;


    /**
     * The following sequence are used to workaround bugs
     * of card power control
     */ 
	if (!host->dev_index)
    	gpio_direction_output(32, 1); //sd0: pwr off card
	else if (1 == host->dev_index)
    	gpio_direction_output(64, 1); //sd1: pwr off card
	else if (2 == host->dev_index)
		gpio_direction_output(48, 1); //sd2: pwr off card

    /* enable SD output clock */
    host->reg->busm |= 0x10;
    /* disable SD output clock */	
    host->reg->busm &= ~0x10;

	if (!host->dev_index) {/* SD0 */
    	elite_gpio_pullupdown_ctrl0_set(28, ELITE_PINCONF_PULL_DOWN);//CD
    	elite_gpio_pullupdown_ctrl0_set(32, ELITE_PINCONF_PULL_DOWN);
    	for (i = 33; i < 40; i++)
        	elite_gpio_pullupdown_ctrl0_set(i, ELITE_PINCONF_PULL_DOWN);
	} else if (1 == host->dev_index) {/* SD1 */	
    	elite_gpio_pullupdown_ctrl0_set(64, ELITE_PINCONF_PULL_DOWN);
    	for (i = 65; i < 73; i++)
        	elite_gpio_pullupdown_ctrl0_set(i, ELITE_PINCONF_PULL_DOWN);
	} else if (2 == host->dev_index) {
		/* clk pin */
		elite_gpio_pullupdown_ctrl0_set(43, ELITE_PINCONF_PULL_DOWN);//CLK
		/* cmd pin */
		elite_gpio_pullupdown_ctrl0_set(44, ELITE_PINCONF_PULL_DOWN);//CMD
		/* WP pin */
		elite_gpio_pullupdown_ctrl0_set(49, ELITE_PINCONF_PULL_DOWN);//WP
		/* SD2 pin config */
		elite_gpio_pullupdown_ctrl0_set(50, ELITE_PINCONF_PULL_DOWN);//CD
		/* Power up SD card connected to SDC2 */
		elite_gpio_pullupdown_ctrl0_set(48, ELITE_PINCONF_PULL_DOWN);
		for (i = 56; i < 64; i++)//data
			elite_gpio_pullupdown_ctrl0_set(i, ELITE_PINCONF_PULL_DOWN);
	}


	if (!host->dev_index)
    	gpio_direction_output(32, 0); //sd0: pwr on card
	else if (1 == host->dev_index)
    	gpio_direction_output(64, 0); //sd1: pwr on card
	else if (2 == host->dev_index)
		gpio_direction_output(48, 0); //sd2: pwr on card


    if (!host->dev_index) {/* SD0 */
    	elite_gpio_pullupdown_ctrl0_set(28, ELITE_PINCONF_PULL_UP);//CD
    	elite_gpio_pullupdown_ctrl0_set(32, ELITE_PINCONF_PULL_UP);
    	for (i = 33; i < 40; i++)
        	elite_gpio_pullupdown_ctrl0_set(i, ELITE_PINCONF_PULL_UP);
	} else if (1 == host->dev_index) {/* SD1 */	
    	elite_gpio_pullupdown_ctrl0_set(64, ELITE_PINCONF_PULL_UP);
		for (i = 65; i < 73; i++)
			elite_gpio_pullupdown_ctrl0_set(i, ELITE_PINCONF_PULL_UP);
	} else if (2 == host->dev_index) {
		/* clk pin */
		elite_gpio_pullupdown_ctrl0_set(43, ELITE_PINCONF_PULL_UP);//CLK
		/* cmd pin */
		elite_gpio_pullupdown_ctrl0_set(44, ELITE_PINCONF_PULL_UP);//CMD
		/* WP pin */
		elite_gpio_pullupdown_ctrl0_set(49, ELITE_PINCONF_PULL_UP);//WP
		/* SD2 pin config */
		elite_gpio_pullupdown_ctrl0_set(50, ELITE_PINCONF_PULL_UP);//CD
		/* Power up SD card connected to SDC2 */
		elite_gpio_pullupdown_ctrl0_set(48, ELITE_PINCONF_PULL_UP);
		for (i = 56; i < 64; i++)//data
			elite_gpio_pullupdown_ctrl0_set(i, ELITE_PINCONF_PULL_UP);
	}

    /* enable SD output clock */
    host->reg->busm |= 0x10;
    /*------------------------------end of seqence---------------------------*/

    /* automatic clock freezing enable */
    host->reg->str2 = DIS_FORCECLK;	

    host->reg->timeval= 0xefff;

	if (!host->dev_index)
    	elite_mci_set_clock(0, SD_Clk_44MHz);
	else if (1 == host->dev_index)
    	elite_mci_set_clock(1, SD_Clk_44MHz);
	else if (2 == host->dev_index)
		elite_mci_set_clock(2, SD_Clk_400KHz);

    return 0;
}


static void mmc_set_ios(struct mmc *mmc)
{
    struct mmc_host *host = mmc->priv;

    switch(mmc->clock) {
        case SD_Clk_400KHz:
            elite_mci_set_clock(host->dev_index, SD_Clk_400KHz); 	 //400Khz
            printf( "SD/MMC: Select 400KHz as clock rate\n");
            break;
        case SD_Clk_15MHz:
            elite_mci_set_clock(host->dev_index, SD_Clk_15MHz);		//15Mhz
            printf( "SD/MMC: Select 15MHz as clock rate\n");
            break;
        case SD_Clk_25MHz:
            elite_mci_set_clock(host->dev_index, SD_Clk_25MHz);	   //25Mhz
            printf( "SD/MMC: Select 25MHz as clock rate\n");
            break;
        case SD_Clk_44MHz:
            elite_mci_set_clock(host->dev_index, SD_Clk_44MHz);	   //25Mhz
            printf( "SD/MMC: Select 44MHz as clock rate\n");
            break;
        case SD_Clk_50MHz:
            elite_mci_set_clock(host->dev_index, SD_Clk_50MHz);		//48Mhz
            printf( "SD/MMC: Select 50MHz as clock rate\n");
            break;
        default:
            break;

    }

//;elite1k-520016c-YSW-07 - start
	if(mmc->bus_width == 1)
	{
		host->reg->busm &= ~0x2;
	}
	else if(mmc->bus_width == 4)
	{
		host->reg->busm |= 0x2;
	}
	else
		printf("MMC:bus width not support\n");
//;elite1k-520016c-YSW-07 - end

}

int elite_mci_init_desc(unsigned long *desc_addr, unsigned int req_count,
        unsigned char *buffer_addr)
{
    struct sd_pdma_desc *curdes_s = (struct sd_pdma_desc *)desc_addr;

    curdes_s->req_count = req_count;
    curdes_s->i = 1; 
    curdes_s->format = 0;
    curdes_s->end = 1;
    curdes_s->databuffer_addr = (unsigned long*)buffer_addr;
    flush_dcache_range((unsigned long)desc_addr, 
            (unsigned long)desc_addr + sizeof(struct sd_pdma_desc));

}

void elite_mci_config_pdma(struct mmc_host *host,struct sd_pdma_desc_s *desc_addr, char dir)
{
    host->dma_reg->dma_ier = DMA_IER_INT_EN;
    host->dma_reg->dma_despr = (volatile unsigned long *)desc_addr;
    if (dir == MMC_DATA_WRITE)
        host->dma_reg->dma_ccr &= DMA_CCR_IF_TO_PERIPHERAL;
    else
        host->dma_reg->dma_ccr |= DMA_CCR_PERIPHERAL_TO_IF;
    host->dma_reg->dma_ccr |= DMA_CCR_RUN;

}


static void mmc_config_dma(struct mmc *mmc,struct mmc_host *host, struct mmc_cmd *cmd,struct mmc_data *data)
{
    volatile void *  data_buf;
    unsigned char *buffer_addr_no_cache;

    host->dma_reg->dma_gcr = DMA_GCR_DMA_EN;
    host->dma_reg->dma_gcr = DMA_GCR_SOFTRESET;
    host->dma_reg->dma_gcr = DMA_GCR_DMA_EN;

    if(0==mmc->high_capacity){
        if(MMC_DATA_READ==data->flags)
            data_buf=(volatile void *)(data->dest+host->offset);
        else
            data_buf=(volatile void *)(data->src+ host->offset);

    }else{
        if(MMC_DATA_READ==data->flags)
            data_buf=(volatile void *)(data->dest+512*host->offset);
        else
            data_buf=(volatile void *)(data->src+512*host->offset);
    }

//;elite1k-520016c-YSW-07    if((cmd->cmdidx == MMC_CMD_WRITE_MULTIPLE_BLOCK)||(cmd->cmdidx == MMC_CMD_READ_SINGLE_BLOCK)||(cmd->cmdidx == MMC_CMD_READ_MULTIPLE_BLOCK)||(cmd->cmdidx == MMC_CMD_WRITE_SINGLE_BLOCK)){
//;elite1k-520016c-YSW-07        buffer_addr_no_cache = (uchar *)dma_map_single(data_buf,512*host->blkcounts,DMA_FROM_DEVICE);
//;elite1k-520016c-YSW-07        elite_mci_init_desc(&(host->desc),host->blkcounts*512,buffer_addr_no_cache);
//;elite1k-520016c-YSW-07    }
//;elite1k-520016c-YSW-07 - start
    if((cmd->cmdidx == MMC_CMD_READ_SINGLE_BLOCK) || (cmd->cmdidx == MMC_CMD_READ_MULTIPLE_BLOCK)) {

        buffer_addr_no_cache = (uchar *)dma_map_single(dma_buff, DMA_BUFF_SIZE, DMA_FROM_DEVICE);
        elite_mci_init_desc(&(host->desc), host->blkcounts*512, buffer_addr_no_cache);
    }
    else if((cmd->cmdidx == MMC_CMD_WRITE_SINGLE_BLOCK) || (cmd->cmdidx == MMC_CMD_WRITE_MULTIPLE_BLOCK)) {
        buffer_addr_no_cache = (uchar *)dma_map_single(dma_buff, DMA_BUFF_SIZE, DMA_TO_DEVICE);
        elite_mci_init_desc(&(host->desc),host->blkcounts*512,buffer_addr_no_cache);
    }
//;elite1k-520016c-YSW-07 - end
//;elite1k-520016c-YSW-07    else{
//;elite1k-520016c-YSW-07        buffer_addr_no_cache = (uchar *)dma_map_single(data_buf,512*host->blkcounts,DMA_TO_DEVICE);
//;elite1k-520016c-YSW-07        elite_mci_init_desc(&(host->desc),data->blocksize,buffer_addr_no_cache);
//;elite1k-520016c-YSW-07    }
//;elite1k-520016c-YSW-07 - start
    else{
        buffer_addr_no_cache = (uchar *)dma_map_single(dma_buff, DMA_BUFF_SIZE, DMA_FROM_DEVICE);
        elite_mci_init_desc(&(host->desc),data->blocksize,buffer_addr_no_cache);
    }
//;elite1k-520016c-YSW-07 - end

    elite_mci_config_pdma(host,&(host->desc), data->flags);
}


void change_cmd_respo_format( struct mmc_cmd *cmd,struct mmc_host *host)
{

//;elite1k-520016c-YSW-07    switch(cmd->cmdidx)
//;elite1k-520016c-YSW-07    {
//;elite1k-520016c-YSW-07        case SD_CMD_APP_SEND_SCR:
//;elite1k-520016c-YSW-07        case SD_CMD_SWITCH_FUNC:
//;elite1k-520016c-YSW-07        case MMC_CMD_WRITE_SINGLE_BLOCK:
//;elite1k-520016c-YSW-07        case MMC_CMD_WRITE_MULTIPLE_BLOCK:
//;elite1k-520016c-YSW-07        case  MMC_CMD_READ_SINGLE_BLOCK:
//;elite1k-520016c-YSW-07        case  MMC_CMD_READ_MULTIPLE_BLOCK:
//;elite1k-520016c-YSW-07            cmd->resp_type=0x01;
//;elite1k-520016c-YSW-07            break;
//;elite1k-520016c-YSW-07
//;elite1k-520016c-YSW-07        case MMC_CMD_STOP_TRANSMISSION:
//;elite1k-520016c-YSW-07            cmd->resp_type=0x09;
//;elite1k-520016c-YSW-07            break;
//;elite1k-520016c-YSW-07        case MMC_CMD_ALL_SEND_CID:
//;elite1k-520016c-YSW-07        case MMC_CMD_SEND_CSD:
//;elite1k-520016c-YSW-07            cmd->resp_type=0x02;
//;elite1k-520016c-YSW-07            break;
//;elite1k-520016c-YSW-07        case SD_CMD_APP_SEND_OP_COND:
//;elite1k-520016c-YSW-07            cmd->resp_type=0x03;
//;elite1k-520016c-YSW-07            break;
//;elite1k-520016c-YSW-07        default:
//;elite1k-520016c-YSW-07            break;
//;elite1k-520016c-YSW-07    }
//;elite1k-520016c-YSW-07    //=============================================

    //elite mmc controller need command type
    host->reg->ctlr &= 0x0F;

    switch(cmd->cmdidx)
    {
        case SD_CMD_APP_SEND_SCR:
        case MMC_CMD_READ_SINGLE_BLOCK:
//;elite1k-520016c-YSW-07        case SD_CMD_SWITCH_FUNC:
//;elite1k-520016c-YSW-07            //case MMC_CMD_SEND_EXT_CSD:  sd do not need
//;elite1k-520016c-YSW-07 - start
	case MMC_CMD_SEND_EXT_CSD:
//;elite1k-520016c-YSW-07 - end
            host->reg->ctlr |= (2<< 4);
            break;
//;elite1k-520016c-YSW-07 - start
        case SD_CMD_SWITCH_FUNC:
	    if(cmd->resp_type == MMC_RSP_R1b)//mmc switch
		host->reg->ctlr |= (0<< 4);
	    else if(cmd->resp_type == MMC_RSP_R1)//sd switch
            host->reg->ctlr |= (2<< 4);
            break;
//;elite1k-520016c-YSW-07 - end
        case MMC_CMD_WRITE_SINGLE_BLOCK:
            host->reg->ctlr |= (1<< 4);
            break;

        case MMC_CMD_READ_MULTIPLE_BLOCK:
        case 53:
            host->reg->ctlr |= (4<< 4);
            break;	

        case MMC_CMD_WRITE_MULTIPLE_BLOCK:
            host->reg->ctlr |= (3<< 4);
            break;	

        case MMC_CMD_STOP_TRANSMISSION:
            host->reg->ctlr |= (7<< 4);
        default:
            host->reg->ctlr |= (0<< 4);
            break;
    }

}

void config_sd_register(struct mmc_host *host,struct mmc_data *data)
{

    host->reg->ctlr |= 0x08 ;   /* response FIFO reset */
    host->reg->str0 |= 0xbF;
    host->reg->str1 |= 0xFF;

    if(!data){
        host->reg->blklen  = 0x0;
        host->reg->blkcnt  = 0x0;	
    }else
    {
        host->reg->blklen  = data->blocksize-1;
        host->reg->blkcnt  = (unsigned short)(host->blkcounts& 0xFFFF);
    }

}

int elite_mci_command(struct mmc_host *host, unsigned char command,unsigned int  arg, unsigned char rsptype,struct mmc_data *data)
{

//;elite1k-520016c-YSW-07    unsigned int i,retry = 0x100000,num=1000;
//;elite1k-520016c-YSW-07 - start
//;elite1k-520016c-YSW-08    unsigned int i, retry = 0x100000, num=1000, wait = 100000;
    unsigned int i, retry = 0x100000, num=1000, wait = 300000;					//;elite1k-520016c-YSW-08
//;elite1k-520016c-YSW-07 - end
    unsigned int mask;

    debug("cmd->arg: %08x\n", arg);

    host->reg->cmdarg0 = (unsigned char)arg;
    host->reg->cmdarg1 = (unsigned char)(arg >> 8);/* move 8 bits */
    host->reg->cmdarg2 = (unsigned char)(arg>> 16);/* move 16 bits */
    host->reg->cmdarg3 = (unsigned char)(arg >> 24);/* move 24 bits */

    //host->reg->cmdarg=cmd->cmdarg;//cmd arg


    //cmd idex and type
    host->reg->cmdi = (unsigned char)command;
//;elite1k-520016c-YSW-07    host->reg->respt = (unsigned char)rsptype;
//;elite1k-520016c-YSW-07 - start
    switch(command)
    {
	case MMC_CMD_GO_IDLE_STATE:
	     host->reg->respt=0x00;
	     break;
	case SD_CMD_APP_SEND_SCR:
	case MMC_CMD_WRITE_SINGLE_BLOCK:
	case MMC_CMD_WRITE_MULTIPLE_BLOCK:
	case MMC_CMD_READ_SINGLE_BLOCK:
	case MMC_CMD_READ_MULTIPLE_BLOCK:
	case MMC_CMD_SEND_STATUS:
	case MMC_CMD_SET_RELATIVE_ADDR:
	case MMC_CMD_SELECT_CARD:
	case MMC_CMD_SEND_EXT_CSD:
	case MMC_CMD_APP_CMD:
	case MMC_CMD_SET_BLOCKLEN:
            host->reg->respt=0x01;
            break;
        case MMC_CMD_SWITCH:
        case MMC_CMD_STOP_TRANSMISSION:
	     if(rsptype == MMC_RSP_R1b)
		host->reg->respt=0x09;
	     else
		host->reg->respt=0x01;
	    break;
        case MMC_CMD_ALL_SEND_CID:
        case MMC_CMD_SEND_CSD:
            host->reg->respt=0x02;
            break;
        case SD_CMD_APP_SEND_OP_COND:
	case MMC_CMD_SEND_OP_COND:
            host->reg->respt=0x03;
            break;
        default:
	     host->reg->respt=0x0;
            break;
    }
//;elite1k-520016c-YSW-07 - end

    /* clear all status registers */
//;elite1k-520016c-YSW-07    host->reg->str0 &= 0xbf;
//;elite1k-520016c-YSW-07    host->reg->str1 &= 0xff;
//;elite1k-520016c-YSW-07 - start
    host->reg->str0 |= 0xbf;
    host->reg->str1 |= 0xff;
//;elite1k-520016c-YSW-07 - end

    host->reg->busm |= 0x10;

    /* send out the command */
    host->reg->ctlr |= CMD_START;

    /* wait for Ctrl[busy] clear */
    while ((host->reg->ctlr & 0x1) != 0);

//;elite1k-520016c1-YSW-04     /* wait for command completion, STS1[cmd-rsp-done] */
//;elite1k-520016c1-YSW-04 //;elite1k-520016c-YSW-07    for (i = 0; i < retry; i++) {
//;elite1k-520016c1-YSW-04 //;elite1k-520016c-YSW-07 - start
//;elite1k-520016c1-YSW-04     for (i = 0; i < wait; i++) {
//;elite1k-520016c1-YSW-04 //;elite1k-520016c-YSW-07 - end
//;elite1k-520016c1-YSW-04         mask = host->reg->str1;
//;elite1k-520016c1-YSW-04         /* wait for command completion, STS1[cmd-rsp-done] */
//;elite1k-520016c1-YSW-04         if((mask & 0x2) == 0x2) 
//;elite1k-520016c1-YSW-04             break ;
//;elite1k-520016c1-YSW-04     }
//;elite1k-520016c1-YSW-04 
//;elite1k-520016c1-YSW-04 //;elite1k-520016c-YSW-07    if (i == retry) {
//;elite1k-520016c1-YSW-04 //;elite1k-520016c-YSW-07 - start
//;elite1k-520016c1-YSW-04     if (i == wait) {
//;elite1k-520016c1-YSW-04 //;elite1k-520016c-YSW-07 - end
//;elite1k-520016c1-YSW-04         printf("mask data:%x\n",mask);
//;elite1k-520016c1-YSW-04         printf("%s: waiting for status update\n", __func__);
//;elite1k-520016c1-YSW-04         return TIMEOUT;
//;elite1k-520016c1-YSW-04     }
//;elite1k-520016c1-YSW-04 
//;elite1k-520016c1-YSW-04     if ((mask & 0x4) == 0x4) {
//;elite1k-520016c1-YSW-04         /* Timeout Error */
//;elite1k-520016c1-YSW-04         printf("timeout: %08x cmd %d\n", mask,command);
//;elite1k-520016c1-YSW-04         return TIMEOUT;
//;elite1k-520016c1-YSW-04     } else if ((mask & 0x8) == 0x8) {
//;elite1k-520016c1-YSW-04         /* Error Interrupt */
//;elite1k-520016c1-YSW-04         printf("error: %08x cmd %d\n", mask,command);
//;elite1k-520016c1-YSW-04         return -1;
//;elite1k-520016c1-YSW-04     } else if((mask & 0x80)==0x80){
//;elite1k-520016c1-YSW-04         printf("Write CRC Error:%08x cmd %d\n",mask,command);
//;elite1k-520016c1-YSW-04         return -1;
//;elite1k-520016c1-YSW-04     }else if((mask & 0x40)==0x40){
//;elite1k-520016c1-YSW-04         printf("Read CRC Error:%08x cmd %d\n",mask, command);
//;elite1k-520016c1-YSW-04         return -1;
//;elite1k-520016c1-YSW-04     }else if((mask & 0x20)==0x20){
//;elite1k-520016c1-YSW-04         printf("Response CRC Error:%08x cmd %d\n",mask, command);
//;elite1k-520016c1-YSW-04         //return -1;
//;elite1k-520016c1-YSW-04     }
//;elite1k-520016c1-YSW-04 
//;elite1k-520016c1-YSW-04     while((host->reg->str2 & 0x20))
//;elite1k-520016c1-YSW-04         if(0 == num--)
//;elite1k-520016c1-YSW-04             break;
//;elite1k-520016c1-YSW-04     if(0 == num)
//;elite1k-520016c1-YSW-04         printf("SD data-response always busy!\n");
//;elite1k-520016c1-YSW-04 
//;elite1k-520016c1-YSW-04     /* SD status : command-response time-out */
//;elite1k-520016c1-YSW-04     if((host->reg->str1 & 0x4) == 0x4)
//;elite1k-520016c1-YSW-04         return  -1 ;
//;elite1k-520016c1-YSW-04 
//;elite1k-520016c1-YSW-04     //=====================================
//;elite1k-520016c1-YSW-04     if(NULL!=data)
//;elite1k-520016c1-YSW-04     {
//;elite1k-520016c1-YSW-04         for (i = 0; i < retry; i++) {
//;elite1k-520016c1-YSW-04             mask = host->reg->str0;
//;elite1k-520016c1-YSW-04             /* wait for command completion, STS1[cmd-rsp-done] */
//;elite1k-520016c1-YSW-04             if((mask & 0x30) == 0x30) 
//;elite1k-520016c1-YSW-04             {
//;elite1k-520016c1-YSW-04 //;elite1k-520016c-YSW-07                host->reg->str0 &=0xbf;
//;elite1k-520016c1-YSW-04                 break ;
//;elite1k-520016c1-YSW-04             }
//;elite1k-520016c1-YSW-04         }
//;elite1k-520016c1-YSW-04         if (i == retry) {
//;elite1k-520016c1-YSW-04             printf("mask data:%x\n",mask);
//;elite1k-520016c1-YSW-04             printf("%s: wait  interrupt timeout\n", __func__);
//;elite1k-520016c1-YSW-04             return TIMEOUT;
//;elite1k-520016c1-YSW-04         }
//;elite1k-520016c1-YSW-04     }
//;elite1k-520016c1-YSW-04 
//;elite1k-520016c1-YSW-04     //=====================================
//;elite1k-520016c1-YSW-04     return 0 ;

//;elite1k-520016c1-YSW-04 - start
    
	/* wait for command completion, STS1[cmd-rsp-done] */	
	do {
		mask = host->reg->str1;

		if ((mask & 0x4) == 0x4) {
		    /* Timeout Error */
		    printf("timeout: %08x cmd %d\n", mask,command);
		    return TIMEOUT;
		} else if ((mask & 0x8) == 0x8) {
		    /* Error Interrupt */
		    printf("error: %08x cmd %d\n", mask,command);
		    return -1;
		} else if((mask & 0x80) == 0x80){
		    printf("Write CRC Error:%08x cmd %d\n",mask,command);
		    return -1;
		}else if((mask & 0x40) == 0x40){
		    printf("Read CRC Error:%08x cmd %d\n",mask, command);
		    return -1;
		}else if((mask & 0x20) == 0x20){
		    printf("Response CRC Error:%08x cmd %d\n",mask, command);
		    //return -1;
		}
	}while((mask & 0x2) != 0x2);

	/* wait SD data-response not busy (str2[5] == 0) */	
	while((host->reg->str2 & 0x20))

	/* Check SD status : command-response time-out */
	if((host->reg->str1 & 0x4) == 0x4)
		return  -1 ;

	/* For Command with Data, check Block Data & Multi-Block Data Transfer done (str0[5:4] = 11b) */ 
	if(NULL != data)
	{
	    while ((host->reg->str0 & 0x30) != 0x30);
	}	

	return 0 ;

//;elite1k-520016c1-YSW-04 - end

}

int elite_mci_check_dma_status(struct mmc_host *host,struct mmc_cmd *cmd,struct mmc_data *data)
{
//;elite1k-520016c-YSW-07    unsigned int retry = 0x100000,count = 0x100000;
//;elite1k-520016c-YSW-07 - start
	unsigned int retry = 0x100000,count = 0x10000000;
//;elite1k-520016c-YSW-07 - end

    unsigned long status = DMA_CCR_EVT_NO_STATUS;
    //========================================================
    //get respose

    if (cmd->resp_type & MMC_RSP_136) {
        cmd->response[0] = (host->reg->resr[1]<<24)|(host->reg->resr[2]<<16)|(host->reg->resr[3]<<8)|(host->reg->resr[4]);
        cmd->response[1] = (host->reg->resr[5]<<24)|(host->reg->resr[6]<<16)|(host->reg->resr[7]<<8)|(host->reg->resr[8]);
        cmd->response[2] = (host->reg->resr[9]<<24)|(host->reg->resr[10]<<16)|(host->reg->resr[11]<<8)|(host->reg->resr[12]);
        cmd->response[3] = (host->reg->resr[13]<<24)|(host->reg->resr[14]<<16)|(host->reg->resr[15]<<8);

        debug("cmd->resp[0]: %08x\n", cmd->response[0]);
        debug("cmd->resp[1]: %08x\n", cmd->response[1]);
        debug("cmd->resp[2]: %08x\n", cmd->response[2]);
        debug("cmd->resp[3]: %08x\n", cmd->response[3]);
    }else {
        cmd->response[0] = (host->reg->resr[1]<<24)|(host->reg->resr[2]<<16)|(host->reg->resr[3]<<8)|(host->reg->resr[4]);
        debug("cmd->resp[0]: %08x\n", cmd->response[0]);
    }

    //=======================================================
    //if trandfer data, judge end and deal with error
    if (data) {
        do {
            count--;
            if (host->dma_reg->dma_isr & DMA_IER_INT_STS) {
                status = host->dma_reg->dma_ccr & DMA_CCR_EVTCODE;
                host->dma_reg->dma_isr |= DMA_IER_INT_STS; 
                break ;
            }
            if (count == 0) {
                printf("Time Out wating for PDMA interrupt\n");
                break;
            }
        } while (1);


        if (status == DMA_CCR_EVT_FF_UNDERRUN)
            printf("PDMA Buffer under run!\n");
        if (status == DMA_CCR_EVT_FF_OVERRUN)
            printf("PDMA Buffer over run!\n");
        if (status == DMA_CCR_EVT_DESP_READ)
            printf("PDMA read Descriptor error!\n");
        if (status == DMA_CCR_EVT_DATA_RW)
            printf("PDMA read/write memory error!\n");
        if (status == DMA_CCR_EVT_EARLY_END)
            printf("PDMA read early end!\n");
        if (count == 0) {
            printf("PDMA TimeOut!\n");
            return -1;
        }

//;elite1k-520016c-YSW-07        count = 0x100000;
//;elite1k-520016c-YSW-07 - start
	count = 0x10000000;
//;elite1k-520016c-YSW-07 - end

        if ((host->dma_reg->dma_ccr & DMA_CCR_PERIPHERAL_TO_IF) == DMA_CCR_IF_TO_PERIPHERAL) {
            if (host->reg->cmdi == WRITE_MULTIPLE_BLOCK) {
                while (!(host->reg->str0 & 0x10)) { 
                    count--;
                    if (count == 0) {
                        printf("Timeout waiting for Multiple Block Transfer Done Interrupt!\n");
                        return -1;
                    }
                }
            }
            while (!(host->reg->str0 & 0x30)) { 
                count--;
                if (count == 0) {
                    printf("Time out waiting for Block Data Transfer Done Interrupt & "
                            "check Multiple Block Transfer Done Interrupt:%x!\n",host->reg->str0);
                    return -1;
                }
            }
        }
        host->dma_reg->dma_despr	  = 0;
        host->dma_reg->dma_gcr = 0;
    }

    udelay(1000);
    return 0;
}

static int mmc_send_cmd_max(struct mmc *mmc, struct mmc_cmd *cmd,
        struct mmc_data *data)
{
    struct mmc_host *host = (struct mmc_host *)mmc->priv;

    unsigned int timeout;
    unsigned int mask;
    int ret;					//;elite1k-520016c-YSW-07

#if 0
    printf("cmd:%d:%d\n",cmd->cmdidx,cmd->cmdarg);
    if(NULL!=data)
        printf("blocks:%d:blocksize:%d:offset:%d:blkcounts:%d\n",data->blocks,data->blocksize,host->offset,host->blkcounts);
#endif
    /* Wait max 10 ms */
    timeout = 10;

    //make sure bus is free for new cmd
    mask = (1 << 0);

    if ((data != NULL) || (cmd->resp_type & MMC_RSP_BUSY))
        mask |= (1 << 1);

    /*
     * We shouldn't wait for data inihibit for stop commands, even
     * though they might use busy signaling
     */
    if (data)
        mask &= ~(1 << 1);

    /* wait for Ctrl[busy] clear */
    while (( host->reg->ctlr & mask) != 0) {
        if (timeout == 0) {
            printf("%s: timeout error\n", __func__);
            return -1;
        }

        timeout--;
        udelay(1000);
    }
    //=======================================================
    if ((cmd->resp_type & MMC_RSP_136) && (cmd->resp_type & MMC_RSP_BUSY))
        return -1;

    config_sd_register(host,data);

//;elite1k-520016c-YSW-07    if (data)
//;elite1k-520016c-YSW-07        mmc_config_dma(mmc,host,cmd,data);
//;elite1k-520016c-YSW-07 - start
    if (data)
    {
	if((cmd->cmdidx == MMC_CMD_READ_SINGLE_BLOCK) || (cmd->cmdidx == MMC_CMD_READ_MULTIPLE_BLOCK))
		;
	else if((cmd->cmdidx == MMC_CMD_WRITE_SINGLE_BLOCK) || (cmd->cmdidx == MMC_CMD_WRITE_MULTIPLE_BLOCK))
        	memcpy(dma_buff, data->src, host->blkcounts*512);
	else
	{
		if(data->flags == MMC_DATA_WRITE)
			memcpy(dma_buff, data->src, data->blocksize);
	}	
        mmc_config_dma(mmc,host,cmd,data);	
    }
//;elite1k-520016c-YSW-07 - end

    change_cmd_respo_format( cmd,host);


//;elite1k-520016c-YSW-07    if(0!=elite_mci_command(host,cmd->cmdidx,cmd->cmdarg+host->offset,cmd->resp_type,data))
//;elite1k-520016c-YSW-07        return -1;
//;elite1k-520016c-YSW-07 - start
    ret = elite_mci_command(host,cmd->cmdidx,cmd->cmdarg+host->offset,cmd->resp_type,data);
    if(ret != 0)
        return ret;
//;elite1k-520016c-YSW-07 - end

    if (0!=elite_mci_check_dma_status(host,cmd,data)) {
        host->reg->timeval = 0xefff;
        return -1 ;
    }

//;elite1k-520016c-YSW-07 - start
    if(data != NULL)
    {
	if((cmd->cmdidx == MMC_CMD_READ_SINGLE_BLOCK) || (cmd->cmdidx == MMC_CMD_READ_MULTIPLE_BLOCK))
		memcpy(data->dest, dma_buff, host->blkcounts*512);
	else if((cmd->cmdidx == MMC_CMD_WRITE_SINGLE_BLOCK) || (cmd->cmdidx == MMC_CMD_WRITE_MULTIPLE_BLOCK))
		;
	else 
	{
		if(data->flags == MMC_DATA_READ)  
			memcpy(data->dest, dma_buff, data->blocksize);
	}
   }
//;elite1k-520016c-YSW-07 - end

    return 0;
}

static int mmc_send_cmd(struct mmc *mmc, struct mmc_cmd *cmd,
        struct mmc_data *data)
{
    int flag=-1,ret;
    unsigned int blknum;
    struct mmc_cmd cmd_inner;
    struct mmc_host *host = (struct mmc_host *)mmc->priv;

    if(NULL==data)
        blknum=0;	
    else
        blknum=data->blocks;

    host->offset=0;

    if(blknum <= max_block)
    {
        host->blkcounts=blknum;

        ret=mmc_send_cmd_max(mmc,cmd,data);

//;elite1k-520016c-YSW-07        if(4==mmc->bus_width)
//;elite1k-520016c-YSW-07            host->reg->busm |= FOURBIT_MODE;

        return ret;

    }

    while(blknum > max_block)
    {
        host->blkcounts=max_block;
        flag=mmc_send_cmd_max(mmc,cmd,data);
        if(0!=flag)
            return flag;

        blknum-=max_block;

        if(0 == mmc->high_capacity)
            host->offset+=max_block*512;
        else
            host->offset+=max_block;

        cmd_inner.cmdidx = MMC_CMD_STOP_TRANSMISSION;
        cmd_inner.cmdarg = 0;
        cmd_inner.resp_type = MMC_RSP_R1b;
        if (mmc_send_cmd_max(mmc, &cmd_inner, NULL)) {
            printf("mmc fail to send stop cmd\n");
            return -1;
        }

    }

    if(blknum!=0)
    {
        host->blkcounts=blknum;
        flag=mmc_send_cmd_max(mmc,cmd,data);
        if(0!=flag)
            return flag;
    }

    return 0;
}

int elite_mmc_init(int dev_index, int bus_width)
{
    struct mmc *mmc;

    mmc= &(mmc_host->mmc_dev[dev_index]);
    sprintf(mmc->name, "S3Graphics Elite SD/MMC");
    mmc->priv = &mmc_host[dev_index];
    mmc->send_cmd = mmc_send_cmd;
    mmc->set_ios = mmc_set_ios;
    mmc->init = mmc_core_init;

    mmc->voltages = MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_165_195;
    if (bus_width == 8)
        mmc->host_caps = MMC_MODE_8BIT;
    else
        mmc->host_caps = MMC_MODE_4BIT;
    mmc->host_caps |= MMC_MODE_HS_52MHz | MMC_MODE_HS | MMC_MODE_HC;

    mmc->f_min = SD_Clk_400KHz;
    mmc->f_max = SD_Clk_50MHz;

    mmc_host[dev_index].dev_index = dev_index;
    mmc_host[dev_index].clock = 0;
    mmc_host[dev_index].reg = elite_get_base_mmc(dev_index);
    mmc_host[dev_index].dma_reg=elite_get_base_dma_mmc(dev_index);
    mmc->b_max = 0;

    mmc_register(mmc);

    return 0;
}


void mmc_test(struct mmc *mmc)
{
    printf("###%s\n",mmc->name);
    printf("###%d\n",mmc->high_capacity);
    printf("###%d\n",mmc->read_bl_len);
    printf("###%x\n",mmc->host_caps);

}

