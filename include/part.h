/*
 * (C) Copyright 2000-2004
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#ifndef _PART_H
#define _PART_H

#include <ide.h>
#include <errno.h>
#include <jffs2/load_kernel.h>

typedef struct block_dev_desc {
	int		if_type;	/* type of the interface */
	int		dev;		/* device number */
	unsigned char	part_type;	/* partition type */
	unsigned char	target;		/* target SCSI ID */
	unsigned char	lun;		/* target LUN */
	unsigned char	type;		/* device type */
	unsigned char	removable;	/* removable device */
#ifdef CONFIG_LBA48
	unsigned char	lba48;		/* device can use 48bit addr (ATA/ATAPI v7) */
#endif
	lbaint_t	lba;		/* number of blocks */
	unsigned long	blksz;		/* block size */
	int		log2blksz;	/* for convenience: log2(blksz) */
	char		vendor [40+1];	/* IDE model, SCSI Vendor */
	char		product[20+1];	/* IDE Serial no, SCSI product */
	char		revision[8+1];	/* firmware revision */
	unsigned long	(*block_read)(int dev,
				      lbaint_t start,
				      lbaint_t blkcnt,
				      void *buffer);
	unsigned long	(*block_write)(int dev,
				       lbaint_t start,
				       lbaint_t blkcnt,
				       const void *buffer);
	unsigned long   (*block_erase)(int dev,
				       lbaint_t start,
				       lbaint_t blkcnt);
	void		*priv;		/* driver private struct pointer */
}block_dev_desc_t;

#define BLOCK_CNT(size, block_dev_desc) (PAD_COUNT(size, block_dev_desc->blksz))
#define PAD_TO_BLOCKSIZE(size, block_dev_desc) \
	(PAD_SIZE(size, block_dev_desc->blksz))
#define LOG2(x) (((x & 0xaaaaaaaa) ? 1 : 0) + ((x & 0xcccccccc) ? 2 : 0) + \
		 ((x & 0xf0f0f0f0) ? 4 : 0) + ((x & 0xff00ff00) ? 8 : 0) + \
		 ((x & 0xffff0000) ? 16 : 0))
#define LOG2_INVALID(type) ((type)((sizeof(type)<<3)-1))

/* Interface types: */
#define IF_TYPE_UNKNOWN		0
#define IF_TYPE_IDE		1
#define IF_TYPE_SCSI		2
#define IF_TYPE_ATAPI		3
#define IF_TYPE_USB		4
#define IF_TYPE_DOC		5
#define IF_TYPE_MMC		6
#define IF_TYPE_SD		7
#define IF_TYPE_SATA		8
#define IF_TYPE_MTD		0x100
#define IF_TYPE_MTD_NOR		(IF_TYPE_MTD | MTD_DEV_TYPE_NOR)
#define IF_TYPE_MTD_NAND	(IF_TYPE_MTD | MTD_DEV_TYPE_NAND)
#define IF_TYPE_MTD_ONENAND	(IF_TYPE_MTD | MTD_DEV_TYPE_ONENAND)

/* Part types */
#define PART_TYPE_UNKNOWN	0x00
#define PART_TYPE_MAC		0x01
#define PART_TYPE_DOS		0x02
#define PART_TYPE_ISO		0x03
#define PART_TYPE_AMIGA		0x04
#define PART_TYPE_EFI		0x05
#define PART_TYPE_MTD		0x06

/*
 * Type string for U-Boot bootable partitions
 */
#define BOOT_PART_TYPE	"U-Boot"	/* primary boot partition type	*/
#define BOOT_PART_COMP	"PPCBoot"	/* PPCBoot compatibility type	*/

/* device types */
#define DEV_TYPE_UNKNOWN	0xff	/* not connected */
#define DEV_TYPE_HARDDISK	0x00	/* harddisk */
#define DEV_TYPE_TAPE		0x01	/* Tape */
#define DEV_TYPE_CDROM		0x05	/* CD-ROM */
#define DEV_TYPE_OPDISK		0x07	/* optical disk */

typedef struct disk_partition {
	lbaint_t	start;	/* # of first block in partition	*/
	lbaint_t	size;	/* number of blocks in partition	*/
	ulong	blksz;		/* block size in bytes			*/
	uchar	name[32];	/* partition name			*/
	uchar	type[32];	/* string type description		*/
	int	bootable;	/* Active/Bootable flag is set		*/
#ifdef CONFIG_PARTITION_UUIDS
	char	uuid[37];	/* filesystem UUID as string, if exists	*/
#endif
} disk_partition_t;

/* Misc _get_dev functions */
#ifdef CONFIG_PARTITIONS
block_dev_desc_t *get_dev(const char *ifname, int dev);
block_dev_desc_t* ide_get_dev(int dev);
block_dev_desc_t* sata_get_dev(int dev);
block_dev_desc_t* scsi_get_dev(int dev);
block_dev_desc_t* usb_stor_get_dev(int dev);
block_dev_desc_t* mmc_get_dev(int dev);
block_dev_desc_t* systemace_get_dev(int dev);
block_dev_desc_t* mg_disk_get_dev(int dev);
block_dev_desc_t *nand_get_dev(int dev);

/* disk/part.c */
/* Refer to doc/README.partition_funcs for information about these functions. */
int get_partition_by_name(block_dev_desc_t *dev, const char *partition_name,
				disk_partition_t *partition);
int partition_erase_pre(disk_partition_t *ptn);
int partition_erase_post(disk_partition_t *ptn);
int partition_read_pre(disk_partition_t *ptn);
int partition_read_post(disk_partition_t *ptn);
int partition_write_pre(disk_partition_t *ptn);
int partition_write_post(disk_partition_t *ptn);
int partition_erase_blks(block_dev_desc_t *dev, disk_partition_t *partition,
				lbaint_t *blkcnt);
int partition_erase_bytes(block_dev_desc_t *dev, disk_partition_t *partition,
				loff_t *bytecnt);
#ifdef CONFIG_MD5
void partition_md5_helper(block_dev_desc_t *dev, lbaint_t blk_start,
				lbaint_t *blkcnt, unsigned char md5[16]);
int partition_md5_blks(block_dev_desc_t *dev, disk_partition_t *partition,
				lbaint_t *blkcnt, unsigned char md5[16]);
int partition_md5_bytes(block_dev_desc_t *dev, disk_partition_t *partition,
				loff_t *bytecnt, unsigned char md5[16]);
#endif /* CONFIG_MD5 */
int partition_read_blks(block_dev_desc_t *dev, disk_partition_t *partition,
				lbaint_t *blkcnt, void *buffer);
int partition_read_bytes(block_dev_desc_t *dev, disk_partition_t *partition,
				loff_t *bytecnt, void *buffer);
int partition_write_blks(block_dev_desc_t *dev, disk_partition_t *partition,
				lbaint_t *blkcnt, const void *buffer);
int partition_write_bytes(block_dev_desc_t *dev, disk_partition_t *partition,
				loff_t *bytecnt, const void *buffer);
int get_partition_info (block_dev_desc_t * dev_desc, int part, disk_partition_t *info);
void print_part (block_dev_desc_t *dev_desc);
void  init_part (block_dev_desc_t *dev_desc);
void dev_print(block_dev_desc_t *dev_desc);
int get_device(const char *ifname, const char *dev_str,
	       block_dev_desc_t **dev_desc);
int get_device_and_partition(const char *ifname, const char *dev_part_str,
			     block_dev_desc_t **dev_desc,
			     disk_partition_t *info, int allow_whole_dev);
#ifndef CONFIG_MIN_PARTITION_NUM
#define CONFIG_MIN_PARTITION_NUM 0
#endif
#ifndef CONFIG_MAX_PARTITION_NUM
#define CONFIG_MAX_PARTITION_NUM 16
#endif

#else /* CONFIG_PARTITIONS */
static inline block_dev_desc_t *get_dev(const char *ifname, int dev)
{ return NULL; }
static inline block_dev_desc_t* ide_get_dev(int dev) { return NULL; }
static inline block_dev_desc_t* sata_get_dev(int dev) { return NULL; }
static inline block_dev_desc_t* scsi_get_dev(int dev) { return NULL; }
static inline block_dev_desc_t* usb_stor_get_dev(int dev) { return NULL; }
static inline block_dev_desc_t* mmc_get_dev(int dev) { return NULL; }
static inline block_dev_desc_t* systemace_get_dev(int dev) { return NULL; }
static inline block_dev_desc_t* mg_disk_get_dev(int dev) { return NULL; }
static inline block_dev_desc_t *nand_get_dev(int dev) { return NULL; }
static inline int get_partition_by_name(block_dev_desc_t *dev,
				const char *partition_name,
				disk_partition_t *partition) { return -ENODEV; }
static inline int partition_erase_pre(disk_partition_t *ptn)
							{ return -ENODEV; }
static inline int partition_erase_post(disk_partition_t *ptn)
							{ return -ENODEV; }
static inline int partition_read_pre(disk_partition_t *ptn)
							{ return -ENODEV; }
static inline int partition_read_post(disk_partition_t *ptn)
							{ return -ENODEV; }
static inline int partition_write_pre(disk_partition_t *ptn)
							{ return -ENODEV; }
static inline int partition_write_post(disk_partition_t *ptn)
							{ return -ENODEV; }
static inline int partition_erase_blks(block_dev_desc_t *dev,
				disk_partition_t *partition,
				lbaint_t *blkcnt) { return -ENODEV; }
static inline int partition_erase_bytes(block_dev_desc_t *dev,
				disk_partition_t *partition,
				loff_t *bytecnt) { return -ENODEV; }
#ifdef CONFIG_MD5
static inline void partition_md5_helper(block_dev_desc_t *dev,
				lbaint_t blk_start, lbaint_t *blkcnt,
				unsigned char md5[16]) { *blkcnt = 0; }
static inline int partition_md5_blks(block_dev_desc_t *dev,
				disk_partition_t *partition, lbaint_t *blkcnt,
				unsigned char md5[16]) { return -ENODEV; }
static inline int partition_md5_bytes(block_dev_desc_t *dev,
				disk_partition_t *partition, loff_t *bytecnt,
				unsigned char md5[16]) { return -ENODEV; }
#endif /* CONFIG_MD5 */
static inline int partition_read_blks(block_dev_desc_t *dev,
				disk_partition_t *partition, lbaint_t *blkcnt,
				void *buffer) { return -ENODEV; }
static inline int partition_read_bytes(block_dev_desc_t *dev,
				disk_partition_t *partition, loff_t *bytecnt,
				void *buffer) { return -ENODEV; }
static inline int partition_write_blks(block_dev_desc_t *dev,
				disk_partition_t *partition, lbaint_t *blkcnt,
				const void *buffer) { return -ENODEV; }
static inline int partition_write_bytes(block_dev_desc_t *dev,
				disk_partition_t *partition, loff_t *bytecnt,
				const void *buffer) { return -ENODEV; }

static inline int get_partition_info (block_dev_desc_t * dev_desc, int part,
	disk_partition_t *info) { return -1; }
static inline void print_part (block_dev_desc_t *dev_desc) {}
static inline void  init_part (block_dev_desc_t *dev_desc) {}
static inline void dev_print(block_dev_desc_t *dev_desc) {}
static inline int get_device(const char *ifname, const char *dev_str,
	       block_dev_desc_t **dev_desc)
{ return -1; }
static inline int get_device_and_partition(const char *ifname,
					   const char *dev_part_str,
					   block_dev_desc_t **dev_desc,
					   disk_partition_t *info,
					   int allow_whole_dev)
{ *dev_desc = NULL; return -1; }
#ifndef CONFIG_MIN_PARTITION_NUM
#define CONFIG_MIN_PARTITION_NUM 0
#endif
#ifndef CONFIG_MAX_PARTITION_NUM
#define CONFIG_MAX_PARTITION_NUM 0
#endif

#endif /* CONFIG_PARTITIONS */

#ifdef CONFIG_MAC_PARTITION
/* disk/part_mac.c */
int get_partition_info_mac (block_dev_desc_t * dev_desc, int part, disk_partition_t *info);
void print_part_mac (block_dev_desc_t *dev_desc);
int   test_part_mac (block_dev_desc_t *dev_desc);
#endif

#ifdef CONFIG_DOS_PARTITION
/* disk/part_dos.c */
int get_partition_info_dos (block_dev_desc_t * dev_desc, int part, disk_partition_t *info);
void print_part_dos (block_dev_desc_t *dev_desc);
int   test_part_dos (block_dev_desc_t *dev_desc);
#endif

#ifdef CONFIG_ISO_PARTITION
/* disk/part_iso.c */
int get_partition_info_iso (block_dev_desc_t * dev_desc, int part, disk_partition_t *info);
void print_part_iso (block_dev_desc_t *dev_desc);
int   test_part_iso (block_dev_desc_t *dev_desc);
#endif

#ifdef CONFIG_AMIGA_PARTITION
/* disk/part_amiga.c */
int get_partition_info_amiga (block_dev_desc_t * dev_desc, int part, disk_partition_t *info);
void print_part_amiga (block_dev_desc_t *dev_desc);
int   test_part_amiga (block_dev_desc_t *dev_desc);
#endif

#ifdef CONFIG_EFI_PARTITION
#include <part_efi.h>
/* disk/part_efi.c */
int get_partition_info_efi (block_dev_desc_t * dev_desc, int part, disk_partition_t *info);
void print_part_efi (block_dev_desc_t *dev_desc);
int   test_part_efi (block_dev_desc_t *dev_desc);

/**
 * write_gpt_table() - Write the GUID Partition Table to disk
 *
 * @param dev_desc - block device descriptor
 * @param gpt_h - pointer to GPT header representation
 * @param gpt_e - pointer to GPT partition table entries
 *
 * @return - zero on success, otherwise error
 */
int write_gpt_table(block_dev_desc_t *dev_desc,
		  gpt_header *gpt_h, gpt_entry *gpt_e);

/**
 * gpt_fill_pte(): Fill the GPT partition table entry
 *
 * @param gpt_h - GPT header representation
 * @param gpt_e - GPT partition table entries
 * @param partitions - list of partitions
 * @param parts - number of partitions
 *
 * @return zero on success
 */
int gpt_fill_pte(gpt_header *gpt_h, gpt_entry *gpt_e,
		disk_partition_t *partitions, int parts);

/**
 * gpt_fill_header(): Fill the GPT header
 *
 * @param dev_desc - block device descriptor
 * @param gpt_h - GPT header representation
 * @param str_guid - disk guid string representation
 * @param parts_count - number of partitions
 *
 * @return - error on str_guid conversion error
 */
int gpt_fill_header(block_dev_desc_t *dev_desc, gpt_header *gpt_h,
		char *str_guid, int parts_count);

/**
 * gpt_restore(): Restore GPT partition table
 *
 * @param dev_desc - block device descriptor
 * @param str_disk_guid - disk GUID
 * @param partitions - list of partitions
 * @param parts - number of partitions
 *
 * @return zero on success
 */
int gpt_restore(block_dev_desc_t *dev_desc, char *str_disk_guid,
		disk_partition_t *partitions, const int parts_count);
#endif

#ifdef CONFIG_CMD_MTDPARTS
/* common/cmd_mtdparts.c */
int get_partition_info_mtd(block_dev_desc_t *dev_desc, int part,
						disk_partition_t *info);
void print_part_mtd(block_dev_desc_t *dev_desc);
int   test_part_mtd(block_dev_desc_t *dev_desc);
#endif

#endif /* _PART_H */
