/*
 * (C) Copyright 2005
 * 2N Telekomunikace, a.s. <www.2n.cz>
 * Ladislav Michl <michl@2n.cz>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
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
#include <nand.h>
#include <errno.h>

#ifndef CONFIG_SYS_NAND_BASE_LIST
#define CONFIG_SYS_NAND_BASE_LIST { CONFIG_SYS_NAND_BASE }
#endif

DECLARE_GLOBAL_DATA_PTR;

int nand_curr_device = -1;


nand_info_t nand_info[CONFIG_SYS_MAX_NAND_DEVICE];

#ifndef CONFIG_SYS_NAND_SELF_INIT
static struct nand_chip nand_chip[CONFIG_SYS_MAX_NAND_DEVICE];
static ulong base_address[CONFIG_SYS_MAX_NAND_DEVICE] = CONFIG_SYS_NAND_BASE_LIST;
#endif


static struct block_dev_desc nand_blkdev[CONFIG_SYS_MAX_NAND_DEVICE];
static const char default_nand_name[] = "nand";

static char dev_name[CONFIG_SYS_MAX_NAND_DEVICE][8];

static unsigned long total_nand_size; /* in kiB */

#ifdef CONFIG_PARTITIONS
static nand_info_t *nand_get_info(int dev)
{
	if (dev < 0 || dev >= CONFIG_SYS_MAX_NAND_DEVICE)
		return NULL;
	if (nand_blkdev[dev].if_type != IF_TYPE_MTD_NAND)
		return NULL;
	return &nand_info[dev];
}

block_dev_desc_t *nand_get_dev(int dev)
{
	if (dev < 0 || dev >= CONFIG_SYS_MAX_NAND_DEVICE)
		return NULL;
	if (nand_blkdev[dev].if_type != IF_TYPE_MTD_NAND)
		return NULL;
	return &nand_blkdev[dev];
}

static ulong nand_berase(int dev_num, lbaint_t start, lbaint_t blkcnt)
{
	if (blkcnt == 0)
		return 0;

	nand_info_t *nand = nand_get_info(dev_num);
	if (!nand)
		return 0;

	uint32_t blk_size = nand->writesize;

	nand_erase_options_t opts = {
	    .length = blkcnt * (loff_t)blk_size,
	    .offset = start  * (loff_t)blk_size,
	    .quiet = 1,
	    .jffs2 = 0,
	    .scrub = 0
	};

	/*
	 * Note nand_erase_opts will skip over and not fail on bad blocks.
	 * In addition, it will round up the number of blocks requested to an
	 * erase block boundary.  This can cause more blocks to be erased
	 * than requested, but this is much better than too few in the case
	 * of someone trying to erase blocks so they can subsequently write
	 * blocks.
	 */
	return nand_erase_opts(nand, &opts) ? 0 : blkcnt;
}

static ulong nand_bwrite(int dev_num, lbaint_t start, lbaint_t blkcnt,
								const void *src)
{
	if (blkcnt == 0)
		return 0;

	nand_info_t *nand = nand_get_info(dev_num);
	block_dev_desc_t *blkdev = nand_get_dev(dev_num);
	if (!nand || !blkdev)
		return 0;

	loff_t offset = (loff_t)nand->writesize * start;
	size_t length = (size_t)blkdev->blksz * blkcnt;
	int flags = 0;

#ifdef CONFIG_CMD_NAND_YAFFS
	if (blkdev->blksz == (nand->writesize + nand->oobsize))
		flags |= WITH_YAFFS_OOB;
#endif
	/*
	 * nand_write_skip_bad will update length with the number of bytes
	 * written.  By using the length to return the number of blocks
	 * written, the return value can be ignored.
	 */
	(void)nand_write_skip_bad(nand, offset, &length, NULL, nand->size, (u_char *)src, flags);

	return length / blkdev->blksz;
}

static ulong nand_bread(int dev_num, lbaint_t start, lbaint_t blkcnt,
								void *dst)
{
	if (blkcnt == 0)
		return 0;

	nand_info_t *nand = nand_get_info(dev_num);
	if (!nand)
		return 0;

	uint32_t blk_size = nand->writesize;
	loff_t offset = (loff_t)blk_size * start;
	size_t length = (size_t)blk_size * blkcnt;

	/*
	 * nand_read_skip_bad will update length with the number of bytes
	 * read.  By using the length to return the number of blocks
	 * read, the return value can be ignored.
	 */
	(void)nand_read_skip_bad(nand, offset, &length, NULL, nand->size, dst);

	return length / blk_size;
}

#ifdef CONFIG_CMD_NAND_YAFFS
static int do_nandoob(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int use_oob, i;

	if (argc != 2)
		return cmd_usage(cmdtp);

	if (!strcmp(argv[1], "enable"))
		use_oob = 1;
	else if (!strcmp(argv[1], "disable"))
		use_oob = 0;
	else
		return cmd_usage(cmdtp);

	for (i = 0; i < CONFIG_SYS_MAX_NAND_DEVICE; i++) {
		if (use_oob)
			nand_blkdev[i].blksz = nand_info[i].writesize +
							nand_info[i].oobsize;
		else
			nand_blkdev[i].blksz = nand_info[i].writesize;
	}
	printf("Writing to the OOB data is %s.\n",
					use_oob ? "enabled" : "disabled");
	return 0;
}
U_BOOT_CMD(nandoob, 2,	0, do_nandoob,
	"enable or disable writing to the OOB data",
	"(enable|disable)\n"
	"    - enable or disable writing to the OOB data"
);
#endif
#endif


/* Register an initialized NAND mtd device with the U-Boot NAND command. */
int nand_register(int devnum)
{
	struct mtd_info *mtd;
	struct block_dev_desc *blkdev = &nand_blkdev[devnum];

	if (devnum >= CONFIG_SYS_MAX_NAND_DEVICE)
		return -EINVAL;

	mtd = &nand_info[devnum];

	sprintf(dev_name[devnum], "nand%d", devnum);
	mtd->name = dev_name[devnum];

#ifdef CONFIG_MTD_DEVICE
	/*
	 * Add MTD device so that we can reference it later
	 * via the mtdcore infrastructure (e.g. ubi).
	 */
	add_mtd_device(mtd);
#endif

	blkdev->priv = &nand_chip[devnum];	/* Not currently used. */

#ifdef CONFIG_PARTITIONS
	blkdev->if_type = IF_TYPE_MTD_NAND;
	blkdev->dev = blkdev - nand_blkdev; /* array index */
	blkdev->type = DEV_TYPE_HARDDISK;
	blkdev->lba = mtd->size / mtd->writesize;
	blkdev->blksz = mtd->writesize;
	blkdev->block_read = nand_bread;
	blkdev->block_write = nand_bwrite;
	blkdev->block_erase = nand_berase;
	/*
	 * At this point, we don't have environment variables
	 * available.  The MTD's partition definitions are
	 * stored in the environment.  That means that we can
	 * not do an init_part() yet.
	 */
	blkdev->part_type = PART_TYPE_UNKNOWN;
#endif

	total_nand_size += mtd->size / 1024;

	if (nand_curr_device == -1)
		nand_curr_device = devnum;

	return 0;
}

#ifndef CONFIG_SYS_NAND_SELF_INIT
static void nand_init_chip(int i)
{
	struct mtd_info *mtd = &nand_info[i];
	struct nand_chip *nand = &nand_chip[i];
	ulong base_addr = base_address[i];
	int maxchips = CONFIG_SYS_NAND_MAX_CHIPS;

	if (maxchips < 1)
		maxchips = 1;

	mtd->priv = nand;
	nand->IO_ADDR_R = nand->IO_ADDR_W = (void  __iomem *)base_addr;

	if (board_nand_init(nand))
		return;

	if (nand_scan(mtd, maxchips))
		return;

	nand_register(i);
}
#endif

void nand_init(void)
{
#ifdef CONFIG_SYS_NAND_SELF_INIT
	board_nand_init();
#else
	int i;

	for (i = 0; i < CONFIG_SYS_MAX_NAND_DEVICE; i++)
		nand_init_chip(i);
#endif

	printf("%lu MiB\n", total_nand_size / 1024);

#ifdef CONFIG_SYS_NAND_SELECT_DEVICE
	/*
	 * Select the chip in the board/cpu specific driver
	 */
	board_nand_select_device(nand_info[nand_curr_device].priv, nand_curr_device);
#endif
}
