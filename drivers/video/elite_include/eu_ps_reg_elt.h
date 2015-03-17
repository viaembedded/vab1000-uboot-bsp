//    Spec Last Modified Time: 2011/2/24 16:52:14
#ifndef _EU_PS_REG_ELT_H
#define _EU_PS_REG_ELT_H


#ifndef        EU_PS_BLOCKBASE_INF
#define    EU_PS_BLOCKBASE_INF
#define    BLOCK_EU_PS_VERSION 1
#define    BLOCK_EU_PS_TIMESTAMP "2011/2/24 16:52:14"
#define    EU_PS_BLOCK                                                0x5 // match with BlockID.h
#define    EU_PS_REG_START                                            0x0 // match with BlockID.h
#define    EU_PS_REG_END                                              0x85 // match with BlockID.h
#define    EU_PS_REG_LIMIT                                            0x85 // match with BlockID.h
#endif

// Register offset definition
#define        Reg_Ps_Cfg_Offset                                          0x0
#define        Reg_Ps_Ctrl_Offset                                         0x1
#define        Reg_Ps_Start_Pc_Offset                                     0x2
#define        Reg_Ps_Mem_Range_Offset                                    0x3
#define        Reg_Ps_Out_Fmt_Offset                                      0x4
#define        Reg_Ps_Const_Offset                                        0x5
#define        Reg_Ps_Const_HWAddr                                        0x80


// Block constant definition
typedef enum
{
    PS_CFG_PS_ON_OFF                         = 0,   // 0: Pixel Shader Off
    PS_CFG_PS_ON_ON                          = 1,   // 1: Pixel Shader On
} PS_CFG_PS_ON;
typedef enum
{
    PS_CFG_AND_V_MASK_DISABLED               = 0,   // 0: Disable
    PS_CFG_AND_V_MASK_ENABLED                = 1,   // 1: Enable, HW ands the sample predicate with the P7 quad
    // based mask
} PS_CFG_AND_V_MASK;
typedef enum
{
    PS_CFG_CB_MODE_DOUBLE_CONTEXT            = 0,   // 0: Double context mode, the driver programs this mode when
    // the total PS constant size is less than or equal to 16.
    PS_CFG_CB_MODE_SINGLE_CONTEXT            = 1,   // 1: Single context mode, the driver programs this mode when
    // the total PS constant size is greater than 16.
} PS_CFG_CB_MODE;
typedef enum
{
    PS_CFG_PP_MODE_FP_MODE                   = 0,   // 0: PS runs in FP mode.
    PS_CFG_PP_MODE_PP_MODE                   = 1,   // 1: PS runs in PP mode.
} PS_CFG_PP_MODE;
typedef enum
{
    PS_CTRL_DUAL_SRC_EN_DUAL_SRC_OFF         = 0,   // 0: dual source blending is off in the WBU
    PS_CTRL_DUAL_SRC_EN_DUAL_SRC_ON          = 1,   // 1: dual source blending is on in the WBU
} PS_CTRL_DUAL_SRC_EN;
typedef enum
{
    PS_CTRL_RD_MODE_NEAR                     = 0,   // 00: Round toward nearest even
    PS_CTRL_RD_MODE_ZERO                     = 1,   // 01: Round to Zero
    PS_CTRL_RD_MODE_POS                      = 2,   // 10: Round to Positive Infinity
    PS_CTRL_RD_MODE_NEG                      = 3,   // 11: Round to Negative infinity
} PS_CTRL_RD_MODE;
typedef enum
{
    PS_CTRL_FP_MODE_STD_IEEE                 = 0,   // 00: Standard IEEE-745
    PS_CTRL_FP_MODE_NSTD_IEEE                = 1,   // 01: Non-Standard IEEE-745
    PS_CTRL_FP_MODE_MS_ALT                   = 2,   // 10: Microsoft Alt float point
    PS_CTRL_FP_MODE_RESERVED                 = 3,   // 11: Reserved
} PS_CTRL_FP_MODE;
typedef enum
{
    PS_CTRL_CLR_OUT_MODE_4P                  = 0,   // 00: 4P, data in low 128-bit
    PS_CTRL_CLR_OUT_MODE_8P                  = 1,   // 01: 8P
} PS_CTRL_CLR_OUT_MODE;
typedef enum
{
    PS_OUT_FMT_O0_DATA_FMT_FP32_2C           = 0,   // 000: Floating-point 32-bit 2C only
    PS_OUT_FMT_O0_DATA_FMT_FP16              = 1,   // 001: Floating-point 16-bit
    PS_OUT_FMT_O0_DATA_FMT_INT32             = 2,   // 010: Integer 32-bit
    PS_OUT_FMT_O0_DATA_FMT_INT16             = 3,   // 011: Integer 16-bit
    PS_OUT_FMT_O0_DATA_FMT_UINT32            = 4,   // 100: Unsigned integer 32-bit
    PS_OUT_FMT_O0_DATA_FMT_UINT16            = 5,   // 101: Unsigned integer 16-bit
    PS_OUT_FMT_O0_DATA_FMT_UNORM16           = 6,   // 110: Unorm 16-bit
    PS_OUT_FMT_O0_DATA_FMT_UNORM10           = 7,   // 111: Unorm 10-bit
    PS_OUT_FMT_O0_DATA_FMT_UNORM8            = 8,   // 1000: Unorm 8-bit
    PS_OUT_FMT_O0_DATA_FMT_INT8              = 9,   // 1001: Integer 8-bit
    PS_OUT_FMT_O0_DATA_FMT_UINT8             = 10,  // 1010: Unsigned integer 8-bit
    PS_OUT_FMT_O0_DATA_FMT_UNORM24           = 11,  // 1011: Unorm 24-bit
    PS_OUT_FMT_O0_DATA_FMT_FP32_4C           = 12,  // 1100: Floating-point 32-bit 4C
} PS_OUT_FMT_O0_DATA_FMT;
typedef enum
{
    PS_OUT_FMT_Z_DATA_FMT_UNORM16            = 6,   // 0110: Unorm 16-bit
    PS_OUT_FMT_Z_DATA_FMT_UNORM24            = 11,  // 1011: Unorm 24-bit
} PS_OUT_FMT_Z_DATA_FMT;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////            EU_PS Block (Block ID = 5) Register Definitions                           ///////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef union
{
    struct
    {
        unsigned int Ps_On                     : 1;   // Pixel Shader On
        unsigned int And_V_Mask                : 1;   // And sample predicate with P7 quad based mask in PS
        unsigned int Cb_Mode                   : 1;   // PS Constant Buffer update mode
        unsigned int Pp_Mode                   : 1;   // PS precision mode PP/FP
        unsigned int Max_Threads               : 4;   // Maximum number of total PS threads kickoff. Value 0~15 for
        // 1~16 threads.
        unsigned int Reserved1                 : 24;  // Reserved
    } reg;
    unsigned int uint;
} Reg_Ps_Cfg;

typedef union
{
    struct
    {
        unsigned int Thread_Size               : 2;   // Thread packing size. Value is 0/1/2 for 1/2/4 segments,
        // each segment contains 32P
        unsigned int Crf_Size                  : 6;   // Total Common Register File (CRF) space. Value is 1~32
        // 4-component elements, including temp, input, output
        // registers.
        unsigned int Clr_In_Size               : 4;   // PS Varying Attribute Input Size. Value is 1~8 4-component
        // elements.
        //  
        unsigned int Dual_Src_En               : 1;   // Dual source blending enable. 
        unsigned int Clr_Out_En                : 1;   // Color output to WBU enable. 
        unsigned int Rd_Mode                   : 2;   // EU floating point rounding mode
        unsigned int Fp_Mode                   : 2;   // EU floating point operation mode
        unsigned int Z_Out_En                  : 1;   // oDepth output to WBU enable.
        unsigned int Clr_Out_Size              : 3;   // Color output (to WBU) size. 
        unsigned int Clr_Out_Mode              : 1;   // Color output (to WBU) mode.
        unsigned int Reserved                  : 9;   // Reserved
    } reg;
    unsigned int uint;
} Reg_Ps_Ctrl;

typedef union
{
    struct
    {
        unsigned int Address                   : 32;  // Pixel Shader Instruction Start Address
    } reg;
    unsigned int uint;
} Reg_Ps_Start_Pc;

typedef union
{
    struct
    {
        unsigned int Th_Mem_Range              : 14;  // Range of thread external storage used by OCL Local Memory
        // or CS x#s in memory. Note the compiler used external thread
        // storage i.e. call stacks, etc. is not included, it should
        // be placed in the upper memory.
        unsigned int Reserved                  : 18;  // Reserved
    } reg;
    unsigned int uint;
} Reg_Ps_Mem_Range;

typedef union
{
    struct
    {
        unsigned int O0_Data_Fmt               : 4;   // EU to WBU PS output format for O0
        unsigned int O1_Data_Fmt               : 4;   // EU to WBU PS output format for O1
        unsigned int O2_Data_Fmt               : 4;   // EU to WBU PS output format for O2
        unsigned int O3_Data_Fmt               : 4;   // EU to WBU PS output format for O3
        unsigned int Z_Data_Fmt                : 4;   // EU to WBU PS output format for Z
        unsigned int Reserved                  : 12;  // Reserved
    } reg;
    unsigned int uint;
} Reg_Ps_Out_Fmt;

typedef union
{
    struct
    {
        unsigned int Const                     : 32;  // PS Constant Buffer 32*128-bit
    } reg;
    unsigned int uint;
} Reg_Ps_Const;

typedef struct _Eu_Ps_regs
{
    Reg_Ps_Cfg                       reg_Ps_Cfg;
    Reg_Ps_Ctrl                      reg_Ps_Ctrl;
    Reg_Ps_Start_Pc                  reg_Ps_Start_Pc;
    Reg_Ps_Mem_Range                 reg_Ps_Mem_Range;
    Reg_Ps_Out_Fmt                   reg_Ps_Out_Fmt;
    Reg_Ps_Const                     reg_Ps_Const[128];
} Eu_Ps_regs;

#endif
