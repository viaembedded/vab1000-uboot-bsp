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
#include <malloc.h>
#include <common.h>
#include "s3g_display.h"
#include "cbios_interface.h"
#include "CBios_uboot.h"

void* s3g_malloc_priv(unsigned long size)
{
    void* addr = NULL;
    
    addr = malloc(size);

    return addr;
}

void* s3g_calloc_priv(unsigned long size)
{
    void* addr = s3g_malloc_priv(size);
    
    if(addr != NULL)
    {
        memset(addr, 0, size);
    }

    return addr;
}

void s3g_free_priv(void* addr)
{
    if(addr == NULL)  return;

    free(addr);
}

void* s3g_malloc(unsigned long size)
{
    return s3g_malloc_priv(size);
}

void* s3g_calloc(unsigned long size)
{
    return s3g_calloc_priv(size);
}

void s3g_free(void* addr)
{
    s3g_free_priv(addr);
}

int  s3g_vsprintf(char *buf, const char *fmt, ...)
{
    va_list args;
    int ret;

    va_start(args, fmt);

    ret = vsprintf(buf, fmt, args);
 
    va_end(args);
   
    return ret;
}

unsigned short s3g_read16(void* addr)
{
    return *(volatile unsigned short*)(addr);
}


void s3g_write16(void* addr, unsigned short val)
{
    *(volatile unsigned short *)(addr) = (val);   
}

unsigned long long s3g_do_div(unsigned long long x, unsigned long long y)
{
    do_div(x, y);
    return x;
}

void s3g_sleep(int num)
{
    ssleep(num);
}

void s3g_delay_msecs(unsigned int msecs)
{
    msleep(msecs);
}

void s3g_assert(int match)
{
   if(!match)
   {
       BUG();
   }
}

void* s3g_memset(void* s, int c, unsigned long count)
{
    return memset(s, c, count);
}

void* s3g_memcpy(void* d, const void* s, unsigned long count)
{
    return memcpy(d, s, count);
}

int   s3g_memcmp(const void *s1, const void *s2, unsigned long count)
{
    return memcmp(s1, s2, count);
}

void s3g_getsecs(unsigned long * secs, unsigned long * usecs)
{
    return;
}

/* cbios call back functions */
void s3g_dbg_print(unsigned int debug_level, char *debug_message)
{
    printf("%s\n", debug_message);
}

void s3g_delay_micro_seconds(unsigned int msecs)
{
    udelay(msecs);
}

unsigned char s3g_read_uchar(void *mmio, unsigned int port)
{
    return s3g_read8((char*)mmio + port);
}

unsigned short s3g_read_ushort(void *mmio, unsigned int port)
{
    return s3g_read16((char*)mmio + port);
}

unsigned int s3g_read_ulong(void *mmio, unsigned int port)
{
    return s3g_read32((char*)mmio + port);
}

void s3g_write_uchar(void *mmio, unsigned int port, unsigned char value)
{
    s3g_write8((char*)mmio + port, value);
}

void s3g_write_ushort(void *mmio, unsigned int port, unsigned short value)
{
    s3g_write16((char*)mmio + port, value);
}

void s3g_write_ulong(void *mmio, unsigned int port, unsigned int value)
{
    s3g_write32((char*)mmio + port, value);
}

unsigned int s3g_write_buffer_ulong(unsigned int *port)
{
    printf("s3g_write_buffer_ulong is called, not implemented\n");
    return 0;
}

void s3g_read_buffer_ulong(unsigned int *port, unsigned int value)
{
    printf("s3g_read_buffer_ulong is called, not implemented\n");
}

unsigned char s3g_read_port_uchar(unsigned char *port)
{
    return 0;
}

void s3g_write_port_uchar(unsigned char *port, unsigned char value)
{
}

void s3g_write_reg32(void *data, unsigned int type, unsigned int port, unsigned int value)
{
    unsigned char *mmio;
    mmio = data;
    switch(type)
    {
    case CBIOS_REGISTER_MMIO:
        s3g_write32(mmio + port, value);
        break;

    case CBIOS_REGISTER_PMU:
        s3g_write32(0xd8390000 + port, value);
        break;

    case CBIOS_REGISTER_GPIO:
        s3g_write32(0xd8110000 + port, value);
        break;

    case CBIOS_REGISTER_PMC:
        s3g_write32(0xd8130000 + port, value);
        break;

    default:
        break;
    }
}

unsigned int s3g_read_reg32(void *data, unsigned int type, unsigned int port)
{
    unsigned char *mmio;
    mmio = data;

    unsigned int value = 0;

    switch(type)
    {
    case CBIOS_REGISTER_MMIO:
        value = s3g_read32(mmio + port);
        break;

    case CBIOS_REGISTER_PMU:
        value = s3g_read32(0xd8390000 + port);
        break;

    case CBIOS_REGISTER_GPIO:
        value = s3g_read32(0xd8110000 + port);
        break;

    case CBIOS_REGISTER_PMC:
        value = s3g_read32(0xd8130000 + port);
        break;

    default:
        break;
    }

    return value;
}

void* s3g_alloc_nonpaged_memory(unsigned int size)
{
    return s3g_calloc(size);
}

void s3g_free_nonpaged_memory(void *base)
{
    s3g_free(base);
}

void* s3g_get_io_base(void *adapter, unsigned int io_base, unsigned int size)
{
    return (void*)io_base;
}

void s3g_free_io_base(void *adapter, void *map_address)
{
}

void s3g_free_pool(void *pool)
{
    s3g_free(pool);
}

void s3g_acquire_spin_lock(void *spin_lock, void *oldirq)
{

}

void s3g_release_spin_lock(void *spin_lock, unsigned int newirq)
{

}


void s3g_query_sys_time(unsigned int *result)
{
    unsigned long sec, usec;
    unsigned long long *u64_time = (unsigned long long *)result;

    if( u64_time == NULL )
    {      
        return ;
    }
    
    sec = usec = 0;
    
    s3g_getsecs(&sec, &usec);
    
    *u64_time = ((unsigned long long)sec) <<32 | (usec & 0xFFFFFFFF);    
}

/* utility functions */


int s3g_biosdev_to_output(int bios_device, int *duoview)
{
    int output=0;

    if (bios_device & 0x01)
        output |= UT_OUTPUT_CRT1;
    if (bios_device & 0x02)
        output |= UT_OUTPUT_LCD;
    if (bios_device & 0x04)
        output |= UT_OUTPUT_TV;
    if (bios_device & 0x08)
        output |= UT_OUTPUT_HDTV;
    if (bios_device & 0x10)
        output |= UT_OUTPUT_DVI2;
    if (bios_device & 0x20)
        output |= UT_OUTPUT_DVI;
    if (bios_device & 0x40)
        output |= UT_OUTPUT_CRT2;
    if (bios_device & 0x100)
        output |= UT_OUTPUT_LCD2;

    /* the high 7 bits means same*/
    output |= ( bios_device & 0x3ff00 );

    if (duoview && (bios_device & 0x80))
        *duoview = 1;

    return output;
}

int s3g_output_to_ediddev(int output)
{
    int edid_device = 0;

    switch( output )
    {
    case UT_OUTPUT_CRT1:
        edid_device = CBIOS_DeviceINFO_CRT;
        break;
    case UT_OUTPUT_LCD:
        edid_device = CBIOS_DeviceINFO_LCD;
        break;
    case UT_OUTPUT_TV:
        edid_device = CBIOS_DeviceINFO_TV;
        break ;
    case UT_OUTPUT_DVI:
        edid_device = CBIOS_DeviceINFO_DVI;
        break;
    case UT_OUTPUT_CRT2:
        edid_device = CBIOS_DeviceINFO_CRT2;
        break;
    case UT_OUTPUT_HDTV:
        edid_device = CBIOS_DeviceINFO_HDTV;
        break;
    case UT_OUTPUT_DVI2:
        edid_device = CBIOS_DeviceINFO_DVI2;
        break;
    case UT_OUTPUT_LCD2:
        edid_device = CBIOS_DeviceINFO_LCD2;
        break;
    case UT_OUTPUT_HDMI3:
        edid_device = CBIOS_DeviceINFO_HDMI3;
        break;
    case UT_OUTPUT_DVI3:
        edid_device = CBIOS_DeviceINFO_DVI3;
        break;
    case UT_OUTPUT_DVI4:
        edid_device = CBIOS_DeviceINFO_DVI4;
        break;
    case UT_OUTPUT_HDMI1:
        edid_device = CBIOS_DeviceINFO_HDMI1;
        break;
    case UT_OUTPUT_HDMI2:
        edid_device = CBIOS_DeviceINFO_HDMI2;
        break;
    case UT_OUTPUT_HDMI4:
        edid_device = CBIOS_DeviceINFO_HDMI4;
        break;
    case UT_OUTPUT_DP5:
        edid_device = CBIOS_DeviceINFO_DP5;
        break;
    case UT_OUTPUT_DP6:
        edid_device = CBIOS_DeviceINFO_DP6;
        break;
    default:
        edid_device = CBIOS_DeviceINFO_CRT;
        break;
    }
    return edid_device;
}


int s3g_output_to_biosdev(int output, int duoview)
{
    int bios_device = 0;

    if (output & UT_OUTPUT_CRT1)
        bios_device |= 0x01;
    if (output & UT_OUTPUT_LCD)
        bios_device |= 0x02;
    if (output & UT_OUTPUT_TV)
        bios_device |= 0x04;
    if (output & UT_OUTPUT_HDTV)
        bios_device |= 0x08;
    if (output & UT_OUTPUT_DVI2)
        bios_device |= 0x10;
    if (output & UT_OUTPUT_DVI)
        bios_device |= 0x20;
    if (output & UT_OUTPUT_CRT2)
        bios_device |= 0x40;
    if (output & UT_OUTPUT_LCD2)
        bios_device |= 0x100;

    bios_device |= (output & 0x3ff00);

    if (duoview)
        bios_device |= 0x80;

    return bios_device;
}

CHIPID_HW s3g_chipid_to_cbios_chipid(unsigned int chip_id)
{
     CHIPID_HW cb_chip_id = 0;

     switch(chip_id)
     {
     case CHIP_DST2:
         cb_chip_id = CHIPID_DST2;
         break;
     case CHIP_DST3:
         cb_chip_id = CHIPID_DST3;
         break;
     case CHIP_DST4:
         cb_chip_id = CHIPID_DST4;
         break;
     case CHIP_DUMA:
         cb_chip_id = CHIPID_DUMA;
         break;
     case CHIP_E2UMA:
         cb_chip_id = CHIPID_E2UMA;
         break;
     case CHIP_ELT:
         cb_chip_id = CHIPID_ELITE;
         break;
     default:
         s3g_assert(0);
         break;
     }

     return cb_chip_id;

}


void* s3g_get_rom_image(adapter_t *adapter)
{
    return adapter->rom_image;    
}


/*CBIOS Initialization sequence:
 * 1) set Call back function
 * 2) Set Mmio Endian Mode
 * 3) CbiosInit
 * 4) CbisoPost if needed
 * Before 2, we can't let CBIOS access MMIO as we don't set MMIO mode as we required
 * */
int s3g_init_cbios(adapter_t *adapter)
{
    void *pcbe = NULL;
    CBIOS_PARAM_INIT                 CBParamInit = {0};
    CBIOS_CALLBACK_FUNCTIONS fnCallBack = {0};
    CBIOS_CHIP_ID CBChipId;
    unsigned int CBiosStatus;
    unsigned int CBiosExtensionSize;
    CBIOSPROC  fnCbios;
    PFN_CBiosSetMmioEndianMode pfnCbiosSetMmioEndianMode = NULL;
    CBIOS_VCP_DATA_ADDR_PARA    CBVCPAddrParam = {0};
	
    if (CBIOS_OK == CBiosGetProcAddress("CBiosSetMmioEndianMode", &fnCbios))
    {
        pfnCbiosSetMmioEndianMode = (PFN_CBiosSetMmioEndianMode)fnCbios;
    }
    else
    {
        s3g_error("Get CbiosSetMmioEndianMode function pointer failed\n");
    }

    fnCallBack.Size = sizeof(CBIOS_CALLBACK_FUNCTIONS);
    fnCallBack.pFnDbgPrint = s3g_dbg_print;
    fnCallBack.pFnDelayMicroSeconds = s3g_delay_micro_seconds;
    fnCallBack.pFnReadUchar = s3g_read_uchar;
    fnCallBack.pFnReadUshort = s3g_read_ushort;
    fnCallBack.pFnReadUlong = s3g_read_ulong;
    fnCallBack.pFnWriteUchar = s3g_write_uchar;
    fnCallBack.pFnWriteUshort = s3g_write_ushort;
    fnCallBack.pFnWriteUlong = s3g_write_ulong;
    fnCallBack.pFnReadBufferUlong = s3g_read_buffer_ulong;
    fnCallBack.pFnWriteBufferUlong = s3g_write_buffer_ulong;
    fnCallBack.pFnQuerySystemTime = s3g_query_sys_time;
    fnCallBack.pFnAllocateNonpagedMemory = s3g_alloc_nonpaged_memory;
    fnCallBack.pFnFreeNonpagedMemory = s3g_free_nonpaged_memory;
    fnCallBack.pFnGetDeviceBase = s3g_get_io_base;
    fnCallBack.pFnFreeDeviceBase = s3g_free_io_base;
    fnCallBack.pFnFreePool = s3g_free_pool;
    fnCallBack.pFnAcquireSpinLock = s3g_acquire_spin_lock;
    fnCallBack.pFnReleaseSpinLock = s3g_release_spin_lock;
    fnCallBack.pFnReadPortUchar = s3g_read_port_uchar;
    fnCallBack.pFnWritePortUchar = s3g_write_port_uchar;
    fnCallBack.pFnWriteRegisterU32 = s3g_write_reg32;
    fnCallBack.pFnReadRegisterU32 = s3g_read_reg32;

    //fnCallBack.pFnStrcmp            = s3g_strcmp;
    //fnCallBack.pFnStrcpy            = s3g_strcpy;
    //fnCallBack.pFnStrncmp           = s3g_strncmp;
    fnCallBack.pFnMemset            = s3g_memset;
    fnCallBack.pFnMemcpy            = s3g_memcpy;
    fnCallBack.pFnMemcmp            = s3g_memcmp;
    fnCallBack.pFnDodiv             = s3g_do_div;
    //fnCallBack.pFnVsprintf          = s3g_vsprintf;
    //fnCallBack.pFnVsprintf          = vsprintf; // for cbios debug print

    CBiosSetCallBackFunctions(&fnCallBack);

    if (CBIOS_OK != pfnCbiosSetMmioEndianMode(adapter->mmio))
    {
        s3g_error("Get cbios extension size failed\n");
    }

    s3g_memset(&CBChipId, 0, sizeof(CBIOS_CHIP_ID));
    CBChipId.Size = sizeof(CBIOS_CHIP_ID);
    CBChipId.GenericChipID = adapter->generic_id;
    CBChipId.ChipID = s3g_chipid_to_cbios_chipid(adapter->chip_id);

    CBiosStatus = CBiosGetExtensionSize(&CBChipId, &CBiosExtensionSize);

    if(CBiosExtensionSize == 0 || CBiosStatus != CBIOS_OK)
    {
        s3g_error("Get cbios extension size failed\n");
    }

    pcbe = s3g_calloc(CBiosExtensionSize);

    if(pcbe == NULL)
    {
        s3g_error("Alloc memory for cbios pcbe failed\n");
    }

    s3g_memset(&CBParamInit, 0, sizeof(CBIOS_PARAM_INIT));
    CBParamInit.pAdapterContext = adapter->mmio;
    CBParamInit.MmioBase = adapter->mmio;
    CBParamInit.RsvdVGAMmioBase = adapter->mmio + 0x8000;

    CBParamInit.MAMMPrimaryAdapter = 0;
    CBParamInit.GeneralChipID = adapter->generic_id;
    CBParamInit.PCIDeviceID = 0x110f; //adapter->device_id;
    //CBParamInit.BusType = adapter->BusType;
    CBParamInit.ChipID = s3g_chipid_to_cbios_chipid(adapter->chip_id);
    CBParamInit.ChipRevision = adapter->revision_id;
    CBParamInit.RsvdFBPhysicalAddressLow = (u32)adapter->fb_base;//vidmm_bus_addr;
    CBParamInit.RsvdFBPhysicalAddressHigh = 0;

    CBParamInit.RsvdTvChipID = 0;
    //CBParamInit.RsvdSavedRegs = NULL;
    CBParamInit.RomImage = adapter->rom_image;
    CBParamInit.RomImageLength = S3G_VBIOS_ROM_SIZE;
    CBParamInit.pSpinLock = NULL;
    CBParamInit.Size = sizeof(CBIOS_PARAM_INIT);

#if 0
    if(adapter->chip_id == CHIP_ELT)
    {
        CBParamInit.bRunOnQT = 0x1;
    }
#endif

    CBiosInit(pcbe, &CBParamInit);

     CBVCPAddrParam.VCPBaseAddr = 0x80000000 - 0x800000;    // 2G-8M
//      CBVCPAddrParam.VCPBaseAddr = 0x80000000 - 0x800000;    // 1G-8M
    CBiosSaveVCPData(pcbe, &CBVCPAddrParam);

    adapter->cbios_ext= pcbe;

    return S_OK;
    
}


extern void s3g_get_strap_regs(adapter_t *adapter, strap_reg_t *regs, int *reg_count);

int s3g_cbios_post(adapter_t *adapter)
{
    PFN_CBiosSetMmioEndianMode pfnCbiosSetMmioEndianMode = NULL;   
    CBIOS_BOOT_STRAP_REGS      strap_regs = {0};
    CBIOSPROC                  fnCbios    = NULL;
    CBIOS_PARAM_POST           param_post = {0};
    strap_reg_t                regs[10]   = {{0}};
    int                        reg_count  = 0;
    int                        ret        = S_OK;
    int                        cb_status;
    int                        test_buffer_size = 0;
    void                       *virt_address;

    if(CBIOS_OK == CBiosGetProcAddress("CBiosSetMmioEndianMode", &fnCbios))
    {
        pfnCbiosSetMmioEndianMode = (PFN_CBiosSetMmioEndianMode)fnCbios;
    }
    else
    {        
        s3g_error("Get CbiosSetMmioEndianMode function pointer failed\n");
        return E_FAIL;
    }

    if(CBIOS_OK != pfnCbiosSetMmioEndianMode(adapter->mmio))
    {      
        s3g_error("CbiosSetMmioEndianMode failed.\n");
    }

    test_buffer_size = 4 * 1024 * 1024; //4MB
    virt_address = s3g_get_io_base(adapter, (u32)adapter->fb_base, test_buffer_size);
    param_post.Size = sizeof(CBIOS_PARAM_POST);
    param_post.MappedBufferSize = test_buffer_size; 
    param_post.FBVirtualLinearAddress = virt_address;
    
    s3g_get_strap_regs(adapter, regs, &reg_count);

    strap_regs.Size = sizeof(CBIOS_BOOT_STRAP_REGS);
    strap_regs.ulCBiosVersion = CBIOSVERSION;
    strap_regs.RegCount       = reg_count;

    s3g_memcpy(&strap_regs.BootStrapReg, &regs, strap_regs.RegCount * sizeof(strap_reg_t));

    cb_status = CBiosPost(adapter->cbios_ext, &param_post, &strap_regs);
 
    if((cb_status != CBIOS_OK))
    {       
        s3g_error("CBiosPost failed. cbios status is %x\n",cb_status);
      	ret = E_FAIL;
    }
   
    s3g_free_io_base(adapter, virt_address);

    return ret;
}

int s3g_cbios_init_chip(adapter_t *adapter)
{
    CBIOS_INIT_CHIP_PARA       init_para = {0};
    int                        ret       = S_OK;
    int                        cb_status;

    init_para.Size = sizeof(CBIOS_INIT_CHIP_PARA);
    cb_status = CBiosInitChip(adapter->cbios_ext, &init_para);

    if((cb_status != CBIOS_OK))
    {       
        s3g_error("CBiosPost failed. cbios status is %x\n",cb_status);
      	ret = E_FAIL;
    }
   
    return ret;
}

int s3g_cbios_cleanup(adapter_t *adapter)
{
    CBiosUnload(adapter->cbios_ext);
    
    if(adapter->cbios_ext)
    {
        s3g_free(adapter->cbios_ext);
        
        adapter->cbios_ext = NULL;
    }

    return S_OK;
}


/* VBE: VESA BIOS Extensions */
void s3g_query_vbeinfo(adapter_t *adapter)
{
    void* pcbe = adapter->cbios_ext;
    CBIOS_VBINFO_PARAM vbeinfo = {0};
    int status = CBIOS_OK;
    
    vbeinfo.Size = sizeof(vbeinfo);
    vbeinfo.BiosVersion = CBIOSVERSION;
    status = CBiosGetVBiosInfo(pcbe, &vbeinfo);
    if(status != CBIOS_OK)
    {
        s3g_error("Get cbios vbeinfo failed\n");
    }
    else
    {
        char biosVerStr[32] = {0};
        unsigned int  tmp;

        adapter->support_output = s3g_biosdev_to_output(vbeinfo.SupportDev, 0);

        tmp = vbeinfo.BiosVersion;
        s3g_vsprintf(biosVerStr, (const char *)"%02x.%02x.%02x.%02x", (tmp>>24)&0xff, (tmp>>16)&0xff, (tmp>>8)&0xff, tmp & 0xff);
        s3g_info("bios version: %s\n", biosVerStr);
        s3g_info("bios supported device: %s\n", ut_dev_str(adapter->support_output));
    }    
}

int s3g_get_bios_modes(adapter_t *adapter, s3g_output_t *output)
{
    void                          *pcbe = adapter->cbios_ext;

    unsigned int                  mode_list_size = 0;
    unsigned int                  num_mode     = 0;
    unsigned int                  i = 0, j = 0;
    CBiosModeInfoExt              *mode_list   = NULL;

    //int mode_num_total = 0;
    //CBiosModeInfoExt *modes_total = NULL;
    int                         ret  = E_FAIL;
    
    CBIOS_ACTIVE_TYPE    active_dev = s3g_output_to_biosdev(output->dev_type, 0);

    output->mode_num = 0;
    output->mode_list = NULL;

    CBiosGetDeviceModeListBufferSize(pcbe, active_dev, &mode_list_size);
    if (mode_list_size <= 0)
    {
        return ret;
    }

    num_mode = mode_list_size / sizeof(CBiosModeInfoExt);
    mode_list = (CBiosModeInfoExt*)s3g_calloc(mode_list_size);
    if (mode_list == NULL)
    {
        return ret;
    }

    for(i = 0; i < num_mode; i++)
    {
       mode_list[i].Size = sizeof(CBiosModeInfoExt);
    }

    CBiosGetDeviceModeList(pcbe, active_dev, mode_list, &mode_list_size);

    output->mode_num = num_mode;
    output->mode_list = (void*)mode_list;

    // if the monitor supports both progressive and interlaced modes of the same xres and yres, 
    // should separate them for future use
    /*for (i = 0; i < output->mode_num; i++)
    {
        if (mode_list[i].InterlaceProgressiveCaps & 0x1)
        {
            mode_num_total++;
        }

        if (mode_list[i].InterlaceProgressiveCaps & 0x2)
        {
            mode_num_total++;
        }
    }

    if (mode_num_total > output->mode_num)
    {
        modes_total = (CBiosModeInfoExt *)s3g_calloc(mode_num_total * sizeof(CBiosModeInfoExt));
        for (i = 0; i < output->mode_num; i++)
        {
            if (mode_list[i].InterlaceProgressiveCaps & 0x1)
            {
                memcpy(&modes_total[j], &mode_list[i], sizeof(CBiosModeInfoExt));
                modes_total[j].InterlaceProgressiveCaps = 0x1;
                j++;
            }

            if (mode_list[i].InterlaceProgressiveCaps & 0x2)
            {
                memcpy(&modes_total[j], &mode_list[i], sizeof(CBiosModeInfoExt));
                modes_total[j].InterlaceProgressiveCaps = 0x2;
                j++;
            }
        }

        s3g_free(output->mode_list);
        output->mode_list = NULL;   
        
        output->mode_list = (void*)modes_total;
        output->mode_num = mode_num_total;
    }*/

    ret = S_OK;
    return ret;   
}

void s3g_output_dump_mode(s3g_output_t *output)
{
    int i = 0;
    CBiosModeInfoExt *mode_list = NULL;
    CBiosModeInfoExt *mode = NULL;

    if (output == NULL ||output->mode_list == NULL)
    {
        s3g_error("The first param is a NULL pointer or contains a NULL pointer!\n");
        return;
    }

    mode_list = (CBiosModeInfoExt*)output->mode_list;
    
    s3g_info("------modes dump for output %x------\n", output->dev_type);

    for(i = 0; i < output->mode_num; i++)
    {
        mode = &mode_list[i];
        s3g_info("%d:[%dx%d @ %dHz,  i/p caps: %x, depth_caps: %x, %s] \n", i, mode->XRes, mode->YRes,
                mode->RefreshRate, mode->InterlaceProgressiveCaps, mode->ColorDepthCaps,
                mode->isPreferredMode ? "Preferred mode" : "");
    }

    s3g_info("----modes dump for output %x end----\n", output->dev_type);
}

int s3g_output_get_output_mode(s3g_output_t *output, s3g_mode_t *mode)
{
    CBiosModeInfoExt *mode_list = NULL;
    CBiosModeInfoExt *mode_temp = NULL;
    CBiosModeInfoExt *preferred = NULL;
    CBiosModeInfoExt *max_mode = NULL;
    int i = 0;

    int ret = E_FAIL;

    if (output == NULL || output->mode_list == NULL || mode == NULL)
    {
        s3g_error("s3g_output_get_output_mode: params contain NULL pointer!\n");
        return ret;
    }

    mode_list = (CBiosModeInfoExt*)output->mode_list;

    // find the preferred mode first
    for(i = 0; i < output->mode_num; i++)
    {
        mode_temp = &mode_list[i];

        if(mode_temp->isPreferredMode)
        {
            preferred = mode_temp;
            break;
        }
    }

    // If there is no preferred mode in the total mode list, choose the maximum mode
    if(preferred == NULL)
    {
        max_mode = &mode_list[output->mode_num - 1];
    }

    if (preferred != NULL)
    {
        mode->xres = preferred->XRes;
        mode->yres = preferred->YRes;
        mode->refresh_rate = preferred->RefreshRate;
        mode->interlace_progressive_caps= preferred->InterlaceProgressiveCaps;
        mode->isPreferredMode = preferred->isPreferredMode & 0x1 ? 1 : 0;
    }
    else if (max_mode != NULL)
    {
        mode->xres = max_mode->XRes;
        mode->yres = max_mode->YRes;
        mode->refresh_rate = max_mode->RefreshRate;
        mode->interlace_progressive_caps= max_mode->InterlaceProgressiveCaps;
        mode->isPreferredMode = max_mode->isPreferredMode & 0x1 ? 1 : 0;
    }
    
    ret = S_OK;
    return ret;
}

void* s3g_read_edid(adapter_t *adapter, unsigned char port)
{
    void                           *pcbe = adapter->cbios_ext;   
    CBIOS_PARAM_GET_EDID           cbParamGetEdid;
    unsigned char                  *pEdid;
    
    pEdid = s3g_calloc(256);
    s3g_memset(&cbParamGetEdid, 0, sizeof(cbParamGetEdid));
    cbParamGetEdid.Size = sizeof(cbParamGetEdid);
    cbParamGetEdid.PortNumber = port;
    cbParamGetEdid.EdidBuffer = pEdid;
    cbParamGetEdid.EdidBufferLen = 256;

    if(CBiosGetEdid(pcbe, &cbParamGetEdid) != CBIOS_OK)
    {
        s3g_info("PORT: %d has no valid EDID.\n", port);
        s3g_free(pEdid);
        return NULL;
    }

    return pEdid;
}

int s3g_set_output_active(adapter_t *adapter, int output1, int output2)
{
    void                        *pcbe = adapter->cbios_ext;
    VIDEO_X86_BIOS_ARGUMENTS    biosArguments = {0};
    int                         status;
    int                         ret  = E_FAIL;
    
    biosArguments.Eax = CBIOS_SET_ACTDEV;
    biosArguments.Ecx = s3g_output_to_biosdev(output1, 0);
    biosArguments.Edx = s3g_output_to_biosdev(output2, 0);
    
    status = CBiosInt10(pcbe, &biosArguments);

    if(status == 0)
    {
        adapter->iga_active[0] = output1;
        adapter->iga_active[1] = output2;

        ret =  S_OK;
    }

    return ret;
}

int s3g_get_connected_output(adapter_t *adapter)
{
    void                        *pcbe = adapter->cbios_ext;
    VIDEO_X86_BIOS_ARGUMENTS    biosArguments = {0};
    int                         status;
    int                         ret  = E_FAIL;
    
    biosArguments.Ebx = S3_QUERY_ATTACHED;
    biosArguments.Eax = S3_EXTBIOS_INFO;

    status = CBiosInt10(pcbe, &biosArguments);

    if(status == CBIOS_OK)
    {
        adapter->connected_output = s3g_biosdev_to_output(biosArguments.Ecx & 0xffff,NULL);
        s3g_info("connected devices = 0x%x\n", adapter->connected_output);
        ret = S_OK;
    }
    
    return ret;
}

int s3g_get_output_for_IGA(adapter_t *adapter)
{
    void                        *pcbe = adapter->cbios_ext;
    int                         status;
    int                         ret  = E_FAIL;
    CBIOS_BOOT_DEVICE_PARA      CBiosBootDevParam = {0};
    unsigned int                boot_output = 0;

    CBiosBootDevParam.ConnectDevice = s3g_output_to_biosdev(adapter->connected_output, 0);
    s3g_info("Bios connect devices is 0x%x.\n", CBiosBootDevParam.ConnectDevice);
    CBiosBootDevParam.Size = sizeof(CBIOS_BOOT_DEVICE_PARA);
    CBiosGetBootDevice(pcbe, &CBiosBootDevParam);

    adapter->iga_active[0] = s3g_biosdev_to_output(CBiosBootDevParam.IgaActive[0], NULL);
    adapter->iga_active[1] = s3g_biosdev_to_output(CBiosBootDevParam.IgaActive[1], NULL);

    s3g_info("IGA1 device is 0x%x\n", adapter->iga_active[0]);
    s3g_info("IGA2 device is 0x%x\n", adapter->iga_active[1]);

    if (adapter->iga_active[0] != UT_OUTPUT_NONE)
    {
        s3g_info("Output for IGA1 is: 0x%x\n", adapter->iga_active[0]);
    }

    if (adapter->iga_active[1] != UT_OUTPUT_NONE)
    {
        s3g_info("Output for IGA2 is: 0x%x\n", adapter->iga_active[1]);
    }

    ret = S_OK;
    return ret;
}

int s3g_output_is_connected(adapter_t *adapter, int drv_dev)
{
    s3g_get_connected_output(adapter);

    return adapter->connected_output & drv_dev;
}

int s3g_cbios_set_mode(adapter_t *adapter, s3g_modeset_t *mode_set)
{
    void*                   pcbe = adapter->cbios_ext;
    CBIOS_STATUS            cb_status;
    char*                   str = NULL;
    char*                   str1 = NULL;
    
    CBiosSettingModeParams  mode_param = {0};

    mode_param.Size = sizeof(CBiosSettingModeParams);
    mode_param.ulCBiosVersion = CBIOSVERSION;

    mode_param.SourcModeParams.Size = sizeof(CBiosSourceModeParams);
    mode_param.SourcModeParams.XRes = mode_set->width;
    mode_param.SourcModeParams.YRes = mode_set->height;
    mode_param.SourcModeParams.ColorDepth = mode_set->bpp;

    if(mode_set->compressed)
    {
        mode_param.SourcModeParams.MemberInvalidFlags = 0x30;
    }
    
    mode_param.DestModeParams.Size = sizeof(CBiosDestModeParams);
    mode_param.DestModeParams.XRes = mode_set->dstWidth;
    mode_param.DestModeParams.YRes = mode_set->dstHeight;
    mode_param.DestModeParams.RefreshRate = mode_set->dstRefresh;
    mode_param.DestModeParams.InterlaceFlag = mode_set->interlaceflag;
    mode_param.DestModeParams.AspectRatioFlag = 0;
    mode_param.DestModeParams.Technology.TVTechnology = 0;
    mode_param.DestModeParams.ReservedFlags = 0;
    mode_param.DestModeParams.ReservedPtr = 0;
    mode_param.DestModeParams.MemberInvalidFlags = 0;
    
    mode_param.ScalerSizeParams.Size = sizeof(CBiosScalerSizeParams);
    if (mode_set->width <= 1600 && mode_set->height <=1200)
    {
        //Upscaling
        mode_param.ScalerSizeParams.XRes = mode_set->dstWidth;
        mode_param.ScalerSizeParams.YRes = mode_set->dstHeight;
    }
    else 
    {
        //Centering
        mode_param.ScalerSizeParams.XRes = mode_set->width;
        mode_param.ScalerSizeParams.YRes = mode_set->height;
    }
    mode_param.ScalerSizeParams.MemberInvalidFlags = 0;
    mode_param.IGAIndex = mode_set->pipe;
    mode_param.MemberInvalidFlags = 0;
    
    mode_param.SpecifyTiming.Size = sizeof(CBiosSpecifyDstTimingSrc);
    mode_param.SpecifyTiming.UserCustDestTiming.Size = sizeof(CBiosCustmizedDestTiming);

    cb_status = CBiosSetModeToIGA(pcbe, &mode_param);

    str = (cb_status == CBIOS_OK) ? "OK":"FAILED";
    str1 = (mode_set->preferred_mode == 1) ? "(Preferred Mode)" : "";

    s3g_info("set mode on IGA[%d]->%s%s     [%s]\n", mode_set->pipe, ut_dev_str(mode_set->output), str1, str);
    s3g_info("    source mode: %dx%d, %d bit\n", mode_param.SourcModeParams.XRes, mode_param.SourcModeParams.YRes, mode_set->bpp);
    s3g_info("    scaler size: %dx%d\n", mode_param.ScalerSizeParams.XRes, mode_param.ScalerSizeParams.YRes);
    s3g_info("    dest   mode: %dx%d@%d\n", mode_set->dstWidth, mode_set->dstHeight, mode_set->dstRefresh);
    
    return (cb_status == CBIOS_OK) ? S_OK:E_FAIL; 
}


int s3g_power_onoff_iga(adapter_t *adapter, int iga, int bOn)
{
    void                        *pcbe = adapter->cbios_ext;
    VIDEO_X86_BIOS_ARGUMENTS    biosArguments;
    int                         status;
    int                         ret = S_OK;

    s3g_memset (&biosArguments, 0, sizeof(VIDEO_X86_BIOS_ARGUMENTS));

    if(bOn)
    {
        biosArguments.Eax = 0x4f60;
    }
    else
    {
        biosArguments.Eax = 0x4f61;
    }

    biosArguments.Ebx = iga;
    biosArguments.Ecx = s3g_output_to_biosdev(adapter->iga_active[iga], 0);

    status = CBiosInt10(pcbe, &biosArguments);
  
    if(status != 0)
    {
        s3g_error("power onoff iga fail\n");
        ret = E_FAIL;
    }
    
    return ret;
}

extern CBIOS_STATUS CBiosGetDstTiming(CBIOS_VOID *pvcbe, CBIOS_U32 IGAIndex, PCBiosDestModeParams);
int s3g_get_timing(adapter_t *adapter, int iga_index)
{
    int                    status;
    CBiosDestModeParams    DstMode;
    
    s3g_memset(&DstMode, 0, sizeof(DstMode));
    DstMode.Size = sizeof(DstMode);
    status =  CBiosGetDstTiming(adapter->cbios_ext, iga_index, &DstMode);

    if(status == 0)
    {
        return DstMode.RefreshRate;
    }
    else
    {
        s3g_error("cbios get refresh rate fail\n");
    }

    return 0;
}


int s3g_get_mode_timing(adapter_t *adapter, s3g_mode_timing_info_t *info)
{
    void                       *pcbe          = adapter->cbios_ext;
    CBIOS_ACTIVE_TYPE          active_dev     = s3g_output_to_biosdev(info->uDev, 0);
    int                        status;
    CBIOS_MODE_EXT             modeExt; 
    CBIOS_TIMING_EXT           timingExt;
    
    CBIOS_GET_MODE_TIMING_PARAM timingParam;
    
    s3g_memset(&modeExt, 0, sizeof(modeExt));
    s3g_memset(&timingExt, 0, sizeof(timingExt));
    s3g_memset(&timingParam, 0, sizeof(timingParam));
    
    modeExt.Size = sizeof(modeExt);
    modeExt.XRes = info->xRes;
    modeExt.YRes = info->yRes;
    modeExt.RefreshRate = info->refresh;
    modeExt.bInterlaced = info->bInterlaced;
    modeExt.AspectRatioFlag = info->aspectRatio;
    
    timingExt.Size = sizeof(timingExt);

    timingParam.Size = sizeof(timingParam);
    timingParam.Device = active_dev;
    timingParam.pModeExt = &modeExt;
    timingParam.pTimingExt = &timingExt;
    
    status = CBiosGetModeTiming(pcbe, &timingParam);
    
    if (status != 0)
    {
        s3g_error("get mode timing fail\n");
        return E_FAIL;
    }
    
    /* pixel clock freq (kHz), orignal unit is Hz */
    info->Clock = timingExt.PixelClock / 1000;
    info->HDisplay = timingExt.XResolution;
    info->HSyncStart = timingExt.HorSyncStart;
    info->HSyncEnd = timingExt.HorSyncEnd;
    info->HTotal = timingExt.HorTotal;
    info->VDisplay = timingExt.YResolution;
    info->VSyncStart = timingExt.VerSyncStart;
    info->VSyncEnd = timingExt.VerSyncEnd;
    info->VTotal = timingExt.VerTotal;
    
    return S_OK;
}

bool s3g_i2c_data_write(adapter_t *adapter,
	u8  busnumber, 
	u8  slaveaddress,
	u8  offset,
	u8  data)
{
    void *pcbe = adapter->cbios_ext;
    CBIOS_PARAM_I2C_DATA cbParamI2cData;
    memset(&cbParamI2cData, 0, sizeof(CBIOS_PARAM_I2C_DATA));
    cbParamI2cData.Size = sizeof(CBIOS_PARAM_I2C_DATA);
    cbParamI2cData.PortNumber = busnumber;
    cbParamI2cData.SlaveAddress = slaveaddress;
    cbParamI2cData.RequestType = 1; // Not DDCCI
    cbParamI2cData.OffSet = offset;
    cbParamI2cData.Buffer = &data;
    cbParamI2cData.BufferLen = 1;

    if(CBIOS_OK == CBiosI2CDataWrite(pcbe, &cbParamI2cData))
        return true;

    return false;
}

bool s3g_i2c_data_read(adapter_t *adapter,
	u8  busnumber, 
	u8  slaveaddress,
	u8  offset,
	u8  *data)
{
    void *pcbe = adapter->cbios_ext;
    CBIOS_PARAM_I2C_DATA cbParamI2cData;
    memset(&cbParamI2cData, 0, sizeof(CBIOS_PARAM_I2C_DATA));
    cbParamI2cData.Size = sizeof(CBIOS_PARAM_I2C_DATA);
    cbParamI2cData.PortNumber = busnumber;
    cbParamI2cData.SlaveAddress = slaveaddress;
    cbParamI2cData.RequestType = 1; // Not DDCCI
    cbParamI2cData.OffSet = offset;
    cbParamI2cData.Buffer = data;
    cbParamI2cData.BufferLen = 1;

    if(CBIOS_OK == CBiosI2CDataRead(pcbe, &cbParamI2cData))
        return true;
    return false;
}

int s3g_get_clocks(adapter_t *adapter, unsigned int clock_index, unsigned int *clock)
{   
    VIDEO_X86_BIOS_ARGUMENTS bios_argu = {0};

    bios_argu.Eax = S3_EXTBIOS_INFO;
    bios_argu.Ebx = S3_GET_CLOCKS;
    bios_argu.Ecx = clock_index;

    CBiosInt10(adapter->cbios_ext, &bios_argu);

    *clock = (bios_argu.Edi + 5000) / 10000;

    return S_OK;
}

int s3g_set_clocks(adapter_t *adapter, unsigned int clock_index, unsigned int clock)
{   
    VIDEO_X86_BIOS_ARGUMENTS bios_argu = {0};

    bios_argu.Eax = S3_EXTBIOS_INFO;
    bios_argu.Ebx = S3_GET_CLOCKS;
    bios_argu.Ecx = clock_index;
    bios_argu.Edi = clock*10000 - 5000;

    CBiosInt10(adapter->cbios_ext, &bios_argu);

    return S_OK;
}


extern CBIOS_STATUS CBiosGetActiveDevice(CBIOS_VOID *pcbe, CBIOS_U32 IGAIndex, CBIOS_BOOL flag, PCBIOS_ACTIVE_TYPE pDevice);


unsigned int s3g_get_active_output(adapter_t *adapter, int iga)
{
    void              *pcbe  = adapter->cbios_ext;
    CBIOS_ACTIVE_TYPE cb_dev = 0;
    unsigned int      output = 0;

    if(CBiosGetActiveDevice(pcbe, iga, TRUE, &cb_dev) == CBIOS_OK)
    {
        output = s3g_biosdev_to_output(cb_dev, 0);
    }

    return output;
    
}

void s3g_set_downscaler(adapter_t *adapter, unsigned int width, unsigned int height, unsigned int addr, unsigned int pitch)
{
    CBiosSettingDownscalerParams downscaler_para = {0};
    void *pcbe  = adapter->cbios_ext;
    
    downscaler_para.Size = sizeof(CBiosSettingDownscalerParams);
    downscaler_para.DownscalerDestinationBase = addr;
    downscaler_para.DownscalerDestinationPitch = pitch;
    downscaler_para.SourceModeXResolution = width;
    downscaler_para.SourceModeYResolution = height;

    CBiosSetDownscaler(pcbe, &downscaler_para);
}

