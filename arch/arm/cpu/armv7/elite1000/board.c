/*
 *  (C) Copyright 2010,2011
 *  S3 Graphics, Inc <www.s3graphics.com>
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
#include <asm/io.h>
#include <asm/cache.h>
#include <asm/dma-mapping.h>
#include <asm/arch/pmc_clock.h>
#include <asm/arch/sys_proto.h>

DECLARE_GLOBAL_DATA_PTR;

extern elite_sysinfo sysinfo;

#ifdef CONFIG_SYS_DMA_ALLOC_LEN
static unsigned long dma_alloc_start;
static unsigned long dma_alloc_end;
static unsigned long dma_alloc_brk;
static unsigned long is_initialized = 0;

static void dma_alloc_init(void)
{
	if (is_initialized)
		return;
	is_initialized = 1;
	/* we must reserve a region of un-cached memory for DMA */
	dma_alloc_end = CONFIG_SYS_SDRAM_BASE;
	dma_alloc_start = dma_alloc_end - CONFIG_SYS_DMA_ALLOC_LEN;
	dma_alloc_brk = dma_alloc_start;

	debug("DMA:   Using memory from 0x%08lx to 0x%08lx\n",
	       dma_alloc_start, dma_alloc_end);

	invalidate_dcache_range(dma_alloc_start,
				dma_alloc_end);
}

void *dma_alloc_coherent(size_t len, unsigned long *handle)
{
#define CONFIG_SYS_DCACHE_LINESZ	32
	unsigned long paddr;

	dma_alloc_init();

	paddr = dma_alloc_brk;

	if (dma_alloc_brk + len > dma_alloc_end)
		return NULL;

	dma_alloc_brk = ((paddr + len + CONFIG_SYS_DCACHE_LINESZ - 1)
			 & ~(CONFIG_SYS_DCACHE_LINESZ - 1));

	*handle = paddr;
	return (void*)paddr;
}

unsigned long dma_map_single(volatile void *vaddr, size_t len,
					   enum dma_data_direction dir)
{
	switch (dir) {
	case DMA_BIDIRECTIONAL:
		flush_dcache_range(vaddr, vaddr + len);
		break;
	case DMA_TO_DEVICE:
		flush_dcache_range(vaddr, vaddr + len);
		break;
	case DMA_FROM_DEVICE:
		invalidate_dcache_range(vaddr, vaddr + len);
		break;
	default:
		BUG();
	}

	return virt_to_phys(vaddr);
}

#else
static inline void dma_alloc_init(void)
{

}
#endif

int board_mmc_init(bd_t *bis)
{
    return (elite_mmc_init(0, 4)) || (elite_mmc_init(1, 4))
#ifdef CONFIG_USE_EMMC
	|| (elite_mmc_init(2, 4))
#endif
	;
}

/******************************
 Routine:
 Description:
******************************/
int dram_init(void)
{
	/* dram_init must store complete ramsize in gd->ram_size */
	gd->ram_size = get_ram_size(
			(void *)CONFIG_SYS_SDRAM_BASE,
			CONFIG_MAX_RAM_BANK_SIZE);
	/* override RAM size to reserv 8M memory for GPU */
#ifdef __HDMIIN__
	gd->ram_size = CONFIG_MAX_RAM_BANK_SIZE - CONFIG_SYS_SDRAM_BASE - SZ_8M - SZ_8M*21;
	printf("Support HDMI-IN u-boot, reserve %d MB \n", SZ_8M+SZ_8M*21);
#else
	gd->ram_size = CONFIG_MAX_RAM_BANK_SIZE - CONFIG_SYS_SDRAM_BASE - SZ_8M;
	printf("No support HDMI-IN u-boot, Only reserve 8MB for secure boot \n");
#endif
return 0;
}

void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = CONFIG_SYS_SDRAM_BASE;
	gd->bd->bi_dram[0].size = gd->ram_size;
}


/*
 * Routine: misc_init_r
 * Description: A basic misc_init_r
 */
int __weak misc_init_r(void)
{
	/* currently empty */

	return 0;
}

#ifdef CONFIG_DISPLAY_BOARDINFO
/**
 * Print board information
 */
int checkboard(void)
{
	printf("Board: %s\n", sysinfo.board_string);
	return 0;
}
#endif	/* CONFIG_DISPLAY_BOARDINFO */


#ifndef CONFIG_SYS_DCACHE_OFF
void enable_caches(void)
{
	/* Enable D-cache. I-cache is already enabled in start.S */
	dcache_enable();
}
#endif


