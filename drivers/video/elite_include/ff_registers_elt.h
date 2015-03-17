//    Spec Last Modified Time: 6/27/2011 7:21:26 PM
#ifndef _FF_REGISTERS_ELT_H
#define _FF_REGISTERS_ELT_H


#ifndef        FIXED_FUNCTION_BLOCKBASE_INF
    #define    FIXED_FUNCTION_BLOCKBASE_INF
    #define    BLOCK_FIXED_FUNCTION_VERSION 1
    #define    BLOCK_FIXED_FUNCTION_TIMESTAMP "6/27/2011 7:21:26 PM"
    #define    FIXED_FUNCTION_BLOCK                                       0x3 // match with BlockID.h
    #define    FIXED_FUNCTION_REG_START                                   0x0 // match with BlockID.h
    #define    FIXED_FUNCTION_REG_END                                     0x1E // match with BlockID.h
    #define    FIXED_FUNCTION_REG_LIMIT                                   0x1E // match with BlockID.h
#endif

// Register offset definition
#define        Reg_Zs_Req_Ctrl_Offset                                     0x0
#define        Reg_Ffc_Config_Offset                                      0x1
#define        Reg_Ff_Z_Clip_Offset                                       0x2
#define        Reg_Ff_Stencil_Control_Offset                              0x4
#define        Reg_Zs_Ctl_Offset                                          0x6
#define        Reg_Ff_Rt_Offset                                           0x7
#define        Reg_Zv_Fmt_Offset                                          0x13
#define        Reg_Zv_Desc_Base_Offset                                    0x14
#define        Reg_Sv_Desc_Base_Offset                                    0x15
#define        Reg_Dz_Shared_Ctl_Offset                                   0x16
#define        Reg_Bs_Blend_Ctl_Offset                                    0x17
#define        Reg_E2d_Desc_Offset                                        0x18
#define        Reg_Ff_Color_Offset                                        0x19

// Block constant definition
typedef enum
{
    ZS_REQ_CTRL_CHECK_BOARD_128X16           = 0, 
    ZS_REQ_CTRL_CHECK_BOARD_64X16            = 1, 
    ZS_REQ_CTRL_CHECK_BOARD_32X16            = 2, 
    ZS_REQ_CTRL_CHECK_BOARD_16X16            = 3, 
    ZS_REQ_CTRL_CHECK_BOARD_64X8             = 4, 
    ZS_REQ_CTRL_CHECK_BOARD_32X8             = 5, 
    ZS_REQ_CTRL_CHECK_BOARD_16X8             = 6, 
    ZS_REQ_CTRL_CHECK_BOARD_8X8              = 7, 
} ZS_REQ_CTRL_CHECK_BOARD;
typedef enum
{
    STENCIL_STATE_ST_OP_SFAIL_KEEP           = 0,   // 000: WGF_STENCILOP_KEEP = 1
    STENCIL_STATE_ST_OP_SFAIL_ZERO           = 1,   // 001: WGF_STENCILOP_ZERO  = 2
    STENCIL_STATE_ST_OP_SFAIL_REPLACE        = 2,   // 010: WGF_STENCILOP_REPLACE = 3
    STENCIL_STATE_ST_OP_SFAIL_INCRSAT        = 3,   // 011: WGF_STENCILOP_INCRSAT  = 4
    STENCIL_STATE_ST_OP_SFAIL_DECRSAT        = 4,   // 100: WGF_STENCILOP_DECRSAT  = 5
    STENCIL_STATE_ST_OP_SFAIL_INVERT         = 5,   // 101: WGF_STENCILOP_INVERT = 6
    STENCIL_STATE_ST_OP_SFAIL_INCRSAT1       = 6,   // 110: WGF_STENCILOP_INCR   = 7
    STENCIL_STATE_ST_OP_SFAIL_DECRSAT1       = 7,   // 111: WGF_STENCILOP_DECR  = 8
} STENCIL_STATE_ST_OP_SFAIL;
typedef enum
{
    ZS_CTL_Z_CMP_MODE_NEVER                  = 0,   // 000: WGF_COMPARISON_NEVER = 1
    ZS_CTL_Z_CMP_MODE_LESS                   = 1,   // 001: WGF_COMPARISON_LESS = 2
    ZS_CTL_Z_CMP_MODE_EQUAL                  = 2,   // 010: WGF_COMPARISON_EQUAL = 3
    ZS_CTL_Z_CMP_MODE_LESSEQUAL              = 3,   // 011: WGF_COMPARISON_LESSEQUAL = 4
    ZS_CTL_Z_CMP_MODE_GREATER                = 4,   // 100: WGF_COMPARISON_GREATER = 5
    ZS_CTL_Z_CMP_MODE_NOTEQUAL               = 5,   // 110: WGF_COMPARISON_NOTEQUAL = 6
    ZS_CTL_Z_CMP_MODE_GREATEREQUAL           = 6,   // 101: WGF_COMPARISON_GREATEREQUAL = 7
    ZS_CTL_Z_CMP_MODE_ALWAYS                 = 7,   // 111: WGF_COMPARISON_ALWAYS = 8
} ZS_CTL_Z_CMP_MODE;
typedef enum
{
    ZS_CTL_Z_UPDATE_EN_DISABLE               = 0,   // 0: Disable
    ZS_CTL_Z_UPDATE_EN_ENABLE                = 1,   // 1: Enable 
} ZS_CTL_Z_UPDATE_EN;
typedef enum
{
    ZS_CTL_ZS_STAGE_MODE_DISABLE             = 0,   // disable Z/S (likely for 2D)
    ZS_CTL_ZS_STAGE_MODE_ZL2_ONLY            = 1,   // ZL2 stage enabled for test and write 
    ZS_CTL_ZS_STAGE_MODE_Z23_BOTH            = 2,   // ZL2 test and ZL3 write
    ZS_CTL_ZS_STAGE_MODE_ZL3_ONLY            = 3,   // ZL3 stage enabled for test and write
} ZS_CTL_ZS_STAGE_MODE;
typedef enum
{
    ZS_CTL_S_EN_DISABLE                      = 0,   // 0: Disable
    ZS_CTL_S_EN_ENABLE                       = 1,   // 1: Enable 
} ZS_CTL_S_EN;
typedef enum
{
    ZS_CTL_A2C_EN_DISABLE                    = 0,   // 0: Disable
    ZS_CTL_A2C_EN_ENABLE                     = 1,   // 1: Enable
} ZS_CTL_A2C_EN;
typedef enum
{
    ZS_CTL_Z_CLIP_EN_DISABLE                 = 0,   // 0: Disable
    ZS_CTL_Z_CLIP_EN_ENABLE                  = 1,   // 1: Enable
} ZS_CTL_Z_CLIP_EN;
typedef enum
{
    ZS_CTL_Z_EN_DISABLE                      = 0,   // 0: Disable
    ZS_CTL_Z_EN_ENABLE                       = 1,   // 1: Enable 
} ZS_CTL_Z_EN;
typedef enum
{
    ZS_CTL_Z_BIAS_EN_DISABLE                 = 0,   // 0: Disable
    ZS_CTL_Z_BIAS_EN_ENABLE                  = 1,   // 1: Enable 
} ZS_CTL_Z_BIAS_EN;
typedef enum
{
    ZS_CTL_Z_16_OPTIMIZED_EN_DISABLE         = 0,   // 0: Disable
    ZS_CTL_Z_16_OPTIMIZED_EN_ENABLE          = 1,   // 1: Enable 
} ZS_CTL_Z_16_OPTIMIZED_EN;
typedef enum
{
    RT_MISC_SWIZZLE_A_NOP                    = 0, 
    RT_MISC_SWIZZLE_A_A_TO_LSB               = 1, 
} RT_MISC_SWIZZLE_A;
typedef enum
{
    RT_MISC_SWIZZLE_RB_NOP                   = 0, 
    RT_MISC_SWIZZLE_RB_R_B_SWAP              = 1, 
} RT_MISC_SWIZZLE_RB;
typedef enum
{
    RT_MISC_LUMINENCE_NOP                    = 0, 
    RT_MISC_LUMINENCE_IS_L                   = 1,   // R is luminence. G is Alpha.
} RT_MISC_LUMINENCE;
typedef enum
{
    RT_MISC_DITHER_EN_DISABLE                = 0,   // 0: Disabled.
    RT_MISC_DITHER_EN_ENABLE                 = 1,   // 1: Enable
} RT_MISC_DITHER_EN;
typedef enum
{
    RT_MISC_IS_GAMMA_NON_GAMMA               = 0,   // linear 
    RT_MISC_IS_GAMMA_GAMMA                   = 1,   // gamma
} RT_MISC_IS_GAMMA;
typedef enum
{
    RT_MISC_IS_PREMULTIPLY_NON_PRE           = 0,   // non premultiplied
    RT_MISC_IS_PREMULTIPLY_PRE               = 1,   // premultiplied with alpha channel
} RT_MISC_IS_PREMULTIPLY;
typedef enum
{
    RT_MISC_IS_SRC_GAMMA_NON_GAMMA           = 0,   // linear 
    RT_MISC_IS_SRC_GAMMA_GAMMA               = 1,   // gamma
} RT_MISC_IS_SRC_GAMMA;
typedef enum
{
    RT_MISC_IS_SRC_PREMULTIPLY_NON_PRE       = 0,   // non premultiplied
    RT_MISC_IS_SRC_PREMULTIPLY_PRE           = 1,   // premultiplied with alpha channel
} RT_MISC_IS_SRC_PREMULTIPLY;
typedef enum
{
    ZV_FMT_FORMAT_Z16                        = 0, 
    ZV_FMT_FORMAT_Z24                        = 1, 
    ZV_FMT_FORMAT_Z24_BANK_SWIZZLE           = 3, 
} ZV_FMT_FORMAT;
typedef enum
{
    DZ_SHARED_CTL_STG_COMPUTE_COVERAGE_EN_NORMAL= 0, 
    DZ_SHARED_CTL_STG_COMPUTE_COVERAGE_EN_AA= 1,    // the meaning is now changed to compute coverage in STG. 
} DZ_SHARED_CTL_STG_COMPUTE_COVERAGE_EN;
typedef enum
{
    DZ_SHARED_CTL_MSAA_MODE_NORMAL_PATTERN   = 0,   // Samples are in different locations inside pixel.
    DZ_SHARED_CTL_MSAA_MODE_CENTER_PATTERN   = 1,   // All samples are in the cener. This is a special mode in fact.
} DZ_SHARED_CTL_MSAA_MODE;
typedef enum
{
    DZ_SHARED_CTL_EU_KILL_PIXEL_DISABLE      = 0, 
    DZ_SHARED_CTL_EU_KILL_PIXEL_ENABLE       = 1, 
} DZ_SHARED_CTL_EU_KILL_PIXEL;
typedef enum
{
    DZ_SHARED_CTL_EU_OUTPUT_2_DATA_DISABLE   = 0, 
    DZ_SHARED_CTL_EU_OUTPUT_2_DATA_ENABLE    = 1, 
} DZ_SHARED_CTL_EU_OUTPUT_2_DATA;
typedef enum
{
    DZ_SHARED_CTL_D_ALLOCATION_MODULE_ZL3    = 0,   // ZL3 do the allocation
    DZ_SHARED_CTL_D_ALLOCATION_MODULE_ZL2    = 1,   // ZL2 do the allocation
} DZ_SHARED_CTL_D_ALLOCATION_MODULE;
typedef enum
{
    BS_BLEND_CTL_RGB_SBLEND_ZERO             = 0,   // 0x0: WGF_BLEND_ZERO = 1 (0, 0, 0, 0)
    BS_BLEND_CTL_RGB_SBLEND_ONE              = 1,   // 0x1: WGF_BLEND_ONE  = 2 (1, 1, 1, 1)
    BS_BLEND_CTL_RGB_SBLEND_SRCCOLOR         = 2,   // 0x2: WGF_BLEND_SRCCOLOR  = 3  (oN.r, oN.g, oN.b, oN.a)
    BS_BLEND_CTL_RGB_SBLEND_INVSRCCOLOR      = 3,   // 0x3: WGF_BLEND_INVSRCCOLOR  = 4  (1- oN.r, 1- oN.g, 1- oN.b,
                                                    // 1- oN.a)
    BS_BLEND_CTL_RGB_SBLEND_SRCALPHA         = 4,   // 0x4: WGF_BLEND_SRCALPHA  = 5 (oN.a, oN.a, oN.a,  oN.a)
    BS_BLEND_CTL_RGB_SBLEND_INVSRCALPHA      = 5,   // 0x5: WGF_BLEND_INVSRCALPHA = 6  (1- oN.a, 1- oN.a, 1- oN.a,
                                                    // 1- oN.a)
    BS_BLEND_CTL_RGB_SBLEND_DESTALPHA        = 6,   // 0x6: WGF_BLEND_DESTALPHA  = 7  (D.a, D.a , D.a , D.a)
    BS_BLEND_CTL_RGB_SBLEND_INVDESTALPHA     = 7,   // 0x7: WGF_BLEND_INVDESTALPHA  = 8 (1- D.a, 1-D.a, 1-D.a,
                                                    // 1-D.a )
    BS_BLEND_CTL_RGB_SBLEND_DESTCOLOR        = 8,   // 0x8: WGF_BLEND_DESTCOLOR = 9  (D.r, D.g, D.b, D.a )
    BS_BLEND_CTL_RGB_SBLEND_INVDESTCOLOR     = 9,   // 0x9: WGF_BLEND_INVDESTCOLOR = 10  (1-D.r, 1-D.g, 1-D.b,
                                                    // 1- D.a )
    BS_BLEND_CTL_RGB_SBLEND_SRCALPHASAT      = 10,  // 0xA: WGF_BLEND_SRCALPHASAT  = 11 (f,f,f,1) f = min( oN.a,
                                                    // 1-D.a )
    BS_BLEND_CTL_RGB_SBLEND_CONSTANTCOLOR    = 11,  // 0x13: (BF.RGB, BF.RGB, BF.A) for OpenGL. No
                                                    // corresponding MS value. 
    BS_BLEND_CTL_RGB_SBLEND_INVCONSTANTCOLOR= 12,   // 0x14: (1-BF.RGB, 1- BF.RGB, 1-BF.RGB, 1- BF.A) for
                                                    // OpenGL. No corresponding MS value.
    BS_BLEND_CTL_RGB_SBLEND_CONSTANTALPHA    = 13,  // 0x13: (BF.a, BF.a, BF.a, BF.a) for OpenGL. No
                                                    // corresponding MS value. 
    BS_BLEND_CTL_RGB_SBLEND_INVCONSTANTALPHA= 14,   // 0x14: (1-BF.a, 1- BF.a, 1-BF.a, 1- BF.a) for OpenGL. No
                                                    // corresponding MS value.
    BS_BLEND_CTL_RGB_SBLEND_SRCCONSTANTALPHA= 15,   // 0x15: (1-oN.a*BF.a, 1-oN.a*BF.a, 1-oN.a*BF.a,
                                                    // 1-oN.a*BF.a) for TextOut No corresponding MS value.
} BS_BLEND_CTL_RGB_SBLEND;
typedef enum
{
    BS_BLEND_CTL_RGB_BLEND_OP_ADD            = 0,   // 000: WGF_BLENDOP_ADD = 1
    BS_BLEND_CTL_RGB_BLEND_OP_SUBTRACT       = 1,   // 001: WGF_BLENDOP_SUBTRACT = 2
    BS_BLEND_CTL_RGB_BLEND_OP_REVSUBTRACT    = 2,   // 010: WGF_BLENDOP_REVSUBTRACT  = 3
    BS_BLEND_CTL_RGB_BLEND_OP_MAX_OP         = 3,   // 011:MAX_OP of source and destination.
} BS_BLEND_CTL_RGB_BLEND_OP;
typedef enum
{
    BS_BLEND_CTL_RGB_BLEND_EN_DISABLE        = 0,   // 0: Disable
    BS_BLEND_CTL_RGB_BLEND_EN_ENABLE         = 1,   // 1: Alpha Blend for color channels enable
} BS_BLEND_CTL_RGB_BLEND_EN;
typedef enum
{
    BS_BLEND_CTL_OVG_RGB_EXTRA_NORMAL        = 0,   // 0: Use RGB blend mode as normal case
    BS_BLEND_CTL_OVG_RGB_EXTRA_OVG_MULTIPLY  = 1,   // 1:OVG MULTIPLY mode
    BS_BLEND_CTL_OVG_RGB_EXTRA_OVG_DARKEN    = 2,   // 3:OVG DARKEN mode
    BS_BLEND_CTL_OVG_RGB_EXTRA_OVG_LIGHTEN   = 3,   // 4:OVG DARKEN mode
    BS_BLEND_CTL_OVG_RGB_EXTRA_SKIA_ALPHA    = 4,   // 6. Skia alpha mask blend
} BS_BLEND_CTL_OVG_RGB_EXTRA;
typedef enum
{
    BS_BLEND_CTL_OVG_DUAL_SOURCE_SRC0        = 0,   // 0: RGB_DBLEND source color uses src0.rgb only
    BS_BLEND_CTL_OVG_DUAL_SOURCE_SRC1        = 1,   // 1: RGB_DBLEND source color uses src1.rgb
} BS_BLEND_CTL_OVG_DUAL_SOURCE;
typedef enum
{
    BS_BLEND_CTL_PRE_MULTIPLY_EN_DISABLE     = 0,   // 0: no premultiplied with alpha when blending
    BS_BLEND_CTL_PRE_MULTIPLY_EN_ENABLE      = 1,   // 1: premultiplied with alpha when blending
} BS_BLEND_CTL_PRE_MULTIPLY_EN;
typedef enum
{
    BS_BLEND_CTL_GAMMA_BLEND_EN_DISABLE      = 0,   // 0: Disabled.
    BS_BLEND_CTL_GAMMA_BLEND_EN_ENABLE       = 1,   // 1: Enable
} BS_BLEND_CTL_GAMMA_BLEND_EN;
typedef enum
{
    BS_BLEND_CTL_ALPHA_BLEND_EN_DISABLE      = 0,   // 0: Disable
    BS_BLEND_CTL_ALPHA_BLEND_EN_ENABLE       = 1,   // 1: Alpha Blend for Alpha Channel enable
} BS_BLEND_CTL_ALPHA_BLEND_EN;
typedef enum
{
    E2D_DESC_SRC_BITS_MODE_NORMAL            = 0,   // this is normal mode: h/w will look at source surface format
                                                    // to determine source data
    E2D_DESC_SRC_BITS_MODE_IS_1BPP           = 1,   // this mode tells h/w that 1BPP source is enabled, so h/w will
                                                    // lookup fg_color or bg_color using each source bit. Source
                                                    // format is ignored.
    E2D_DESC_SRC_BITS_MODE_IS_4BPP           = 2,   // this mode tells h/w that 4 bit alpha from 2D source surface 
                                                    // is enabled (Source format is ignored), so h/w will upscale
                                                    // it to alpha8=alpha4<<4|alpha4, and use it to blend:
                                                    // (fg_color*alpha8+bg_color*(~alpha8)+128)>>8. 
    E2D_DESC_SRC_BITS_MODE_UNUSED            = 3,   // reserved
} E2D_DESC_SRC_BITS_MODE;
typedef enum
{
    E2D_DESC_SPECIAL_AA_EN_NO                = 0,   // regular AA
    E2D_DESC_SPECIAL_AA_EN_YES               = 1,   // multi-step AA. If destination alpha is not used by app and
                                                    // APP requires better quality, we will store
                                                    // coverage_new=coverage_old-current_coverage into
                                                    // alpha. And blend coverage with formula: Alpha_new=
                                                    // Alpha_old-A, 
                                                    //  R_new=(F*A+R_old*Alpha_new)/Alpha_old  (R=buffer
                                                    // color, F=curr color)
                                                    //  (when alpha_new<=0, set to 0xff, and set R_new=F)
} E2D_DESC_SPECIAL_AA_EN;
typedef enum
{
    E2D_DESC_COV_FROM_EU_EN_NO               = 0,   // from STG if AA enabled or Alpha mask enabled
    E2D_DESC_COV_FROM_EU_EN_YES              = 1,   // from EU shader source alpha. If dual source enabled, it is
                                                    // from second source alpha
} E2D_DESC_COV_FROM_EU_EN;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////            FF Block (Block ID = 3) Register Definitions                              ///////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
typedef union
{
    struct
    {
        unsigned int Z_Read_En                 : 1;   // need to read Z from memory 
        unsigned int S_Read_En                 : 1;   // need to read S from memory
        unsigned int Z_Alloc_En                : 1;   // need to allocate Z for either read or write or both
        unsigned int S_Alloc_En                : 1;   // need to allocate Stencil for either read or write or both
        unsigned int A_Alloc_En                : 1;   // need to allocate and read Alpha mask from DS buffer. Read
                                                      // only
        unsigned int Z_Is_Read_Only            : 1;   // 1=no write to buffer (help FFC to make decision)
        unsigned int S_Is_Read_Only            : 1;   // 1=no write to buffer (help FFC to make decision)
        unsigned int Bottom_Inclusive          : 1;   // Whether it is bottom edge will be included or the top edge
                                                      // will be included. Or bottom-left or top-left
                                                      // rasterization rule.
        unsigned int Ab_Attr_Num               : 4;   // Number of total attributes, 1~11
        unsigned int Z_Resolved_Read_En        : 1;   // used by h/w to determine if Z resolve read is needed. used by
                                                      // h/w to determine if Z resolve read is needed. Cannot be on
                                                      // when z_read_en is on. When both off, it may mean
                                                      // write-only. 
        unsigned int S_Resolved_Read_En        : 1;   // used by h/w to determine if S resolve read is needed  Can be on
                                                      // when s_read_en is also on. When both off, it may mean
                                                      // write-only. 
        unsigned int Vertical_Split_Threshold : 2;    // When it is 0, disable the whole split behavior. Or can
                                                      // easily think it as vertical split condition is false.
                                                      //  When it is 1, vertical split condition satisfies if
                                                      // bounding box vertical distance >= 16.
                                                      //  When it is 2, vertical split condition satisfies if
                                                      // bounding box vertical distance >= 32.
                                                      //  When it is 3, vertical split condition satisfies if
                                                      // bounding box vertical distance >= 64.
                                                      //  Only when vertical split condition and horizontal split
                                                      // condition both satisfies, the split will happen.
        unsigned int Horizontal_Split_Threshold : 2;  // When it is 0, horizontal split condition satisfies if
                                                      // bounding box horizontal distance >= 16.
                                                      //  When it is 1, horizontal split condition satisfies if
                                                      // bounding box horizontal distance >= 32.
                                                      //  When it is 2, horizontal split condition satisfies if
                                                      // bounding box horizontal distance >= 64.
                                                      //  When it is 3, horizontal split condition satisfies if
                                                      // bounding box horizontal distance >= 128.
        unsigned int Dx9                       : 1;   // When API is dx9, set this bit to 1. Else, set it to 0. This
                                                      // register bit is just used to bypass to TAS to save its drain
                                                      // pipeline cost when 3d blit in dx9 path.
        unsigned int Reserved                  : 1; 
        unsigned int Check_Board               : 3;   // To driver, when in linear case, better set as 128x16.
                                                      //  When in tile case, better set as 16x16.
        unsigned int Slice1_Valid              : 1;   // Whether slice 1 is valid or not.
        unsigned int Fc_St_Wr_Mask             : 8;   // Stencil write mask when it is fast clear
    } reg;
    unsigned int uint;
} Reg_Zs_Req_Ctrl;
 
typedef union
{
    struct
    {
        unsigned int S_Start                   : 6;   // starting cache line location for stencil
        unsigned int Reserved1                 : 2;   // Reserved
        unsigned int D_Start                   : 6;   // starting cache line location for D. 
        unsigned int Reserved2                 : 2;   // Reserved
        unsigned int Reserve_Cache_Line        : 1;   // Purposely reserve 4 cahche lines for scaling or other
                                                      // usage.
                                                      //  Before setting this bit, driver will do the flush itself.
                                                      //  And when this bit is reset, no need to flush the data into
                                                      // memory at all.
        unsigned int Reserved3                 : 15; 
    } reg;
    unsigned int uint;
} Reg_Ffc_Config;
 
typedef union
{
    struct
    {
        unsigned int Data                      : 24;  // Default value of 0 is 0x000000, 1 is 0xffffff.
        unsigned int Reserved                  : 8;   // Reserved
    } reg;
    unsigned int uint;
} Reg_Z_Clip_Value;
 
typedef struct _Group_Ff_Z_Clip
{
    Reg_Z_Clip_Value                 reg_Z_Clip_Value;
} Reg_Ff_Z_Clip_Group;
 
typedef union
{
    struct
    {
        unsigned int St_Cmp_Mode               : 3;   // Stencil compare mode, 
        unsigned int Reserved1                 : 1;   // Reserved
        unsigned int St_Op_Sfail               : 3;   // Stencil update mode when stencil-fail (no matter z pass or
                                                      // fail)
        unsigned int Reserved2                 : 1;   // Reserved
        unsigned int St_Op_Zfail               : 3;   // Stencil update mode when stencil pass but Z fail. Values
                                                      // are same as ST_OP_SFAIL
        unsigned int Reserved3                 : 1;   // Reserved
        unsigned int St_Op_Pass                : 3;   // Stencil update mode when both stencil and z pass, values
                                                      // are same as ST_OP_SFAIL
        unsigned int Reserved4                 : 1;   // Reserved
        unsigned int St_Cmp_Mask               : 8;   // Stencil compare mask
        unsigned int St_Wr_Mask                : 8;   // Stencil write mask
    } reg;
    unsigned int uint;
} Reg_Stencil_State;
 
typedef struct _Group_Ff_Stencil_Control
{
    Reg_Stencil_State                reg_Stencil_State;
} Reg_Ff_Stencil_Control_Group;
 
typedef union
{
    struct
    {
        unsigned int Z_Cmp_Mode                : 3;   // Compare Modes
        unsigned int Z_Update_En               : 1;   // Z write is on or not
        unsigned int Zs_Stage_Mode             : 2;   // For Z and/or S stage control. h/w will judge
                                                      // test/write/interpolate details by other bits. 
        unsigned int S_En                      : 1;   // Stencil enabled or not
        unsigned int Zl2_End_Pipe_En           : 1;   // ZL2 may end pipe
        unsigned int A2c_En                    : 1;   // Enables Alpha-to-coverage
        unsigned int A_Mask_En                 : 1;   // alpha mask is enabled. If 1, alpha buffer must be valid. 
        unsigned int Z_Clip_En                 : 1;   // Z clip enable
        unsigned int Z_En                      : 1;   // Z enabled or not
        unsigned int Z_Bias_En                 : 1;   // Z enabled or not
        unsigned int Z_16_Optimized_En         : 1;   // optimize Z16 in the way that h/w detects first 8 MS bits of
                                                      // interpolated Z to see if it is all 1's or all 0's. if so, it
                                                      // will replace the 8 bits with b11 or b00, and "rescue" more LS
                                                      // bits. Otherwise, put b10 or b01 in MS bits with the rest.
                                                      // This will improve some app's Z16 precision when Z range is
                                                      // bias to Z near or Z far. 
        unsigned int Z_Occlusion_Counting_Disable : 1;// For performance counter
        unsigned int Stencil_Test_Enabled_In_Zl2_When_Both_On : 1;
                                                      // Whether stencil test will be enabled in ZL2 when it is both
                                                      // on mode.
        unsigned int St_Ref_Front              : 8;   // Stencil reference value for front facing triangles
        unsigned int St_Ref_Back               : 8;   // Stencil reference value for back facing triangles
    } reg;
    unsigned int uint;
} Reg_Zs_Ctl;
 
typedef union
{
    struct
    {
        unsigned int Width                     : 12;  // Width of surface in pixels.  
        unsigned int Height                    : 12;  // Height of surface in pixels. 
        unsigned int Is_Tiling                 : 1;   // for linear<->tiling conversion
        unsigned int Bank_Swizzle              : 1;   // Whether BANK swizzle will happen or not, only affects FFC.
                                                      //  Mainly for 32BPE MSAA case.
                                                      //  To FFC, it will just according to this bit, instead of using
                                                      // MSAA/Format and this bit to do the decision.
        unsigned int Rt_Enable                 : 1;   // Whether the RT is enabled or not.
                                                      //  OGL3.1 DrawBuffer()
        unsigned int Reserved1                 : 1; 
        unsigned int Format                    : 4;   // RT format. See Surface_format.xls for definition. 
    } reg;
    unsigned int uint;
} Reg_Rt_Fmt;
 
typedef union
{
    struct
    {
        unsigned int Base_Addr                 : 24;  // Base address 
        unsigned int Reserved                  : 8;   // Reserved
    } reg;
    unsigned int uint;
} Reg_Rt_Desc_Base;
 
typedef union
{
    struct
    {
        unsigned int Rt_Write_Mask             : 4; 
        unsigned int Swizzle_A                 : 1;   // only do it for 8 bit or under Alpha channel format
        unsigned int Swizzle_Rb                : 1;   // only do it for 8 bit or under channel format
        unsigned int Luminence                 : 1; 
        unsigned int Dither_En                 : 1;   // dither enable
        unsigned int Is_Gamma                  : 1; 
        unsigned int Is_Premultiply            : 1; 
        unsigned int Resolve_Read_En           : 1;   // used to determine if resolve read needed
        unsigned int Resolve_Write_En          : 1;   // used to determine if resolve write needed
        unsigned int Honor_Alpha               : 1;   // for color key, if this bit is set to 1, color compare
                                                      // includes alpha compare.      We will use this bit to indicate
                                                      // RT0's alpha is don't or not in normal case (e.g. XRGB or
                                                      // ARGB). 
        unsigned int Is_Src_Gamma              : 1; 
        unsigned int Is_Src_Premultiply        : 1; 
        unsigned int D_Read_En                 : 1;   // need to read D from memory
                                                      //  even if it is write only case, the channel is not full and the
                                                      // missing channel can not occupy full bytes, we will still
                                                      // enable this bit for the RT.
        unsigned int Blend_Enable              : 1;   // When the RT is enabled, whether it is blendable or not.
                                                      //  OGL3.1 Enablei(blend, i)
        unsigned int Reserved                  : 15; 
    } reg;
    unsigned int uint;
} Reg_Rt_Misc;
 
typedef struct _Group_Ff_Rt
{
    Reg_Rt_Fmt                       reg_Rt_Fmt;
    Reg_Rt_Desc_Base                 reg_Rt_Desc_Base;
    Reg_Rt_Misc                      reg_Rt_Misc;
} Reg_Ff_Rt_Group;
 
typedef union
{
    struct
    {
        unsigned int Width                     : 12;  // Width of surface in pixels.  
        unsigned int Height                    : 12;  // Height of surface in pixels. 
        unsigned int Reserved                  : 4;   // Reserved
        unsigned int Format                    : 4;   // Z format
    } reg;
    unsigned int uint;
} Reg_Zv_Fmt;
 
typedef union
{
    struct
    {
        unsigned int Base_Addr                 : 24;  // Base address 
        unsigned int Reserved                  : 8;   // Reserved
    } reg;
    unsigned int uint;
} Reg_Zv_Desc_Base;
 
typedef union
{
    struct
    {
        unsigned int Base_Addr                 : 24;  // Base address 
        unsigned int Reserved                  : 8;   // Reserved
    } reg;
    unsigned int uint;
} Reg_Sv_Desc_Base;
 
typedef union
{
    struct
    {
        unsigned int Stg_Compute_Coverage_En   : 1;   // Rasterizer computes coverage 
        unsigned int Reserved2                 : 1;   // reserved. No used any more. 
        unsigned int D_Alloc_En                : 1;   // need to allocate D from memory read or write or both
        unsigned int Src_Read_Alloc_En         : 1;   // need to read/allocate 2D source cacheline. 
        unsigned int Msaa_Mask                 : 4;   // 4 bit msaa mask
        unsigned int Msaa_En                   : 1;   // 4X MSAA
        unsigned int Msaa_Mode                 : 1; 
        unsigned int Eu_Kill_Pixel             : 1;   // Whether EU will kill the pixel or not. If it is enabled, then
                                                      // ZL3 will do D allocation; otherwise, TGZ do the job. To HW,
                                                      // this bit also have other usage.
        unsigned int Eu_Output_Depth_En        : 1;   // DX9 odepth 
        unsigned int Eu_Output_2_Data          : 1;   // Whether EU will output 2 data entries to ZL3. ZL3 will use to
                                                      // ease decoding the condition, and driver can also easily
                                                      // set this register according to corresponding one in EU PS.
        unsigned int Reserved1                 : 2;   // reserved. No used any more. 
        unsigned int D_Allocation_Module       : 1;   // Whether it is ZL2 do the D allocation or ZL3 do the job.
                                                      //  When ZL3 is disabled, only ZL2 can do the job.
        unsigned int Reserved0                 : 1;   // Enable 4x1 rendering when RT is linear format. 
        unsigned int Is_8p_Mode                : 1;   // Whether it is in 8p mode or not.
        unsigned int Reserved                  : 14;  // Reserved
    } reg;
    unsigned int uint;
} Reg_Dz_Shared_Ctl;
 
typedef union
{
    struct
    {
        unsigned int Rgb_Sblend                : 4;   // Selects blend factor for source for RGB channels
        unsigned int Rgb_Dblend                : 4;   // Selects blend factor for destination for RGB channels,
                                                      // definition is the same as RGB_SBLEND
        unsigned int Rgb_Blend_Op              : 2;   // Blend Operation for RGB channels Select
        unsigned int Alpha_Blend_Op            : 2;   // Selects blending operation for alpha channel, values are
                                                      // same as RGB_BLEND_OP
        unsigned int Alpha_Sblend              : 4;   // Selects blend factor for source for alpha channel, values
                                                      // are same as RGB_SBLEND
        unsigned int Alpha_Dblend              : 4;   // Selects blend factor for destination for alpha channel,
                                                      // values are same as RGB_SBLEND
        unsigned int Reserved                  : 4;   // Component write mask
        unsigned int Rgb_Blend_En              : 1;   // Enables alpha-blending for color channels
        unsigned int Ovg_Rgb_Extra             : 3;   // Some more RGB blend modes for OpenVG, SKIA
        unsigned int Ovg_Dual_Source           : 1;   // OpenVG: RGB_DBELND source color special
        unsigned int Pre_Multiply_En           : 1; 
        unsigned int Gamma_Blend_En            : 1; 
        unsigned int Alpha_Blend_En            : 1;   // Enables alpha-blending for alpha channel
    } reg;
    unsigned int uint;
} Reg_Bs_Blend_Ctl;
 
typedef union
{
    struct
    {
        unsigned int Rop                       : 5;   // ROP2/OGL Logic op operation: include all 2-operand ROP3,
                                                      // except for op between s and p. Here, the p stands for either s
                                                      // or p. Examples 
                                                      //  P&D        =(b¡¯01000)
                                                      //  P&(~D)      =(b¡¯01001)
                                                      //  P           =(b¡¯00000)  //source copy
                                                      //  D&(~P)      =(b¡¯01010)
                                                      //  P^D         =(b¡¯01100)
                                                      //  P|D         =(b¡¯00100)
                                                      //  ~(P|D)       =(b¡¯10100)
                                                      //  ~(P^D)       =(b¡¯11100)
                                                      //  ~D          =(b¡¯00001)  //special
                                                      //  P|(~D)       =(b¡¯00101)
                                                      //  ~P          =(b¡¯00010)
                                                      //  D|(~P)       =(b¡¯00110)
                                                      //  ~P&D        =(b¡¯11000)
        unsigned int Color_Key_En              : 1;   // enable color key for 2D command (transparent blit), using
                                                      // BG_COLOR as key. 
        unsigned int L2l_Blit_Optimization_En : 1;    // Whether enable linear to linear 2d blit optimization or
                                                      // not.
        unsigned int Reserved0                 : 5; 
        unsigned int Coverage_As_Color_En      : 1;   // Computed coverage is used as source0 alpha. This is
                                                      // special for some app which will store coverage into RT
                                                      // buffer. 
        unsigned int Bypass_Ps_En              : 1;   // this bit tells h/w that PS will not be on, so Color data is not
                                                      // from PS. This will be set to 1 by driver for the cases:
                                                      // COVERAGE_AS_COLOR_EN=1, DP_mode_lineDraw, or any
                                                      // future use. 
        unsigned int Src_Bits_Mode             : 2;   // for 2D copy functions (clear/image transfer/blit)
        unsigned int Aa_Coverage_Blend_En      : 1;   // to explicitly let FLAU to blend coverage value. The
                                                      // coverage value can be from EU or STG generated. 
        unsigned int Reserved1                 : 6;   // Reserved
        unsigned int Special_Aa_En             : 1;   // whether we will have special AA mode
        unsigned int Cov_From_Eu_En            : 1;   // whether AA coverage data is from EU
        unsigned int Reserved2                 : 7; 
    } reg;
    unsigned int uint;
} Reg_E2d_Desc;
 
typedef union
{
    struct
    {
        unsigned int C_R                       : 8;   // for fp16 blending, RG combined as R or B
        unsigned int C_G                       : 8; 
        unsigned int C_B                       : 8;   // for fp16 blending, BA combined as G or A
        unsigned int C_A                       : 8; 
    } reg;
    unsigned int uint;
} Reg_Color;
 
typedef struct _Group_Ff_Color
{
    Reg_Color                        reg_Color;
} Reg_Ff_Color_Group;
 
typedef struct _Ff_regs
{
    Reg_Zs_Req_Ctrl                  reg_Zs_Req_Ctrl;
    Reg_Ffc_Config                   reg_Ffc_Config;
    Reg_Ff_Z_Clip_Group              reg_Ff_Z_Clip[2];
    Reg_Ff_Stencil_Control_Group     reg_Ff_Stencil_Control[2];
    Reg_Zs_Ctl                       reg_Zs_Ctl;
    Reg_Ff_Rt_Group                  reg_Ff_Rt[4];
    Reg_Zv_Fmt                       reg_Zv_Fmt;
    Reg_Zv_Desc_Base                 reg_Zv_Desc_Base;
    Reg_Sv_Desc_Base                 reg_Sv_Desc_Base;
    Reg_Dz_Shared_Ctl                reg_Dz_Shared_Ctl;
    Reg_Bs_Blend_Ctl                 reg_Bs_Blend_Ctl;
    Reg_E2d_Desc                     reg_E2d_Desc;
    Reg_Ff_Color_Group               reg_Ff_Color[5];
} Ff_regs;

#endif
