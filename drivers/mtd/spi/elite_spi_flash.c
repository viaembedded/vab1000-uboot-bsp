#include <common.h>
#include <asm/io.h>
#include <malloc.h>
#include <spi_flash.h>
#include <asm/arch/elite.h>

#include "elite_spi_flash.h"

char flash_name[100];
const char * chip_name = "SST 25VF032B";

volatile void *reg_base;

#define SPI_FLASH_TOTAL_SIZE   ((CONFIG_MAX_SPI_FLASH_BANKS)*(SPI_FLASH_SIZE))

ulong flash_id[CONFIG_MAX_SPI_FLASH_BANKS];

unsigned int phy_flash_addr_0;
unsigned int phy_flash_addr_1;
unsigned int phy_flash_base;

static int flash_error_info(unsigned long code)
{
    if (code & SF_BIT_WR_PROT_ERR) {
        printf("ERROR : SPI flash write protect!\n");
        writeb(0x3f, reg_base + SPI_ERROR_STATUS);
        return 128;
    }

    if (code & SF_BIT_MEM_REGION_ERR) {
        printf("ERROR : SPI flash memory region rrror!\n");
        writeb(0x3f, reg_base + SPI_ERROR_STATUS);
        return 128;
    }

    if (code & SF_BIT_PWR_DWN_ACC_ERR) {
        printf("ERROR : SPI flash power down access error!\n");
        writeb(0x3f, reg_base + SPI_ERROR_STATUS);
        return 128;
    }


    if (code & SF_BIT_PCMD_OP_ERR) {
        printf("ERROR : SPI flash program command OP error!\n");
        writeb(0x3f, reg_base + SPI_ERROR_STATUS);
        return 128;
    }


    if (code & SF_BIT_PCMD_ACC_ERR) {
        printf("ERROR : SPI flash program command OP access error!\n");
        writeb(0x3f, reg_base + SPI_ERROR_STATUS);
        return 128;
    }


    if (code & SF_BIT_MASLOCK_ERR) {
        printf("ERROR : SPI flash master lock error!\n");
        writeb(0x3f, reg_base + SPI_ERROR_STATUS);
        return 128;
    }

    /* OK, no error */
    return 0;
}

unsigned long spi_flash_init(int index)
{
    unsigned int phy_flash_size = 0;

    int i;

    if (index >= CONFIG_MAX_SPI_FLASH_BANKS)
        return -1;

    reg_base = get_spi_flash_reg_base();    
    
    /*for read ID */
    writeb(0x10, reg_base + SPI_RD_WR_CTRL);

    if (index == 0)
        flash_id[index] = readl(reg_base + SPI_MEM0_SR_ACC);
    else if (index == 1)
        flash_id[index] = readl(reg_base + SPI_MEM1_SR_ACC);
 
    writeb(0x00, reg_base + SPI_RD_WR_CTRL);
   
    if (index == 0) {
        phy_flash_addr_0 = 0xffffffff - SPI_FLASH_SIZE + 1;
        phy_flash_base = phy_flash_addr_0;
    } else {
        phy_flash_addr_1 = phy_flash_addr_0 - SPI_FLASH_SIZE;
        phy_flash_base = phy_flash_addr_1;
    }


    for (i = 0; i < 32; i++) {
        if ((SPI_FLASH_SIZE >> i) == 0x8000)
            phy_flash_size = i;
    }

    if (index == 0)
        writel(phy_flash_addr_0 | (phy_flash_size << 8), reg_base + CHIP_SEL_0_CFG);
    else
        writel(phy_flash_addr_1 | (phy_flash_size << 8), reg_base + CHIP_SEL_1_CFG);

    writel(0x00000000, reg_base + SPI_IF_CFG);

    return 0;    
}

static int wait_process_done(int chip_index)
{
    int rc;
    unsigned char status;

    do {
        writeb(0x00, reg_base + SPI_RD_WR_CTRL);
        status = readb(reg_base + SPI_MEM0_SR_ACC + chip_index*0x10);

        rc = flash_error_info(readl(reg_base + SPI_ERROR_STATUS));
        if (rc != 0) {
            printf("Failed, Quit\n");
            return rc;
        }
    } while(status & 0x01);

    return 0;
}

static int elite_spi_write(struct spi_flash *flash, u32 offset, size_t len, const void *buf)
{
    int i = 0;
    int ret = 0;
    unsigned int chip_index = 0;
    unsigned int status = 0;
    unsigned char *buf_local;

    unsigned long start_addr = phy_flash_base + offset;

    if ((offset + len) > SPI_FLASH_TOTAL_SIZE) {
        printf("ERROR : Addition of 'offset' and 'len' exceeds the size of SPI flash!\n");
        return 8;
    }

    printf("Writing SPI Flash @%08lx from Memory @%08lx...", start_addr, (ulong)buf);
    buf_local = (unsigned char *)buf;

    writel(0XFFFFFFFF, reg_base + SPI_ERROR_STATUS);

    writel(readl(reg_base + SPI_IF_CFG)|BIT(6), reg_base + SPI_IF_CFG);
    writeb(0x00, reg_base + SPI_RD_WR_CTRL);

    while(i < len) {
        start_addr = phy_flash_base + offset;
        if (start_addr >= phy_flash_addr_0)
            chip_index = 0;
        else
            chip_index = 1;
       
        writeb(0X06, reg_base + SPI_PROG_CMD_WBF);
        writel(0x1<<24 | (chip_index << 1) | 0x1, reg_base + SPI_PROG_CMD_CTRL);

        while(readl(reg_base + SPI_ERROR_STATUS) & BIT(31));
        while(readb(reg_base + SPI_PROG_CMD_CTRL) & BIT(0));

        writeb(0X2, reg_base + SPI_PROG_CMD_WBF);
        writeb((offset>>16)&0xFF, reg_base + SPI_PROG_CMD_WBF+1);
        writeb((offset>>8)&0xFF, reg_base + SPI_PROG_CMD_WBF+2);
        writeb((offset)&0xFF, reg_base + SPI_PROG_CMD_WBF+3);

        writeb(buf_local[i], reg_base + SPI_PROG_CMD_WBF+4);

        writel((5<<24) | (chip_index << 1) | 0x1, reg_base + SPI_PROG_CMD_CTRL);

        if (wait_process_done(chip_index))
            return -1;

        offset++;
        i++;
    } 

    writel(readl(reg_base + SPI_IF_CFG) & ~(BIT(6)), reg_base + SPI_IF_CFG);

    printf("Done\n");
    
    return 0;
}

static int elite_spi_read(struct spi_flash *flash, u32 offset, size_t len, void *buf)
{
    int ret = 0;
    unsigned int chip_index = 0;
    unsigned int i = 0;
    unsigned int start_addr = 0;
    unsigned int burst_size = 0;

    if ((offset + len) > SPI_FLASH_TOTAL_SIZE) {
        printf("ERROR : Addition of 'offset' and 'len' exceeds the size of SPI flash\n");
        return -1;
    }
    
    start_addr = phy_flash_base+offset;

    writel(0XFFFFFFFF, reg_base + SPI_ERROR_STATUS);
    writel(readl(reg_base + SPI_IF_CFG)|BIT(6), reg_base + SPI_IF_CFG);
    
    while (len > 0) {
#if 0
        start_addr = phy_flash_base + offset;
        if (start_addr >= phy_flash_addr_0)
            chip_index = 0;
        else
            chip_index = 1;
#endif
        if (len > 32)
            burst_size = 32;
        else 
            burst_size = len;

        writeb(0x3, (void*)(reg_base + SPI_PROG_CMD_WBF));
        writeb((offset>>16) & 0xff, (void*)(reg_base + SPI_PROG_CMD_WBF+1));
        writeb((offset>>8) & 0xff, (void*)(reg_base + SPI_PROG_CMD_WBF+2));
        writeb(offset & 0xff, (void*)(reg_base + SPI_PROG_CMD_WBF+3));
        writel((0x4<<24) | (burst_size<<16) | (chip_index<<1) | 0x1, (void*)(reg_base + SPI_PROG_CMD_CTRL));

        while(readl(reg_base + SPI_ERROR_STATUS) & BIT(31));

        /* wait data ready */
        while (readl((void*)(reg_base + SPI_PROG_CMD_CTRL)) & BIT(0)) {
            ret = flash_error_info(readl(reg_base + SPI_ERROR_STATUS));
            if (ret != 0) {
                return -1;
            }
        }

        for (i = 0; i < burst_size; i++) 
            *(unsigned char*)(buf + i) = readb((void*)(reg_base + SPI_PROG_CMD_RBF+i));

        len -= burst_size;
        offset += burst_size;
        buf += burst_size;
        
    }

    writel(readl(reg_base + SPI_IF_CFG) & ~(BIT(6)), reg_base + SPI_IF_CFG);

    return 0;
}

static int elite_spi_erase(struct spi_flash *flash, u32 offset, size_t len)
{
    int sector_index;
    int chip_index;
    u32 start = offset;
    u32 end = offset + len;
    u32 addr;
    int status;
    int rc;

    if (offset % SPI_SECTOR_SIZE) {
        printf("ERROR : Erase offset not multiple of sector size\n");
        return -1;
    }
    
    if (end > SPI_FLASH_TOTAL_SIZE) {
        printf("ERROR : Addition of 'offset' and 'len' exceeds the size of SPI flash\n");
        return -1;
    }
    writel(0XFFFFFFFF, reg_base + SPI_ERROR_STATUS);

    while(start < end) {
        sector_index = start/SPI_SECTOR_SIZE;
        addr = phy_flash_base + start;

        printf("Erasing Sector %2d @%08lx...", sector_index, (long unsigned int)addr);

        if (addr >= phy_flash_addr_0)
            chip_index = 0;
        else
            chip_index = 1;

        /* read status register */
        writeb(0x00, reg_base + SPI_RD_WR_CTRL);
        status = readb(reg_base + SPI_MEM0_SR_ACC + chip_index*0x10);
        while(readl(reg_base + SPI_ERROR_STATUS) & BIT(31));
        while(readb(reg_base + SPI_PROG_CMD_CTRL) & BIT(0));

        if (status & (7 << 2)) {
            status &= ~(7 << 2);

            writeb(0x3, reg_base + SPI_WR_EN_CTRL);
            if (wait_process_done(chip_index))
                return -1;
		
            writeb(status, reg_base + SPI_MEM0_SR_ACC + chip_index*0x10);

            if (wait_process_done(chip_index))
                return -1;
        }

        writeb(0x3, reg_base + SPI_WR_EN_CTRL);
        if (wait_process_done(chip_index))
            return -1;

        /* set starting address of erasing */
        addr &= 0xffff0000;
        writel(addr, reg_base + SPI_ERASE_START_ADDR);

        /* start to transfer erase command */
        writew(0x8000, reg_base + SPI_ERASE_CTRL);

        //while(readl(reg_base + SPI_ERROR_STATUS) & BIT(31));

        if (wait_process_done(chip_index))
            return -1;

        /* set SPI flash write disenable */
        //writeb(0x00, reg_base + SPI_WR_EN_CTRL);

        printf("Done!\n");
        
        start += SPI_SECTOR_SIZE;
    }        

    return 0;
}

struct spi_flash *spi_flash_probe(unsigned int bus, unsigned int cs,
                unsigned int max_hz, unsigned int spi_mode)
{
    int i, size;
    struct spi_flash *flash_tmp;
    
    for (i = 0; i < CONFIG_MAX_SPI_FLASH_BANKS; i++) {
        size = spi_flash_init(i);
        if (size == -1) {
            printf("ERROR : Failed to Initialize SPI Flash\n");
            return NULL;
        }
    }

    flash_tmp = malloc(sizeof(*flash_tmp));
    memset(flash_tmp, 0, sizeof(*flash_tmp));

    sprintf(flash_name, "BANK[0]: %s(ID - %08lx)\nBANK[1]: None-attached!", chip_name, flash_id[0]);

    flash_tmp->name = flash_name;
    flash_tmp->write = elite_spi_write;
    flash_tmp->erase = elite_spi_erase;
    flash_tmp->read = elite_spi_read;
    flash_tmp->size = SPI_FLASH_TOTAL_SIZE;

    return flash_tmp;
}

void spi_flash_free(struct spi_flash *flash)
{
    free(flash);
}
