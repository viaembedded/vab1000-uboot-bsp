#ifndef __S3G_BIOS_H__
#define __S3G_BIOS_H__

/*
 * ** Copyright (c) 2001-2003, S3 Graphics, Inc.
 * ** Copyright (c) 2004-2008, S3 Graphics Co., Ltd.
 * ** All Rights Reserved.
 * **
 * ** This is UNPUBLISHED PROPRIETARY SOURCE CODE of S3 Graphics, Inc.;
 * ** the contents of this file may not be disclosed to third parties, copied or
 * ** duplicated in any form, in whole or in part, without the prior written
 * ** permission of S3 Graphics, Inc.
 * **
 * ** RESTRICTED RIGHTS LEGEND:
 * ** Use, duplication or disclosure by the Government is subject to restrictions
 * ** as set forth in subdivision (c)(1)(ii) of the Rights in Technical Data
 * ** and Computer Software clause at DFARS 252.227-7013, and/or in similar or
 * ** successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished -
 * ** rights reserved under the Copyright Laws of the United States.
 * */

#include <common.h>
#include <asm/atomic.h>
#include <asm/errno.h>
#include <asm/io.h>
#include <asm/processor.h>
#include <linux/compiler.h>
#include <linux/types.h>
#include <div64.h>
#include "CBios_uboot.h"


#define S_OK   0
#define E_FAIL 1

#define S3G_VBIOS_ROM_SIZE       0x10000 

/* adapter */
#define GRAPHICS_MODE_COLD          0  /* chip uninitialized */
#define GRAPHICS_MODE_VGA_TEXT      1
#define GRAPHICS_MODE_VGA_GRAPHICS  2
#define GRAPHICS_MODE_VESA_GRAPHICS 4
#define GRAPHICS_MODE_S3G_GRAPHICS  8

#define S3_SELECT_MCLK          0x00
#define S3_SELECT_DCLK1         0x01
#define S3_SELECT_DCLK2         0x02
#define S3_SELECT_TVCLK         0x03
#define S3_SELECT_ECLK          0x04
#define S3_SELECT_ICLK          0x05

/*
 *  all vesa bios functions(including s3 extended bios)
 */

#define BIOS_SET_VBE_MODE       0x4F02
#define BIOS_GET_VBE_MODE       0x4F03
#define BIOS_SVGA_STATE         0x4F04
#define BIOS_LOG_SCANLINE       0x4F06
#define BIOS_DAC_PALFMT         0x4F08
#define BIOS_VBE_PM_SERVICE     0x4F10
#define S3_EXTBIOS_INFO         0x4F14  /* S3 Extended BIOS services */
#define BIOS_VBE_DDC            0x4F15
#define CBIOS_SETMODE_IGA       0x4F80 /* set one IGA  */
#define CBIOS_SETMODE_IGAS      0x4F81 /* set two IGAs */
#define CBIOS_GETMODE_IGAS      0x4F82
#define CBIOS_QUERYREFRESHRATE  0x4F88
#define CBIOS_SET_ACTDEV        0x4F90
#define CBIOS_GET_ACTDEV        0x4F91
#define CBIOS_GET_DISPINFO      0x4F98

/* 
 * VGA save/restore flags copy from s3g_vgaHW.h
 */
#define VGA_SR_MODE     0x01
#define VGA_SR_FONTS    0x02
#define VGA_SR_CMAP     0x04

/*
 * For S3_EXTBIOS_INFO (0x4F14) services
 */
#define S3_VBE_INFO              0x0000  /* fn0: Query S3/VBE info */

#define S3_SET_REFRESH           0x0001  /* fn1,sub0: Set Refresh Rate for Mode */
#define S3_GET_REFRESH           0x0101  /* fn1,sub1: Get Refresh Rate for Mode */
#define S3_QUERY_REFRESH         0x0201  /* fn1,sub2: Query Refresh Rates for Mode */

#define S3_QUERY_MODELIST        0x0202  /* fn2,sub2: Query Mode List */
#define S3_GET_EXT_MODEINFO      0x0302  /* fn2,sub3: Get Extended Mode Info */

#define S3_QUERY_ATTACHED        0x0004  /* fn4,sub0: Query detected displays */

#define S3_GET_ACTIVE_DISP       0x0103  /* fn3,sub1: Get Active Display */
#define S3_SET_ACTIVE_DISP       0x0003  /* fn3,sub0: Set Active Display */
#define S3_SET_EXT_DISPLAY       0x8003  /* fn03,sub80: Alt Set Active Display */

#define S3_SET_TV_CONFIG         0x0007  /* fn07,sub00: Set TV Configuration */
#define S3_GET_TV_CONFIG         0x0107  /* fn07,sub01: Get TV Configuration */
#define S3_GET_TV_SUPPORT        0x0207  /* fn07,sub02: Get TV Standard Support */

#define S3_SET_CLOCKS            0x0005  /* fn05,sub00: Set Clocks */
#define S3_GET_CLOCKS            0x0105  /* fn05,sub01: Get Clocks */


#define S3_GET_FLATP_INFO        0x0006  /* fn06,sub00: Get Flat Panel Info */
#define S3_SET_CTREXP            0x0106  /* fn06,sub01: Set Center/Expansion */
#define S3_GET_CTREXP            0x0206  /* fn06,sub02: Get Center/Expansion */
#define S3_SET_CTREXP_EX         0x0306  /* fn06,sub03: Alternate Set Center/Expansion */
#define S3_GET_CTREXP_EX         0x0406  /* fn06,sub04: Alternate Get Center/Expansion */
#define S3_SET_LCD_REFRESH       0x0506  /* fn06,sub05: Set Flat Panel Refresh Rate */
#define S3_GET_LCD_REFRESH       0x0606  /* fn06,sub06: Get current Flat Panel Refresh Rate */
#define S3_GET_LCD_REFRESH_SUPPORT 0x0706 /* fn06,sub07: Get supported Flat Panel Refresh Rates */

enum
{
    FAMILY_CMODEL,
    FAMILY_CLB,
    FAMILY_DST,
    FAMILY_CSR,
    FAMILY_INV,
    FAMILY_EXC,
    FAMILY_ELT,
    FAMILY_LAST,
};

enum
{
    CHIP_CMODEL,
    CHIP_CLB,
    CHIP_DST,
    CHIP_CSR,
    CHIP_INV,
    CHIP_H5,
    CHIP_H5S1,
    CHIP_H6S2,
    CHIP_CMS,
    CHIP_METRO,
    CHIP_MANHATTAN,
    CHIP_MATRIX,
    CHIP_DST2,
    CHIP_DST3,
    CHIP_DUMA,
    CHIP_H6S1,
    CHIP_DST4,
    CHIP_EXC1,      //Excalibur-1
    CHIP_E2UMA,     //E2UMA
    CHIP_ELT,       //Elite
    CHIP_LAST,
};

#undef PCI_ID_DST2
#undef PCI_ID_GENERIC_ELITE

#define PCI_ID_DST      0x900F
#define PCI_ID_DST2     0x902F
#define PCI_ID_DST2M    0x903F
#define PCI_ID_DST3     0x904F
#define PCI_ID_DST3M    0x905F
#define PCI_ID_DST4     0x906F
#define PCI_ID_DST4M    0x907F
#define PCI_ID_DUMA     0x6122
#define PCI_ID_EXC2UMA  0x3A01 // Excalibur2_UMA
#define PCI_ID_ELT      0x110F          // Excalibur1 - 1100-111F

#define PCI_ID_GENERIC_DESTINATION PCI_ID_DST
#define PCI_ID_GENERIC_DUMA        PCI_ID_DUMA
#define PCI_ID_GENERIC_EXCALIBUR   PCI_ID_EXC2UMA
#define PCI_ID_GENERIC_ELITE       PCI_ID_ELT

#define DEVICE_MASK     0xFF00
#define CHIP_MASK       0x00FF
#define DST_CHIP_MASK   0x00F0

#define DEVICE_DESTINATION (PCI_ID_DST  & DEVICE_MASK)
#define DEVICE_DUMA        (PCI_ID_DUMA & DEVICE_MASK)
#define DEVICE_EXC2UMA     (PCI_ID_EXC2UMA & DEVICE_MASK)
#define DEVICE_ELITE       (PCI_ID_ELT  & DEVICE_MASK)

#define msleep(a) udelay(a * 1000)
#define ssleep(a) msleep(a * 1000)
#define s3g_info(fmt, args...)    printf(fmt, ##args)
#define s3g_error(fmt, args...)   printf(fmt, ##args)
#define s3g_printk(msglevel, fmt, args...) printf(fmt, ##args)

typedef struct
{
    unsigned int    xres;
    unsigned int    yres;
    unsigned int    refresh_rate;
    unsigned int    interlace_progressive_caps; /* Bit0: Progressive Caps Bit1:Interlace Caps */
    unsigned int    adpater_device_flags;       /* 0: Means adapter mode, 1:Means device mode */
    unsigned int    device_flags;               /* Bit definition same as device bit definitions */
    unsigned int    color_depth_caps;           /* Bit0: 32Bits ARGB color depth capability*/
                                                /* Bit1: 16Bits color depth capability*/
                                                /* Bit2: 8Bits color depth capability*/
                                                /* Bit3: ABGR888 capability */
                                                /* Bit4: ARGB 2101010 capability */
                                                /* Bit5: ABGR 2101010 capability */
    unsigned int    aspect_ratio_caps;          /* Bit0: 4:3 capability */
                                                /* Bit1: 16:9 capability */
    unsigned int    native_mode_flags;          /* =0: Means normal mode */
                                                /* =1: Means native mode */
    union
    {
        unsigned int       mode_flags;
        struct
        {
            unsigned int   isCEAMode           :1; /* Bit0 = 1, Means is a CE mode */
                                                   /*      = 0, Means is a PC normal mode */
            unsigned int   isAddedDevMode      :1; /* Bit1 = 1, Means is a added device mode */
                                                   /*           In modes whose XRes x YRes is 1920x1080, 1280x720, or 720x480, we should make */
                                                   /*           mode having RefreshRate 6000(3000) and mode having RefreshRate between 5900(2900) and */
                                                   /*           5999(2999) paried with each other  */
                                                   /*      = 0, Means is a normal mode */
                                                   /* Bit2:15  14 bits for different timing types, 5 types at present*/
            unsigned int   isEstablishedTiming :1; /*    bit2 = 1, the mode is from Established timing block */
            unsigned int   isStandardTiming    :1; /*    bit3 = 1, the mode is from Standard timing block */
            unsigned int   isDetailedTiming    :1; /*    bit4 = 1, the mode is from Detailed timing block */
            unsigned int   isSVDTiming         :1; /*    bit5 = 1, the mode is from Short Video Descriptor */
            unsigned int   isDTDTiming         :1; /*    bit6 = 1, the mode is from Detailed Timing Descriptor */
            unsigned int   RsvdModeType        :9; /*    bit7:15  for future mode types use   */
            unsigned int   Reserved            :2; /* Bit 17-16 2 bits reserved */
            unsigned int   isPreferredMode     :1; /* Bit 18: Preferred mode flag*/
                                                   /*    bit18 = 1: preferred mode*/
                                                   /*    bit18 = 0: not preferred mode*/
            unsigned int   RsvdModeFlags       :13;/* Other bits reserved for future use */
        };
    };
}s3g_mode_t;

typedef struct
{
    int  dev_type;            /* in */
    int  mode_num;        /* output */
    void *mode_list;    /* output */
}s3g_output_t;

extern void* s3g_malloc_priv(unsigned long size);
extern void* s3g_calloc_priv(unsigned long size);
extern void s3g_free_priv(void* addr);
extern void* s3g_malloc(unsigned long size);
extern void* s3g_calloc(unsigned long size);
extern void s3g_free(void* addr);
extern unsigned char s3g_read8(void* addr);
extern unsigned short s3g_read16(void* addr);
extern unsigned int  s3g_read32(void* addr);
extern void s3g_write8(void* addr, unsigned char val);
extern void s3g_write16(void* addr, unsigned short val);
extern void s3g_write32(void* addr, unsigned int val);
extern unsigned int s3g_read_ulong(void *mmio, unsigned int port);
extern void s3g_write_ulong(void *mmio, unsigned int port, unsigned int value);
extern unsigned long long s3g_do_div(unsigned long long x, unsigned long long y);
extern void s3g_sleep(int num);
extern void s3g_delay_msecs(unsigned int msecs);
extern void s3g_assert(int match);
extern void* s3g_memset(void* s, int c, unsigned long count);
extern void* s3g_memcpy(void* d, const void* s, unsigned long count);
extern int   s3g_memcmp(const void *s1, const void *s2, unsigned long count);
extern bool s3g_i2c_data_read(adapter_t *adapter, u8 busnumber, u8 slaveaddress, u8 offset, u8 *data);
extern bool s3g_i2c_data_write(adapter_t *adapter, u8 busnumber, u8 slaveaddress, u8 offset, u8 data);
extern int s3g_set_clocks(adapter_t *adapter, unsigned int clock_index, unsigned int clock);
extern int s3g_get_clocks(adapter_t *adapter, unsigned int clock_index, unsigned int *clock);
extern void* s3g_read_edid(adapter_t *adapter, unsigned char port);
extern void s3g_query_vbeinfo(adapter_t *adapter);
extern int s3g_get_connected_output(adapter_t *adapter);
extern int s3g_set_output_active(adapter_t *adapter, int output1, int output2);
extern int s3g_power_onoff_iga(adapter_t *adapter, int iga, int bOn);
extern int s3g_cbios_set_mode(adapter_t *adapter, s3g_modeset_t *mode_set);
extern int s3g_init_cbios(adapter_t *adapter);
extern int s3g_cbios_post(adapter_t *adapter);
extern int s3g_cbios_init_chip(adapter_t *adapter);
extern void s3g_set_downscaler(adapter_t *adapter, unsigned int width, unsigned int height, unsigned int addr, unsigned int pitch);
extern int s3g_get_bios_modes(adapter_t *adapter, s3g_output_t *output);
extern void s3g_output_dump_mode(s3g_output_t *output);
extern int s3g_output_get_output_mode(s3g_output_t *output, s3g_mode_t *mode);
#endif /*__S3G_BIOS_H__*/
