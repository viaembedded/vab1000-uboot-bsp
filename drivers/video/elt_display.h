#ifndef __DISPLAY_MGR_H__
#define __DISPLAY_MGR_H__
#include "s3g_display.h"
#include "cbios_interface.h"

#define ELT_TV_DAC_CONTROL_19           0x19
 
#define ENABLE_TVFP_GAMMA_MASK          0xC0
#define ENABLE_SP1_LUT_INTERPOLATION    0x04 // SR19_2 = enable SP1 LUT interpolation
#define ENABLE_TVFP_GAMMA_PS1           0x40 // SR19_6 = enable on PS1 for TV/FP
#define ENABLE_TVFP_GAMMA_SS1           0x80 // SR19_7 = enable on SS1 for TV/FP

//
//  Extended sequencer 1A register (SR1A)
//
#define ELT_EXT_SEQ_REG_1A          0x1A

//
//  Extended sequencer 1B register (SR1B)
//
#define ELT_EXT_SEQ_REG_1B          0x1B

#define ENABLE_CRT_GAMMA_MASK       0x18
#define ENABLE_CRT_GAMMA_PS1        0x08   // SR1B_3 = enable on PS1 for CRT output
#define ENABLE_CRT_GAMMA_SS1        0x10   // SR1B_4 = enable on SS1 for CRT output

//  SR1B_6,5 CLUT1 color data output
#define CLUT1_CONFIG_COLOR_SEL      0x60   // SR1B_6,5 selects CLUT1 color data output
#define CLUT1_CONFIG_18BIT_COLOR    0x00   // SR1B_6,5=00 CLUT1 18bit color data output
#define CLUT1_CONFIG_24BIT_COLOR    0x20   // SR1B_6,5=01 CLUT1 24bit color data output
#define CLUT1_CONFIG_30BIT_COLOR    0x40   // SR1B_6,5=10 CLUT1 30bit color data output

//  SR1A_6,5 CLUT2 color data output
#define CLUT2_CONFIG_COLOR_SEL      0x60  // SR1A_6,5 selects CLUT2 color data output
#define CLUT2_CONFIG_18BIT_COLOR    0x00  // SR1A_6,5=00 CLUT2 18bit color data output
#define CLUT2_CONFIG_24BIT_COLOR    0x20  // SR1A_6,5=01 CLUT2 24bit color data output
#define CLUT2_CONFIG_30BIT_COLOR    0x40  // SR1A_6,5=10 CLUT2 30bit color data output

#define ENABLE_SP1_LUT_SPLIT        0x80  // SR1A_7 = enable SP1 LUT Split mode
                                          //        = 0 : single 256 entry SP2 LUT
                                          //        = 1 : Slit into two 128-entry LUTs

//
//  Extended sequencer 1C register (SR1C)
//
#define ELT_EXT_SEQ_REG_1C           0x1C

#define ENABLE_SP2_LUT_INTERPOLATION 0x01 // SR1C_0 = enable SP1 LUT interpolation
#define ENABLE_SP2_LUT_SPLIT         0x02 // SR1C_1 = enable SP1 LUT Split mode
                                          //        = 0 : single 256 entry SP2 LUT
                                          //        = 1 : Slit into two 128-entry LUTs

//
//  Extended sequencer 47 register (SR47)
//  CLUT Control Register
//
//  AlphaChrome and DeltaChrome both use this
//
#define ELT_CLUT_CTRL_REG            0x47

#define LUT_SELECT_MASK              0x07    // SR47_0-2 are CLUT select bits
                                             //
#define SELECT_LUT1_LUT2             0x00    // SR47_0-2=000 CLUT1+2 enabled for
                                             //   CPU writes.  Only CLUT1 reads
                                             //   are enabled.
#define SELECT_LUT1                  0x01    // SR47_0-2=001 CLUT1 only for CPU
                                             //   writes and reads
#define SELECT_LUT2                  0x02    // SR47_0-2=010 CLUT2 only for CPU
                                             //   writes and reads
#define SELECT_SS_LUT                0x03    // spectrum spread LUT for CPU
                                             //   writes and reads
#define ENABLE_CRT_GAMMA_MASK        0x18
#define ENABLE_TVFP_GAMMA_MASK       0xC0

#define ENABLE_CRT_GAMMA_PS2         0x08    // SR47_3 = enable on PS2 for CRT output
#define ENABLE_CRT_GAMMA_SS2         0x10    // SR47_4 = enable on SS2 for CRT output
#define ENABLE_TVFP_GAMMA_PS2        0x40    // SR47_6 = enable on PS2 for TV/FP
#define ENABLE_TVFP_GAMMA_SS2        0x80    // SR47_7 = enable on SS2 for TV/FP

#define ELT_BACK_COMPATIBLE2_REG     0x33
#define DISABLE_DAC_WRITE            0x10  // CR33_4=1 disable writes to DAC registers

#define CR_INDEX        0x83D4
#define CR_DATA         0x83D5
#define SR_INDEX        0x83C4
#define SR_DATA         0x83C5

//mmioOffset of registers
#define MMIO_OFFSET_SR_GROUP_A_ELT  0x8600
#define MMIO_OFFSET_SR_GROUP_B_ELT  0x8700
#define MMIO_OFFSET_CR_GROUP_A_ELT  0x8800
#define MMIO_OFFSET_CR_GROUP_B_ELT  0x8900
#define MMIO_OFFSET_CR_GROUP_C_ELT  0x8A00
#define MMIO_OFFSET_CR_GROUP_D_ELT  0x8B00  // for 4-channel MIU CR_D registers
#define MMIO_OFFSET_CR_GROUP_D0_ELT 0x8C00  // MIU channel 0 CR_D registers
#define MMIO_OFFSET_CR_GROUP_D1_ELT 0x8D00  // MIU channel 1 CR_D registers
#define MMIO_OFFSET_CR_GROUP_D2_ELT 0x8E00  // MIU channel 2 CR_D registers
#define MMIO_OFFSET_CR_GROUP_D3_ELT 0x8F00  // MIU channel 3 CR_D registers

//AR registers
#define MMIO_ATTR_INITIALIZE_REG_ELT    0x83da
#define MMIO_ATTR_ADDR_REG_ELT          0x83c0
#define MMIO_ATTR_DATA_WRITE_REG_ELT    0x83c1

//GR registers
#define MMIO_GRAPH_ADDR_REG_ELT         0x83CE
#define MMIO_GRAPH_DATA_REG_ELT         0x83CF

#define MMIO_BASE_OF_VGA3C0             0X83C0
#define MMIO_BASE_OF_VGA3D0             0X83D

#define ATTR_ADDRESS_REG        \
    (MMIO_BASE_OF_VGA3C0 + (0x03C0 - 0x03C0))
#define ATTR_DATA_WRITE_REG     \
    (MMIO_BASE_OF_VGA3C0 + (0x03C0 - 0x03C0))
#define ATTR_DATA_READ_REG      \
    (MMIO_BASE_OF_VGA3C0 + (0x03C1 - 0x03C0))
#define VIDEO_SUBSYSTEM_ENABLE  \
    (MMIO_BASE_OF_VGA3C0 + (0x03C3 - 0x03C0))
#define SEQ_ADDRESS_REG         \
    (MMIO_BASE_OF_VGA3C0 + (0x03C4 - 0x03C0))
#define SEQ_DATA_REG            \
    (MMIO_BASE_OF_VGA3C0 + (0x03C5 - 0x03C0))
#define DAC_PIXEL_MASK_REG      \
    (MMIO_BASE_OF_VGA3C0 + (0x03C6 - 0x03C0))
#define DAC_PEL_MASK            \
    (MMIO_BASE_OF_VGA3C0 + (0x03C6 - 0x03C0))
#define DAC_STATUS_REG          \
    (MMIO_BASE_OF_VGA3C0 + (0x03C7 - 0x03C0))
#define DAC_ADDRESS_READ_REG    \
    (MMIO_BASE_OF_VGA3C0 + (0x03C7 - 0x03C0))
#define DAC_ADDRESS_WRITE_REG   \
    (MMIO_BASE_OF_VGA3C0 + (0x03C8 - 0x03C0))
#define DAC_DATA_REG            \
    (MMIO_BASE_OF_VGA3C0 + (0x03C9 - 0x03C0))
#define DAC_DATA_REG_PORT       \
    (MMIO_BASE_OF_VGA3C0 + (0x03C9 - 0x03C0))
#define MISC_OUTPUT_REG_WRITE   \
    (MMIO_BASE_OF_VGA3C0 + (0x03C2 - 0x03C0))
#define MISC_OUTPUT_REG_READ    \
    (MMIO_BASE_OF_VGA3C0 + (0x03CC - 0x03C0))
#define GR_ADDRESS_REG          \
    (MMIO_BASE_OF_VGA3C0 + (0x03CE - 0x03C0))
#define GR_DATA_REG             \
    (MMIO_BASE_OF_VGA3C0 + (0x03CF - 0x03C0))
#define WAKEUP_REG              \
    (MMIO_BASE_OF_VGA3C0 + (0x0510 - 0x03C0))

#define CRT_ADDRESS_REG         \
    (MMIO_BASE_OF_VGA3D0 + (0x03D4 - 0x03D0))
#define CRT_DATA_REG            \
    (MMIO_BASE_OF_VGA3D0 + (0x03D5 - 0x03D0))
#define SYSTEM_CONTROL_REG      \
    (MMIO_BASE_OF_VGA3D0 + (0x03DA - 0x03D0))
    
#define BLEND_CONTROL   0x8190
#define SS0_WH          0x81fc
#define SS1_WH          0x8210

#define GAMMA_NUM_ENTRIES 256

#define BLEND_CONTROL_COEF0_MASK        0x000000FF
#define BLEND_CONTROL_DATAFORMAT0_MASK  0x00000F00
#define BLEND_CONTROL_COEF1_MASK        0x00FF0000
#define BLEND_CONTROL_DATAFORMAT1_MASK  0x0F000000

#define S3_DMDO_DEFAULT 0
#define S3_DMDO_90      1
#define S3_DMDO_180     2
#define S3_DMDO_270     3

/* same to  CBIOS_REGISTER_TYPE */
enum
{
    /* CR_SET = 0,     //CR registers to be set, used in s3postreg */
    /* SR_SET,         //SR registers to be set */
    /* GR_SET,         //GR registers to be set */
    CR=0,                /*CR registers           */
    SR,                /*SR registers           */
    AR,
    GR,
    MISC,
    CR_X,
    SR_X,
    CR_BB,               /* B-set of CR */
    CR_CC,              /* C-set of CR */
    CR_DD,               /* D-set of CR */
    CR_Pp,               /* CR on IGA2  */
    SR_P,               /* SR on FP2   */
    SR_BB,
    CR_D_0,
    CR_D_1,
    CR_D_2,
    CR_D_3,
    RESERVED=0xFF
};

enum
{
    IGA1 = 0,
    IGA2,
    IGA_LAST
};

typedef union
{
    struct
    {
        unsigned int       Ss1_Y_Start                 :12;  // Secondary Stream 1 Y-Start
        unsigned int       Reserved0                   :4;   // Reserved
        unsigned int       Ss1_X_Start                 :13;  // Secondary Stream 1 X-Start
        unsigned int       Reserved1                   :3;   // Reserved
    } reg;
    unsigned int uint;
} Reg_Mm81f8;

typedef union
{
    struct
    {
        unsigned int       Ss2_Y_Start                     :12;
        // Secondary Stream 2 Y-Start
        // Value = Screen line number of the first line of 
        // the secondary stream window shadow register
        unsigned int       Reserved0                       :4;
        unsigned int       Ss2_X_Start                     :12;
        // Secondary Stream 2 X-Start low 13bit
        // Value = Screen pixel number of the first pixel of the secondary stream 
        // window.shadow register
        unsigned int       Reserved1                       :4;
    } reg;
    unsigned int uint;
} Reg_Mm820c;

typedef union
{
    struct
    {
        unsigned int       Ss1_Line_Height             : 12;
        // Secondary Stream 1 Line Height. 12 bit value = H1_0 lines of the secondary stream 1 before stretching.
        // shadow register
        unsigned int       Reserved0                   : 1;
        unsigned int       Ss1_Buffer_Select           : 1;
        // 0=Ss1_Start_Address0    0: With single buffering enabled, SS1 start address 0 ( MM81D0[27:3] ) is used for the SS1
        // 1=Ss1_Start_Address1    1: With single buffering enabled, SS1 start address 1 ( MM8290[27:3] ) is used for the SS1
        unsigned int       Ss1_Double_Buffering_Select : 1;
        // 0=Single 0: Single buffering if bit[31] of this register = 0  
        // 1=Double 1: Double buffering if bit[31] of this register = 0 
        // If double buffering is selected, the active display buffer is automatically selected by hardware, ping-ponging between secondary stream buffer 0 and secondary stream buffer 1.  
        unsigned int       Ss1_Triple_Buffering_Enable : 1;
        // 0=Buffer_Per_Bit14 0 : Disable triple buffering. Secondary stream 1 buffering mode selected by bit[14] of this register  
        // 1=Enable 1: Enable triple buffering for secondary stream 1 
        // If triple buffering is selected, MM81EC[27:3] specifies the start address for the third buffer. Hardware automatically cycles through the buffers. 
        unsigned int       Ss2_Line_Height             : 12;
        // Secondary Stream 2 Line Height. 12 bit value = H1_0 lines of the secondary stream 2 before stretching.
        // shadow register
        unsigned int       Reserved1                   : 1;
        unsigned int       Ss2_Buffer_Select           : 1;
        // 0=Ss2_Start_Address0 0: With single buffering enabled, SS2 start address 0 ( MM81BC[27:3] ) is used for the SS2  
        // 1=Ss2_Start_Address1 1: With single buffering enabled, SS2 start address 1 ( MM8370[27:3] ) is used for the SS2  
        unsigned int       Ss2_Double_Buffering_Select : 1;
        // 0=Single 0: Single buffering if bit[31] of this register = 0  
        // 1=Double 1: Double buffering if bit[31] of this register = 0 
        // If double buffering is selected, the active display buffer is automatically selected by hardware, ping-ponging between secondary stream buffer 0 and secondary stream buffer 1.  
        unsigned int       Ss2_Triple_Buffering_Enable : 1;
        // 0=Buffer_Per_Bit30 0 : Disable triple buffering. Secondary stream 2 buffering mode selected by bit[30] of this register  
        // 1=Enable 1: Enable triple buffering for secondary stream 2 
        // If triple buffering is selected, MM8374[27:3] specifies the start address for the third buffer. Hardware automatically cycles through the buffers. 
    } reg;
    unsigned int uint;
} Reg_Mm81a8;

typedef union
{
    struct
    {
        unsigned int       Reserved0                   :3;
        unsigned int       Ss1_Fb_Start_Address_0      :27;
        unsigned int       Reserved1                   :1;
        unsigned int       Ss1_Work_Reg_En             :1;
    } reg;
    unsigned int uint;
} Reg_Mm81d0;

typedef union
{
    struct
    {
        unsigned int       Ss1_Read_Length                 : 2;
        // 0=512bit_Read 0:512bit read  
        // 1=256bit_Read 1:256bit read  
        // 2=1024bit_Read 2:1024bit read  
        // 3=Reserved 3:reserved  
        unsigned int       Reserved0                       : 2;
        unsigned int       Ss1_Stride                      : 12;
        // Secondary Stream 1 Stride. For linear addressing(bit 31=0), this is an 12 bit [15:4] 
        // HexWord(256-bit) aligned Value = byte offset of vertically adjacent pixels in the secondary stream 1 frame buffer. For tiled addressing (bit 31=1), this is a 8 bit [12:5] value = Tile Width, the number of horizontal tiles of SS1 data in the frame buffer. Stride value is in 16K bytes.
        unsigned int       Ss1_Start_Address_Byte_Offset   : 4;
        unsigned int       Reserved1                       : 11;
        unsigned int       Ss1_Abgr_En                     : 1;
    } reg;
    unsigned int uint;
} Reg_Mm81d8;

typedef union
{
    struct
    {
        unsigned int           Lut_ssc_hdmi_Info_dp_Info_Read_write_Start_Index:8;
        unsigned int           Lut_ssc_hdmi_Info_dp_Info_Read_Current_Index    :8;
        unsigned int           Lut_ssc_hdmi_Info_dp_Info_Write_Current_Index   :8;
        unsigned int           Keying_Mode                                     :4;
        // Secondary Stream1 Keying Mode Select
        unsigned int           Invert_Alpha1_Or_Ka1                            :1;
        // 0=Normal 0: Normal (when alpha1/Ka1=0, the blender output is all contributed by PS1)  
        // 1=Invert 1: Invert the alpha1/Ka1 data (when alpha1/Ka1=0, the blender output is all contributed by SS1)  
        unsigned int           Ss1_Compressed_Mode_Enable                      :1;
        unsigned int           Ss1_X_Rotation                                  :1;
        unsigned int           Ss1_Y_Rotation                                  :1;
    } reg;
    unsigned int uint;
}Reg_Mm8194;

typedef union
{
    struct
    {
        unsigned int           Ss1_Source_Line_Width           : 13;
        // Secondary Stream 1 Source Line Width. 13 bit Value = Width W1_0 in pixels of the secondary stream 1 before stretching.
        unsigned int           Reserved0                       : 1;
        unsigned int           Ss1_Fifo_Deepth                 : 2;
        // SS1 FIFO Deepth
        unsigned int           Ss2_Source_Line_Width           : 13;
        // Secondary Stream 2 Source Line Width. 13 bit Value = Width
        unsigned int           Reserved1                       : 1;   // Reserved
        unsigned int           Ss2_Fifo_Deepth                 : 2;   // SS2 FIFO Deepth
    } reg;
    unsigned int uint;
} Reg_Mm81dc;

typedef union
{
    struct
    {
        unsigned int       Reserved0                       : 3;
        unsigned int       Ss2_Fb_Start_Address_0          : 27;
        // Secondary Stream 2 frame buffer starting address 0. 27-bit Value must be 256-bit aligned secondary stream 2 frame buffer starting address 0. 
        // shadow register
        unsigned int       Reserved1                       : 1;
        unsigned int       Ss2_Work_Reg_En                 : 1;
        // 0=Dont_Update 0: Dont update  
        // 1=Update_At_Next_Vsync 1: Update SS2 work registers from shadow registers at next Vsync CR65{6]=1, and then clear this bit to "0".
    } reg;
    unsigned int uint;
}Reg_Mm81bc;

typedef union
{
    struct
    {
        unsigned int           Ss2_Read_Length                 : 2;
        // 0=512bit_Read 0: 512bit read  
        // 1=256bit_Read 1: 256bit read  
        // 2=1024bit_Read 2: 1024bit read  
        // 3=Reserved 3: reserved  
        unsigned int           Reserved0                       : 2;
        unsigned int           Ss2_Stride                      : 12;
        // Secondary Stream 2 Stride. 
        // For linear addressing(bit 31=0), this is an 12 bit [15:4]
        // HexWord(256-bit) aligned Value = byte offset of vertically 
        // adjacent pixels in the secondary stream 2 frame buffer. For tiled addressing (bit 31=1), 
        // this is a 8 bit [12:5] value = Tile Width, the number of horizontal 
        // tiles of SS2 data in the frame buffer. Stride value is in 16K bytes.
        unsigned int           Ss2_Start_Address_Pixel_Offset  : 4;
        // Secondary Stream 2 Start Address Pixel Offset.
        // These bits are programmed to enable the SS2 to
        // start at any pixel location within a 256-bit address.
        // For RGB-32, YUV (16bit video), a 3-bit [18:16] value 
        // should be programmed (range 0-15) and bit 21:19 are not used. 
        // For RGB-16, YUV (8bit video), a 4-bit [19:16] value 
        // should be programmed (range 0-15) and bits 21:20 are not used. 
        unsigned int           Reserved1                       :11;
        unsigned int           Ss2_Abgr_En                     :1;
        // 0=Disable 0: Disable. R and B are not swapped.
        // 1=Enable 1: DX10 ABGR format enabled for SS2
        // When this bit is enabled, SS2 swaps the bytes of Red and Blue for display.
    } reg;
    unsigned int uint;
}Reg_Mm81cc;

typedef union
{
    struct
    {
        unsigned int       Ka1_3to0_Or_Ks1             :4;
        // Ks1 = Secondary Stream 1 Blend Coefficient, or Ka1[3:0]. When MM8194[27:24]=1001(SS1 Key Mode Constant Alpha), this value is the lower four bits of an 8-bit Constant Alpha value Ka1 (bits 7:0 f this register).
        unsigned int       Ka1_7to4_Or_Kp1             :4;
        // Kp1 = Primary Stream 1 Blend Coefficient, or Ka1[7:4]. When MM8194[27:24]=1001(Key Mode Constant Alpha), this value is the upper four bits of an 8-bit Constant Alpha value Ka1 (bits 7:0 f this register).
        // KS1 + KP1 must be LESSEQUAL 8.
        unsigned int       Ss1_Input_Format            :3;
        // 0=Reserved 000: Reserved.  
        // 1=Rgb5.6.5 001: RGB5.6.5  
        // 2=Ycbcr422 010: YCbCr422 16bit  
        // 3=Ycbcr444 011: YCbCr444(8.8.8.8)  
        // 4=Argb 100: aRGB8.8.8.8  
        // 5=Argb 101: aRGB2.10.10.10  
        // 6=Ycbcr422 110: YCbCr422 32bit  
        // 7=Ycbcr444 111: YCbCr444(2.10.10.10)  
        unsigned int       Ss1_Ycbcr_Order             :1;
        // 0=Ycbycr 0: YCbYCr  
        // 1=Cbycry 1:CbYCrY  
        unsigned int       Reserved0                   :3;
        unsigned int       Pass_Ss1_Yuv_Direct         :1;
        // 0=Convert 0: No. Convert SS1 to RGB when SS1 is YCbCr/YUV format.  
        // 1=Pass_Direct 1: Pass SS1 to TV, YPbPr HDTV and/or HDTV DVI directly when SS1 is YCbCr/YUV format.
        unsigned int       Ka2_3to0_Or_Ks2             :4;
        // KS2 = Secondary Stream 2 Blend Coefficient, or KA2[3:0]. When MM818C[27:24]=1001(Key Mode Constant Alpha), this value is the lower four bits of an 8-bit Constant Alpha value KA2 (bits 7:0 f this register).
        unsigned int       Ka2_7to4_Or_Kp2             :4;
        // KP2 = Primary Stream 2 Blend Coefficient, or KA2[7:4]. When MM818C[27:24]=1001(Key Mode Constant Alpha), this value is the upper four bits of an 8-bit Constant Alpha value KA2 (bits 7:0 f this register).
        // KS2 + KP2 must be LESSEQUAL 8.
        unsigned int       Ss2_Input_Format            :3;
        // 0=Reserved 000: Reserved.  
        // 1=Rgb5.6.5 001: RGB5.6.5  
        // 2=Ycbcr422 010: YCbCr422 16bit  
        // 3=Ycbcr444 011: YCbCr444(8.8.8.8)  
        // 4=Argb 100: aRGB8.8.8.8  
        // 5=Argb 101: aRGB2.10.10.10  
        // 6=Ycbcr422 110: YCbCr422 32bit  
        // 7=Ycbcr444 111: YCbCr444(2.10.10.10)  
        unsigned int       Ss2_Ycbcr_Order             :1;
        // 0=Ycbycr 0: YCbYCr
        // 1=Cbycry 1:CbYCrY
        unsigned int       Reserved1                   :3;
        unsigned int       Pass_Ss2_Yuv_Direct         :1;
    }reg;
    unsigned int uint;
}Reg_Mm8190;

typedef union
{
    struct
    {
        unsigned int       Ps1_K4                          : 9;
        unsigned int       Ps1_K5                          : 9;
        unsigned int       Ps1_K6                          : 9;
        unsigned int       Reserved                        : 5;
    } reg;
    unsigned int uint;
}Reg_Mm81ac;

typedef union
{
    struct
    {
        unsigned int           Reserved                        :24;
        unsigned int           Ss2_Keying_Mode_Select          :4;
        // Secondary Stream 2 Keying Mode Select
        unsigned int           Invert_Alpha2_Or_Ka2            :1;
        // 0=Normal 0: normal (when alpha2/Ka2=0, the blender output is all contributed by PS2)  
        // 1=Invert 1: Invert the alpha2/Ka2 data (when alpha2/Ka2=0, the blender output is all contributed by SS2) 
        unsigned int           Ss2_Compressed_Mode_Enable      :1;
        unsigned int           Ss2_X_Rotation_Enable           :1;
        unsigned int           Ss2_Y_Rotation_Enable           :1;
    } reg;
    unsigned int uint;
} Reg_Mm818c;

static __inline__ void s3g_set_sr(unsigned char *mmio, unsigned char index, unsigned char data)
{
    unsigned int offset;

    offset = MMIO_OFFSET_SR_GROUP_A_ELT + index;
    s3g_write8(mmio + offset, data);
}

static __inline__ void s3g_set_cr(unsigned char *mmio, unsigned char index, unsigned char data)
{
    unsigned int offset;

    offset = MMIO_OFFSET_CR_GROUP_A_ELT + index;
    s3g_write8(mmio + offset, data);
}

static __inline__ unsigned char s3g_get_sr(unsigned char *mmio, unsigned char index)
{
    unsigned int offset;
    unsigned char temp;

    offset = MMIO_OFFSET_SR_GROUP_A_ELT + index;
    temp   = s3g_read8(mmio + offset);
	
    return temp;
}

static __inline__ unsigned char s3g_get_cr(unsigned char *mmio, unsigned char index)
{
    unsigned int offset;
    unsigned char temp;

    offset = MMIO_OFFSET_CR_GROUP_A_ELT + index;
    temp   = s3g_read8(mmio + offset);
	
    return temp;
}


static __inline__ void s3g_write_reg_no_mask(unsigned char *mmio, int type, unsigned char index, unsigned char value)
{
    unsigned int  offset = 0;

    switch(type)
    {
        case AR:
        {
            s3g_read8(mmio + MMIO_ATTR_INITIALIZE_REG_ELT);
            s3g_write8(mmio + MMIO_ATTR_ADDR_REG_ELT, index);     
            s3g_write8(mmio + MMIO_ATTR_DATA_WRITE_REG_ELT, value);
            s3g_write8(mmio + MMIO_ATTR_ADDR_REG_ELT, 0x20);
            break;
        }
        case GR:
        {
            s3g_write8(mmio + MMIO_GRAPH_ADDR_REG_ELT, index);
            s3g_write8(mmio + MMIO_GRAPH_DATA_REG_ELT, value);
            break;
        }
        case SR:
        {
            offset = MMIO_OFFSET_SR_GROUP_A_ELT + index;
            s3g_write8(mmio + offset, value);

            break;
        }
        case SR_BB:
        {
            offset = MMIO_OFFSET_SR_GROUP_B_ELT + index;
            s3g_write8(mmio + offset, value);

            break;
        }
        case CR:
        {
            offset = MMIO_OFFSET_CR_GROUP_A_ELT + index;
            s3g_write8(mmio + offset, value);

            break;
        }
        case CR_BB:
        {
            offset = MMIO_OFFSET_CR_GROUP_B_ELT + index;
            s3g_write8(mmio + offset, value);

            break;
        }
        case CR_CC:
        {
            offset = MMIO_OFFSET_CR_GROUP_C_ELT + index;
            s3g_write8(mmio + offset, value);

            break;
        }
        case CR_DD:
        {
            offset = MMIO_OFFSET_CR_GROUP_D_ELT + index;
            s3g_write8(mmio + offset, value);

            break;
        }

        default:
            break;     
        
    }
}

static __inline__ void s3g_write_reg(unsigned char *mmio, int type, unsigned char index, unsigned char value, unsigned char mask)
{
    unsigned int  offset = 0;
    unsigned char temp   = 0;

    switch(type)
    {
        case AR:
        {
            s3g_read8(mmio + MMIO_ATTR_INITIALIZE_REG_ELT);
            s3g_write8(mmio + MMIO_ATTR_ADDR_REG_ELT, index);     
            temp = s3g_read8(mmio + MMIO_ATTR_DATA_WRITE_REG_ELT); 
            temp   = (temp & mask) | value; 
            s3g_write8(mmio + MMIO_ATTR_DATA_WRITE_REG_ELT, temp);
            s3g_write8(mmio + MMIO_ATTR_ADDR_REG_ELT, 0x20);
            break;
        }
        case GR:
        {
            s3g_write8(mmio + MMIO_GRAPH_ADDR_REG_ELT, index);
            temp   = s3g_read8(mmio + MMIO_GRAPH_DATA_REG_ELT);
            temp   = (temp & mask) | value;
            s3g_write8(mmio + MMIO_GRAPH_DATA_REG_ELT, temp);
            break;
        }
        case SR:
        {
            offset = MMIO_OFFSET_SR_GROUP_A_ELT + index;
            temp   = s3g_read8(mmio + offset);
            temp   = (temp & mask) | value;
            s3g_write8(mmio + offset, temp);

            break;
        }
        case SR_BB:
        {
            offset = MMIO_OFFSET_SR_GROUP_B_ELT + index;
            temp   = s3g_read8(mmio + offset);
            temp   = (temp & mask) | value;
            s3g_write8(mmio + offset, temp);

            break;
        }
        case CR:
        {
            offset = MMIO_OFFSET_CR_GROUP_A_ELT + index;
            temp   = s3g_read8(mmio + offset);
            temp   = (temp & mask) | value;
            s3g_write8(mmio + offset, temp);

            break;
        }
        case CR_BB:
        {
            offset = MMIO_OFFSET_CR_GROUP_B_ELT + index;
            temp   = s3g_read8(mmio + offset);
            temp   = (temp & mask) | value;
            s3g_write8(mmio + offset, temp);

            break;
        }
        case CR_CC:
        {
            offset = MMIO_OFFSET_CR_GROUP_C_ELT + index;
            temp   = s3g_read8(mmio + offset);
            temp   = (temp & mask) | value;
            s3g_write8(mmio + offset, temp);

            break;
        }
        case CR_DD:
        {
            offset = MMIO_OFFSET_CR_GROUP_D_ELT + index;
            temp   = s3g_read8(mmio + offset);
            temp   = (temp & mask) | value;
            s3g_write8(mmio + offset, temp);

            break;
        }

        default:
            break;     
        
    }
}

static __inline__ unsigned char s3g_read_reg(unsigned char *mmio, int type, unsigned char index)
{
    unsigned int  offset = 0;
    unsigned char temp   = 0;

    switch(type)
    {
        case AR:
        {
            s3g_read8(mmio + MMIO_ATTR_INITIALIZE_REG_ELT);
            s3g_write8(mmio + MMIO_ATTR_ADDR_REG_ELT, index | 0x20);     
            temp = s3g_read8(mmio + MMIO_ATTR_DATA_WRITE_REG_ELT); 
            break;
        }
        case GR:
        {
            s3g_write8(mmio + MMIO_GRAPH_ADDR_REG_ELT, index);
            temp = s3g_read8(mmio + MMIO_GRAPH_DATA_REG_ELT);
            break;
        }
        case SR:
        {
            offset = MMIO_OFFSET_SR_GROUP_A_ELT + index;
            temp   = s3g_read8(mmio + offset);

            break;
        }
        case SR_BB:
        {
            offset = MMIO_OFFSET_SR_GROUP_B_ELT + index;
            temp   = s3g_read8(mmio + offset);

            break;
        }
        case CR:
        {
            offset = MMIO_OFFSET_CR_GROUP_A_ELT + index;
            temp   = s3g_read8(mmio + offset);

            break;
        }
        case CR_BB:
        {
            offset = MMIO_OFFSET_CR_GROUP_B_ELT + index;
            temp   = s3g_read8(mmio + offset);

            break;
        }
        case CR_CC:
        {
            offset = MMIO_OFFSET_CR_GROUP_C_ELT + index;
            temp   = s3g_read8(mmio + offset);

            break;
        }
        case CR_DD:
        {
            offset = MMIO_OFFSET_CR_GROUP_D_ELT + index;
            temp   = s3g_read8(mmio + offset);

            break;
        }

        default:
            break;     
        
    }

    return temp;
}



static __inline__ void s3g_unlock_extcr(unsigned char *mmio)
{
    s3g_set_sr(mmio, 0x38, 0x48);
    s3g_set_sr(mmio, 0x39, 0xA0);
} 

static __inline__ void s3g_unlock_extsr(unsigned char *mmio)
{
    s3g_set_sr(mmio, 0x08, 0x06);
}

static __inline__ void s3g_unlock_extregs(unsigned char *mmio)
{
    s3g_unlock_extcr(mmio);
    s3g_unlock_extsr(mmio);
}


/*
**********************************************************************************************
**   export interface
**********************************************************************************************
*/

extern void s3g_init_adapter(void);

extern int s3g_set_mode(adapter_t *adapter, s3g_modeset_t *mode_set);

#endif

