#include "common.h"
#include "malloc.h"
#include "adapter.h"
#include "elite_include/registercommands_elt.h"
#include "elite_include/mxu_registers_elt.h"
#include "elite_include/bci_global_register_elt.h"
#include "elite_include/registerdef_elt.h"
//#include "elite_include/bci_opcode_elt.h"
#include "elite_include/hwm_elt.h"
#include "elite_include/miu_reg_elt.h"
#define util_get_offset(type, element) (long)(&(((type*)0)->element))
#define ELT_CONTEXT_BUFFER_SIZE 0x40000

#define FLAG_BUFFER_SIZE 0x100000

static int is_pow2(unsigned int value)
{
    return !((value - 1 ) & value);
}

static unsigned int nearest_log2(unsigned int value)
{
    unsigned int result;
    unsigned int input;

    result = 0xFFFFFFFF;
    input = value;

    while (value & 0xFFFFFFF0)
    {
        value >>= 4;
        result += 4;
    }

    while (value)
    {
        value >>= 1;
        result++;
    }

    //
    // if val is not a pow2, increment by one, since
    // this function floors
    //

    if (is_pow2(input) == 0)
    {
        result++;
    }

    return(result);
}

int s3g_init_hardware_setting(adapter_t *adapter)
{
    int                             result                        = 0;
    Reg_Mxu_Misc                    reg_mxu_misc                  = {{0}};
    Reg_Page_Table_System_Address   reg_page_table_system_address = {{0}};
    unsigned int                    range_set_mask                = 0xffffffff;
    unsigned int                    range_entry_index             = 0;
    Reg_Mxu_Channel_Control         reg_mxu_channel_control;


    //init HW MXU settings through MMIO, so later we can use Ring Buffer
    // Mxu_Misc
    reg_mxu_misc.reg.Sys_Vid_Boundary = nearest_log2(adapter->total_local_memory_size)-25;
    s3g_write32(adapter->mmio + Reg_Mxu_Misc_Offset*4 + MMIO_MXU_START_ADDRESS, reg_mxu_misc.uint);

    // Page_Table_System_Address
    reg_page_table_system_address.reg.Addr = adapter->gart_table.phy_addr >> 6;
    s3g_write32(adapter->mmio + Reg_Page_Table_System_Address_Offset*4 + MMIO_MXU_START_ADDRESS, reg_page_table_system_address.uint);

    /* snoop boundary */
    s3g_write32(adapter->mmio + 8*4 + MMIO_MXU_START_ADDRESS, 1);

    // Init RangeBuf
    s3g_write32(adapter->mmio + MMIO_MXU_START_ADDRESS + MMIO_MXU_RANGETBL_MASK, range_set_mask); 

    // 2048 / 32 = 64
    // 2048 for RangeBuf size; 32 for a max size a RangeSetMask can value
    for(; range_entry_index < 512; range_entry_index++)
    {
        s3g_write32(adapter->mmio + MMIO_MXU_START_ADDRESS + MMIO_MXU_RANGETBL_VALUE + range_entry_index*4, 0);
    }
    reg_mxu_channel_control.uint = 0;
    reg_mxu_channel_control.reg.Single_Channel_Enable = 1;
    s3g_write32(adapter->mmio + MMIO_MXU_START_ADDRESS + Reg_Mxu_Channel_Control_Offset*4, reg_mxu_channel_control.uint);
#if LARGE_CHANNEL_SWIZZLE
    // Init Large Channel Swizzle
    reg_mxu_channel_control.reg.Large_Channel_Swizzle_Enable   = TRUE;
    reg_mxu_channel_control.reg.Large_Channel_Swizzle_Boudnary = (adapter->available_local_memory_size  + 0x3fffff) & ~0x3fffff;
    s3g_write32(adapter->mmio + MMIO_MXU_START_ADDRESS + Reg_Mxu_Channel_Control_Offset, reg_mxu_channel_control.uint);
#endif

    return result;
}

void elite_reset_adapter(adapter_t* adapter)
{

    unsigned int            i;

    s3g_write32(adapter->mmio + 0x8524, 0x8020);

    //wait a while
    for (i=0; i<0x10000; i++);
    
    s3g_write32(adapter->mmio + 0x8524, 0);

    s3g_write32(adapter->mmio + 0x8524, 0x8000);

    //wait a while
    for (i=0; i<0x10000; i++);

    //make a pulse to reset        
    s3g_write32(adapter->mmio + 0x8524, 0x00000000);

    //wait a while
    for (i=0; i<0x100000; i++);
}

unsigned int is_secure_boot(void)
{
    return s3g_read32(0xd8393418);
}
extern void svc_system_write_pmu_reg(unsigned int reg, unsigned int value);
void s3g_write_pmu_reg(unsigned int reg, unsigned int value)
{
    if (is_secure_boot()) {
        svc_system_write_pmu_reg(reg, value);
    } else {
        s3g_write32(reg, value);
    }
}

/* export function */


void s3g_init_local_video(adapter_t *adapter)
{
    unsigned int *reg_addr;
    unsigned int fb_base;

    int          i;

    Reg_Dynamic_Fb_Index            reg_dynamic_fb_idx       = {{0}};
    Reg_Dynamic_Fb_Content          reg_dynamic_fb_content   = {{0}};
    Reg_Dynamic_Fb_Control          reg_Dynamic_Fb_Ctl       = {{0}};

    fb_base = (unsigned int)adapter->fb_base;

    reg_addr = (unsigned int*)(adapter->pmu_mmio + 0xd200 + Reg_Dynamic_Fb_Control_Offset*4);
    reg_Dynamic_Fb_Ctl.reg.Dynamic_Fb_En = 1;    
    //s3g_write32(reg_addr, reg_Dynamic_Fb_Ctl.uint);
    s3g_write_pmu_reg(reg_addr, reg_Dynamic_Fb_Ctl.uint);

    for(i = 0; i < adapter->total_local_memory_size/0x400000; i++)
    {
        reg_addr = (unsigned int*)(adapter->pmu_mmio + 0xd200 + Reg_Dynamic_Fb_Content_Offset*4);
        reg_dynamic_fb_content.reg.Address = fb_base>>17;
        //s3g_write32(reg_addr, reg_dynamic_fb_content.uint);
        s3g_write_pmu_reg(reg_addr, reg_dynamic_fb_content.uint);

        reg_addr = (unsigned int*)(adapter->pmu_mmio + 0xd200 + Reg_Dynamic_Fb_Index_Offset*4);
        reg_dynamic_fb_idx.reg.Index  = i;
     
        //s3g_write32(reg_addr, reg_dynamic_fb_idx.uint);
        s3g_write_pmu_reg(reg_addr, reg_dynamic_fb_idx.uint);

        fb_base += 0x400000;
    }
}

/* only support 32bit and 16 bit surface */
unsigned int s3g_query_surface(adapter_t *adapter, unsigned int w, unsigned int h, unsigned int bpp, int tiled)
{
    unsigned int hw_format;
    unsigned int w_align; 
    unsigned int h_align;

    Reg_Rt_Fmt desc1 = {{0}};

    hw_format = (bpp == 32) ? HSF_B8G8R8A8_UNORM : HSF_B5G6R5_UNORM;

    if(bpp == 32)
    {
        w_align = (w + 0x40 - 1) & (~(0x40-1));
    }
    else if(bpp == 16)
    {
       w_align = (w + 0x80 - 1) & (~(0x80-1));
    }
    else
    {
        w_align = w;
        assert(0);
    }

    h_align = (h + 0x40 - 1) & (~(0x40-1));

    desc1.reg.Is_Tiling = (tiled & 1);
    desc1.reg.Width = w_align;
    desc1.reg.Height = h_align;
    desc1.reg.Format = hw_format_table[hw_format].MappedDstFormat;
    desc1.reg.Rt_Enable =1;

    return desc1.uint;
}

void s3g_create_surface(adapter_t *adapter, surface_t *surface)
{
    Reg_Rt_Fmt desc1 = {{0}};

    desc1.uint = s3g_query_surface(adapter, surface->w, surface->h, surface->bit_count, surface->tiled);

    surface->offset = FLAG_BUFFER_SIZE;

    surface->pitch = desc1.reg.Width*surface->bit_count/8;

    surface->base = (unsigned char*)adapter->fb_base + FLAG_BUFFER_SIZE;
}


