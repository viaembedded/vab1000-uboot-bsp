#include "elt_display.h"
#include "cbios_interface.h"
#include "fb_lib.h"
#include "surface_format_elt.h"

#ifdef CONFIG_ELITE_NAND
#include <nand.h>
#endif

//elite1k-320118d-YSW-08 - start
#ifdef CONFIG_CMD_DISPLAY
#define S3G_PMU_BASE 0xd8390000
#define IGA1_FB_OFFSET 0x100000
#define IGA2_FB_OFFSET2 0x600000
#include "fb_util.c" 
#include "fb_draw.c" 
#include "videoputx.c"
#endif
//elite1k-320118d-YSW-08 - end

extern adapter_t *gadapter;

#define S3G_PMU_BASE 0xd8390000
#define IGA1_FB_OFFSET 0x100000
#define IGA2_FB_OFFSET 0xB00000
//;gary #define SS_FB_OFFSET   0x1500000
//;gary-start
#define SS_FB_OFFSET   48*1024*1024
//;gary-end

static unsigned int rle[] = 
{
    43604, 57, 44, 22, 75, 22, 41, 65, 41, 57, 216, 57, 44, 22, 75, 22, 41, 65,
    41, 57, 216, 57, 44, 22, 75, 22, 41, 65, 41, 57, 216, 57, 44, 22, 75, 22,
    41, 65, 41, 57, 216, 57, 44, 22, 75, 22, 41, 65, 41, 57, 216, 57, 44, 22,
    75, 22, 41, 65, 41, 57, 216, 57, 44, 22, 75, 22, 41, 65, 41, 57, 216, 57,
    44, 22, 75, 22, 41, 65, 41, 57, 216, 57, 44, 22, 75, 22, 41, 65, 41, 57,
    216, 57, 44, 22, 75, 22, 41, 65, 41, 57, 216, 57, 44, 22, 75, 22, 41, 65,
    41, 57, 216, 57, 44, 22, 75, 22, 41, 65, 41, 57, 216, 57, 44, 22, 75, 22,
    41, 65, 41, 57, 216, 57, 44, 22, 75, 22, 41, 65, 41, 57, 216, 57, 44, 22,
    75, 22, 41, 65, 41, 57, 216, 57, 44, 22, 75, 22, 41, 65, 41, 57, 216, 22,
    79, 22, 75, 22, 63, 22, 62, 22, 251, 22, 79, 22, 75, 22, 63, 22, 62, 22,
    251, 22, 79, 22, 75, 22, 63, 22, 62, 22, 251, 22, 79, 22, 75, 22, 63, 22,
    62, 22, 251, 22, 79, 22, 75, 22, 63, 22, 62, 22, 251, 22, 79, 22, 75, 22,
    63, 22, 62, 22, 251, 22, 79, 22, 75, 22, 63, 22, 62, 22, 251, 22, 79, 22,
    75, 22, 63, 22, 62, 22, 251, 22, 79, 22, 75, 22, 63, 22, 62, 22, 251, 22,
    79, 22, 75, 22, 63, 22, 62, 22, 251, 55, 46, 22, 75, 22, 63, 22, 62, 55,
    218, 55, 46, 22, 75, 22, 63, 22, 62, 55, 218, 55, 46, 22, 75, 22, 63, 22,
    62, 55, 218, 55, 46, 22, 75, 22, 63, 22, 62, 55, 218, 55, 46, 22, 75, 22,
    63, 22, 62, 55, 218, 55, 46, 22, 75, 22, 63, 22, 62, 55, 218, 55, 46, 22,
    75, 22, 63, 22, 62, 55, 218, 55, 46, 22, 75, 22, 63, 22, 62, 55, 218, 55,
    46, 22, 75, 22, 63, 22, 62, 55, 218, 55, 46, 22, 75, 22, 63, 22, 62, 55,
    218, 55, 46, 22, 75, 22, 63, 22, 62, 55, 218, 55, 46, 22, 75, 22, 63, 22,
    62, 55, 218, 55, 46, 22, 75, 22, 63, 22, 62, 55, 218, 55, 46, 22, 75, 22,
    63, 22, 62, 55, 218, 22, 79, 22, 75, 22, 63, 22, 62, 22, 251, 22, 79, 22,
    75, 22, 63, 22, 62, 22, 251, 22, 79, 22, 75, 22, 63, 22, 62, 22, 251, 22,
    79, 22, 75, 22, 63, 22, 62, 22, 251, 22, 79, 22, 75, 22, 63, 22, 62, 22,
    251, 22, 79, 22, 75, 22, 63, 22, 62, 22, 251, 22, 79, 22, 75, 22, 63, 22,
    62, 22, 251, 22, 79, 22, 75, 22, 63, 22, 62, 22, 251, 22, 79, 22, 75, 22,
    63, 22, 62, 22, 251, 22, 79, 22, 75, 22, 63, 22, 62, 22, 251, 22, 79, 22,
    75, 22, 63, 22, 62, 22, 251, 22, 79, 22, 75, 22, 63, 22, 62, 22, 251, 22,
    79, 55, 42, 22, 63, 22, 62, 22, 251, 58, 43, 55, 42, 22, 63, 22, 62, 58,
    215, 58, 43, 55, 42, 22, 63, 22, 62, 58, 215, 58, 43, 55, 42, 22, 63, 22,
    62, 58, 215, 58, 43, 55, 42, 22, 63, 22, 62, 58, 215, 58, 43, 55, 42, 22,
    63, 22, 62, 58, 215, 58, 43, 55, 42, 22, 63, 22, 62, 58, 215, 58, 43, 55,
    42, 22, 63, 22, 62, 58, 215, 58, 43, 55, 42, 22, 63, 22, 62, 58, 215, 58,
    43, 55, 42, 22, 63, 22, 62, 58, 215, 58, 43, 55, 42, 22, 63, 22, 62, 58,
    215, 58, 43, 55, 42, 22, 63, 22, 62, 58, 215, 58, 43, 55, 42, 22, 63, 22,
    62, 58, 215, 58, 43, 55, 42, 22, 63, 22, 62, 58, 215, 58, 43, 55, 42, 22,
    63, 22, 62, 58, 215, 58, 43, 55, 42, 22, 63, 22, 62, 58, 215, 58, 43, 55,
    42, 22, 63, 22, 62, 58, 40451,
};

static void elite_get_ps_addr(surface_t *surface, unsigned int *ps_pitch, unsigned int *ps_addr)
{
    unsigned int address = 0;
    int top     = 0;
    int left    = 0;

    int xoffset = 0;
    int yoffset = 0;
    int pixeloffset = 0;
    int pitch;
    int x = 0;
    int y = 0;
    int fb_pitch;
    int fb_offset;


    fb_pitch  = surface->pitch;
    fb_offset = surface->offset;

    if(surface->tiled)
    {
        left = x - (x%31);
        top = y;

        address = (top*fb_pitch) + left*surface->bit_count/8;
        address = (address >> 5) << 5;
    }
    else
    {
        top = y - y %64;
        yoffset = y %64;

        if(surface->bit_count == 32)
        {
            left = x - x %64;
            xoffset = ((x %64)/8) & 0x7;
            pixeloffset = ((x%64)%8);
            pixeloffset = pixeloffset & 0x1f;
            address = top*fb_pitch + left*4*4096/64;
        }
        else if(surface->bit_count == 16)
        {
            left = x - x%128;
            xoffset = ((x %128)%16);
            pixeloffset = ((x%128)%16);
            pixeloffset = pixeloffset & 0x1f;
            address = top * fb_pitch + left * 4*4096 / 128;
        }
    }

    address += fb_offset;
    address >>=2;
    address |= 0x80000000;

    // StreamStride hold 15-4 and 256bit(32 BYTE) align
    pitch = ((fb_pitch + 63) & ~63)*16/32;     // non-tiled width aligned

    if(surface->tiled)
    {
        pitch = (pitch << 5) | (1<<31);
    }

    switch(surface->bit_count)
    {
        case 32:
            pitch |= (pixeloffset <<16);
            break;

        case 16:
            pitch |= (pixeloffset << 16);
            break;

        case 8:
        default:
            pitch |= (pixeloffset << 16);
    }

    pitch |= (xoffset << 21);
    pitch |= (yoffset << 24);

    *ps_pitch = pitch;
    *ps_addr  = address;
}

static void elite_set_ps_origin(adapter_t *adapter, int iga_index)
{
    unsigned char *mmio = adapter->mmio;
    surface_t *surface = &adapter->surface[iga_index];
    unsigned int dac_color_mode = 0, fetch_count = 0, shadow_reg = 0;

    if (surface->bit_count == 16) //hardcode RGB565 here
    {
        dac_color_mode = 1;
    }
    else//use ARGB8888 as default
    {
        dac_color_mode = 4;
    }
    fetch_count = (surface->w * surface->bit_count + 255) / 256;
    shadow_reg = (dac_color_mode << 18) | (fetch_count << 7) + (0x01 << 4) + (1 << 6);

    if (iga_index == 0)
    {
        s3g_write_reg(mmio, CR, 0x67, 0x08, ~0x0a);
        s3g_write32(mmio + 0x81c8, surface->pitch / 2);
        s3g_write32(mmio + 0x81fc, shadow_reg);
        s3g_write32(mmio + 0x81c0, (surface->offset / 4) | 0x80000000);
    }
    else
    {
        s3g_write_reg(mmio, CR_BB, 0x67, 0x08, ~0x0a);
        if (adapter->iga_active[iga_index] == UT_OUTPUT_TV)
        {
            s3g_write32(mmio + 0x81b8, surface->pitch);
        }
        else
        {
            s3g_write32(mmio + 0x81b8, surface->pitch / 2);
        }
        s3g_write32(mmio + 0x828c, shadow_reg);
        s3g_write32(mmio + 0x81b0, (surface->offset / 4) | 0x80000000);
        
    }   

}

static void elite_set_ss_overlay(adapter_t *adapter, int iga_index, surface_t *surface, unsigned int x, unsigned int y)
{
    unsigned char *mmio = adapter->mmio;
    unsigned char constant_alpha = 0xFF;
    unsigned int stride = 0;

    if (iga_index == 0)
    {
        Reg_Mm81f8      mm81f8 = {0};
        Reg_Mm81a8      mm81a8 = {0};
        Reg_Mm81dc      mm81dc = {0};
        Reg_Mm81d0      mm81d0 = {0};
        Reg_Mm81d8      mm81d8 = {0};
        Reg_Mm8190      mm8190 = {0};
        Reg_Mm8194      mm8194 = {0};

        // Set SS1 Show Position
        mm81f8.reg.Ss1_X_Start = x;
        mm81f8.reg.Ss1_Y_Start = y;

        // Set SS1 Width and Height
        mm81dc.reg.Ss1_Source_Line_Width    = surface->w;
        mm81a8.reg.Ss1_Line_Height          = surface->h;

        mm81d0.reg.Ss1_Fb_Start_Address_0   = surface->offset >> 5;
        mm81d0.reg.Ss1_Work_Reg_En          = 1;

        // Set SS1 Stride
        stride = surface->pitch >> 5;

        mm81d8.reg.Ss1_Stride = stride;
        mm81d8.reg.Ss1_Read_Length = 1;

        mm8190.reg.Ss1_Input_Format = 4;
        mm8194.reg.Keying_Mode = 9;
        mm8190.reg.Ka1_3to0_Or_Ks1 = constant_alpha & 0x0F;
        mm8190.reg.Ka1_7to4_Or_Kp1 = (constant_alpha >> 4) & 0x0F;

        s3g_write32_mask(mmio + 0x81f8, mm81f8.uint, 0x00000000);
        s3g_write32_mask(mmio + 0x81a8, mm81a8.uint, 0xFFFF0000);
        s3g_write32_mask(mmio + 0x81d8, mm81d8.uint, 0x00000000);
        s3g_write32_mask(mmio + 0x81dc, mm81dc.uint, 0xFFFF0000);
        s3g_write32_mask(mmio + 0x8190, mm8190.uint, 0xFFFF0000);
        s3g_write32_mask(mmio + 0x8194, mm8194.uint, 0x00000000);
        s3g_write32_mask(mmio + 0x81d0, mm81d0.uint, 0x00000000);
        s3g_write_reg(mmio, CR, 0x67, 0x04, ~0x04); // enable SS1
    }
    else
    {
        Reg_Mm820c      mm820c = {0};
        Reg_Mm81a8      mm81a8 = {0};
        Reg_Mm81dc      mm81dc = {0};
        Reg_Mm81bc      mm81bc = {0};
        Reg_Mm81cc      mm81cc = {0};
        Reg_Mm8190      mm8190 = {0};
        Reg_Mm818c      mm818c = {0};

        // Set SS2 Show Position
        mm820c.reg.Ss2_X_Start = x;
        mm820c.reg.Ss2_Y_Start = y;

        // Set SS2 Width and Height
        mm81dc.reg.Ss2_Source_Line_Width    = surface->w;
        mm81a8.reg.Ss2_Line_Height          = surface->h;

        // Set SS2 Start Address
        mm81bc.reg.Ss2_Fb_Start_Address_0   = surface->offset >> 5;
        mm81bc.reg.Ss2_Work_Reg_En          = 1;

        // Set SS2 Stride
        stride = surface->pitch >> 5;
        mm81cc.reg.Ss2_Stride = stride;
        mm81cc.reg.Ss2_Read_Length          = 1;// Set Read Length

        mm8190.reg.Ss2_Input_Format = 4;
        mm818c.reg.Ss2_Keying_Mode_Select = 9;
        mm8190.reg.Ka2_3to0_Or_Ks2 = constant_alpha & 0x0F;
        mm8190.reg.Ka2_7to4_Or_Kp2 = (constant_alpha >> 4) & 0x0F;

        s3g_write32_mask(mmio + 0x820c, mm820c.uint, 0x00000000);
        s3g_write32_mask(mmio + 0x81a8, mm81a8.uint, 0x0000FFFF);
        s3g_write32_mask(mmio + 0x81cc, mm81cc.uint, 0x00000000);
        s3g_write32_mask(mmio + 0x81dc, mm81dc.uint, 0x0000FFFF);
        s3g_write32_mask(mmio + 0x8190, mm8190.uint, 0x0000FFFF);
        s3g_write32_mask(mmio + 0x818c, mm818c.uint, 0xE0000000);
        s3g_write32_mask(mmio + 0x81bc, mm81bc.uint, 0x00000000);
        s3g_write_reg(mmio, CR_BB, 0x67, 0x04, ~0x04); // enable SS2
    }
}

static int s3g_decode_rle(surface_t *surface)
{
    int ret = -1;
    unsigned int i = 0, j = 0;
    unsigned int color = 0xFF99CC33;
    unsigned int *temp_buf = NULL;

    if (surface == NULL)
    {
        s3g_info("s3g_decode_rle: input params contain NULL pointers.\n");
        return ret;
    }

    temp_buf = (unsigned int *)surface->base;

    for (i = 0; i < sizeof(rle)/sizeof(rle[0]); i++)
    {
        if ((i & 0x1) == 0)
        {
            s3g_memset((void*)temp_buf, 0, rle[i] * 4);
            temp_buf += rle[i];
        }
        else
        {
            for (j = 0; j < rle[i]; j++)
            {
                *(temp_buf++) = color;
            }
        }
    }

    ret = 0;
    return ret;
}

void s3g_get_strap_regs(adapter_t *adapter, strap_reg_t *regs, int *reg_count)
{
    unsigned char *mmio = adapter->mmio;

    if(mmio)
    {
        regs[0].type  = CR;
        regs[0].mask  = 0x40;
        regs[0].index = 0x36;
        regs[0].value = s3g_get_cr(mmio, 0x36);

        regs[1].type  = CR;
        regs[1].mask  = 0x67;
        regs[1].index = 0x37;
        regs[1].value = s3g_get_cr(mmio, 0x37);

        regs[2].type  = CR;
        regs[2].mask  = 0x38;
        regs[2].index = 0x68;
        regs[2].value = s3g_get_cr(mmio, 0x68);

        regs[3].type  = CR;
        regs[3].mask  = 0x00;
        regs[3].index = 0x6f;
        regs[3].value = s3g_get_cr(mmio, 0x6f);

        regs[4].type  = CR_CC;
        regs[4].mask  = 0x01;
        regs[4].index = 0xf0;
        regs[4].value = s3g_read_reg(mmio, CR_CC, 0xf0);

        regs[5].type  = CR_CC;
        regs[5].mask  = 0x80;
        regs[5].index = 0xf1;
        regs[5].value = s3g_read_reg(mmio, CR_CC, 0xf1);

        *reg_count = 6;
    }

}

#define GET_LAST_BIT(n)     ((~((n)-1))&(n))

void s3g_set_default_active_output(adapter_t *adapter)
{
    s3g_get_connected_output(adapter);

    s3g_get_output_for_IGA(adapter);
}


extern void s3g_create_surface(adapter_t *adapter, surface_t *surface);

int s3g_set_mode(adapter_t *adapter, s3g_modeset_t *mode_set)
{
    int left = 0, top = 0;
    surface_t surface = adapter->surface_ss;
    
    s3g_power_onoff_iga(adapter, mode_set->pipe, FALSE);

    s3g_set_output_active(adapter, adapter->iga_active[0], adapter->iga_active[1]);

    s3g_cbios_set_mode(adapter, mode_set);

    elite_set_ps_origin(adapter, mode_set->pipe);

    // ss overlay to ps
    if (adapter->surface[mode_set->pipe].w < surface.w)
    {
        left = 1;
        surface.w = adapter->surface[mode_set->pipe].w;
    }
    else
    {
        left = ((adapter->surface[mode_set->pipe].w - adapter->surface_ss.w) >> 1) + 1;
    }
    
    if (adapter->surface[mode_set->pipe].h < surface.h)
    {
        top = 1;
        surface.h = adapter->surface[mode_set->pipe].h;
    }
    else
    {
        top = ((adapter->surface[mode_set->pipe].h - adapter->surface_ss.h) >> 1) + 1;
    }

    elite_set_ss_overlay(adapter, mode_set->pipe, &surface, left, top);

    s3g_power_onoff_iga(adapter, mode_set->pipe, TRUE);
    return S_OK;
}

static int s3g_get_iga_mode(adapter_t *adapter, int iga_index, s3g_mode_t *mode)
{
    s3g_output_t output = {0};
    int mode_find = 0;

    int ret = E_FAIL;

    if (adapter == NULL || mode == NULL)
    {
        s3g_error("s3g_get_mode: params contain NULL pointer!\n");
        return ret;
    }

    if (adapter->iga_active[iga_index] != UT_OUTPUT_NONE)
    {
        output.dev_type = adapter->iga_active[iga_index];
        s3g_get_bios_modes(adapter, &output);
        if (output.mode_list)
        {
            //dump all the timing mode of output
            //s3g_output_dump_mode(&output);
            ret = s3g_output_get_output_mode(&output, mode);
            if (ret == S_OK)
            {
                mode_find = 1;
            }
            else
            {
                s3g_info("Cannot get the preferred mode for output: %x, will use the hardcode mode!\n", output.dev_type);
            }
        }
        else
        {
            s3g_error("Cannot get the modelist for output: %x\n", output.dev_type);
        }

        if (output.mode_list)
        {
            s3g_free(output.mode_list);
            output.mode_list = NULL;
        }

        // if do not get the timing mode from above modelist, use hardcode mode.
        if (!mode_find)
        {
            // both IGA1 & IGA2: hardcode 1280x720@60P
            mode->xres = 1280;
            mode->yres = 720;
            mode->refresh_rate = 6000;
            mode->interlace_progressive_caps = 1;
            mode->isPreferredMode = 0; // not the preferred mode
        }
        ret = S_OK;
    }
    return ret;
}

static int s3g_init_surface(adapter_t *adapter, s3g_mode_t *iga1_mode, s3g_mode_t *iga2_mode)
{
    surface_t *surface = NULL;
    unsigned int len = 0;
    unsigned int logo_loaded = 0;
    int ret = E_FAIL;

    if (adapter == NULL || iga1_mode == NULL || iga2_mode == NULL)
    {
        s3g_error("s3g_init_surface: params contain NULL pointer!\n");
        return ret;
    }

    // init the surface of ps0
    if (adapter->iga_active[0] != UT_OUTPUT_NONE)
    {
        surface = &adapter->surface[0];
    
        surface->w = iga1_mode->xres;
        surface->h = iga1_mode->yres;
        surface->bit_count = 32;
        surface->pitch = (surface->w * surface->bit_count / 8 + 0xFF) & (~0xFF); // aligned with 2K bit
        surface->offset = IGA1_FB_OFFSET;
        surface->base = (unsigned char*)adapter->fb_base + surface->offset;
        len = surface->h * surface->pitch;
        s3g_memset((void*)surface->base, 0, len); // init the frame buffer to 0
    }

    // init the surface of ps1
    if (adapter->iga_active[1] != UT_OUTPUT_NONE)
    {
        surface = &adapter->surface[1];
    
        surface->w = iga2_mode->xres;
        surface->h = iga2_mode->yres;
        surface->bit_count = 32;
        surface->pitch = (surface->w * surface->bit_count / 8 + 0xFF) & (~0xFF); // aligned with 2K bit
        surface->offset = IGA2_FB_OFFSET;
        surface->base = (unsigned char*)adapter->fb_base + surface->offset;
        len = surface->h * surface->pitch;
        s3g_memset((void*)surface->base, 0, len); // init the frame buffer to 0
    }

    // init the ss and load the logo
    {
        surface = &adapter->surface_ss;

        surface->offset = SS_FB_OFFSET;
        surface->base = (unsigned char*)adapter->fb_base + surface->offset;
        surface->base = (unsigned char*)(((unsigned int)surface->base + 3) & (~3)); // aligned with 4 bytes

#ifdef CONFIG_ELITE_NAND
        nand_info_t* nand = &nand_info[nand_curr_device];
        logo_info_t logo_info = {0};
        loff_t maxsize = 0;

        maxsize = len = sizeof(logo_info_t);
        // read the logo info
        if(nand_read_skip_bad(nand, BOOT_LOGO_OFFSET, &len, NULL, maxsize, (unsigned char*)&logo_info) != 0)
        {
            s3g_error("nand read splash log fail!\n");
        }

        // check the tag to verify if having data in NAND
        if(logo_info.tag == 0xA5A5A5A5)
        {
            surface->w = logo_info.w;
            surface->h = logo_info.h;
            surface->bit_count = logo_info.bit_count;
            surface->pitch = logo_info.pitch;

            maxsize = len = surface->pitch * surface->h + logo_info.image_data_offset;

            if(nand_read_skip_bad(nand, BOOT_LOGO_OFFSET, &len, NULL, maxsize, (unsigned char*)surface->base) != 0)
            {
                s3g_error("nand read splash log fail!\n");
            }
            else
            {
                surface->base += logo_info.image_data_offset;
                surface->offset += logo_info.image_data_offset;
                logo_loaded = 1;
            }
        }
        else
        {
            s3g_info("No data in NAND, show default image!\n");
            logo_loaded = 0;
        }
#endif
//;elite1k-520016c-GLU-10 -start
       logo_loaded = 1;
//;elite1k-520016c-GLU-10 -end
        if (!logo_loaded)
        {
            surface->w = 600;
            surface->h = 200;
            surface->bit_count = 32;
            surface->pitch = ((surface->w * surface->bit_count / 8 + 0xFF) & (~0xFF)); // aligned with 2K bit
            s3g_decode_rle(surface);
        }
    }

    ret = S_OK;
    return ret;
}

int s3g_set_mode_to_iga(adapter_t *adapter, int iga_index, s3g_mode_t *mode)
{
    s3g_modeset_t   mode_set = {0};

    int ret = E_FAIL;

    if (adapter == NULL || mode == NULL)
    {
        s3g_error("s3g_set_mode_to_iga: params contain NULL pointer!\n");
        return ret;
    }

    if (adapter->iga_active[iga_index] != UT_OUTPUT_NONE)
    {
        mode_set.output = adapter->iga_active[iga_index];
        mode_set.pipe = iga_index;
        mode_set.width     = mode->xres;
        mode_set.height    = mode->yres;
        mode_set.dstWidth  = mode_set.width;
        mode_set.dstHeight = mode_set.height;
        mode_set.bpp       = 32;
        mode_set.refresh   = mode->refresh_rate;
        mode_set.dstRefresh = mode_set.refresh;
        mode_set.interlaceflag = mode->interlace_progressive_caps & 0x1 ? 0 : 1;
        mode_set.preferred_mode = mode->isPreferredMode & 0x1 ? 1 : 0;
        s3g_set_mode(adapter, &mode_set);

        ret = S_OK;
    }
    return ret;
}

extern void s3g_init_local_video(adapter_t *adapter);

static unsigned int get_board_id(void)
{
	return readl(0xd839341c);
}

void s3g_init_adapter(void)
{
    s3g_mode_t iga1_mode = {0};
    s3g_mode_t iga2_mode = {0};
    
    *(volatile unsigned int*)0xd8130250 = 0xffffffff;

    adapter_t *adapter     = s3g_calloc(sizeof(adapter_t));

    memset(adapter, 0, sizeof(adapter_t));

    adapter->family_id = FAMILY_ELT;

    adapter->generic_id = PCI_ID_GENERIC_ELITE;

    adapter->device_id = DEVICE_ELITE;

    adapter->chip_id = CHIP_ELT;

    adapter->revision_id = 0;

    adapter->total_local_memory_size = S3G_FB_SIZE;

    adapter->fb_base = (void*)S3G_FB_BASE;

    if ((get_board_id() & 0xfff0) == 0x73a0) {
        adapter->fb_base = (void*)(S3G_FB_BASE + 0x40000000);
    }

    adapter->mmio = (unsigned char*)S3G_MMIO_BASE;
    adapter->pmu_mmio = (unsigned char*)S3G_PMU_BASE;

    s3g_init_local_video(adapter);

    s3g_init_hardware_setting(adapter);

    s3g_init_cbios(adapter);

    s3g_cbios_post(adapter);

    s3g_cbios_init_chip(adapter);

    s3g_set_default_active_output(adapter);

    s3g_get_iga_mode(adapter, 0, &iga1_mode);
    s3g_get_iga_mode(adapter, 1, &iga2_mode);

    s3g_init_surface(adapter, &iga1_mode, &iga2_mode);

    s3g_set_mode_to_iga(adapter, 0, &iga1_mode);
    s3g_set_mode_to_iga(adapter, 1, &iga2_mode);

//elite1k-320118d-YSW-08 - start
#ifdef CONFIG_CMD_DISPLAY
    gadapter = adapter; 
#endif
//elite1k-320118d-YSW-08 - end

}

