#ifndef __ADAPTER_H__
#define __ADAPTER_H__

//;gary #define S3G_FB_BASE        (0x40000000 - 0x800000)
//;gary-start
#define S3G_FB_BASE        (0x40000000 - 0x4000000) //1G-64M
//;gary-end
#define S3G_MMIO_BASE      0xd80a0000
//;gary  #define S3G_FB_SIZE        0x100000*24
//;gary-start
#define S3G_FB_SIZE        64*1024*1024 //use 64M instead of 24M for 4kx2k logo
//;gary-end
#define S3G_MAX_IGA        2 
#define S3G_ROM_IMAGE_BASE 0xffc00000 


typedef struct _fillrect_t
{
    unsigned int offset;
    unsigned int x;
    unsigned int y;
    unsigned int w;
    unsigned int h;
    unsigned int color;
    unsigned int gbd;
}fillrect_t;

typedef struct _surface_t
{
    unsigned int  w;
    unsigned int  h;
    unsigned int  bit_count;
    unsigned int  pitch;
    unsigned int  offset;
    unsigned int  tiled;
    unsigned char *base;
}surface_t;

typedef struct _logo_info_t
{
    unsigned int  size;
    unsigned int  tag; // 0xA5A5A5A5
    unsigned int  w;
    unsigned int  h;
    unsigned int  bit_count;
    unsigned int  pitch;
    unsigned int  image_data_offset;
}logo_info_t;

typedef struct _gart_table_t
{
    void *virt_addr;
    void *pcie_addr;
    unsigned int phy_addr;

    unsigned int size;
    unsigned int page_count;
    unsigned int pcie_count;
    
    int dirty;
    unsigned int gart_table_dirty_addr;
    unsigned int gart_table_dirty_mask;

}gart_table_t;

typedef struct _adapter_t
{
    unsigned int family_id;
    unsigned int generic_id;
    unsigned int device_id;
    unsigned int chip_id;
    unsigned int revision_id;

    void         *rom_image;
    void         *cbios_ext;
    unsigned int support_output;
    unsigned int connected_output;
    unsigned int iga_active[S3G_MAX_IGA];
    unsigned int cursor_offset;
    unsigned int last_cursor_offset;
    surface_t    surface[S3G_MAX_IGA];
    surface_t    surface_ss;

    unsigned char *mmio;
    unsigned char *pmu_mmio;

    /*ring buffer management */
    unsigned int engine_index;
    unsigned int *ring_buffer;
    unsigned int *current_ring_buffer;
    unsigned int ring_buffer_phy_addr;
    unsigned int ring_buffer_size;
    unsigned int ring_buffer_size_uint;
    unsigned int ring_buffer_size_left_uint;

    /* context buffer */
    void         *context_buffer;
    unsigned int context_buffer_phy_addr;

    /* fence buffer */
    void         *fence_buffer;
    unsigned int fence_buffer_phy_addr;

    /* dma buffer */
    void         *dma_buffer;
    unsigned int dma_buffer_phy_addr;

    /* pci-e memory management */
    char         *pcie;
    char         *current_pcie;
    unsigned int total_pcie_size;

    gart_table_t gart_table;

    unsigned int total_local_memory_size;
    unsigned int available_local_memory_size;
    void         *fb_base;

    void         *submit_rb_cmd;

    int two_slice_enable;

    unsigned long long last_send_fence_id;

}adapter_t;

static __inline__ void s3g_write32(void *addr, unsigned int value)
{
    *((volatile unsigned int*)addr) = value;
}

static __inline__ void s3g_write32_mask(void *addr, unsigned int value, unsigned int mask)
{
    unsigned int temp = *((volatile unsigned int*)addr);
    temp &= mask;
    value &= ~mask;
    temp |= value;
    *((volatile unsigned int*)addr) = temp;
}

static __inline__ unsigned int s3g_read32(void *addr)
{
    return *((volatile unsigned int*)addr);
}

static __inline__ void s3g_write8(void *addr, unsigned char value)
{
    *((volatile unsigned char*)addr) = value;
}

static __inline__ unsigned char s3g_read8(void *addr)
{
    return *((volatile unsigned char*)addr);
}

#endif

