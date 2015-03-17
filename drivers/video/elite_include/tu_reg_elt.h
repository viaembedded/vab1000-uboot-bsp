//    Spec Last Modified Time: 2011/6/13 10:46:03
#ifndef _TU_REG_ELT_H
#define _TU_REG_ELT_H


#ifndef        TU_BLOCKBASE_INF
#define    TU_BLOCKBASE_INF
#define    BLOCK_TU_VERSION 1
#define    BLOCK_TU_TIMESTAMP "2011/6/13 10:46:03"
#define    TU_BLOCK                                                   0x6 // match with BlockID.h
#define    TU_REG_START                                               0x0 // match with BlockID.h
#define    TU_REG_END                                                 0xF4 // match with BlockID.h
#define    TU_REG_LIMIT                                               0xF4 // match with BlockID.h
#endif

// Register offset definition
#define        Reg_T_Sampler_Vs_Offset                                    0x0
#define        Reg_T_Sampler_Ps_Offset                                    0x40
#define        Reg_T_Vb_Offset                                            0x80
#define        Reg_T_Ve_Offset                                            0xB0
#define        Reg_Tu_Si_Ctl_Offset                                       0xC0
#define        Reg_Tu_Control_Offset                                      0xC1
#define        Reg_Tu_Sadcomp_Cur_Offset                                  0xC2
#define        Reg_Tu_Sadcomp_Other_Offset                                0xC3
#define        Reg_T_Instance_Offset                                      0xC4
#define        Reg_T_Bc_Vs_Offset                                         0xD4
#define        Reg_T_Bc_Ps_Offset                                         0xE4

// Block constant definition
typedef enum
{
    TU_SAMP1_PS_MSAA_MODE_1X                 = 0,   // 00: 1X Normal
    TU_SAMP1_PS_MSAA_MODE_4X                 = 1,   // 01: 4X MSAA
} TU_SAMP1_PS_MSAA_MODE;
typedef enum
{
    TU_SAMP1_PS_LINEAR_TEXTURE_FLAG_TILED    = 0,   // 00: Tiled texture
    TU_SAMP1_PS_LINEAR_TEXTURE_FLAG_LINEAR   = 1,   // 01: Linear texture
} TU_SAMP1_PS_LINEAR_TEXTURE_FLAG;
typedef enum
{
    TU_SAMP1_PS_RESOURCE_TYPE_BUFFER         = 0, 
    TU_SAMP1_PS_RESOURCE_TYPE_1D_TEXTURE     = 1, 
    TU_SAMP1_PS_RESOURCE_TYPE_2D_TEXTURE     = 2, 
    TU_SAMP1_PS_RESOURCE_TYPE_3D_TEXTURE     = 3, 
    TU_SAMP1_PS_RESOURCE_TYPE_2DARRAY_TEXTURE= 4, 
    TU_SAMP1_PS_RESOURCE_TYPE_CUBIC_TEXTURE  = 5, 
    TU_SAMP1_PS_RESOURCE_TYPE_STRUCT_BUFFER  = 6, 
    TU_SAMP1_PS_RESOURCE_TYPE_RAW_BUFFER     = 7, 
} TU_SAMP1_PS_RESOURCE_TYPE;
typedef enum
{
    TU_SAMP2_PS_COMPARE_EN_DISABLE           = 0,   // 0: Not enabled.
    TU_SAMP2_PS_COMPARE_EN_ENABLE            = 1,   // 1: Enable Comparision function for any filter mode that
    // has compare_ as pre-fix. 
} TU_SAMP2_PS_COMPARE_EN;
typedef enum
{
    TU_SAMP2_PS_CMP_FUNC_NEVER               = 0,   // 000: NEVER 
    TU_SAMP2_PS_CMP_FUNC_LESS                = 1,   // 001: LESS
    TU_SAMP2_PS_CMP_FUNC_EQUAL               = 2,   // 010: EQUAL
    TU_SAMP2_PS_CMP_FUNC_LESSEQUAL           = 3,   // 011: LESSEQUAL
    TU_SAMP2_PS_CMP_FUNC_GREATER             = 4,   // 100: GREATER
    TU_SAMP2_PS_CMP_FUNC_NOTEQUAL            = 5,   // 101: NOTEQUAL
    TU_SAMP2_PS_CMP_FUNC_GREATEREQUAL        = 6,   // 110: GREATEREQUAL
    TU_SAMP2_PS_CMP_FUNC_ALWAYS              = 7,   // 111: ALWAYS
} TU_SAMP2_PS_CMP_FUNC;
typedef enum
{
    TU_SAMP2_PS_MODE_OGL                     = 0,   // 00: OGL mode
    TU_SAMP2_PS_MODE_DX9                     = 1,   // 01: DX9 mode
    TU_SAMP2_PS_MODE_OCL                     = 2,   // 10: OCL mode
    TU_SAMP2_PS_MODE_OVG                     = 3,   // 11: OVG mode
} TU_SAMP2_PS_MODE;
typedef enum
{
    TU_SAMP2_PS_AA_EN_DISABLE                = 0, 
    TU_SAMP2_PS_AA_EN_ENABLE                 = 1, 
} TU_SAMP2_PS_AA_EN;
typedef enum
{
    TU_SAMP2_PS_LOD_BRILINEAR_THRESHOLD_BRI_0= 0,   // 00: Brilinear Threshold 0
    TU_SAMP2_PS_LOD_BRILINEAR_THRESHOLD_BRI_PT0625= 1,
    // 01: Threshold 0.125
    TU_SAMP2_PS_LOD_BRILINEAR_THRESHOLD_BRI_PT125= 2,
    // 10:Threshold 0.25
    TU_SAMP2_PS_LOD_BRILINEAR_THRESHOLD_BRI_PT1875= 3,
    // 11: Threshold 0.375
} TU_SAMP2_PS_LOD_BRILINEAR_THRESHOLD;
typedef enum
{
    TU_SAMP2_PS_DEPTH_MODE_NORMAL            = 0,   // 000: Normal
    TU_SAMP2_PS_DEPTH_MODE_AMODE             = 1,   // 001: AMODE: tex format set as R_
    TU_SAMP2_PS_DEPTH_MODE_IMODE             = 2,   // 010: IMODE: tex format set as R_
    TU_SAMP2_PS_DEPTH_MODE_LMODE             = 3,   // 011: LMODE: tex format set as R_
    TU_SAMP2_PS_DEPTH_MODE_LAMODE            = 4,   // 100: LAMODE: tex format set as RG_
    TU_SAMP2_PS_DEPTH_MODE_RAMODE            = 5,   // 101: RMODE: tex format set as RG_
} TU_SAMP2_PS_DEPTH_MODE;
typedef enum
{
    TU_SAMP2_PS_UNNORMALIZED_COORD_NORMALIZED= 0,   // 00: coordinates is normalized
    TU_SAMP2_PS_UNNORMALIZED_COORD_UNNORMALIZED= 1, // 01: coordinates is unnormalized
} TU_SAMP2_PS_UNNORMALIZED_COORD;
typedef enum
{
    TU_SAMP3_PS_MAG_FILTER_POINT             = 0,   // 00: Point sample
    TU_SAMP3_PS_MAG_FILTER_LINEAR            = 1,   // 01: Bilinear
    TU_SAMP3_PS_MAG_FILTER_ANISOPOINT        = 2,   // 10: Anisotropic point
    TU_SAMP3_PS_MAG_FILTER_ANISO             = 3,   // 11: Anisotropic
} TU_SAMP3_PS_MAG_FILTER;
typedef enum
{
    TU_SAMP3_PS_MIP_FILTER_NONE              = 0,   // 00: No Mip Map Filter
    TU_SAMP3_PS_MIP_FILTER_POINT             = 1,   // 01: Mip Map Filter is point sampled
    TU_SAMP3_PS_MIP_FILTER_LINEAR            = 2,   // 10: Mip Map Filter is two-level linearly sampled
    TU_SAMP3_PS_MIP_FILTER_RESERVED          = 3,   // 11: Reserved
} TU_SAMP3_PS_MIP_FILTER;
typedef enum
{
    TU_SAMP3_PS_ADDR_U_WRAP                  = 0,   // 000: WRAP. Tiles the texture at every integer junction. No
    // mirroring is performed.
    TU_SAMP3_PS_ADDR_U_MIRROR                = 1,   // 001: MIRROR. Similar to WRAP, except that the texture is
    // flipped at every integer junction. 
    TU_SAMP3_PS_ADDR_U_CLAMP                 = 2,   // 010: CLAMP. Texture coordinates outside the range [0.0,
    // 1.0] are set to the texture color at 0.0 or 1.0,
    // respectively.
    TU_SAMP3_PS_ADDR_U_BORDER                = 3,   // 011: BORDER.
    TU_SAMP3_PS_ADDR_U_MIRROR_ONCE           = 4,   // 100: MIRROR ONCE.
    TU_SAMP3_PS_ADDR_U_HALF_BORDER           = 5,   // 101: half BORDER.
    TU_SAMP3_PS_ADDR_U_CUBE_WRAP             = 6,   // 110: Seamless cube enable
} TU_SAMP3_PS_ADDR_U;
typedef enum
{
    TU_CONTROL_SECTOR_CACHE_MODE_256B        = 0,   // 00: 256b(default)
    TU_CONTROL_SECTOR_CACHE_MODE_512B        = 1,   // 01: 512b
    TU_CONTROL_SECTOR_CACHE_MODE_1K          = 2,   // 10: 1k
    TU_CONTROL_SECTOR_CACHE_MODE_2K          = 3,   // 11: 2k
} TU_CONTROL_SECTOR_CACHE_MODE;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////            TU Block (Block ID = 6) Register Definitions                              ///////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef union
{
    struct
    {
        unsigned int Resource_Address          : 24;  // Address. Virtual or physical address. 16GB,   16KB
        // aligned. 
        unsigned int Resource_Format           : 7;   // Texture Format.
        unsigned int Valid                     : 1;   // register is valid.
    } reg;
    unsigned int uint;
} Reg_Tu_Samp0_Vs;

typedef union
{
    struct
    {
        unsigned int Res_Width                 : 13;  // Width. 1D and above. 0 means null texture. Up to 4k  
        unsigned int Res_Height                : 13;  // Height. 2D and above. 0 means null texture. Up to 4k
        unsigned int Msaa_Mode                 : 1;   // MSAA Mode
        unsigned int Mipmap                    : 1;   // MipMap More than One
        unsigned int Linear_Texture_Flag       : 1;   // Linear Texture Flag.
        unsigned int Resource_Type             : 3;   // Texture Construction Type
    } reg;
    unsigned int uint;
} Reg_Tu_Samp1_Vs;

typedef union
{
    struct
    {
        unsigned int Sample_C_Fail_Value       : 8;   // Used for OGL shadow map. In the format as UNORM8 in range of
        // [0, 1].  
        unsigned int Compare_En                : 1;   // Comparison Function Enable
        unsigned int Cmp_Func                  : 3;   // Comparision Function to use if any comparison flag is
        // specified in filter. Compare function for shadow map. 
        unsigned int Dx_Mode                   : 2;   // DirectX mode 
        unsigned int Aa_En                     : 1;   // only used in PS.
        unsigned int Is_4x4_Tiling             : 1;   // if 32bpt 4x4 tiling is enable
        unsigned int Pitch                     : 10;  // StrideInByte for structure buffer.
        unsigned int Lod_Brilinear_Threshold   : 2;   // Brilinear threshold
        unsigned int Depth_Mode                : 3;   // Depth Mode. Used for OGL depth texture.
        unsigned int Unnormalized_Coord        : 1;   // coordinates is unnormalized or not.
    } reg;
    unsigned int uint;
} Reg_Tu_Samp2_Vs;

typedef union
{
    struct
    {
        unsigned int Mip_Bias                  : 8;   // MIP BIAS. assume s4.3
        unsigned int Min_Lod                   : 4;   // Combined with MIN_LOD_FRC as 4 bits integer part
        unsigned int Max_Lod                   : 4;   // Combined with MAX_LOD_FRC as 4 bits integer part
        unsigned int Mag_Filter                : 2;   // Texture Magnification Filter
        unsigned int Min_Filter                : 2;   // Texture Minification Filter
        unsigned int Mip_Filter                : 2;   // Sampler Mip Map Filter
        unsigned int Addr_U                    : 3;   // Sampler Texture Address Mode AddressU
        unsigned int Addr_V                    : 3;   // Sampler Texture Address Mode AddressV
        unsigned int Addr_R                    : 3;   // Sampler Texture Address Mode AddressR
        unsigned int Bypasstag_En              : 1; 
    } reg;
    unsigned int uint;
} Reg_Tu_Samp3_Vs;

typedef struct _Group_T_Sampler_Vs
{
    Reg_Tu_Samp0_Vs                  reg_Tu_Samp0_Vs;
    Reg_Tu_Samp1_Vs                  reg_Tu_Samp1_Vs;
    Reg_Tu_Samp2_Vs                  reg_Tu_Samp2_Vs;
    Reg_Tu_Samp3_Vs                  reg_Tu_Samp3_Vs;
} Reg_T_Sampler_Vs_Group;

typedef union
{
    struct
    {
        unsigned int Resource_Address          : 24; 
        unsigned int Resource_Format           : 7;   // Texture Format.
        unsigned int Valid                     : 1;   // register is valid.
    } reg;
    unsigned int uint;
} Reg_Tu_Samp0_Ps;

typedef union
{
    struct
    {
        unsigned int Res_Width                 : 13;  // Width. 1D and above. Up to 4k  
        unsigned int Res_Height                : 13;  // Height. 2D and above. Up to 4k
        unsigned int Msaa_Mode                 : 1;   // MSAA Mode
        unsigned int Mipmap                    : 1;   // MipMap More than One
        unsigned int Linear_Texture_Flag       : 1;   // Linear Texture Flag.
        unsigned int Resource_Type             : 3;   // Texture Construction Type
    } reg;
    unsigned int uint;
} Reg_Tu_Samp1_Ps;

typedef union
{
    struct
    {
        unsigned int Sample_C_Fail_Value       : 8;   // Used for OGL shadow map. In the format as UNORM8 in range of
        // [0, 1].  
        unsigned int Compare_En                : 1;   // Comparison Function Enable
        unsigned int Cmp_Func                  : 3;   // Comparision Function to use if any comparison flag is
        // specified in filter. Compare function for shadow map. 
        unsigned int Mode                      : 2;   // TU Running mode 
        unsigned int Aa_En                     : 1;   // OVG AA used,   stencil surface for TU's coverage sample
        unsigned int Is_4x4_Tiling             : 1;   // if 32bpt 4x4 tiling is enable
        unsigned int Pitch                     : 10;  // StrideInByte for structure buffer.
        unsigned int Lod_Brilinear_Threshold   : 2;   // Brilinear threshold
        unsigned int Depth_Mode                : 3;   // Depth Mode. Used for OGL depth texture.
        unsigned int Unnormalized_Coord        : 1;   // coordinates is unnormalized or not.
    } reg;
    unsigned int uint;
} Reg_Tu_Samp2_Ps;

typedef union
{
    struct
    {
        unsigned int Mip_Bias                  : 8;   // MIP BIAS. assume s4.3 
        unsigned int Min_Lod                   : 4;   // Combined with MIN_LOD_FRC as 4 bits integer part
        unsigned int Max_Lod                   : 4;   // Combined with MAX_LOD_FRC as 4 bits integer part
        unsigned int Mag_Filter                : 2;   // Texture Magnification Filter
        unsigned int Min_Filter                : 2;   // Texture Minification Filter
        unsigned int Mip_Filter                : 2;   // Sampler Mip Map Filter
        unsigned int Addr_U                    : 3;   // Sampler Texture Address Mode AddressU
        unsigned int Addr_V                    : 3;   // Sampler Texture Address Mode AddressV
        unsigned int Addr_R                    : 3;   // Sampler Texture Address Mode AddressR
        unsigned int Bypasstag_En              : 1;   // bypass tag(lod, scaling, float->fix).
    } reg;
    unsigned int uint;
} Reg_Tu_Samp3_Ps;

typedef struct _Group_T_Sampler_Ps
{
    Reg_Tu_Samp0_Ps                  reg_Tu_Samp0_Ps;
    Reg_Tu_Samp1_Ps                  reg_Tu_Samp1_Ps;
    Reg_Tu_Samp2_Ps                  reg_Tu_Samp2_Ps;
    Reg_Tu_Samp3_Ps                  reg_Tu_Samp3_Ps;
} Reg_T_Sampler_Ps_Group;

typedef union
{
    struct
    {
        unsigned int Resource_Address          : 24; 
        unsigned int Vb_Stride_Inbyte          : 8; 
    } reg;
    unsigned int uint;
} Reg_Tu_Vb0;

typedef union
{
    struct
    {
        unsigned int Vb_Size_Inbyte            : 28; 
        unsigned int Reserved                  : 4; 
    } reg;
    unsigned int uint;
} Reg_Tu_Vb1;

typedef union
{
    struct
    {
        unsigned int Vb_Start_Offset           : 32; 
    } reg;
    unsigned int uint;
} Reg_Tu_Vb2;

typedef struct _Group_T_Vb
{
    Reg_Tu_Vb0                       reg_Tu_Vb0;
    Reg_Tu_Vb1                       reg_Tu_Vb1;
    Reg_Tu_Vb2                       reg_Tu_Vb2;
} Reg_T_Vb_Group;

typedef union
{
    struct
    {
        unsigned int Resource_Format           : 7;   // Texture Format.
        unsigned int Valid                     : 1;   // register is valid.
        unsigned int Element_Offset            : 8; 
        unsigned int Vb_Id                     : 4; 
        unsigned int Instance_En               : 1; 
        unsigned int Reserved1                 : 11;  // Reserved
    } reg;
    unsigned int uint;
} Reg_Tu_Ve;

typedef struct _Group_T_Ve
{
    Reg_Tu_Ve                        reg_Tu_Ve;
} Reg_T_Ve_Group;

typedef union
{
    struct
    {
        unsigned int Element_Num               : 6;   // element number does not include the vid
        unsigned int Vid_En                    : 1;   // vid is always inserted in the 1st place
        unsigned int Iid_En                    : 1;   // iid is always inserted in the 2st place
        unsigned int Reserved                  : 22; 
        unsigned int Dx_Mode                   : 2; 
    } reg;
    unsigned int uint;
} Reg_Tu_Si_Ctl;

typedef union
{
    struct
    {
        unsigned int Sector_Cache_Mode         : 2;   // Sector cache sector size
        unsigned int Af_Odd_Disable            : 2;   // disable AF odd sample(3x)
        unsigned int Vb_Sector_Cache_Mode      : 2;   // VB sector cache size
        unsigned int Reserved                  : 26;  // Reserved
    } reg;
    unsigned int uint;
} Reg_Tu_Control;

typedef union
{
    struct
    {
        unsigned int Compensate                : 8;   // 8bit unsigned of current frame's Y's average value
        unsigned int Reserved                  : 24; 
    } reg;
    unsigned int uint;
} Reg_Tu_Sadcomp_Cur;

typedef union
{
    struct
    {
        unsigned int Compensate                : 8;   // 8bit unsigned,  other frame's Y's average value
        unsigned int Reserved                  : 24; 
    } reg;
    unsigned int uint;
} Reg_Tu_Sadcomp_Other;

typedef union
{
    struct
    {
        unsigned int Step_Rate                 : 26;  // StepRate is how many instances to draw before stepping one 
        // unit forward in a vertex buffer containing instance Data.
        //            
        unsigned int Reserved                  : 6; 
    } reg;
    unsigned int uint;
} Reg_Tu_Instance_Ctl;

typedef struct _Group_T_Instance
{
    Reg_Tu_Instance_Ctl              reg_Tu_Instance_Ctl;
} Reg_T_Instance_Group;

typedef union
{
    struct
    {
        unsigned int Bc_Address                : 24;  // border color surface address, when SAD it is used as ref
        // surface address          
        //            
        unsigned int Min_Lod_Frc               : 3;   // Combined with tssharp3 MIN_LOD  as 3 bits fraction
        unsigned int Max_Lod_Frc               : 3;   // Combined with tssharp3 MAX_LOD  as 3 bits fraction
        unsigned int Reserved                  : 2;   // when af enable, used as af max ratio msb 2 bits
    } reg;
    unsigned int uint;
} Reg_Tu_Bc_Vs;

typedef struct _Group_T_Bc_Vs
{
    Reg_Tu_Bc_Vs                     reg_Tu_Bc_Vs;
} Reg_T_Bc_Vs_Group;

typedef union
{
    struct
    {
        unsigned int Bc_Address                : 24;  // border color surface address, when SAD it is used as ref
        // surface address          
        unsigned int Min_Lod_Frc               : 3;   // Combined with tssharp3 MIN_LOD  as 3 bits fraction
        unsigned int Max_Lod_Frc               : 3;   // Combined with tssharp3 MAX_LOD as 3 bits frcation
        unsigned int Reserved                  : 2;   // when af enable, used as af max ratio msb 2 bits
    } reg;
    unsigned int uint;
} Reg_Tu_Bc_Ps;

typedef struct _Group_T_Bc_Ps
{
    Reg_Tu_Bc_Ps                     reg_Tu_Bc_Ps;
} Reg_T_Bc_Ps_Group;

typedef struct _Tu_regs
{
    Reg_T_Sampler_Vs_Group           reg_T_Sampler_Vs[16];
    Reg_T_Sampler_Ps_Group           reg_T_Sampler_Ps[16];
    Reg_T_Vb_Group                   reg_T_Vb[16];
    Reg_T_Ve_Group                   reg_T_Ve[16];
    Reg_Tu_Si_Ctl                    reg_Tu_Si_Ctl;
    Reg_Tu_Control                   reg_Tu_Control;
    Reg_Tu_Sadcomp_Cur               reg_Tu_Sadcomp_Cur;
    Reg_Tu_Sadcomp_Other             reg_Tu_Sadcomp_Other;
    Reg_T_Instance_Group             reg_T_Instance[16];
    Reg_T_Bc_Vs_Group                reg_T_Bc_Vs[16];
    Reg_T_Bc_Ps_Group                reg_T_Bc_Ps[16];
} Tu_regs;

#endif
